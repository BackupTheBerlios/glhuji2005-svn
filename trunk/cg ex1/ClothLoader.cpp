#include "StdAfx.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "ClothLoader.h"
#include "GravityForce.h"
#include "Particle.h"
#include "CLoadIni.h"
#include "ForwardEulerSolver.h"
#include "RungeKuttaSolver.h"
#include "MidPointSolver.h"

#include <stdlib.h>
#include <string>

//Useful macros
#define LOAD_L( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl; break;} \
                                            else store_x = atol(val.c_str());

#define LOAD_F( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl; break;} \
                                            else store_x = atof(val.c_str());

#define LOAD_I( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl; break;} \
                                            else store_x = atoi(val.c_str());


//This function loads the cloth mesh, and initialises the particle system with it
//it's very long but it gets the job done
bool 
ClothLoader::Load( ParticleSystem &inSystem, string &inFileName )
{
    CLoadIni loader;
    string   val;
    bool     ret             = false;
    int      rc              = -1;
    bool     meshAutoCreated = false;
    idx_t    meshWidth       = 0;
    idx_t    meshHeight      = 0;
    double   stepSize        = -1;

	SolverType solverType = C_FORWARD_EULER_SOLVER;
    do{

        //---------------- Load Ini File --------------------
        if( loader.ReadIni(inFileName.c_str()) != 0) {
            cout << "ERROR: Couldn't open ini file: " << inFileName << endl;
            break;
        }

        readSolver( inSystem, loader );

        if( !readGlobalConstants( inSystem, loader, meshWidth, meshHeight ) )
            break;

        if( !readMesh( inSystem, loader, meshWidth, meshHeight ))
            break;
        
        if( !readSprings( inSystem, loader ) )
            break;

        //if we got here everything is ok
        //debug
        cout << "LOADED OK!" << endl;
        ret = true;
       } while(0);

    return ret;
}

void
ClothLoader::readSolver( ParticleSystem &inSystem, CLoadIni &inLoader )
{
    int      rc              = -1;
    string   val;
    SolverType solverType    = C_FORWARD_EULER_SOLVER;

    //--------- Load in tag from file ---------
    rc = inLoader.GetField( C_SOLVER_TYPE_TAG, val );
    if ( rc != 0 )
        cout << "Warning: solver type field is missing - using forward Euler" << endl;
    else
        solverType = (SolverType)atoi( val.c_str() );

    NumericalSolver *solver = NULL;
    switch( solverType )
    {
    case C_FORWARD_EULER_SOLVER:
        cout << "using forward euler" << endl;
        solver = new ForwardEulerSolver();
        break;

        //todo: reinstate?
        //case C_REVERSE_EULER_SOLVER:
        //    cout << "using reverse euler" << endl;
        //    solver = new ReverseEulerSolver();
        //    break;

    case C_MIDPOINT_SOLVER:
        cout << "using midpoint solver" << endl;
        solver = new MidPointSolver();
        break;

    case C_RUNGE_KUTTA_SOLVER:
        cout << "using Runge Kutta Solver" << endl;
        solver = new RungeKuttaSolver();
        break;

    default:
        cout << "ERROR: unknown solver type: " << val.c_str() << " - using forward Euler" << endl;
        solver = new ForwardEulerSolver();
        break;
    }

    inSystem.setSolver( solver );
}

bool
ClothLoader::readGlobalConstants( ParticleSystem &inSystem, CLoadIni &inLoader,
                                    idx_t &outMeshWidth, idx_t &outMeshHeight )
{
    bool     ret                  = false;
    string   val;
    int8     airResistancePercent = 0;
    double   gravity              = 0;
    double   stepSize             = 0;
	double*  arr = NULL;

    do {
        //3. Load Cloth Dimensions
        LOAD_L( C_MESH_WIDTH_TAG, "ERROR: meshwidth field is missing ", outMeshWidth );

        if( outMeshWidth <= 0 )
        {
            cout << "ERROR: meshwidth must be positive (" << outMeshWidth << ")" << endl;
            break;
        }

        //load cloth height
        LOAD_L( C_MESH_HEIGHT_TAG, "ERROR: meshheight field is missing ", outMeshHeight );

        if( outMeshHeight <= 0 )
        {
            cout << "ERROR: meshheight must be positive (" << outMeshHeight << ")" << endl;
            break;
        }

        inSystem.setDimensions( outMeshWidth, outMeshHeight );

        LOAD_I( C_AIR_RESISTANCE_TAG, "ERROR: air resistance field is missing ", airResistancePercent );

        if( airResistancePercent < 0 || airResistancePercent > 100 )
        {
            cout << "ERROR: air resistance is specified in percent (0<=a<100):" << airResistancePercent << endl;
            break;
        }

        inSystem.setAirResistance( airResistancePercent );

        //4. Load Forces
        LOAD_F( C_GRAVITY_TAG, "ERROR: gravity field is missing ", gravity );
        inSystem.setGravity( gravity );

        //load step size
        LOAD_F( C_STEP_SIZE_TAG, "ERROR: step size not specified", stepSize );

        if( stepSize <= 0 )
        {
            cout << "ERROR: illegal stepsize, must be >= 0 " << endl;
            break;
        }
        inSystem.setStepSize( stepSize );

		//------------------------ Set wind parameters ---------------------------
		Vector3d         WindDirection(0.0,0.0,0.0);
		Vector3d         Wind(0.0,0.0,0.0);
		double	         WindLen = 0;
		double	         WindMinFactor = 1;
		double	         WindMaxFactor = 1;
		double	         WindMaxChange = 0;
		if( inLoader.GetField(C_WIND_TAG, val ) == 0 )
		{
			int xDim, yDim;
			inLoader.GetDblArray( val, arr, &xDim, &yDim );
			
			if( yDim != 1 || xDim != 3 )
				cout << "ERROR: dimensions of wind vector are wrong" << endl;
			else
			{
				Wind = Vector3d(arr[0], arr[1], arr[2]);
				WindDirection = Wind;
				WindDirection.normalize();
				WindLen = Wind.length();
			}
			SAFE_DELETE_ARR( arr );
		}
		if( inLoader.GetField(C_WIND_MIN_FACTOR_TAG, val ) == 0 )
		{
			WindMinFactor = atof(val.c_str());
		}
		if( inLoader.GetField(C_WIND_MAX_FACTOR_TAG, val ) == 0 )
		{
			WindMaxFactor = atof(val.c_str());
		}
		if( inLoader.GetField(C_WIND_MAX_CHANGE_TAG, val ) == 0 )
		{
			WindMaxChange = atof(val.c_str());
		}
		inSystem.setWind (WindDirection, Wind, WindMinFactor*WindLen, WindMaxFactor*WindLen, WindMaxChange);

		ret = true;
    } while( 0 );

    return ret;
}

bool
ClothLoader::readMesh( ParticleSystem &inSystem, CLoadIni &inLoader,
                     idx_t &inMeshWidth, idx_t &inMeshHeight )
{
    string val;
    bool ret             = false;
    bool meshAutoCreated = false;
    double *arr          = NULL;

    do {

    //------------------------ AutoCreate Mesh ---------------------------
    if( inLoader.GetField(C_AUTOCREATE_MESH_TAG, val ) == 0 )
    {
        int xDim, yDim;

        inLoader.GetDblArray( val, arr, &xDim, &yDim );

        //sanity
        if( yDim != 1 || xDim != C_NUM_FIELDS_IN_AUTOCREATE )
        {
            cout << "ERROR: dimensions of autocreate parameter are wrong" << endl;
            break;
        }

        //perform autocreation
        inSystem.autoCreateMesh( arr[0], arr[1], arr[2], arr[3], arr[4], arr[5] );

        SAFE_DELETE_ARR( arr );

        meshAutoCreated = true;
    }

    //------------------------ Load Individual particles -----------------
    bool noerr = true;
    for( int i = 0; noerr && i < inMeshHeight; i++ )
        for( int j = 0; noerr && j < inMeshWidth; j++ )
        {
            char tmp[256];
            int xDim;
            int yDim;
            bool particleDefined = false;

            sprintf( tmp, "(%d,%d)", j, i );

            //try to load particle, particleDefined = true if the i,j particle
            //was specified in the donfig file.
            particleDefined = (inLoader.GetField( tmp , val ) == 0); 

            if( !particleDefined && !meshAutoCreated )
            {
                cout << "ERROR: no parameters specified for particle @ " << j << "," << i  << endl;
                noerr = false;
                break;
            }

            //------------------------------------------------------------------
            // If a mesh is autocreated then individual particle positions can
            // be overrided by specifying them explicitly, however if the mesh
            // was not autocreated each particle must be specified.
            //
            // There are 2 different syntaxes for specifying a particle:
            // 1. (i,j) = x,y,z,m,p - particle pos., mass, and true/fals as to
            //                        whether the particle is pinned ornot.
            //    Note that this version has 5 fields.
            // 2. (i,j) = PIN       - pin the specified particle in place, this
            //                        option only has 1 parameter and can only
            //                        be specified when autocreating a mesh.
            //      
            //------------------------------------------------------------------
            if( particleDefined )
            {

                //parse
                inLoader.GetDblArray( val, arr, &xDim, &yDim );

                //particle should only be defined with 1 dimension in config file
                //if there is more than 1 then we have an error
                if( yDim != 1 )
                {
                    cout << "ERROR: yDim of particle @" << j << "," << i <<
                        "must be exactly 1 is (" << yDim << ")" << endl;
                    noerr = false;
                    break;
                }

                //check we don't get extra data
                if( !meshAutoCreated && xDim != C_NUM_FIELDS_IN_PARTICLE )
                {
                    cout << "ERROR: xDim of particle @" << j << "," << i <<
                        "should be" << C_NUM_FIELDS_IN_PARTICLE << "is (" << yDim << ")" << endl;
                    noerr = false;
                    break;
                }
                else if( meshAutoCreated && xDim != C_NUM_FIELDS_IN_PARTICLE && xDim != 1 )
                {
                    cout << "ERROR: autocreate xDim of particle @" << j << "," << i <<
                        "should be" << C_NUM_FIELDS_IN_PARTICLE << "or 1 is (" << yDim << ")" << endl;
                    noerr = false;
                    break;
                }
                else if( xDim == C_NUM_FIELDS_IN_PARTICLE )
                {
                    cout << "SUCCESS: particle @ " << j << "," << i << "=" <<
                        arr[0] << "," << arr[1] << "," << arr[2] << "," << arr[3]<< "," << arr[4] << endl;

                    Particle p( arr[0], arr[1], arr[2], arr[3], (arr[4]==1) );

                    inSystem.addParticleAt( j, i, p );
                }
                else if( meshAutoCreated && xDim == 1 )
                {
                    cout << "Pinning particle @ " << j << "," << i << endl;
                    inSystem.pinParticle( j,i );
                }

                SAFE_DELETE_ARR( arr );
            }

        }

        //missing particle
        if( !noerr )
            break;
        ret = true;
        } while( 0 );

        //----------- Cleanup ---------
        SAFE_DELETE_ARR( arr );

        return ret;
}

bool
ClothLoader::readSprings( ParticleSystem &inSystem, CLoadIni &inLoader )
{
    bool ret = false;
    string val;
    double k, b, shearK, shearB, flexK, flexB;

    do {
        //Read spring constants
        LOAD_F( C_SPRING_CONST_TAG, "ERROR: spring constant undefined", k );
        LOAD_F( C_SPRING_DRAG_TAG, "ERROR: spring drag undefined", b );
        LOAD_F( C_SHEAR_SPRING_CONST_TAG, "ERROR: shear spring constant undefined", shearK );
        LOAD_F( C_SHEAR_SPRING_DRAG_TAG, "ERROR: shear spring drag constant undefined", shearB );
        LOAD_F( C_FLEX_SPRING_CONST_TAG, "ERROR: Flexion spring constant undefined", flexK );
        LOAD_F( C_FLEX_SPRING_DRAG_TAG, "ERROR: Flexion spring drag constant undefined", flexB );

        // add springs to system
        inSystem.constructSprings( k, b, shearK, shearB, flexK, flexB );

        ret = true;
    } while(0);

    return ret;
}

#undef LOAD_L
#undef LOAD_F
#undef LOAD_I
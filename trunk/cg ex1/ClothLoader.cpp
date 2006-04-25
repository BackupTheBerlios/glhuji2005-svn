#include "StdAfx.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "ClothLoader.h"
#include "GravityForce.h"
#include "Particle.h"
#include "CLoadIni.h"
#include "ForwardEulerSolver.h"
#include "ReverseEulerSolver.h"

#include <stdlib.h>
#include <string>

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

        //1. Midpoint - false by default
        if( loader.GetField( C_MIDPOINT_TYPE_TAG, val ) == 0 )
		    inSystem.setIsMidPoint( atoi(val.c_str())==0?false:true );
        else
            cout << "Warning: Midpoint not specified, default: FALSE" << endl;

        //2. Load Solver - forward Euler by default
		rc = loader.GetField( C_SOLVER_TYPE_TAG, val );
		if ( rc != 0 )
		{
            cout << "Warning: solver type field is missing - using forward Euler" << endl;
		}
		else
	        solverType = (SolverType)atoi( val.c_str() );

        NumericalSolver *solver = NULL;
        switch( solverType )
        {
        case C_FORWARD_EULER_SOLVER:
            cout << "using forward euler" << endl;
            solver = new ForwardEulerSolver();
            break;

        case C_REVERSE_EULER_SOLVER:
            cout << "using reverse euler" << endl;
            solver = new ReverseEulerSolver();
            break;

        default:
            cout << "ERROR: unknown solver type: " << val.c_str() << " - using forward Euler" << endl;
            solver = new ForwardEulerSolver();
            break;
        }

        inSystem.setSolver( solver );

        //3. Load Cloth Dimensions
        if( loader.GetField( C_MESH_WIDTH_TAG, val ) != 0 )
        {
            cout << "ERROR: meshwidth field is missing " << endl;
            break;
        }
        meshWidth = atol( val.c_str() );
        if( meshWidth <= 0 )
        {
            cout << "ERROR: meshwidth must be positive (" << meshWidth << ")" << endl;
            break;
        }

        //load cloth height
        if( loader.GetField( C_MESH_HEIGHT_TAG, val ) != 0 )
        {
            cout << "ERROR: meshheight field is missing " << endl;
            break;
        }
        meshHeight = atol( val.c_str() );
        if( meshHeight <= 0 )
        {
            cout << "ERROR: meshheight must be positive (" << meshHeight << ")" << endl;
            break;
        }

        inSystem.setDimensions( meshWidth, meshHeight );

        //4. Load Forces
        if( loader.GetField( C_GRAVITY_TAG, val ) != 0 )
        {
            cout << "ERROR: gravity field is missing " << endl;
            break;
        }
        double gravity = atof( val.c_str() );
        inSystem.addForce( new GravityForce( gravity ) );

        //------------------------ AutoCreate Mesh ---------------------------
        if( loader.GetField(C_AUTOCREATE_MESH_TAG, val ) == 0 )
        {
            int xDim, yDim;
            double *arr = 0;

            loader.GetDblArray( val, arr, &xDim, &yDim );

            //sanity
            if( yDim != 1 || xDim != C_NUM_FIELDS_IN_AUTOCREATE )
            {
                cout << "ERROR: dimensions of autocreate parameter are wrong" << endl;

                if( arr != NULL )
                    delete [] arr;

                break;
            }

            //perform autocreation
            inSystem.autoCreateMesh( arr[0], arr[1], arr[2], arr[3], 
                                                        arr[4], arr[5] );

            if( arr != NULL )
                delete [] arr;

            meshAutoCreated = true;
        }

        //------------------------ Load Individual particles -----------------
        bool noerr = true;
        for( int i = 0; noerr && i < meshHeight; i++ )
            for( int j = 0; noerr && j < meshWidth; j++ )
        {
            char tmp[256];
            sprintf( tmp, "(%d,%d)", j, i );

            double *arr = 0;
            int xDim;
            int yDim;
            rc = loader.GetField( tmp , val ); 
            if( rc != 0 && !meshAutoCreated )
            {
                cout << "ERROR: no parameters specified for particle @ " << j << "," << i  << endl;
                noerr = false;
                break;
            }

            if( rc == 0 )
            {

                //parse
                loader.GetDblArray( val, arr, &xDim, &yDim );

                //particle only has 1 dimension thank you!
                if( yDim != 1 )
                {
                    if( arr != 0 ) delete[] arr; //deallocate memory...

                    cout << "ERROR: yDim of particle @" << j << "," << i <<
                        "must be exactly 1 is (" << yDim << ")" << endl;
                    noerr = false;
                    break;
                }

                //check we don't get extra data
                if( !meshAutoCreated && xDim != C_NUM_FIELDS_IN_PARTICLE )
                {
                    if( arr != 0 ) delete[] arr; //deallocate memory...

                    cout << "ERROR: xDim of particle @" << j << "," << i <<
                        "should be" << C_NUM_FIELDS_IN_PARTICLE << "is (" << yDim << ")" << endl;
                    noerr = false;
                    break;
                }
                else if( meshAutoCreated && xDim != C_NUM_FIELDS_IN_PARTICLE && xDim != 1 )
                {
                    if( arr != 0 ) delete[] arr; //deallocate memory...

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
                    //debug
                    cout << "SUCCESS: pinning particle @ " << j << "," << i << endl;
                    inSystem.pinParticle( j,i );
                }

                delete [] arr;
            }

        }

        //missing particle
        if( !noerr )
            break;

        //------------- Read StepSize ------------------
        if( loader.GetField( C_STEP_SIZE_TAG, val ) != 0 )
        {
            cout << "ERROR: step size not specified: " << C_STEP_SIZE_TAG << endl;
            break;
        }
        
        stepSize = atof( val.c_str() );

        if( stepSize <= 0 )
        {
            cout << "ERROR: illegal stepsize, must be >= 0 " << endl;
            break;
        }

        inSystem.setStepSize( stepSize );
        
        //------------- Setup Springs ------------------
        double k, b, shearK, shearB, flexK, flexB;
        if( loader.GetField( C_SPRING_CONST_TAG , val ) != 0 )
        {
            cout << "ERROR: spring constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        k = atof( val.c_str() );

        if( loader.GetField( C_SPRING_DRAG_TAG , val ) != 0 )
        {
            cout << "ERROR: spring drag constant undefined " << C_SPRING_DRAG_TAG << endl;
            break;
        }
        b = atof( val.c_str() );

        if( loader.GetField( C_SHEAR_SPRING_CONST_TAG , val ) != 0 )
        {
            cout << "ERROR: shear spring constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        shearK = atof( val.c_str() );

        if( loader.GetField( C_SHEAR_SPRING_DRAG_TAG , val ) != 0 )
        {
            cout << "ERROR: shear spring drag constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        shearB= atof( val.c_str() );

        if( loader.GetField( C_FLEX_SPRING_CONST_TAG , val ) != 0 )
        {
            cout << "ERROR: Flexion spring constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        flexK = atof( val.c_str() );
        if( loader.GetField( C_FLEX_SPRING_DRAG_TAG , val ) != 0 )
        {
            cout << "ERROR: Flexion spring drag constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        flexB = atof( val.c_str() );

        // add springs to system
        inSystem.constructSprings( k, b, shearK, shearB, flexK, flexB );


        //if we got here everything is ok
        //debug
        cout << "LOADED OK!" << endl;
        ret = true;
       } while(0);

    return ret;
}
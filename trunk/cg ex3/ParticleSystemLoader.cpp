#include "StdAfx.h"
#include "SimulationsParams.h"
#include "ParticleSystem.h"
#include "NewtonianParticleSystem.h"
#include "ParticleSystemLoader.h"
#include "CLoadIni.h"
#include "constants.h"

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


//This function loads the ParticleSystem mesh, and initialises the particle system with it
//it's very long but it gets the job done
bool 
CParticleSystemLoader::Load( CSimulationsParams &inParams, const char* inFileName )
{
    CLoadIni loader;
    string   val;
    bool     ret             = false;
    int      rc              = -1;

    do{

        //---------------- Load Ini File --------------------
        if( loader.ReadIni(inFileName) != 0) {
            cout << "ERROR: Couldn't open ini file: " << inFileName << endl;
            break;
        }

        if( !readGlobalConstants( inParams, loader ))
            break;

		if( !createParticleSystem( inParams, loader ))
			break;

        if( !readParticleDefaults( inParams, loader ))
            break;

        //if we got here everything is ok
        //debug
        cout << "LOADED OK!" << endl;
        ret = true;
       } while(0);

    return ret;
}

bool
CParticleSystemLoader::createParticleSystem( CSimulationsParams &inParams, CLoadIni &inLoader )
{
    int		rc              = -1;
	bool	ret				= false;
    string   val;
    ParticleSystemType systemType    = C_NEWTONIAN_SYSTEM;

    //--------- Load in tag from file ---------
    rc = inLoader.GetField( C_PARTICLE_SYSTEM_TYPE_TAG, val );
    if ( rc != 0 )
        cout << "Warning: system type field is missing - using newtonian system" << endl;
    else
        systemType = (ParticleSystemType)atoi( val.c_str() );

	if (inParams.m_particleSystem != NULL){
		delete inParams.m_particleSystem;
	    inParams.m_particleSystem = NULL;
	}

    switch( systemType )
    {
    case C_NEWTONIAN_SYSTEM:
        cout << "using newtonian system" << endl;
        inParams.m_particleSystem = new CNewtonianParticleSystem();
		ret = readNewtonianParticleSystem( inParams, inLoader );

    default:
        cout << "ERROR: unknown system type: " << val.c_str() << " - using newtonian system" << endl;
        inParams.m_particleSystem = new CNewtonianParticleSystem();
		ret = readNewtonianParticleSystem( inParams, inLoader );
    }

	return ret;
}

bool
CParticleSystemLoader::readGlobalConstants( CSimulationsParams &inParams, CLoadIni &inLoader )
{
    bool     ret                  = false;
    string   val;
	double*  arr = NULL;
	Point3d color;

    do {
        //TODO: read camera stuff
		//TODO: read time-step
		// clear-color
		if (inLoader.GetField( C_CLEAR_COLOR_TAG, val ) == 0 ){
			if (inLoader.GetPoint3d(val, color) == 0){
				inParams.m_clearColor = color;
			}
			else{
				cerr << "Failed loading clear-color value: " << val << endl;
				break;
			}
		}
		/*
        LOAD_L( C_MESH_WIDTH_TAG, "ERROR: meshwidth field is missing ", outMeshWidth );

        if( outMeshWidth <= 0 )
        {
            cout << "ERROR: meshwidth must be positive (" << outMeshWidth << ")" << endl;
            break;
        }

        //load ParticleSystem height
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
		Vector3d         WindDirection;
		Vector3d         Wind;
		double	         WindLen = 0;
		double	         WindMinFactor = 1;
		double	         WindMaxFactor = 1;
		double	         WindMaxChange = 0;
		if( inLoader.GetField( C_WIND_TAG, val ) == 0 )
		{
			int xDim, yDim;
			inLoader.GetDblArray( val, arr, &xDim, &yDim );
			
			if( yDim != 1 || xDim != 3 )
            {
				cout << "ERROR: dimensions of wind vector are wrong" << endl;
                break;
            }
			else
			{
				Wind = Vector3d(arr[0], arr[1], arr[2]);
				WindDirection = Wind;
				WindDirection.normalize();
				WindLen = Wind.length();
			}
			SAFE_DELETE_ARR( arr );
		}
		if( inLoader.GetField( C_WIND_MIN_FACTOR_TAG, val ) == 0 )
		{
			WindMinFactor = atof(val.c_str());
		}
		if( inLoader.GetField( C_WIND_MAX_FACTOR_TAG, val ) == 0 )
		{
			WindMaxFactor = atof(val.c_str());
		}
		if( inLoader.GetField( C_WIND_MAX_CHANGE_TAG, val ) == 0 )
		{
			WindMaxChange = atof(val.c_str());
		}
		inSystem.setWind (WindDirection, Wind, WindMinFactor*WindLen, WindMaxFactor*WindLen, WindMaxChange);
		*/

		ret = true;
    } while( 0 );

    return ret;
}

bool
CParticleSystemLoader::readParticleDefaults( CSimulationsParams &inParams, CLoadIni &inLoader )
{
    bool ret = false;
	string val;
	int iVal;
	double dVal;
	Point3d size, color;

    do {

	if (inParams.m_particleSystem == NULL){
		cout << "ERROR: Particle System was not ceated" << endl;
        break;
	}

	//TODO: should not be a must
	LOAD_F( C_DEFAULT_LIFESPAN_TAG, "ERROR: defaultLifespan field is missing ", inParams.m_particleSystem->m_dDefaultLifespan );
	LOAD_F( C_DEFAULT_MASS_TAG, "ERROR: defaultMass field is missing ", inParams.m_particleSystem->m_dDefaultMass );
	LOAD_F( C_DEFAULT_PERSISTANCE_TAG, "ERROR: defaultPersistance field is missing ", inParams.m_particleSystem->m_dDefaultPersistance);
	LOAD_F( C_DEFAULT_SPAN_TAG, "ERROR: defaultSpan field is missing ", inParams.m_particleSystem->m_dDefaultSpan);
	LOAD_F( C_DEFAULT_RADIUS_TAG, "ERROR: defaultRadius field is missing ", inParams.m_particleSystem->m_dDefaultRadius );

	// particle shape
	if (inLoader.GetField(C_PARTICLE_SHAPE_TAG, val) == 0){
		iVal = atoi(val.c_str());
		if (iVal <= 0 || iVal >= C_PARTICLESHAPE_NUM){
			cout << "Illegal ParticleShape value" << val << endl;
			break;
		}
		inParams.m_particleSystem->m_particleShape = (ParticleShapeType)iVal;
	}
	// particle size
	if (inLoader.GetField( C_PARTICLE_SIZE_TAG, val ) == 0 ){
		if (inLoader.GetPoint3d(val, size) == 0){
			inParams.m_particleSystem->m_pParticleSize = size;
		}
		else{
			cerr << "Failed loading size value: " << val << endl;
			break;
		}
	}
	// particle color
	if (inLoader.GetField( C_PARTICLE_COLOR_TAG, val ) == 0 ){
		if (inLoader.GetPoint3d(val, color) == 0){
			inParams.m_particleSystem->m_pParticleColor = color;
		}
		else{
			cerr << "Failed loading color value: " << val << endl;
			break;
		}
	}
	// alpha
	if (inLoader.GetField(C_PARTICLE_ALPHA_TAG, val) == 0){
		inParams.m_particleSystem->m_dParticleAlpha = atof(val.c_str());
	}

	ret = true;

	}while(0);
    return ret;
}

bool
CParticleSystemLoader::readNewtonianParticleSystem( CSimulationsParams &inParams, CLoadIni &inLoader )
{
    bool ret = false;
    string val;
	Point3d gravity;
	Point3d origin;

    do {

		if (inParams.m_particleSystem == NULL){
			cout << "ERROR: Particle System was not ceated" << endl;
			break;
		}

		//TODO: all this should be in the right class, or at least verify casting
		CNewtonianParticleSystem* system = (CNewtonianParticleSystem*) inParams.m_particleSystem;

        //Read spring constants
		LOAD_I( C_PARTICLES_PER_FRAME_TAG, "ERROR: ParticlesPerFrame undefined", system->m_nParticlesPerFrame );
		LOAD_F( C_NEWTONIAN_PS_HEADING_STEP_TAG, "ERROR: HeadingStep undefined",  system->m_dHeadingStep);
		// load gravity
		if ((inLoader.GetField( C_NEWTONIAN_PS_GRAVITY_TAG, val ) == 0 ) &&
			inLoader.GetPoint3d(val, gravity) == 0){
			system->m_Gravity = gravity;
		}
		else{
			cerr << "Failed loading gravity value: " << val << endl;
			break;
		}
		// load origin
		if ((inLoader.GetField( C_NEWTONIAN_PS_ORIGIN_TAG, val ) == 0 ) &&
			inLoader.GetPoint3d(val, origin) == 0){
			system->m_Origin = origin;
		}
		else{
			cerr << "Failed loading origin value: " << val << endl;
			break;
		}


        ret = true;
    } while(0);

    return ret;
}

#undef LOAD_L
#undef LOAD_F
#undef LOAD_I
#include "StdAfx.h"
#include "SimulationsParams.h"
#include "ParticleSystem.h"
#include "NewtonianParticleSystem.h"
#include "FireworksParticleSystem.h"
#include "FlockParticleSystem.h"
#include "ParticleSystemLoader.h"
#include "CLoadIni.h"
#include "constants.h"

#include <stdlib.h>
#include <string>

//Useful macros
#define LOAD_L( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl;} \
                                            else store_x = atol(val.c_str());

#define LOAD_F( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl;} \
                                            else store_x = atof(val.c_str());

#define LOAD_I( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl;} \
                                            else store_x = atoi(val.c_str());
#define LOAD_P3d( field_x, err_x, store_x ) if( inLoader.GetField( (field_x), val ) != 0 )\
                                            { cout << (err_x) << endl;} \
                                            inLoader.GetPoint3d(val.c_str(), store_x);


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
		break;
    case C_FIREWORKS_SYSTEM:
        cout << "using fireworks system" << endl;
        inParams.m_particleSystem = new CFireworksParticleSystem();
		ret = readNewtonianParticleSystem( inParams, inLoader );
		break;
    case C_FLOCK_SYSTEM:
        cout << "using flock (boids) system" << endl;
        inParams.m_particleSystem = new CFlockParticleSystem();
        ret = readFlockParticleSystem( inParams, inLoader );
        break;
    default:
        cout << "ERROR: unknown system type: " << val.c_str() << " - using newtonian system" << endl;
        inParams.m_particleSystem = new CNewtonianParticleSystem();
		ret = readNewtonianParticleSystem( inParams, inLoader );
		break;
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
		LOAD_P3d( C_CAMERA_POS_TAG, "ERROR: CameraPos field is missing ", inParams.m_cameraPos );
		LOAD_P3d( C_CAMERA_DIR_TAG, "ERROR: CameraDir field is missing ", inParams.m_cameraDir );
        LOAD_P3d( C_CAMERA_UP_TAG,  "ERROR: CameraUp field is missing ", inParams.m_cameraUp );
        LOAD_F( C_TIME_DELTA_TAG,   "ERROR: TimeDelta field is missing ", inParams.m_dT );

        LOAD_F( C_FOVY_TAG,    "ERROR: FovY field is missing ", inParams.m_dFovY );
        LOAD_F( C_ZNEAR_TAG,   "ERROR: zNear field is missing ", inParams.m_dZNear );
        LOAD_F( C_ZFAR_TAG,    "ERROR: zFar field is missing ", inParams.m_dZFar );

		// clear-color
		if (inLoader.GetField( C_CLEAR_COLOR_TAG, val ) == 0 ){
			if (inLoader.GetPoint3d(val, color) == 0){
				inParams.m_clearColor = color;
			}
			else{
				cerr << "Failed loading clear-color value: " << val << endl;
			}
		}

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
	LOAD_P3d( C_PARTICLES_ORIGIN_TAG, "ERROR: ParticlesOrigin field is missing ", inParams.m_particleSystem->m_dDefaultOrigin );
	// particle shape
	if (inLoader.GetField(C_PARTICLE_SHAPE_TAG, val) == 0){
		iVal = atoi(val.c_str());
		if (iVal <= 0 || iVal >= C_NUM_PARTICLESHAPES){
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
	// color randomness
	if (inLoader.GetField(C_PARTICLE_COLOR_RAND_TAG, val) == 0){
		inParams.m_particleSystem->m_dColorRandomness = atof(val.c_str());
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
CParticleSystemLoader::readFlockParticleSystem( CSimulationsParams &inParams, CLoadIni &inLoader )
{
    string val; //for LOAD_* macros
    bool   ret = false;

    do {
        CFlockParticleSystem* system = (CFlockParticleSystem*) inParams.m_particleSystem;

        int numParticlesPerFrame = 0;
        
        LOAD_I( C_PARTICLES_PER_FRAME_TAG, "ERROR: ParticlesPerFrame undefined", numParticlesPerFrame );
        system->setNumParticles( numParticlesPerFrame );

        LOAD_F( C_PARTICLE_MAX_VELOCITY, "ERROR: max particle velocity field is missing ", system->mMaxParticleVelocity );

        double tmp;
        LOAD_F( C_PARTICLE_SYSTEM_RADIUS, "ERROR: particle system radius is missing ", tmp );
        system->setParticleSystemRadius( tmp );

        LOAD_F( C_FLOCK_PARTICLE_SEPARATION, "ERROR: particle system separation is missing ", tmp );
        system->setParticleDistance( tmp );

        LOAD_F( C_FLOCK_FOV_ANGLE, "ERROR: particle system conic angle is missing ", tmp );
        system->setParticleFOVAngle( tmp*DEG2RAD );
        

        ret = true;
    } while(0);
    
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
		// load gravity
		if ((inLoader.GetField( C_NEWTONIAN_PS_GRAVITY_TAG, val ) == 0 ) &&
			inLoader.GetPoint3d(val, gravity) == 0){
			system->m_Gravity = gravity;
		}
		else{
			cerr << "Failed loading gravity value: " << val << endl;
			break;
		}
		// heading step
		if (inLoader.GetField( C_NEWTONIAN_PS_HEADING_STEP_TAG, val ) == 0 ){
			system->m_dHeadingStep = atof(val.c_str());
		}		

        LOAD_F( C_PARTICLE_MAX_VELOCITY, "ERROR: max particle velocity field is missing ", system->mMaxParticleVelocity );

		/*
		//DE
		// load origin
		if ((inLoader.GetField( C_NEWTONIAN_PS_ORIGIN_TAG, val ) == 0 ) &&
			inLoader.GetPoint3d(val, origin) == 0){
			system->m_Origin = origin;
		}
		else{
			cerr << "Failed loading origin value: " << val << endl;
			system->m_Origin = Point3d(0,0,0);
		}*/


        ret = true;
    } while(0);

    return ret;
}

#undef LOAD_L
#undef LOAD_F
#undef LOAD_I
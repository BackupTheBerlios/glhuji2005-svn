#ifndef _CONSTANTS_H__
#define _CONSTANTS_H__

const int C_WINDOW_WIDTH     = 500;
const int C_WINDOW_HEIGHT    = 400;

//delay in milliseconds for calling timer function.
const unsigned int C_TIMER_DELAY   = 15;

#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

//code for keyboard keys
const char C_ESCAPE_KEY_CODE = 27;
const char C_ENTER_KEY_CODE  = 13;

//constants for use throughout program
#define C_CAMERA_POS_TAG          ("CameraPos")
#define C_CAMERA_DIR_TAG          ("CameraDir")
#define C_CAMERA_UP_TAG           ("CameraUp")
#define C_FOVY_TAG                ("FovY")
#define C_ZNEAR_TAG               ("ZNear")
#define C_ZFAR_TAG                ("ZFar")
#define C_ASPECT_TAG			  ("Aspect")
#define C_CLEAR_COLOR_TAG	      ("ClearColor")
#define C_LINEAR_ATTENUATION_TAG  ("LinearAttenuation")

#define C_TIME_DELTA_TAG          ("TimeDelta")

#define C_DEFAULT_SPAN_TAG         ("DefaultSpan")
#define C_DEFAULT_LIFESPAN_TAG     ("DefaultLifeSpan")
#define C_DEFAULT_MASS_TAG         ("DefaultMass")
#define C_DEFAULT_RADIUS_TAG       ("DefaultRadius")
#define C_DEFAULT_PERSISTANCE_TAG  ("DefaultPersistance")
#define C_PARTICLE_SHAPE_TAG       ("ParticleShape")
#define C_PARTICLE_SIZE_TAG        ("ParticleSize")
#define C_PARTICLE_COLOR_TAG       ("ParticleColor")
#define C_PARTICLE_COLOR2_TAG      ("ParticleColor2")
#define C_PARTICLE_COLOR_RAND_TAG  ("ParticleColorRand")
#define C_PARTICLE_ALPHA_TAG       ("ParticleAlpha")
#define C_PARTICLES_ORIGIN_TAG	   ("ParticlesOrigin")

#define C_PARTICLES_PER_FRAME_TAG  ("ParticlesPerFrame")
#define C_PARTICLE_SYSTEM_TYPE_TAG ("ParticleSystemType")

const int C_NUM_FIELDS_IN_PARTICLE   = 5;
const int C_NUM_FIELDS_IN_AUTOCREATE = 6;

// newtonian PS stuff
#define C_NEWTONIAN_PS_GRAVITY_TAG			    ("Gravity")
#define C_NEWTONIAN_PS_HEADING_STEP_TAG			("HeadingStep")
#define C_NEWTONIAN_PS_FRAMES_DELAY_TAG			("FramesDelay")
#define C_NEWTONIAN_PS_ACC_RAND_TAG			    ("AccelerationRand")

// flock PS stuff
#define C_FLOCK_PARTICLE_SEPARATION_TAG ("FlockCloseParticleDistance")
#define C_FLOCK_FOV_ANGLE_TAG           ("FlockFOVAngle")
#define C_PARTICLE_MAX_VELOCITY_TAG		("FlockMaxVelocity")
#define C_PARTICLE_SYSTEM_RADIUS_TAG	("ParticleSystemRadius")
#define C_PARTICLE_MAX_ACCELERATION_TAG ("FlockMaxAcceleration")
#define C_FLOCK_UNIFORM_ACCEL_TAG       ("FlockUniformAcceleration")
#define C_FLOCK_LOCK_Y_TAG				("FlockLockY")


typedef enum {
    C_NEWTONIAN_SYSTEM    = 1,
	C_FIREWORKS_SYSTEM    = 2,
    C_FLOCK_SYSTEM        = 3,
	C_SYSTEM_NUM	
} ParticleSystemType;

typedef enum {
    C_PARTICLESHAPE_DOT    = 1,
	C_PARTICLESHAPE_SPHERE,
	C_PARTICLESHAPE_LEAF,
	C_PARTICLESHAPE_CONE,
	C_PARTICLESHAPE_BALOON,
    C_PARTICLESHAPE_TEAPOT,
	C_PARTICLESHAPE_ANT,
	C_NUM_PARTICLESHAPES
} ParticleShapeType;

#define      M_PI                          3.14159265358979323846
#define      NEAR_ZERO                     0.001

const float  C_SCALE_FACTOR              = 30.0; //factors for scale
const float  C_TRANSLATION_FACTOR        = -0.1f; //factor for translation
const float  C_OBJECT_B_RAD              = 45.0;   //default object radius

#endif //_CONSTANTS_H__
#ifndef __MY_MACROS_H__
#define __MY_MACROS_H__

//useful for cleaning up variables
#define SAFE_DELETE( todel_x ) if( (todel_x) != NULL ){ delete (todel_x); (todel_x) = NULL; }

#define SAFE_DELETE_ARR( todel_x ) if( (todel_x) != NULL ){ delete [] (todel_x); (todel_x) = NULL; }

//convert angles
#define RAD2DEG(x)  ((x)*(360.0)/(2.0*M_PI))
#define RADTODEG(x) ((x)*(2.0*M_PI)/(360.0))

#define SQR(x)      ((x)*(x))

#endif //__MY_MACROS_H__
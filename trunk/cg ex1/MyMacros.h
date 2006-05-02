#ifndef __MY_MACROS_H__
#define __MY_MACROS_H__

//useful for cleaning up variables
#define SAFE_DELETE( todel_x ) if( (todel_x) != NULL ){ delete (todel_x); (todel_x) = NULL; }

#define SAFE_DELETE_ARR( todel_x ) if( (todel_x) != NULL ){ delete [] (todel_x); (todel_x) = NULL; }

#endif //__MY_MACROS_H__
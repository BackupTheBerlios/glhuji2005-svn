#ifndef _CONSTANTS_H__
#define _CONSTANTS_H__

#define C_DEFAULT_SIMULATION_FILE ("finecloth.psim")

const int C_WINDOW_WIDTH     = 500;
const int C_WINDOW_HEIGHT    = 400;

//delay in milliseconds for calling timer function.
const uint16 C_TIMER_DELAY   = 15;

//code fore escape key
const char C_ESCAPE_KEY_CODE = 27;

//constants for use throughout program
#define C_MESH_WIDTH_TAG         ("MeshWidth")
#define C_MESH_HEIGHT_TAG        ("MeshHeight")
#define C_GRAVITY_TAG            ("Gravity")
#define C_STEP_SIZE_TAG          ("h")
#define C_SPRING_CONST_TAG       ("K")
#define C_SPRING_DRAG_TAG        ("B")
#define C_SHEAR_SPRING_CONST_TAG ("ShearK")
#define C_SHEAR_SPRING_DRAG_TAG  ("ShearB")
#define C_FLEX_SPRING_CONST_TAG  ("FlexK")
#define C_FLEX_SPRING_DRAG_TAG   ("FlexB")
#define C_SOLVER_TYPE_TAG        ("Solver")
#define C_MIDPOINT_TYPE_TAG      ("Midpoint")
#define C_AUTOCREATE_MESH_TAG    ("AutoCreateMesh")

const int C_NUM_FIELDS_IN_PARTICLE   = 5;
const int C_NUM_FIELDS_IN_AUTOCREATE = 6;

typedef enum {
    C_FORWARD_EULER_SOLVER = 1,
    C_REVERSE_EULER_SOLVER = 2
} SolverType;

#endif //_CONSTANTS_H__
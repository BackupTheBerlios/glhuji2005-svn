#ifndef _CONSTANTS_H__
#define _CONSTANTS_H__

//constants for use throughout program

#define C_MESH_WIDTH_TAG  ("MeshWidth")
#define C_MESH_HEIGHT_TAG ("MeshHeight")
#define C_GRAVITY_TAG     ("Gravity")
#define C_SPRING_CONST_TAG ("K")
#define C_SPRING_DRAG_TAG ("B")
#define C_SHEAR_SPRING_CONST_TAG ("ShearK")
#define C_SHEAR_SPRING_DRAG_TAG ("ShearB")
#define C_FLEX_SPRING_CONST_TAG ("FlexK")
#define C_FLEX_SPRING_DRAG_TAG ("FlexB")
#define C_SOLVER_TYPE_TAG ("Solver")
#define C_MIDPOINT_TYPE_TAG ("Midpoint")

const int C_NUM_FIELDS_IN_PARTICLE = 5;

#define C_FORWARD_EULER_SOLVER 1
#define C_REVERSE_EULER_SOLVER 2

#endif //_CONSTANTS_H__
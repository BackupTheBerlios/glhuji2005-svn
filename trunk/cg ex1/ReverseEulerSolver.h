#ifndef __REVERSE_EULER_SOLVER_H__
#define __REVERSE_EULER_SOLVER_H__

#include "NumericalSolver.h"

class ReverseEulerSolver : public NumericalSolver
{
public:
    virtual void step( double h );
};

#endif //__REVERSE_EULER_SOLVER_H__
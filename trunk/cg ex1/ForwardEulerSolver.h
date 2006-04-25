#ifndef __FORWARD_EULER_SOLVER_H__
#define __FORWARD_EULER_SOLVER_H__

#include "NumericalSolver.h"

class ForwardEulerSolver : public NumericalSolver
{
public:
    virtual void step( double h );
};

#endif //__FORWARD_EULER_SOLVER_H__
#ifndef __FORWARD_EULER_SOLVER_H__
#define __FORWARD_EULER_SOLVER_H__

#include "NumericalSolver.h"

class ForwardEulerSolver : public NumericalSolver
{
public:
    ForwardEulerSolver();
    ~ForwardEulerSolver();

    virtual void step( double h );

protected:
    Vector3d *mAccel;
};

#endif //__FORWARD_EULER_SOLVER_H__
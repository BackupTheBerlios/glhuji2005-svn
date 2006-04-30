#ifndef __RUNGE_KUTTA_SOLVER_H__
#define __RUNGE_KUTTA_SOLVER_H__

#include "NumericalSolver.h"

class RungeKuttaSolver : public NumericalSolver
{
public:
    RungeKuttaSolver();
    ~RungeKuttaSolver();

    void step( double h );

protected:
    Vector3d *mAccel;
    Vector3d *mTmpPos;
    Vector3d *mTmpV;
    Vector3d *mK1;
    Vector3d *mK2;
    Vector3d *mK3;
    Vector3d *mK4;

};

#endif //__RUNGE_KUTTA_SOLVER_H__
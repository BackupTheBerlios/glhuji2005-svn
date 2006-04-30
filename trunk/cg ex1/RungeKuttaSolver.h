#ifndef __RUNGE_KUTTA_SOLVER_H__
#define __RUNGE_KUTTA_SOLVER_H__

#include "NumericalSolver.h"

class RungeKuttaSolver : public NumericalSolver
{
public:
    void step( double h );

};

#endif //__RUNGE_KUTTA_SOLVER_H__
//#ifndef __NUMERICAL_SOLVER_H__
//#define __NUMERICAL_SOLVER_H__

#pragma once

#include "NumericalSolver.h"

//base class for numerical solvers
//todo: fill in later...
class ForwardEulerSolver : public NumericalSolver
{
public:
    ForwardEulerSolver(ParticleSystem *pParticleSystem, bool bMidPoint = false);
    virtual ~ForwardEulerSolver();

    virtual void step( double h );

protected:
	int mDummy;
};

//#endif //__NUMERICAL_SOLVER_H__
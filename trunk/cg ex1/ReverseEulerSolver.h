//#ifndef __NUMERICAL_SOLVER_H__
//#define __NUMERICAL_SOLVER_H__

#pragma once

#include "NumericalSolver.h"

//base class for numerical solvers
//todo: fill in later...
class ReverseEulerSolver : public NumericalSolver
{
public:
    ReverseEulerSolver(ParticleSystem *pParticleSystem, bool bMidPoint = false);
    virtual ~ReverseEulerSolver();

    virtual void step( double h );

protected:
	int mDummy;
};

//#endif //__NUMERICAL_SOLVER_H__
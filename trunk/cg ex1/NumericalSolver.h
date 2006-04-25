//#ifndef __NUMERICAL_SOLVER_H__
//#define __NUMERICAL_SOLVER_H__

#pragma once

//base class for numerical solvers
//todo: fill in later...
#include "Spring.h"

class ParticleSystem;
class NumericalSolver
{

//-------------- internal types --------------
protected:
	typedef vector<Spring>       SpringList;
    typedef SpringList::iterator SpringListIt;

public:
    NumericalSolver(ParticleSystem *pParticleSystem, bool bMidPoint = false);
    virtual ~NumericalSolver();

	ParticleSystem *mParticleSystem;
    virtual void step( double h );

protected:
    int mType;
	bool mMidpoint;
};

//#endif //__NUMERICAL_SOLVER_H__
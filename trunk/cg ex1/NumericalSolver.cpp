#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"


NumericalSolver::NumericalSolver(ParticleSystem *pParticleSystem, bool bMidPoint)
{
	mType = 0;
	mParticleSystem = pParticleSystem;
	mMidpoint = bMidPoint;
}

NumericalSolver::~NumericalSolver()
{
}

void 
NumericalSolver::step( double h )
{
    Vector3d gravity( 0, 0, -9.81 );

	int width = mParticleSystem->getWidth();
	int height = mParticleSystem->getHeight();
    //clear force vector for particles and add gravity
    for( int i = 0; i < (width * height); i++ )
        mParticleSystem->getParticleAt(i).getForce() = gravity;

    //iterate over all springs and sum force on each particle
    for( SpringListIt it = mParticleSystem->mSprings.begin(); it != mParticleSystem->mSprings.end(); it++ )
    {
        Spring &theSpring = *it;
    }

    //add other forces (gravity)

    //run solver on all particles

}
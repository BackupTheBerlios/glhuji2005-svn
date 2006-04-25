#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"


NumericalSolver::NumericalSolver()
{
}

NumericalSolver::~NumericalSolver()
{
}

void 
NumericalSolver::attachToParticleSystem( ParticleSystem *inParticleSystem, bool inMidPoint = false )
{
    mParticleSystem = inParticleSystem;
    mMidpoint       = inMidPoint;
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

    SpringList &springsList = mParticleSystem->getSprings();

    //iterate over all springs and sum force on each particle
    for( SpringListIt it = springsList.begin(); it != springsList.end(); it++ )
    {
        Spring &theSpring = *it;
    }

    //add other forces (gravity)

    //run solver on all particles

}
#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"


typedef ParticleSystem::SpringList       SpringList;
typedef ParticleSystem::SpringListIt     SpringListIt;

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
NumericalSolver::calcAccel( Vector3d *inPositions, Vector3d *inVelocities, 
                             double *inInvMasses, Vector3d *outAccel )
{
    Vector3d gravity(0,0,0);
    idx_t    numParticles = mParticleSystem->getNumParticles();

    gravity.pY = -(mParticleSystem->getGravity());

    //---------- Zero Particle Forces ----------
    for( int i = 0; i < numParticles; i++ )
        outAccel[i].set(0,0,0);

    //---------- Sum Forces ----------
    for( SpringListIt it = mParticleSystem->getSprings().begin(); 
        it != mParticleSystem->getSprings().end(); it++ )
    {
        Spring &theSpring = *it;
        idx_t aIdx = theSpring.getParticleA();
        idx_t bIdx = theSpring.getParticleB();

        Vector3d dx = (inPositions[aIdx]-inPositions[bIdx]);
        Vector3d dv = (inVelocities[aIdx]-inVelocities[bIdx]);

        //project velocities onto spring axis
        dv = dv.proj(dx);

        Vector3d F = dx.normalized()*((dx.length()-theSpring.getRestLength())*theSpring.getK());
        Vector3d FB = dv*theSpring.getB();
		Vector3d Fn = F.normalized();
		Vector3d FBn = FB.normalized();
        if (!Fn.sameAs(FBn))	//Opposing force must be in the opposite direction of the original force
		{
			if (F.length() <= FB.length())	//Opposing force cannot be greater the original force
				FB = Vector3d(0,0,0);
			else
				F += FB;
		}

        //force affects both springs in opposite directions
        outAccel[aIdx] -= F;
        outAccel[bIdx] += F;
    }

    //----------------- calculate Acceleration -------------------
    for( int i = 0; i < numParticles; i++ )
    {
        //todo: divide at end...
        outAccel[i] = gravity + (outAccel[i] * inInvMasses[i]);
    }
}

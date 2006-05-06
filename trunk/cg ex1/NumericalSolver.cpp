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
    idx_t    numParticles   = mParticleSystem->getNumParticles();

    gravity.pY = -(mParticleSystem->getGravity());

    //---------- Initialize Particle Forces to wind force ----------
	Vector3d Wind = mParticleSystem->getNewWind();
    for( int i = 0; i < numParticles; i++ )
	{
        outAccel[i] = Wind;
	}

    //---------- Sum Forces ----------
    for( SpringListIt it = mParticleSystem->getSprings().begin(); 
        it != mParticleSystem->getSprings().end(); it++ )
    {
        Spring &theSpring = *it;
        idx_t aIdx = theSpring.getParticleA();
        idx_t bIdx = theSpring.getParticleB();

        Vector3d dx = (inPositions[aIdx]-inPositions[bIdx]);
        Vector3d dv = (inVelocities[aIdx]-inVelocities[bIdx]);

        double xLen = dx.length();
        double len  = xLen - theSpring.getRestLength();

        //avoid divide by zero
        if( ABS(xLen) < NEAR_ZERO || ABS(len) < NEAR_ZERO )
            continue;

        //don't allow springs to stretch more than twice their original length
//        if( len > (3 * theSpring.getRestLength()) )
//            continue;

        Vector3d F = (dx/xLen)*(len*theSpring.getK() + (dv.dot(dx)/xLen)*theSpring.getB());

        //force affects both springs in opposite directions
        outAccel[aIdx] -= F;
        outAccel[bIdx] += F;
    }

    //----------------- calculate Acceleration -------------------
    for( int i = 0; i < numParticles; i++ )
    {
        outAccel[i] = gravity + (outAccel[i] * inInvMasses[i]);
		if (outAccel[i].length() > gravity.length()*2)
		{
			outAccel[i].normalize();
			outAccel[i] = outAccel[i]*(gravity.length()*2);
		}
    }
}

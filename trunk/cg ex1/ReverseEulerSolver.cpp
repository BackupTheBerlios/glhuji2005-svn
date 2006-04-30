#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ReverseEulerSolver.h"

typedef ParticleSystem::SpringList       SpringList;
typedef ParticleSystem::SpringListIt     SpringListIt;
typedef ParticleSystem::ForceList        ForceList;
typedef ParticleSystem::ForceListIt      ForceListIt;

void 
ReverseEulerSolver::step( double h )
{
	Vector3d zero( 0, 0, 0 );
	int width = mParticleSystem->getWidth();
	int height = mParticleSystem->getHeight();
    //clear force vector for particles and add gravity
    for( int i = 0; i < (width * height); i++ )
        mParticleSystem->getParticleAt(i).force() = zero;

	Vector3d F;
	Particle* A;
	Particle* B;
	Vector3d dx;
	Vector3d dv;
	Vector3d ABaxis;
	Vector3d tmp;

	
    //iterate over all springs and sum force on each particle
    for( SpringListIt it = mParticleSystem->getSprings().begin(); it != mParticleSystem->getSprings().end(); it++ )
    {
        Spring &theSpring = *it;
		A = &mParticleSystem->getParticleAt(theSpring.getParticleA());
		B = &mParticleSystem->getParticleAt(theSpring.getParticleB());
		if (mMidpoint)
		{
			Vector3d AMidPoint = A->pos()+(A->getNextPos()-A->pos())*0.5;
			Vector3d BMidPoint = B->pos()+(B->getNextPos()-B->pos())*0.5;
			dx = AMidPoint-BMidPoint;
		}
		else
			dx = (A->getNextPos())-(B->getNextPos());

        ABaxis = dx.normalized();
		dv = A->velocity()-B->velocity();
		dv = dv.proj(ABaxis);
		
		F = ABaxis*((dx.length()-theSpring.getRestLength())*theSpring.getK()) + dv*theSpring.getB();

        //force works in separate directions on both particles
		A->force() -= F;
		B->force() += F;
    }

    //add other forces (wind...)

    //run solver on all particles
	Vector3d vA;
	Vector3d dPos;
	
    for( int i = 0; i < (width * height); i++ )
	{
        Particle &P = mParticleSystem->getParticleAt(i);

		if (!P.isPinned())
		{
            //start with spring forces
			vA = P.force();

            // a += other forces
            for( ForceListIt it = mParticleSystem->getForces().begin(); 
                    it != mParticleSystem->getForces().end(); it++)
            {
                Vector3d &force = (*it)->getForceAt( P.getPos(), P );
                vA += force;
            }

            // a = F/m
            vA = vA / P.getMass();

            // newV = oldV + a * h
			P.velocity() += vA*h;

            //dx = newV*h
			dPos = P.velocity() * h;

            //todo: midpoint?
			if (mMidpoint)
				P.midPos() = P.pos() + (dPos*0.5);	//Calculate mid pos for midpoint calculation

			P.pos() += dPos;
		}
	}

}

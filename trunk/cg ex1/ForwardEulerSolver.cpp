#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ForwardEulerSolver.h"

typedef ParticleSystem::SpringList       SpringList;
typedef ParticleSystem::SpringListIt     SpringListIt;
typedef ParticleSystem::ForceList        ForceList;
typedef ParticleSystem::ForceListIt      ForceListIt;

void 
ForwardEulerSolver::step( double h )
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
			dx = (A->midPos())-(B->midPos());
		else
			dx = (A->pos())-(B->pos());
		ABaxis = dx;
		dv = A->velocity()-B->velocity();

        dv = dv.proj(ABaxis);
		
		F = ABaxis*((dx.length()-theSpring.getRestLength())*theSpring.getK()) + 
                                                dv*theSpring.getB();

		//force affects both springs in opposite directions
        A->force() -= F;
		B->force() += F;
    }

    //add other forces (wind...)

    //-------------- run solver on all particles --------------
	Vector3d vA;   //force accumulator
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
                Vector3d &force = (*it)->getForceAt( P.pos() );
                vA += force;
            }

            // a = F/m
            vA = vA / P.getMass();

            // newV = oldV + a * h
			P.velocity() += vA*h;

            //dx = newV*h
			dPos = P.velocity()*h;

            //todo: midpoint?
			if (mMidpoint)
				P.midPos() = P.pos() + (dPos*0.5);	//Calculate mid pos for midpoint calculation

			P.pos() += dPos;
		}
	}

}

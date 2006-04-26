#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ReverseEulerSolver.h"

void 
ReverseEulerSolver::step( double h )
{
	Vector3d gravity( 0, -9.81, 0 );
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
		ABaxis = dx;
		ABaxis.normalize();
		dv = A->velocity()-B->velocity();
		dv = dv.proj(ABaxis);
		
		F = ABaxis*((dx.length()-theSpring.getRestLength())*theSpring.getK()) + dv.proj(ABaxis)*theSpring.getB();
		A->force() -= F;
		B->force() += F;
    }

    //add other forces (wind...)

    //run solver on all particles
	double a;
	Vector3d vA;
	Vector3d dPos;
	
    for( int i = 0; i < (width * height); i++ )
	{
        Particle &P = mParticleSystem->getParticleAt(i);
		if (!P.isPinned())
		{
			vA = P.force();
			vA.normalize();
			a = P.force().length()/P.getMass();
			vA *= a*h;
			vA += gravity*h;
			P.velocity() += vA;
			dPos = P.velocity();
			dPos *= h;
			if (mMidpoint)
				P.midPos() = P.pos() + (dPos*0.5);	//Calculate mid pos for midpoint calculation
			P.pos() += dPos;
		}
	}

}

#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ForwardEulerSolver.h"

ForwardEulerSolver::ForwardEulerSolver(ParticleSystem *pParticleSystem, bool bMidPoint) :NumericalSolver(pParticleSystem, bMidPoint)
{
}

ForwardEulerSolver::~ForwardEulerSolver()
{
}

void 
ForwardEulerSolver::step( double h )
{
	Vector3d gravity( 0, -9.81, 0 );
    Vector3d zero( 0, 0, 0 );
	int width = mParticleSystem->getWidth();
	int height = mParticleSystem->getHeight();
    //clear force vector for particles and add gravity
    for( int i = 0; i < (width * height); i++ )
        mParticleSystem->getParticleAt(i).getForce() = zero;

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
//		dx = (A->getNextPos())-(B->getNextPos());
		if (mMidpoint)
			dx = (A->getMidPos())-(B->getMidPos());
		else
			dx = (A->getPos())-(B->getPos());
		ABaxis = dx;
		ABaxis.normalize();
		dv = A->getVelocity()-B->getVelocity();
		dv = dv.proj(ABaxis);
		
		F = ABaxis*((dx.length()-theSpring.getRestLength())*theSpring.getK()) + dv.proj(ABaxis)*theSpring.getB();
		A->getForce() -= F;
		B->getForce() += F;
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
			vA = P.getForce();
			vA.normalize();
			a = P.getForce().length()/P.getMass();
			vA *= a*h;
			vA += gravity*h;
			P.getVelocity() += vA;
			dPos = P.getVelocity();
			dPos *= h;
			if (mMidpoint)
				P.getMidPos() = P.getPos() + (dPos*0.5);	//Calculate mid pos for midpoint calculation
			P.getPos() += dPos;
		}
	}

}

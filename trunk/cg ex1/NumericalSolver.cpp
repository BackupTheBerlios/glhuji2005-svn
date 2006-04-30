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
typedef ParticleSystem::ForceList        ForceList;
typedef ParticleSystem::ForceListIt      ForceListIt;

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
}

void 
NumericalSolver::calcAccel( idx_t inNumParticles, 
                             Vector3d *inPositions, Vector3d *inVelocities, 
                             double *inMasses, Vector3d *outAccel )
{
    Vector3d gravity(0, -9.81, 0);

    //---------- Zero Particle Forces ----------
    for( int i = 0; i < inNumParticles; i++ )
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
        dv.proj(dx);

        //todo: handle damping
        Vector3d F = dx.normalized()*((dx.length()-theSpring.getRestLength())*theSpring.getK());
            //dv * theSpring.getB();

        //force affects both springs in opposite directions
        outAccel[aIdx] -= F;
        outAccel[bIdx] += F;
    }

    //----------------- calculate Acceleration -------------------
    for( int i = 0; i < inNumParticles; i++ )
    {
        //todo: divide at end...
        outAccel[i] = gravity + (outAccel[i]/inMasses[i]);
    }
}

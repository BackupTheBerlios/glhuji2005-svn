#include "StdAfx.h"
#include "Vector3d.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ForwardEulerSolver.h"


ForwardEulerSolver::ForwardEulerSolver()
{
    mAccel = NULL;
}

ForwardEulerSolver::~ForwardEulerSolver()
{
    if( mAccel != NULL )
        delete [] mAccel;
}

void 
ForwardEulerSolver::step( double h )
{

    idx_t numParticles = mParticleSystem->getNumParticles();

    //allocate tmp store if it hasn't been allocated already
    if( mAccel == NULL )
        mAccel = new Vector3d[ numParticles ];

    //Copy out original position and velocity
    Vector3d *pos       = mParticleSystem->getParticlePositions();
    Vector3d *velocity  = mParticleSystem->getParticleVelocities();
    double   *invMasses = mParticleSystem->getParticleInvMasses();

    ParticleSystem::ParticleInfo *pInfo =
                                mParticleSystem->getParticleInfo();

    //calc dA
    calcAccel( pos, velocity, invMasses, mAccel );

    //store changes
    for( idx_t i = 0; i < numParticles; i++ )
    {
        //skip over pinned particles
        if( pInfo[i].pIsPinned )
            continue;

        velocity[i] += mAccel[i] * h;
        pos[i] += velocity[i] * h;
    }

}

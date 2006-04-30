#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "MidPointSolver.h"

MidPointSolver::MidPointSolver()
{
    mAccel  = NULL;
    mTmpPos = NULL;
    mTmpV   = NULL;
}

MidPointSolver::~MidPointSolver()
{
    if( mAccel != NULL )
        delete [] mAccel;

    if( mTmpPos != NULL )
        delete [] mTmpPos;

    if( mTmpV != NULL )
        delete [] mTmpV;
}

void 
MidPointSolver::step( double h )
{
    int numParticles     = mParticleSystem->getNumParticles();
    double halfH         = h / 2.0;

    //allocate tmp store if it hasn't been allocated already
    if( mAccel == NULL )
    {
        mAccel = new Vector3d[ numParticles ];
        mTmpPos = new Vector3d[ numParticles ];
        mTmpV = new Vector3d[ numParticles ];
    }


    //Copy out original position and velocity
    Vector3d *origPos   = mParticleSystem->getParticlePositions();
    Vector3d *origV     = mParticleSystem->getParticleVelocities();
    double   *invMasses = mParticleSystem->getParticleInvMasses();

    ParticleSystem::ParticleInfo *pInfo =
        mParticleSystem->getParticleInfo();

    //---------- 1. calculate da at original position ----------
    calcAccel( origPos, origV, invMasses, mAccel );

    //---------- 2. calculate x & v at midpoint ----------
    for( int i = 0; i < numParticles; i++ )
    {
        mTmpV[i]   = origV[i]   + (mAccel[i] * halfH);
        mTmpPos[i] = origPos[i] + (mTmpV[i] * halfH);
    }

    //---------- 3. calculate dA at Midpoint ----------
    calcAccel( mTmpPos, mTmpV, invMasses, mAccel );

    //---------- 4. set x & v of particles based on 3 ----------
    for( int i = 0; i < numParticles; i++ )
    {
        if( pInfo[i].pIsPinned )
            continue;

        origV[i]   += mAccel[i] * h;
        origPos[i] += origV[i] * h;
    }

}
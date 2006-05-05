#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "Rungekuttasolver.h"

RungeKuttaSolver::RungeKuttaSolver()
{
    mTmpPos = NULL;
    mTmpV   = NULL;
    mK1     = NULL;
    mK2     = NULL;
    mK3     = NULL;
    mK4     = NULL;
}

RungeKuttaSolver::~RungeKuttaSolver()
{
    SAFE_DELETE_ARR( mTmpPos );
    SAFE_DELETE_ARR( mTmpV );
    SAFE_DELETE_ARR( mK1 );
    SAFE_DELETE_ARR( mK2 );
    SAFE_DELETE_ARR( mK3 );
    SAFE_DELETE_ARR( mK4 );
}

void 
RungeKuttaSolver::step( double h )
{
    int    numParticles  = mParticleSystem->getNumParticles();
    double airResistance = mParticleSystem->getAirResistance();
    double halfH         = h / 2.0;
    double h6            = h / 6.0;

    //Allocate temp. storage on first call
    if( mTmpPos == NULL )
    {
        mTmpPos = new Vector3d[numParticles];
        mTmpV   = new Vector3d[numParticles];
        mK1     = new Vector3d[numParticles];
        mK2     = new Vector3d[numParticles];
        mK3     = new Vector3d[numParticles];
        mK4     = new Vector3d[numParticles];
    }

    //Copy out original position and velocity
    Vector3d *origPos   = mParticleSystem->getParticlePositions();
    Vector3d *origV     = mParticleSystem->getParticleVelocities();
    double   *invMasses = mParticleSystem->getParticleInvMasses();

    ParticleSystem::ParticleInfo *pInfo =
        mParticleSystem->getParticleInfo();

    //---------- 1. k1 = h*f(x,y) ----------
    calcAccel( origPos, origV, invMasses, mK1 );

    //---------- 2. k2 = h*f(x,y+k1/2) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        mTmpV[i]    = origV[i]   + (mK1[i] * halfH);
        mTmpPos[i]  = origPos[i] + (mTmpV[i] * halfH);
    }

    calcAccel( mTmpPos, mTmpV, invMasses, mK2 );

    //---------- 3. k3 = h*f(x,y+k2/2) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        mTmpV[i]   = origV[i]     + (mK2[i] * halfH);
        mTmpPos[i] = origPos[i] + (mTmpV[i] * halfH);
    }

    calcAccel( mTmpPos, mTmpV, invMasses, mK3 );

    //---------- 3. k4 = h*f(x,y+k3) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        mTmpV[i]   =  origV[i]   + (mK3[i] * h);
        mTmpPos[i] = origPos[i] + (mTmpV[i] * h);
    }

    calcAccel( mTmpPos, mTmpV, invMasses, mK4 );

    //---------- 4. set x & v of particles ----------
    
    //Set particle position
    for( int i = 0; i < numParticles; i++ )
    {
        //don't move pinned particles
        if( pInfo[i].pIsPinned )
            continue;

        Vector3d &tmpV = origV[i];
        //origPos[i] += ( tmpV + (tmpV + mK1[i]*halfH)*2.0 + (tmpV+mK2[i]*halfH)*2.0 + (tmpV+mK3[i]*h) ) * h6;
        origPos[i] += tmpV*h + ( mK1[i]*h + mK2[i]*h + mK3[i]*h ) * h6;
        tmpV   += (mK1[i] + mK2[i]*2.0 + mK3[i]*2.0 + mK4[i]) * h6;

        origV[i] *= airResistance;
    }

}
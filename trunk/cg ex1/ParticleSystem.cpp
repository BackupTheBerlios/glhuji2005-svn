#include "StdAfx.h"

#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "NumericalSolver.h"

#include "Particlesystem.h"

ParticleSystem::ParticleSystem(  )
{
    mWidth  = 0;
    mHeight = 0;
    mParticles = 0;
}

void 
ParticleSystem::setDimensions( idx_t inMeshWidth, idx_t inMeshHeight )
{
    //sanity
    assert( inMeshWidth > 0 && inMeshHeight > 0 );

    //free allocated memory
    if( mParticles != 0 )
    {
        delete [] mParticles;
        mParticles = 0;
    }

    //set
    mWidth = inMeshWidth;
    mHeight = inMeshHeight;
    mParticles = new Particle[ mWidth * mHeight ];
}

ParticleSystem::~ParticleSystem()
{
    delete[] mParticles;
    mParticles = 0;

    for( ForceList::iterator it = mForces.begin(); it != mForces.end(); it++ )
    {
        //delete force pointer
        delete (*it);
    }

    mForces.clear();
}

void 
ParticleSystem::addParticleAt( idx_t inX, idx_t inY, Particle &inParticle )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    //assign
    mParticles[ inX + (inY*mWidth) ] = inParticle;
}

void 
ParticleSystem::addForce( Force *inForce )
{
    mForces.push_back( inForce );
}

Particle &
ParticleSystem::getParticleAt( idx_t inX, idx_t inY )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    return mParticles[ inX + (inY*mWidth) ];
}

void 
ParticleSystem::setSolver( NumericalSolver &inSolver )
{
    mSolver = inSolver;
}
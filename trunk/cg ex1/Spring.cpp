#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"

Spring::Spring()
{
    Spring( -1, -1, 0,  0, 0 );
}

Spring::Spring( idx_t inParticleA, idx_t inParticleB,
       double inLength, double inK, double inB )
{
    mParticleA  = inParticleA;
    mParticleB  = inParticleB;
    mK          = inK;
    mB          = inB;
    mRestLength = inLength;
}

Spring::~Spring()
{
}

idx_t
Spring::getParticleA()
{
    return mParticleA;
}

idx_t
Spring::getParticleB()
{
    return mParticleB;
}

double    
Spring::getK()
{
    return mK;
}

double    
Spring::getB()
{
    return mB;
}

double
Spring::getRestLength()
{
    return mRestLength;
}
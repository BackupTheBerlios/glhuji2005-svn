#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"

Spring::Spring()
{
    Spring( -1, -1, 0, 0 );
}

Spring::Spring( idx_t inParticleA, idx_t inParticleB,
       float inK, float inB )
{
    mParticleA = inParticleA;
    mParticleB = inParticleB;
    mK         = inK;
    mB         = inB;
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

float    
Spring::getK()
{
    return mK;
}

float    
Spring::getB()
{
    return mB;
}
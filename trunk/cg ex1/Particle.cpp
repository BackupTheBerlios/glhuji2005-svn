#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"

Particle::Particle(void)
{
    Particle(-1,-1,-1,-1, false);
}

Particle::~Particle(void)
{
}

Particle::Particle( float inX, float inY, float inZ, float inMass,
                    bool inIsPinned)
{
    mVelocity = Vector3d(0,0,0);
    mPos      = Vector3d(inX, inY, inZ);
    mMass     = inMass;
    mIsPinned = inIsPinned;
}

Particle& 
Particle::operator= (Particle& rhs)
{
    mVelocity = rhs.mVelocity;
    mPos      = rhs.mPos;
    mMass     = rhs.mMass;
    mIsPinned = rhs.mIsPinned;

    return *this;
}

Vector3d &
Particle::getPos()
{
    return mPos;
}

Vector3d &
Particle::getVelocity()
{
    return mVelocity;
}

float    
Particle::getMass()
{
    return mMass;
}
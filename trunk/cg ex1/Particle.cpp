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

Particle::Particle( double inX, double inY, double inZ, double inMass,
                    bool inIsPinned)
{
    mVelocity = Vector3d(0,0,0);
    mPos      = Vector3d(inX, inY, inZ);
    mMass     = inMass;
    mIsPinned = inIsPinned;
}

Vector3d 
Particle::getPos()
{
    return mPos;
}

Vector3d
Particle::getVelocity()
{
    return mVelocity;
}

double    
Particle::getMass()
{
    return mMass;
}
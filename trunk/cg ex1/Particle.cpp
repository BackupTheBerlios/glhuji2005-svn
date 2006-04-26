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
    mMidPos   = Vector3d(inX, inY, inZ);
    mMass     = inMass;
    mIsPinned = inIsPinned;
}

Particle& 
Particle::operator= (Particle& rhs)
{
    mVelocity = rhs.mVelocity;
    mPos      = rhs.mPos;
    mMidPos   = rhs.mMidPos;
    mMass     = rhs.mMass;
    mIsPinned = rhs.mIsPinned;

    return *this;
}

Vector3d &
Particle::pos()
{
    return mPos;
}

Vector3d 
Particle::getPos()
{
    return mPos;
}

Vector3d &
Particle::midPos()
{
    return mMidPos;
}
Vector3d
Particle::getNextPos( double h )
{
    return Vector3d(mPos + mVelocity*h);
}

Vector3d &
Particle::velocity()
{
    return mVelocity;
}

double    
Particle::getMass()
{
    return mMass;
}
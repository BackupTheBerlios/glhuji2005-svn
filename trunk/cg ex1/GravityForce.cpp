#include "StdAfx.h"
#include "Gravityforce.h"
#include "Particle.h"

GravityForce::GravityForce( double inG )
{
    //gravity points down negative y axis
    mForceVec.set(0, -inG, 0 );
}

Vector3d 
GravityForce::getForceAt( Vector3d &pos, Particle &inParticle )
{
    return mForceVec * inParticle.getMass();
}
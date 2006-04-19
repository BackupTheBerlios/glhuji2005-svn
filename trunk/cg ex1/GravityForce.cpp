#include "StdAfx.h"
#include "Gravityforce.h"

GravityForce::GravityForce( float inG )
{
    mG = inG;
}

float 
GravityForce::getForceAt( Vector3d &pos )
{
    return mG;
}
#include "StdAfx.h"
#include "Gravityforce.h"

GravityForce::GravityForce( double inG )
{
    mG = inG;
}

double 
GravityForce::getForceAt( Vector3d &pos )
{
    return mG;
}
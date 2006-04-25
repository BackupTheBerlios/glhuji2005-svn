#include "StdAfx.h"
#include "Gravityforce.h"

GravityForce::GravityForce( double inG )
{
    //gravity points down negative z axis
    mForceVec.set(0, 0, -inG );
}
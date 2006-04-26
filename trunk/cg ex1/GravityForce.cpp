#include "StdAfx.h"
#include "Gravityforce.h"

GravityForce::GravityForce( double inG )
{
    //gravity points down negative y axis
    mForceVec.set(0, -inG, 0 );
}
#ifndef __GRAVITY_FORCE_H__
#define __GRAVITY_FORCE_H__

#include "Vector3d.h"
#include "Force.h"

//useful constants in MKS
#define C_EARTH_GRAVITY (9.81)

//force for gravity
class GravityForce : public Force
{

public:
    //default to earth's gravitational field
    GravityForce( double inG = C_EARTH_GRAVITY );

    Vector3d getForceAt( Vector3d &pos ){ return mForceVec; }

protected:
    Vector3d mForceVec;
};

#endif //__GRAVITY_FORCE_H__
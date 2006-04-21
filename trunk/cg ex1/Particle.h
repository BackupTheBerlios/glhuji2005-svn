#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Vector3d.h"

//Class holding information about an individual particle
class Particle
{
public:
    Particle();
    Particle(double inX, double inY, double inZ, double inMass, bool inIsPinned );
    Particle& operator= (Particle& rhs); 
    virtual ~Particle();

    Vector3d &getPos();
    Vector3d &getVelocity();
    double    getMass();
    Vector3d &getForce(){ return mForce; }


protected:
    bool     mIsPinned;  //is particle pinned to spot
    double    mMass;     //mass
    Vector3d mPos;      //position
    Vector3d mVelocity; //velocity
    Vector3d mForce; //velocity
};

#endif //__PARTICLE_H__
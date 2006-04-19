#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Vector3d.h"

//Class holding information about an individual particle
class Particle
{
public:
    Particle();
    Particle(float inX, float inY, float inZ, float inMass, bool inIsPinned );
    Particle& operator= (Particle& rhs); 
    virtual ~Particle();

    Vector3d &getPos();
    Vector3d &getVelocity();
    float    getMass();


protected:
    bool     mIsPinned;  //is particle pinned to spot
    float    mMass;     //mass
    Vector3d mPos;      //position
    Vector3d mVelocity; //velocity
};

#endif //__PARTICLE_H__
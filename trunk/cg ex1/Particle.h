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

    //Pin this particle
    void     pin(){ mIsPinned = true; }

    //getters
    Vector3d getPos();
    Vector3d getVelocity();
    double   getMass();
	bool     isPinned(){return mIsPinned;}


protected:
    bool     mIsPinned; //is particle pinned to spot
    double   mMass;     //mass
    Vector3d mPos;      //position
    Vector3d mVelocity; //velocity
};

#endif //__PARTICLE_H__
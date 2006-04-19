#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <vector>
#include "NumericalSolver.h"

using namespace std;

class Force;
class Particle;
class Spring;

//class representing a particle system
class ParticleSystem
{
//-------------- internal types --------------
protected:
    typedef vector<Spring>     SpringList;
    typedef vector<Force *>    ForceList;

//------------ public interface --------------
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void setDimensions( idx_t inMeshWidth, idx_t inMeshHeight );

    void addParticleAt( idx_t inX, idx_t inY, Particle &inParticle );
    
    //setters

    //ParticleSystem takes ownership of pointer (will delete).
    void addForce( Force *inForce );
    void setSolver( NumericalSolver &inSolver );

    //getters
    Particle &getParticleAt( idx_t inX, idx_t inY );

//----------------- storage --------------
protected:
    idx_t           mWidth;
    idx_t           mHeight;
    Particle        *mParticles;
    SpringList      mSprings;
    ForceList       mForces;
    NumericalSolver mSolver;
};

#endif //__PARTICLE_SYSTEM_H__
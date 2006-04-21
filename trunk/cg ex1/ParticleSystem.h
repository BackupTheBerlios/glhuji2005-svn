#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <vector>
#include "Spring.h"
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
    typedef vector<Spring>       SpringList;
    typedef SpringList::iterator SpringListIt;
    typedef vector<Force *>      ForceList;

//------------ public interface --------------
public:
    ParticleSystem();
    virtual ~ParticleSystem();

    //advance simulation in time using parameter h
    //todo: dagan this is whatt the GUI should call to get the next
    //stage of the simulation.
    void step( double h );

    void setDimensions( idx_t inMeshWidth, idx_t inMeshHeight );

    void addParticleAt( idx_t inX, idx_t inY, Particle &inParticle );

    void constructSprings( double inK, double inB );
    
    //setters

    //ParticleSystem takes ownership of pointer (will delete).
    void addForce( Force *inForce );
    void setSolver( NumericalSolver &inSolver );

    //getters
    Particle &getParticleAt( idx_t inX, idx_t inY );
	idx_t getWidth(){ return mWidth; };
	idx_t getHeight(){ return mHeight; };

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
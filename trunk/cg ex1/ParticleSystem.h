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

    //advance simulation in time, step size (h) is set via the setStepSize
    //function
    void step();

    void setDimensions( idx_t inMeshWidth, idx_t inMeshHeight );

    void addParticleAt( idx_t inX, idx_t inY, Particle &inParticle );

    void pinParticle( idx_t inX, idx_t inY );

    void autoCreateMesh( double inOriginX, double inOriginY, double inOriginZ,
                            double inMass, double inXOfs, double inZofs );

    void constructSprings( double inK, double inB, double shearK, double shearB, double flexK, double flexB );
    
    //setters
    void setStepSize( double inStepSize );

    //ParticleSystem takes ownership of pointer (will delete).
    void addForce( Force *inForce );
    void setSolver( NumericalSolver *inSolver );

    //getters
    Particle &getParticleAt( idx_t inX, idx_t inY );
    Particle &getParticleAt( idx_t index );
	idx_t getWidth(){ return mWidth; };
	idx_t getHeight(){ return mHeight; };

//----------------- storage --------------
public:
	SpringList& getSprings(){return mSprings;}
	bool IsMidPoint(){return mIsMidPoint;}
	void setIsMidPoint(bool bMidPoint){mIsMidPoint = bMidPoint;}
protected:
    idx_t            mWidth;
    idx_t            mHeight;
    Particle         *mParticles;
    SpringList       mSprings;
    ForceList        mForces;
    NumericalSolver  *mSolver;
	bool			 mIsMidPoint;
    double           mStepSize;
};

#endif //__PARTICLE_SYSTEM_H__
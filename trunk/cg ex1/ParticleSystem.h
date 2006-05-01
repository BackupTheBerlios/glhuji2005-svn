#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <vector>
#include "Spring.h"
#include "NumericalSolver.h"

using namespace std;

class Particle;
class Spring;

//class representing a particle system
class ParticleSystem
{
//-------------- internal types --------------
public:
    typedef vector<Spring>       SpringList;
    typedef SpringList::iterator SpringListIt;

public:
//-------------- internal classes --------------
    class ParticleInfo
    {
    public:
        ParticleInfo()
        {
            pIsPinned = false;
        }

        void pin()
        {
            pIsPinned = true;
        }

    public:
        bool pIsPinned;
    };

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

    void freeParticleStorage();

    //access to particle storage
    inline Vector3d     *getParticlePositions(){ return mParticlePos; }
    inline Vector3d     *getParticleVelocities(){ return mParticleVelocity; }
    inline double       *getParticleInvMasses(){ return mParticleInvMass; }
    inline ParticleInfo *getParticleInfo(){ return mParticleInfo; }
    
    //setters
	double getStepSize() {return mStepSize;}
    void setStepSize( double inStepSize );
    void setGravity( double inGravity );

    //ParticleSystem takes ownership of pointer (will delete).
    void setSolver( NumericalSolver *inSolver );

    //getters
    Vector3d    &getParticlePos( idx_t inX, idx_t inY );
    idx_t       getNumParticles();
	idx_t       getWidth(){ return mWidth; };
	idx_t       getHeight(){ return mHeight; };
    double      getGravity();
    SpringList& getSprings(){ return mSprings; }

//----------------- storage --------------
public:
    bool IsMidPoint(){return mIsMidPoint;}
    void setIsMidPoint(bool bMidPoint){mIsMidPoint = bMidPoint;}

protected:
    idx_t            mWidth;
    idx_t            mHeight;
    SpringList       mSprings;
    NumericalSolver  *mSolver;
	bool			 mIsMidPoint;
    double           mStepSize;
    double           mGravity;

    Vector3d         *mParticlePos;
    Vector3d         *mParticleVelocity;
    ParticleInfo     *mParticleInfo;
    double           *mParticleInvMass; // 1/m
};

#endif //__PARTICLE_SYSTEM_H__
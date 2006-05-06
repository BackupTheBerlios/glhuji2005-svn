#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <vector>
#include "Vector3d.h"
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

    void getSpatialDimensions( Vector3d &outCenter, double &outRadius );

    void addParticleAt( idx_t inX, idx_t inY, Particle &inParticle );

    void pinParticle( idx_t inX, idx_t inY );

    void autoCreateMesh( double inOriginX, double inOriginY, double inOriginZ,
                            double inMass, double inXOfs, double inZofs );

    void constructSprings( double inK, double inB, double shearK, double shearB, double flexK, double flexB );

	void calculateNormals();
	Vector3d calcTriangleNormal(Vector3d vertex1, Vector3d vertex2, Vector3d vertex3);

	void move( Vector3d direction );

    void freeParticleStorage();

    //access to particle storage
    inline Vector3d     *getParticlePositions(){ return mParticlePos; }
    inline Vector3d     *getParticleVelocities(){ return mParticleVelocity; }
    inline double       *getParticleInvMasses(){ return mParticleInvMass; }
    inline ParticleInfo *getParticleInfo(){ return mParticleInfo; }
    
    //setters
	double getStepSize() {return mStepSize;}
    void   setStepSize( double inStepSize );
    void   setGravity( double inGravity );
    void   setAirResistance( int8 inAirResistancePercent );

    //ParticleSystem takes ownership of pointer (will delete).
    void setSolver( NumericalSolver *inSolver );

    //getters
    Vector3d    &getParticlePos( idx_t inX, idx_t inY );
	Vector3d    &getParticleNormal( idx_t inX, idx_t inY );
    double      getStiffestSpring();
    idx_t       getNumParticles();
	idx_t       getWidth(){ return mWidth; };
	idx_t       getHeight(){ return mHeight; };
    double      getGravity();
    SpringList& getSprings(){ return mSprings; }
    double      getAirResistance(){ return mAirResistance; }

//----------------- storage --------------
public:
    bool IsMidPoint(){return mIsMidPoint;}
    void setIsMidPoint(bool bMidPoint){mIsMidPoint = bMidPoint;}
	void setWind (Vector3d WindDirection, Vector3d Wind, double WindMinLen, double WindMaxLen, double WindMaxChange)
	{
		mWindDirection = WindDirection;
		mWind = Wind;
		mWindMinLen = WindMinLen;
		mWindMaxLen = WindMaxLen;
		mWindMaxChange = WindMaxChange;
	}

	Vector3d getNewWind();

protected:
    idx_t            mWidth;
    idx_t            mHeight;
    SpringList       mSprings;
    NumericalSolver  *mSolver;
	bool			 mIsMidPoint;
    double           mStepSize;
    double           mGravity;
    double           mAirResistance;
    double           mStiffestSpring;

    Vector3d         *mParticlePos;
    Vector3d         *mParticleVelocity;
    ParticleInfo     *mParticleInfo;
    double           *mParticleInvMass; // 1/m

    Vector3d         mWindDirection;
    Vector3d         mWind;
    double	         mWindMinLen;
    double	         mWindMaxLen;
    double	         mWindMaxChange;

	Vector3d         *mFaceNormals;
	Vector3d         *mVertexNormals;

};

#endif //__PARTICLE_SYSTEM_H__
#pragma once
#include "ParticleSystem.h"

class CFlockParticleSystem :
	public CParticleSystem
{
public:
	CFlockParticleSystem(void);
	virtual ~CFlockParticleSystem(void);
	virtual bool calcNextFrame();
	virtual bool init();
	virtual bool InitFrame();
	virtual bool display(int nFrameNum, int nShading);
	virtual bool updateParticle(int nIdx);
	virtual bool getForces(int nIdx);
	bool calcAcceleration(int nIdx, Point3d &inIncA);
	virtual bool calculateVelocity(int nIdx);
	virtual bool calculatePosition(int nIdx);
	virtual Point3d getLookAtPoint();

    void setNumParticles(int inNumParticles) { mNumParticles = inNumParticles; }
    void setParticleSystemRadius( double inRadius ) { mParticleSystemRadius = inRadius; }
    void setParticleDistance( double inDist ) { mParticleDistance = inDist; }
    void setParticleMaxAcceleration( double inAccel ) { mParticleMaxAccelartion = inAccel; }
    void setUniformAccel( Point3d &inAccel ) { mUniformAccelaration = inAccel; }

    //input in radians
    void setParticleFOVAngle( double inAngle ) { mParticleFOVAngle = inAngle; }

protected:
    int     mNumParticles;
    double  mParticleMaxAccelartion;
    double  mParticleSystemRadius;
    double  mParticleDistance;
    double  mParticleFOVAngle;
    Point3d mUniformAccelaration;

};

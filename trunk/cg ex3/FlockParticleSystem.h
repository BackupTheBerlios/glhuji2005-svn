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
	virtual bool getAcceleration(int nIdx);
	virtual bool calculateVelocity(int nIdx);
	virtual bool calculatePosition(int nIdx);

    void setNumParticles(int inNumParticles) { mNumParticles = inNumParticles; }

protected:
    int     mNumParticles;

};

#pragma once
#include "particlesystem.h"

class CFireworksParticleSystem :
	public CParticleSystem
{
public:
	Point3d m_Gravity;
	Point3d m_Origin;

	CFireworksParticleSystem(void);
	virtual ~CFireworksParticleSystem(void);
	virtual bool calcNextFrame();
	virtual bool prevFrame();
	virtual bool gotoFrame(int nFrame);
	virtual bool init();
	virtual bool InitFrame();
	virtual bool display(int nFrameNum, int nShading);
	virtual bool updateParticle(int nIdx);
	virtual bool getForces(int nIdx);
	virtual bool getAcceleration(int nIdx);
	virtual bool calculateVelocity(int nIdx);
	virtual bool calculatePosition(int nIdx);
	virtual void killParticle(unsigned int nParticle);
};

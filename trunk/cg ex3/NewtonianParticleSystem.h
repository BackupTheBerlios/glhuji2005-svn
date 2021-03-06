#pragma once
#include "particlesystem.h"

class CNewtonianParticleSystem :
	public CParticleSystem
{
public:
	Point3d m_Gravity;
	double m_dHeading;
	double m_dHeadingStep;
	double m_dAccelerationRand;
	int	   m_nFramesDelay;

	CNewtonianParticleSystem(void);
	virtual ~CNewtonianParticleSystem(void);
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
};

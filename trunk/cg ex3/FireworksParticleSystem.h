#pragma once
#include "NewtonianParticleSystem.h"

class CFireworksParticleSystem :
	public CNewtonianParticleSystem
{
public:
	CFireworksParticleSystem(void);
	virtual ~CFireworksParticleSystem(void);
	virtual bool InitFrame();
	virtual bool updateParticle(int nIdx);
	virtual void killParticle(unsigned int nParticle);
};

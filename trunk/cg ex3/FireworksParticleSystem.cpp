#include "StdAfx.h"
#include "FireworksParticleSystem.h"

CFireworksParticleSystem::CFireworksParticleSystem(void) : 
CNewtonianParticleSystem()
{
	
}

CFireworksParticleSystem::~CFireworksParticleSystem(void)
{
}

//This is where little baby particles are made
bool CFireworksParticleSystem::InitFrame()
{
	static frames = 0;
	CParticle p;
	p.X = m_dDefaultOrigin;
	p.span = m_dDefaultSpan;
	p.lifepan = m_dDefaultLifespan;
	p.mass = m_dDefaultMass;
	p.persistance = m_dDefaultPersistance;
	p.alpha = m_dParticleAlpha;
	p.color = m_pParticleColor;
	p.shape = m_particleShape;
	p.size = m_pParticleSize;
	p.type = 1+round(frand()*2);
	static Point3d az(2.0,2.0,2.0);	
	if (frames == m_nParticlesPerFrame)
	{
		frames = 0;
		if (frand() < 0.7)
		{
			p.V = Point3d(0+frand()*M_PI/10, 0+frand()*M_PI/10, -1-frand()*M_PI/10);	//9 degree freedom in every axis rotation
			p.V *= 20 + (frand()-0.5)*6;
			AddParticle(p);
		}
	}
	else
		frames++;
	return true;
}

bool CFireworksParticleSystem::updateParticle(int nIdx)
{
	CParticle& p = (*m_pNewSystem)[nIdx];
	if (p.type == 0)
	{
		p.alpha = p.energy;
		p.energy *= 0.90;
	}
	return true;
}

#define MIN_CHILDREN 6
#define MAX_CHILDREN 24
void CFireworksParticleSystem::killParticle(unsigned int nParticle)
{
	const CParticle& oldp = (*m_pNewSystem)[nParticle];
	if (oldp.type > 0)
	{
		int nChildren = MIN_CHILDREN + round(frand()*(MAX_CHILDREN-MIN_CHILDREN));	//That's actually the square root of the number of children created
		CParticle p;
		p.X = oldp.X;
		p.V = oldp.V;
		p.lifepan = m_dDefaultLifespan/3;
		p.span = 0.0;
		p.energy = 1.0;
		p.persistance = 1;
		p.type = 0;
		p.color = Color3d(0.5+frand()*0.5,frand(),frand());
		p.shape = C_PARTICLESHAPE_DOT;
		p.alpha = 1.0;
		double headingy = 0;
		double headingz = 0;
		double expforce = 8.0 + frand()*8.0;
		if (oldp.type == 1)
			expforce *= 2;
		for (int zr=0; zr<nChildren; zr++)
		{
			for (int yr=0; yr<nChildren; yr++)
			{
				headingy = frand()*M_PI/10+(2.0*M_PI*(double)yr)/(double)nChildren;
				p.V = Point3d(0, 0, expforce);
				RotateYZ(headingy,headingz, p.V);
				AddParticle(p);
			}
			headingz = frand()*M_PI/10+(2.0*M_PI*(double)zr)/(double)nChildren;
		}
		if (oldp.type > 1)
		{
			CParticle p = oldp;
			p.type = oldp.type-1;
			p.span = 0.0;
			p.lifepan = 0.15*(frand()*10+1);
			p.mass = 0.0;
			p.shape = C_PARTICLESHAPE_DOT;
			p.persistance = 1.0;
			p.color = Color3d(0.0,0.0,0.0);
			p.alpha = 0.0;
			p.V = Point3d(0,0,0);
			AddParticle(p);
		}
	}
	CParticleSystem::killParticle(nParticle);
}

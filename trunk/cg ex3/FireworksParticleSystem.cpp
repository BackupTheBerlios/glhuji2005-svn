#include "StdAfx.h"
#include ".\FireworksParticleSystem.h"

CFireworksParticleSystem::CFireworksParticleSystem(void) : 
CParticleSystem(), 
m_Origin(0.0, 0.0, 0.0),
m_Gravity(0.0,-9.8,0.0)
{
	
}

CFireworksParticleSystem::~CFireworksParticleSystem(void)
{
}
bool CFireworksParticleSystem::calcNextFrame()
{
	bool rc = CParticleSystem::calcNextFrame();
	if (!rc)
		return false;
	rc = InitFrame();
	if (!rc)
		return false;
	
	m_pNewSystem->clear();
	int nCurParticle = 0;
	for (unsigned int i=0; i<m_pCurSystem->size(); i++)
	{
		m_pNewSystem->push_back((*m_pCurSystem)[i]);
		nCurParticle = (int)m_pNewSystem->size()-1;
		CParticle &particle = (*m_pNewSystem)[nCurParticle];
		if (isParticleDead(nCurParticle))
		{
			killParticle(nCurParticle);
			continue;
		}
		updateParticle(nCurParticle);		//Allows a chance to change particle mass, radius etc.

		particle.F = Point3d(0.0,0.0,0.0);
		getForces(nCurParticle);			//Get forces currently operating on the object

		particle.a = Point3d(0.0,0.0,0.0);
		getAcceleration(nCurParticle);		//Get any acceleration not dependant on mass

		calculateVelocity(nCurParticle);	//Calculate velocity (using acceleration and forces)

		calculatePosition(nCurParticle);	//Use velocity to calculate position
	}

	return true;
}

bool CFireworksParticleSystem::prevFrame()
{
	return false;
}

bool CFireworksParticleSystem::display(int nFrameNum, int nShading)
{
	return CParticleSystem::display(nFrameNum, nShading);
}

bool CFireworksParticleSystem::init()
{
	return true;
}

bool CFireworksParticleSystem::gotoFrame(int nFrame)
{
	if (nFrame == 0)
	{
		//ToDo: Restart simulation
	}
	else if (nFrame == m_nCurFrame)
	{
		return true;
	}
	else if (nFrame == m_nCurFrame+1)
	{
		return calcNextFrame();
	}
	else if (nFrame == m_nCurFrame-1)
	{
		return prevFrame();
	}
	else
	{
		//ToDo: Jump (if allowed)
	}
	return false;
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

bool CFireworksParticleSystem::getForces(int nIdx)
{
	return true;
}

bool CFireworksParticleSystem::getAcceleration(int nIdx)
{
	if ((*m_pNewSystem)[nIdx].mass > 0)
		(*m_pNewSystem)[nIdx].a += m_Gravity;
	return true;
}

bool CFireworksParticleSystem::calculateVelocity(int nIdx)
{
	(*m_pNewSystem)[nIdx].V += ((*m_pNewSystem)[nIdx].a + (*m_pNewSystem)[nIdx].F*(*m_pNewSystem)[nIdx].mass)*m_dt;
	return true;
}

bool CFireworksParticleSystem::calculatePosition(int nIdx)
{
	(*m_pNewSystem)[nIdx].X += (*m_pNewSystem)[nIdx].V * m_dt;
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

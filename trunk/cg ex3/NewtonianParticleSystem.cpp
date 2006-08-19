#include "StdAfx.h"
#include ".\newtonianparticlesystem.h"

inline double frand();
CNewtonianParticleSystem::CNewtonianParticleSystem(void) : CParticleSystem(), m_Gravity(0.0,-9.8,0.0)
{
	
}

CNewtonianParticleSystem::~CNewtonianParticleSystem(void)
{
}
bool CNewtonianParticleSystem::calcNextFrame()
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

bool CNewtonianParticleSystem::prevFrame()
{
	return false;
}

bool CNewtonianParticleSystem::display(int nFrameNum, int nShading)
{
	return CParticleSystem::display(nFrameNum, nShading);
}

bool CNewtonianParticleSystem::init()
{
	return true;
}

bool CNewtonianParticleSystem::gotoFrame(int nFrame)
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

bool CNewtonianParticleSystem::InitFrame()
{
	CParticle p;
	p.X = Point3d(0.0,5.0,0.0);
	p.span = 20;
	p.lifepan = 200;
	p.mass = 0.2;
	p.persistance = 1.0;
	static Point3d az(2.0,2.0,2.0);
	static double heading = 0.0;
	for (int i=0; i<10; i++)
	{
		p.V = Point3d(sin(heading)*3.0, 3.0, cos(heading)*3.0);
		p.V += Point3d(frand()*2.0 - 1.0,frand()*2.0,frand()*2.0 - 1.0);
		heading += 0.03;
		AddParticle(p);
	}
	return true;
}

bool CNewtonianParticleSystem::updateParticle(int nIdx)
{
	return true;
}

bool CNewtonianParticleSystem::getForces(int nIdx)
{
	return true;
}

bool CNewtonianParticleSystem::getAcceleration(int nIdx)
{
	(*m_pNewSystem)[nIdx].a += m_Gravity;
	return true;
}

bool CNewtonianParticleSystem::calculateVelocity(int nIdx)
{
	(*m_pNewSystem)[nIdx].V += ((*m_pNewSystem)[nIdx].a + (*m_pNewSystem)[nIdx].F*(*m_pNewSystem)[nIdx].mass)*m_dt;
	return true;
}

bool CNewtonianParticleSystem::calculatePosition(int nIdx)
{
	(*m_pNewSystem)[nIdx].X += (*m_pNewSystem)[nIdx].V * m_dt;
	return true;
}


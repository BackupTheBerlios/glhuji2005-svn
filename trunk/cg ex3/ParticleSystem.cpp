#include "StdAfx.h"
#include ".\particlesystem.h"

CParticleSystem::CParticleSystem(void)
{
	m_dt = 0.033;
	m_bUsingA = true;
	m_nParticlesPerFrame = 10;
	m_pCurSystem = &m_ParticlesA;
	m_pNewSystem = &m_ParticlesB;
}

CParticleSystem::~CParticleSystem(void)
{
}
bool CParticleSystem::calcNextFrame()
{
	m_nCurFrame++;
	if (m_bUsingA)
	{
		m_pCurSystem = &m_ParticlesA;
		m_pNewSystem = &m_ParticlesB;
	}
	else
	{
		m_pCurSystem = &m_ParticlesB;
		m_pNewSystem = &m_ParticlesA;
	}
	m_bUsingA = !m_bUsingA;
	return true;
}

inline double frand()
{
	return ((double)rand())/((double)RAND_MAX);
}

bool CParticleSystem::isParticleDead (int i)
{
	//Check dying of old age
	(*m_pNewSystem)[i].age += m_dt;
	if ((*m_pNewSystem)[i].age > (*m_pNewSystem)[i].lifepan)
		return true;

	//Check dying by chance
	if (frand() > (*m_pNewSystem)[i].persistance)
		return true;

	//Check dying by distance from birthplace
	if ((*m_pNewSystem)[i].span > 0 && 
		((*m_pNewSystem)[i].X-(*m_pNewSystem)[i].birthplace).length() > (*m_pNewSystem)[i].span)
		return true;
	return false;
}

bool CParticleSystem::prevFrame()
{
	return false;
}

bool CParticleSystem::display(int nFrameNum, int nShading)
{
	glPushMatrix();
Point3d a;
	for (unsigned int i=0; i<m_pNewSystem->size(); i++)
	{
		CParticle &particle = (*m_pNewSystem)[i];
//		glRotatef(particle.V[0],1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
//		glRotatef(particle.V[1],0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
//		glRotatef(particle.V[2],0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )

		// draw
		glColor3f(0,0,1);
		glPointSize(4.0);
		glBegin(GL_POINTS);
		glVertex3f(particle.X[0],particle.X[1],particle.X[2]);
		glEnd();
	}
	glPopMatrix();
	return true;
}

bool CParticleSystem::init()
{
	return true;
}
bool CParticleSystem::gotoFrame(int nFrame)
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


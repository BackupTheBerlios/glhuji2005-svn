#include "StdAfx.h"
#include ".\particlesystem.h"

CParticleSystem::CParticleSystem(void):
	m_pParticleSize(1,1,1),
	m_pParticleColor(0,0,1)
{
	m_dt = 0.033;
	m_bUsingA = true;
	m_nParticlesPerFrame = 10;
	m_pCurSystem = &m_ParticlesA;
	m_pNewSystem = &m_ParticlesB;

	m_dDefaultMass = 0.1; //[Kg]
	m_dDefaultRadius = 1; //[m]
	m_dDefaultSpan = 20; //[m]
	m_dDefaultLifespan = 7; //[sec]
	m_dDefaultPersistance = 1;
	ParticleShapeType m_particleShape = C_PARTICLESHAPE_DOT;	
	m_dParticleAlpha = 1;
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
	Point3d a;
	for (unsigned int i=0; i<m_pNewSystem->size(); i++)
	{
		glPushMatrix();
		CParticle &particle = (*m_pNewSystem)[i];
//		glRotatef(particle.V[0],1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
//		glRotatef(particle.V[1],0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
//		glRotatef(particle.V[2],0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )

		Point3d color = particle.color;
		Point3d size = particle.size;
		float alpha = (float)particle.alpha;
		
		// draw
		glColor3f(color[0], color[1], color[2]);
		GLfloat ambient[] = { color[0], color[1], color[2], alpha };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		switch (particle.shape){
			case C_PARTICLESHAPE_DOT:
				glPointSize(size[0]);		
				glBegin(GL_POINTS);		
				glVertex3f(particle.X[0],particle.X[1],particle.X[2]);
				glEnd();
				break;

			case C_PARTICLESHAPE_SPHERE:
				/*GLdouble boneLength = sqrt(pow(pPosition[0],2) + pow(pPosition[1],2) + pow(pPosition[2],2));	
				Vector3d v1(pPosition[0],pPosition[1],pPosition[2]);
				Vector3d v2(0,0,1);
				v1.normalize();
				Vector3d norm = v1%v2;
				double alfa1 = dot(v1,v2);
				double alfa = acos(alfa1)*RAD2DEG;
				glTranslated(pPosition[0]/2, pPosition[1]/2, pPosition[2]/2);
				glRotated(-alfa, norm[0], norm[1] ,norm[2]);*/
				glTranslatef(particle.X[0],particle.X[1],particle.X[2]);
				glScaled(size[0], size[1], size[2]);
				glutSolidSphere(1, 10, 10);
				break;

			case C_PARTICLESHAPE_CUBE:
				glTranslatef(particle.X[0],particle.X[1],particle.X[2]);
				glScaled(size[0], size[1], size[2]);
				glutSolidCube(1);
				break;

			case C_PARTICLESHAPE_CONE:
				/*glRotated(-alfa, norm[0], norm[1] ,norm[2]);
				GLdouble coneRadius = boneLength/5.0;*/
				glTranslatef(particle.X[0],particle.X[1],particle.X[2]);
				glScaled(size[0], size[1], size[2]);
				glRotated(-90, 1,0,0);	// rotate the top towards Y axis
				glutSolidCone(1, 0.5, 10, 10);
				break;

			default:
				break;
		}
		glPopMatrix();
	}	
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


#include "StdAfx.h"
#include "ParticleSystem.h"
#include "constants.h"
#include "SimulationsParams.h"
extern CSimulationsParams  g_simulationParams;

CParticleSystem::CParticleSystem(void):
    m_nCurFrame(0),
    mMaxParticleVelocity(100),
    m_pParticleSize(1,1,1),
	m_pParticleColor(0,0,1),
	m_pParticleColor2(0,0,1)
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
	m_dColorRandomness = 0;
	m_dParticleSizeRand = 0;
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

bool CParticleSystem::isParticleDead (int i)
{
	//Check dying of old age
	(*m_pNewSystem)[i].age += m_dt;
	if ((*m_pNewSystem)[i].lifepan >= 0 && 
		(*m_pNewSystem)[i].age > (*m_pNewSystem)[i].lifepan)
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
    unsigned int numParticles = (unsigned int)m_pNewSystem->size();

    //for faster drawing of teapot
    static GLuint teapotDisplayList;
    static bool teapotCreated = false;
	static bool drawnLines = false;
	bool drawLines = (nShading%2==1);
	if (drawLines != drawnLines){
		teapotCreated = false;
		drawnLines = drawLines;
	}
	for (unsigned int i=0; i<numParticles; i++)
	{
		glPushMatrix();
		CParticle &particle = (*m_pNewSystem)[i];

       	Point3d color = particle.getColor();
		Point3d size = particle.size;
		float alpha = (float)particle.alpha;

		// calculate rotation angle
		GLdouble lengthV = particle.V.norm();
		Vector3d v1(particle.V);
		Vector3d v2(0,0,1);
        
        //special case for particles with zero velocity...
        if (lengthV > 0){
            v1.normalize();
        }
        else{
            v1 = Point3d(0,1,0);
        }
		Vector3d norm = v1%v2;
		double angle1 = dot(v1,v2);
		double angle = acos(angle1)*RAD2DEG;
		
		// draw
		glColor4f(color[0], color[1], color[2], alpha);
		GLfloat ambient[] = { color[0], color[1], color[2], alpha };
		GLfloat diffuse[] = { color[0], color[1], color[2], alpha };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glTranslatef(particle.X[0],particle.X[1],particle.X[2]);
		glRotated(-angle, norm[0], norm[1] ,norm[2]);
		glScaled(size[0], size[1], size[2]);
		
		switch (particle.shape){
			case C_PARTICLESHAPE_DOT:
				glPointSize(size[0]);		
				glBegin(GL_POINTS);		
				glVertex3f(0,0,0);
				glEnd();
				break;

			case C_PARTICLESHAPE_SPHERE:
				if (drawLines)
					glutWireSphere(1, 10, 10);
				else
					glutSolidSphere(1, 10, 10);
				break;

			case C_PARTICLESHAPE_BALOON:
				if (drawLines)
					glutWireSphere(1,10,10);
				else
					glutSolidSphere(1, 10, 10);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(0,0,-5);
				glEnd();
				break;

			case C_PARTICLESHAPE_CONE:
				if (drawLines)
					glutWireCone(1,1,10,10);
				else
					glutSolidCone(1,1,10,10);
				break;

			case C_PARTICLESHAPE_LEAF:
				glRotated(45,0,1,0);
				if (drawLines)
				{
					glTranslated(-0.5,0,0);
					glutWireSphere(0.6,10,10);
					glTranslated(0,0,0.5);
					glutWireCube(1);
					glTranslated(0.5,0,0);
					glutWireSphere(0.6,10,10);
				}
				else
				{
					glTranslated(-0.5,0,0);
					glutSolidSphere(0.6,10,10);
					glTranslated(0,0,0.5);
					glutSolidCube(1);
					glTranslated(0.5,0,0);
					glutSolidSphere(0.6,10,10);
				}
				break;

			case C_PARTICLESHAPE_TEAPOT:
                //Create Display list on first time through, then just render display list...
                if( !teapotCreated )
                {
                    teapotCreated = true;
                    teapotDisplayList = glGenLists(1);
                    glNewList(teapotDisplayList,GL_COMPILE);

                    //scale teapot to be about same size as other particle types
					glRotated(-90,0,1,0);
                    if (drawLines)
						glutWireTeapot( size[0]*0.3 );
					else
						glutSolidTeapot( size[0]*0.3 );

                    glEndList();
                }

                glCallList(teapotDisplayList);
                
				break;

			case C_PARTICLESHAPE_ANT:
				drawAnt();
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

Point3d CParticleSystem::getLookAtPoint()
{
	unsigned int numParticles = (int)m_pCurSystem->size();
	Point3d centerOfMass(0,0,0);
    for( unsigned int j = 0; j < numParticles; j++ )
    {
        centerOfMass += (*m_pCurSystem)[j].X;
    }
    centerOfMass /= numParticles;
	return centerOfMass;
//	return g_simulationParams.m_cameraDir;
}

#define RAND_POINT Point3d(frand()-0.5, frand()-0.5, frand()-0.5)*0.2
void CParticleSystem::drawAnt()
{
	Point3d p;

	glScaled(1,1,1.2);
	glutSolidSphere(0.5, 10, 10);
	glTranslated(0,0,0.65);
	glutSolidSphere(0.2, 10, 10);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	p = Point3d(0.5,-0.5,0.5);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);
	glVertex3f(0,0,0);
	p = Point3d(0.5,-0.5,0);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);
	glVertex3f(0,0,0);
	p = Point3d(0.5,-0.5,-0.5);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);

	glVertex3f(0,0,0);
	p = Point3d(-0.5,-0.5,0.5);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);
	glVertex3f(0,0,0);
	p = Point3d(-0.5,-0.5,0);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);
	glVertex3f(0,0,0);
	p = Point3d(-0.5,-0.5,-0.5);
	p += RAND_POINT;
	glVertex3f(p[0],p[1],p[2]);
	glEnd();
	glTranslated(0,0,0.4);
	glutSolidSphere(0.3, 10, 10);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0.2,0.6,0.3);
	glVertex3f(0,0,0);
	glVertex3f(-0.2,0.6,0.3);
	glEnd();
}
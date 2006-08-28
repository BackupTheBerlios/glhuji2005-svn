#pragma once

class CParticleSystem;

class CSimulationsParams
{
public:
	CSimulationsParams():
		m_cameraPos(10,10,10),
		m_cameraDir(0,0,0),
		m_cameraUp(0,1,0),
		m_dFovY(30),
		m_dAspect(1),
		m_dZNear(1),
		m_dZFar(20000),
		m_particleSystem(NULL),
		m_clearColor(1,1,1),
        m_dT(0.1)
		{}

	~CSimulationsParams(void);

public:
	Vector3d m_cameraPos;
	Vector3d m_cameraDir;
	Vector3d m_cameraUp;
	Point3d  m_clearColor;
	double   m_dFovY;
	double   m_dAspect;
	double   m_dZNear;
	double   m_dZFar;
    double   m_dT;

	CParticleSystem* m_particleSystem;
};

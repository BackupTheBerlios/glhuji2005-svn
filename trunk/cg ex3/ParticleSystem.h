#pragma once
#include "constants.h"

class CParticle
{
public:
	CParticle() : 
	    m_bAlive(true),
		birthplace(0.0,0.0,0.0),
		type(0),
		X(0.0,0.0,0.0),
		a(0.0,0.0,0.0),
		V(0.0,0.0,0.0),
		F(0.0,0.0,0.0),
		mass(1.0),
		radius(0.01),
		span(-1),
		age(0.0),
		lifepan(-1.0),
		persistance(1.0),
		elasticity(1.0),
		energy(0.0),
		shape(C_PARTICLESHAPE_DOT),
		size(1,1,1),
		color(0,0,1),
		alpha(1)
	{
	}

	//The following 3 are autofilling on Add
	bool m_bAlive;
	double age;	//[sec]
	Point3d birthplace;	//X at birth
	int type; //Can mean anything - inside a specific system. defaults to 0

	Point3d X;	//position
	Point3d a;	//acceleration
	Point3d V;	//velocity
	Point3d F;	//Force
	double mass; //[Kg]
	double radius; //[m]
	double span; //[m]

	double lifepan;	//[sec]
	double persistance;	//0.0-1.0 - the chance of dying (per frame)
	double elasticity; //how much of the velocity is preserved when hitting a surface [0.0 - stop when hit anything, 1.0 - full elasticity]

	double energy; //how much internal energy does the particle have - isn't used - may be used by each system as needed

	ParticleShapeType shape;
	Point3d size;	//X,Y,Z dimensions
	Point3d color;	//RGB value
	double alpha;	//transparency
};

class CParticleSystem
{
protected:
    typedef vector<CParticle> ParticleList;
protected:
	int m_nCurFrame;
	bool m_bUsingA;
	double m_dt;
	ParticleList  m_ParticlesA;
	ParticleList  m_ParticlesB;
	ParticleList* m_pCurSystem;
	ParticleList* m_pNewSystem;

	// particle defaults
public: //TODO: fix this
	int               m_nParticlesPerFrame;
	double            m_dDefaultMass; //[Kg]
	double            m_dDefaultRadius; //[m]
	double            m_dDefaultSpan; //[m]

	double            m_dDefaultLifespan; //[sec]
	double            m_dDefaultPersistance; //0.0-1.0 - the chance of dying (per frame)

	Point3d           m_dDefaultOrigin;
	ParticleShapeType m_particleShape;
	Point3d           m_pParticleSize;	//X,Y,Z dimensions
	Point3d           m_pParticleColor;	//RGB value
	double			  m_dColorRandomness;
	double            m_dParticleAlpha;	//transparency
    double            mMaxParticleVelocity;

public:
	void AddParticle(CParticle particle)
	{
		particle.m_bAlive = true;
		particle.birthplace = particle.X;
		particle.age = 0.0;
		m_pCurSystem->push_back(particle);
	}
	CParticleSystem(void);
	virtual ~CParticleSystem(void);
	virtual bool calcNextFrame();
	virtual bool prevFrame();
	virtual bool gotoFrame(int nFrame);
	virtual bool init();
	virtual bool display(int nFrameNum, int nShading);
	virtual bool isParticleDead (int i);
	virtual void killParticle(unsigned int nParticle)
	{
		m_pNewSystem->erase(m_pNewSystem->begin()+nParticle);
	}
};

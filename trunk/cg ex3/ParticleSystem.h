#pragma once

class CParticle
{
public:
	CParticle() : 
	    m_bAlive(true),
		birthplace(0.0,0.0,0.0),
		X(0.0,0.0,0.0),
		a(0.0,0.0,0.0),
		V(0.0,0.0,0.0),
		F(0.0,0.0,0.0),
		mass(1.0),
		radius(0.01),
		span(-1),
		age(0.0),
		lifepan(-1.0),
		persistance(1.0)
	{
	}
	//The following 3 are autofilling on Add
	bool m_bAlive;
	double age;	//[sec]
	Point3d birthplace;	//X at birth

	Point3d X;	//position
	Point3d a;	//acceleration
	Point3d V;	//velocity
	Point3d F;	//Force
	double mass; //[Kg]
	double radius; //[m]
	double span; //[m]

	double lifepan;	//[sec]
	double persistance;	//0.0-1.0 - the chance of dying (per frame)
};

class CParticleSystem
{
protected:
	int m_nCurFrame;
	bool m_bUsingA;
	double m_dt;
	vector<CParticle> m_ParticlesA;
	vector<CParticle> m_ParticlesB;
	vector<CParticle>* m_pCurSystem;
	vector<CParticle>* m_pNewSystem;
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

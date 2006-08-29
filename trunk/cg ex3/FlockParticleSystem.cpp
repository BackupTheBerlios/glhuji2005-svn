#include "StdAfx.h"
#include "FlockParticleSystem.h"

CFlockParticleSystem::CFlockParticleSystem(void) : 
CParticleSystem(),
mNumParticles(0)
{
	
}

CFlockParticleSystem::~CFlockParticleSystem(void)
{
}

bool 
CFlockParticleSystem::calcNextFrame()
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
		/*nCurParticle = (int)m_pNewSystem->size()-1;
		CParticle &particle = (*m_pNewSystem)[nCurParticle];
		
        updateParticle(nCurParticle);		//Allows a chance to change particle mass, radius etc.

		particle.F = Point3d(0.0,0.0,0.0);
		getForces(nCurParticle);			//Get forces currently operating on the object

		particle.a = Point3d(0.0,0.0,0.0);
		getAcceleration(nCurParticle);		//Get any acceleration not dependant on mass

		calculateVelocity(nCurParticle);	//Calculate velocity (using acceleration and forces)

		calculatePosition(nCurParticle);	//Use velocity to calculate position*/
	}

	return true;
}

bool CFlockParticleSystem::display(int nFrameNum, int nShading)
{
	return CParticleSystem::display(nFrameNum, nShading);
}

bool CFlockParticleSystem::init()
{
	return true;
}

bool CFlockParticleSystem::InitFrame()
{
    //only create particles on first frame...
    if( m_nCurFrame == 1 )
    {
        CParticle p;
        p.X           = Point3d(0,0,0);
        p.span        = m_dDefaultSpan;
        p.lifepan     = m_dDefaultLifespan;
        p.mass        = m_dDefaultMass;
        p.persistance = m_dDefaultPersistance;
        p.alpha       = m_dParticleAlpha;
        p.color       = Point3d(1,0,0);
        p.shape       = m_particleShape;
        p.size        = Point3d(0.2,0.2, 0.2);
        AddParticle(p);

	    p.span        = m_dDefaultSpan;
	    p.lifepan     = m_dDefaultLifespan;
	    p.mass        = m_dDefaultMass;
	    p.persistance = m_dDefaultPersistance;
	    p.alpha       = m_dParticleAlpha;
	    p.color       = m_pParticleColor + 
			Point3d(frand()-0.5, frand()-0.5, frand()-0.5) * m_dColorRandomness;
	    p.shape       = m_particleShape;
	    p.size        = m_pParticleSize;

	    for (int i=0; i<mNumParticles; i++)
	    {
            //todo: read distribution from ini file

            p.color[0] = frand();
            p.color[1] = frand();
            p.color[2] = frand();

            //distribute particles randomly
            p.X[0] = m_dDefaultOrigin[0] + 10.0*(frand()-0.5);
            p.X[1] = m_dDefaultOrigin[1] + 10.0*(frand()-0.5);
            p.X[2] = m_dDefaultOrigin[2] + 10.0*(frand()-0.5);

		    AddParticle(p);
	    }
    }

	return true;
}

bool CFlockParticleSystem::updateParticle(int nIdx)
{
	return true;
}

bool CFlockParticleSystem::getForces(int nIdx)
{
	return true;
}

bool CFlockParticleSystem::getAcceleration(int nIdx)
{
	//if ((*m_pNewSystem)[nIdx].mass > 0)
	//	(*m_pNewSystem)[nIdx].a += m_Gravity;
	return true;
}

bool CFlockParticleSystem::calculateVelocity(int nIdx)
{
	(*m_pNewSystem)[nIdx].V += ((*m_pNewSystem)[nIdx].a + (*m_pNewSystem)[nIdx].F*(*m_pNewSystem)[nIdx].mass)*m_dt;
	return true;
}

bool CFlockParticleSystem::calculatePosition(int nIdx)
{
	(*m_pNewSystem)[nIdx].X += (*m_pNewSystem)[nIdx].V * m_dt;
	return true;
}


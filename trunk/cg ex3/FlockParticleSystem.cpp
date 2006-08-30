#include "StdAfx.h"
#include "FlockParticleSystem.h"

CFlockParticleSystem::CFlockParticleSystem(void) : 
CParticleSystem(),
mNumParticles(0),
mParticleSystemRadius(0),
mParticleDistance(0),
mParticleFOVAngle(0)
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
    unsigned int numParticles = (int)m_pCurSystem->size();

    //0. copy over all particles as is to new system.
    for (unsigned int i=0; i<numParticles; i++)
    {
	    m_pNewSystem->push_back((*m_pCurSystem)[i]);
        CParticle &aParticle = (*m_pCurSystem)[i];
        CParticle &bParticle = (*m_pNewSystem)[i];
        CParticle &theParticle1 = (*m_pNewSystem)[i];
    }

    //--------- Move all particles ------

    
    for (unsigned int i=0; i<numParticles; i++)
    {
        Point3d centerOfMass(0,0,0);
        Point3d v1(0,0,0), v2(0,0,0), v3(0,0,0);
        Point3d avgNeighbourVelocity(0,0,0);

        CParticle &curParticle = (*m_pNewSystem)[i];

        int numParticlesInFOV = 0;
        for( unsigned int j = 0; j < numParticles; j++ )
        {
            CParticle &theParticle = (*m_pCurSystem)[j];
            //only calculate using positions of other particles
            if( j == i) continue;

            //-------------- Check if this particle is within the current particles field of vision ----------
            //vector that points from the center of the current particle to the center
            //of the particle we're examining
            Vector3d tmp = theParticle.X - curParticle.X;

            double distBetweenParticles = sqrt( tmp[0]*tmp[0]+tmp[1]*tmp[1]+tmp[2]*tmp[2] );

            //relative angle between curParticle and this particle
            Point3d a(0,0,0), b(0,0,0);

            if( curParticle.V.norm() > 0 )
                a = curParticle.V/curParticle.V.norm();
            
            if( tmp.norm() > 0 )
                b = tmp/tmp.norm();

            double relativeAngle = acos( dot(a, b ) );

            //particles can only see other particles if both are close enough together and one
            //particle is within the angel of another
            if( !(distBetweenParticles < mParticleDistance && (abs(relativeAngle) <= mParticleFOVAngle)) )
                continue;

            //---------- This particle is within our field of view ---------- 
            numParticlesInFOV++;
            centerOfMass         += theParticle.X;   //rule 1 - try to fly towards center of mass
            v2                   -= tmp;             //rule 2 - try to avoid other boids
            avgNeighbourVelocity += theParticle.V;   //rule 3 - calc average neighbour velocity
        }

        
        //---------- Calculate New Velocity ---------- 
        //if there are no other particles visible to this particle, then theres
        //no point in doing these calculation.
        if( numParticlesInFOV > 0 )
        {
            //1. Rule 1 - move particles towards center of mass of other particles
            //calc center of mass of particle cloud
            centerOfMass /= numParticlesInFOV;
            v1 = (centerOfMass-curParticle.X)/100.0;

            //Rule 3 - try to keep velocity similar to nearby neighbours
            avgNeighbourVelocity /= numParticlesInFOV;
            v3 = (avgNeighbourVelocity-curParticle.V)/8.0;
        }

        Point3d incV = v1 + v2 + v3;

        calculateVelocity(i, incV);
        calculatePosition(i);

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
        p.color       = Point3d(0,1,0);
        p.shape       = m_particleShape;
        p.size        = m_pParticleSize;
        p.V           = Point3d(1,0,0);
        AddParticle(p);

	    p.span        = m_dDefaultSpan;
	    p.lifepan     = m_dDefaultLifespan;
	    p.mass        = m_dDefaultMass;
	    p.persistance = m_dDefaultPersistance;
	    p.alpha       = m_dParticleAlpha;
		Point3d colorRand = Point3d(frand()-0.5, frand()-0.5, frand()-0.5) * m_dColorRandomness;
		p.color = m_pParticleColor + colorRand;
		p.color2 = m_pParticleColor2 + colorRand;
	    p.shape       = m_particleShape;
	    p.size        = m_pParticleSize;

	    for (int i=0; i<mNumParticles; i++)
	    {
            //todo: read distribution from ini file

            p.color[0] = frand();
            p.color[1] = frand();
            p.color[2] = frand();

            p.V = Point3d(1,0,0);

            //distribute particles randomly
            p.X[0] = m_dDefaultOrigin[0] + 30.0*(frand()-0.5);
            p.X[1] = m_dDefaultOrigin[1] + 30.0*(frand()-0.5);
            p.X[2] = m_dDefaultOrigin[2] + 30.0*(frand()-0.5);

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

bool CFlockParticleSystem::calculateVelocity(int nIdx, Point3d &inIncV)
{
    CParticle &curParticle = (*m_pNewSystem)[nIdx];

    //separate added velocity into 2 components
    Point3d vDirection = curParticle.V/curParticle.V.norm();

    Point3d incVSameDir = vDirection*dot(inIncV,vDirection);
    Point3d incVOtherDir = inIncV - incVSameDir;

    //finaly calc this particles new velocity
    curParticle.V += incVSameDir + incVOtherDir*0.5;

    //calculate new position
    Point3d newPosition = curParticle.X + curParticle.V * m_dt;
    //check if particle is outside allowable radius, if so - bring it back towards the origin
    Point3d vecToOrigin = (m_dDefaultOrigin-newPosition);
    double  distFromOrigin = vecToOrigin.norm();
    if( distFromOrigin > mParticleSystemRadius )
    {
        //cancel velocity in "bad" direction
        curParticle.V -= vecToOrigin*dot( vecToOrigin, curParticle.V );
    }

    //limit particle velocity
    double velocity = curParticle.V.norm();
    if( velocity > mMaxParticleVelocity )
        curParticle.V *= mMaxParticleVelocity / velocity;

	return true;
}

bool CFlockParticleSystem::calculatePosition(int nIdx)
{
    CParticle &curParticle = (*m_pNewSystem)[nIdx];

	curParticle.X += curParticle.V * m_dt;
	return true;
}


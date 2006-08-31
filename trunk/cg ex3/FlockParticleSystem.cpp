#include "StdAfx.h"
#include "FlockParticleSystem.h"

CFlockParticleSystem::CFlockParticleSystem(void) : 
CParticleSystem(),
mNumParticles(0),
mParticleSystemRadius(0),
mParticleDistance(0),
mParticleFOVAngle(0),
mParticleMaxAccelartion(0),
mUniformAccelaration(0,0,0),
mLockY(0)
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
    //calculate COI on the way...
    Point3d particleSystemCenterOfMass(0,0,0);
    for (unsigned int i=0; i<numParticles; i++)
    {
        CParticle &theParticle = (*m_pCurSystem)[i];
	    m_pNewSystem->push_back(theParticle);
        particleSystemCenterOfMass += theParticle.X;
    }

    particleSystemCenterOfMass /= numParticles;
    
    for (unsigned int i=0; i<numParticles; i++)
    {
        Point3d centerOfMass(0,0,0);
        Point3d a1(0,0,0), a2(0,0,0), a3(0,0,0);
        Point3d avgNeighbourVelocity(0,0,0);

        CParticle &curParticle = (*m_pNewSystem)[i];

        int numParticlesInFOV = 0;
        for( unsigned int j = 0; j < numParticles; j++ )
        {
            CParticle &theParticle = (*m_pCurSystem)[j];
            //only calculate using positions of other particles
            if( j == i) continue;

            centerOfMass         += theParticle.X;   //rule 1 - try to fly towards center of mass

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
            a2                   -= tmp/3;             //rule 2 - try to avoid other boids
            avgNeighbourVelocity += theParticle.V;   //rule 3 - calc average neighbour velocity
        }
      
        //---------- Calculate New Velocity ---------- 

        //1. Rule 1 - move particles towards center of mass of other particles
        //calc center of mass of particle cloud
        centerOfMass /= numParticles;
        a1 = (centerOfMass-curParticle.X)/100.0;

        //if there are no other particles visible to this particle, then theres
        //no point in doing these calculation.
        if( numParticlesInFOV > 0 )
        {
            //Rule 3 - try to keep velocity similar to nearby neighbours
            avgNeighbourVelocity /= numParticlesInFOV;
            a3 = (avgNeighbourVelocity-curParticle.V)/8.0;
        }

        Point3d incA = a1 + a2 + a3+mUniformAccelaration;

        calcAcceleration(i, incA);
        calculateVelocity(i);
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
	int neededParticles = 0;
	Point3d velocity = Point3d(1,0,0);

    if( m_nCurFrame == 1 )
    {
		velocity = Point3d(7,7,0);
		neededParticles = 1;
	}
	else if ( m_nCurFrame%10==0 && m_nCurFrame < 10*mNumParticles )
    {
		velocity = Point3d(1,0,0);
		neededParticles = 1;
	}

	if (neededParticles > 0)
	{
        CParticle p;

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

	    for (int i=0; i<neededParticles; i++)
	    {
            //todo: read distribution from ini file

			Point3d colorRand = Point3d(frand()-0.5, frand()-0.5, frand()-0.5) * m_dColorRandomness;
			p.color = m_pParticleColor + colorRand;
			p.color2 = p.color;

			p.V = velocity;

            //distribute particles randomly
            p.X[0] = m_dDefaultOrigin[0] + 30.0*(frand()-0.5);
			p.X[1] = (mLockY != 0)? 0 : m_dDefaultOrigin[1] + 30.0*(frand()-0.5);
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

bool CFlockParticleSystem::calcAcceleration(int nIdx, Point3d &inIncA)
{     
	CParticle &curParticle = (*m_pNewSystem)[nIdx];
	if (nIdx == 0){
		if (frand() > 0.5){
			// once in a while add random acceleration
			curParticle.a += Point3d(frand()-0.5,frand()-0.5,frand()-0.5);
			// but make sure we don't run too far from the origin
			Point3d dist = m_dDefaultOrigin - curParticle.X;
			curParticle.a += dist*frand()*0.05;
		}
		else{
			// otherwise just decrease acceleration
			curParticle.a *= 0.6;
		}
	}
	else{
		//----------- Calc new accelaration -----------
		curParticle.a += inIncA;

		//----------- go towards the leader -----------
		Point3d dist = (*m_pNewSystem)[0].X - curParticle.X;
			curParticle.a += dist*frand()*0.05;

		//----------- limit accelaration -----------
		if( curParticle.a.norm() > mParticleMaxAccelartion )
			curParticle.a *= mParticleMaxAccelartion/curParticle.a.norm();
	}

	return true;
}

bool CFlockParticleSystem::calculateVelocity(int nIdx)
{
    CParticle &curParticle = (*m_pNewSystem)[nIdx];
	double maxVelocity = mMaxParticleVelocity;

    curParticle.V += curParticle.a * m_dt;

    //---------------- Make sure particles don't stray outside the particle system's radius -----------
	if ( nIdx == 0 ){
		maxVelocity += 3;
	}
	else if ( mParticleSystemRadius != 0 )
    {
        //calculate position at t+1
        Point3d newPosition = curParticle.X + curParticle.V * m_dt;
        //check if particle is outside allowable radius, if so - bring it back towards the origin
        Point3d vecToOrigin = (m_dDefaultOrigin-newPosition);
        double  distFromOrigin = vecToOrigin.norm();
        if( distFromOrigin > mParticleSystemRadius )
        {
            //cancel velocity and acceleration in "bad" direction
            curParticle.V -= vecToOrigin*dot( vecToOrigin, curParticle.V );
            curParticle.a -= vecToOrigin*dot( vecToOrigin, curParticle.a );
        }
    }

    //------------ limit particle velocity -------------
    double velocity = curParticle.V.norm();
    if( velocity > mMaxParticleVelocity )
        curParticle.V *= mMaxParticleVelocity / velocity;

	if (mLockY != 0)
		curParticle.V[1] = 0;
	return true;
}

bool CFlockParticleSystem::calculatePosition(int nIdx)
{
    CParticle &curParticle = (*m_pNewSystem)[nIdx];

	curParticle.X += curParticle.V * m_dt;
	return true;
}

Point3d CFlockParticleSystem::getLookAtPoint()
{
	unsigned int numParticles = (int)m_pCurSystem->size();
	Point3d centerOfMass(0,0,0);
    for( unsigned int j = 0; j < numParticles; j++ )
    {
        centerOfMass += (*m_pCurSystem)[j].X;
    }
    centerOfMass /= numParticles;
	return centerOfMass;
}
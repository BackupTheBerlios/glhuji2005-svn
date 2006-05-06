#include "StdAfx.h"

#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "constants.h"
#include "NumericalSolver.h"

#include "Particlesystem.h"

ParticleSystem::ParticleSystem(  )
{
    mWidth      = 0;
    mHeight     = 0;
    mIsMidPoint = false;
    mSolver     = NULL;
    mStepSize   = 0;
    mStiffestSpring = 0;

    mGravity       = 0;
    mAirResistance = 0;

	mWindDirection = Vector3d(0,0,0);
    mWind = Vector3d(0,0,0);
    mWindMinLen = 0.0;
    mWindMaxLen = 0.0;
    mWindMaxChange = 0.0;

    mParticlePos         = NULL;
    mParticleVelocity    = NULL;
    mParticleInvMass     = NULL;
    mParticleInfo        = NULL;
}

void 
ParticleSystem::step()
{
	mSolver->step( mStepSize );
}

void
ParticleSystem::setStepSize( double inStepSize )
{
    mStepSize = inStepSize;
}

void 
ParticleSystem::setDimensions( idx_t inMeshWidth, idx_t inMeshHeight )
{
    //sanity
    assert( inMeshWidth > 0 && inMeshHeight > 0 );

    freeParticleStorage();

    //set dimensions
    mWidth        = inMeshWidth;
    mHeight       = inMeshHeight;
    
    //allocate storage
    idx_t numParticles = mWidth * mHeight;
    mParticleInfo      = new ParticleInfo[numParticles];
    mParticlePos       = new Vector3d[numParticles];
    mParticleInvMass   = new double[numParticles];
    mParticleVelocity  = new Vector3d[numParticles];
}

void 
ParticleSystem::autoCreateMesh( double inOriginX, double inOriginY, double inOriginZ,
                    double inMass, double inXOfs, double inZofs )
{
    double xC = inOriginX;
    double yC = inOriginY;
    double zC = inOriginZ;

    for( idx_t yOfs = 0; yOfs < mHeight; yOfs++ ) {
        for( idx_t xOfs = 0; xOfs < mWidth; xOfs++ ) {

            Particle p( xC, yC, zC, inMass, false );
            addParticleAt( xOfs, yOfs, p );
            
            xC += inXOfs;
        }
        xC = inOriginX;
        zC += inZofs;
    }
}

void 
ParticleSystem::constructSprings( double inK, double inB, double shearK, double shearB, double flexK, double flexB )
{
    //set stiffest spring constant
    mStiffestSpring = max( inK, max(shearK, flexK) );

    //useful macro for calculating indexes
    #define IDX(u,v) ( (u)+((v)*mWidth) )

    //----- add 4-connectivity springs ----
    for( idx_t y = 0; y < mHeight; y++ )
        for( idx_t x = 0; x < mWidth; x++ )
        {
            //connect spring to the right on all except for rightmost particle
            if( x!= mWidth-1)
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x+1,y);
                Vector3d &p1V = mParticlePos[a];
                Vector3d &p2V = mParticlePos[b];
                double dist = (p2V-p1V).length();
				if (dist < 0)
					dist *= -1;
                //double dist = 0.5;

                mSprings.push_back( Spring( a, b, dist, inK, inB ) );
            }

            //connect springs down for all except bottom row
            if( y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x,y+1);
                Vector3d p1V = mParticlePos[a];
                Vector3d p2V = mParticlePos[b];
                double dist = (p2V-p1V).length();
				if (dist < 0)
					dist *= -1;
                //double dist = 0.5;

                mSprings.push_back( Spring( a, b, dist, inK, inB ) );
            }

			//Add backslashed Shear springs for all but rightmost particles and bottom row
            if( x!= mWidth-1 && y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x+1,y+1);
                Vector3d &p1V = mParticlePos[a];
                Vector3d &p2V = mParticlePos[b];
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, shearK, shearB ) );
            }
			//Add slashed Shear springs for all but leftmost particles and bottom row
            if( x != 0 && y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x-1,y+1);
                Vector3d &p1V = mParticlePos[a];
                Vector3d &p2V = mParticlePos[b];
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, shearK, shearB ) );
            }


			//Add flexion springs for all but 2 rightmost particles
            if( x < mWidth-2 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x+2,y);
                Vector3d &p1V = mParticlePos[a];
                Vector3d &p2V = mParticlePos[b];
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, flexK, flexB ) );
            }
			//Add flexion springs for all but 2 bottommost rows
            if( y < mHeight-2 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x,y+2);
                Vector3d &p1V = mParticlePos[a];
                Vector3d &p2V = mParticlePos[b];
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, flexK, flexB ) );
            }
        }

    #undef IDX
}

ParticleSystem::~ParticleSystem()
{
    freeParticleStorage();

    if( mSolver != NULL )
        delete mSolver;
}

void
ParticleSystem::freeParticleStorage()
{
    if( mParticlePos != NULL )
    {
        delete [] mParticlePos;
        mParticlePos = NULL;
    }

    if( mParticleVelocity != NULL )
    {
        delete [] mParticleVelocity;
        mParticleVelocity = NULL;
    }

    if( mParticleInvMass != NULL )
    {
        delete [] mParticleInvMass;
        mParticleInvMass = NULL;
    }

    if( mParticleInfo != NULL )
    {
        delete [] mParticleInfo;
        mParticleInfo    = NULL;
    }
}

void 
ParticleSystem::getSpatialDimensions( Vector3d &outCenter, double &outRadius )
{
    idx_t numParticles = getNumParticles();

    //------------ Find Center Of Mass -------------
    Vector3d center;
    center.print();
    for( idx_t i = 0; i < numParticles; i++ )
    {
        center += mParticlePos[i];
    }
    center = center / numParticles;
    center.print();

    outCenter = center;

    //------------ Find Radius -------------
    double dist = 0;

    for( idx_t i = 0; i < numParticles; i++ )
    {
        double tmp = abs( center.dist(mParticlePos[i]) );

        if( tmp > dist )
            dist = tmp;
    }

    outRadius = dist;

}

void 
ParticleSystem::addParticleAt( idx_t inX, idx_t inY, Particle &inParticle )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    idx_t i = inX + (inY*mWidth);

    //assign
    mParticlePos[ i ]         = inParticle.getPos();
    mParticleInvMass[ i ]     = 1.0 / inParticle.getMass();
    mParticleInfo[i]          = ParticleInfo();
    mParticleVelocity[i]      = inParticle.getVelocity();

    if( inParticle.isPinned() )
        mParticleInfo[i].pin();

}

Vector3d    &
ParticleSystem::getParticlePos( idx_t inX, idx_t inY )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    return mParticlePos[ inX + (inY*mWidth) ];
}

idx_t    
ParticleSystem::getNumParticles()
{
    return mWidth*mHeight;
}

void 
ParticleSystem::pinParticle( idx_t inX, idx_t inY )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    mParticleInfo[ inX + (inY*mWidth) ].pin();
}

void 
ParticleSystem::setGravity( double inGravity )
{
    mGravity = inGravity;
}

void   
ParticleSystem::setAirResistance( int8 inAirResistancePercent )
{
    //air resistance specified in percent
    mAirResistance = 1-(0.01*inAirResistancePercent);
}

double
ParticleSystem::getGravity()
{
    return mGravity;
}

double
ParticleSystem::getStiffestSpring()
{
    return mStiffestSpring;
}

void 
ParticleSystem::setSolver( NumericalSolver *inSolver )
{
    if( mSolver != NULL )
        delete mSolver;

    mSolver = inSolver;
    mSolver->attachToParticleSystem( this, mIsMidPoint );
}

Vector3d
ParticleSystem::getNewWind()
{
	double change = mWindMaxChange*(1.0-double(rand())/double(RAND_MAX));
	mWind += mWindDirection*change;
	double WindLen = mWind.length();
	if (WindLen > mWindMaxLen)
		mWind = mWindDirection*mWindMaxLen;
	else if (WindLen < mWindMinLen)
		mWind = mWindDirection*mWindMinLen;
	return mWind;
}

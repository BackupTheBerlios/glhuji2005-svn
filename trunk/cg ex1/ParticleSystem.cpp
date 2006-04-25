#include "StdAfx.h"

#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "NumericalSolver.h"

#include "Particlesystem.h"

ParticleSystem::ParticleSystem(  )
{
    mWidth  = 0;
    mHeight = 0;
    mParticles = 0;
	mSolverType = C_FORWARD_EULER_SOLVER;
}

void 
ParticleSystem::step( double h )
{
	mSolver->step(h);/*
    Vector3d gravity( 0, 0, -9.81 );

    //clear force vector for particles and add gravity
    for( int i = 0; i < (mWidth * mHeight); i++ )
        mParticles[i].getForce() = gravity;

    //iterate over all springs and sum force on each particle
    for( SpringListIt it = mSprings.begin(); it != mSprings.end(); it++ )
    {
        Spring &theSpring = *it;
    }

    //add other forces (gravity)

    //run solver on all particles
*/
}

void 
ParticleSystem::setDimensions( idx_t inMeshWidth, idx_t inMeshHeight )
{
    //sanity
    assert( inMeshWidth > 0 && inMeshHeight > 0 );

    //free allocated memory
    if( mParticles != 0 )
    {
        delete [] mParticles;
        mParticles = 0;
    }

    //set
    mWidth = inMeshWidth;
    mHeight = inMeshHeight;
    mParticles = new Particle[ mWidth * mHeight ];
}

void 
ParticleSystem::constructSprings( double inK, double inB, double shearK, double shearB, double flexK, double flexB )
{
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
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = 0.5;

                mSprings.push_back( Spring( a, b, dist, inK, inB ) );
            }

            //connect springs down for all except bottom row
            if( y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x,y+1);
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = 0.5;

                mSprings.push_back( Spring( a, b, dist, inK, inB ) );
            }

			//Add backslashed Shear springs for all but rightmost particles and bottom row
            if( x!= mWidth-1 && y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x+1,y+1);
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, shearK, shearB ) );
            }
			//Add slashed Shear springs for all but leftmost particles and bottom row
            if( x != 0 && y != mHeight-1 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x-1,y+1);
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, shearK, shearB ) );
            }


			//Add flexion springs for all but 2 rightmost particles
            if( x < mWidth-2 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x+2,y);
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, flexK, flexB ) );
            }
			//Add flexion springs for all but 2 bottommost rows
            if( y < mHeight-2 )
            {
                idx_t a = IDX(x,y);
                idx_t b = IDX(x,y+2);
                Vector3d p1V = mParticles[a].getPos();
                Vector3d p2V = mParticles[b].getPos();
                //double dist = abs((p2V-p1V).length());
                double dist = (p1V - p2V).length();	//Set rest distance as current distance

                mSprings.push_back( Spring( a, b, dist, flexK, flexB ) );
            }
        }

    #undef IDX
}

ParticleSystem::~ParticleSystem()
{
    delete[] mParticles;
    mParticles = 0;

    for( ForceList::iterator it = mForces.begin(); it != mForces.end(); it++ )
    {
        //delete force pointer
        delete (*it);
    }

    mForces.clear();
}

void 
ParticleSystem::addParticleAt( idx_t inX, idx_t inY, Particle &inParticle )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    //assign
    mParticles[ inX + (inY*mWidth) ] = inParticle;
}

void 
ParticleSystem::addForce( Force *inForce )
{
    mForces.push_back( inForce );
}

Particle &
ParticleSystem::getParticleAt( idx_t index )
{
    //sanity, check we're inside mesh bounds...
    assert( (index < mWidth*mHeight) && (index >= 0) );

    return mParticles[ index ];
}

Particle &
ParticleSystem::getParticleAt( idx_t inX, idx_t inY )
{
    //sanity, check we're inside mesh bounds...
    assert( (inX < mWidth) && (inY < mHeight) );

    return mParticles[ inX + (inY*mWidth) ];
}

void 
ParticleSystem::setSolver( NumericalSolver *inSolver )
{
    mSolver = inSolver;
}
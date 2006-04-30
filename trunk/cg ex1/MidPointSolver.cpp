#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "MidPointSolver.h"

//todo: cleanup - no need to copy so many arrays around...
void 
MidPointSolver::step( double h )
{
    int numParticles     = mParticleSystem->getNumParticles();
    double halfH         = h / 2.0;

    double   *masses       = new double[ numParticles ];
    Vector3d *forces1      = new Vector3d[ numParticles ];
    Vector3d *origPos      = new Vector3d[ numParticles ];
    Vector3d *origV        = new Vector3d[ numParticles ];
    Vector3d *velocities2  = new Vector3d[ numParticles ];
    Vector3d *pos2         = new Vector3d[ numParticles ];

    //---------- Store Away Particle Positions ----
    for( int i = 0; i < numParticles; i++ )
    {
        Particle &p    = mParticleSystem->getParticleAt( i );
        origPos[i]     = p.getPos();
        origV[i]       = p.velocity();
        masses[i]      = p.getMass();
    }

    //---------- 1. calculate da at x,v ----------
    //calculate k1
    calcAccel( numParticles, origPos, origV, masses, forces1 );

    //---------- 2. calculate x & v at midpoint ----------
    for( int i = 0; i < numParticles; i++ )
    {
        velocities2[i] = origV[i]   + (forces1[i] * halfH);
        pos2[i]        = origPos[i] + (velocities2[i] * halfH);
    }

    //---------- 3. calculate dA at Midpoint ----------
    calcAccel( numParticles, pos2, velocities2, masses, forces1 );

    //---------- 4. set x & v of particles based on 3 ----------

    //Set particle position
    for( int i = 0; i < numParticles; i++ )
    {
        if( mParticleSystem->getParticleAt( i ).isPinned() )
            continue;

        Vector3d &newV = mParticleSystem->getParticleAt( i ).velocity();

        newV += (forces1[i] * h);

        mParticleSystem->getParticleAt( i ).pos() += newV * h;
    }

    delete [] masses;
    delete [] forces1;
    delete [] pos2;
    delete [] velocities2;
    delete [] origPos;
    delete [] origV;

}
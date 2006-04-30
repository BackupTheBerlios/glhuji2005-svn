#include "StdAfx.h"
#include "Vector3d.h"
#include "Particle.h"
#include "Spring.h"
#include "Force.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "Rungekuttasolver.h"

typedef ParticleSystem::SpringList       SpringList;
typedef ParticleSystem::SpringListIt     SpringListIt;
typedef ParticleSystem::ForceList        ForceList;
typedef ParticleSystem::ForceListIt      ForceListIt;

void 
RungeKuttaSolver::step( double h )
{
    int numParticles     = mParticleSystem->getNumParticles();
    double halfH         = h / 2.0;

    double   *masses       = new double[ numParticles ];
    Vector3d *k1a          = new Vector3d[ numParticles ];
    Vector3d *k2a          = new Vector3d[ numParticles ];
    Vector3d *k3a          = new Vector3d[ numParticles ];
    Vector3d *k4a          = new Vector3d[ numParticles ];
    Vector3d *origPos      = new Vector3d[ numParticles ];
    Vector3d *origV        = new Vector3d[ numParticles ];
    Vector3d *tmpPos       = new Vector3d[ numParticles ];
    Vector3d *tmpV         = new Vector3d[ numParticles ];

    //---------- Store Away Particle Positions ----
    for( int i = 0; i < numParticles; i++ )
    {
        Particle &p    = mParticleSystem->getParticleAt( i );
        origPos[i]     = p.getPos();
        origV[i]       = p.velocity();
        masses[i]      = p.getMass();
    }

    //---------- 1. k1 = h*f(x,y) ----------
    calcAccel( numParticles, origPos, origV, masses, k1a );

    //---------- 2. k2 = h*f(x,y+k1/2) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        tmpV[i]    = origV[i]   + (k1a[i] * halfH);
        tmpPos[i] = origPos[i] + (tmpV[i] * halfH);
    }

    calcAccel( numParticles, tmpPos, tmpV, masses, k2a );

    //---------- 3. k3 = h*f(x,y+k2/2) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        tmpV[i]   = origV[i]     + (k2a[i] * halfH);
        tmpPos[i] = origPos[i] + (tmpV[i] * halfH);
    }

    calcAccel( numParticles, tmpPos, tmpV, masses, k3a );

    //---------- 3. k4 = h*f(x,y+k3) ----------
    for( int i = 0; i < numParticles; i++ )
    {
        tmpV[i]   =  origV[i]   + (k3a[i] * h);
        tmpPos[i] = origPos[i] + (tmpV[i] * h);
    }

    calcAccel( numParticles, tmpPos, tmpV, masses, k4a );

    //---------- 4. set x & v of particles ----------
    
    //Set particle position
    for( int i = 0; i < numParticles; i++ )
    {
        if( mParticleSystem->getParticleAt( i ).isPinned() )
            continue;

        Vector3d &newV = mParticleSystem->getParticleAt( i ).velocity();

        newV += (k1a[i] + k2a[i]*2.0 + k3a[i]*2.0 + k4a[i]) * h/6.0;
        
        mParticleSystem->getParticleAt( i ).pos() += newV * h;
    }

    delete [] masses;
    delete [] origPos;
    delete [] origV;
    delete [] tmpV;
    delete [] tmpPos;
    delete [] k1a;
    delete [] k2a;
    delete [] k3a;
    delete [] k4a;

}
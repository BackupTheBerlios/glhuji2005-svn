#include "StdAfx.h"
#include "Vector3d.h"
#include "ParticleSystem.h"
#include "NumericalSolver.h"
#include "ForwardEulerSolver.h"


ForwardEulerSolver::ForwardEulerSolver()
{
    mAccel = NULL;
}

ForwardEulerSolver::~ForwardEulerSolver()
{
    if( mAccel != NULL )
        delete [] mAccel;
}

void 
ForwardEulerSolver::step( double h )
{

    idx_t numParticles   = mParticleSystem->getNumParticles();
    double airResistance = mParticleSystem->getAirResistance();

    //allocate tmp store if it hasn't been allocated already
    if( mAccel == NULL )
    {
        mAccel = new Vector3d[ numParticles ];

        //print warnings

        //todo: make more readable
        double k = mParticleSystem->getStiffestSpring();
        if( h > 2.0/k )
            cout << "Warning: dt > 2/k Forward Euler is going to explode " << endl;
        else if( h > 1.0/k )
            cout << "Warning: dt > 1/k Forward Euler is going to oscillate " << endl;
        else
            cout << "Forward Euler should run fine with specified dt parameter " << endl;
    }

    //Copy out original position and velocity
    Vector3d *pos       = mParticleSystem->getParticlePositions();
    Vector3d *velocity  = mParticleSystem->getParticleVelocities();
    double   *invMasses = mParticleSystem->getParticleInvMasses();

    ParticleSystem::ParticleInfo *pInfo =
                                mParticleSystem->getParticleInfo();

    //calc dA
    calcAccel( pos, velocity, invMasses, mAccel );

    //store changes
    for( idx_t i = 0; i < numParticles; i++ )
    {
        //skip over pinned particles
        if( pInfo[i].pIsPinned )
            continue;

        //Vnew = Vold + h * dv/dt
        velocity[i] += mAccel[i] * h;
        
        //Xnew = Xold + h * Vnew
        pos[i]      += velocity[i] * h;

        velocity[i] *= airResistance;
    }

}

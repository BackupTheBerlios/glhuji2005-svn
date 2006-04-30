#ifndef __NUMERICAL_SOLVER_H__
#define __NUMERICAL_SOLVER_H__

class ParticleSystem;
class Vector3d;

class NumericalSolver
{
public:
    NumericalSolver();
    virtual ~NumericalSolver();

    //called by particle system when this solver is attached to one
    void attachToParticleSystem( ParticleSystem *inParticleSystem, bool inMidPoint );

    virtual void step( double h ) = 0;

protected:
    //calculate dA given position, velocity and inverted masses.
    void calcAccel( Vector3d *inPositions, Vector3d *inVelocities, 
        double *inInvMasses, Vector3d *outAccel );

protected:
    bool           mMidpoint;
    ParticleSystem *mParticleSystem;
};

#endif //__NUMERICAL_SOLVER_H__
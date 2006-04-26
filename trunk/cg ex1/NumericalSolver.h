#ifndef __NUMERICAL_SOLVER_H__
#define __NUMERICAL_SOLVER_H__

class ParticleSystem;

class NumericalSolver
{
public:
    NumericalSolver();
    virtual ~NumericalSolver();

    //called by particle system when this solver is attached to one
    void attachToParticleSystem( ParticleSystem *inParticleSystem, bool inMidPoint );

    virtual void step( double h );

protected:
    bool           mMidpoint;
    ParticleSystem *mParticleSystem;
};

#endif //__NUMERICAL_SOLVER_H__
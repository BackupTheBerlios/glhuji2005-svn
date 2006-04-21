#ifndef __SPRING_H__
#define __SPRING_H__

class Particle;

//Damped Spring connecting 2 particles
//
//Particles are stored as indexes into global particle array.
class Spring
{
public:
    Spring();
    Spring( idx_t inParticleA, idx_t inParticleB,
            double inLength, double inK, double inB );
    virtual ~Spring();

    idx_t getParticleA();
    idx_t getParticleB();

    double    getK();
    double    getB();

    double    getRestLength();

protected:
    idx_t    mParticleA;    //Particle 1
    idx_t    mParticleB;    //Particle 2
    double    mK;            //Spring Constant
    double    mB;            //Damping constant
    double    mRestLength;   //rest length of spring
    
};

#endif //__SPRING_H__
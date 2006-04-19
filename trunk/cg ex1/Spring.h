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
            float inK, float inB );
    virtual ~Spring();

    idx_t getParticleA();
    idx_t getParticleB();

    float    getK();
    float    getB();

protected:
    idx_t    mParticleA;    //Particle 1
    idx_t    mParticleB;    //Particle 2
    float    mK;            //Spring Constant
    float    mB;            //Damping constant
    
};

#endif //__SPRING_H__
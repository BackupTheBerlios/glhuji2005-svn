#ifndef  __PARTICLESYSTEM_LOADER_H__
#define  __PARTICLESYSTEM_LOADER_H__

class CSimulationsParams;
class CLoadIni;

//utility class to load ParticleSystem from a file into the particle system
class CParticleSystemLoader
{
public:
    //returns true if everything was loaded OK
    static bool Load( CSimulationsParams &inParams, const char* inFileName );

protected:
	static bool createParticleSystem( CSimulationsParams &inParams, CLoadIni &inLoader );
	static bool readGlobalConstants( CSimulationsParams &inParams, CLoadIni &inLoader );
	static bool readParticleDefaults( CSimulationsParams &inParams, CLoadIni &inLoader );
	static bool readNewtonianParticleSystem( CSimulationsParams &inParams, CLoadIni &inLoader );
};

#endif  //__PARTICLESYSTEM_LOADER_H__
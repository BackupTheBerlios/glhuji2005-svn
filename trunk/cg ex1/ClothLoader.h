#ifndef  __CLOTH_LOADER_H__
#define  __CLOTH_LOADER_H__

class ParticleSystem;
class CLoadIni;

//utility class to load cloth from a file into the particle system
class ClothLoader
{
public:
    //returns true if everything was loaded OK
    static bool Load( ParticleSystem &inSystem, std::string &inFileName );

protected:
    static void readSolver( ParticleSystem &inSystem, CLoadIni &inLoader );
    static bool readGlobalConstants( ParticleSystem &inSystem, CLoadIni &inLoader,
                                     idx_t &outMeshWidth, idx_t &outMeshHeight );
    static bool readMesh( ParticleSystem &inSystem, CLoadIni &inLoader,
                                    idx_t &inMeshWidth, idx_t &inMeshHeight );

    static bool readSprings( ParticleSystem &inSystem, CLoadIni &inLoader );
};

#endif  //__CLOTH_LOADER_H__
#ifndef  __CLOTH_LOADER_H__
#define  __CLOTH_LOADER_H__

class ParticleSystem;

//utility class to load cloth from a file into the particle system
class ClothLoader
{
public:
    //returns true if everything was loaded OK
    static bool Load( ParticleSystem &inSystem, std::string &inFileName );
};

#endif  __CLOTH_LOADER_H__
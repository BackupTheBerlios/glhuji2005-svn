// DataLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "ClothLoader.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    ParticleSystem pSystem;

    ClothLoader::Load( pSystem, std::string("cloth.psim") );

    while(1);

	return 0;
}


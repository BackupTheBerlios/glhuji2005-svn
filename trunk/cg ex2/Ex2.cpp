// Ex2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ArticulatedFigure.h"
#include "BVHParser.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    bool ok;

    ArticulatedFigure fig;
    BVHParser parser( string("aero working.bvh"), fig, ok );

    cout << "Loading BVH File..." << endl;
    parser.parse( ok );

    cout << "done loading BVH File " << endl;

    while(1);

	return 0;
}


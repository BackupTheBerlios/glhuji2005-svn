#include "StdAfx.h"
#include "constants.h"
#include "ParticleSystem.h"
#include "ClothLoader.h"
#include "GravityForce.h"
#include "Particle.h"
#include "CLoadIni.h"

#include <stdlib.h>
#include <string>

bool 
ClothLoader::Load( ParticleSystem &inSystem, string &inFileName )
{
    CLoadIni loader;
    bool ret = false;
    string val;
    idx_t meshWidth = 0;
    idx_t meshHeight = 0;

    do{

        if( loader.ReadIni(inFileName.c_str()) != 0) break;

        //load cloth width
        if( loader.GetField( C_MESH_WIDTH_TAG, val ) != 0 )
        {
            cout << "ERROR: meshwidth field is missing " << endl;
            break;
        }
        meshWidth = atol( val.c_str() );
        if( meshWidth <= 0 )
        {
            cout << "ERROR: meshwidth must be positive (" << meshWidth << ")" << endl;
            break;
        }

        //load cloth height
        if( loader.GetField( C_MESH_HEIGHT_TAG, val ) != 0 )
        {
            cout << "ERROR: meshheight field is missing " << endl;
            break;
        }
        meshHeight = atol( val.c_str() );
        if( meshHeight <= 0 )
        {
            cout << "ERROR: meshheight must be positive (" << meshHeight << ")" << endl;
            break;
        }

        inSystem.setDimensions( meshWidth, meshHeight );

        //load forces
        if( loader.GetField( C_GRAVITY_TAG, val ) != 0 )
        {
            cout << "ERROR: gravity field is missing " << endl;
            break;
        }
        double gravity = atof( val.c_str() );
        inSystem.addForce( new GravityForce( gravity ) );

        //load particles
        bool noerr = true;
        for( int i = 0; noerr && i < meshHeight; i++ )
            for( int j = 0; noerr && j < meshWidth; j++ )
        {
            char tmp[256];
            sprintf( tmp, "(%d,%d)", j, i );

            double *arr = 0;
            int xDim;
            int yDim;
            if( loader.GetField( tmp , val ) != 0 )
            {
                cout << "ERROR: particle @ " << j << "," << i <<" is missing " << endl;
                noerr = false;
                break;
            }

            //parse
            loader.GetDblArray( val, arr, &xDim, &yDim );

            //particle only has 1 dimension thank you!
            if( yDim != 1 )
            {
                if( arr != 0 ) delete[] arr; //deallocate memory...

                cout << "ERROR: yDim of particle @" << j << "," << i <<
                    "must be exactly 1 is (" << yDim << ")" << endl;
                break;
            }

            //check we don't get extra data
            if( xDim != C_NUM_FIELDS_IN_PARTICLE)
            {
                if( arr != 0 ) delete[] arr; //deallocate memory...

                cout << "ERROR: xDim of particle @" << j << "," << i <<
                    "should be" << C_NUM_FIELDS_IN_PARTICLE << "is (" << yDim << ")" << endl;
                break;
            }

            //debug
            cout << "SUCCESS: particle @ " << j << "," << i << "=" <<
                arr[0] << "," << arr[1] << "," << arr[2] << "," << arr[3]<< "," << arr[4] << endl;

            Particle p( arr[0], arr[1], arr[2], arr[3], (arr[4]==1) );

            inSystem.addParticleAt( j, i, p );

            delete [] arr;

        }

        //missing particle
        if( !noerr )
            break;

        //------------- Setup Springs ------------------
        double k, b;
        if( loader.GetField( C_SPRING_CONST_TAG , val ) != 0 )
        {
            cout << "ERROR: spring constant undefined " << C_SPRING_CONST_TAG << endl;
            break;
        }
        k = atof( val.c_str() );

        if( loader.GetField( C_SPRING_DRAG_TAG , val ) != 0 )
        {
            cout << "ERROR: spring drag constant undefined " << C_SPRING_DRAG_TAG << endl;
            break;
        }
        b = atof( val.c_str() );

        // add springs to system
        inSystem.constructSprings( k, b );


        //if we got here everything is ok
        //debug
        cout << "LOADED OK!" << endl;
        ret = true;
       } while(0);

    return ret;
}
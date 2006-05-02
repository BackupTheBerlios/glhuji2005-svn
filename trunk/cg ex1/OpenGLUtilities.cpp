/*	
    FileName: OpenGLUtilities.cpp

    Utility class to help us perform certain operations with
    OpenGL which should be built in...
*/

//------------ Includes ----------------
#include "stdafx.h"
#include <math.h>
#include "constants.h"
#include <GL/glut.h>
#include "OpenGLUtilities.h"

//------------ Implementation ----------------
void
OpenGLUtilities::drawCircle( double inRadius, double inCenterX, 
                                double inCenterY, double inCenterZ, double inLineWidth )
{
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    glTranslated( inCenterX, inCenterY, inCenterZ );

    glLineWidth( inLineWidth );

    // We could probably do this with far far less lines, but requirements where
    //not made for optimization...
    glBegin(GL_LINE_LOOP);
    
    for( int i=0; i < 360; i++ )
    {
        double angle = RADTODEG( i );
        glVertex3d( cos(angle)*inRadius, sin(angle)*inRadius , 0.0);
    }

    glEnd();

    //reset line width to default...
    glLineWidth( 1.0 );
    glPopMatrix();
}



void
OpenGLUtilities::initIdentityMatrix( GLdouble *outMatrix )
{
    for( int i = 0; i < 16; i++ )
    {
        outMatrix[i] = ((i % 5) == 0) ? 1.0 : 0.0;
    }
}

void
OpenGLUtilities::initTranslationMatrix( double inX, double inY, double inZ, GLdouble *outMatrix )
{
    initIdentityMatrix( outMatrix );
    outMatrix[12] = inX;
    outMatrix[13] = inY;
    outMatrix[14] = inZ;
}

//for debugging purposes, thus commented out - ON PURPOSE...
/*void
OpenGLUtilities::printMatrix( GLdouble *inMatrix )
{
    std::cout << "Matrix: " << endl;
    for( int i = 0; i < 4; i++ )
    {
        cout << inMatrix[i] << ", " << inMatrix[i+4] << ", ";
        cout << inMatrix[i+8] << ", " << inMatrix[i+12] << endl;

    }

    cout << endl;
}*/

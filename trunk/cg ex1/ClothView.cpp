#include "StdAfx.h"
#include "Particle.h"
#include "ClothLoader.h"
#include "Clothview.h"
#include "constants.h"
#include <GL/glut.h>

void timerCallback( int inVal );

void 
ClothView::display()
{
    drawParticleSystem();
    glFlush();
    glutSwapBuffers();
}

void 
ClothView::reshape( int inWidth, int inHeight )
{
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
    gluPerspective(40.0,(GLdouble)inWidth/(GLdouble)inHeight,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,inWidth,inHeight);

    /*mWindowWidth = inWidth;
    mWindowHeight = inHeight;

    //----- Calculate New Values for Radius and frustum -----
    mRadius = 0.4 * min(inWidth,inHeight); // Setting arcball's radius

    //----- Update Object Properties -----
    m3dObject.setFrustum( min(2*C_OBJECT_B_RAD, tan(0.5*mFrustumDeg)*mNear) );

    //----- Update Arcball Controller -----
    m3dController.setRadius( mRadius );
    m3dController.setScreenCenter( Point2d( inWidth/2, inHeight/2 ) );

    //----- Set Viewport Geometry -----
    glViewport(0, 0, (GLsizei)inWidth, (GLsizei)inHeight);*/
}

void 
ClothView::mousePressed( int inButton, int inState, int inX, int inY )
{

}

void 
ClothView::mouseMoved( int inX, int inY )
{
}

void 
ClothView::keyPressed( unsigned char inKey, int inX, int inY )
{
    switch( inKey )
    {
        //exit on esc or q
        case C_ESCAPE_KEY_CODE:
        case 'q':
            exit( 0 );
        break;
        
        //stop or advance one step on space
        case ' ':
            if( mIsRunning == false )
            {
                mParticleSystem.step();
                redraw();
            }
            else
                stop();
            break;

        //continue running on enter
        case C_ENTER_KEY_CODE:
            mIsRunning = true;
            break;

        default:
            break;
    }
}

void 
ClothView::timerCalled( int inVal )
{
    //keep timer running
    glutTimerFunc( C_TIMER_DELAY, timerCallback, 0 );

    if( mIsRunning )
    {
        mParticleSystem.step();
        redraw();
    }
}

void
ClothView::redraw()
{
    //just call through to glut
    glutPostRedisplay();
}

void
ClothView::drawParticleSystem()
{
    GLfloat gOriginX = -2.0f;
    GLfloat gOriginY = 2.0f;
    GLfloat gOriginZ = -12.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity();									// Reset The Current Modelview Matrix
    // Here we draw our mesh
    // ---------------------
    idx_t w = mParticleSystem.getWidth();
    idx_t h = mParticleSystem.getHeight();

    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(gOriginX,gOriginY,gOriginZ);		
    glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
    for (int y=0; y<h-1; y++){
        glBegin(GL_QUAD_STRIP);									// Draw A Quad strip for every row
        for (int x=0; x<w; x++){
            Particle& p1 = mParticleSystem.getParticleAt(x, y+1);
            Particle& p2 = mParticleSystem.getParticleAt(x, y);
            glVertex3f( (p1.getPos()).pX, (p1.getPos()).pY, (p1.getPos()).pZ);
            glVertex3f( (p2.getPos()).pX, (p2.getPos()).pY, (p2.getPos()).pZ);
        }
        glEnd();											// Done Drawing The Quad
    }
}


ClothView::ClothView( )
{
    mWindowWidth  = C_WINDOW_WIDTH;
    mWindowHeight = C_WINDOW_HEIGHT;
    mIsRunning    = true;
}

void 
ClothView::loadSimulation( string &inFileName )
{
    ClothLoader::Load( mParticleSystem, inFileName );
}

void 
ClothView::stop()
{
    mIsRunning = false;
}

ClothView::~ClothView(void)
{
}

ClothView *
ClothView::getInstance()
{
    if( mInstance == NULL )
        mInstance = new ClothView();

    return mInstance;
}

ClothView * ClothView::mInstance = NULL;
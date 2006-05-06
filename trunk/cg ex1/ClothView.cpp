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
    gluPerspective(40.0,(GLdouble)inWidth/(GLdouble)inHeight,0.5,20000.0);
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

#define MOVEMENT_STEP 0.009
#define ROTATION_STEP 0.3
#define ZOOM_STEP 1
bool gLBDown = false;
bool gRBDown = false;
int gLastX;
int gLastY;
GLfloat gOriginX = -2.0f;
GLfloat gOriginY = 2.0f;
GLfloat gOriginZ = -12.0f;
GLfloat g_xRotated = 60.0f;
GLfloat g_yRotated = 20.0f;
GLfloat g_zRotated = 0.0f;

void 
ClothView::mousePressed( int inButton, int inState, int inX, int inY )
{
	// if we want both mouse and ALT (for example) use this instead of next line
	//specialKey = glutGetModifiers();
	//if ((inState == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT)) {
	gLastX = inX;
	gLastY = inY;
	if (inState == GLUT_DOWN && inButton == GLUT_LEFT_BUTTON) {
		gLBDown = true;
	}
	else if (inState == GLUT_UP && inButton == GLUT_LEFT_BUTTON) {
		gLBDown = false;
	}
	if (inState == GLUT_DOWN && inButton == GLUT_RIGHT_BUTTON) {
		gRBDown = true;
	}
	else if (inState == GLUT_UP && inButton == GLUT_RIGHT_BUTTON) {
		gRBDown = false;
	}
}

void 
ClothView::mouseMoved( int inX, int inY )
{
	int dx = (inX-gLastX);
	int dy = (inY-gLastY);
	if (gLBDown && gRBDown)
	{
		gOriginZ -= ((float)dy)*ZOOM_STEP;
	}
	else if (gLBDown)
	{
		int dx = (inX-gLastX);
		int dy = (inY-gLastY);
		g_xRotated += ((float)dy)*ROTATION_STEP;
		g_yRotated += ((float)dx)*ROTATION_STEP;
	}
	else if (gRBDown)
	{
		gOriginX -= ((float)dx)*MOVEMENT_STEP*(gOriginZ/(ZOOM_STEP*3));
		gOriginY += ((float)dy)*MOVEMENT_STEP*(gOriginZ/(ZOOM_STEP*3));
	}
	gLastX = inX;
	gLastY = inY;
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity();									// Reset The Current Modelview Matrix
    // Here we draw our mesh
    // ---------------------
    idx_t w = mParticleSystem.getWidth();
    idx_t h = mParticleSystem.getHeight();

    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(gOriginX,gOriginY,gOriginZ);		
	glRotatef(g_xRotated,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
	glRotatef(g_yRotated,0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
	glRotatef(g_zRotated,0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )
    for (int y=0; y<h-1; y++){
        glBegin(GL_QUAD_STRIP);									// Draw A Quad strip for every row
        for (int x=0; x<w; x++){
		    glColor3f(((float)x/(float)w)*0.5f+0.5f,((float)(x+y)/(float)(w+h))*0.5f+0.5f,((float)y/(float)h)*0.5f+0.5f);
			Vector3d &p1 = mParticleSystem.getParticlePos(x, y+1);
            Vector3d &p2 = mParticleSystem.getParticlePos(x, y);
            glVertex3f( p1.pX, p1.pY, p1.pZ);
            glVertex3f( p2.pX, p2.pY, p2.pZ);
//			Particle& p1 = mParticleSystem.getParticleAt(x, y+1);
//            Particle& p2 = mParticleSystem.getParticleAt(x, y);
//            glVertex3f( (p1.getPos()).pX, (p1.getPos()).pY, (p1.getPos()).pZ);
//            glVertex3f( (p2.getPos()).pX, (p2.getPos()).pY, (p2.getPos()).pZ);
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
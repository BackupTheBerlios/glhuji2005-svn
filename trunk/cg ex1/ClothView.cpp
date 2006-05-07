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
GLfloat gOriginY = 6.0f;
GLfloat gOriginZ = -52.0f;
GLfloat g_xRotated = 15.0f;
GLfloat g_yRotated = 10.0f;
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
        
		//toggle wireframe/solid
        case 'w':
            mWireFrameMode = !mWireFrameMode;
            redraw();
            break;

		//show/hide normals
        case 'n':
            mNormalsMode = !mNormalsMode;
            redraw();
            break;
        
		//enable/disable normals' smoothing
        case 's':
            mSmoothByNormals = !mSmoothByNormals;
            redraw();
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

GLfloat light0Pos[4] = { -50, 40, 0, 1 };
GLfloat light1Pos[4] = { 50, 40, 0, 1 };
GLfloat white[4] = { 1, 1, 1, 1 };

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

	// update normals
	mParticleSystem.calculateNormals();
	// show normals?
	if (mNormalsMode)
	{
		for (int y=0; y<h; y++){
			for (int x=0; x<w; x++){
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				glBegin(GL_LINES);
				glColor3f(1.0,0,0);
				Vector3d &p1 = mParticleSystem.getParticlePos(x, y);
				Vector3d &normal = mParticleSystem.getParticleNormal(x, y);
				glVertex3f(p1.pX, p1.pY, p1.pZ);
				glVertex3f(p1.pX + normal.pX, p1.pY + normal.pY, p1.pZ + normal.pZ);
				glEnd();
			}
		}
	}

	//only draw outline in wireframe mode
    if( mWireFrameMode )
	{
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
    else
	{
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	/*	GLfloat red[4] = { 0.4, 0, 0, 5 };
		GLfloat white[4] = { 6, 6, 6, 6 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, red);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialf(GL_FRONT, GL_SHININESS, 90.0);*/
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT0, GL_POSITION, light1Pos);

		//attenuate
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

		glPointSize(12.0);
		glBegin(GL_POINTS);
		glVertex3fv(light0Pos);
		glEnd();
	}


    for (int y=0; y<h-1; y++){
        glBegin(GL_QUAD_STRIP);									// Draw A Quad strip for every row
        for (int x=0; x<w; x++){
		    glColor3f(((float)x/(float)w)*0.5f+0.5f,((float)(x+y)/(float)(w+h))*0.5f+0.5f,((float)y/(float)h)*0.5f+0.5f);
			Vector3d &p1 = mParticleSystem.getParticlePos(x, y+1);
            Vector3d &p2 = mParticleSystem.getParticlePos(x, y);
			
			Vector3d &n1 = mParticleSystem.getParticleNormal(x, y+1);
            Vector3d &n2 = mParticleSystem.getParticleNormal(x, y);

			if (mSmoothByNormals) glNormal3f(-n1.pX, -n1.pY, -n1.pZ);
            glVertex3f( p1.pX, p1.pY, p1.pZ);

			if (mSmoothByNormals) glNormal3f(-n2.pX, -n2.pY, -n2.pZ);
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
	mWireFrameMode   = true;
	mNormalsMode     = false;
	mSmoothByNormals = true;
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
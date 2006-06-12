// Ex2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ArticulatedFigure.h"
#include "BVHParser.h"
#include "windows.h"
#include "basemotionfilter.h"
#include <string>
#include <iostream>

#include <math.h>
#include <stdlib.h>

using namespace std;

static const float WINSIZE = 512;

ArticulatedFigure g_articulatedFigure;
int g_nFrameNum = 0;
bool g_bLinesOnly = false;
bool g_bPause = false;
SYSTEMTIME gLastFrameTime;
double g_fFrameTime = 0.33333;

//Gets number of milliseconds in a systemtime variable
int getms(const SYSTEMTIME& st)
{
	return (st.wMilliseconds + (st.wSecond+(st.wMinute+(st.wHour+st.wDay*24)*60)*60)*1000);
}


#define MOVEMENT_STEP 0.009
#define ROTATION_STEP 0.3
#define ZOOM_STEP 1
bool gLBDown = false;
bool gRBDown = false;
int gLastX;
int gLastY;
GLfloat gOriginX = 0.0f;
GLfloat gOriginY = 0.0f;
GLfloat gOriginZ = -163.0f;
GLfloat g_xRotated = 15.0f;
GLfloat g_yRotated = 10.0f;
GLfloat g_zRotated = 0.0f;


void 
mousePressedCallback( int inButton, int inState, int inX, int inY )
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

void DisplayCallback();
void 
mouseMovedCallback( int inX, int inY )
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
    DisplayCallback();
}

/////////////////////////////////////////////////////
// FUNC: Initialize()
// DOES: basic OpenGL initialization
/////////////////////////////////////////////////////

void Initialize()
{
      // initialize matrix stacks
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();									// Reset The Current Modelview Matrix

	gluPerspective(30,1,1,20000);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	gluLookAt(0,0,-150,  0,0,0,  0,1,0);
    glViewport( 0, 0, WINSIZE, WINSIZE );
    glEnable( GL_DEPTH_TEST );
	glClearColor(0.8f,0.8f,1.0f,0);

   // Default values for material and light properties.
   GLfloat mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0 };
   GLfloat mat_diffuse[] = { 0.6f,0.6f,0.6f, 1.0 };
   GLfloat mat_specular[] = { 0,0,10, 1.0 };
   GLfloat mat_shininess[] = { 100.0 };
   GLfloat light_ambient[] = { 1, 1, 1, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 10.0, 10.0, 10.0, 1.0 };
   glShadeModel (GL_SMOOTH);

//   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

   glEnable(GL_LIGHT0);
   glEnable(GL_NORMALIZE); 
}

//////////////////////////////////////////////////
// PROC:   BVH::drawGround()
// DOES:   draws a ground plane and grid
//////////////////////////////////////////////////

void drawGround()
{
	float x,z;
	int nx, nz;
	const float xmin=-10, xmax=10;
	const float zmin=-10, zmax=10;
	const int nsteps = 20;
	glColor3f(0,0,0); 
	float dx = (xmax - xmin)/nsteps;
	float dz = (zmax - zmin)/nsteps;
		// draw grid lines in xz-plane, parallel to z axis
	glPushMatrix();
	float sf = g_articulatedFigure.getMaxOffset();
	glScalef(sf,sf,sf);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (nx=0; nx<=nsteps; nx++) {
		x = xmin + nx*dx;
		glVertex3f(x,0,zmin);
		glVertex3f(x,0,zmax);
	}
	glEnd();
		// draw grid lines in xz-plane, parallel to x-axis
	glBegin(GL_LINES);
	for (nz=0; nz<=nsteps; nz++) {
		z = zmin + nz*dz;
		glVertex3f(xmin,0,z);
		glVertex3f(xmax,0,z);
	}
	glEnd();
	glPopMatrix();
}

/////////////////////////////////////////////////////
// FUNC: DisplayCallback()
// DOES: glut calls this whenever the window needs to be redrawn
/////////////////////////////////////////////////////
void DisplayCallback()
{
	glClearColor(1.0f,1.0f,1.0f,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
    glLoadIdentity();									// Reset The Current Modelview Matrix
    glTranslatef(gOriginX,gOriginY,gOriginZ);		
	glRotatef(g_xRotated,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
	glRotatef(g_yRotated,0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
	glRotatef(g_zRotated,0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )

	//	glScalef(drawScale,drawScale,drawScale);
//	glTranslatef(-drawCenter[0],-drawCenter[1],-drawCenter[2]);
	
	// draw a mesh at the "ground level"
	//TODO: not sure we need that
	drawGround();

	glColor3f(0,0,1);
//	float sphereSize = 0.06f*g_articulatedFigure.getMaxOffset();
//	glutSolidSphere(sphereSize,20,10);// a ball at the (0,0,0) point

	g_articulatedFigure.draw(g_nFrameNum, g_bLinesOnly);

	glPopMatrix();
	glutSwapBuffers();
}

/////////////////////////////////////////////////////
// FUNC: keypress()
// DOES: glut calls this whenever a key is pressed
/////////////////////////////////////////////////////

void keypress(unsigned char key, int x, int y)
{
	BaseMotionFilter* pFilter=NULL;

	switch(key) {
	case 'q':          // quit
		exit(0);
		break;
	case ' ':
		if (!g_bPause)
			break;
		g_nFrameNum++;
		if (g_nFrameNum >= g_articulatedFigure.getNumOfFrames()){
			g_nFrameNum = 0;
		}
		break;
	case 0x08:	//Backspace
		if (!g_bPause)
			break;
		g_nFrameNum--;
		if (g_nFrameNum < 0){
			g_nFrameNum = g_articulatedFigure.getNumOfFrames()-1;
		}
		break;
	case 's':
		pFilter = BaseMotionFilter::createFilter(BaseMotionFilter::MF_SMOOTH);
		pFilter->loadFilter(3.0);
		g_articulatedFigure.applyFilter(pFilter);
		break;
	case 'r':
		g_articulatedFigure.applyFilter(NULL);
		break;
	case 'l':
		g_bLinesOnly = !g_bLinesOnly;
		break;
	case '*':
		g_fFrameTime = g_articulatedFigure.getFrameTime();
		break;
	case '-':
		g_fFrameTime += g_articulatedFigure.getFrameTime()*0.33;
		break;
	case '+':
		g_fFrameTime -= g_articulatedFigure.getFrameTime()*0.33;
		break;
	case '0':
		g_nFrameNum = 0;
		break;
	case 'p':
		g_bPause = !g_bPause;
		break;
	case 'h':           // lists commands
		printf("\n");
		printf(" h --> help\n");
		printf(" space --> next frame\n");
		printf(" backspae --> previous frame\n");
		printf(" + --> increase speed\n");
		printf(" - --> decrease speed\n");
		printf(" * --> default speed\n");
		printf(" 0 --> goto first frame\n");
		printf(" p --> toggle pause/play\n");
		printf(" l --> toggle line-figure/muscular-figure\n");
		printf(" s --> smooth filter\n");
		printf(" r --> remove filter\n");
		//TODO:...
		printf(" q -- quit\n");
		printf("\n");
	}
	if (pFilter!=NULL){
		delete pFilter;
	}

	glutPostRedisplay();
}

void idleFunc(void)
{
	SYSTEMTIME st;

	if (g_bPause)
	{
		GetSystemTime(&gLastFrameTime);	//Set 'last frame time'
		return;
	}
	GetSystemTime(&st);	//Set initial 'last frame time'
	if (getms(st)-getms(gLastFrameTime) > (int)(g_fFrameTime*1000))
	{
		g_nFrameNum++;
		if (g_nFrameNum >= g_articulatedFigure.getNumOfFrames()){
			g_nFrameNum = 0;
		}
		DisplayCallback();
		GetSystemTime(&gLastFrameTime);	//Set 'last frame time'
	}
}

/////////////////////////////////////////////////////
// FUNC: main()
// DOES: initializes glut, reads BVH file, then hands control to glut
/////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // create window and rendering context
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( WINSIZE, WINSIZE );
    glutCreateWindow( "BVH Player" );
    glutDisplayFunc( DisplayCallback );
    glutIdleFunc( idleFunc );
    glutMouseFunc( mousePressedCallback );
    glutMotionFunc( mouseMovedCallback );
	
//    glutMouseFunc(mousebutton);
//    glutMotionFunc(mousemotion);
	glutKeyboardFunc(keypress);

    Initialize();

    bool ok;

    BVHParser parser( string("aero.bvh"), g_articulatedFigure, ok );

    cout << "Loading BVH File..." << endl;
    parser.parse( ok );

    cout << "done loading BVH File " << endl;

//    while(1);

	g_fFrameTime = g_articulatedFigure.getFrameTime();	//Initialize to loaded value
	GetSystemTime(&gLastFrameTime);	//Set initial 'last frame time'
    glutMainLoop();
    return 0;           // never reached
}

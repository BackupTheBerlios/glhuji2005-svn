#include "stdafx.h"

#include "Ex3MFCDlg.h"
#include ".\openglwin.h"
#include <string>
#include <iostream>
#include <math.h>
#include "ParticleSystem.h"
#include "NewtonianParticleSystem.h"
#include "SimulationsParams.h"
#include "ParticleSystemLoader.h"

using namespace std;

static const float WINSIZE = 512;

COpenGLWin g_OpenGLWin;
//CNewtonianParticleSystem g_ParticleSystem;
CSimulationsParams g_simulationParams;

void usage()
{
	char buff [1500];
	strcpy(buff, "\n");
	strcat(buff, " h --> help\n");
	strcat(buff, " backspace --> previous frame\n");
	strcat(buff, " + --> increase speed\n");
	strcat(buff, " - --> decreanse speed\n");
	strcat(buff, " * --> default speed\n");
	strcat(buff, " 0 --> goto first frame\n");
	strcat(buff, " p --> toggle pause/play\n");
	strcat(buff, " l --> toggle line-figure/spacefill-figure\n");
	strcat(buff, " s --> toggle shading\n");
	strcat(buff, " q -- quit\n");
	strcat(buff, "\n");
	AfxMessageBox(buff, MB_OK|MB_ICONINFORMATION);
}
void DisplayCallback(void)
{g_OpenGLWin.DisplayCallback();}

void resize(int w, int h)
{g_OpenGLWin.Resize(w, h);}

void mousePressedCallback( int inButton, int inState, int inX, int inY )
{g_OpenGLWin.mousePressedCallback(inButton, inState, inX, inY);}

void mouseMovedCallback( int inX, int inY )
{g_OpenGLWin.mouseMovedCallback(inX, inY);}

void keypress(unsigned char key, int x, int y)
{g_OpenGLWin.keypress(key, x, y);}

void idleFunc(void)
{g_OpenGLWin.idleFunc();}


int g_nFrameNum = 0;
bool g_bLinesOnly = false;
bool g_bLighting = true;
bool g_bPause = false;
SYSTEMTIME gLastFrameTime;
double g_fFrameTime = 0.03333;
bool gLineBackground = false;
//Frame time change increment
#define FRAME_TIME 0.05

bool g_DrawGround = false;
static GLuint gTexName;
const int gCheckImageWidth = 64;
const int gCheckImageHeight = 64;
static GLubyte gCheckImage[gCheckImageHeight][gCheckImageWidth][4];

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
GLfloat gOriginY = 40.0f;
GLfloat gOriginZ = -615.0f;
GLfloat g_xRotated = 44.0f;	//15.0f;
GLfloat g_yRotated = -12.0f;	//10.0f;
GLfloat g_zRotated = 0.0f;

// Default values for material and light properties.
GLfloat mat_ambient[] = { 0.3f, 0.3f, 0.6f, 0.5 };
GLfloat mat_diffuse[] = { 0.0f,0.0f,1.0f, 1.0 };
GLfloat mat_diffuse_tr[] = { 1.0f,0.0f,0.0f, 0.5 };
GLfloat mat_specular[] = { 0,0,10, 0.5 };
GLfloat mat_shininess[] = { 80.0 };
GLfloat light_ambient[] = { 1, 1, 1, 1.0 };
GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat light_specular[] = { 10.0, 10.0, 10.0, 1.0 };
GLfloat light_position1[]= { -14.0f, 50.0f, -4.0f, 1.0f };//DE


void
COpenGLWin::mousePressedCallback( int inButton, int inState, int inX, int inY )
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
COpenGLWin::mouseMovedCallback( int inX, int inY )
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
    glutPostRedisplay();
}

void
generateCheckerBoard()
{
    int width = 1;

    for( int i = 0; i < gCheckImageHeight; i++) {
        for( int j = 0; j < gCheckImageWidth; j++) {

            gCheckImage[i][j][0] = (GLubyte) 0;
            gCheckImage[i][j][3] = (GLubyte) 128;

            bool blueSquare = ((i / width)%2==0) ^ ((j / width)%2==0);
            if( blueSquare )
            {
                gCheckImage[i][j][1] = (GLubyte) 0;
                gCheckImage[i][j][2] = (GLubyte) 240;
            }
            else
            {
                gCheckImage[i][j][1] = (GLubyte) 200;
                gCheckImage[i][j][2] = (GLubyte) 0;
            }

        }
    }
}

/////////////////////////////////////////////////////
// FUNC: Initialize()
// DOES: basic OpenGL initialization
/////////////////////////////////////////////////////

void COpenGLWin::Initialize()
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

   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   glShadeModel (GL_SMOOTH);

//   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   glEnable(GL_BLEND);                                      /**/
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);       /**/

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

   //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	//attenuate
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.05);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

	glEnable(GL_LIGHTING);
   glEnable(GL_NORMALIZE);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   generateCheckerBoard();
   glGenTextures(1, &gTexName);
   glBindTexture(GL_TEXTURE_2D, gTexName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gCheckImageWidth, gCheckImageHeight,
       0, GL_RGBA, GL_UNSIGNED_BYTE, gCheckImage);
   g_simulationParams.m_particleSystem->init();
}

/////////////////////////////////////////////////////
// FUNC: DisplayCallback()
// DOES: glut calls this whenever the window needs to be redrawn
/////////////////////////////////////////////////////
void COpenGLWin::DisplayCallback()
{
	glClearColor(1.0f,1.0f,1.0f,0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	double maxOffset = 1;//ToDo: Get max offset for camera positioning
	double drawScale = 20/maxOffset;
	Point3d bodyCenter =  Point3d(0.0,0.0,0.0);//ToDo: Get geometrical center for camera positioning;

	glPushMatrix();
    glLoadIdentity();										// Reset The Current Modelview Matrix

    glTranslatef(gOriginX,gOriginY,gOriginZ);
	glRotatef(g_xRotated,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
	glRotatef(g_yRotated,0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
	glRotatef(g_zRotated,0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )

    glScalef(drawScale,drawScale,drawScale);
	glTranslatef(-bodyCenter[0],-bodyCenter[1],-bodyCenter[2]);

	// draw a mesh at the "ground level"
	if (g_DrawGround)
		drawGround();

	// draw
	glColor3f(0,0,1);
	int nShading = 0;
	if (g_bLinesOnly)
		nShading += 1;
	if (g_bLighting)
		nShading += 2;
	g_simulationParams.m_particleSystem->display(g_nFrameNum, nShading);

	glPopMatrix();
	glutSwapBuffers();
}


void COpenGLWin::gotoNextFrame()
{
	if (!m_bLoaded)
		return;
	g_nFrameNum++;
	g_simulationParams.m_particleSystem->calcNextFrame();
	glutPostRedisplay();
}
void COpenGLWin::gotoPrevFrame()
{
	if (!m_bLoaded)
		return;
	g_simulationParams.m_particleSystem->prevFrame();
	return;
}
void COpenGLWin::gotoFrame(int nFrame)
{
	if (!m_bLoaded)
		return;
	int nTmp = g_nFrameNum;
	g_nFrameNum = nFrame;
	if (g_nFrameNum < 0)
	{
		g_nFrameNum = 0;
	}
	if (g_simulationParams.m_particleSystem->gotoFrame(nFrame))
		m_pParent->updateFarme();
	else
		g_nFrameNum = nTmp;
	glutPostRedisplay();
}

bool COpenGLWin::gotoZero()
{
	if (!m_bLoaded)
		return true;
	gotoFrame(0);
	return true;
}

bool COpenGLWin::isPaused()
{
	return g_bPause;
}
bool COpenGLWin::playPause()
{
	if (!m_bLoaded)
		return true;
	g_bPause = !g_bPause;
	m_pParent->updateFarme();
	m_pParent->UpdateData(TRUE);
	m_pParent->m_PlayBtn.SetWindowText(g_bPause?"|>":"||");
	m_pParent->UpdateData(FALSE);
	return g_bPause;
}

int COpenGLWin::getFrameCount()
{
	if (!m_bLoaded)
		return 0;
	return g_nFrameNum;
}
int COpenGLWin::getCurFrame()
{
	if (!m_bLoaded)
		return 0;
	return g_nFrameNum;
}

/////////////////////////////////////////////////////
// FUNC: keypress()
// DOES: glut calls this whenever a key is pressed
/////////////////////////////////////////////////////
void COpenGLWin::keypress(unsigned char key, int x, int y)
{

	switch(key) {
	case 'q':          // quit
		exit(0);
		break;
	case 'p':
		playPause();
		break;
	case ' ':
		if (!g_bPause)
			break;
		gotoNextFrame();
		return;
	case 0x08:	//Backspace
		if (!g_bPause)
			break;
		gotoPrevFrame();
		return;
	case '0':
		gotoZero();
		break;
	case 'l':
		g_bLinesOnly = !g_bLinesOnly;
		break;
	case 's': //toggle lighting
		if (g_bLighting)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		g_bLighting = !g_bLighting;
		break;
	case '*':
		g_fFrameTime = FRAME_TIME;
		break;
	case '-':
		g_fFrameTime += (FRAME_TIME*0.2);
		break;
	case '+':
		g_fFrameTime -= (FRAME_TIME*0.2);
		break;
	case 'h':           // lists commands
		usage();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void COpenGLWin::idleFunc(void)
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
		gotoNextFrame();
		GetSystemTime(&gLastFrameTime);	//Set 'last frame time'
	}
}

COpenGLWin::COpenGLWin(void)
{
	m_bLoaded = false;
	m_bCreated = false;
	m_pParent = NULL;
}

COpenGLWin::~COpenGLWin(void)
{
}

void COpenGLWin::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width / (float)height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set the viewing position and orientation
	gluLookAt(
	0.0, 0.0, 200.0, // eye location
	0.0, 0.0, 0.0, // center location
	0.0, 1.0, 0.0); // up vector
}

void COpenGLWin::Save(CString filename)
{
}

void COpenGLWin::Run(CString sFilename)
{
	m_sFilename = sFilename;
	int ac = 1;
	char v = '\0';
	char* av = &v;

	CParticleSystemLoader psLoader;
	if (psLoader.Load(g_simulationParams, m_sFilename.GetBuffer()))
	{
		if (!m_bCreated)
		{
			glutInit( &ac, &av );
			glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
			glutInitWindowSize( WINSIZE, WINSIZE );
			glutCreateWindow( "BVH Player" );
			glutDisplayFunc( ::DisplayCallback );
			glutIdleFunc( ::idleFunc );
			glutMouseFunc( ::mousePressedCallback );
			glutMotionFunc( ::mouseMovedCallback );
			glutKeyboardFunc(::keypress);
			m_bCreated = true;
		}

		Initialize();//TODO: get params from ini

		m_bLoaded = true;
		//g_fFrameTime = 0.33;	//Initialize to loaded value
		GetSystemTime(&gLastFrameTime);	//Set initial 'last frame time'
		glutMainLoop();
	}
	else
	{
		AfxMessageBox("Failed to load the ini file!", MB_OK|MB_ICONINFORMATION);
	}
	m_sFilename.ReleaseBuffer();
    return;           // never reached
}

void
COpenGLWin::Close()
{
	//ToDo: Closing
	if (m_bLoaded)
	{
		m_bLoaded = false;
		glutPostRedisplay();
		m_pParent->updateFarme();
	}
}

//////////////////////////////////////////////////
// PROC:   BVH::drawGround()
// DOES:   draws a ground plane and grid
//////////////////////////////////////////////////
void COpenGLWin::drawGround()
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
    double sf = 10;	//max offset distance
	float minY = 0;	//ground level
    glScaled(sf,1,sf);

    if( !gLineBackground )
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, gTexName);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(xmin, minY, zmin);
        glTexCoord2f(0.0, 1.0); glVertex3f(xmax, minY, zmin);
        glTexCoord2f(1.0, 1.0); glVertex3f(xmax, minY, zmax);
        glTexCoord2f(1.0, 0.0); glVertex3f(xmin, minY, zmax);
        glEnd();
        glFlush();
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
	    glLineWidth(1);
	    glBegin(GL_LINES);
	    for (nx=0; nx<=nsteps; nx++) {
		    x = xmin + nx*dx;
		    glVertex3f(x,minY,zmin);
		    glVertex3f(x,minY,zmax);
	    }
	    glEnd();
		    // draw grid lines in xz-plane, parallel to x-axis
	    glBegin(GL_LINES);
	    for (nz=0; nz<=nsteps; nz++) {
		    z = zmin + nz*dz;
		    glVertex3f(xmin,minY,z);
		    glVertex3f(xmax,minY,z);
	    }
	    glEnd();
    }
	glPopMatrix();
}
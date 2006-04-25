// glutest.cpp : Defines the entry point for the console application.
//

#include <windows.h>		// Header File For Windows
#include "stdafx.h"
#include <GL/glut.h>

#include "Particle.h"
#include "ParticleSystem.h"
#include "ClothLoader.h"
#include "vector3d.h"
#include "constants.h"
#include "ForwardEulerSolver.h"

#define MOVEMENT_STEP 0.1

GLfloat g_xRotated, g_yRotated, g_zRotated;
ForwardEulerSolver* gFESolver = NULL;

ParticleSystem pSystem;
GLfloat g_deltaX = 0;//0.3;
GLfloat g_deltaY = 0;//0.1;
GLfloat g_deltaZ = 0;//-0.4;
GLfloat gOriginX = -2.0f;//-0.4;
GLfloat gOriginY = 2.0f;//-0.4;
GLfloat gOriginZ = -12.0f;//-0.4;

int DrawGLSimulation(ParticleSystem& pSystem)		// Here's Where We Do All The Drawing
{
	// This part is for debug only - so that we have something on screen always
	// and we can have a reference for our model
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	/*
	glTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	glRotatef(g_yRotated,0.0f,1.0f,0.0f);						// Rotate The Triangle On The Y axis ( NEW )
	glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Right Of Triangle (Right)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);					// Right Of Triangle (Back)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
	glEnd();											// Done Drawing The Pyramid
*/

	// Here we draw our mesh
	// ---------------------
	idx_t w = pSystem.getWidth();
	idx_t h = pSystem.getHeight();

	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(gOriginX,gOriginY,gOriginZ);						// Move Right 1.5 Units, down 1 Unit And Away from The Screen 8.0
	glRotatef(g_xRotated,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
	glRotatef(g_yRotated,0.0f,1.0f,0.0f);					// Rotate The Quad On The Y axis ( NEW )
	glRotatef(g_zRotated,0.0f,0.0f,1.0f);					// Rotate The Quad On The Z axis ( NEW )
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		for (int y=0; y<h-1; y++){
	glBegin(GL_QUAD_STRIP);									// Draw A Quad strip for every row
			for (int x=0; x<w; x++){
				Particle& p1 = pSystem.getParticleAt(x, y+1);
				Particle& p2 = pSystem.getParticleAt(x, y);
//				Particle& p3 = pSystem.getParticleAt(x+1, y);
//				Particle& p4 = pSystem.getParticleAt(x+1, y+1);

				glVertex3f( (p1.getPos()).pX, (p1.getPos()).pY, (p1.getPos()).pZ);
				glVertex3f( (p2.getPos()).pX, (p2.getPos()).pY, (p2.getPos()).pZ);
//				glVertex3f( (p3.getPos()).pX, (p3.getPos()).pY, (p3.getPos()).pZ);
//				glVertex3f( (p4.getPos()).pX, (p4.getPos()).pY, (p4.getPos()).pZ);
			}
	glEnd();											// Done Drawing The Quad
		}

	return 1;										// Keep Going
}

void Display(void)
{
	/*
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-4.0);
	glRotatef(g_xRotated,1.0,0.0,0.0);
	glRotatef(g_yRotated,0.0,1.0,0.0);
	glRotatef(g_zRotated,0.0,0.0,1.0);
	glScalef(2.0,1.0,1.0);
	glutWireCube(1.0);
	*/

	DrawGLSimulation(pSystem);
	glFlush();			//Finish rendering
	glutSwapBuffers();
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,x,y);  //Use the whole window for rendering
}

void Idle(void)
{
	g_xRotated += g_deltaX;
	g_yRotated += g_deltaY;
	g_zRotated += g_deltaZ;

	pSystem.step(0.01);
	Display();
}

//handle "special" keyboard events (arrows,F1,F2,...)
void processSpecialKeys(int key, int x, int y) 
{
	/*GLUT_KEY_F1		F1 function key
	GLUT_KEY_F2		F2 function key
	GLUT_KEY_F3		F3 function key
	GLUT_KEY_F4		F4 function key
	GLUT_KEY_F5		F5 function key
	GLUT_KEY_F6		F6 function key
	GLUT_KEY_F7		F7 function key
	GLUT_KEY_F8		F8 function key
	GLUT_KEY_F9		F9 function key
	GLUT_KEY_F10		F10 function key
	GLUT_KEY_F11		F11 function key
	GLUT_KEY_F12		F12 function key
	GLUT_KEY_LEFT		Left function key
	GLUT_KEY_RIGHT		Up function key
	GLUT_KEY_UP		Right function key
	GLUT_KEY_DOWN		Down function key
	GLUT_KEY_PAGE_UP	Page Up function key
	GLUT_KEY_PAGE_DOWN	Page Down function key
	GLUT_KEY_HOME		Home function key
	GLUT_KEY_END		End function key
	GLUT_KEY_INSERT		Insert function key
	*/
	
	switch(key) {
		case GLUT_KEY_F1 : 
			g_deltaX *= 2;
			break;
		case GLUT_KEY_F2 : 
			g_deltaY *= 2;
			break;
		case GLUT_KEY_F3 : 
			g_deltaX /= 2;
			g_deltaY /= 2;
			break;
		case GLUT_KEY_F5 : 
			gOriginZ += 0.5;
			break;
		case GLUT_KEY_F6 : 
			gOriginZ -= 0.5;
			break;
	}
}
//handle "normal " keyboard events
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}
bool gLBDown = false;
bool gRBDown = false;
int gLastX;
int gLastY;
void processMouse(int button, int state, int x, int y) 
{
	// if we want both mouse and ALT (for example) use this instead of next line
	//specialKey = glutGetModifiers();
	//if ((state == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT)) {
	gLastX = x;
	gLastY = y;
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		gLBDown = true;
	}
	else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		gLBDown = false;
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		gRBDown = true;
	}
	else if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON) {
		gRBDown = false;
	}
}
void processMotion (int x, int y)
{
	int dx = (x-gLastX);
	int dy = (y-gLastY);
	if (gLBDown)
	{
		int dx = (x-gLastX);
		int dy = (y-gLastY);
		g_xRotated += dy;
		g_yRotated += dx;
	}
	if (gRBDown)
	{
		gOriginX += ((float)dx)*MOVEMENT_STEP;
		gOriginY -= ((float)dy)*MOVEMENT_STEP;
	}
	gLastX = x;
	gLastY = y;
}
int main (int argc, char **argv)
{
	//Load simulation stuff
    ClothLoader::Load( pSystem, std::string("finecloth.psim") );

	switch (pSystem.mSolverType)
	{
	case C_FORWARD_EULER_SOLVER:
	default:
			gFESolver = new ForwardEulerSolver(&pSystem);
			pSystem.setSolver(gFESolver);
		break;
	}

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  //For animations you should use double buffering
	glutInitWindowSize(500,400);
	//Create a window with rendering context and everything else we need
	glutCreateWindow("Cloth Simulation");
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FILL or GL_LINE
	g_xRotated = 60.0;
	g_yRotated = 20.0;
	g_zRotated = 0.0;
	glClearColor(0.0,0.0,0.0,0.0);
	//Assign the two used Msg-routines
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	
	//Keyboard/Mouse stuff
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);	
	glutMouseFunc(processMouse);
	glutMotionFunc(processMotion);
	//Let GLUT get the msgs
	glutMainLoop();
	delete gFESolver;
	return 0;
}

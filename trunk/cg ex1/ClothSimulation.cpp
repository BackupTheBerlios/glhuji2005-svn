//Include Files
#include <windows.h>
#include "stdafx.h"
#include <GL/glut.h>

#include "constants.h"
#include "ClothView.h"
#include "Vector3d.h"

//Prototypes for opengl callbacks
void displayCallback();
void reshapeCallback( int inWidth, int inHeight );
void mousePressedCallback( int inButton, int inState, int inX, int inY );
void mouseMovedCallback( int inX, int inY );
void keyPressedCallback( unsigned char inKey, int inX, int inY );
void timerCallback( int inVal );

//Lights settings
GLfloat light_ambient[]= { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position1[]= { -14.0f, -1.0f, -4.0f, 1.0f };
//Materials settings
GLfloat mat_ambient[]= { 0.3f, 0.3f, 0.3f, 0.0f };
GLfloat mat_diffuse[]= { 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat mat_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };

int main (int argc, char **argv)
{
    string simFile = string(C_DEFAULT_SIMULATION_FILE);
    bool   pauseAtStart = false;

    //---- Load Simulation File ----
    if( argc < 2 )
        cout << "No cloth file provided, trying to load default:" << simFile << endl;
    else
        simFile = string( argv[1] );

    //---- check for pause parameter ----
    if( argc >= 3 && (strcmp( argv[2], C_PAUSE_STRING ) == 0) )
    {
        pauseAtStart = true;
        cout << "pausing at start of execution" << endl;
    }

    ClothView::getInstance()->loadSimulation( simFile );

    if( pauseAtStart )
        ClothView::getInstance()->stop();


    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize( C_WINDOW_WIDTH, C_WINDOW_HEIGHT );

    //Create a window with rendering context and everything else we need
    glutCreateWindow("Cloth Simulation");
    glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
    glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	// shed some light
	glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
	//Materials initialization and activation
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_specular);
    glMaterialfv (GL_FRONT_AND_BACK, GL_POSITION, mat_shininess);

    //Assign callbacks
    glutDisplayFunc( displayCallback );
    glutReshapeFunc( reshapeCallback );
    glutMouseFunc( mousePressedCallback );
    glutMotionFunc( mouseMovedCallback );
    glutKeyboardFunc( keyPressedCallback );
    glutTimerFunc( C_TIMER_DELAY, timerCallback, 0 );

    //go!!!
    glutMainLoop();
    return 0;
}

//--------- OpenGL Callbacks ----------
void displayCallback()
{
    ClothView::getInstance()->display();
}

void reshapeCallback( int inWidth, int inHeight )
{
    ClothView::getInstance()->reshape( inWidth, inHeight );
}

void 
mousePressedCallback( int inButton, int inState, int inX, int inY )
{
    ClothView::getInstance()->mousePressed( inButton, inState, inX, inY );
}

void mouseMovedCallback( int inX, int inY )
{
    ClothView::getInstance()->mouseMoved( inX, inY );
}

void 
keyPressedCallback( unsigned char inKey, int inX, int inY )
{
    ClothView::getInstance()->keyPressed( inKey, inX, inY );
}

void timerCallback( int inVal )
{
    ClothView::getInstance()->timerCalled( inVal );
}
#include "StdAfx.h"
#include "Particle.h"
#include "ClothLoader.h"
#include "Clothview.h"
#include "constants.h"
#include "OpenGLUtilities.h"
#include <math.h>

void timerCallback( int inVal );

void 
ClothView::display()
{
    drawParticleSystem();

    //todo: remove completely
    //drawArcBallCircle();

    glFlush();
    glutSwapBuffers();
}

void 
ClothView::reshape( int inWidth, int inHeight )
{
    /*//Set a new projection matrix
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
    gluPerspective(40.0,(GLdouble)inWidth/(GLdouble)inHeight,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,inWidth,inHeight);*/
    
    
    mWindowWidth = inWidth;
    mWindowHeight = inHeight;

    //----- Calculate New Values for Radius and frustum -----
    mArcballRadius = 0.48 * min(inWidth,inHeight); // Setting arcball's radius

    //----- Frustum -----
    mFrustumDeg = 40;

    //----- Update Arcball Controller -----
    m3dController.setRadius( mArcballRadius );
    m3dController.setScreenCenter( Point2d( inWidth/2, inHeight/2 ) );

    //----- Set Viewport Geometry -----
    glViewport(0, 0, (GLsizei)inWidth, (GLsizei)inHeight);
}

void
ClothView::resetViewPoint()
{
    OpenGLUtilities::initTranslationMatrix( -2, 2, -12, mTransformation );

    mFrustumDeg   = 40;

    //todo: change to constants
    mNear         = 0.5;
    mFar          = 20;

    mCommitTransformation = false;
    mRotation             = Quaternion();
    mTranslation          = Vector3d();
}

void 
ClothView::mousePressed( int inButton, int inState, int inX, int inY )
{
    //---------------- Translate coordinates -----------------------
    int translatedX = inX;
    int translatedY = mWindowHeight-inY;

    //---------------- Mouse Up -----------------------
    if( inState == GLUT_UP ) 
    {
        if( mMode == MODE_ROTATE )
        {
            //Accumulate rotation
            commitTransform();
        }
        else if( mMode == MODE_TRANSLATE )
        {
            //doTranslate( translatedX, translatedY, false, false );
        }

        mMode = MODE_MOUSE_UP;
    }
    //---------------- Mouse Down -----------------------
    else
    {

        if( inButton == GLUT_LEFT_BUTTON )                            //Rotate
        {
            mMode = MODE_ROTATE;
            mMouseDownPt = Point2d( translatedX, translatedY );
        }
        else if (inButton == GLUT_RIGHT_BUTTON)                       //Translate
        {
            mMode = MODE_TRANSLATE;
            doTranslate( translatedX, translatedY, true, false );
        }
        else if (inButton == GLUT_MIDDLE_BUTTON)                      //Scale
        {
            mMode = MODE_SCALE;
            mMouseDownPt = Point2d( translatedX, translatedY );
        }
    }

    //---------------- Redraw WIndow -----------------------   
    redraw() ;	
}

void 
ClothView::mouseMoved( int inX, int inY )
{
    //---------------- Translate coordinates -----------------------
    int translatedX = inX;
    int translatedY = mWindowHeight-inY;

    //---------------- Rotate -----------------------
    if( mMode == MODE_ROTATE )
    {
        Point2d p = Point2d( translatedX,  translatedY );

        m3dController.getRotation( mMouseDownPt,  p, mRotation );
    }
    //---------------- Scale -----------------------
    else if( mMode == MODE_SCALE )
    {
        double temp =  (double)(translatedY - mMouseDownPt.getY())/C_SCALE_FACTOR;

        if( mFrustumDeg + temp < 180.0 )
            mFrustumDeg += temp;

        //limit
        if( mFrustumDeg < 0.1 ) mFrustumDeg = 0.1;
    }
    //---------------- Translate -----------------------
    else if( mMode == MODE_TRANSLATE )
    {
        doTranslate( translatedX, translatedY, false, true );
    }

    //---------------- Redraw -----------------------
    redraw();
}

void
ClothView::commitTransform()
{
    mCommitTransformation = true;
}

void
ClothView::doTranslate( int inX, int inY, bool inMouseDown, bool inMouseDrag )
{
    int x = inX;
    int y = inY;

    if( inMouseDown )
    {
        mMouseDownPt = Point2d( inX, inY );
    }
    else //mouse drag and mouse up
    {

        //Limit Mouse Coordinates to inside window bounds...
        if( inX < 0 ) x = 0;
        if( inX > mWindowWidth ) x = mWindowWidth;
        if( inY < 0 ) y = 0;
        if( inY > mWindowHeight ) y = mWindowHeight;

        Point2d a( inX, inY );
        a -= mMouseDownPt;
         mTranslation = Vector3d( a.getX()*C_TRANSLATION_FACTOR,
                                  a.getY()*C_TRANSLATION_FACTOR, 0 );

    }

    if( !inMouseDown && !inMouseDrag ) //mouse Up
        commitTransform();
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

        //reset viewpoing on 'r' keypress
        case 'r':
            resetViewPoint();
            redraw();
            break;

        //toggle wireframe/solid
        case 'w':
            mWireFrameMode = !mWireFrameMode;
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

void
ClothView::drawParticleSystem()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    gluPerspective( mFrustumDeg, (GLfloat)mWindowWidth/(GLfloat)mWindowHeight, 0.5, 20 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector3d v = mRotation.vec();
    double   t = mRotation.theta();
    glTranslated( -mClothCenter.pX, -mClothCenter.pY, -mClothCenter.pZ );
    glRotated( RAD2DEG(t), v.pX, v.pY, v.pZ );
    glTranslated( mClothCenter.pX, mClothCenter.pY, mClothCenter.pZ );

    //---------------- Perform Translation ---------------------
    glTranslated( mTranslation.pX, mTranslation.pY, mTranslation.pZ );

    //---------------- Perform Transformations that occurred till now -------
    glMultMatrixd( mTransformation );

    //committransform causes teh current transformation to be saved!
    if( mCommitTransformation )
    {
        //save transformation
        glGetDoublev( GL_MODELVIEW_MATRIX, mTransformation );

        mCommitTransformation = false;
        mRotation             = Quaternion();
        mTranslation          = Vector3d();
    }

    // Here we draw our mesh
    // ---------------------
    idx_t w = mParticleSystem.getWidth();
    idx_t h = mParticleSystem.getHeight();

    //only draw outline in wireframe mode
    if( mWireFrameMode )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    //do actual drawing
    for (int y=0; y<h-1; y++){
        for (int x=0; x<w-1; x++){

            {
            glColor3f(((float)x/(float)w)*0.5f+0.5f,
                ((float)(x+y)/(float)(w+h))*0.5f+0.5f,
                ((float)y/(float)h)*0.5f+0.5f);
            glBegin(GL_QUADS);

            //draw in CCW order!
            Vector3d &p1 = mParticleSystem.getParticlePos(x, y);
            Vector3d &p2 = mParticleSystem.getParticlePos(x, y+1);
            Vector3d &p3 = mParticleSystem.getParticlePos(x+1, y+1);
            Vector3d &p4 = mParticleSystem.getParticlePos(x+1, y);
            glVertex3f( p1.pX, p1.pY, p1.pZ);
            glVertex3f( p2.pX, p2.pY, p2.pZ);
            glVertex3f( p3.pX, p3.pY, p3.pZ);
            glVertex3f( p4.pX, p4.pY, p4.pZ);
            glEnd();
            }
        }
    }
}

void
ClothView::drawArcBallCircle()
{
    glColor3f( 0, 1.0, 0 );
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluOrtho2D(0, (GLdouble)mWindowWidth, 0, (GLdouble)mWindowHeight) ;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    OpenGLUtilities::drawCircle( mArcballRadius, mWindowWidth/2, mWindowHeight/2, 0.0, 3.0  );
}


ClothView::ClothView( )
{
    mWindowWidth          = C_WINDOW_WIDTH;
    mFrustumDeg           = C_DEFAULT_FRUSTRUM_ANGLE;
    mWindowHeight         = C_WINDOW_HEIGHT;
    mArcballRadius        = 0;
    mIsRunning            = true;
    mMode                 = MODE_MOUSE_UP;
    mTranslation          = Vector3d();
    mCommitTransformation = false;
    mWireFrameMode        = true;

    resetViewPoint();
}

void 
ClothView::loadSimulation( string &inFileName )
{
    ClothLoader::Load( mParticleSystem, inFileName );
    mParticleSystem.getSpatialDimensions( mClothCenter, mClothRadius );

    resetViewPoint();
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
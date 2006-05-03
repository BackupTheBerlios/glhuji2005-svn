#ifndef __CLOTH_VIEW_H__
#define __CLOTH_VIEW_H__

#include <string>
#include "ParticleSystem.h"
#include "ArcBallController.h"
#include <GL/glut.h>

using namespace std;

class ClothView
{
//--------------- Internal Data Types ---------
protected:
    typedef enum {
        MODE_SCALE,
        MODE_ROTATE,
        MODE_TRANSLATE,
        MODE_MOUSE_UP
    } PROGRAM_STATE;

public:
    static ClothView *getInstance();

    void loadSimulation( string &inFileName );

    //stop simulation
    void stop();
    
    //Functions called by opengl
    void display();
    void reshape( int inWidth, int inHeight );
    void mousePressed( int inButton, int inState, int inX, int inY );
    void mouseMoved( int inX, int inY );
    void keyPressed( unsigned char inKey, int inX, int inY );
    void timerCalled( int inVal );

protected:
    void drawParticleSystem();
    void drawArcBallCircle();

    void commitTransform();
    void doTranslate( int inX, int inY, bool inMouseDown, bool inMouseDrag );

    void resetViewPoint();

    //redraw our view
    void redraw();

    ClothView();
    virtual ~ClothView();


protected:
    ParticleSystem    mParticleSystem;
    ArcBallController m3dController;
    int               mWindowWidth;
    int               mWindowHeight;
    bool              mWireFrameMode; //wire frame/solid toggle
    double            mFrustumDeg;
    double            mArcballRadius;
    Vector3d          mClothCenter;
    double            mClothRadius;
    bool              mIsRunning; //true = running false = paused
    PROGRAM_STATE     mMode;
    Point2d           mMouseDownPt;
    Quaternion        mRotation;
    double            mNear;
    double            mFar;
    GLdouble          mTransformation[16]; //saved GL Transformation matrix
    Vector3d          mTranslation;
    bool              mCommitTransformation;

//Singleton storage
protected:
    static ClothView *mInstance;
};

#endif //__CLOTH_VIEW_H__
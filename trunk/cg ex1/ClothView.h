#ifndef __CLOTH_VIEW_H__
#define __CLOTH_VIEW_H__

#include <string>
#include "ParticleSystem.h"
#include "ArcBallController.h"

using namespace std;

class ClothView
{
public:
    static ClothView *getInstance();

    void loadSimulation( string &inFileName );
    
    //Functions called by opengl
    void display();
    void reshape( int inWidth, int inHeight );
    void mousePressed( int inButton, int inState, int inX, int inY );
    void mouseMoved( int inX, int inY );
    void keyPressed( unsigned char inKey, int inX, int inY );
    void timerCalled( int inVal );

protected:
    void drawParticleSystem();

    //redraw our view
    void redraw();

    ClothView();
    virtual ~ClothView();


protected:
    ParticleSystem    mParticleSystem;
    ArcBallController m3dController;
    int               mWindowWidth;
    int               mWindowHeight;

//Singleton storage
protected:
    static ClothView *mInstance;
};

#endif //__CLOTH_VIEW_H__
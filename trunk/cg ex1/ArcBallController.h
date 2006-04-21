/*	Login: playmobil	ID. 036382489

    FileName: ArcBallController.h

    Implementation of the Arcball Controller
*/

#ifndef __ARCBALL_CONTROLLER_H__
#define __ARCBALL_CONTROLLER_H__

//--------------- Includes ---------
#include "Quaternion.h"
#include "Point2d.h"
#include "Vector3d.h"

//------------------------
// ArcBallController
// 
// Implementation of Arcball 3d controller, including option to constrain
// rotation to x,y, or z axes.
//------------------------
class ArcBallController
{

//--------------- Internal Data Types ---------
protected:
    typedef enum {
        NO_CONSTRAINT,
        X_CONSTRAINT,
        Y_CONSTRAINT,
        Z_CONSTRAINT
    } AxisConstraint;

//--------------- Public Function Prototypes ---------
public:

    //default constructor
    ArcBallController();

    //construct with specified radius.
    ArcBallController( double inRadius );

    //sets the center of the arcball in screen coordinates.
    void setScreenCenter( Point2d inScreenCenter );

    //sets the arcball's radius in pixels.
    void setRadius( double inRadius );

    //Given 2 points, returns a quaternion representing the rotation.
    void getRotation( Point2d inMouseStart, Point2d inMouseEnd, 
                              Quaternion &outRotation );

    //toggle x,y, & z rotation constraints.
    void     toggleXConstraint();
    void     toggleYConstraint();
    void     toggleZConstraint();

    //removes any rotation constraints.
    void     resetConstraints();

//--------------- Internal Methods ---------
protected:
    //used in constraint process.
    Vector3d getConstraintVector();

    //converts a 2d screen coordinate to one on the arcball.
    Vector3d convertToBallCoord( Point2d inPt );

//--------------- Member Variables ---------
protected:
    double          mRadius;            //Arcball radius in pixels.
    Point2d         mScreenCenter;      //Screen center in pixels.
    AxisConstraint  mConstraint;        //Current rotation constraint.
};
#endif //__ARCBALL_CONTROLLER_H__

/*	Login: playmobil	ID. 036382489

    FileName: ArcBallController.cpp

    Implementation of the Arcball Controller
*/

//------------ Includes ----------------
#include "stdafx.h"
#include "ArcBallController.h"
#include <math.h>

using namespace std;

ArcBallController::ArcBallController()
{
    mRadius       = -1;
    mScreenCenter = Point2d( 0,0 );
    mConstraint   = NO_CONSTRAINT;
}

ArcBallController::ArcBallController( double inRadius ) :
            mRadius( inRadius), mScreenCenter( 0, 0 )
{
    mConstraint = NO_CONSTRAINT;
}

void 
ArcBallController::getRotation( Point2d inMouseStart, Point2d inMouseEnd, Quaternion &outRotation )
{
    Vector3d mouseStart = convertToBallCoord( inMouseStart );
    Vector3d mouseEnd   = convertToBallCoord( inMouseEnd );

    //Constrain Rotation by multiplying by a vector of 1s with a zero
    //in the position corresponding to the axis we want to constrain
    //around.
    Vector3d constraint = getConstraintVector();

    mouseStart *= constraint;
    mouseEnd   *= constraint;

    mouseStart.normalize();
    mouseEnd.normalize();


    //% means cross product in the case of a Vector3d - operator overloading
    //in C++ sucks when it comes to readability!
    Vector3d rot = mouseStart.cross(mouseEnd);

    outRotation.set( rot.pX, rot.pY, rot.pZ, mouseStart.dot(mouseEnd) );
}

Vector3d
ArcBallController::convertToBallCoord( Point2d inPt )
{
    inPt -= mScreenCenter;

    double x = inPt.getX();
    double y = inPt.getY();
    double z = 0;

    //if x,y are inside ball, z coordinate is not zero.
    if( (x < mRadius) && (y < mRadius) && (sqrt((double)(x*x+y*y)) < mRadius) )
        z = sqrt( mRadius*mRadius - x*x - y*y );

    return Vector3d( x, y, z );
}

void
ArcBallController::setScreenCenter( Point2d inScreenCenter )
{
    mScreenCenter = inScreenCenter;
}

void 
ArcBallController::setRadius( double inRadius )
{
    mRadius = inRadius;
}

Vector3d
ArcBallController::getConstraintVector()
{
    Vector3d ret( 1.0, 1.0, 1.0 );

    if( mConstraint == X_CONSTRAINT )
        ret.pX = 0;
    else if( mConstraint == Y_CONSTRAINT )
        ret.pY = 0;
    else if( mConstraint == Z_CONSTRAINT )
        ret.pZ = 0;

    return ret;
}

void 
ArcBallController::toggleXConstraint()
{
    if( mConstraint != X_CONSTRAINT )
    {
        mConstraint = X_CONSTRAINT;
        cout << "Rotations Constrained to X Axis" << endl;
    }
    else
    {
        mConstraint = NO_CONSTRAINT;
        cout << "Rotations Constraints canceled" << endl;
    }
}

void 
ArcBallController::toggleYConstraint()
{
    if( mConstraint != Y_CONSTRAINT )
    {
        mConstraint = Y_CONSTRAINT;
        cout << "Rotations Constrained to Y Axis" << endl;
    }
    else
    {
        mConstraint = NO_CONSTRAINT;
        cout << "Rotations Constraints canceled" << endl;
    }
}

void 
ArcBallController::toggleZConstraint()
{
    if( mConstraint != Z_CONSTRAINT )
    {
        mConstraint = Z_CONSTRAINT;
        cout << "Rotations Constrained to Z Axis" << endl;
    }
    else
    {
        mConstraint = NO_CONSTRAINT;
        cout << "Rotations Constraints canceled" << endl;
    }
}

void 
ArcBallController::resetConstraints()
{
    mConstraint = NO_CONSTRAINT;
}

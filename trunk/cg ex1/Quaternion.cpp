/*	Login: playmobil	ID. 036382489
    Login:gutmant       ID. 039777289


    FileName: Quaternion.cpp

    A class representing a quaterion
*/
#include "stdafx.h"
#include "Quaternion.h"
#include "Vector3d.h"
#include <math.h>

Quaternion::Quaternion() :
        mX(0), mY(0), mZ(0), mW(1.0)
{
}

Quaternion::Quaternion( double inX, double inY, 
                       double inZ, double inW ) :
        mX( inX ), mY( inY ), mZ( inZ ), mW( inW )
{
}

void
Quaternion::set( double inX, double inY, 
                double inZ, double inW )
{
    mX = inX;
    mY = inY;
    mZ = inZ;
    mW = inW;
}

double
Quaternion::theta()
{
    return 2.0 * acos( mW );
}

Vector3d 
Quaternion::vec()
{
    return Vector3d( mX, mY, mZ );
}

Quaternion 
Quaternion::operator*(const Quaternion &rhs)
{
    Quaternion ret;

    ret.mX = mW*rhs.mX + mX*rhs.mW + mY*rhs.mZ - mZ*rhs.mY;
    ret.mY = mW*rhs.mY + mY*rhs.mW + mZ*rhs.mX - mX*rhs.mZ;
    ret.mZ = mW*rhs.mZ + mZ*rhs.mW + mX*rhs.mY - mY*rhs.mX;
    ret.mW = mW*rhs.mW - mX*rhs.mX - mY*rhs.mY - mZ*rhs.mZ;
    
    return ret;
}

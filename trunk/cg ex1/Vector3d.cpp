#include "StdAfx.h"
#include "Vector3d.h"
#include <math.h>

Vector3d::Vector3d()
{
    Vector3d( 0, 0, 0 );
}

Vector3d::~Vector3d(void)
{
}

Vector3d::Vector3d( double inX, double inY, double inZ )
{
    pX = inX; pY = inY; pZ = inZ;
}
void Vector3d::set(double inX, double inY, double inZ)
{
	pX = inX; pY = inY; pZ = inZ;
}

Vector3d 
Vector3d::cross( Vector3d &rhs )
{
   return Vector3d(
             pY*rhs.pZ-pZ*rhs.pY,
             pZ*rhs.pX-pX*rhs.pZ,
             pX*rhs.pY-pY*rhs.pX
        );
}

double 
Vector3d::dot( Vector3d &rhs )
{
    return (pX*rhs.pX)+(pY*rhs.pY)+(pZ*rhs.pZ);
}

void 
Vector3d::normalize()
{
    double fac = sqrt( (pX*pX)+(pY*pY)+(pZ*pZ) );

    pX /= fac;
    pY /= fac;
    pZ /= fac;
}

double 
Vector3d::length()
{
    return sqrt( (pX*pX)+(pY*pY)+(pZ*pZ) );
}

Vector3d
Vector3d::plus( Vector3d &rhs )
{
    return Vector3d( pX + rhs.pX, pY + rhs.pY, pZ + rhs.pZ );
}

void 
Vector3d::operator*=( Vector3d &rhs )
{
    pX *= rhs.pX;
    pY *= rhs.pY;
    pZ *= rhs.pZ;
}

void 
Vector3d::operator=( Vector3d &rhs )
{
    pX = rhs.pX;
    pY = rhs.pY;
    pZ = rhs.pZ;
}

//todo: cleanup
/*Vector3d &
Vector3d::operator-( const Vector3d &lhs, const Vector3d &rhs )
{
    return Vector3d( lhs.pX - rhs.pX, lhs.pY - rhs.pY, lhs.pZ - rhs.pZ );
}*/
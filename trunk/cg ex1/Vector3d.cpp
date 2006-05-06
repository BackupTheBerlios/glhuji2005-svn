#include "StdAfx.h"
#include "Vector3d.h"
#include <math.h>
#include <iostream> //debug

using namespace std;

Vector3d::Vector3d() :
    pX(0),pY(0),pZ(0)
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

double 
Vector3d::dist( Vector3d &rhs )
{
    return sqrt(
        SQR(pX-rhs.pX) +
        SQR(pY-rhs.pY) +
        SQR(pZ-rhs.pZ)
        );
}

void 
Vector3d::normalize()
{
    double fac = length();

	if( fac != 0 )
	{
		pX /= fac;
		pY /= fac;
		pZ /= fac;
	}
}


Vector3d 
Vector3d::normalized()
{
	Vector3d ret (pX, pY, pZ);
	ret.normalize();
	return ret;
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

Vector3d Vector3d::operator-( Vector3d &rhs )
{
	Vector3d ret;
	ret.pX = pX - rhs.pX;
	ret.pY = pY - rhs.pY;
	ret.pZ = pZ - rhs.pZ;
	return ret;
}

Vector3d 
Vector3d::operator+( Vector3d &rhs )
{
	return Vector3d( pX + rhs.pX, pY + rhs.pY, pZ + rhs.pZ );
}

Vector3d 
Vector3d::operator*( double rhs )
{
	Vector3d ret;
	ret.pX = pX * rhs;
	ret.pY = pY * rhs;
	ret.pZ = pZ * rhs;
	return ret;
}

Vector3d 
Vector3d::operator/( double rhs )
{
    return Vector3d( pX / rhs, pY / rhs, pZ / rhs );
}


void 
Vector3d::operator-=( Vector3d &rhs )
{
	pX -= rhs.pX;
	pY -= rhs.pY;
	pZ -= rhs.pZ;
}

void 
Vector3d::operator+=( Vector3d &rhs )
{
	pX += rhs.pX;
	pY += rhs.pY;
	pZ += rhs.pZ;
}

void 
Vector3d::operator*=( Vector3d &rhs )
{
    pX *= rhs.pX;
    pY *= rhs.pY;
    pZ *= rhs.pZ;
}

void
Vector3d::operator*=( double rhs )
{
    pX *= rhs;
    pY *= rhs;
    pZ *= rhs;
}

void 
Vector3d::operator=( Vector3d &rhs )
{
    pX = rhs.pX;
    pY = rhs.pY;
    pZ = rhs.pZ;
}

void
Vector3d::print()
{
    cout << "(" << pX << ", " << pY << ", " << pZ << ")" << endl;
}

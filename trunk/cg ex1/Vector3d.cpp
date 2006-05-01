#include "StdAfx.h"
#include "Vector3d.h"
#include <math.h>
#include <iostream> //debug

using namespace std;

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

Vector3d 
Vector3d::proj( Vector3d &rhs )
{
    //return rhs.normalized() * dot(rhs);
	return rhs.normalized() * (dot(rhs)/rhs.length());
}

double 
Vector3d::dot( Vector3d &rhs )
{
    return (pX*rhs.pX)+(pY*rhs.pY)+(pZ*rhs.pZ);
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
    double fac = length();

	Vector3d ret;
	if (fac == 0)
	{
		ret.pX = ret.pY = ret.pZ = 0.0;
	}
	else
	{
		ret.pX = pX / fac;
		ret.pY = pY / fac;
		ret.pZ = pZ / fac;
	}
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

bool 
Vector3d::sameAs( Vector3d &rhs )
{
    return( abs(pX - rhs.pX) < 0.001 &&
            abs(pY - rhs.pY) < 0.001 &&
            abs(pZ - rhs.pZ) < 0.001 );
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

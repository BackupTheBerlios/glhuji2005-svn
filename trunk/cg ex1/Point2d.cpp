#include "StdAfx.h"
#include "Point2d.h"

Point2d::Point2d()
{
    Point2d(0,0);
}

Point2d::Point2d(double inX, double inY)
{
    mX = inX;
    mY = inY;
}

void
Point2d::operator-=( Point2d &rhs )
{
    mX -= rhs.mX;
    mY -= rhs.mY;
}

double 
Point2d::getX()
{
    return mX;
}

double 
Point2d::getY()
{
    return mY;
}
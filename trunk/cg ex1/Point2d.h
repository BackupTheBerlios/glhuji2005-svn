#ifndef __POINT_2D_H__
#define __POINT_2D_H__

class Point2d
{
public:
    Point2d();
    Point2d(double inX, double inY);

    void operator-=( Point2d &rhs );

    double getX();
    double getY();

protected:
    double mX, mY;
};

#endif __POINT_2D_H__
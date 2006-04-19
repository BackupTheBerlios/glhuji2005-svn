/*	Login: playmobil	ID. 036382489
    Login:gutmant       ID. 039777289


    FileName: Quaternion.cpp

    A class representing a quaternion
*/

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

class Vector3d;

//------------------------
// Quaternion
// 
// Very simple class representing a quaternion.
//------------------------
class Quaternion
{
//--------------- Public Function Prototypes ---------
public:

    //Create a quaternion specifying a zero rotation.
    Quaternion();

    //create a quaternion with specified values.
    Quaternion( double inX, double inY, double inZ, double inW );

    //set the values of a quaternion
    void set( double inX, double inY, double inZ, double inW );

    // rotate this followed by rhs rotation
    Quaternion operator*(const Quaternion &rhs);

    //return rotation angle (2 * acos(mW)
    double   theta();

    //return rotation vector (mX, mY, mZ)
    Vector3d vec();

protected:
    double mX, mY, mZ, mW;
};

#endif //__QUATERNION_H__

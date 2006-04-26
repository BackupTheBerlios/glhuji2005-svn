#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

class Vector3d
{
public:
    Vector3d();
    Vector3d( double inX, double inY, double inZ );
    virtual ~Vector3d();

	void set(double inX, double inY, double inZ);

    //perform cross operation
    Vector3d cross( Vector3d &rhs );

    //project this vector onto vector passed in
    Vector3d proj( Vector3d &rhs );

    //dot product
    double dot( Vector3d &rhs );

    //add this vector to another vector and return the result
    Vector3d plus( Vector3d &rhs );

    //normalize this vector.
    void normalize();
	Vector3d normalized();

    //returns vector length
    double length();

	Vector3d operator-( Vector3d &rhs );
	Vector3d operator+( Vector3d &rhs );
	Vector3d operator*( double rhs );
    void operator+=( Vector3d &rhs );
    void operator-=( Vector3d &rhs );
    void operator*=( Vector3d &rhs );
    void operator*=( double rhs );
    void operator=( Vector3d &rhs );

    //todo: cleanup
    //Vector3d &operator-( const Vector3d &lhs, const Vector3d &rhs );

    //for readability, make these publicly accessible
public:
    double pX, pY, pZ;
};

#endif //__VECTOR_3D_H__
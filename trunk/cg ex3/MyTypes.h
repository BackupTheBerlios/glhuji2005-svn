#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <vector>
#include <math.h>

typedef OpenMesh::Vec3d Point3d;
typedef OpenMesh::Vec3d Vector3d;
typedef OpenMesh::Vec3d Color3d;
typedef OpenMesh::Vec4d Color4d;

typedef std::vector<Point3d> PointVec;
typedef PointVec::iterator   PointVecIt;
typedef std::vector<double> DoubleVec;

//Channel Mappings
typedef enum {
    INVALID_PLCMNT,
    XROT, YROT, ZROT,
    XPOS, YPOS, ZPOS
} ParamPlacement;


inline void RotateYZ(double headingy,double headingz, Point3d &p)
{
	Point3d pt = p;
	//rotate around Y axis
	p[0] = pt[2]*sin(headingy) + pt[0]*cos(headingy);		//x' = z*sin q1 + x*cos q1
	p[2] = pt[2]*cos(headingy) - pt[0]*sin(headingy);		//z' = z*cos q1 - x*sin q1
	//y' = y - no change

	pt = p;
	//rotate around Z axis
	p[0] = pt[0]*cos(headingz) - pt[1]*sin(headingz);		//x' = x*cos q2 - y*sin q2
	p[1] = pt[0]*sin(headingz) + pt[1]*cos(headingz);		//y' = x*sin q2 + y*cos q2
	//z' = z
}

inline double round(double d)
{
	if (floor(d)+0.5 <= d)
		return ceil(d);
	else
		return floor(d);
}

inline double frand()
{
	return ((double)rand())/((double)RAND_MAX);
}


#endif //__MY_TYPES_H__
#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <vector>

typedef OpenMesh::Vec3d Point3d;
typedef OpenMesh::Vec3d Vector3d;
typedef OpenMesh::Vec3d Color3d;

typedef std::vector<Point3d> PointVec;
typedef PointVec::iterator   PointVecIt;
typedef std::vector<double> DoubleVec;

//Channel Mappings
typedef enum {
    INVALID_PLCMNT,
    XROT, YROT, ZROT,
    XPOS, YPOS, ZPOS
} ParamPlacement;

#endif //__MY_TYPES_H__
#ifndef __CONV_MOTION_FILTER_H__
#define __CONV_MOTION_FILTER_H__

#include "MyTypes.h" 

class ConvMotionFilter
{
public:
	ConvMotionFilter();
	~ConvMotionFilter();

	// interface methods

	//set the convolution vectors externally
	void setConvVector(const PointVec& inRotationVec, const PointVec& inOffsetVec,
										double inMaskFactor, double inIntensity );

	bool applyFilter(PointVec& inRotations, PointVec& inOffsets,
							 PointVec& inRotationDiff, PointVec& inOffsetDiff,
							 PointVec& outRotations, PointVec& outOffsets);

protected:
	double fixRotation(const double inVal);
	Point3d fixRotation(Point3d inPoint);

	double subRotation(double inPointA, double inPointB);

	bool applyConv(const PointVec& inVector, const PointVec& inVectorDiff, bool inIsRotation, PointVec& inConvVector,
							 PointVec& outVector);

protected:
	PointVec mOffsetVec; //Convolution Mask for Offsets
	Point3d  mOffsetUTH; //upper threshold for offsets after filtering
	Point3d  mOffsetLTH; //lower threshold for offsets after filtering
	
	PointVec mRotationVec; //Convolution Mask for Rotations
	Point3d  mRotationUTH; //upper threshold for rotation angles after filtering
	Point3d  mRotationLTH; //lower thershold for rotatoin angles after filtering

	double    mIntensity;
	double   mMaskFactor; //factor to use for unsharp mask if 0 don't perform mask

};
#endif __CONV_MOTION_FILTER_H__

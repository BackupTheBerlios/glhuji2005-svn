#pragma once
#include "basemotionfilter.h"

class ConvMotionFilter :
	public BaseMotionFilter
{
	PointVec _OffsetVec;
	Point3d _OffsetUTH;
	Point3d _OffsetLTH;
	PointVec _RotationVec;
	Point3d _RotationUTH;
	Point3d _RotationLTH;
	bool _ConvolveMask;
public:
	ConvMotionFilter(void);
	~ConvMotionFilter(void);

	// interface methods
	int setConvVector(const PointVec& inRotationVec, const PointVec& inOffsetVec);
	virtual bool loadFilter(double inIntensity);
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets,
							 PointVec& inRotationDiff, PointVec& inOffsetDiff,
							 PointVec& outRotations, PointVec& outOffsets);

	bool applyConv(const PointVec& inVector, const PointVec& inVectorDiff, bool inIsRotation, PointVec& inConvVector,
							 PointVec& outVector);
};

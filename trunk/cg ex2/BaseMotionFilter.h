#pragma once

#include "mytypes.h" 

class BaseMotionFilter
{
public:
	enum MotionFilterType { MF_SMOOTH, MF_SHARPEN };

	BaseMotionFilter(void);
	~BaseMotionFilter(void);

	// static initializer
	static BaseMotionFilter* createFilter(MotionFilterType inType);
	static void convolutePoints(PointVec& inPoints, DoubleVec& inFilter, PointVec& outPoints, bool isAngles);
	static void sumAngles(Point3d& inP1, Point3d& inP2);
	static void fixAngles(Point3d& inPoint);
	static inline bool isNegative(double inVal) { return (inVal < 0); }

	// interface methods
	virtual bool loadFilter(double inIntensity) = 0;
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& outRotations, PointVec& outOffsets) = 0;

protected:
	DoubleVec mFilter;	//DE convolution
};

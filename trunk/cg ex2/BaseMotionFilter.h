#pragma once

#include "mytypes.h" 

class BaseMotionFilter
{
public:
	enum MotionFilterType { MF_SMOOTH, MF_SHARPEN, MF_CONVOLVE };

	BaseMotionFilter(void);
	~BaseMotionFilter(void);

	// static initializer
	static BaseMotionFilter* createFilter(MotionFilterType inType);
	static void convolutePoints(PointVec& inPoints, DoubleVec& inFilter, PointVec& outPoints, bool isAngles);
	static void sumAngles(Point3d& inP1, Point3d& inP2);
	static void fixAngles(Point3d& inPoint);
	static inline bool isNegative(double inVal) { return (inVal < 0); }

	static double fixRotation(const double inPoint);
	static double sumRotation(const double inPointA, const double inPointB);
	static double subRotation(const double inPointA, const double inPointB);
	static Point3d fixRotation(const Point3d inPoint);
	static Point3d sumRotation(const Point3d inPointA, const Point3d inPointB);
	static Point3d subRotation(const Point3d inPointA, const Point3d inPointB);
	// interface methods
	virtual bool loadFilter(double inIntensity) = 0;
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& inRotationDiff, PointVec& inOffsetDiff,
		PointVec& outRotations, PointVec& outOffsets) = 0;

protected:
	double mIntensity;
	DoubleVec mFilter;	//DE convolution
};

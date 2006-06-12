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

	// interface methods
	virtual bool loadFilter(double inIntensity) = 0;
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& outRotations, PointVec& outOffsets) = 0;
};

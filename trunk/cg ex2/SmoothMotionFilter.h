#pragma once
#include "basemotionfilter.h"

class SmoothMotionFilter :
	public BaseMotionFilter
{
public:
	SmoothMotionFilter(void);
	~SmoothMotionFilter(void);

	// interface methods
	virtual bool loadFilter(double inIntensity);
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& outRotations, PointVec& outOffsets);

private:
	int mIntensity;
};

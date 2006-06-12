#pragma once
#include "basemotionfilter.h"

class SharpenMotionFilter :
	public BaseMotionFilter
{
public:
	SharpenMotionFilter(void);
	~SharpenMotionFilter(void);

	// interface methods
	virtual bool loadFilter(double inIntensity){ return true; }
	virtual bool applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& outRotations, PointVec& outOffsets){ return true; }//TODO!!!
};

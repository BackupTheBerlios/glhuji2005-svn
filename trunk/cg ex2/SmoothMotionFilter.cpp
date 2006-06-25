#include "StdAfx.h"
#include ".\smoothmotionfilter.h"

SmoothMotionFilter::SmoothMotionFilter(void)
{
	mIntensity = 2;
}

SmoothMotionFilter::~SmoothMotionFilter(void)
{
}

bool SmoothMotionFilter::loadFilter(double inIntensity)
{
	if (inIntensity < 0){
		cerr << "Smooth filter cannot handle non-positive values\n";
		return false;
	}
	mIntensity = (int) inIntensity;
}

bool SmoothMotionFilter::applyFilter(PointVec& inRotations, PointVec& inOffsets, 
		PointVec& outRotations, PointVec& outOffsets)
{
	int nFrames = (int)inRotations.size();
	assert(inOffsets.size()==nFrames);

	int smoothRadius = mIntensity;
	if (smoothRadius > nFrames/2)
	{
		smoothRadius = (int)nFrames/2;
	}

	int index=-1;
	int smoothSize = 2*smoothRadius + 1;

	outRotations.clear();
	outOffsets.clear();
	Point3d sumRot;
	Point3d sumOff;
	Point3d finalRot;
	Point3d finalOff;
	for (int i=0; i<nFrames; i++)
	{
		sumRot[0] = sumRot[1] = sumRot[2] = 0;
		sumOff[0] = sumOff[1] = sumOff[2] = 0;
		for (int j=-smoothRadius; j<=smoothRadius; j++)
		{
			//TODO:
			// (1) switch to the convolution method
			// (2) there's a problem in averaging degrees since 179 and -179 become 0 and not 180
			index = i+j;
			if (index<0) {
				index=nFrames+index;
			}
			else if (index>=nFrames){
				index = index-nFrames;
			}
			sumAngles(sumRot, inRotations[index]);
			sumOff += inOffsets[index];
		}
		finalRot = sumRot/smoothSize;
		fixAngles(finalRot);
		finalOff = sumOff/smoothSize;
		outRotations.push_back(finalRot);
		outOffsets.push_back(finalOff);
	}

	return true;
}
#include "StdAfx.h"
#include ".\basemotionfilter.h"
#include ".\SmoothMotionFilter.h"
#include ".\SharpenMotionFilter.h"

BaseMotionFilter::BaseMotionFilter(void)
{
}

BaseMotionFilter::~BaseMotionFilter(void)
{
}

BaseMotionFilter* BaseMotionFilter::createFilter(MotionFilterType inType)
{
	BaseMotionFilter* pBaseFilter=NULL;
	switch (inType)
	{
	case MF_SMOOTH:
		pBaseFilter = new SmoothMotionFilter();
		break;
	case MF_SHARPEN:
		pBaseFilter = new SharpenMotionFilter();
		break;
	default:
		cerr << "Unrecognized filter type!!!\n";
		break;
	}

	return pBaseFilter;
}

void BaseMotionFilter::convolutePoints(PointVec& inPoints, DoubleVec& inFilter, PointVec& outPoints)
{
	int index=0;
	int filterSize = inFilter.size();
	int pointsSize = inPoints.size();
	Point3d sumPoint;
	outPoints.clear();
	for (int i=0; i<pointsSize; i++)
	{
		sumPoint[0] = sumPoint[1] = sumPoint[2] = 0;
		for (int j=0; j<filterSize; j++)
		{
			index = i-j;
			if (index >= 0 && index < pointsSize)
			{
				sumPoint += (inPoints[index] * inFilter[j]);
			}
		}
		outPoints.push_back(sumPoint);
	}
}
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

void BaseMotionFilter::convolutePoints(PointVec& inPoints, DoubleVec& inFilter, PointVec& outPoints, bool isAngles)
{
	int index=0;
	int filterSize = (int)inFilter.size();
	int pointsSize = (int)inPoints.size();
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
				if (isAngles){
					sumAngles(sumPoint, inPoints[index] * inFilter[j]);
				}
				else{
					sumPoint += (inPoints[index] * inFilter[j]);
				}
			}
		}
		outPoints.push_back(sumPoint);
	}
}

void BaseMotionFilter::sumAngles(Point3d& inP1, Point3d& inP2)
{
	Point3d p2 = inP2;
	for (int i=0; i<=2; i++){
		if (isNegative(p2[i]) != isNegative(inP1[i])){
			if (p2[i] < -90){
				p2[i] = 360 + p2[i];
			}
			else if (p2[i] > 90){
				p2[i] = -360 + p2[i];
			}
		}

		inP1[i] += p2[i];
	}
}

void BaseMotionFilter::fixAngles(Point3d& inPoint)
{
	for (int i=0; i<=2; i++){
		if (inPoint[i] < -180){
			inPoint[i] = -360 - inPoint[i];
		}
		else if (inPoint[i] > 180){
			inPoint[i] = 360 - inPoint[i];
		}

	}
}
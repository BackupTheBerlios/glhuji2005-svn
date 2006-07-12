#include "StdAfx.h"
#include ".\basemotionfilter.h"
#include ".\ConvMotionFilter.h"

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
//		pBaseFilter = new SmoothMotionFilter();
		break;
	case MF_SHARPEN:
//		pBaseFilter = new SharpenMotionFilter();
		break;
	case MF_CONVOLVE:
		pBaseFilter = new ConvMotionFilter();
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


double BaseMotionFilter::fixRotation(const double inVal)
{
	double ret = inVal;
	while (ret < 0)
		ret = 360+ret;
	while (ret >= 360)
		ret = ret-360;
	/*
	if (ret < 0)
		ret += (double)((abs(((int)ret)/360)+1)*360);
	if (ret >= 360)
		ret -= (double)((abs(((int)ret)/360))*360);
		*/
	return ret;
}
double BaseMotionFilter::sumRotation(const double inPointA, const double inPointB)
{
	double ret = fixRotation(inPointA) + fixRotation(inPointB);
	return fixRotation(ret);
}
double BaseMotionFilter::subRotation(const double inPointA, const double inPointB)
{
	double ret = fixRotation(inPointA) - fixRotation(inPointB);
	return fixRotation(ret);
}

Point3d BaseMotionFilter::fixRotation(const Point3d inPoint)
{
	Point3d ret = inPoint;
	for (int i=0; i<=2; i++)
	{
		ret[i] = fixRotation(ret[i]);/*
		while (ret[i] < 0)
			ret[i] += 360;
		while (ret[i] >= 360)
			ret[i] -= 360;*/
	}
	return ret;
}

Point3d BaseMotionFilter::sumRotation(const Point3d inPointA, const Point3d inPointB)
{
	Point3d ret = fixRotation(inPointA) + fixRotation(inPointB);
	return fixRotation(ret);
}
Point3d BaseMotionFilter::subRotation(const Point3d inPointA, const Point3d inPointB)
{
	Point3d ret = fixRotation(inPointA) - fixRotation(inPointB);
	return fixRotation(ret);
}

void BaseMotionFilter::sumAngles(Point3d &inP1, Point3d& inP2)
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
#include "StdAfx.h"
#include "ConvMotionFilter.h"

#define pi 3.14159265358978
#define DEG(r) ((r)*180.0f/pi)
#define RAD(d) ((d)*pi/180.0f)

ConvMotionFilter::ConvMotionFilter(void)
{
	mIntensity  = 0;
	mMaskFactor = 0;

	mOffsetUTH   = Point3d(10001000,10001000,10001000);	//big - allow free movement
	mOffsetLTH   = Point3d(0,0,0);      //zero threshold - always filter
	mRotationUTH = Point3d(44,44,44); //Do not allow more then 12 degrees rotation per DOF per joint pre frame
	mRotationLTH = Point3d(0,0,0);    //zero threshold - always filter
}

ConvMotionFilter::~ConvMotionFilter(void)
{
}

void ConvMotionFilter::setConvVector(const PointVec& inRotationVec, 
									const PointVec& inOffsetVec,
									double inMaskFactor,
									double inIntensity )
{
	mRotationVec.clear();
	mOffsetVec.clear();
	for( unsigned int i=0; i<inRotationVec.size(); i++)
	{
		mRotationVec.push_back(inRotationVec[i]);
	}

	for( unsigned int i=0; i<inOffsetVec.size(); i++)
	{
		mOffsetVec.push_back(inOffsetVec[i]);
	}

	mMaskFactor = inMaskFactor;
	mIntensity  = inIntensity;
}

bool ConvMotionFilter::applyConv( const PointVec& inVector, 
								  const PointVec& inVectorDiff, 
								  bool inIsRotation, 
								  PointVec& inConvVector,
								  PointVec& outVector)
{
	outVector.clear();

	int nFrames = (int)inVector.size();

	int nRadius = (int)inConvVector.size()/2;

	Point3d Val, ValX, ValY, Sum, SumX, SumY;
	int nAnglesInSum = 0;
	
	for (int nFrame=0; nFrame<nFrames; nFrame++)
	{
		if (inIsRotation)
		{
			SumX = Point3d(0.0,0.0,0.0);
			SumY = Point3d(0.0,0.0,0.0);
		}
		else
			Sum = Point3d(0.0,0.0,0.0);

		Point3d Factor = Point3d(0.0,0.0,0.0);
		int nConvVecPos = 0;

		int w = nFrame+((int)inConvVector.size()-nRadius);
		for (int i=nFrame-nRadius; i<w; i++)
		{
			assert(nConvVecPos <(int)inConvVector.size());
			
			if (i < 0)
			{	//duplicate left edge
				Val = inVector[0];
			}
			else if (i >= nFrames)
			{	//duplicate right edge
				Val = inVector[nFrames-1];
			}
			else
			{
				Val = inVector[i];
			}

			Factor += inConvVector[nConvVecPos];

			for (int n=0; n<3; n++)
			{
				if (inIsRotation)
				{
					ValX[n] = cos(RAD(Val[n]));
					ValY[n] = sin(RAD(Val[n]));
					SumX[n] = SumX[n]+(inConvVector[nConvVecPos][n]*ValX[n]);
					SumY[n] = SumY[n]+(inConvVector[nConvVecPos][n]*ValY[n]);
				}
				else
				{
					Sum[n] = Sum[n]+(inConvVector[nConvVecPos][n]*Val[n]);
				}
			}

			nConvVecPos++;
		}
		for (int n=0; n<3; n++)
		{
			if (Factor[n] != 0)
				Factor[n] = 1.0/Factor[n];

			if (inIsRotation)
			{
				SumX[n] = mIntensity*Factor[n]*SumX[n];
				SumY[n] = mIntensity*Factor[n]*SumY[n];


				Sum[n] = DEG(atan2(SumY[n],SumX[n]));

				if (nFrame > 0)
				{
					double diff = subRotation(Sum[n], outVector[outVector.size()-1][n]);
					
					if (diff > 180)
						diff = 360 - diff;
					if (diff < -180)
						diff = 360 + diff;

					if( abs(diff) > mRotationUTH[n] )
					{
						Sum[n] = fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*mRotationUTH[n];
					}
					if (abs(diff) <= mRotationLTH[n])
					{
						Sum[n] = inVector[nFrame][n];//fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*mRotationUTH[n];
					}
				}
			}
			else
			{
				Sum[n] = mIntensity*Factor[n]*Sum[n];
				if (nFrame > 0)
				{
					double diff = Sum[n] - outVector[outVector.size()-1][n];
					if (abs(diff) > mOffsetUTH[n])
					{
						Sum[n] = outVector[outVector.size()-1][n] + (diff/abs(diff))*mOffsetUTH[n];
					}
					if (abs(diff) <= mOffsetLTH[n])
					{
						Sum[n] = inVector[nFrame][n];//fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*mRotationUTH[n];
					}
				}
			}
		}

		Point3d result = Sum;

		//perform unsharp mask - A-(A-B)*factor
		//B being the blurred vector
		if( mMaskFactor != 0 )
		{
			result = Val + (Val-Sum)*mMaskFactor;
		}

		outVector.push_back(result);
	}
	return true;
}

double 
ConvMotionFilter::fixRotation(const double inVal)
{
	double ret = inVal;
	while (ret < 0)
		ret = 360+ret;
	while (ret >= 360)
		ret = ret-360;

	return ret;
}

Point3d 
ConvMotionFilter::fixRotation(Point3d inPoint)
{
	Point3d ret = inPoint;
	for (int i=0; i<3; i++)
	{
		ret[i] = fixRotation(ret[i]);
	}
	return ret;
}

double 
ConvMotionFilter::subRotation(const double inPointA, const double inPointB)
{
	double ret = fixRotation(inPointA) - fixRotation(inPointB);
	return fixRotation(ret);
}

bool 
ConvMotionFilter::applyFilter(PointVec& inRotations, PointVec& inOffsets,
									 PointVec& inRotationDiff, PointVec& inOffsetDiff,
		PointVec& outRotations, PointVec& outOffsets)
{
	bool ret = true;

	if( inRotations.size() > 0 )
		ret = applyConv (inRotations, inRotationDiff, true, mRotationVec, outRotations);

	bool noErrInRotationConvolution = ret;

	if( noErrInRotationConvolution && inOffsets.size() > 0 )
		ret = applyConv (inOffsets, inOffsetDiff, false, mOffsetVec, outOffsets);

	return ret;
}

#include "StdAfx.h"
#include "ConvMotionFilter.h"

#define pi 3.14159265358978
#define DEG(r) ((r)*180/pi)
#define RAD(d) ((d)*pi/180)
ConvMotionFilter::ConvMotionFilter(void)
{
	_OffsetVec.push_back(Point3d(1,1,1));
/*
	_OffsetVec.push_back(Point3d(0.01,0.01,0.01));
	_OffsetVec.push_back(Point3d(0.1,0.1,0.1));
	_OffsetVec.push_back(Point3d(0.2,0.2,0.2));
	_OffsetVec.push_back(Point3d(1,1,1));
	_OffsetVec.push_back(Point3d(2,2,2));
	_OffsetVec.push_back(Point3d(4,4,4));
	_OffsetVec.push_back(Point3d(6,6,6));
	_OffsetVec.push_back(Point3d(8,8,8));
	_OffsetVec.push_back(Point3d(9,9,9));
	_OffsetVec.push_back(Point3d(0,0,0));	//The rest are zero so it is only done of previous frames
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
	_OffsetVec.push_back(Point3d(0,0,0));
*/
	_OffsetUTH = Point3d(10001000,10001000,10001000);	//big - allow free movement
	_OffsetLTH = Point3d(0,0,0); //zero threshold - always filter

	_RotationVec.push_back(Point3d(1,1,1));
	_RotationVec.push_back(Point3d(3,3,3));
	_RotationVec.push_back(Point3d(4,4,4));
	_RotationVec.push_back(Point3d(3,3,3));
	_RotationVec.push_back(Point3d(1,1,1));

	_RotationUTH = Point3d(12,12,12); //Do not allow more then 12 degrees rotation per DOF per joint pre frame
	_RotationLTH = Point3d(0,0,0); //zero threshold - always filter

	/*
	_RotationVec.push_back(Point3d(0.01,0.01,0.01));
	_RotationVec.push_back(Point3d(0.1,0.1,0.1));
	_RotationVec.push_back(Point3d(0.2,0.2,0.2));
	_RotationVec.push_back(Point3d(1,1,1));
	_RotationVec.push_back(Point3d(2,2,2));
	_RotationVec.push_back(Point3d(4,4,4));
	_RotationVec.push_back(Point3d(6,6,6));
	_RotationVec.push_back(Point3d(8,8,8));
	_RotationVec.push_back(Point3d(9,9,9));
	_RotationVec.push_back(Point3d(0,0,0));	//The rest are zero so it is only done of previous frames
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	_RotationVec.push_back(Point3d(0,0,0));
	*/
	_ConvolveMask = true;
	mIntensity = 1.0;
}

ConvMotionFilter::~ConvMotionFilter(void)
{
}

int ConvMotionFilter::setConvVector(const PointVec& inRotationVec, const PointVec& inOffsetVec)
{
	_RotationVec.clear();
	_OffsetVec.clear();
	for (unsigned int i=0; i<inRotationVec.size(); i++)
	{
		_RotationVec.push_back(inRotationVec[i]);
	}
	for (unsigned int i=0; i<inOffsetVec.size(); i++)
	{
		_OffsetVec.push_back(inOffsetVec[i]);
	}
	return 0;
}
bool ConvMotionFilter::loadFilter(double inIntensity)
{
//	if (inIntensity < 0){
//		cerr << "Convolution filter cannot handle non-positive values\n";
//		return false;
//	}
	mIntensity = (int) inIntensity;

	//convolution stuff
	int filtSize = 2*mIntensity + 1;
	double filtVal = 1.0/filtSize;

	mFilter.clear();
	for (int i=0; i<filtSize; i++){
		mFilter.push_back(filtVal);
	}
	return true;
}

bool ConvMotionFilter::applyConv(const PointVec& inVector, const PointVec& inVectorDiff, bool inIsRotation, PointVec& inConvVector,
								 PointVec& outVector)
{
	outVector.clear();
	int nFrames = (int)inVector.size();
	if (nFrames == 0)
		return true;
	int nRadius = (int)inConvVector.size()/2;
	Point3d Val, ValX, ValY, Sum, SumX, SumY, Factor;
	int nConvVecPos;
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
		Factor = Point3d(0.0,0.0,0.0);
		nConvVecPos = 0;
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
					if (abs(diff) > _RotationUTH[n])
					{
						Sum[n] = fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*_RotationUTH[n];
					}
					if (abs(diff) <= _RotationLTH[n])
					{
						Sum[n] = inVector[nFrame][n];//fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*_RotationUTH[n];
					}
				}
			}
			else
			{
				Sum[n] = mIntensity*Factor[n]*Sum[n];
				if (nFrame > 0)
				{
					double diff = Sum[n] - outVector[outVector.size()-1][n];
					if (abs(diff) > _OffsetUTH[n])
					{
						Sum[n] = outVector[outVector.size()-1][n] + (diff/abs(diff))*_OffsetUTH[n];
					}
					if (abs(diff) <= _OffsetLTH[n])
					{
						Sum[n] = inVector[nFrame][n];//fixRotation(outVector[outVector.size()-1][n]) + (diff/abs(diff))*_RotationUTH[n];
					}
				}
			}
		}
		outVector.push_back(Sum);
	}
	return true;
}
/*
bool ConvMotionFilter::applyConv(const PointVec& inVector, const PointVec& inVectorDiff, bool inIsRotation, PointVec& inConvVector,
								 PointVec& outVector)
{
	outVector.clear();
	int nFrames = (int)inVector.size();
//	if (_ConvolveDiff)
//		nFrames = (int)inVectorDiff.size();
	if (nFrames == 0)
		return true;
	int nRadius = (int)inConvVector.size()/2;
	Point3d Val, Sum, Factor;
	int nConvVecPos;
	for (int nFrame=0; nFrame<nFrames; nFrame++)
	{
		Sum = Point3d(0.0,0.0,0.0);
		Factor = Point3d(0.0,0.0,0.0);
		nConvVecPos = 0;
		for (int i=nFrame-nRadius; i<nFrame+((int)inConvVector.size()-nRadius); i++)
		{
			assert(nConvVecPos <(int)inConvVector.size());
			if (i < 0)
			{
//				if (_ConvolveDiff)
//					Val = inVectorDiff[i];
//				else
					Val = inVector[i];
			}
			else if (i >= nFrames)
			{
//				if (_ConvolveDiff)
//					Val = inVectorDiff[nFrames-1];
//				else
					Val = inVector[nFrames-1];
			}
			else
			{
//				if (_ConvolveDiff)
//					Val = inVectorDiff[i];
//				else
					Val = inVector[i];
			}
			Factor += inConvVector[nConvVecPos];
			for (int n=0; n<3; n++)
			{
				if (inIsRotation)
					Sum[n] = sumRotation(Sum[n],(inConvVector[nConvVecPos][n]*Val[n]));
				else
					Sum[n] = Sum[n]+(inConvVector[nConvVecPos][n]*Val[n]);
			}
			nConvVecPos++;
		}
		for (int n=0; n<3; n++)
		{
			Factor[n] = Factor[n]*mIntensity;
			if (Factor[n] != 0)
				Factor[n] = 1.0/Factor[n];
			Sum[n] = Sum[n]*Factor[n];
			if (inIsRotation)
				Sum[n] = fixRotation(Sum[n]);
		}
		if (_ConvolveDiff)
		{
				if (inIsRotation)
				{
					if (nFrame != 0)
					{
						Sum = sumRotation(outVector[outVector.size()-1], subRotation(inVector[nFrame], Sum));
					}
					outVector.push_back(Sum);
				}
				else
				{
					if (nFrame != 0)
					{
						Sum = outVector[outVector.size()-1] + (inVector[nFrame] - Sum);
					}
					outVector.push_back(Sum);
				}
		}
		else
		{
			for (int n=0; n<3; n++)
			{
				if (Sum[n] > 180)
					Sum[n] = Sum[n] - 360;
			}
			outVector.push_back(Sum);
		}
	}
	return true;
}
*/
bool ConvMotionFilter::applyFilter(PointVec& inRotations, PointVec& inOffsets,
									 PointVec& inRotationDiff, PointVec& inOffsetDiff,
		PointVec& outRotations, PointVec& outOffsets)
{
	if (!applyConv (inRotations, inRotationDiff, true, _RotationVec, outRotations))
		return false;
	return applyConv (inOffsets, inOffsetDiff, false, _OffsetVec, outOffsets);
}
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
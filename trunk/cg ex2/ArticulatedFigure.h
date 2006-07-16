#ifndef __ARTICULATED_FIGURE_H__
#define __ARTICULATED_FIGURE_H__

#include "MyTypes.h"

class ConvMotionFilter;

class ArticulatedFigure
{
//--------------- Internal Class Definitions -----------
public:
    class Node;
    typedef std::vector<Node>     NodeList;
    typedef std::vector<Node *>   NodePtrList;
    typedef NodePtrList::iterator NodePtrListIt;

    //------------- Keeping track of Channel positions -----------
    class ParamMappingArr
    {
    public:
        ParamMappingArr()
        {
            for( int i = 0; i < 6; i++ ) pArr[i] = INVALID_PLCMNT;
        }

        ParamPlacement pArr[6];
    };

    //------------- The Basic Node Structure -----------
    class Node {
    public:
        Node( std::string &inName, Point3d &inPos );
        ~Node();
        Node( const Node &inNode );

        void setNumChannels( int inNumChannels ){ pNumChannels = inNumChannels; }
		void draw(int frameNum, bool lineFigure, bool isFiltered, double jointRadius);
		void applyFilter(ConvMotionFilter* pFilter);

    public:
        std::string     pNodeName;
        int             pNumChannels;
        ParamMappingArr pChannelMapping;
        Point3d         pPosition;
        PointVec        pOffsets;

        //VERY IMPORTANT: order to perform these rotations is:
        // Xrotation Zrotation Yrotation - according to BVH file format
        PointVec        pRotations;

        NodePtrList     pChildren; //child nodes

		// filtered data
		PointVec		pFilteredOffsets;
        PointVec		pFilteredRotations;

		//first derivative of the motions - calculated when figure is loaded.
        PointVec        pOffsetDiff;
		PointVec        pRotationDiff;
    };

//--------------- Public Methods -----------
public:
    ArticulatedFigure();
    ~ArticulatedFigure();

	double getMinY();
    void setRuntimeParamters( int inNumFrames, double inFrameTime );
	double getMaxOffsetDistance(){ return mMaxOffsetDistance; }
	double getJointRadius(){ return getMaxOffsetDistance()/50.0; }
	Point3d getInitialOffset();
	Point3d getBodyCenter();
	void draw(int frameNum, bool lineFigure);
	void applyFilter(ConvMotionFilter* pFilter);
	int getNumOfFrames(){ return mNumFrames; }
	bool isFiltered(){ return mIsFiltered; }
	double getFrameTime(){ return mFrameTime; }

//--------------- Storage -----------
protected:
    NodePtrList mRootNodes; //can be more than one root node!
    int         mNumFrames;
    double      mFrameTime;
	bool		mIsFiltered;

	Point3d		mMaxOffset;
	Point3d		mMinOffset;
	double		mMaxOffsetDistance;
	Point3d		mTotalOffset;
	Point3d		mBodyCenter;
	int			mNodesCounter;

friend class BVHParser;
friend class BVHWriter;

};

#endif //__ARTICULATED_FIGURE_H__

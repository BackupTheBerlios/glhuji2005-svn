#ifndef __ARTICULATED_FIGURE_H__
#define __ARTICULATED_FIGURE_H__

#include "MyTypes.h"

class ArticulatedFigure
{
//--------------- Internal Class Definitions -----------
public:
    class Node;
    typedef std::vector<Node>     NodeList;
    typedef std::vector<Node *>   NodePtrList;
    typedef NodePtrList::iterator NodePtrListIt;

    //------------- The Basic Node Structure -----------
    class Node {
    public:
        Node( std::string &inName, Point3d &inPos );
        ~Node();
        Node( const Node &inNode );

        void setNumChannels( int inNumChannels ){ pNumChannels = inNumChannels; }
		void draw(int frameNum, bool lineFigure);

    public:
        std::string   pNodeName;
        int           pNumChannels;
        Point3d       pPosition;
        PointVec      pOffsets;

        //VERY IMPORTANT: order to perform these rotations is:
        // Xrotation Zrotation Yrotation - according to BVH file format
        PointVec      pRotations;

        NodePtrList pChildren; //child nodes

    };

//--------------- Public Methods -----------
public:
    ArticulatedFigure();
    ~ArticulatedFigure();

    void setRuntimeParamters( int inNumFrames, double inFPS );
	float getMaxOffset(){ return 100.0; }//TODO: should return the maxmal joint offset
	static float getJointRadius(){ return 0.5; }//TODO: let the user decide
	void draw(int frameNum, bool lineFigure);
	int getNumOfFrames(){ return mNumFrames; }
	double getFrameTime(){ return mFPS; }

//--------------- Storage -----------
protected:
    NodePtrList mRootNodes; //can be more than one root node!
    int         mNumFrames;
    double      mFPS;

friend class BVHParser;

};

#endif //__ARTICULATED_FIGURE_H__
#include "StdAfx.h"

#include "ArticulatedFigure.h"

#include <iostream>
#include <string>
using namespace std;

ArticulatedFigure::Node::Node( string &inName, Point3d &inPos ) : 
pNodeName(inName),
pNumChannels(0),
pPosition(inPos)
{ }

ArticulatedFigure::Node::~Node()
{
    NodePtrListIt it = pChildren.begin();
    for( ; it != pChildren.end(); it++ )
    {
        Node *ptr = *it;
        delete ptr;
    }
}

ArticulatedFigure::Node::Node( const Node &inRhs )
{
    pNodeName    = inRhs.pNodeName;
    pNumChannels = inRhs.pNumChannels;
    pPosition    = inRhs.pPosition;
    pOffsets     = inRhs.pOffsets;
    pRotations   = inRhs.pRotations;
    pChildren    = inRhs.pChildren;
}

ArticulatedFigure::ArticulatedFigure(void)
{
    mFPS       = -1;
    mNumFrames = -1;
}

ArticulatedFigure::~ArticulatedFigure(void)
{
    NodePtrListIt it = mRootNodes.begin();
    for( ; it != mRootNodes.end(); it++ )
    {
        Node *ptr = *it;
        delete ptr;
    }
}

void 
ArticulatedFigure::setRuntimeParamters( int inNumFrames, double inFPS )
{
    mFPS       = inFPS;
    mNumFrames = inNumFrames;
}
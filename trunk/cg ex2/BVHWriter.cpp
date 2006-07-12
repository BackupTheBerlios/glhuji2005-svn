#include "StdAfx.h"
#include "BVHWriter.h"

using namespace std;

//Make the code a little more readable
typedef ArticulatedFigure::Node            Node;
typedef ArticulatedFigure::ParamMappingArr ParamMappingArr;



BVHWriter::BVHWriter(void)
{
}

BVHWriter::~BVHWriter(void)
{
}

void
BVHWriter::write( ArticulatedFigure &inFigure, string &inOutputFilename )
{
    mOutputStream.open( inOutputFilename.c_str(), ios_base::out );

    //----------------- Write Out Start Tag ----------------
    mOutputStream << "HIERARCHY" << endl << endl;

    //-------------- Write Out Nodes -------------
    ArticulatedFigure::NodePtrListIt it = inFigure.mRootNodes.begin();
    for( ; it != inFigure.mRootNodes.end(); it++ )
    {
        Node *n = *it;
        mLinearNodeList.push_back(n);
        mOutputStream << "ROOT " << n->pNodeName.c_str() << endl;
        writeNode(*n,  0);
    }

    //-------------- Write Out Movements -------------
    mOutputStream << "MOTION" << endl;
    mOutputStream << "Frames: " << inFigure.getNumOfFrames() << endl;
    mOutputStream << "Frame Time: " << inFigure.getFrameTime() << endl;
    mOutputStream << endl << endl;

    //-------------- Write Out Actual Motion Vectors! -------------
    for( int i = 0; i < inFigure.getNumOfFrames(); i++ )
    {
        ArticulatedFigure::NodePtrListIt it = mLinearNodeList.begin();
        for( ; it != mLinearNodeList.end(); it++ )
        {
            if( it != mLinearNodeList.begin() )
                mOutputStream << " ";

            Node *n = *it;
            ParamMappingArr &mapArr = n->pChannelMapping;
            int numChannels = n->pNumChannels;

            Vector3d &pos = n->pOffsets[i];
            Vector3d &rot = n->pRotations[i];

			if( !n->pFilteredOffsets.empty() )
				pos = n->pFilteredOffsets[i];

			if( !n->pFilteredRotations.empty() )
				rot = n->pFilteredRotations[i];


            for( int j = 0; j < numChannels; j++ )
            {
                switch( mapArr.pArr[j] )
                {
                case XROT:
                    mOutputStream << rot[0];
                    break;
                case YROT:
                    mOutputStream << rot[1];
                    break;
                case ZROT:
                    mOutputStream << rot[2];
                    break;
                case XPOS:
                    mOutputStream << pos[0];
                    break;
                case YPOS:
                    mOutputStream << pos[1];
                    break;
                case ZPOS:
                    mOutputStream << pos[2];
                    break;
                }
                if( j != (numChannels-1))
                    mOutputStream << " ";
            }
        }

         mOutputStream << endl;
    }

    mOutputStream.close();
}

void 
BVHWriter::writeNode( ArticulatedFigure::Node &inNode, int inLevel )
{
    string indent = "";
    for( int i = 0; i < inLevel; i++ ) indent += "\t";

    //----------- Opening "{" ----------------
    mOutputStream << indent.c_str() << "{" << endl;

    //----------- OFFSET ----------------
    mOutputStream << indent.c_str() << "\t" << "OFFSET ";
    mOutputStream << inNode.pPosition[0] << " "  << inNode.pPosition[1] 
    << " " << inNode.pPosition[2] << endl;

    if( inNode.pChildren.size() > 0 ) //not an end node
    {
        //-------------------------- Channels -------------------------------
        int numChannels = inNode.pNumChannels;
        mOutputStream << indent.c_str() << "\t" << "CHANNELS " << numChannels << " ";
        
        ParamMappingArr &mapArr = inNode.pChannelMapping;
        for( int i = 0; i < numChannels; i++ )
        {
            switch( mapArr.pArr[i] )
            {
            case XROT:
                mOutputStream << "Xrotation";
                break;
            case YROT:
                mOutputStream << "Yrotation";
                break;
            case ZROT:
                mOutputStream << "Zrotation";
                break;
            case XPOS:
                mOutputStream << "Xposition";
                break;
            case YPOS:
                mOutputStream << "Yposition";
                break;
            case ZPOS:
                mOutputStream << "Zposition";
                break;

            default:
            case INVALID_PLCMNT:
                cout << "ERROR: invalid channel mapping in node " << inNode.pNodeName.c_str() << endl;
                    exit(0);
                break;
            }

            if( i != (numChannels-1) )
                mOutputStream << " ";
        }

        mOutputStream << endl;

        //-------------------------- Print Children -------------------------------
        for( ArticulatedFigure::NodePtrListIt it = inNode.pChildren.begin();
                it != inNode.pChildren.end(); it++ )
        {
            Node *n = *it;

            mLinearNodeList.push_back(n);

            mOutputStream << indent.c_str() << "\t" ;
            if( n->pChildren.size() == 0 )
                mOutputStream << "End Site" << endl;
            else
                mOutputStream << "JOINT " << n->pNodeName.c_str() << endl;

            writeNode( *n, inLevel+1 );

        }

    }
    
    //----------- Closing "}" ----------------
    mOutputStream << indent.c_str() << "}" << endl;

}
#include "StdAfx.h"
#include "BVHParser.h"
#include <cctype> //tolower
#include <algorithm> //transform

//Make the code a little more readable
typedef ArticulatedFigure::Node            Node;
typedef ArticulatedFigure::ParamMappingArr ParamMappingArr;

BVHParser::BVHParser( string &inFilename, ArticulatedFigure &inFigure, bool &outOK ) :
                mInputStream(inFilename.c_str(), ios_base::in ),
                mArticulatedFigure( inFigure )
{
    outOK = mInputStream.good();
}
BVHParser::~BVHParser()
{
    if( mInputStream.is_open() )
        mInputStream.close();
}

#define CHECK_STR(str_,cmpStr_,errStr_)    { \
                                                string bla_ = (str_); \
                                                if( bla_ != string(cmpStr_) )\
                                                { cout << "ERROR:" << (errStr_) << " (" << bla_ << ")" <<endl; break;}\
                                             }

#define CHECK_TOKEN(cmpStr_, errStr_ ) CHECK_STR( getNextToken(), cmpStr_, errStr_ )

void
BVHParser::parse(bool &outParseSuccesfull )
{
    string token;
    bool   readOK = false;

    outParseSuccesfull = false;
    
    do {
        //------- Read Start Token ----------
        CHECK_TOKEN( "hierarchy", "invalid BVH start token" );

        //------ Read Hierarchy -----------
        CHECK_TOKEN( "root", "expected 'ROOT' token" );

        readHierarchy( NULL, false, readOK ); if( !readOK ) break;

        //------ Read Frames ---------
        CHECK_TOKEN( "motion", "expected 'MOTION' token" );
        readFrames( readOK ); if( !readOK ) break;

        outParseSuccesfull = true;
    } while(0);
    
}

void 
BVHParser::readFrames( bool &outParseSuccesfull )
{
    outParseSuccesfull = false;

    do {
        //------------ Read # of frames ---------
        CHECK_TOKEN( "frames:", "expected 'Frames:' token" );

        int numFrames = 0;
        numFrames = atoi( getNextToken().c_str() );

        if( !(numFrames > 0) )
        {
            cout << "ERROR: invalid number of frames: (" << numFrames << ")" << endl;
            break;
        }

        //------------ Read FPS ---------
        CHECK_TOKEN( "frame", "expected 'frame' token" );
        CHECK_TOKEN( "time:", "expected 'time:' token" );
        double fps = 0;

        fps = atof( getNextToken().c_str() );
        if( !(fps > 0) )
        {
            cout << "ERROR: invalid fps value: (" << fps << ")" << endl;
            break;
        }

        //------------ Optimization - reserve space in vectors ---------------
        for( ArticulatedFigure::NodePtrListIt it = mLinearNodeList.begin(); 
                                             it != mLinearNodeList.end(); it++ )
        {
            Node *n = *it;
            
            n->pOffsets.reserve( numFrames );
            n->pRotations.reserve( numFrames );
        }

        //------------ Read movement vectors --------
        mArticulatedFigure.setRuntimeParamters( numFrames, fps );

        bool readMotionsOK = true;
        for( int i = 0; i < numFrames && readMotionsOK; i++ )
        {
            ArticulatedFigure::NodePtrListIt it = mLinearNodeList.begin();
            for( ; it != mLinearNodeList.end() && readMotionsOK; it++ )
            {
                Node *n = *it;
                ParamMappingArr &pMap = n->pChannelMapping;

                readNodeMotions( n, pMap, readMotionsOK );
            }
        }

        //if there was a problem reading the motion parameters for a single frame
        if( !readMotionsOK ) break;

        outParseSuccesfull = true;
    } while(0);
}

void
BVHParser::readNodeMotions( ArticulatedFigure::Node *inNodePtr, 
                    ArticulatedFigure::ParamMappingArr &inArr, bool &outParseOK )
{
    double xRot = 0, yRot = 0, zRot = 0;
    double xPos = 0, yPos = 0, zPos = 0;

    outParseOK = false;

    int numChannels = inNodePtr->pNumChannels;

    if( numChannels == 0 )
    {
        outParseOK = true; //nothing to read
    }
    else
    {
        do {
            
            if( !mInputStream.good() ) break;

            //MAP channels to parameters
            bool err = false;
            for( int i = 0; i < numChannels && !err; i++ )
            {
                string token = getNextToken();
                double num = atof( token.c_str() );

                switch( inArr.pArr[i] )
                {
                case XPOS:
                    xPos = num;
                    break;
                case YPOS:
                    yPos = num;
                    break;
                case ZPOS:
                    zPos = num;
                    break;
                case XROT:
                    xRot = num;
                    break;
                case YROT:
                    yRot = num;
                    break;
                case ZROT:
                    zRot = num;
                    break;
                default:
                    cout << "ERROR: invalid value @ position " << i << endl;
                    err = true;
                    break;
                }

            }

            if( err ) break;

            //Finally, set parameters...
            inNodePtr->pOffsets.push_back( Point3d(xPos, yPos, zPos) );
            inNodePtr->pRotations.push_back( Point3d(xRot, yRot, zRot) );
            
            outParseOK = true;
        } while(0);
    }

}

void 
BVHParser::readHierarchy( ArticulatedFigure::Node *inParent, bool isEnd, bool &outParseSuccesfull )
{
    outParseSuccesfull = false;
    string hierarchyName(""), token;
    ArticulatedFigure::Node *curNode = NULL;

    do {

        //----------- Read Hierarchy Name and '{' --------------
        token = getNextTokenPreserveCase();

        if( token !=  string("{") )
        {
            hierarchyName = token;
            token = getNextToken();
        }

        CHECK_STR( token, "{", "expected '{' at start of hierarchy" );

        //----------- Read Offset --------------
        CHECK_TOKEN( "offset", "expected 'OFFSET'" );

        double x, y, z;
        x = atof( getNextToken().c_str() );
        y = atof( getNextToken().c_str() );
        z = atof( getNextToken().c_str() );

        if( inParent == NULL )
        {
            Node *theNode = new Node( hierarchyName, Point3d(x,y,z) );
            mArticulatedFigure.mRootNodes.push_back( theNode );

            curNode = theNode;
            mLinearNodeList.push_back( theNode );
        }
        else
        {
            Node *theNode = new Node( hierarchyName, Point3d(x,y,z) );
            inParent->pChildren.push_back( theNode );

            curNode = theNode;
            mLinearNodeList.push_back( theNode );
        }

        //----------- Read channels --------------
        if( !isEnd )
        {
            CHECK_TOKEN( "channels", "expected 'CHANNELS'" );
            int numChannels;
            numChannels = atoi( getNextToken().c_str() );

            if( numChannels != 6 && numChannels != 3)
            {
                cout << "ERROR: can only accept 3 or 6 channel BVH file got " << numChannels
                    << " channels instead" << endl;
                break;
            }

            curNode->setNumChannels( numChannels );

            //-------- Read off channel name tokens ------
            bool plcmntError = false;
            ParamMappingArr &plcmnt = curNode->pChannelMapping;
            for( int i = 0; i < numChannels; i++ )
            {
                token = getNextToken();

                if( token == string("xposition"))
                    plcmnt.pArr[i] = XPOS;
                else if( token == string("yposition"))
                    plcmnt.pArr[i] = YPOS;
                else if( token == string("zposition"))
                    plcmnt.pArr[i] = ZPOS;
                else if( token == string("xrotation"))
                    plcmnt.pArr[i] = XROT;
                else if( token == string("yrotation"))
                    plcmnt.pArr[i] = YROT;
                else if( token == string("zrotation"))
                    plcmnt.pArr[i] = ZROT;
                else
                {
                    cout  << "ERROR: invalid channel name (" << token << ")" << endl;
                    plcmntError = true;
                    break;
                }
            }

            if( plcmntError ) break;

            //-------- End site or joint? ------
       
            token = getNextToken();

            if( token != string("end") && token != string("joint"))
            {
                cout << "ERROR: was expecting End or JOINT, got \"" << token
                    << "\" instead" << endl;
                break;
            }

            bool readOK = false;

            readHierarchy( curNode, token == string("end"), readOK ); if( !readOK ) break;
        }

        bool nextJointReadOK = false;
        while(1)
        {
            token = getNextToken();
            
            bool isJoint = (token == string("joint"));

            if( !isJoint && token != string("}") )
            {
                cout << "ERROR: was expecting JOINT or } , got \"" << token
                    << "\" instead" << endl;
                break;
            }

            if( isJoint )
            {
                bool readOK = false;

                readHierarchy( curNode, false, readOK ); if(!readOK ) break;
            }
            else // read "}"
            {
                nextJointReadOK = true;
                break;
            }
        }

        if( !nextJointReadOK ) break;

        outParseSuccesfull = true;
    } while(0);
}

#undef CHECK_STR
#undef CHECK_TOKEN

bool
BVHParser::hasMoreTokens()
{
    return mInputStream.good();
}

string
BVHParser::getNextToken()
{
    string ret;
    mInputStream >> ret;

    //convert to lowercase
    transform( ret.begin(),ret.end(), ret.begin(), tolower);
    return ret;
}

string 
BVHParser::getNextTokenPreserveCase()
{
    string ret;
    mInputStream >> ret;
    return ret;
}
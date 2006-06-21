#ifndef __BVH_PARSER_H__
#define __BVH_PARSER_H__

#include <string>
#include <fstream>
#include <vector>
#include "ArticulatedFigure.h"

using namespace std;

class BVHParser
{

//-------------- Public Methods ---------------
public:
    BVHParser( string &inFilename, ArticulatedFigure &inFigure, bool &outOK );
    ~BVHParser();

    //initiates parsing - 2nd parameter is true if no errors during parse.
    void parse( bool &outParseSuccesfull );

//-------------- Protected Methods ---------------
protected:

    //returns next token in lowercase
    string getNextToken();

    //retrieve next token, without converting it to lowercase.
    string getNextTokenPreserveCase();
    
    //returns true if there are more tokens in the stream.
    bool   hasMoreTokens();

    //this is the recursive descent part...
    void readHierarchy( ArticulatedFigure::Node *inParent, bool isEnd, bool &outParseSuccesfull );

    //Read actual motion vectors (the MOTIONS section)
    void readFrames( bool &outParseSuccesfull );

    //read a single motion for a node
    void readNodeMotions( ArticulatedFigure::Node *inNodePtr, 
                ArticulatedFigure::ParamMappingArr &inArr, bool &outParseOK );


//-------------- Storage ---------------
protected:
    ifstream                          mInputStream;
    ArticulatedFigure                 &mArticulatedFigure;
    ArticulatedFigure::NodePtrList    mLinearNodeList;
};

#endif __BVH_PARSER_H__
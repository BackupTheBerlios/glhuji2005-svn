#ifndef __BVH_WRITER_H__
#define __BVH_WRITER_H__

#include "ArticulatedFigure.h"
#include <fstream>


class BVHWriter
{
public:
    BVHWriter();

    void write( ArticulatedFigure &inFigure, string &inOutputFilename );

    ~BVHWriter();

protected:
    void writeNode( ArticulatedFigure::Node &inNode, int inLevel );

protected:
    std::ofstream                  mOutputStream;
    ArticulatedFigure::NodePtrList mLinearNodeList;
};

#endif //__BVH_WRITER_H__
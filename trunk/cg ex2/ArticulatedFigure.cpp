#include "StdAfx.h"

#include "ArticulatedFigure.h"
#include "ConvMotionFilter.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

extern GLfloat mat_diffuse[];
extern GLfloat mat_diffuse_tr[];

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
    mFrameTime  = -1;
    mNumFrames	= -1;
	mIsFiltered	= false;
	mIsCoupled = true;

	mMaxOffset[0] = mMaxOffset[1] = mMaxOffset[2] = 0;
	mMinOffset[0] = mMinOffset[1] = mMinOffset[2] = 100;
	mTotalOffset[0] = mTotalOffset[1] = mTotalOffset[2] = 0;
	mBodyCenter[0] = mBodyCenter[1] = mBodyCenter[2] = 0;
	mNodesCounter = 0;
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
ArticulatedFigure::setRuntimeParamters( int inNumFrames, double inFrameTime )
{
    mFrameTime = inFrameTime;
	if (mNumFrames > 0)
		//mNumFrames = min(mNumFrames,inNumFrames);
		mNumFrames = max(mNumFrames,inNumFrames);
	else
		mNumFrames = inNumFrames;
}

void 
ArticulatedFigure::draw(int frameNum, bool lineFigure)
{
	double jointRadius = getJointRadius();
	int modFrameNum = 0;
	for (unsigned int i=0; i<mRootNodes.size(); i++){
		modFrameNum = ((frameNum-1) % mRootNodes[i]->pRotations.size()) + 1;
		if (mIsFiltered){
			// the filtered motion is opaque
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
			mRootNodes[i]->draw(modFrameNum, lineFigure, true, false, jointRadius);
			if (mIsCoupled){
				// while the original is semi transparent
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_tr);
				mRootNodes[i]->draw(modFrameNum, lineFigure, false, true, jointRadius);
			}
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		}
		else{
			mRootNodes[i]->draw(modFrameNum, lineFigure, false, false, jointRadius);
		}
	}
}

void ArticulatedFigure::applyFilter(ConvMotionFilter* pFilter)
{
	if (pFilter == NULL)
	{
		// it means we are asked to remove the filter
		mIsFiltered = false;
		// note: nothing to do anymore, the filtered data will be deleted by the next filter
	}
	else
	{
		mIsFiltered = true;
		for (unsigned int i=0; i<mRootNodes.size(); i++){
			mRootNodes[i]->applyFilter(pFilter);
		}
	}
}

Point3d ArticulatedFigure::getInitialOffset()
{
	if (mRootNodes.size() == 0 || mRootNodes[0]->pOffsets.size() == 0){
		Point3d initialOffset(0,0,0);
		return initialOffset;
	}
	//NOTE: I assume everything is set according to the first body
	return mRootNodes[0]->pOffsets[0];
}

double ArticulatedFigure::getMinY()
{
	Point3d minPoint = mMinOffset + getInitialOffset();
	return minPoint[1];
}

Point3d ArticulatedFigure::getBodyCenter()
{ 
	Point3d midPoint = mBodyCenter + getInitialOffset();
	return midPoint; 
}
//////////////////////////////////////////////////
// PROC:   JOINT::draw()
// DOES:   recursively draws joints
//////////////////////////////////////////////////
void ArticulatedFigure::Node::draw(int frameNum, bool lineFigure, 
								   bool isFiltered, bool isCoupled, double jointRadius)
{
	glPushMatrix();
	int nc = 0;       // number of channels already processed

	
	// draw line from parent joint to current joint
	if (lineFigure)
	{
		glLineWidth(3);
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);		
		glVertex3f(pPosition[0],pPosition[1],pPosition[2]);
		glEnd();
	}
	else
	{
		// let's draw some bones:

		// calculate the rotation axis and the rotation angle
		// between the position vector and the (0,0,1) vector
		GLdouble boneLength = sqrt(pow(pPosition[0],2) + pow(pPosition[1],2) + pow(pPosition[2],2));	
		Vector3d v1(pPosition[0],pPosition[1],pPosition[2]);
		Vector3d v2(0,0,1);
		v1.normalize();
		Vector3d norm = v1%v2;
		double alfa1 = dot(v1,v2);
		double alfa = acos(alfa1)*RAD2DEG;
		glColor3f(0,0,1);	//TODO: let the user choose the colors

		//TODO: currently we can draw an ellipsoid or a cone
		// I think we should add a callback function to draw a bone
		// this method should get the pPosition vector and draw the bone form (0,0,0) to pPosition
		glPushMatrix();

		// Draw an ellipsoid
		glTranslated(pPosition[0]/2, pPosition[1]/2, pPosition[2]/2);
		glRotated(-alfa, norm[0], norm[1] ,norm[2]);
		glScaled(0.3, 0.3, 1.0);
		glutSolidSphere(boneLength/2, 10, 10);

		// Or maybe a cone
		/*
		glRotated(-alfa, norm[0], norm[1] ,norm[2]);
		GLdouble coneRadius = boneLength/5.0; 
		glutSolidCone(boneLength, coneHeight, 10, 10);
		*/

		glPopMatrix();
	}

     // apply joint offset
	glTranslatef(pPosition[0],pPosition[1],pPosition[2]);

	// if the data was filtered, get the new data
	PointVec& curOffsets = isFiltered? pFilteredOffsets : pOffsets;
	PointVec& curRotations = isFiltered? pFilteredRotations : pRotations;
	// apply translation channels (if any)
	if (curOffsets.size()>0) {
		float x = curOffsets[frameNum][0];
		float y = curOffsets[frameNum][1];
		float z = curOffsets[frameNum][2];
		glTranslatef(x,y,z);
	}
	
	// apply rotations (notice the order)
	if (curRotations.size() > 0)
	{
		// X
		if (curRotations[frameNum][0] > 0.0001 || curRotations[frameNum][0] < 0.0001)
			glRotatef(curRotations[frameNum][0],1,0,0);
		// Z
		if (curRotations[frameNum][2] > 0.0001 || curRotations[frameNum][2] < 0.0001)
			glRotatef(curRotations[frameNum][2],0,0,1);
		// Y
		if (curRotations[frameNum][1] > 0.0001 || curRotations[frameNum][1] < 0.0001)
			glRotatef(curRotations[frameNum][1],0,1,0);
	}

  // draw the joint
	glColor3f(0,1,0);
	glutSolidSphere(jointRadius,20,10);
	
	  // draw line from joint to end site (if necessary)
	if( pChildren.size() == 0 ) {
		glLineWidth(3);
		glColor3f(0.8,0.4,0);
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(pPosition[0],pPosition[1],pPosition[2]);
		glEnd();
	}

	  // recursively draw all child joints
	for (unsigned int n=0; n<pChildren.size(); n++)
		pChildren[n]->draw(frameNum, lineFigure, isFiltered, isCoupled, jointRadius);
	glPopMatrix();
}

//start debug
//static ofstream out( "c:\\out.txt", ios_base::trunc );
//end debug

//////////////////////////////////////////////////
// PROC:   JOINT::applyFilter(BaseMotionFilter* pFilter)
// DOES:   recursively filter the motion data (rotation and offsets)
//////////////////////////////////////////////////
void ArticulatedFigure::Node::applyFilter(ConvMotionFilter* pFilter)
{
	//start debug
	/*static bool f=true;


	if (pNodeName.compare("DNeck") == 0)
	{
		int a = 0;
	}
	if (pNodeName.compare("EHead") == 0)
	{
		int a = 0;
	} */
	//end debug

	pFilter->applyFilter( pRotations, pOffsets, 
		pRotationDiff, pOffsetDiff, 
		pFilteredRotations, pFilteredOffsets );

	//start debug
	if (pNodeName.compare("AHip") == 0)
	{
		#define PRN(g) do{ std::cout << "(" << g[0] << ", " << g[1] << ", " << g[2] << ")"; }while(0);
		for( int i = 0; i < (int)pRotations.size(); i++ )
		{
			Point3d a = pRotations[i];
			Point3d b = pFilteredRotations[i];

			PRN(a);
			std::cout << "->";
			PRN(b);
			std::cout << std::endl;
		}
	}
	//end debug


	//start debug
	/*out << pNodeName << std::endl;
	out << "before filter" << std::endl;

	out << "offsets:" << std::endl;
	for( PointVecIt it = pOffsets.begin();  it != pOffsets.end(); it++ )
	{
		Point3d &p = *it;
		out << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
	}

	out << "rotations:" << std::endl;
	for( PointVecIt it = pRotations.begin(); it != pRotations.end(); it++ )
	{
		Point3d &p = *it;
		out << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
	}

	out << "after filter" << std::endl;

	out << "offsets:" << std::endl;
	for( PointVecIt it = pFilteredOffsets.begin(); it != pFilteredOffsets.end(); it++ )
	{
		Point3d &p = *it;
		out << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
	}

	out << "rotations:" << std::endl;
	for( PointVecIt it = pFilteredRotations.begin(); it != pFilteredRotations.end(); it++ )
	{
		Point3d &p = *it;
		out << p[0] << ", " << p[1] << ", " << p[2] << std::endl;
	}*/

	
	//start debug
	/* if (f){
	for (unsigned int i=0; i<pRotations.size(); i++)
	{
		if (pRotations[i] != pFilteredRotations[i])
			cout << "Rot: " << pRotations[i] << " > " << pFilteredRotations[i] << endl;
		if (pOffsets[i] != pFilteredOffsets[i])
		{
			if (pRotations[i] != pFilteredRotations[i])
				cout << " | ";
			cout << "Off: " << pOffsets[i] << " > " << pFilteredOffsets[i] << endl;
		}
	}
	cout << endl;
	}
	f=false;*/
	//end debug

	// recursively draw all child joints
	for (unsigned int n=0; n<pChildren.size(); n++)
		pChildren[n]->applyFilter(pFilter);

}

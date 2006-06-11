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

void 
ArticulatedFigure::draw(int frameNum, bool lineFigure)
{
	for (unsigned int i=0; i<mRootNodes.size(); i++){
		mRootNodes[i]->draw(frameNum, lineFigure);
	}
}

//////////////////////////////////////////////////
// PROC:   JOINT::draw()
// DOES:   recursively draws joints
//////////////////////////////////////////////////
#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

void ArticulatedFigure::Node::draw(int frameNum, bool lineFigure)
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
	// apply translation channels (if any)
	// TODO: apply animation translate/rotate
	if (pOffsets.size()>0) {
		float x = pOffsets[frameNum][0];
		float y = pOffsets[frameNum][0];
		float z = pOffsets[frameNum][0];
		glTranslatef(x,y,z);
	}
	
	// apply rotations:
	// X
	if (pRotations.size() > 0)
	{
		if (pRotations[frameNum][0] != 0)
			glRotatef(pRotations[frameNum][0],1,0,0);
		// Z
		if (pRotations[frameNum][2] != 0)
			glRotatef(pRotations[frameNum][2],0,0,1);
		// Y
		if (pRotations[frameNum][1] != 0)
			glRotatef(pRotations[frameNum][1],0,1,0);
	}

  // draw the joint
	glColor3f(0,1,0);
	glutSolidSphere(getJointRadius(),20,10);
	
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
		pChildren[n]->draw(frameNum, lineFigure);
	glPopMatrix();
}
/*	FileName: OpenGLUtilities.h

    Utility class to help us perform certain operations with
    OpenGL which should be built in...
*/

#ifndef __OPENGL_UTILITIES_H__
#define __OPENGL_UTILITIES_H__

//------------------------
// OpenGLUtilities
// 
// Class containing static members to perform OpenGL related operations
// esp. drawing and matrix manipulation.
//------------------------
class OpenGLUtilities
{

//--------------- Public Function Prototypes ---------
public:

   // Draws a circle of a specified radius around a specified point, with
   //varying line width
   static void drawCircle( double inRadius, double inCenterX, 
                                double inCenterY, double inCenterZ, 
                                double inLineWidth = 1.0 );

    //Methods to perform operations on OpenGL Matrix structures:

   //Init the specified matrix to the identity matrix.
   static void initIdentityMatrix( GLdouble *outMatrix );

   //create a translation matrix using the specified parameters.
   static void initTranslationMatrix( double inX, double inY, double inZ, GLdouble *outMatrix );

   //Print the specified matrix to standard output - this is commented out on purpose!
   //static void printMatrix( GLdouble *inMatrix );
};

#endif //__OPENGL_UTILITIES_H__

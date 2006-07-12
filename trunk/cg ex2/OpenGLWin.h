#pragma once

#include "gl\glut.h"

class COpenGLWin
{
public:
	COpenGLWin(void);
	~COpenGLWin(void);
	void Run(CString filename);
	void Resize(int width, int height);
	void DisplayCallback();
	void mousePressedCallback( int inButton, int inState, int inX, int inY );
	void mouseMovedCallback( int inX, int inY );
	void keypress(unsigned char key, int x, int y);
	void idleFunc(void);
	void Initialize();
	void drawGround();
};

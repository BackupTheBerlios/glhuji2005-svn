#pragma once

#include "gl\glut.h"

class COpenGLWin
{
	CString m_Filename;
public:
	CString getFileName(){return m_Filename;}
	COpenGLWin(void);
	~COpenGLWin(void);
	void Run(CString filename);
	void Save(CString filename);
	void Resize(int width, int height);
	void DisplayCallback();
	void mousePressedCallback( int inButton, int inState, int inX, int inY );
	void mouseMovedCallback( int inX, int inY );
	void keypress(unsigned char key, int x, int y);
	void idleFunc(void);
	bool playPause();
	void gotoFrame(int nFrame);
	void gotoNextFrame();
	void gotoPrevFrame();
	void Initialize();
	int getFrameCount();
	void drawGround();
};

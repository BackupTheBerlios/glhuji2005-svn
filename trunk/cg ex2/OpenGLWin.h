#pragma once

#include "gl\glut.h"

class CEx2MFCDlg;
class COpenGLWin
{
	bool m_bLoaded;
	bool m_bCreated;
public:
	bool m_bFiltered;
	CString m_Filename;
	CEx2MFCDlg* m_pParent;
	CString getFileName(){return m_Filename;}
	COpenGLWin(void);
	~COpenGLWin(void);
	void Run(CString filename);
	bool FilterToggle(int nCode = 0);
	void Close();
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
	int getCurFrame();
	void drawGround();
	bool isPaused();
	bool toggleCoupled(int nSet = 0);
};

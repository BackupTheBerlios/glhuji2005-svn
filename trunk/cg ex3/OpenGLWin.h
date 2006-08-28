#pragma once

#include "gl\glut.h"

class CEx3MFCDlg;
class COpenGLWin
{
public:
	CEx3MFCDlg* m_pParent;
	CString getFileName(){return m_sFilename;}
	COpenGLWin(void);
	~COpenGLWin(void);
	void Run(CString sFilename);
	bool FilterToggle(int nCode = 0);
	void Close();
	void Save(CString filename);
	void Resize(int width, int height);
	void DisplayCallback();
	void mousePressedCallback( int inButton, int inState, int inX, int inY );
	void mouseMovedCallback( int inX, int inY );
	void keypress(unsigned char key, int x, int y);
	void idleFunc(void);
	bool gotoZero();
	bool playPause();
	void gotoFrame(int nFrame);
	void gotoNextFrame();
	void gotoPrevFrame();
	void Initialize();
	int getFrameCount();
	int getCurFrame();
	bool isPaused();

protected:
    bool m_bLoaded;
    bool m_bCreated;
    CString m_sFilename;
};

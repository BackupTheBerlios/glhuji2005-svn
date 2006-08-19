// Ex3MFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CEx3MFCApp:
// See Ex3MFC.cpp for the implementation of this class
//

class CEx3MFCApp : public CWinApp
{
public:
	CEx3MFCApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEx3MFCApp theApp;
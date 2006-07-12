// Ex2MFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CEx2MFCApp:
// See Ex2MFC.cpp for the implementation of this class
//

class CEx2MFCApp : public CWinApp
{
public:
	CEx2MFCApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEx2MFCApp theApp;
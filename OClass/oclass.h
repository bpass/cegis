// classy.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// COClassApp:
// See classy.cpp for the implementation of this class
//

class COClassApp : public CWinApp
{
public:
	COClassApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COClassApp theApp;
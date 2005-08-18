#if !defined(AFX_TARDEM_H__C029610B_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_)
#define AFX_TARDEM_H__C029610B_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// tardem.h : main header file for TARDEM.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTardemApp : See tardem.cpp for implementation.

class CTardemApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARDEM_H__C029610B_ACE2_11D3_AD09_0050048D1D2C__INCLUDED)

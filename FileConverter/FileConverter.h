// FileConverter.h : main header file for the FILECONVERTER application
//

#if !defined(AFX_FILECONVERTER_H__0B0A754C_4D97_402E_B17B_84F47C872B9F__INCLUDED_)
#define AFX_FILECONVERTER_H__0B0A754C_4D97_402E_B17B_84F47C872B9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileConverterApp:
// See FileConverter.cpp for the implementation of this class
//

class CFileConverterApp : public CWinApp
{
public:
	CFileConverterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileConverterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileConverterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECONVERTER_H__0B0A754C_4D97_402E_B17B_84F47C872B9F__INCLUDED_)

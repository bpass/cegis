#if !defined(AFX_TARDEMPPG_H__C0296115_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_)
#define AFX_TARDEMPPG_H__C0296115_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TardemPpg.h : Declaration of the CTardemPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CTardemPropPage : See TardemPpg.cpp.cpp for implementation.

class CTardemPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CTardemPropPage)
	DECLARE_OLECREATE_EX(CTardemPropPage)

// Constructor
public:
	CTardemPropPage();

// Dialog Data
	//{{AFX_DATA(CTardemPropPage)
	enum { IDD = IDD_PROPPAGE_TARDEM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CTardemPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARDEMPPG_H__C0296115_ACE2_11D3_AD09_0050048D1D2C__INCLUDED)

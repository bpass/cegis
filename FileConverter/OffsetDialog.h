#if !defined(AFX_OFFSETDIALOG_H__33CC4AD3_19D9_4AAB_952C_A9BFD8CACAE2__INCLUDED_)
#define AFX_OFFSETDIALOG_H__33CC4AD3_19D9_4AAB_952C_A9BFD8CACAE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OffsetDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COffsetDialog dialog

class COffsetDialog : public CDialog
{
// Construction
public:
	COffsetDialog(double *Offset, CWnd* pParent = NULL);   // added double *Offset RS
// Dialog Data
	//{{AFX_DATA(COffsetDialog)
	enum { IDD = IDD_GetOffset };
	double	m_Offset;
   double *pOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COffsetDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COffsetDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OFFSETDIALOG_H__33CC4AD3_19D9_4AAB_952C_A9BFD8CACAE2__INCLUDED_)

/* Programmer: Ryan Stelzleni
 * Date: 6-3-04
 *
 * Purpose: This file defines a gui intended to make it easy to convert
 *          some automatically generated files into a more useful format.
 */




// FileConverterDlg.h : header file
//

#if !defined(AFX_FILECONVERTERDLG_H__2F7B6305_F452_442C_957D_598303547833__INCLUDED_)
#define AFX_FILECONVERTERDLG_H__2F7B6305_F452_442C_957D_598303547833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFileConverterDlg dialog

class CFileConverterDlg : public CDialog
{
// Construction
public:
	CFileConverterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileConverterDlg)
	enum { IDD = IDD_FILECONVERTER_DIALOG };
	CString	m_szFromFileOne;
	CString	m_szToFileOne;
	CString	m_szFromFileTwo;
	CString	m_szToFileTwo;
	CString	m_szFromFileThree;
	CString	m_szToFileThree;
   CString  m_szFromFileOneFull;
   CString  m_szFromFileTwoFull;
   CString  m_szFromFileThreeFull;
   CString  m_szToFileOneFull;
   CString  m_szToFileTwoFull;
   CString  m_szToFileThreeFull;
   double m_fOffsetOne;
   double m_fOffsetTwo;
   double m_fOffsetThree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileConverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileConverterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFromFileOne();
	afx_msg void OnFromFileTwo();
	afx_msg void OnFromFileThree();
	afx_msg void OnToFileOne();
	afx_msg void OnToFileThree();
	afx_msg void OnToFileTwo();
	afx_msg void OnExitButton();
	afx_msg void OnConvertButton();
	afx_msg void OnClearButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILECONVERTERDLG_H__2F7B6305_F452_442C_957D_598303547833__INCLUDED_)

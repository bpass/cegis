// classyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"

// CclassyDlg dialog
class CclassyDlg : public CDialog
{
// Construction
public:
	CclassyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLASSY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void enableControls(BOOL enable);
	void waitForClassifier();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedBrowse();
	int m_curDataType;
	CString m_filename;
	CComboBox m_dataType;
	
	BOOL m_CLAOutput;
	int m_numClasses;
	BOOL m_htmlOutput;
	CEdit m_FileEdit;
	CEdit m_numClassesEdit;
	CButton m_htmlCheckBox;
	CButton m_claCheckBox;
	CButton m_browseButton;
	CButton m_quitButton;
	CButton m_runButton;
	BOOL m_textOutput;
	CButton m_textCheckBox;
	afx_msg void OnBnClickedGenmodel();
	CProgressCtrl m_progress;

	
	CEdit m_claFile;
	CButton m_claButton;
	afx_msg void OnBnClickedClaoutput();
	CStatic m_claEditStaticText;
	afx_msg void OnBnClickedBrowsecla();
	CString m_claFileName;
	CButton m_genModelButton;
};



// classyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>

// CclassyDlg dialog
class genModelDlg : public CDialog
{
// Construction
public:
	genModelDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GENMODEL_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	CString m_origFile;
	CString m_outputFile;
	std::vector<CString> m_files;
	afx_msg void onBnClickedBrowseInput();
	afx_msg void onBnClickedBrowseOutput();
	afx_msg void onBnClickedGenerate();
	CString m_claFile;
	afx_msg void OnBnClickedGenmodelBrowseclafile();
	CString m_modelName;
	afx_msg void OnBnClickedGenmodelBrowsemodeloutput();
};

// GenModelDlg.h : header file
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
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_origFile; //original Imagine image file
	CString m_outputFile; //newly reclassified Imagine image file
	CString m_claFile; //cla file that contains the classification information
	CString m_modelName; //the model file to generate

	CButton m_browseClaButton; 
	CButton m_browseOrigButton;
	CButton m_browseMdlButton;
	CButton m_browseImgButton;
	CButton m_generateButton;
	CButton m_cancelButton;

	CEdit m_claEdit;
	CEdit m_origEdit;
	CEdit m_imgEdit;
	CEdit m_mdlEdit;

	CToolTipCtrl m_toolTips;

	afx_msg void onBnClickedBrowseInput();
	afx_msg void onBnClickedBrowseOutput();
	afx_msg void onBnClickedGenerate();
	afx_msg void OnBnClickedGenmodelBrowseclafile();
	afx_msg void OnBnClickedGenmodelBrowsemodeloutput();

};

// classyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"

// COClassDlg dialog
class COClassDlg : public CDialog
{
// Construction
public:
	COClassDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OCLASS_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	
	//data members
	int m_curDataType;
	size_t m_numLayers;
	size_t m_numClasses;
	size_t m_imageWidth;
	size_t m_imageHeight;

	BOOL m_CLAOutput;
	BOOL m_htmlOutput;
	BOOL m_textOutput;
	CString m_claFileName;
	CString m_imageFileName;

	//controls
	CComboBox m_dataType;
	
	CButton m_htmlCheckBox;
	CButton m_claCheckBox;
	CButton m_browseButton;
	CButton m_quitButton;
	CButton m_runButton;
	CButton m_textCheckBox;
	CProgressCtrl m_progress;
	CEdit m_claFile;
	CEdit m_imageFileEdit;
	CEdit m_numLayersEdit;
	CEdit m_numClassesEdit;
	CEdit m_imageWidthEdit;
	CEdit m_imageHeightEdit;
	CButton m_claButton;
	CButton m_imageFileButton;
	CStatic m_claEditStaticText;
	CButton m_genModelButton;
	CListBox m_fileList;
	CStatic m_progText;
	CButton m_addFiles;
	CButton m_removeFiles;

	//control functions
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedGenmodel();
	afx_msg void OnBnClickedBrowsecla();
	afx_msg void OnBnClickedBrowseImage();
	afx_msg void OnBnClickedClaoutput();
};



// classyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"
#include <string>
#include <vector>

class BatchJobDlg;

struct classificationParams {
	classificationParams():
		 dataType(0),
		 numClasses(0),
		 numLayers(0),
		 imageWidth(0),
		 imageHeight(0),
		 claOutput(false),
		 htmlOutput(false),
		 textOutput(false) { }

    classificationParams(int _dataType, size_t _numClasses, size_t _numLayers, size_t _imageWidth,
						 size_t _imageHeight, bool _claOutput, bool _htmlOutput, bool _textOutput,
						 std::string _imageFileName, std::string _claFileName):
						 dataType(_dataType),
						 numClasses(_numClasses),
						 numLayers(_numLayers),
						 imageWidth(_imageWidth),
						 imageHeight(_imageHeight),
						 claOutput(_claOutput),
						 htmlOutput(_htmlOutput),
						 textOutput(_textOutput),
						 imageFileName(_imageFileName),
						 claFileName(_claFileName) { }
						 
	int dataType;
	size_t numClasses;
	size_t numLayers;
	size_t imageWidth;
	size_t imageHeight;
	bool claOutput;
	bool htmlOutput;
	bool textOutput;
	std::string imageFileName;
	std::string claFileName;
};

// COClassDlg dialog
class COClassDlg : public CDialog
{
// Construction
public:
	COClassDlg(CWnd* pParent = NULL);	// standard constructor
	~COClassDlg();
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
	std::vector<classificationParams*> m_batchParams;
	classificationParams m_curConfig;
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

	//Dialogs
	BatchJobDlg* m_batchDlg;

	//control functions
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedGenmodel();
	afx_msg void OnBnClickedBrowsecla();
	afx_msg void OnBnClickedBrowseImage();
	afx_msg void OnBnClickedClaoutput();
	afx_msg void OnBnClickedBatch();

	//helper functions
	void updateParams();
	void loadParams(classificationParams* params);
	classificationParams getCurrentConfig();
	std::vector<classificationParams*> getJobList() {return m_batchParams;}
	void deleteJob(size_t index);
	void addJob(classificationParams* params);
	void clearJobList();
	bool updateAndCheckInput();
	void editJob(size_t index, classificationParams* newParams);
	void runBatchJob(size_t index);
	void runClassification(classificationParams* params);
	
	
};



// classyDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"
#include <string>
#include <vector>

class BatchJobDlg;

//struct to store all parameters neccessary for running 
//an image classification
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
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	//data members
	int m_curDataType; //currently selected data type
	size_t m_numLayers; //number of layers in the image
	size_t m_numClasses; //number of classes to generate
	size_t m_imageWidth; //width of the image (in pixels)
	size_t m_imageHeight; //height of the image (in pixels)
	BOOL m_CLAOutput; //check for outputting CLA file
	BOOL m_htmlOutput; //check for outputting HTML file 
	BOOL m_textOutput; //check for outputting text file
	CString m_claFileName; //cla filename to output
	CString m_imageFileName; //filename of the image being classified
	CString m_numLayersString;
	CString m_numClassesString;
	CString m_WidthString;
	CString m_HeightString;

	std::vector<classificationParams*> m_batchParams; //job queue for use with BatchJobDlg
	classificationParams m_curConfig; //the current configuration

	//controls
	CComboBox m_dataType;
	CButton m_htmlCheckBox;
	CButton m_claCheckBox;
	CButton m_browseButton;
	CButton m_quitButton;
	CButton m_runButton;
	CButton m_textCheckBox;
	CButton m_claButton;
	CButton m_imageFileButton;
	CButton m_genModelButton;
	CButton m_addFiles;
	CButton m_removeFiles;
	CButton m_batchButton;
	
	CEdit m_claFile;
	CEdit m_imageFileEdit;
	CEdit m_numLayersEdit;
	CEdit m_numClassesEdit;
	CEdit m_imageWidthEdit;
	CEdit m_imageHeightEdit;
	CStatic m_claEditStaticText;
	CListBox m_fileList;
	CStatic m_progText;
	CToolTipCtrl m_toolTips;

	CProgressCtrl m_progress;
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

	//pulls out current field values and places another entry
	//in the batch job queue
	void updateParams();

	//populate fields with some specific values
	void loadParams(classificationParams* params); 

	//get the current field values
	classificationParams getCurrentConfig();

	//get the current job list
	std::vector<classificationParams*> getJobList() {return m_batchParams;}

	//remove a job from the queue
	void deleteJob(size_t index);

	//add a job to the queue
	void addJob(classificationParams* params);

	//clear the job queue
	void clearJobList();

	//check all fields for errors and set m_curConfig to new values.
	bool updateAndCheckInput();

	//modify a job in the queue
	void editJob(size_t index, classificationParams* newParams);

	//run a particular job in the queue
	void runBatchJob(size_t index);

	//run a single classification
	void runClassification(classificationParams* params);

	//enable / disable all controls
	void enableControls(BOOL enable);

	bool isNumeric(CString num);
	
	
};



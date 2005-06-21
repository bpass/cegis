

#pragma once
#include "afxwin.h" 
class BatchJobDlg : public CDialog {
public:
	BatchJobDlg(CWnd* parent = NULL);

	enum {IDD = IDD_JOBDIALOG};

protected:
	virtual void DoDataExchange(CDataExchange* dx);

	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_editChecked; //are we in edit mode?
	
	CListBox m_jobList; //listbox control containing the list of jobs in the queue
	CButton m_addCurrentButton; //button to add a new job.
	CButton m_removeButton; //button to remove a job.
	CButton m_saveButton; //button to save current job list.
	CButton m_loadButton; //button to load a previously saved list
	CButton m_runButton; //button to run all queued jobs
	CButton m_closeButton; //button to close the job dialog
	CButton m_clearButton; //button to clear the job queue
	CButton m_editCheckBox; //check box to enter edit mode.
	CProgressCtrl m_progBar; //total job progress bar
	CString m_progBarText; //displays the filename of the current image being processed.
	CStatic m_progBarStatic;
	CToolTipCtrl m_toolTips; //tool tips for all controls

	//control functions
	afx_msg void onAddCurrentButton();
	afx_msg void onRemoveButton();
	afx_msg void onSaveButton();
	afx_msg void onLoadButton();
	afx_msg void onRunButton();
	afx_msg void onCloseButton();

private:
	void refreshJobList(); //refresh the job queue in the list box
	void saveJobList(const char* filename); //save the job list out to a file
	void loadJobList(const char* filename); //load the job list from a file.
	void enableControls(BOOL enable); //enable/disable all controls.
	void setListBoxHExtent(); //set the horizontal extent of the list box 
							  //based on the longest string.
public:
	afx_msg void OnBnClickedClear();
	afx_msg void OnLbnSelchangeJoblist();
	afx_msg void OnBnClickedEditJob();
};
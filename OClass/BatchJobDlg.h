

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
	BOOL m_editChecked;
	
	CListBox m_jobList;
	CButton m_addCurrentButton;
	CButton m_removeButton;
	CButton m_saveButton;
	CButton m_loadButton;
	CButton m_runButton;
	CButton m_closeButton;
	CButton m_clearButton;
	CButton m_editCheckBox;
	CProgressCtrl m_progBar;
	CString m_progBarText;
	CStatic m_progBarStatic;
	CToolTipCtrl m_toolTips;

	//control functions
	afx_msg void onAddCurrentButton();
	afx_msg void onRemoveButton();
	afx_msg void onSaveButton();
	afx_msg void onLoadButton();
	afx_msg void onRunButton();
	afx_msg void onCloseButton();

private:
	void refreshJobList();
	void saveJobList(const char* filename);
	void loadJobList(const char* filename);
	void enableControls(BOOL enable);
	void setListBoxHExtent();
public:
	afx_msg void OnBnClickedClear();
	afx_msg void OnLbnSelchangeJoblist();
	afx_msg void OnBnClickedEditJob();
};
// classyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "classy.h"
#include "classyDlg.h"
#include ".\classydlg.h"
#include "ImageClassifier.h"
#include "ModelMaker.h"
#include "genModelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


		
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CclassyDlg dialog



CclassyDlg::CclassyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CclassyDlg::IDD, pParent)
	, m_filename(_T(""))
	, m_CLAOutput(FALSE)
	, m_numClasses(0)
	, m_htmlOutput(FALSE)
	, m_textOutput(FALSE)
	
	, m_claFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USGS);
	

}

void CclassyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FILE, m_filename);
	DDV_MaxChars(pDX, m_filename, 255);
	DDX_Control(pDX, IDC_DATATYPE, m_dataType);

	DDX_Check(pDX, IDC_CLAOUTPUT, m_CLAOutput);
	DDX_Text(pDX, IDC_NUMCLASSES, m_numClasses);
	DDX_Check(pDX, IDC_HTMLOUTPUT, m_htmlOutput);
	DDX_Control(pDX, IDC_FILE, m_FileEdit);
	DDX_Control(pDX, IDC_NUMCLASSES, m_numClassesEdit);
	DDX_Control(pDX, IDC_HTMLOUTPUT, m_htmlCheckBox);
	DDX_Control(pDX, IDC_CLAOUTPUT, m_claCheckBox);
	DDX_Control(pDX, IDC_BROWSE, m_browseButton);
	DDX_Control(pDX, IDQUIT, m_quitButton);
	DDX_Control(pDX, IDRUN, m_runButton);
	DDX_Check(pDX, IDC_TextOutput, m_textOutput);
	DDX_Control(pDX, IDC_TextOutput, m_textCheckBox);
	DDX_Control(pDX, IDC_CLAFILE, m_claFile);
	DDX_Control(pDX, IDC_BROWSECLA, m_claButton);
	DDX_Control(pDX, IDC_CLA_STATIC_TEXT, m_claEditStaticText);
	DDX_Text(pDX, IDC_CLAFILE, m_claFileName);
	DDX_Control(pDX, IDGENMODEL, m_genModelButton);
}

BEGIN_MESSAGE_MAP(CclassyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDQUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDRUN, OnBnClickedRun)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDGENMODEL, OnBnClickedGenmodel)
	ON_BN_CLICKED(IDC_CLAOUTPUT, OnBnClickedClaoutput)
	ON_BN_CLICKED(IDC_BROWSECLA, OnBnClickedBrowsecla)
END_MESSAGE_MAP()


// CclassyDlg message handlers

BOOL CclassyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_dataType.SetCurSel(0);
	m_numClasses = 2;
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CclassyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CclassyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CclassyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CclassyDlg::OnBnClickedQuit()
{
	PostQuitMessage(0);
}

void CclassyDlg::OnBnClickedRun()
{
	UpdateData();
	/*
	int*** a;
	int* b = new int[5];

	a = new int**[5];
	for(int i = 0; i < 5; i++) {
		a[i] = new int*[5];
		b[i] = i;
		for(int j = 0; j < 5; j++) {
			a[i][j] = new int[2];
			a[i][j][0] = 5;
			a[i][j][1] = 10;
		}
	}
	
	try {
	ModelMaker<int> c("d:\\input", "d:\\output", b, a, 5, S16, 5);
	c.generate("d:\\working\\test.mdl");
	}
	catch(GeneralException e) {
		AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
	}
	*/

	
	if(m_filename.IsEmpty()) {
		AfxMessageBox("Please specify an input file.", MB_ICONWARNING | MB_OK);
		return;
	}
	
	if(m_CLAOutput && (m_claFileName.GetLength() == 0)) {
		AfxMessageBox("Please specify a CLA output file.", MB_ICONWARNING | MB_OK);
		return;
	}

	if(m_numClasses < 2) {
		AfxMessageBox("Please enter a positive integer >= 2 for the number of classes.", MB_ICONWARNING | MB_OK);
		return;
	}

	if(!m_htmlOutput && !m_textOutput && !m_CLAOutput) {
		AfxMessageBox("Please choose at least one output format.", MB_ICONWARNING | MB_OK);
		return;
	}
	
	
	CString cstringHtmlFilename = m_filename + ".html";
	CString cstringTextFilename = m_filename + ".txt";
	const char* filename = NULL;
	m_curDataType = m_dataType.GetCurSel();

	//disable all controls
	enableControls(FALSE);
	m_runButton.SetWindowText("Working");
	
	switch(m_curDataType) {
		case 0: {
			try {
				ImageClassifier<unsigned char> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				

				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 1: {
			try {
				ImageClassifier<char> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 2: {
			try {
				ImageClassifier<unsigned int> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 3: {
			try {
				ImageClassifier<int> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 4: {
			try {
				ImageClassifier<unsigned long> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 5: {
			try {
				ImageClassifier<long> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 6: {
			try {
				ImageClassifier<float> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
			}

			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}
			break;
		case 7: {
			try {
				ImageClassifier<double> classifier(m_filename, m_numClasses);
				classifier.classify();
				if(m_CLAOutput) 
					classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

				if(m_htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(m_textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
				
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}

		
	}

	enableControls(TRUE);
	m_runButton.SetWindowText("Run");
	AfxMessageBox("Classification Complete",MB_ICONINFORMATION | MB_OK);
}

void CclassyDlg::OnBnClickedBrowse()
{
	UpdateData();
	CFileDialog fileDialog(TRUE);
	
	if(fileDialog.DoModal() == IDOK) 
		m_filename = fileDialog.GetPathName();
	
	else
		m_filename = "";

	UpdateData(FALSE);
}

void CclassyDlg::enableControls(BOOL enable) {
	m_dataType.EnableWindow(enable);
	m_FileEdit.EnableWindow(enable);
	m_numClassesEdit.EnableWindow(enable);
	m_htmlCheckBox.EnableWindow(enable);
	m_claCheckBox.EnableWindow(enable);
	m_browseButton.EnableWindow(enable);
	m_quitButton.EnableWindow(enable);
	m_runButton.EnableWindow(enable);
	m_textCheckBox.EnableWindow(enable);
	m_genModelButton.EnableWindow(enable);
}
void CclassyDlg::OnBnClickedGenmodel()
{
	genModelDlg dlg;
	dlg.DoModal();
}

		

void CclassyDlg::OnBnClickedClaoutput()
{
	UpdateData();
	if(m_CLAOutput) {
		m_claFile.EnableWindow(TRUE);
		m_claButton.EnableWindow(TRUE);
		m_claEditStaticText.EnableWindow(TRUE);
	}

	else {
		m_claFile.EnableWindow(FALSE);
		m_claButton.EnableWindow(FALSE);
		m_claEditStaticText.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CclassyDlg::OnBnClickedBrowsecla()
{
	UpdateData();
	CFileDialog dlg(TRUE, "*.cla", NULL, OFN_OVERWRITEPROMPT, "Classification Files (*.cla)|*.cla||", NULL, 0);
	if(dlg.DoModal() == IDOK) 
		m_claFileName = dlg.GetPathName();
	
	else
		m_claFileName = "";

	UpdateData(FALSE);
}

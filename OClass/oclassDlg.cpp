// classyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "oclass.h"
#include "oclassDlg.h"
#include ".\oclass.h"
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


// COClassDlg dialog



COClassDlg::COClassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COClassDlg::IDD, pParent)
	, m_CLAOutput(FALSE)
	, m_numClasses(0)
	, m_htmlOutput(FALSE)
	, m_textOutput(FALSE)
	
	, m_claFileName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USGS);
	

}

void COClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATATYPE, m_dataType);

	DDX_Check(pDX, IDC_CLAOUTPUT, m_CLAOutput);
	DDX_Text(pDX, IDC_NUMCLASSES, m_numClasses);
	DDX_Check(pDX, IDC_HTMLOUTPUT, m_htmlOutput);
	DDX_Control(pDX, IDC_NUMCLASSES, m_numClassesEdit);
	DDX_Control(pDX, IDC_HTMLOUTPUT, m_htmlCheckBox);
	DDX_Control(pDX, IDC_CLAOUTPUT, m_claCheckBox);
	DDX_Control(pDX, IDQUIT, m_quitButton);
	DDX_Control(pDX, IDRUN, m_runButton);
	DDX_Check(pDX, IDC_TextOutput, m_textOutput);
	DDX_Control(pDX, IDC_TextOutput, m_textCheckBox);
	DDX_Control(pDX, IDC_CLAFILE, m_claFile);
	DDX_Control(pDX, IDC_BROWSECLA, m_claButton);
	DDX_Control(pDX, IDC_CLA_STATIC_TEXT, m_claEditStaticText);
	DDX_Text(pDX, IDC_CLAFILE, m_claFileName);
	DDX_Control(pDX, IDGENMODEL, m_genModelButton);
	DDX_Control(pDX, IDC_INPUTFILELIST, m_fileList);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
	DDX_Control(pDX, IDC_PROGRESSTEXT, m_progText);
	DDX_Control(pDX, IDC_ADDFILES, m_addFiles);
	DDX_Control(pDX, IDC_REMOVEFILE, m_removeFiles);
}

BEGIN_MESSAGE_MAP(COClassDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDQUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDRUN, OnBnClickedRun)
	ON_BN_CLICKED(IDGENMODEL, OnBnClickedGenmodel)
	ON_BN_CLICKED(IDC_CLAOUTPUT, OnBnClickedClaoutput)
	ON_BN_CLICKED(IDC_BROWSECLA, OnBnClickedBrowsecla)
	ON_BN_CLICKED(IDC_ADDFILES, OnBnClickedAddfiles)
	ON_BN_CLICKED(IDC_REMOVEFILE, OnBnClickedRemovefile)
END_MESSAGE_MAP()


// COClassDlg message handlers

BOOL COClassDlg::OnInitDialog()
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

void COClassDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COClassDlg::OnPaint() 
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
HCURSOR COClassDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COClassDlg::OnBnClickedQuit()
{
	PostQuitMessage(0);
}

void COClassDlg::OnBnClickedRun()
{
	UpdateData();
	if(m_fileList.GetCount() == 0) {
		AfxMessageBox("Please specify at least one input file.", MB_ICONWARNING | MB_OK);
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
	
	
	CString cstringHtmlFilename;
	CString cstringTextFilename;
	CString curFile;
	const char* filename = NULL;
	
	m_curDataType = m_dataType.GetCurSel();
	int numFiles = m_fileList.GetCount();
	//disable all controls
	enableControls(FALSE);
	m_progressBar.ShowWindow(SW_SHOW);
	m_progText.ShowWindow(SW_SHOW);
	m_progressBar.SetRange(0, numFiles);
	m_progressBar.SetPos(0);
	if(numFiles > 1)
		m_progressBar.SetPos(1);
	UpdateWindow();
	
	switch(m_curDataType) {
		case 0: {
			try {
				for(int i = 0; i < numFiles; i++) {
					
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<unsigned char> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
					
				}	
			
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<char> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
				}	
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<unsigned int> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
				UpdateWindow();
				}	
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<int> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
				}	
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<unsigned long> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
				}	
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<long> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
				}	
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<float> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					
					UpdateWindow();
				}	
				
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
				for(int i = 0; i < numFiles; i++) {
					m_fileList.GetText(i, curFile);
					cstringHtmlFilename = curFile + ".html";
					cstringTextFilename = curFile + ".txt";
					ImageClassifier<double> classifier((LPCSTR)curFile, m_numClasses);
					classifier.classify();
					if(m_CLAOutput) 
						classifier.saveReportXML((LPCTSTR)m_claFileName, (DataType)m_curDataType);
				

					if(m_htmlOutput) 
						classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

					if(m_textOutput) 
						classifier.saveReport((LPCTSTR)cstringTextFilename);
					if(i > 0 || numFiles == 1)
						m_progressBar.SetPos(i);
					UpdateWindow();
				}	
				
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run");
			}
		}

		
	}
	m_progressBar.SetPos(numFiles);
	enableControls(TRUE);
	m_runButton.SetWindowText("Run Classification");
	AfxMessageBox("Classification Complete",MB_ICONINFORMATION | MB_OK);
	m_progressBar.ShowWindow(SW_HIDE);
	m_progText.ShowWindow(SW_HIDE);
	
}



void COClassDlg::enableControls(BOOL enable) {
	UpdateData();
	m_dataType.EnableWindow(enable);
	m_FileEdit.EnableWindow(enable);
	m_numClassesEdit.EnableWindow(enable);
	m_htmlCheckBox.EnableWindow(enable);
	m_claCheckBox.EnableWindow(enable);
	m_quitButton.EnableWindow(enable);
	m_runButton.EnableWindow(enable);
	m_textCheckBox.EnableWindow(enable);
	m_genModelButton.EnableWindow(enable);
	m_addFiles.EnableWindow(enable);
	m_removeFiles.EnableWindow(enable);
	m_fileList.EnableWindow(enable);

	if(m_CLAOutput) {
		m_claFile.EnableWindow(enable);
		m_claButton.EnableWindow(enable);
		m_claEditStaticText.EnableWindow(enable);
	}
	UpdateData(FALSE);
}
void COClassDlg::OnBnClickedGenmodel()
{
	genModelDlg dlg;
	dlg.DoModal();
}

		

void COClassDlg::OnBnClickedClaoutput()
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

void COClassDlg::OnBnClickedBrowsecla()
{
	UpdateData();
	CFileDialog dlg(TRUE, "*.cla", NULL, OFN_OVERWRITEPROMPT, "Classification Files (*.cla)|*.cla||", NULL, 0);
	if(dlg.DoModal() == IDOK) 
		m_claFileName = dlg.GetPathName();
	
	

	UpdateData(FALSE);
}

void COClassDlg::setListHExtent() {
	int numEntries = m_fileList.GetCount();
	int max = 0;
	int curLen = 0;
	CString currentString;
	
	//get the length of the longest string in the list box
	for(int i  = 0; i < numEntries; i++) {
		m_fileList.GetText(i, currentString);
		curLen = currentString.GetLength();
		if(curLen > max)
			max = curLen;
	}

	m_fileList.SetHorizontalExtent(max*5 + max/2);
}



void COClassDlg::OnBnClickedAddfiles()
{
	//UpdateData();
	const long buffSize = 100000;
	char fileBuffer[buffSize] = {'\0'};
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT);
	dlg.m_ofn.lpstrFile = fileBuffer;
	dlg.m_ofn.nMaxFile = buffSize;
	POSITION pos;
	CString curString;
	if(dlg.DoModal() == IDOK) {
		pos = dlg.GetStartPosition();
		while(pos) {
			curString = dlg.GetNextPathName(pos);
			m_fileList.AddString((LPCSTR)curString);
		}
		setListHExtent();
	}
}

void COClassDlg::OnBnClickedRemovefile()
{
	const int numSelected = m_fileList.GetSelCount();
	int* selIndices = new int[numSelected];
	m_fileList.GetSelItems(numSelected, selIndices);
	for(int i = 0; i < numSelected; i++) {
		m_fileList.DeleteString(selIndices[0]);
		setListHExtent();
		//get updated selected item list because
		//deleteString shifts the items each time 
		//one is deleted.
		m_fileList.GetSelItems(numSelected, selIndices);
	}


	delete[] selIndices;
}

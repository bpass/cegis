// classyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "classy.h"
#include "genModelDlg.h"
#include ".\genModelDlg.h"

#include "ModelMaker.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About



// CclassyDlg dialog


genModelDlg::genModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(genModelDlg::IDD, pParent)
	, m_outputFile(_T(""))
	, m_origFile(_T(""))
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USGS);
	

}

void genModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_GENMODEL_ORIGINPUT, m_origFile);
	DDV_MaxChars(pDX, m_origFile, 255);
	DDX_Text(pDX, IDC_GENMODEL_OUTPUT, m_outputFile);
	DDV_MaxChars(pDX, m_outputFile, 255);

	DDX_Control(pDX, IDC_GENMODEL_INPUTFILES, m_inputList);
	
}

BEGIN_MESSAGE_MAP(genModelDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDC_GENMODEL_BROWSEINPUT, onBnClickedBrowseInput)
	ON_BN_CLICKED(IDC_GENMODEL_BROWSEOUTPUT, onBnClickedBrowseOutput)
	ON_BN_CLICKED(IDC_GENMODEL_ADDFILE, onBnClickedAddFile)
	ON_BN_CLICKED(ID_GENMODEL_GENERATE, onBnClickedGenerate)
	ON_BN_CLICKED(IDC_GENMODEL_REMOVEFILE, onBnClickedRemoveFile)
END_MESSAGE_MAP()


// CclassyDlg message handlers

BOOL genModelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void genModelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
		CDialog::OnSysCommand(nID, lParam);
	
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void genModelDlg::OnPaint() 
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
HCURSOR genModelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void genModelDlg::onBnClickedBrowseInput() {
}

void genModelDlg::onBnClickedBrowseOutput() {
}

void genModelDlg::onBnClickedAddFile() {
	UpdateData();
	POSITION startPosition;
	CString curPathName;
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER, NULL, NULL, 0);
	char fileBuffer[20000] ={'\0'};
	fileDialog.m_ofn.lpstrFile = fileBuffer;
	fileDialog.m_ofn.nMaxFile = 20000;

	if(fileDialog.DoModal() == IDOK) {
		startPosition = fileDialog.GetStartPosition();
		while(startPosition) {
			curPathName = fileDialog.GetNextPathName(startPosition);
			m_inputList.AddString((LPCTSTR)curPathName);
			m_files.push_back(curPathName);
			setExtent();
		}
	}
	UpdateData(FALSE);
}

void genModelDlg::onBnClickedRemoveFile() {
	int i = 0;
	int selected = m_inputList.GetCurSel();
	m_inputList.DeleteString(selected);
	std::vector<CString>::iterator iter;
	for(iter = m_files.begin(); i < selected; iter++) {
		i++;
	}

	m_files.erase(iter);
	setExtent();
	
	
}

void genModelDlg::onBnClickedGenerate() {
}

void genModelDlg::setExtent() {
	int numStrings = m_inputList.GetCount();
	int largest = 0;
	CString curString;
	for(int i = 0; i < numStrings; i++) {
		m_inputList.GetText(i, curString);
		if(curString.GetLength() > largest)
			largest = curString.GetLength();
	}

	m_inputList.SetHorizontalExtent(largest*5 + largest/2);
}
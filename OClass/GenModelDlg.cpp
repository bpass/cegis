// classyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OClass.h"
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
	
	, m_claFile(_T(""))
	, m_modelName(_T(""))
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


	DDX_Text(pDX, IDC_GENMODEL_CLAFILEEDIT, m_claFile);
	DDX_Text(pDX, IDC_GENMODEL_OUTPUT2, m_modelName);
}

BEGIN_MESSAGE_MAP(genModelDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_BN_CLICKED(IDC_GENMODEL_BROWSEINPUT, onBnClickedBrowseInput)
	ON_BN_CLICKED(IDC_GENMODEL_BROWSEOUTPUT, onBnClickedBrowseOutput)
	ON_BN_CLICKED(ID_GENMODEL_GENERATE, onBnClickedGenerate)
	ON_BN_CLICKED(IDC_GENMODEL_BROWSECLAFILE, OnBnClickedGenmodelBrowseclafile)
	ON_BN_CLICKED(IDC_GENMODEL_BROWSEMODELOUTPUT, OnBnClickedGenmodelBrowsemodeloutput)
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
	UpdateData();
	CFileDialog dlg(TRUE, "*.img", NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, 
					"Imagine Image Files (*.img)|*.img||");
	if(dlg.DoModal() == IDOK) 
		m_origFile = dlg.GetPathName();
	
	UpdateData(FALSE);
}

void genModelDlg::onBnClickedBrowseOutput() {
	UpdateData();
	CFileDialog dlg(FALSE, "*.img", NULL, OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
					"Imagine Image Files (*.img)|*.img||");
	if(dlg.DoModal() == IDOK) 
		m_outputFile = dlg.GetPathName();
	
	UpdateData(FALSE);
}

void genModelDlg::onBnClickedGenerate() {
	UpdateData();
	if((m_origFile.GetLength() == 0) || (m_outputFile.GetLength() == 0) 
		|| (m_claFile.GetLength() == 0) || (m_modelName.GetLength() == 0)) {
		AfxMessageBox("All fields must be filled in to continue", MB_ICONWARNING | MB_OK);
		return;
	}
	try {
		ModelMaker mm((LPCTSTR)m_origFile, (LPCTSTR)m_outputFile, (LPCTSTR)m_claFile);
		mm.generate((LPCTSTR)m_modelName);
		AfxMessageBox("Model Generation Complete!",MB_ICONINFORMATION | MB_OK);
	}
	catch(GeneralException e) {
		AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
	}
}


void genModelDlg::OnBnClickedGenmodelBrowseclafile()
{
	UpdateData();
	CFileDialog dlg(TRUE, "*.img", NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, 
					"Classification Files (*.cla)|*.cla||");
	if(dlg.DoModal() == IDOK) 
		m_claFile = dlg.GetPathName();

	UpdateData(FALSE);
}

void genModelDlg::OnBnClickedGenmodelBrowsemodeloutput()
{
	UpdateData();
	CFileDialog dlg(FALSE, "*.img", NULL, OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
					"Imagine Spatial Modeler File (*.mdl)|*.mdl||");
	if(dlg.DoModal() == IDOK) 
		m_modelName = dlg.GetPathName();
	
	UpdateData(FALSE);
}

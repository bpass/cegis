// classyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "oclass.h"
#include "oclassDlg.h"
#include ".\oclass.h"
#include "ImageClassifier.h"
#include "ModelMaker.h"
#include "genModelDlg.h"
#include "BatchJobDlg.h"
#include ".\oclassdlg.h"

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
	, m_numLayers(0)
	, m_htmlOutput(FALSE)
	, m_textOutput(FALSE)
	, m_claFileName(_T(""))
	, m_imageWidth(0)
	, m_imageHeight(0)
	, m_batchDlg(NULL)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USGS);
}

COClassDlg::~COClassDlg() {
	if(m_batchDlg) {
		m_batchDlg->DestroyWindow();
		delete m_batchDlg;
	}

	for(int i = 0; i < m_batchParams.size(); i++) {
		if(m_batchParams[i])
			delete m_batchParams[i];
	}
}

void COClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DATATYPE, m_dataType);

	DDX_Check(pDX, IDC_CLAOUTPUT, m_CLAOutput);
	DDX_Text(pDX, IDC_NUMCLASSES, m_numClasses);
	DDX_Text(pDX, IDC_NUM_LAYERS, m_numLayers);
	DDX_Text(pDX, IDC_IMAGE_FILE, m_imageFileName);
	DDX_Text(pDX, IDC_IMAGE_HEIGHT, m_imageHeight);
	DDX_Text(pDX, IDC_IMAGE_WIDTH, m_imageWidth);
	DDX_Control(pDX, IDC_IMAGE_HEIGHT, m_imageHeightEdit);
	DDX_Control(pDX, IDC_IMAGE_WIDTH, m_imageWidthEdit);
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
	DDX_Control(pDX, IDC_IMAGE_FILE, m_imageFileEdit);
	DDX_Control(pDX, IDC_NUM_LAYERS, m_numLayersEdit);
	DDX_Control(pDX, IDC_BROWSE_IMAGE, m_imageFileButton);
	DDX_Control(pDX, IDBATCH, m_batchButton);
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
	ON_BN_CLICKED(IDC_BROWSE_IMAGE, OnBnClickedBrowseImage)
	ON_BN_CLICKED(IDBATCH, OnBnClickedBatch)
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

	m_toolTips.Create(this);
	m_toolTips.AddTool(&m_htmlCheckBox, "Output report to an HTML file.");
	m_toolTips.AddTool(&m_claCheckBox, "Output report to a CLA file.");
	m_toolTips.AddTool(&m_textCheckBox, "Output report to a plain text file.");
	m_toolTips.AddTool(&m_imageFileButton, "Browse for an image file.");
	m_toolTips.AddTool(&m_quitButton, "Close the program.");
	m_toolTips.AddTool(&m_runButton, "Run classification with current parameters.");
	m_toolTips.AddTool(&m_claButton, "Browse for CLA output file.");
	m_toolTips.AddTool(&m_genModelButton, "Open model generator.");
	m_toolTips.AddTool(&m_dataType, "Image data type.");
	m_toolTips.AddTool(&m_claFile, "CLA output file.");
	m_toolTips.AddTool(&m_imageFileEdit, "Image file to classify.");
	m_toolTips.AddTool(&m_numLayersEdit, "Number of layers in the image.");
	m_toolTips.AddTool(&m_numClassesEdit, "Number of classes you want to generate.");
	m_toolTips.AddTool(&m_imageWidthEdit, "Width of the image (in pixels).");
	m_toolTips.AddTool(&m_imageHeightEdit, "Height of the image (in pixels).");
	m_toolTips.AddTool(&m_batchButton, "Open the batch queue dialog.");

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL COClassDlg::PreTranslateMessage(MSG* pMsg) {
	 
	switch(pMsg->message) {
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE:

		m_toolTips.RelayEvent(pMsg);
	}

return CDialog::PreTranslateMessage(pMsg);
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
	
	classificationParams* params = NULL;
	if(!updateAndCheckInput()) 
		return;
	
	
	classificationParams* newParams = new classificationParams(m_curDataType,
															m_numClasses,
															m_numLayers,
															m_imageWidth,
															m_imageHeight,
															(bool)m_CLAOutput,
															(bool)m_htmlOutput,
															(bool)m_textOutput,
															(LPCTSTR)m_imageFileName,
															(LPCTSTR)m_claFileName);
	enableControls(FALSE);
	runClassification(newParams);

	enableControls(TRUE);
	m_runButton.SetWindowText("Run Classification");
	AfxMessageBox("Classification Complete",MB_ICONINFORMATION | MB_OK);
	delete newParams;

}


void COClassDlg::enableControls(BOOL enable) {
	UpdateData();
	m_dataType.EnableWindow(enable);
	m_imageFileEdit.EnableWindow(enable);
	m_numClassesEdit.EnableWindow(enable);
	m_htmlCheckBox.EnableWindow(enable);
	m_claCheckBox.EnableWindow(enable);
	m_quitButton.EnableWindow(enable);
	m_runButton.EnableWindow(enable);
	m_textCheckBox.EnableWindow(enable);
	m_genModelButton.EnableWindow(enable);
	m_batchButton.EnableWindow(enable);
	m_numLayersEdit.EnableWindow(enable);
	m_imageWidthEdit.EnableWindow(enable);
	m_imageHeightEdit.EnableWindow(enable);
	m_imageFileButton.EnableWindow(enable);

	if(m_CLAOutput) {
		m_claFile.EnableWindow(enable);
		m_claButton.EnableWindow(enable);
		m_claEditStaticText.EnableWindow(enable);
	}
	UpdateData(FALSE);
	UpdateWindow();
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
		m_claFileName = "";
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

void COClassDlg::OnBnClickedBrowseImage() {
	UpdateData();
	CFileDialog dlg(TRUE, NULL, "*.bsq", OFN_READONLY, "Image Files (*.bsq)|*.bsq||", NULL, 0);
	if(dlg.DoModal() == IDOK)
		m_imageFileName = dlg.GetPathName();

	UpdateData(FALSE);
}



void COClassDlg::OnBnClickedBatch()
{
	if(!m_batchDlg) {
		m_batchDlg = new BatchJobDlg(this);
		m_batchDlg->Create(BatchJobDlg::IDD, this);
		m_batchDlg->ShowWindow(SW_SHOW);
	}

	else {
		m_batchDlg->ShowWindow(SW_SHOW);
		m_batchDlg->SetForegroundWindow();
	}
}

void COClassDlg::updateParams() {
	if(!updateAndCheckInput())
		return;

	else {
		classificationParams* newParams = new classificationParams(m_curDataType,
																   m_numClasses,
																   m_numLayers,
																   m_imageWidth,
																   m_imageHeight,
																   (bool)m_CLAOutput,
																   (bool)m_htmlOutput,
																   (bool)m_textOutput,
																   (LPCTSTR)m_imageFileName,
																   (LPCTSTR)m_claFileName);
		m_batchParams.push_back(newParams);
		
	}
}

bool COClassDlg::updateAndCheckInput() {
	SetForegroundWindow();
	UpdateData();
	
	m_curDataType = m_dataType.GetCurSel();
	
	if(m_imageFileName.GetLength() == 0) {
		AfxMessageBox("Please specify an image file.", MB_ICONWARNING | MB_OK);
		return false;
	}
	
	if(m_CLAOutput && (m_claFileName.GetLength() == 0)) {
		AfxMessageBox("Please specify a CLA output file.", MB_ICONWARNING | MB_OK);
		return false;
	}

	if(m_numClasses < 2) {
		AfxMessageBox("Please enter a positive integer >= 2 for the number of classes.", MB_ICONWARNING | MB_OK);
		return false;
	}
	
	if(m_numLayers < 1) {
		AfxMessageBox("The number of layers in the image must be > 1.", MB_ICONWARNING | MB_OK);
		return false;
	}

	if(m_imageWidth == 0 || m_imageHeight == 0) {
		AfxMessageBox("The width and height of the image must be > 0.", MB_ICONWARNING | MB_OK);
		return false;
	}

	if(!m_htmlOutput && !m_textOutput && !m_CLAOutput) {
		AfxMessageBox("Please choose at least one output format.", MB_ICONWARNING | MB_OK);
		return false;
	}
	
	m_curConfig.dataType      = m_curDataType;
	m_curConfig.numClasses    = m_numClasses;
	m_curConfig.numLayers     = m_numLayers;
	m_curConfig.imageWidth    = m_imageWidth;
	m_curConfig.imageHeight   = m_imageHeight;
	m_curConfig.claOutput     = (bool)m_CLAOutput;
	m_curConfig.htmlOutput    = (bool)m_htmlOutput;
	m_curConfig.textOutput    = (bool)m_textOutput;
	m_curConfig.imageFileName = (LPCTSTR)m_imageFileName;
	m_curConfig.claFileName   = (LPCTSTR)m_claFileName;
	
	return true;
}

void COClassDlg::deleteJob(size_t index) {
	classificationParams* temp = NULL;
	std::vector<classificationParams*> tempVec;
	size_t size = m_batchParams.size();
	if(index >= size)
		return;

	for(size_t i = 0; i < m_batchParams.size(); i++) {
		if(i != index)
			tempVec.push_back(m_batchParams[i]);
	}
	delete m_batchParams[index];
	m_batchParams = tempVec;
}

void COClassDlg::runBatchJob(size_t index) {
	if(index >= m_batchParams.size())
		return;
}

void COClassDlg::runClassification(classificationParams* params) {

	
	CString cstringHtmlFilename;
	CString cstringTextFilename;
	CString curFile;
	const char* filename = NULL;
	if(!params) {
	    AfxMessageBox("Classification Parameters NULL.", MB_ICONWARNING | MB_OK);
		return;
	}
	
	switch(params->dataType) {
		case 0: {
			try {
						
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<unsigned char> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 1: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<char> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 2: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<unsigned short> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 3: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<short> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 4: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<unsigned long> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 5: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<long> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 6: {
			try {
				
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<float> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}
			break;
		case 7: {
			try {
							
				cstringHtmlFilename = params->imageFileName.c_str(); 
				cstringHtmlFilename += ".html";
				cstringTextFilename = params->imageFileName.c_str();
				cstringTextFilename += ".txt";
				ImageClassifier<double> classifier(params->imageFileName.c_str(), params->imageWidth
												  ,params->imageHeight, params->numLayers, params->numClasses);
				classifier.classify();
				if(params->claOutput) 
					classifier.saveReportXML(params->claFileName.c_str(), (DataType)params->dataType);
				

				if(params->htmlOutput) 
					classifier.saveReportHTML((LPCTSTR)cstringHtmlFilename);
				

				if(params->textOutput) 
					classifier.saveReport((LPCTSTR)cstringTextFilename);
			}
			catch(GeneralException e) {
				AfxMessageBox(e.getMessage(), MB_ICONWARNING | MB_OK);
				enableControls(TRUE);
				m_runButton.SetWindowText("Run Classification");
				return;
			}
		}	
	}
}

void COClassDlg::clearJobList() {
	for(size_t i = 0; i < m_batchParams.size(); i++) {
		if(m_batchParams[i])
			delete m_batchParams[i];
	}

	m_batchParams.clear();
}

void COClassDlg::addJob(classificationParams* params) {
	m_batchParams.push_back(params);
}

void COClassDlg::loadParams(classificationParams* params) {
	if(!params)
		return;

	m_imageHeight = params->imageHeight;
	m_imageWidth = params->imageWidth;
	m_numLayers = params->numLayers;
	m_numClasses = params->numClasses;
	m_imageFileName = params->imageFileName.c_str();
	if(params->claOutput) {
		m_claFileName = params->claFileName.c_str();
		m_CLAOutput = params->claOutput;
		m_claFile.EnableWindow(TRUE);
		m_claButton.EnableWindow(TRUE);
		m_claEditStaticText.EnableWindow(TRUE);
	}	

	else {
		m_claFileName = "";
		m_claFile.EnableWindow(FALSE);
		m_claButton.EnableWindow(FALSE);
		m_claEditStaticText.EnableWindow(FALSE);
	}

	m_htmlOutput = params->htmlOutput;
	m_textOutput = params->textOutput;
	m_curDataType = params->dataType;
	m_dataType.SetCurSel(m_curDataType);
	UpdateData(FALSE);
}

classificationParams COClassDlg::getCurrentConfig() {
	updateAndCheckInput();
	return(m_curConfig);
	
}

void COClassDlg::editJob(size_t index, classificationParams* newParams) {
	if(!newParams) {
		AfxMessageBox("Assigning null job params", MB_ICONWARNING | MB_OK);
		return;
	}
	if(index > m_batchParams.size()) {
		AfxMessageBox("Job param index out of bounds", MB_ICONWARNING | MB_OK);
		return;
	}

	if(m_batchParams[index]) 
		delete m_batchParams[index];

	m_batchParams[index] = newParams;
}
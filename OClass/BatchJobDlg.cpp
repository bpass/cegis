
#include "stdafx.h"
#include "BatchJobDlg.h"
#include "oclassDlg.h"
#include ".\batchjobdlg.h"
#include "tinyxml.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BatchJobDlg::BatchJobDlg(CWnd* parent) :
CDialog(BatchJobDlg::IDD, parent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDI_USGS);
}

void BatchJobDlg::DoDataExchange(CDataExchange* dx) {
	CDialog::DoDataExchange(dx);

	DDX_Control(dx, IDC_JOBLIST, m_jobList);
	DDX_Control(dx, IDADDCURRENT, m_addCurrentButton);
	DDX_Control(dx, IDREMOVE, m_removeButton);
	DDX_Control(dx, IDSAVE, m_saveButton);
	DDX_Control(dx, IDLOAD, m_loadButton);
	DDX_Control(dx, IDBATCHCLOSE, m_closeButton);
	DDX_Control(dx, IDBATCHRUN, m_runButton);
	DDX_Control(dx, IDCLEAR, m_clearButton);
	DDX_Control(dx, IDC_BATCH_PROGRESS, m_progBar);
	DDX_Control(dx, IDC_EDIT_JOB, m_editCheckBox);
	DDX_Control(dx, IDC_BATCH_TEXT, m_progBarStatic);
	DDX_Text(dx, IDC_BATCH_TEXT, m_progBarText);
	DDX_Check(dx, IDC_EDIT_JOB, m_editChecked);
}

BEGIN_MESSAGE_MAP(BatchJobDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDADDCURRENT, onAddCurrentButton)
	ON_BN_CLICKED(IDREMOVE, onRemoveButton)
	ON_BN_CLICKED(IDSAVE, onSaveButton)
	ON_BN_CLICKED(IDLOAD, onLoadButton)
	ON_BN_CLICKED(IDBATCHRUN, onRunButton)
	ON_BN_CLICKED(IDBATCHCLOSE, onCloseButton)
	ON_BN_CLICKED(IDCLEAR, OnBnClickedClear)
	ON_LBN_SELCHANGE(IDC_JOBLIST, OnLbnSelchangeJoblist)
	ON_BN_CLICKED(IDC_EDIT_JOB, OnBnClickedEditJob)
END_MESSAGE_MAP()

BOOL BatchJobDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//*******************************************************************
	//* Create new tool tips object and add tips for each control
	//*******************************************************************
	m_toolTips.Create(this);
	m_toolTips.AddTool(&m_jobList, "The classification job queue.");
	m_toolTips.AddTool(&m_addCurrentButton, "Add the current configuration to the queue, or commit changes to the current selection.");
	m_toolTips.AddTool(&m_removeButton, "Remove the current selection from the queue.");
	m_toolTips.AddTool(&m_saveButton, "Save the current job queue to a file.");
	m_toolTips.AddTool(&m_loadButton, "Load a job queue from a file.");
	m_toolTips.AddTool(&m_runButton, "Run all of the jobs currently in the queue.");
	m_toolTips.AddTool(&m_closeButton, "Close the batch dialog.");
	m_toolTips.AddTool(&m_clearButton, "Clear the job queue.");
	m_toolTips.AddTool(&m_editCheckBox, "Turn on job editing.");

	return TRUE;
}

//overloaded to enable tool tips
BOOL BatchJobDlg::PreTranslateMessage(MSG* pMsg) {
	 
	switch(pMsg->message) {
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE:

		m_toolTips.RelayEvent(pMsg);
	}

return CDialog::PreTranslateMessage(pMsg);
}
void BatchJobDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	CDialog::OnSysCommand(nID, lParam);
}

void BatchJobDlg::OnPaint() {
	CDialog::OnPaint();
}

HCURSOR BatchJobDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void BatchJobDlg::onAddCurrentButton() {

	//get a pointer to the main OClass window
	COClassDlg* parent = ((COClassDlg*)GetParent());
	classificationParams* newConfig = NULL;
	UpdateData();

	//if we are not in edit mode 
	//call parent's updateParams which
	//grabs the current configuration in
	//the main window and adds it to the parent's job queue.
	//refreshJobList checks this queue each time it is called
	//and populates the list box with its values.
	if(!m_editChecked) 
		parent->updateParams();
	

	//else if we are editing an allready existing job
	//commit the current configuration to the currently
	//selected job in the list.
	else {
		newConfig = new classificationParams;
		*newConfig = parent->getCurrentConfig();
		parent->editJob(m_jobList.GetCurSel(), newConfig);
	}

	refreshJobList();
}

void BatchJobDlg::onRemoveButton() {
int selIndex = m_jobList.GetCurSel();

//if there is something valid selected in the listbox
//call the parent's deleteJob() function to remove
//it from the queue
if(selIndex != LB_ERR)
	((COClassDlg*)GetParent())->deleteJob((size_t)selIndex);

refreshJobList();

}

void BatchJobDlg::onSaveButton() {
	if(m_jobList.GetCount() == 0) {
		AfxMessageBox("Job queue is empty!", MB_ICONWARNING | MB_OK);
		return;
	}

	CString outputFileName;

	CFileDialog dlg(FALSE, "*.bjb", NULL, OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
					"Batch Queue Files (*.bqf)|*.bqf||");

	if(dlg.DoModal() == IDOK) {
		outputFileName = dlg.GetPathName();
		saveJobList((LPCSTR)outputFileName);
	}

}

void BatchJobDlg::saveJobList(const char* filename) {
	std::vector<classificationParams*> jobList = ((COClassDlg*)GetParent())->getJobList();
	
	if(jobList.empty()) {
		AfxMessageBox("Job queue is empty!", MB_ICONWARNING | MB_OK);
		return;
	}

	if(!filename) {
		AfxMessageBox("Job Queue Filename NULL", MB_ICONWARNING | MB_OK);
		return;
	}

	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", NULL, "yes");
	TiXmlElement rootE("JobQueue");
	rootE.SetAttribute("NumJobs", jobList.size());
	TiXmlElement* jobE = NULL; //each <job> tag
	TiXmlElement* fileE = NULL; //each <imageFile> tag 
	TiXmlElement* imageParamsE = NULL; //each <imageParams> tag
	TiXmlElement* outputParamsE = NULL; //each <outputParams> tag
	TiXmlElement* claFileE = NULL; //each <claFile> tag

	//for each job in the queue
	for(size_t i = 0; i < jobList.size(); i++) {
		
		if(jobE)
			delete jobE;
		if(fileE)
			delete fileE;
		if(imageParamsE)
			delete imageParamsE;
		if(outputParamsE)
			delete outputParamsE;
		if(claFileE)
			delete claFileE;

		//create all tags for current job and set all needed attributes.
		jobE = new TiXmlElement("Job");
		fileE = new TiXmlElement("ImageFile");
		fileE->InsertEndChild(TiXmlText(jobList[i]->imageFileName.c_str()));

		imageParamsE = new TiXmlElement("ImageParams");
		imageParamsE->SetAttribute("Width", jobList[i]->imageWidth);
		imageParamsE->SetAttribute("Height", jobList[i]->imageHeight);
		imageParamsE->SetAttribute("NumLayers", jobList[i]->numLayers);
		imageParamsE->SetAttribute("DataType", jobList[i]->dataType);

		outputParamsE = new TiXmlElement("OutputParams");
		outputParamsE->SetAttribute("NumClasses", jobList[i]->numClasses);
		
		if(jobList[i]->claOutput)
			outputParamsE->SetAttribute("cla", "Y");
		else
			outputParamsE->SetAttribute("cla", "N");
		
		if(jobList[i]->htmlOutput)
			outputParamsE->SetAttribute("html", "Y");
		else
			outputParamsE->SetAttribute("html", "N");
		
		if(jobList[i]->textOutput)
			outputParamsE->SetAttribute("text", "Y");
		else
			outputParamsE->SetAttribute("text", "N");

		claFileE = new TiXmlElement("CLAFile");
		if(!jobList[i]->claFileName.empty())
			claFileE->InsertEndChild(TiXmlText(jobList[i]->claFileName.c_str()));
		else
			claFileE->InsertEndChild(TiXmlText("Not Specified"));

		//add all of the tags under <job>
		jobE->InsertEndChild(*fileE);
		jobE->InsertEndChild(*imageParamsE);
		jobE->InsertEndChild(*outputParamsE);
		jobE->InsertEndChild(*claFileE);

		//add current <job> tag to root <jobQueue> tag
		rootE.InsertEndChild(*jobE);
	}
	
	doc.InsertEndChild(decl);
	doc.InsertEndChild(rootE);
	doc.SaveFile(filename);

	if(jobE)
		delete jobE;
	if(fileE)
		delete fileE;
	if(imageParamsE)
		delete imageParamsE;
	if(outputParamsE)
		delete outputParamsE;
	if(claFileE)
		delete claFileE;
}

void BatchJobDlg::onLoadButton() {
	CString inputFile;
	CFileDialog dlg(TRUE, "*.bqf", NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, 
					"Batch Queue Files (*.bqf)|*.bqf||");
	int answer = 0;
	bool asked = false;
	if(m_jobList.GetCount() != 0) {
		 answer = AfxMessageBox("Are you sure you want to load a new job queue?", MB_YESNO | MB_ICONWARNING);
		 asked = true;
	}
	
	if((asked && answer == IDYES) || !asked) {
		if(dlg.DoModal() == IDOK) {
			inputFile = dlg.GetPathName();
			loadJobList((LPCSTR)inputFile);
		}
	}


}

void BatchJobDlg::loadJobList(const char* filename) {

	//get pointer to the parent window.
	COClassDlg* parent = ((COClassDlg*)GetParent());
	int numJobs = 0;
	int tempNum = 0;
	classificationParams* curParams = NULL;
	if(!filename) {
		AfxMessageBox("Filename null on load!", MB_ICONWARNING | MB_OK);
		return;
	}

	TiXmlDocument doc;
	if(!doc.LoadFile(filename)) {
		AfxMessageBox("Error loading BQF file", MB_ICONWARNING | MB_OK);
		return;
	}

	//clear the job queue
	parent->clearJobList();

	//<jobQueue> tag
	TiXmlElement* rootE = doc.FirstChild("JobQueue")->ToElement();
	TiXmlNode* jobN = NULL; //each <job> tag
	TiXmlElement* imageParamsE = NULL; //each <imageParams> tag
	TiXmlElement* outputParamsE = NULL; //each <outputParams> tag
	TiXmlElement* claFileE = NULL; //each <claFile> tag
	TiXmlElement* imageFileE = NULL; //each <imageFile> tag
	TiXmlHandle handle(&doc);

	if(!rootE) {
		AfxMessageBox("Missing  or malformed <JobQueue> tag", MB_ICONWARNING | MB_OK);
		return;
	}
	
	//get the number of jobs from <jobQueue>
	rootE->Attribute("NumJobs", &numJobs);
	jobN = rootE->FirstChild("Job");

	//for each job in the list
	for(size_t i = 0; i < numJobs; i++) {
		curParams = new classificationParams;
		
		//****************************************************************
		//* Attempt to acquire all tags for current job
		//****************************************************************

		if(!jobN) {
			AfxMessageBox("Missing or malformed <Job> tag", MB_ICONWARNING | MB_OK);
			return;
		}
		
		imageParamsE = jobN->FirstChild("ImageParams")->ToElement();
		if(!imageParamsE) {
			AfxMessageBox("Missing <ImageParams> tag", MB_ICONWARNING | MB_OK);
			return;
		}

		outputParamsE = jobN->FirstChild("OutputParams")->ToElement();
		if(!outputParamsE) {
			AfxMessageBox("Missing <OutputParams> tag", MB_ICONWARNING | MB_OK);
			return;
		}

		claFileE = jobN->FirstChild("CLAFile")->ToElement();
		if(!claFileE) {
			AfxMessageBox("Missing <CLAFile> tag", MB_ICONWARNING | MB_OK);
			return;
		}

		imageFileE = jobN->FirstChild("ImageFile")->ToElement();
		if(!imageFileE) {
			AfxMessageBox("Missing <ImageFile> tag", MB_ICONWARNING | MB_OK);
			return;
		}

		//pull needed parameters out of each element
		curParams->imageFileName = imageFileE->FirstChild()->Value();
		curParams->claFileName = claFileE->FirstChild()->Value();
		imageParamsE->Attribute("NumLayers", (int*)&curParams->numLayers);
		imageParamsE->Attribute("Width", (int*)&curParams->imageWidth);
		imageParamsE->Attribute("Height", (int*)&curParams->imageHeight);
		imageParamsE->Attribute("DataType", (int*)&curParams->dataType);
		
		//check for the type of output this job is generating

		if(!strcmp(outputParamsE->Attribute("cla"), "Y"))
			curParams->claOutput = true;
		else
			curParams->claOutput = false;

		if(!strcmp(outputParamsE->Attribute("html"), "Y"))
			curParams->htmlOutput = true;
		else
			curParams->htmlOutput = false;

		if(!strcmp(outputParamsE->Attribute("text"), "Y"))
			curParams->textOutput = true;
		else
			curParams->textOutput = false;

		outputParamsE->Attribute("NumClasses", (int*)&curParams->numClasses);
		jobN = jobN->NextSibling();
		parent->addJob(curParams);
	}
		
	refreshJobList();

		
}
void BatchJobDlg::onRunButton() {

	//get the job list vector from the parent.
	std::vector<classificationParams*> jobList = ((COClassDlg*)GetParent())->getJobList();
	if(jobList.size() == 0) {
		AfxMessageBox("There are no jobs in the queue!", MB_ICONWARNING | MB_OK);
		return;
	}

	//set up the progress bar
	m_progBar.SetRange32(0, jobList.size());
	m_progBar.SetStep(1);
	m_progBar.ShowWindow(SW_SHOW);
	m_progBarStatic.ShowWindow(SW_SHOW);
	m_progBar.StepIt();
	
	UpdateWindow();
	
	//disable all controls
	enableControls(FALSE);

	//process each job in the list
	for(size_t i = 0; i < jobList.size(); i++) {

		//update progress bar
		m_progBarText = "Processing ";
		m_progBarText += jobList[i]->imageFileName.c_str();
		UpdateData(FALSE);
		UpdateWindow();

		//run the next job
		((COClassDlg*)GetParent())->runClassification(jobList[i]);
		m_progBar.StepIt();
		UpdateWindow();
	}
	m_progBar.SetPos(jobList.size());
	AfxMessageBox("Classification Complete!",MB_ICONINFORMATION | MB_OK);
	m_progBar.ShowWindow(SW_HIDE);
	m_progBarStatic.ShowWindow(SW_HIDE);
	m_progBar.SetPos(0);
	enableControls(TRUE);
}

void BatchJobDlg::onCloseButton() {

	this->ShowWindow(SW_HIDE);
}

void BatchJobDlg::refreshJobList() {

	//get the current job list and clear the listbox
	std::vector<classificationParams*> jobList = ((COClassDlg*)GetParent())->getJobList();
	m_jobList.ResetContent();

	FILE* testFile = NULL; //used to check if the image file listed exists or not.
	
	//for each job in the list
	for(size_t i = 0; i < jobList.size(); i++) {
		CString curJobString;
		
		if(jobList[i]) {

			//build the string to appear in the listbox for this job
			char number[10] = {'\0'};
			curJobString += "File: ";
			curJobString += jobList[i]->imageFileName.c_str();
			curJobString += "    | # Classes: ";
			sprintf(number, "%d", jobList[i]->numClasses);
			curJobString += number;

			//try to open the image file to see if it's there.
			testFile = fopen(jobList[i]->imageFileName.c_str(), "r");

			//if image file does not exists, add an indication to the job string.
			if(!testFile) 
				curJobString += "    *****BAD FILE*****";
			
			else {
				fclose(testFile);
				testFile = NULL;
			}
			
			m_jobList.AddString(curJobString);
		}
	}

	//reset the proper horizontal extent for the list box 
	//because we loaded a new list of strings.
	setListBoxHExtent();
}


void BatchJobDlg::OnBnClickedClear()
{
	int answer = 0;
	bool asked = false;

	//ask if they really want to clear the list.
	if(m_jobList.GetCount() != 0) {
		answer = AfxMessageBox("Are you sure you want to clear the job queue?", MB_YESNO | MB_ICONWARNING);
		asked = true;
	}
	
	//if yes, clear it.
	if((asked && answer == IDYES) || !asked) {
		((COClassDlg*)GetParent())->clearJobList();
		refreshJobList();
	}
	
}

void BatchJobDlg::enableControls(BOOL enable) {

	//get a pointer to the parent and enable or disable it's controls
	((COClassDlg*)GetParent())->enableControls(enable);

	//set the enable state for each control in the batch dialog
	m_addCurrentButton.EnableWindow(enable);
	m_removeButton.EnableWindow(enable);
	m_clearButton.EnableWindow(enable);
	m_closeButton.EnableWindow(enable);
	m_saveButton.EnableWindow(enable);
	m_loadButton.EnableWindow(enable);
	m_runButton.EnableWindow(enable);
	m_editCheckBox.EnableWindow(enable);
	UpdateWindow();
}

void BatchJobDlg::OnLbnSelchangeJoblist()
{
	UpdateData();

	//if we are in edit mode, load the parameters for the currently
	//selected job in the main OClass window.
	if(m_editChecked) {
		int curIndex = m_jobList.GetCurSel();
		COClassDlg* parent = (COClassDlg*)GetParent();
		parent->loadParams(parent->getJobList()[curIndex]);
	}	
}

void BatchJobDlg::OnBnClickedEditJob()
{
	UpdateData();

	//if we are checking "edit"
	if(m_editChecked) {
		int curSel = m_jobList.GetCurSel();

		//change the "add current" button text to reflect 
		//new functionality while in edit mode.
		m_addCurrentButton.SetWindowText("Commit Changes");

		//get a pointer to the parent window and 
		//if there is a selection allready made load those parameters.
		COClassDlg* parent = (COClassDlg*)GetParent();
		if(curSel != LB_ERR)
			parent->loadParams(parent->getJobList()[curSel]);
	}
	else
		m_addCurrentButton.SetWindowText("Add Current");
}

void BatchJobDlg::setListBoxHExtent() {
	size_t longest = 0;
	CString curString;

	//calculate which string in the listbox is the longest.
	for(size_t i = 0; i < m_jobList.GetCount(); i++) {
		m_jobList.GetText(i, curString);
		if(curString.GetLength() > longest)
			longest = curString.GetLength();
	}

	//the formula here was arrived at by trial and error,
	//it's not perfect but it works.
	m_jobList.SetHorizontalExtent(longest*5 + longest/2);
}

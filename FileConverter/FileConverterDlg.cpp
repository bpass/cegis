/* Programmer: Ryan Stelzleni
 * Date: 6-3-04
 *
 * Purpose: This file implements a gui intended to make it easy to convert
 *          some automatically generated files into a more useful format.
 */

/******************************************
* TODO: Make this file a lot more modular *
******************************************/


// FileConverterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileConverter.h"
#include "FileConverterDlg.h"
#include "OffsetDialog.h"
#include "FileParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileConverterDlg dialog

CFileConverterDlg::CFileConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileConverterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileConverterDlg)
	m_szFromFileOne = _T("");
	m_szToFileOne = _T("");
	m_szFromFileTwo = _T("");
	m_szToFileTwo = _T("");
	m_szFromFileThree = _T("");
	m_szToFileThree = _T("");
   m_szFromFileOneFull = _T("");
   m_szFromFileTwoFull = _T("");
   m_szFromFileThreeFull = _T("");
   m_szToFileOneFull = _T("");
   m_szToFileTwoFull = _T("");
   m_szToFileThreeFull = _T("");
   m_fOffsetOne = 0.0f;
   m_fOffsetTwo = 0.0f;
   m_fOffsetThree = 0.0f;


	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileConverterDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szFromFileOne);
	DDX_Text(pDX, IDC_EDIT2, m_szToFileOne);
	DDX_Text(pDX, IDC_EDIT3, m_szFromFileTwo);
	DDX_Text(pDX, IDC_EDIT4, m_szToFileTwo);
	DDX_Text(pDX, IDC_EDIT5, m_szFromFileThree);
	DDX_Text(pDX, IDC_EDIT6, m_szToFileThree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileConverterDlg, CDialog)
	//{{AFX_MSG_MAP(CFileConverterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FromFileOne, OnFromFileOne)
	ON_BN_CLICKED(IDC_FromFileTwo, OnFromFileTwo)
	ON_BN_CLICKED(IDC_FromFileThree, OnFromFileThree)
	ON_BN_CLICKED(IDC_ToFileOne, OnToFileOne)
	ON_BN_CLICKED(IDC_ToFileThree, OnToFileThree)
	ON_BN_CLICKED(IDC_ToFileTwo, OnToFileTwo)
	ON_BN_CLICKED(IDC_ExitButton, OnExitButton)
	ON_BN_CLICKED(IDOK, OnConvertButton)
	ON_BN_CLICKED(IDC_ClearButton, OnClearButton)
	ON_BN_CLICKED(IDC_ConvertButton, OnConvertButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileConverterDlg message handlers

BOOL CFileConverterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileConverterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileConverterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



/*
 * This function presents the user with an open file dialog and prompts for
 * an offset if the file has a .csv extension.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full file name and offset internal variables have been
 *                 set.
 */
void CFileConverterDlg::OnFromFileOne() 
{
   UpdateData(TRUE);

   CFileDialog ldFile(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
              "All (*.*)|*.*|Dat Files (*.dat)|*.dat|CSV Files (*.csv)|*.csv||");

   // Show the File Open dialog and capture the result
  if(ldFile.DoModal() == IDOK)
  {
    // Get the file name selected
    m_szFromFileOne = ldFile.GetFileName();
    m_szFromFileOneFull = ldFile.GetPathName();

     // If the file is a csv file then prompt for an offset.
    if( ldFile.GetFileExt() == "csv" || ldFile.GetFileExt() == "CSV" )
    {
       UpdateData(FALSE);

       COffsetDialog dlgbox(&m_fOffsetOne);
       if(dlgbox.DoModal() != IDOK)
       {
          m_szFromFileOne = "";
          m_szFromFileOneFull = "";
          MessageBox("A *.csv file has to have an offset entered",
                     "Need An Offset", MB_ICONINFORMATION);
       }
       else
       {
          m_szFromFileOne.Format("%s Offset: %.2f", ldFile.GetFileName(), 
                                    m_fOffsetOne);
       }
    }
  }

   UpdateData(FALSE);
}

/*
 * This function presents the user with an open file dialog and prompts for
 * an offset if the file has a .csv extension.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full file name and offset internal variables have been
 *                 set.
 */
void CFileConverterDlg::OnFromFileTwo() 
{   
   UpdateData(TRUE);

   CFileDialog ldFile(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
              "All (*.*)|*.*|Dat Files (*.dat)|*.dat|CSV Files (*.csv)|*.csv||");

   // Show the File Open dialog and capture the result
  if(ldFile.DoModal() == IDOK)
  {
    // Get the file name selected
    m_szFromFileTwo = ldFile.GetFileName();
    m_szFromFileTwoFull = ldFile.GetPathName();

     // If the file is a csv file then prompt for an offset.
    if( ldFile.GetFileExt() == "csv" || ldFile.GetFileExt() == "CSV" )
    {
       UpdateData(FALSE);

       COffsetDialog dlgbox(&m_fOffsetTwo);
       if(dlgbox.DoModal() != IDOK)
       {
          m_szFromFileTwo = "";
          m_szFromFileTwoFull = "";
          MessageBox("A *.csv file has to have an offset entered",
                     "Need An Offset", MB_ICONINFORMATION);
       }
       else
       {
          m_szFromFileTwo.Format("%s Offset: %.2f", ldFile.GetFileName(), 
                                    m_fOffsetTwo);
       }
    }
  }

   UpdateData(FALSE);
}



/*
 * This function presents the user with an open file dialog and prompts for
 * an offset if the file has a .csv extension.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full file name and offset internal variables have been
 *                 set.
 */
void CFileConverterDlg::OnFromFileThree() 
{   
   UpdateData(TRUE);

   CFileDialog ldFile(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
              "All (*.*)|*.*|Dat Files (*.dat)|*.dat|CSV Files (*.csv)|*.csv||");

   // Show the File Open dialog and capture the result
   if(ldFile.DoModal() == IDOK)
   {
     // Get the file name selected
     m_szFromFileThree = ldFile.GetFileName();
     m_szFromFileThreeFull = ldFile.GetPathName();

     // If the file is a csv file then prompt for an offset.
     if( ldFile.GetFileExt() == "csv" || ldFile.GetFileExt() == "CSV" )
     {
       UpdateData(FALSE);

       COffsetDialog dlgbox(&m_fOffsetThree);

       if(dlgbox.DoModal() != IDOK)
       {
          m_szFromFileThree = "";
          m_szFromFileThreeFull = "";
          MessageBox("A *.csv file has to have an offset entered",
                     "Need An Offset", MB_ICONINFORMATION);
       }
       else
       {
          m_szFromFileThree.Format("%s Offset: %.2f", ldFile.GetFileName(), 
                                    m_fOffsetThree);
       }

     }
   }


   UpdateData(FALSE);
}



/*
 * This function presents the user with a save file dialog.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full save file name has been set
 */
void CFileConverterDlg::OnToFileOne() 
{   
   UpdateData(TRUE);

   CFileDialog ldFile(FALSE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
              "All (*.*)|*.*||");

   // Show the File Open dialog and capture the result
  if(ldFile.DoModal() == IDOK)
  {
    // Get the file name selected
    m_szToFileOne = ldFile.GetFileName();
    m_szToFileOneFull = ldFile.GetPathName();
  }

   UpdateData(FALSE);
}


/*
 * This function presents the user with a save file dialog.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full save file name has been set
 */
void CFileConverterDlg::OnToFileThree() 
{   
   UpdateData(TRUE);

   CFileDialog ldFile(FALSE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
              "All (*.*)|*.*||");

   // Show the File Open dialog and capture the result
  if(ldFile.DoModal() == IDOK)
  {
    // Get the file name selected
    m_szToFileThree = ldFile.GetFileName();
    m_szToFileThreeFull = ldFile.GetPathName();
  }

   UpdateData(FALSE);
}


/*
 * This function presents the user with a save file dialog.
 * Post Condition: The selected file name is displayed in the gui and the 
 *                 Full save file name has been set
 */
void CFileConverterDlg::OnToFileTwo() 
{   
   UpdateData(TRUE);

   CFileDialog ldFile(FALSE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
              "All (*.*)|*.*||");

   // Show the File Open dialog and capture the result
  if(ldFile.DoModal() == IDOK)
  {
    // Get the file name selected
    m_szToFileTwo = ldFile.GetFileName();
    m_szToFileTwoFull = ldFile.GetPathName();
  }

   UpdateData(FALSE);
}


/* This function simply closes the program */
void CFileConverterDlg::OnExitButton() 
{
   PostQuitMessage(0);
}


/*
 * This function does the meat of the work for the application.  First it
 * checks to see if all from file names have an associated to file name,
 * and vice verse, and prompts the user if there is a problem.  Next it
 * checks to see if any two destination files are the same and once again
 * prompts the user for correctins.  I there are no problems in this 
 * section then it attempts to parse each file name that has been entered.
 * after each attempted parse the an output message is updated and at the
 * end of the function the output message is presented in a messagebox.
 * Post Condition: The requested files have either been created or a
 *                 a message box has been shown to let the user know that
 *                 the parsing failed.
 */
void CFileConverterDlg::OnConvertButton() 
{
   bool isOK = true;

   UpdateData(TRUE);

   if( !m_szFromFileOne.IsEmpty() && m_szToFileOne.IsEmpty() ||
       !m_szFromFileTwo.IsEmpty() && m_szToFileTwo.IsEmpty() ||
       !m_szFromFileThree.IsEmpty() && m_szToFileThree.IsEmpty() )
   {
      MessageBox("Please enter a destination file for each source file",
                 "Need More Information", MB_ICONEXCLAMATION);
      isOK = false;
   }
   else if( m_szFromFileOne.IsEmpty() && !m_szToFileOne.IsEmpty() ||
            m_szFromFileTwo.IsEmpty() && !m_szToFileTwo.IsEmpty() ||
            m_szFromFileThree.IsEmpty() && !m_szToFileThree.IsEmpty() )
   {
      MessageBox("Please enter a source file for each destination file",
                 "Need More Information", MB_ICONEXCLAMATION);
      isOK = false;
   }
   else if( (!m_szToFileOne.IsEmpty() &&
             (m_szToFileOne == m_szToFileTwo || m_szToFileOne == m_szToFileThree))
            || (!m_szToFileTwo.IsEmpty() && m_szToFileTwo == m_szToFileThree) )
   {
      MessageBox("Two of your destination files are identical\n"
                 "Please change one so that you don't lose data",
                 "Possible Data Loss", MB_ICONEXCLAMATION);
      isOK = false;
   }
 
   if(isOK)
   {
      int ResOne = -1, ResTwo = -1, ResThree = -1;
      CString Message;

      if( !m_szFromFileOne.IsEmpty() )
      {
         FileParser parser( m_szFromFileOneFull.GetBuffer(0),
                           m_szToFileOneFull.GetBuffer(0), m_fOffsetOne );
         ResOne = parser.WasSuccess();
         switch(ResOne)
         {
         case FileParser::GOOD:
            Message = m_szFromFileOne + " converted successfully\n\n";
            break;
         case FileParser::BADFILENAME:
            Message = "Error opening file: " + m_szFromFileOne + "\n\n";
            break;
         case FileParser::UNRECOGNIZED:
            Message = m_szFromFileOne + " is not a recognizable type of file\n\n";
         }
      }
      
      if( !m_szFromFileTwo.IsEmpty() )
      {
         FileParser parser( m_szFromFileTwoFull.GetBuffer(0),
                           m_szToFileTwoFull.GetBuffer(0), m_fOffsetTwo );
         ResTwo = parser.WasSuccess();
         switch(ResTwo)
         {
         case FileParser::GOOD:
            Message += m_szFromFileTwo + " converted successfully\n\n";
            break;
         case FileParser::BADFILENAME:
            Message += "Error opening file: " + m_szFromFileTwo + "\n\n";
            break;
         case FileParser::UNRECOGNIZED:
            Message += m_szFromFileTwo + " is not a recognizable type of file\n\n";
         }
      }
      if( !m_szFromFileThree.IsEmpty() )
      {
         FileParser parser( m_szFromFileThreeFull.GetBuffer(0),
                           m_szToFileThreeFull.GetBuffer(0), m_fOffsetThree );
         ResThree = parser.WasSuccess();
         switch(ResThree)
         {
         case FileParser::GOOD:
            Message += m_szFromFileThree + " converted successfully\n\n";
            break;
         case FileParser::BADFILENAME:
            Message += "Error opening file: " + m_szFromFileThree + "\n\n";
            break;
         case FileParser::UNRECOGNIZED:
            Message += m_szFromFileThree + " is not a recognizable type of file\n\n";
         }
      }

      if( (ResOne == -1 || ResOne == FileParser::GOOD) &&
          (ResTwo == -1 || ResTwo == FileParser::GOOD) &&
          (ResThree == -1 || ResThree == FileParser::GOOD) )
      {
         MessageBox(Message, "Success", MB_ICONINFORMATION);
      }
      else
      {
         MessageBox(Message, "Conversion Error", MB_ICONEXCLAMATION);
      }
   }
}


/* This function clears all fields that the user could have populated
 * and emptys the gui.
 */
void CFileConverterDlg::OnClearButton() 
{
   m_szFromFileOne = "";
   m_szFromFileTwo = "";
   m_szFromFileThree = "";
   m_szToFileOne = "";
   m_szToFileThree = "";
   m_szToFileTwo = "";
   m_szToFileOneFull = "";
   m_szToFileTwoFull = "";
   m_szToFileThreeFull = "";
   m_szFromFileOneFull = "";
   m_szFromFileTwoFull  = "";
   m_szFromFileThreeFull = "";
   m_fOffsetOne = 0.0f;
   m_fOffsetTwo = 0.0f;
   m_fOffsetThree = 0.0f;

   UpdateData(FALSE);
}

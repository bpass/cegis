// OffsetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FileConverter.h"
#include "OffsetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COffsetDialog dialog


COffsetDialog::COffsetDialog(double *Offset, CWnd* pParent /*=NULL*/)
	: CDialog(COffsetDialog::IDD, pParent), pOffset(Offset)
{
	//{{AFX_DATA_INIT(COffsetDialog)
	m_Offset = 0.0f;
	//}}AFX_DATA_INIT
}


void COffsetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COffsetDialog)
	DDX_Text(pDX, IDC_EDIT1, m_Offset);
	DDV_MinMaxFloat(pDX, m_Offset, -1.e+006f, 1.e+006f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COffsetDialog, CDialog)
	//{{AFX_MSG_MAP(COffsetDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COffsetDialog message handlers

void COffsetDialog::OnOK() 
{
   UpdateData(TRUE);
   *pOffset = m_Offset;
	CDialog::OnOK();
}

void COffsetDialog::OnCancel() 
{
	CDialog::OnCancel();
}

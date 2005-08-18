// TardemPpg.cpp : Implementation of the CTardemPropPage property page class.

#include "stdafx.h"
#include "tardem.h"
#include "TardemPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTardemPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CTardemPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CTardemPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTardemPropPage, "TARDEM.TardemPropPage.1",
	0xc0296106, 0xace2, 0x11d3, 0xad, 0x9, 0, 0x50, 0x4, 0x8d, 0x1d, 0x2c)


/////////////////////////////////////////////////////////////////////////////
// CTardemPropPage::CTardemPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CTardemPropPage

BOOL CTardemPropPage::CTardemPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TARDEM_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CTardemPropPage::CTardemPropPage - Constructor

CTardemPropPage::CTardemPropPage() :
	COlePropertyPage(IDD, IDS_TARDEM_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CTardemPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CTardemPropPage::DoDataExchange - Moves data between page and properties

void CTardemPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CTardemPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CTardemPropPage message handlers

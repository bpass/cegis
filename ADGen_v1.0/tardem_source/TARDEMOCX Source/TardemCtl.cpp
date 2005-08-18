// TardemCtl.cpp : Implementation of the CTardemCtrl ActiveX Control class.

#include "stdafx.h"
#include "tardem.h"
#include "TardemCtl.h"
#include "TardemPpg.h"
#include "lsm.h"
#include "StreamBurn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTardemCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CTardemCtrl, COleControl)
	//{{AFX_MSG_MAP(CTardemCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CTardemCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CTardemCtrl)
	DISP_PROPERTY_EX(CTardemCtrl, "demfile", GetDemfile, SetDemfile, VT_BSTR)
	DISP_FUNCTION(CTardemCtrl, "Flood", Flood, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CTardemCtrl, "D8", D8, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CTardemCtrl, "Dinf", Dinf, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CTardemCtrl, "AreaDinf", AreaDinf, VT_BSTR, VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CTardemCtrl, "Gridnet", Gridnet, VT_BSTR, VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CTardemCtrl, "Arclinks", Arclinks, VT_BSTR, VTS_I2 VTS_R8)
	DISP_FUNCTION(CTardemCtrl, "Arcstreams", Arcstreams, VT_BSTR, VTS_I2 VTS_R8)
	DISP_FUNCTION(CTardemCtrl, "SubbasinSetup", SubbasinSetup, VT_BSTR, VTS_I2)
	DISP_FUNCTION(CTardemCtrl, "MaxAreaCoords", MaxAreaCoords, VT_BOOL, VTS_PI4 VTS_PI4)
	DISP_FUNCTION(CTardemCtrl, "BurnDown", BurnDown, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CTardemCtrl, "AreaD8", AreaD8, VT_BSTR, VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CTardemCtrl, "OneOutlet", OneOutlet, VT_I4, VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CTardemCtrl, "ValidateOutlet", ValidateOutlet, VT_I2, VTS_I4 VTS_I4)
	DISP_FUNCTION(CTardemCtrl, "Outlets", Outlets, VT_I2, VTS_I2 VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_BOOL)
	DISP_FUNCTION(CTardemCtrl, "Netsetup", Netsetup, VT_BSTR, VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4 VTS_I4 VTS_BOOL)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CTardemCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CTardemCtrl, COleControl)
	//{{AFX_EVENT_MAP(CTardemCtrl)
	EVENT_CUSTOM("FloodProgress", FireFloodProgress, VTS_I2)
	EVENT_CUSTOM("BurnEvent", FireBurnEvent, VTS_I2  VTS_I2  VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CTardemCtrl, 1)
	PROPPAGEID(CTardemPropPage::guid)
END_PROPPAGEIDS(CTardemCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTardemCtrl, "TARDEM.TardemCtrl.1",
	0xc0296105, 0xace2, 0x11d3, 0xad, 0x9, 0, 0x50, 0x4, 0x8d, 0x1d, 0x2c)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CTardemCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DTardem =
		{ 0xc0296103, 0xace2, 0x11d3, { 0xad, 0x9, 0, 0x50, 0x4, 0x8d, 0x1d, 0x2c } };
const IID BASED_CODE IID_DTardemEvents =
		{ 0xc0296104, 0xace2, 0x11d3, { 0xad, 0x9, 0, 0x50, 0x4, 0x8d, 0x1d, 0x2c } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwTardemOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTardemCtrl, IDS_TARDEM, _dwTardemOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::CTardemCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CTardemCtrl

BOOL CTardemCtrl::CTardemCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_TARDEM,
			IDB_TARDEM,
			afxRegInsertable | afxRegApartmentThreading,
			_dwTardemOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::CTardemCtrl - Constructor

CTardemCtrl::CTardemCtrl()
{
	InitializeIIDs(&IID_DTardem, &IID_DTardemEvents);

	// TODO: Initialize your control's instance data here.
	 fillfile = NULL;
	 slpd8file = NULL;
	 slpdinffile = NULL;
	 d8file = NULL;
	 dinffile = NULL;
	 aread8file = NULL;
	 areadinffile = NULL;
	 tlenfile = NULL;
	 plenfile = NULL;
	 coordfile = NULL;
	 ordfile = NULL;
	 wshedfile = NULL;
	 treefile = NULL;
	 gordfile = NULL;
	 srcfile = NULL;
	 tmpfile = NULL;
	 e00file = NULL;
	 fdrfile = NULL;
	 fdrnfile = NULL;
	 shpfile = NULL;

	 setCtrl(this);
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::~CTardemCtrl - Destructor

CTardemCtrl::~CTardemCtrl()
{
	// TODO: Cleanup your control's instance data here.
	delete fillfile;
	delete slpd8file;
	delete slpdinffile;
	delete d8file;
	delete dinffile;
	delete aread8file;
	delete areadinffile;
	delete tlenfile;
	delete plenfile;
	delete coordfile;
	delete ordfile;
	delete wshedfile;
	delete treefile;
	delete gordfile;
	delete srcfile;
	delete tmpfile;
	delete e00file;
	delete fdrfile;
	delete fdrnfile;
	delete shpfile;
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::OnDraw - Drawing function

void CTardemCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	CPictureHolder m_pic;
	if(m_pic.CreateFromBitmap(IDB_TARDEM))
		m_pic.Render(pdc,rcBounds,rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::DoPropExchange - Persistence support

void CTardemCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CTardemCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags |= windowlessActivate;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::OnResetState - Reset control to default state

void CTardemCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl::AboutBox - Display an "About" box to the user

void CTardemCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_TARDEM);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl message handlers

void CTardemCtrl::InitFilenames(LPCTSTR file)
{
	demfile = CString(file);
	int len = demfile.GetLength();

	fillfile = new char[len+4];			//fel
	d8file = new char[len+2];			//p
	aread8file = new char[len+4];		//ad8
	slpd8file = new char[len+4];		//sd8
	dinffile = new char[len+4];			//ang
	areadinffile = new char[len+4];		//sca
	slpdinffile = new char[len+4];		//slp
	tlenfile = new char[len+5];			//tlen
	plenfile = new char[len+5];			//plen
	coordfile = new char[len+9];		//coord
	ordfile = new char[len+4];			//ord
	wshedfile = new char[len+2];		//w
	treefile = new char[len+8];			//tree
	gordfile = new char[len+5];			//gord
	srcfile = new char[len+4];			//src
	tmpfile = new char[len+4];			//tmp
	e00file = new char[len+4];			//e00
	fdrfile = new char[len+4];			//fdr
	fdrnfile = new char[len+5];			//fdrn
	shpfile = new char[len+6];			//l1 shp

    
//	char *ext=strrchr(file,'.');
//	int nmain=len-strlen(ext);
	char * tfile = new char[len+9];
	strcpy(tfile,file);
	int nmain=nameadd(fillfile,tfile,"fel");
	nmain=nameadd(d8file,tfile,"p");
	nmain=nameadd(aread8file,tfile,"ad8");
	nmain=nameadd(slpd8file,tfile,"sd8");
	nmain=nameadd(dinffile,tfile,"ang");
	nmain=nameadd(areadinffile,tfile,"sca");
	nmain=nameadd(slpdinffile,tfile,"slp");
	nmain=nameadd(tlenfile,tfile,"tlen");
	nmain=nameadd(plenfile,tfile,"plen");
	nmain=nameadd(ordfile,tfile,"ord");
	nmain=nameadd(wshedfile,tfile,"w");
	nmain=nameadd(gordfile,tfile,"gord");
	nmain=nameadd(srcfile,tfile,"src");
	nmain=nameadd(tmpfile,tfile,"tmp");
	nmain=nameadd(e00file,tfile,"e00");
	nmain=nameadd(fdrfile,tfile,"fdr");
	nmain=nameadd(fdrnfile,tfile,"fdrn");
	/*  text  files*/
	strcpy(treefile,"");
	strncat(treefile,file,nmain);
    strcat(treefile,"tree.dat");
	strcpy(coordfile,"");
	strncat(coordfile,file,nmain);
    strcat(coordfile,"coord.dat");
	strcpy(e00file,"");
	strncat(e00file,file,nmain);
    strcat(e00file,".e00");
	strcpy(shpfile,"");
	strncat(shpfile,file,nmain);
	strcat(shpfile,"l1.shp");

/*
	strcpy(fillfile,"");
	strncat(fillfile,file,nmain);
	strcat(fillfile,"fel");
	strcat(fillfile,ext);
	strcpy(d8file,"");
	strncat(d8file,file,nmain);
	strcat(d8file,"p");
	strcat(d8file,ext);
	strcpy(aread8file,"");
	strncat(aread8file,file,nmain);
	strcat(aread8file,"ad8");
	strcat(aread8file,ext);
	strcpy(slpd8file,"");
	strncat(slpd8file,file,nmain);
	strcat(slpd8file,"sd8");
	strcat(slpd8file,ext);
	strcpy(dinffile,"");
	strncat(dinffile,file,nmain);
	strcat(dinffile,"ang");
	strcat(dinffile,ext);
	strcpy(areadinffile,"");
	strncat(areadinffile,file,nmain);
	strcat(areadinffile,"sca");
	strcat(areadinffile,ext);
	strcpy(slpdinffile,"");
	strncat(slpdinffile,file,nmain);
	strcat(slpdinffile,"slp");
	strcat(slpdinffile,ext);
	strcpy(tlenfile,"");
	strncat(tlenfile,file,nmain);
	strcat(tlenfile,"tlen");
	strcat(tlenfile,ext);
	strcpy(plenfile,"");
	strncat(plenfile,file,nmain);
	strcat(plenfile,"plen");
	strcat(plenfile,ext);
	strcpy(coordfile,"");
	strncat(coordfile,file,nmain);
	strcat(coordfile,"coord");
	strcat(coordfile,ext);
	strcpy(gordfile,"");
	strncat(gordfile,file,nmain);
	strcat(gordfile,"gord");
	strcat(gordfile,ext);
	strcpy(tmpfile,"");
	strncat(tmpfile,file,nmain);
	strcat(tmpfile,"tmp");
	strcat(tmpfile,ext);
	strcpy(e00file,"");
	strncat(e00file,file,nmain);
	strcat(e00file,"e00");
	strcat(e00file,ext);
	strcpy(srcfile,"");
	strncat(srcfile,file,nmain);
	strcat(srcfile,"src");
	strcat(srcfile,ext);
	strcpy(treefile,"");
	strncat(treefile,file,nmain);
	strcat(treefile,"tree");
	strcat(treefile,ext);
	strcpy(wshedfile,"");
	strncat(wshedfile,file,nmain);
	strcat(wshedfile,"w");
	strcat(wshedfile,ext);
	strcpy(ordfile,"");
	strncat(ordfile,file,nmain);
	strcat(ordfile,"ord");
	strcat(ordfile,ext); */
}

void CTardemCtrl::ClearNames()
{
	demfile.Empty();
	delete fillfile;
	delete slpd8file;
	delete slpdinffile;
	delete d8file;
	delete dinffile;
	delete aread8file;
	delete areadinffile;
	delete tlenfile;
	delete plenfile;
	delete coordfile;
	delete ordfile;
	delete wshedfile;
	delete treefile;
	delete gordfile;
	delete srcfile;
	delete tmpfile;
	delete e00file;
	delete shpfile;
}

BSTR CTardemCtrl::GetDemfile() 
{
	// TODO: Add your property handler here
	
	return demfile.AllocSysString();
}

void CTardemCtrl::SetDemfile(LPCTSTR lpszNewValue) 
{
	// TODO: Add your property handler here
	ClearNames();
	InitFilenames(lpszNewValue);
	SetModifiedFlag();
}

BSTR CTardemCtrl::Flood(short usefdrfile) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	char* tmp = new char[demfile.GetLength() + 1];
	strcpy(tmp,demfile);

	int retval = flood(tmp,fillfile,fdrfile, usefdrfile, fdrnfile);
	delete tmp;
	
	if(retval)
		strResult.Empty();
	else
		strResult =  fillfile;

	return strResult.AllocSysString();
}

BSTR CTardemCtrl::D8( short usefdrfile ) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int retval = setdird8(fillfile,d8file,slpd8file, fdrnfile, usefdrfile );
	if(retval)
		strResult.Empty();
	else
		strResult =  d8file;
	return strResult.AllocSysString();
}

BSTR CTardemCtrl::Dinf(short usefdrfile) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int retval = setdir(fillfile,dinffile,slpdinffile, fdrnfile, usefdrfile);
	if(retval)
		strResult.Empty();
	else
		strResult =  dinffile;
	return strResult.AllocSysString();
}

BSTR CTardemCtrl::AreaDinf(long outletX, long outletY, LPCTSTR weightFile, BOOL contamination) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int doall,usew,contam;
	if(strcmp(weightFile,"")==0)
		usew = 0;
	else
		usew = 1;
	if(outletX == 0 && outletY == 0)
		doall = 1;
	else
		doall = 0;
	if(contamination)
		contam = 1;
	else
		contam = 0;

	char* tmp = new char[strlen(weightFile)+1];
	strcpy(tmp,weightFile);

	int retval = area(dinffile,areadinffile,outletX,outletY,doall,tmp,usew,contam);
	delete tmp;

	if(retval)
		strResult.Empty();
	else
		strResult =  areadinffile;

	return strResult.AllocSysString();
}

BSTR CTardemCtrl::Gridnet(long outletX, long outletY, LPCTSTR maskFile, long threshold) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int doall,mask;
	if(strcmp(maskFile,"")==0) {
		threshold = 0;
		mask = 0;
	}
	else
		mask = 1;
	if(outletX == 0 && outletY == 0)
		doall = 0;
	else
		doall = 1;

	char* tmp = new char[strlen(maskFile)+1];
	strcpy(tmp,maskFile);

	int retval = gridnet(d8file,plenfile,tlenfile,gordfile,tmp,outletX,outletY,mask,
		doall,threshold);
	delete tmp;

	if(retval)
		strResult.Empty();
	else
		strResult = plenfile;

	return strResult.AllocSysString();
}

BSTR CTardemCtrl::Arclinks(short link, double minArea) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int ilink = link;
	float amin = minArea;
	int err=0;
	ARCLINKS(coordfile,strlen(coordfile),treefile,strlen(treefile),e00file,strlen(e00file),&ilink,&amin,&err);
//	arclinks(coordfile,treefile,e00file,&ilink,&amin,&err);
	if(err)
		strResult.Empty();
	else
		strResult = e00file;

	return strResult.AllocSysString();
}

BSTR CTardemCtrl::Arcstreams(short link, double minArea) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int ilink = link;
	float amin = minArea;
	int err=0;
	ARCSTREAMS(coordfile,strlen(coordfile),treefile,strlen(treefile),e00file,strlen(e00file),&ilink,&amin,&err);
//	arcstreams(coordfile,treefile,e00file,&ilink,&amin,&err);
	if(err)
		strResult.Empty();
	else
		strResult = e00file;
	return strResult.AllocSysString();
}

BSTR CTardemCtrl::SubbasinSetup(short order) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int retval = subbasinsetup(d8file,wshedfile,treefile,coordfile,shpfile,order);
	if(retval)
		strResult.Empty();
	else
		strResult = wshedfile;

	return strResult.AllocSysString();
}

void CTardemCtrl::FloodEvent(short percentage)
{
	FireFloodProgress(percentage);
}

void CTardemCtrl::BurnEvent( short percent, short event_id, char * event_descrip )
{
	FireBurnEvent( percent, event_id, event_descrip );
}

BOOL CTardemCtrl::MaxAreaCoords(long FAR* x, long FAR* y) 
{
	// TODO: Add your dispatch handler code here
	MaxAreaD8Coords(aread8file,x,y);

	if(*x == 0 && *y == 0)
		return FALSE;

	return TRUE;
}

BOOL CTardemCtrl::BurnDown(LPCTSTR flowDirectionFile, LPCTSTR outFile) 
{
	// TODO: Add your dispatch handler code here
	StreamBurn BurnBabyBurn( this );
	bool success = BurnBabyBurn.burn( flowDirectionFile, demfile, outFile );
	if( success )
		demfile = outFile;
	return success;
}


BSTR CTardemCtrl::AreaD8(long outletX, long outletY, LPCTSTR weightFile, BOOL contamination) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	int doall,usew,contam;
	if(strcmp(weightFile,"")==0)
		usew = 0;
	else
		usew = 1;
	if(outletX == 0 && outletY == 0)
		doall = 1;
	else
		doall = 0;
	if(contamination)
		contam = 1;
	else
		contam = 0;

	char* tmp = new char[strlen(weightFile)+1];
	strcpy(tmp,weightFile);

	int retval = aread8(d8file,aread8file,outletX,outletY,doall,tmp,usew,contam);
	delete tmp;

	if(retval)
		strResult.Empty();
	else
		strResult =  aread8file;

	return strResult.AllocSysString();
}


long CTardemCtrl::OneOutlet(short index, BOOL xtrueyfalse) 
{
	// TODO: Add your dispatch handler code here
	long retval;
    if(index >= XoutDeque.size())retval=0.;
	else
	{
		if(xtrueyfalse)retval=XoutDeque[index];
		else retval=YoutDeque[index];
	}
	return retval;
}


short CTardemCtrl::ValidateOutlet(long x, long y) 
{
	// TODO: Add your dispatch handler code here
    if(validateoutlet(x,y,aread8file,srcfile) == 0)return 0;
	else return(1);
}

short CTardemCtrl::Outlets(short nmax, long method, float p1, float p2, float p3, 
						   float p4, short PotentialSourceThreshold, BOOL Contamination) 
{
	// TODO: Add your dispatch handler code here
	// TODO: Add your dispatch handler code here
	// Initialize Deque lists that will be used
	short i;
	int ipar,err,contcheck;
	ipar=method;
	maxno=nmax;
	SourceThreshold=PotentialSourceThreshold;
	XoutDeque.clear();
	YoutDeque.clear();
	DiffDeque.clear();
	xout=new long[nmax];
	yout=new long[nmax];
	diffout=new short[nmax];
	double x=0.;
	double y=0.;
	float p[4];
        p[0]=p1;
        p[1]=p2;
        p[2]=p3;
        p[3]=p4;
	if(Contamination)
		contcheck = 1;
	else
		contcheck = 0;
	err=source(aread8file, slpd8file, plenfile, d8file, 
		    srcfile,  fillfile,  gordfile,  areadinffile,
		    fdrnfile, ipar, p,  x,   y, contcheck);
  
	if(err == 0)
	{
		for(i=0; i<nout; i++)
		{
			XoutDeque.push_back(xout[i]);
			YoutDeque.push_back(yout[i]);
			DiffDeque.push_back(diffout[i]);
		}
	}
/*	delete[] xout;
	delete[] yout;
	delete[] diffout;   */
	if(err != 0)return(0);
	else return (short)XoutDeque.size();
}

BSTR CTardemCtrl::Netsetup(long method, float p1, float p2, float p3, float p4, 
						   long OutletX, long OutletY, BOOL contamination) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	char* tmp = new char[demfile.GetLength() + 1];
	strcpy(tmp,demfile);
	int contcheck;
	if(contamination)
		contcheck = 1;
	else
		contcheck = 0;

	int retval = netsetup(tmp,method,p1,p2,p3,p4,OutletX,OutletY,contcheck);
	if(retval)
		strResult.Empty();
	else
		strResult = srcfile;
	return strResult.AllocSysString();
}

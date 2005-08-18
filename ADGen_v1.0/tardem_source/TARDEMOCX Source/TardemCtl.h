#if !defined(AFX_TARDEMCTL_H__C0296113_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_)
#define AFX_TARDEMCTL_H__C0296113_ACE2_11D3_AD09_0050048D1D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TardemCtl.h : Declaration of the CTardemCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CTardemCtrl : See TardemCtl.cpp for implementation.
#include <deque>
extern long *xout;
extern long *yout;
extern short *diffout;
extern short nout,maxno;
extern short SourceThreshold;

class CTardemCtrl : public COleControl
{
	DECLARE_DYNCREATE(CTardemCtrl)

// Constructor
public:
	CTardemCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTardemCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ClearNames();
	void InitFilenames(LPCTSTR file);
	~CTardemCtrl();

	DECLARE_OLECREATE_EX(CTardemCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CTardemCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CTardemCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CTardemCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CTardemCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CTardemCtrl)
	afx_msg BSTR GetDemfile();
	afx_msg void SetDemfile(LPCTSTR lpszNewValue);
	afx_msg BSTR Flood(short flag);
	afx_msg BSTR D8( short flag );
	afx_msg BSTR Dinf(short flag);
	afx_msg BSTR AreaDinf(long outletX, long outletY, LPCTSTR weightFile, BOOL contamination);
	afx_msg BSTR Gridnet(long outletX, long outletY, LPCTSTR maskFile, long threshold);
	afx_msg BSTR Arclinks(short link, double minArea);
	afx_msg BSTR Arcstreams(short link, double minArea);
	afx_msg BSTR SubbasinSetup(short order);
	afx_msg BOOL MaxAreaCoords(long FAR* x, long FAR* y);
	afx_msg BOOL BurnDown(LPCTSTR flowDirectionFile, LPCTSTR outFile);
	afx_msg BSTR AreaD8(long OutletX, long OutletY, LPCTSTR weightFile, BOOL contamination);
	afx_msg long OneOutlet(short index, BOOL xtrueyfalse);
	afx_msg short ValidateOutlet(long x, long y);
	afx_msg short Outlets(short nmax, long method, float p1, float p2, float p3, float p4, short PotentialSourceThreshold, BOOL Contamination);
	afx_msg BSTR Netsetup(long method, float p1, float p2, float p3, float p4, long OutletX, long OutletY, BOOL contamination);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CTardemCtrl)
	void FireFloodProgress(short percentage)
		{FireEvent(eventidFloodProgress,EVENT_PARAM(VTS_I2), percentage);}
	void FireBurnEvent(short percent, short event_id, LPCTSTR event_descrip)
		{FireEvent(eventidBurnEvent,EVENT_PARAM(VTS_I2  VTS_I2  VTS_BSTR), percent, event_id, event_descrip);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	void FloodEvent(short percentage);
	void BurnEvent( short percent, short event_id, char * event_descrip );
	enum {
	//{{AFX_DISP_ID(CTardemCtrl)
	dispidDemfile = 1L,
	dispidFlood = 2L,
	dispidD8 = 3L,
	dispidDinf = 4L,
	dispidAreaDinf = 5L,
	dispidGridnet = 6L,
	dispidArclinks = 7L,
	dispidArcstreams = 8L,
	dispidSubbasinSetup = 9L,
	dispidMaxAreaCoords = 10L,
	dispidBurnDown = 11L,
	dispidAreaD8 = 12L,
	dispidOneOutlet = 13L,
	dispidValidateOutlet = 14L,
	dispidOutlets = 15L,
	dispidNetsetup = 16L,
	eventidFloodProgress = 1L,
	eventidBurnEvent = 2L,
	//}}AFX_DISP_ID
	};

private:
	CString demfile;
	char* fillfile;
	char* slpd8file;
	char* slpdinffile;
	char* d8file;
	char* dinffile;
	char* aread8file;
	char* areadinffile;
	char* tlenfile;
	char* plenfile;
	char* coordfile;
	char* ordfile;
	char* wshedfile;
	char* treefile;
	char* gordfile;
	char* srcfile;
	char* tmpfile;
	char* e00file;
	char* fdrfile;
	char* fdrnfile;
	char* shpfile;
	std::deque<long> XoutDeque;
	std::deque<long> YoutDeque;
	std::deque<short> DiffDeque;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARDEMCTL_H__C0296113_ACE2_11D3_AD09_0050048D1D2C__INCLUDED)

// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "rlf.h"
#include "rlfdoc.h"

#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_STATUS_0, OnStatus0)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_I, OnStatusRest)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_F, OnStatusRest)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_GW, OnStatusRest)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MAX, OnStatusRest)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MIN, OnStatusRest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
		ID_SEPARATOR,
	ID_VUURVLIEG_VLIEGEN,
	ID_VUURVLIEG_RUSTEN,
	ID_VUURVLIEG_WIPJE,
	ID_VUURVLIEG_WAARDEN,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_I,
	ID_INDICATOR_F,
	ID_INDICATOR_GW,
	ID_INDICATOR_MAX,
	ID_INDICATOR_MIN
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;		// fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;		// fail to create
	}
	return 0;
}

void CMainFrame::OnStatus0()
{
	m_wndStatusBar.SetPaneText(0,"We vliegen tot we erbij neervallen!!",TRUE);
}

void CMainFrame::OnStatusRest(CCmdUI* pCmdUI)
{
	CRlfDoc* ad=(CRlfDoc*)GetActiveDocument();
	char dum[10];
	CString ss;
	
   pCmdUI->Enable(TRUE);
   switch(pCmdUI->m_nIndex) {
   	case 1: sprintf(dum,"%04ld",ad->m_dwLoops); break;
   	case 2: sprintf(dum,"%05d",ad->m_iHuidigFlashes); break;
   	case 3: sprintf(dum,"%06ld",(long)ad->m_dwGemWaarde/(ad->m_iYasBlok*ad->m_iXasBlok)); break;
   	case 4: sprintf(dum,"%07ld",ad->m_dwMaxWaarde); break;
   	case 5: sprintf(dum,"%07ld",(long)ad->m_dwMinWaarde); break;
   }
   ss=dum;
	m_wndStatusBar.SetPaneText(pCmdUI->m_nIndex,ss,TRUE);           
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



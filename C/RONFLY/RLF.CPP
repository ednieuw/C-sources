// rlf.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "rlf.h"

#include "mainfrm.h"
#include "rlfdoc.h"
#include "rlfview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRlfApp

BEGIN_MESSAGE_MAP(CRlfApp, CWinApp)
	//{{AFX_MSG_MAP(CRlfApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VUURVLIEG_VLIEGEN, OnVuurvliegVliegen)
	ON_UPDATE_COMMAND_UI(ID_VUURVLIEG_VLIEGEN, OnUpdateVuurvliegVliegen)
	ON_COMMAND(ID_VUURVLIEG_RUSTEN, OnVuurvliegRusten)
	ON_UPDATE_COMMAND_UI(ID_VUURVLIEG_RUSTEN, OnUpdateVuurvliegRusten)
	ON_UPDATE_COMMAND_UI(ID_VUURVLIEG_WIPJE, OnUpdateVuurvliegVliegen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRlfApp construction

CRlfApp::CRlfApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bFlashenTF=FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRlfApp object

CRlfApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CRlfApp initialization

BOOL CRlfApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();        // set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	m_nCmdShow=SW_SHOWMAXIMIZED;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	AddDocTemplate(new CSingleDocTemplate(IDR_MAINFRAME,
			RUNTIME_CLASS(CRlfDoc),
			RUNTIME_CLASS(CMainFrame),     // main SDI frame window
			RUNTIME_CLASS(CRlfView)));



	// simple command line parsing
	if (m_lpCmdLine[0] == '\0')
	{
		// create a new (empty) document
		OnFileNew();
	}
	else
	{
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}

	return TRUE;
}

BOOL CRlfApp::OnIdle( LONG lCount)
{
	BOOL bMore=CWinApp::OnIdle(lCount);
	if(bMore == FALSE) {
		if(m_bFlashenTF) {
			m_pMainWnd->SendMessage(WM_COMMAND,ID_VUURVLIEG_WIPJE,0);	
			bMore=TRUE;
		}
	}
	return bMore;
	
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CRlfApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRlfApp commands

void CRlfApp::OnVuurvliegVliegen()
{
	// TODO: Add your command handler code here
	m_bFlashenTF=TRUE;	
	m_pMainWnd->SendMessage(WM_COMMAND,ID_STATUS_0);
}

void CRlfApp::OnUpdateVuurvliegVliegen(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bFlashenTF == TRUE) pCmdUI->Enable(FALSE);	
	else pCmdUI->Enable(TRUE);	
}

void CRlfApp::OnVuurvliegRusten()
{
	// TODO: Add your command handler code here
	m_bFlashenTF=FALSE;	
	
}

void CRlfApp::OnUpdateVuurvliegRusten(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(m_bFlashenTF == TRUE) pCmdUI->Enable(TRUE);	
	else pCmdUI->Enable(FALSE);	
}


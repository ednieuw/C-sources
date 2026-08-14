// rlf.h : main header file for the RLF application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRlfApp:
// See rlf.cpp for the implementation of this class
//

class CRlfApp : public CWinApp
{
public:
	CRlfApp();

// Overrides
	virtual BOOL InitInstance();
	virtual BOOL OnIdle( LONG lCount);

// Implementation
	BOOL m_bFlashenTF;					// als TRUE alsmaar flashen

	//{{AFX_MSG(CRlfApp)
	afx_msg void OnAppAbout();
	afx_msg void OnVuurvliegVliegen();
	afx_msg void OnUpdateVuurvliegVliegen(CCmdUI* pCmdUI);
	afx_msg void OnVuurvliegRusten();
	afx_msg void OnUpdateVuurvliegRusten(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

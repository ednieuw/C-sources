// flashdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlashWrdDlg dialog

class CFlashWrdDlg : public CDialog
{
// Construction
public:
	CFlashWrdDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashWrdDlg)
	enum { IDD = IDD_VUURVLIEG_WAARDEN };
	float	m_fEnergiefact;
	int		m_iHoeErbijSW;
	int		m_iHoeFlashSW;
	int		m_iMaxInvloed;
	int		m_iBeginWaardeMax;
	float	m_fMoeheidfact;
	float	m_fAutoErbijfact;
	int		m_iAantFlashStart;
	int		m_iXasBlok;
	int		m_iYasBlok;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFlashWrdDlg)
	afx_msg void OnClickedReset();
	afx_msg void OnClickedDoorgan();
	afx_msg void OnRestartWaarden();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

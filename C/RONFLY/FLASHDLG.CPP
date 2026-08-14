// flashdlg.cpp : implementation file
//

#include "stdafx.h"
#include "rlf.h"
#include "flashdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlashWrdDlg dialog

CFlashWrdDlg::CFlashWrdDlg(CWnd* pParent/*=NULL*/)
	: CDialog(CFlashWrdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashWrdDlg)
	m_fEnergiefact = 0;
	m_iHoeErbijSW = -1;
	m_iHoeFlashSW = -1;
	m_iMaxInvloed = 0;
	m_iBeginWaardeMax = 0;
	m_fMoeheidfact = 0;
	m_fAutoErbijfact = 0;
	m_iAantFlashStart = 0;
	m_iXasBlok = 0;
	m_iYasBlok = 0;
	//}}AFX_DATA_INIT
}

void CFlashWrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashWrdDlg)
	DDX_Text(pDX, IDC_ENERGIE, m_fEnergiefact);
	DDV_MinMaxFloat(pDX, m_fEnergiefact, 0.1, 1.);
	DDX_Radio(pDX, IDC_HOEERBIJ_0, m_iHoeErbijSW);
	DDX_Radio(pDX, IDC_HOEFLASH_0, m_iHoeFlashSW);
	DDX_Text(pDX, IDC_INVLOED, m_iMaxInvloed);
	DDV_MinMaxInt(pDX, m_iMaxInvloed, 1, 50);
	DDX_Text(pDX, IDC_MAX_WAARDE, m_iBeginWaardeMax);
	DDV_MinMaxInt(pDX, m_iBeginWaardeMax, 0, 1000);
	DDX_Text(pDX, IDC_MOEHEID, m_fMoeheidfact);
	DDV_MinMaxFloat(pDX, m_fMoeheidfact, 1., 10.);
	DDX_Text(pDX, IDC_PER_ERBIJ, m_fAutoErbijfact);
	DDV_MinMaxFloat(pDX, m_fAutoErbijfact, 0., 1.);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_START_FLASH, m_iAantFlashStart);
	DDV_MinMaxInt(pDX, m_iAantFlashStart, 0, m_iYasBlok*m_iXasBlok);
	DDX_Text(pDX, IDC_XAS_BLOK, m_iXasBlok);
	DDV_MinMaxInt(pDX, m_iXasBlok, 1, 16000/m_iYasBlok);
	DDX_Text(pDX, IDC_YAS_BLOK, m_iYasBlok);
	DDV_MinMaxInt(pDX, m_iYasBlok, 1, 16000/m_iXasBlok);
}

BEGIN_MESSAGE_MAP(CFlashWrdDlg, CDialog)
	//{{AFX_MSG_MAP(CFlashWrdDlg)
	ON_BN_CLICKED(IDRESET, OnClickedReset)
	ON_BN_CLICKED(IDDOORGAAN, OnClickedDoorgan)
	ON_EN_CHANGE(IDC_YAS_BLOK, OnRestartWaarden) 
	ON_EN_CHANGE(IDC_XAS_BLOK, OnRestartWaarden) 
	ON_EN_CHANGE(IDC_START_FLASH, OnRestartWaarden) 
	ON_EN_CHANGE(IDC_MAX_WAARDE, OnRestartWaarden) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashWrdDlg message handlers

void CFlashWrdDlg::OnClickedReset()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);	
}

void CFlashWrdDlg::OnClickedDoorgan()
{
	// TODO: Add your control notification handler code here
		if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination\n");
		// the UpdateData routine will set focus to correct item
		return;
	}
	EndDialog(IDDOORGAAN);
}

void CFlashWrdDlg::OnRestartWaarden()
{
	// TODO: Add your control notification handler code here
	char str[5];
	int xb,yb,sf,mw;

	GetDlgItemText(IDC_YAS_BLOK,str,4);
	yb=atoi(str);
	GetDlgItemText(IDC_XAS_BLOK,str,4);
	xb=atoi(str);
	GetDlgItemText(IDC_START_FLASH,str,4);
	sf=atoi(str);
	GetDlgItemText(IDC_MAX_WAARDE,str,4);
	mw=atoi(str);
	CWnd* dum=GetDlgItem(IDDOORGAAN);
	if(xb != m_iXasBlok || yb != m_iYasBlok || 
		sf != m_iAantFlashStart || mw != m_iBeginWaardeMax) dum->EnableWindow(FALSE);
	else dum->EnableWindow(TRUE);
}

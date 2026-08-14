// rlfdoc.cpp : implementation of the CRlfDoc class
//

#include "stdafx.h"
#include "rlf.h"
#include "rlfdoc.h"
#include "mainfrm.h"
#include "flashdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRlfDoc

IMPLEMENT_DYNCREATE(CRlfDoc, CDocument)

BEGIN_MESSAGE_MAP(CRlfDoc, CDocument)
	//{{AFX_MSG_MAP(CRlfDoc)
	ON_COMMAND(ID_VUURVLIEG_WIPJE, OnVuurvliegWipje)
	ON_COMMAND(ID_VUURVLIEG_WAARDEN, OnVuurvliegWaarden)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRlfDoc construction/destruction

CRlfDoc::CRlfDoc()
{
	// TODO: add one-time construction code here
	m_iAantFlashStart =5;
	m_iBeginWaardeMax =999;
	m_iYasBlok			=40;
	m_iXasBlok			=60;
	m_dwTreshold		=1000;
	m_iMaxInvloed		=5;
	m_fEnergiefact		=1.0;
	m_fMoeheidfact		=2.0;
	m_fAutoErbijfact	=0.1;
	m_byHoeFlashSW		=0;
	m_byHoeErbijSW		=0;
	m_dwLoops         =0;
	m_dwGemWaarde		=0;
	m_iHuidigFlashes  =0;
	m_dwMaxWaarde     =0;
	m_dwMinWaarde     =0;
}

CRlfDoc::~CRlfDoc()
{
}

void CRlfDoc::DeleteContents()
{
 	m_dwaWaarden.RemoveAll();
 	m_dwaErbij.RemoveAll();
 	m_byaFlashTF.RemoveAll();
 	m_byaPrintST.RemoveAll();
}

void CRlfDoc::InitDocument() 
{
	m_dwaWaarden.SetSize(m_iYasBlok*m_iXasBlok);
	m_dwaErbij.SetSize(min(m_iYasBlok+1,m_iXasBlok+1)*min(m_iYasBlok+1,m_iXasBlok+1));
	m_byaFlashTF.SetSize(m_iYasBlok*m_iXasBlok);
	m_byaPrintST.SetSize(m_iYasBlok*m_iXasBlok);
	m_dwLoops         =0;
	m_dwGemWaarde		=0;
	m_iHuidigFlashes  =0;
	m_dwMaxWaarde     =0;
	m_dwMinWaarde     =0;
	InitFlash();
}

void CRlfDoc::InitFlash() 
{
	int x,y;
	
	for(x=0;x<m_iYasBlok*m_iXasBlok;x++) {
		m_byaPrintST[x]=1;
		m_byaFlashTF[x]=FALSE;
		m_dwaWaarden[x]=rand()%(m_iBeginWaardeMax);
	}
	for(x=0,y=0;x<m_iAantFlashStart;x++) {
		y=rand()%(m_iYasBlok*m_iXasBlok-1);
		m_byaPrintST[y]=6;
		m_dwaWaarden[y]=m_dwTreshold+1;
	}
	InitErbij();
}

void CRlfDoc::InitErbij() 
{
	int x,y;
	long a,ma;

	ma=m_iMaxInvloed*m_iMaxInvloed+m_iMaxInvloed*m_iMaxInvloed;
 	for(y=0;y<=m_iMaxInvloed;y++) {
 		for(x=0;x<=m_iMaxInvloed;x++) {
 			if(!y && !x) continue;
			a=(y*y)+(x*x);
			a=(long)((float)m_dwTreshold*((float)ma/(float)a/(float)ma)*m_fEnergiefact+0.5);
			m_dwaErbij[y*(m_iMaxInvloed+1)+x]=a;
 		}
	}
}

void CRlfDoc::Flashen()
{  
	int x,xx,y,yy,z;
	int xb,xe,yb,ye;

	m_dwLoops++;
	m_iHuidigFlashes=0;
	m_dwGemWaarde=0;
	for(x=0;x<m_iYasBlok*m_iXasBlok;x++) {
		if(m_byaPrintST[x] == 6) m_byaPrintST[x]=0;
		m_byaFlashTF[x]=FALSE;
		if((long)m_dwaWaarden[x] >= (long)m_dwTreshold && !m_byaPrintST[x]) {
			m_byaFlashTF[x]=TRUE;
			m_byaPrintST[x]=5;
			m_iHuidigFlashes++;
		} else {
			if(m_byaPrintST[x] > 0) m_byaPrintST[x]--;
			if((long)m_dwaWaarden[x] < (long)m_dwTreshold) {
				if(m_byHoeErbijSW == 1) {
					m_dwaWaarden[x]+=(long)((float)m_dwTreshold*m_fAutoErbijfact);
				} else if(m_byHoeErbijSW == 2) {
					m_dwaWaarden[x]+=(long)((float)(m_dwTreshold-(long)m_dwaWaarden[x])*m_fAutoErbijfact);
				}
			}
		}
	}
	if(m_iHuidigFlashes) {
	 	for(y=0;y<m_iYasBlok;y++) {
	 		for(x=0;x<m_iXasBlok;x++) {
 				if(m_byaFlashTF[y*m_iXasBlok+x]) {
 					yb=max(0,y-m_iMaxInvloed);
 					xb=max(0,x-m_iMaxInvloed);
 					ye=min(m_iYasBlok-1,y+m_iMaxInvloed);
 					xe=min(m_iXasBlok-1,x+m_iMaxInvloed);
			 		for(yy=yb;yy<=ye;yy++) {
 						for(xx=xb;xx<=xe;xx++) {
 							if(m_byHoeFlashSW == 0) {
	 							if(!m_byaFlashTF[yy*m_iXasBlok+xx]) {
							   	z=abs(y-yy)*(m_iMaxInvloed+1)+abs(x-xx);
									m_dwaWaarden[yy*m_iXasBlok+xx]+=m_dwaErbij[z];
								}
							} else if(m_byHoeFlashSW == 1) {
								if(yy != y && xx != x) {
							   	z=abs(y-yy)*(m_iMaxInvloed+1)+abs(x-xx);
									m_dwaWaarden[yy*m_iXasBlok+xx]+=m_dwaErbij[z];
								}
							} else if(m_byHoeFlashSW == 2) {
								if(yy != y && xx != x) {
							   	z=abs(y-yy)*(m_iMaxInvloed+1)+abs(x-xx);
									m_dwaWaarden[yy*m_iXasBlok+xx]+=m_dwaErbij[z]/(m_byaPrintST[yy*m_iXasBlok+xx]+1);
								}
							}
 						}
 					}
 				}
 			}
 		}
	}
	for(x=0;x<m_iYasBlok*m_iXasBlok;x++) {
		if(!m_iHuidigFlashes) m_dwaWaarden[x]+=10;
		if((long)m_dwaWaarden[x] > (long)m_dwMaxWaarde) 
			m_dwMaxWaarde=m_dwaWaarden[x];
		if(m_byaFlashTF[x]) {
			m_dwaWaarden[x]-=(long)((float)m_dwaWaarden[x]*m_fMoeheidfact);
			m_byaFlashTF[x]=FALSE;
		}
		if((long)m_dwaWaarden[x] < (long)m_dwMinWaarde) 
			m_dwMinWaarde=m_dwaWaarden[x];
		m_dwGemWaarde+=m_dwaWaarden[x];
	}
}

BOOL CRlfDoc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	::SendMessage(HWND_BROADCAST,WM_COMMAND,ID_VUURVLIEG_RUSTEN,0);
	InitDocument();
	return TRUE;
}

BOOL CRlfDoc::OnOpenDocument(const char* pszPathName)
{
	if(!CDocument::OnOpenDocument(pszPathName))
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VUURVLIEG_RUSTEN);
	m_dwaErbij.SetSize(min(m_iYasBlok+1,m_iXasBlok+1)*min(m_iYasBlok+1,m_iXasBlok+1));
	m_byaFlashTF.SetSize(m_iYasBlok*m_iXasBlok);
	InitErbij();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRlfDoc serialization

void CRlfDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << (WORD)m_iYasBlok;
		ar << (WORD)m_iXasBlok;
		ar << m_dwTreshold;
		ar << (WORD)m_iMaxInvloed;
		ar << m_fEnergiefact;
		ar << m_fMoeheidfact;
		ar << m_fAutoErbijfact;
		ar << m_byHoeFlashSW;
		ar << m_byHoeErbijSW;
		ar <<	m_dwLoops;
		ar <<	m_dwMaxWaarde;
		ar << m_dwMinWaarde;
		m_dwaWaarden.Serialize(ar);
		m_byaPrintST.Serialize(ar);
	}
	else
	{
		// TODO: add loading code here
		WORD w;
		ar >> w;
		m_iYasBlok=w;
		ar >> w;
		m_iXasBlok=w;
		ar >> m_dwTreshold;
		ar >> w;
		m_iMaxInvloed=w;
		ar >> m_fEnergiefact;
		ar >> m_fMoeheidfact;
		ar >> m_fAutoErbijfact;
		ar >> m_byHoeFlashSW;
		ar >> m_byHoeErbijSW;
		ar >>	m_dwLoops;
		ar >>	m_dwMaxWaarde;
		ar >> m_dwMinWaarde;
		m_dwaWaarden.Serialize(ar);
		m_byaPrintST.Serialize(ar);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CRlfDoc diagnostics

#ifdef _DEBUG
void CRlfDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRlfDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRlfDoc commands

void CRlfDoc::OnVuurvliegWipje()
{
	// TODO: Add your command handler code here
	Flashen();
   UpdateAllViews(NULL,1);
}


void CRlfDoc::OnVuurvliegWaarden()
{
	// TODO: Add your command handler code here 
/*	CColorDialog dlg;

	CHOOSECOLOR	
	dlg.DoModal();*/
	CFlashWrdDlg dlg;
	
	dlg.m_fEnergiefact=m_fEnergiefact;
	dlg.m_iHoeErbijSW=m_byHoeErbijSW;
	dlg.m_iHoeFlashSW=m_byHoeFlashSW;
	dlg.m_iMaxInvloed=m_iMaxInvloed;
	dlg.m_iBeginWaardeMax=m_iBeginWaardeMax;
	dlg.m_fMoeheidfact=m_fMoeheidfact;
	dlg.m_fAutoErbijfact=m_fAutoErbijfact;
	dlg.m_iAantFlashStart=m_iAantFlashStart;
	dlg.m_iXasBlok=m_iXasBlok;
	dlg.m_iYasBlok=m_iYasBlok;
	int ret=dlg.DoModal();
	CRlfApp* ap=(CRlfApp*)AfxGetApp();
	if(ap->m_bFlashenTF == TRUE) ap->m_pMainWnd->SendMessage(WM_COMMAND,ID_VUURVLIEG_VLIEGEN);
	if(ret == IDOK || ret == IDDOORGAAN) {
		m_fEnergiefact=dlg.m_fEnergiefact;
		m_byHoeErbijSW=dlg.m_iHoeErbijSW;
		m_byHoeFlashSW=dlg.m_iHoeFlashSW;
		m_iMaxInvloed=dlg.m_iMaxInvloed;
		m_fMoeheidfact=dlg.m_fMoeheidfact;
		m_fAutoErbijfact=dlg.m_fAutoErbijfact;
		if(ret == IDOK) {
			m_iYasBlok=dlg.m_iYasBlok;
			m_iXasBlok=dlg.m_iXasBlok;
			if(m_iYasBlok*m_iXasBlok > 16000) m_iYasBlok=16000/m_iXasBlok;
			m_iAantFlashStart=dlg.m_iAantFlashStart;
			m_iBeginWaardeMax=dlg.m_iBeginWaardeMax;
			ap->m_pMainWnd->SendMessage(WM_COMMAND,ID_FILE_NEW);
		}
	}
}

// rlfview.cpp : implementation of the CRlfView class
//

#include "stdafx.h"
#include "rlf.h"

#include "rlfdoc.h"
#include "mainfrm.h"
#include "rlfview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRlfView

IMPLEMENT_DYNCREATE(CRlfView, CView)

BEGIN_MESSAGE_MAP(CRlfView, CView)
	//{{AFX_MSG_MAP(CRlfView)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRlfView construction/destruction

CRlfView::CRlfView()
{
	// TODO: add construction code here
	m_bPrintJN=TRUE;
}                                           

CRlfView::~CRlfView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRlfView drawing

void CRlfView::OnDraw(CDC* pDC)
{
	CRlfDoc* pDoc = GetDocument();

	// TODO: add draw code here
   CRect rect;
   float xs,ys;
   int sobj,sobjv=NULL_BRUSH,x,y;
   BYTE byPrintST;

   GetClientRect(&rect);
   xs=(float)rect.right/pDoc->m_iXasBlok;
   ys=(float)rect.bottom/pDoc->m_iYasBlok;
   rect.bottom=0;
  	for(y=0;y<pDoc->m_iYasBlok;y++) {
  		rect.top=rect.bottom;
  		rect.bottom=(int)(ys*y)+(int)ys;
  		rect.right=0;
  		for(x=0;x<pDoc->m_iXasBlok;x++) {
	  		rect.left=rect.right;
  			rect.right=(int)(xs*x)+(int)xs;
			byPrintST=pDoc->m_byaPrintST[y*pDoc->m_iXasBlok+x];
  			if(byPrintST >= 5) sobj=WHITE_BRUSH;
  			else if(byPrintST == 4) sobj=LTGRAY_BRUSH;
  			else if(byPrintST == 3) sobj=GRAY_BRUSH;
  			else if(byPrintST == 2) sobj=DKGRAY_BRUSH;
  			else {
  				if(!m_bPrintJN && !byPrintST) continue;
  				sobj=BLACK_BRUSH;
  			}
  			if(sobj != sobjv) {
				pDC->SelectStockObject(sobj);
				sobjv=sobj;
			}
  			pDC->Rectangle(rect);
  		}
  	}
  	m_bPrintJN=TRUE;
}

BOOL CRlfView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.lpszClass == NULL);
	cs.lpszClass=AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,
												AfxGetApp()->LoadStandardCursor(IDC_ARROW),
												(HBRUSH)(COLOR_WINDOW+2),
												AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRlfView printing

BOOL CRlfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRlfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRlfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRlfView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	ASSERT(pSender != this);

	// invalidate the entire pane, erase background too
	if(!lHint) {
		m_bPrintJN=TRUE;
		Invalidate(TRUE);
	} else {
		m_bPrintJN=FALSE;
		Invalidate(FALSE);	
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRlfView diagnostics

#ifdef _DEBUG
void CRlfView::AssertValid() const
{
	CView::AssertValid();
}

void CRlfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRlfDoc* CRlfView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRlfDoc)));
	return (CRlfDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRlfView message handlers

void CRlfView::OnFilePrintPreview()
{
	// TODO: Add your command handler code here
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VUURVLIEG_RUSTEN);
	CView::OnFilePrintPreview();
}

void CRlfView::OnFilePrint()
{
	// TODO: Add your command handler code here
	AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND,ID_VUURVLIEG_RUSTEN);
	CView::OnFilePrint();
}

void CRlfView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	CRlfDoc* pDoc=GetDocument();
   CRect rect;
   float xs,ys;
   int x,y;

   GetClientRect(&rect);
   xs=(float)rect.right/pDoc->m_iXasBlok;
   ys=(float)rect.bottom/pDoc->m_iYasBlok;
	x=(int)((float)point.x/xs);
	y=(int)((float)point.y/ys);
	if(pDoc->m_dwaWaarden[y*pDoc->m_iXasBlok+x] > pDoc->m_dwTreshold) {
		pDoc->m_dwaWaarden[y*pDoc->m_iXasBlok+x]=rand()%(pDoc->m_iBeginWaardeMax);
		pDoc->m_byaPrintST[y*pDoc->m_iXasBlok+x]=1;
		pDoc->m_byaFlashTF[y*pDoc->m_iXasBlok+x]=FALSE;
	} else {
		pDoc->m_dwaWaarden[y*pDoc->m_iXasBlok+x]=pDoc->m_dwTreshold+1;
		pDoc->m_byaPrintST[y*pDoc->m_iXasBlok+x]=6;
		pDoc->m_byaFlashTF[y*pDoc->m_iXasBlok+x]=FALSE;
	}
	OnUpdate(NULL,1);
}


// rlfview.h : interface of the CRlfView class
//
/////////////////////////////////////////////////////////////////////////////

class CRlfView : public CView
{
protected: // create from serialization only
	CRlfView();
	DECLARE_DYNCREATE(CRlfView)

// Attributes
public:
	CRlfDoc* GetDocument();

// Operations
public:
	BOOL 	m_bPrintJN;							// FALSE allen de veranderde blokjes tekenen

// Implementation
public:
	virtual ~CRlfView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs); // overridden to create this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint=0, CObject* pHint=NULL);
// Generated message map functions
protected:
	//{{AFX_MSG(CRlfView)
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in rlfview.cpp
inline CRlfDoc* CRlfView::GetDocument()
   { return (CRlfDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

// rlfdoc.h : interface of the CRlfDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CRlfDoc : public CDocument
{
protected: // create from serialization only
	CRlfDoc();
	DECLARE_DYNCREATE(CRlfDoc)

// Attributes
public:                       
	int			m_iAantFlashStart;			// aantal flashes start
	int			m_iBeginWaardeMax;			// maximale beginwaarde voor rest
	int			m_iYasBlok;						// aantal yas blokken
	int			m_iXasBlok;						// aantal xas blokken
	DWORD			m_dwTreshold;					// getal waarboven geflasht word
	int			m_iMaxInvloed;					// aantal blokken die beinvloed worden door flash
	float			m_fEnergiefact;				// percentage van treshold die erbij komt 1.00=100%
	float			m_fMoeheidfact;				// na flash Waarde-=Waarde*Moeheid
	float			m_fAutoErbijfact;				// percentage van treshold die er auto bij komt 1.00=100%
	BYTE        m_byHoeFlashSW;            // 0=flashende kan niet geladen worden
   													// 1=iedereen kan geladen worden
   													// 2=iedereen kan geladen worden energie erbij afhankelijk van kleur
   BYTE			m_byHoeErbijSW;				// 0=niks erbij
   													// 1=percentage van treshold
   													// 2=percentage van vershil met treshold
	CDWordArray m_dwaWaarden;					// waarden van blokken
	CDWordArray m_dwaErbij;						// waarden die erbij komen na flash
	CByteArray	m_byaFlashTF;					// TRUE=hij flasht FALSE=hij flasht niet
	CByteArray	m_byaPrintST;					// 0=niet tekenen 1..5 tekennen in betreffende kleur
	DWORD			m_dwLoops;						// aantal doorlopen loops
	DWORD			m_dwGemWaarde;					// gemiddelde waarde
	int			m_iHuidigFlashes;				// huidigaantal flashes
	DWORD			m_dwMaxWaarde;					// maximale waarde
	DWORD			m_dwMinWaarde;					// minimale waarde
// Operations
public:
	virtual void DeleteContents();			// verwijderd inhoud van document
// Implementation
public:
	virtual ~CRlfDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
protected:
	void InitDocument();							// initialiseerd inhoud van document
	void InitFlash();								// initialiseerd beginwaarden array's
	void InitErbij();								// initialiseerd erbijwaarden array
	void Flashen();								// flashen
	virtual	BOOL	OnNewDocument();
	virtual 	BOOL 	OnOpenDocument(const char* pszPathName);

// Generated message map functions
protected:
	//{{AFX_MSG(CRlfDoc)
	afx_msg void OnVuurvliegWipje();
	afx_msg void OnVuurvliegWaarden();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

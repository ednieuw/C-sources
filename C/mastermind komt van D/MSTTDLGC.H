#if !defined(__msttdlgc_h)              // Sentry, use file only if it's not already included.
#define __msttdlgc_h

/*  Project mastermi
    EDRON
    Copyright © 1995. All Rights Reserved.

    SUBSYSTEM:    mastermi.exe Application
    FILE:         msttdlgc.h
    AUTHOR:       Ed N & Ron L


    OVERVIEW
    ========
    Class definition for mastermiTDLGClient (TDialog).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\static.h>
#include "mstrmapp.rh"                  // Definition of all resources.
#define MAXROW 17
#define MAXCOL  5						// Size mastermind array

//{{TDialog = mastermiTDLGClient}}
struct mastermiTDLGClientXfer {
//{{mastermiTDLGClientXFER_DATA}}
    char    Platform[ 255 ];
//{{mastermiTDLGClientXFER_DATA_END}}
};


class mastermiTDLGClient : public TDialog {
public:
	 mastermiTDLGClient (TWindow *parent, TResId resId = IDD_CLIENT, TModule *module = 0);
    virtual ~mastermiTDLGClient ();


//{{mastermiTDLGClientVIRTUAL_BEGIN}}
public:
    int start;
//{{mastermiTDLGClientVIRTUAL_END}}

//{{mastermiTDLGClientRSP_TBL_BEGIN}}
protected:
	void Controle_Invoer(char cijfer);
	void DrawBitmap(HDC hdc, HBITMAP hbitmap, short xStart, short yStart);
	void SetPaaltjes(int regel);
	void SetLampjes(int regel);
	char MMarray[MAXROW+1][MAXCOL+1];
	char ElementName[255];
 	char Paalnaam[12];
	char regel;
	char Solution;
	char pos_in_regel;
	char GekozenGetal[MAXCOL]; 	//hier staat het te zoeken getal
	char PosGoed[MAXROW],KleurGoed[MAXROW];
	char GameOver;
	short BMPsizex,BMPsizey;// groote van laatst geplaatste BMP
	void BNClickedNewGame ();
	void BNClickedClear ();
	void BNClickedBMP1 ();
	void BNClickedBMP2 ();
	void BNClickedBMP3 ();
	void BNClickedBMP4 ();
	void BNClickedBMP5 ();
	void BNClickedBMP6 ();
	void BNClickedBMP7 ();
	void BNClickedBMP8 ();
	void BNClickedBMP9 ();
    void BNClickedOK ();
    void BNClickedRedraw ();
    void BNClickedSolution ();
    void EvPaint ();
    void BNClickedPos1 ();
    void BNClickedPos2 ();
    void BNClickedPos3 ();
    void BNClickedPos4 ();
    void BNClickedPos5 ();
	void mastermiTDLGClient::CmPlayWave();
//{{mastermiTDLGClientRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(mastermiTDLGClient);

//{{mastermiTDLGClientXFER_DEF}}
protected:
    TStatic *Platform;

//{{mastermiTDLGClientXFER_DEF_END}}
};    //{{mastermiTDLGClient}}


#endif                                      // __msttdlgc_h sentry.

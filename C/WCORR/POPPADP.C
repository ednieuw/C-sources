/*---------------------------------------
   POPPADP.C -- Printing with Banding
  ---------------------------------------*/

#include "print.h"
#include "wcorr.h"
#include "COMMDLG.H"


BOOL PrintMyPage (HWND hwnd);

extern FILE *fpin;
extern short cyChar;
extern char szAppName[] ;
extern TEXTMETRIC    tm;

BOOL   bUserAbort ;

BOOL PrintMyPage (HWND hwnd)
{
static char szSpMsg [] = "Print: Printing" ;
BOOL        bError = FALSE ;
int         xPage, yPage ;
static      PRINTDLG pd;

 /* Set all structure members to zero. */
memset(&pd, 0, sizeof(PRINTDLG));
/* Initialize the necessary PRINTDLG structure members. */
pd.lStructSize = sizeof(PRINTDLG);
pd.hwndOwner = hwnd;
pd.Flags = PD_RETURNDC;

if(PrintDlg(&pd) !=0)
 {

  Escape (pd.hDC, STARTDOC, sizeof szSpMsg - 1, szSpMsg, NULL);
  GetTextMetrics (pd.hDC, &tm) ;
  SetMapMode(pd.hDC,MM_ANISOTROPIC) ;
  xPage = GetDeviceCaps (pd.hDC, HORZRES);
  yPage = GetDeviceCaps (pd.hDC, VERTRES);
  cxClient= xPage;//GetDeviceCaps(hdc,HORZRES);
  cyClient= yPage;//GetDeviceCaps(hdc,VERTRES);
  cyChar = (tm.tmHeight + tm.tmExternalLeading) ;
  cxChar = tm.tmAveCharWidth ;
  cxCaps = (tm.tmPitchAndFamily &1 ? 3: 2) * cxChar / 2;
  MaxX = xPage;
  MaxY = yPage;					// size of screen
  SetWindowExt   (pd.hDC, xPage, yPage);
  SetViewportExt (pd.hDC, xPage *1.0, yPage*0.6) ;
  SetViewportOrg (pd.hDC, xPage *0.0, yPage*0.0) ;
  SetROP2 (pd.hDC, nDrawingMode) ;
  Teken_assen(hwnd,pd.hDC);
  Teken_data(hwnd,pd.hDC);
  teken_graph(hwnd,pd.hDC,axcal.X_As,axcal.Y_As);
  Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
  Escape (pd.hDC, ENDDOC, 0, NULL, NULL) ;
  if (pd.hDevMode != NULL)
      GlobalFree(pd.hDevMode);
  if (pd.hDevNames != NULL)
      GlobalFree(pd.hDevNames);
  }
 else bUserAbort=TRUE;

 return bError || bUserAbort ;
}


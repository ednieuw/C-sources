/*---------------------------------------
   POPPADP.C -- Printing with Banding
  ---------------------------------------*/

#include "print.h"
#include "blast.h"
#include "COMMDLG.H"


BOOL PrintMyPage (HWND hwnd);

extern FILE 	*fpin;
extern short 	cyChar;
extern char 	szAppName[] ;
extern TEXTMETRIC tm;
BOOL   bUserAbort ;

BOOL PrintMyPage (HWND hwnd)
{
static char szSpMsg[] = "Print: Printing" ;
BOOL        bError    = FALSE ;
int         n, xPage, yPage ;
PRINTDLG pd;
HDC      hDCMeta;
//HANDLE   hMeta;

memset(&pd, 0, sizeof(PRINTDLG));  // Set all structure members to zero.
pd.lStructSize 	= sizeof(PRINTDLG);// Initialize PRINTDLG structure members.
pd.hwndOwner 	= hwnd;
pd.Flags 		= PD_RETURNDC;

if(PrintDlg(&pd) !=0)
 {
  Escape (pd.hDC, STARTDOC, sizeof szSpMsg - 1, szSpMsg, NULL);
  GetTextMetrics (pd.hDC, &tm) ;
  SetMapMode(pd.hDC,MM_ANISOTROPIC) ;
  xPage = GetDeviceCaps (pd.hDC, HORZRES)*0.95; // anders valt grafiek in CLB tekst
  yPage = GetDeviceCaps (pd.hDC, VERTRES);
  cxClient= xPage;  cyClient= yPage;
  cyChar = (tm.tmHeight + tm.tmExternalLeading) ;
  cxChar = tm.tmAveCharWidth ;
  cxCaps = (tm.tmPitchAndFamily &1 ? 3: 2) * cxChar / 2;
  MaxX = xPage;     MaxY = yPage;					// size of screen
  SetWindowExt   (pd.hDC, xPage, yPage);
  SetViewportExt (pd.hDC, xPage *1.0, yPage*1.0);
  SetViewportOrg (pd.hDC, xPage *0.0, yPage*0.0);
  SetROP2 (pd.hDC, nDrawingMode) ;
  axcal.X_Min			= 0;
  axcal.X_Max			= MaxX;
  axcal.Y_Min			= 0;
  axcal.Y_Max			= MaxY;
  axcal.Color			= BLUE;				// Als Color ==255 dan RGB values
  axcal.PointColor	= RED;				// Als Color ==255 dan RGB values
  axcal.Linethickness 	= 1;
  axcal.Pointthickness	= 2;
  axcal.X_Offset 		= 0.035 * ScreenX;	// OFFSET PAPIER
  axcal.Y_Offset 		= 0.0   * ScreenY;
  axcal.X_Length 		= 0.95   * ScreenX;  // length axis in units
  axcal.X_NoofTicks 	= 5;
  axcal.Y_Length 		= 1.0   * ScreenY; 	// length axis in units
  axcal.Y_NoofTicks 	= 7;
  n=0;
  while(test_selection[n]!=9999)  		// file reeds geladen met LOAD
	{
	if(read_wks(hwnd,(char*)label[test_selection[n]]))
	  {
		print_letter(pd.hDC);
   	Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;

		hDCMeta = CreateMetaFile( (LPSTR)"test.emf");//(char*)label[test_selection[n]]);
		SetMapMode (hDCMeta, MM_ANISOTROPIC) ;
//		GetClientRect (hwnd, &rect) ;
		SetWindowExt(hDCMeta,xPage, yPage);
		SetViewportExt (hDCMeta, xPage *1.0, yPage*1.0);
		SetViewportOrg (hDCMeta, xPage *0.0, yPage*0.0);
		SetROP2 (hDCMeta, nDrawingMode) ;
		print_letter(hDCMeta);
		CloseMetaFile(hDCMeta);
	  }
	 else break;
	 n++;
	}
 Escape (pd.hDC, ENDDOC, 0, NULL, NULL) ;
 if (pd.hDevMode != NULL)
	  GlobalFree(pd.hDevMode);
 if (pd.hDevNames != NULL)
	  GlobalFree(pd.hDevNames);
 }
else bUserAbort=TRUE;

return bError || bUserAbort ;
}



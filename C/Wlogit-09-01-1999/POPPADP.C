/*---------------------------------------
	POPPADP.C --
  ---------------------------------------*/
#include "wlogit.h"
#include <commdlg.h>
#include "print.h"

BOOL PrintMyPage (HWND hwnd);
HDC GetPrinterDC(void);

extern FILE *fpin;
extern short cyChar;
extern char szAppName[] ;
extern TEXTMETRIC    tm;
BOOL   bUserAbort ;


/*
HDC GetPrinterDC(void)
{
static char szPrinter[80];
char *szDevice, *szDriver, *szOutput;

GetProfileString ("windows", "device" ,"...",szPrinter, 80);
if((szDevice = strtok (szPrinter, ",")) &&
	(szDriver = strtok (NULL,",")) &&
	(szOutput = strtok (NULL, ",")))
return CreateDC(szDriver,szDevice,szOutput, NULL);

return 0;
}
*/

BOOL PrintMyPage (HWND hwnd)
{
char        inputline[255];
short       filetrue;
static char szSpMsg [] = "Logit print job" ;
BOOL        bError = FALSE ;
BOOL   		bUserAbort ;
int         regel,xPage, yPage ;
float			Breedte_grafiek=0.5,Hoogte_grafiek=0.3;
PRINTDLG 	pd;
LPDEVNAMES lpDevNames;
LPSTR      lpszDriverName;
LPSTR      lpszDeviceName;
LPSTR      lpszPortName;

memset(&pd, 0, sizeof(PRINTDLG));
pd.lStructSize = sizeof(PRINTDLG);
pd.hwndOwner = hwnd;
pd.Flags = PD_RETURNIC;//PD_RETURNDC;
//pd.Flags = PD_RETURNDC|PD_NOPAGENUMS|PD_NOSELECTION;//|PD_RETURNDEFAULT	;

if(PrintDlg(&pd) !=0)  //if((pd.hDC = GetPrinterDC()) !=0)    // is zonder printer dialog
 {
lpDevNames = (LPDEVNAMES) GlobalLock(pd.hDevNames);
lpszDriverName = (LPSTR) lpDevNames + lpDevNames->wDriverOffset;
lpszDeviceName = (LPSTR) lpDevNames + lpDevNames->wDeviceOffset;

lpszPortName   = (LPSTR) lpDevNames + lpDevNames->wOutputOffset;
GlobalUnlock(pd.hDevNames);
pd.hDC = CreateDC(lpszDriverName, lpszDeviceName, lpszPortName, NULL);


  if ((fpin=fopen(Tempfile,"r")) == NULL)   filetrue=FALSE;
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
  SetViewportExt (pd.hDC, xPage * Breedte_grafiek, yPage*Hoogte_grafiek) ;
  SetViewportOrg (pd.hDC, xPage *0.1, yPage*0.0) ;
  SetROP2 (pd.hDC, nDrawingMode) ;
  Teken_assen(hwnd,pd.hDC,&axcal);
  Teken_logit_data(hwnd,pd.hDC,&axcal);
  teken_graph(hwnd,pd.hDC,&axcal,axcal.X_As,axcal.Y_As);
  SetWindowExt   (pd.hDC, xPage, yPage);
  SetViewportExt (pd.hDC, xPage, yPage) ;
  SetViewportOrg (pd.hDC, xPage *0.05, yPage*0.0) ;
  filetrue=TRUE;
  regel=2+(yPage*Hoogte_grafiek/cyChar);

	while(filetrue)
	{
	  if(fgets(inputline,255,fpin)==NULL)    break;
	  TextOut(pd.hDC,20,regel++ * cyChar,inputline,strlen(inputline)-1);
	  if(regel>=yPage/cyChar-3)
		 {
			 Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
			 regel=2;
		 }
	 }
  Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
  Escape (pd.hDC, ENDDOC, 0, NULL, NULL) ;
  DeleteDC(pd.hDC);

 if (pd.hDevMode != NULL)   		GlobalFree(pd.hDevMode);
 if (pd.hDevNames != NULL)  		GlobalFree(pd.hDevNames);
  }

 else bUserAbort=TRUE;
 fclose(fpin);
 return bError || bUserAbort ;
}


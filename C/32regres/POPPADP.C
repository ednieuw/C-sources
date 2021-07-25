/*---------------------------------------
	POPPADP.C --
  ---------------------------------------*/
#include "wlogit.h"
#include "COMMDLG.H"
//#include "print.h"

BOOL PrintMyPage (HWND hwnd);

extern FILE *fpin;
//extern int cyChar;
extern char szAppName[] ;
extern TEXTMETRIC    tm;
BOOL   bUserAbort ;

BOOL PrintMyPage (HWND hwnd)
{
char        inputline[255];
short       filetrue;
static char szSpMsg [] = "Print: Printing" ;
BOOL        bError = FALSE ;
int         regel,xPage, yPage ;
float			Breedte_grafiek=0.5,Hoogte_grafiek=0.3;
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
  SetWindowExtEx   (pd.hDC, xPage, yPage,NULL);
  SetViewportExtEx (pd.hDC, xPage * Breedte_grafiek, yPage*Hoogte_grafiek,NULL);
  SetViewportOrgEx (pd.hDC, xPage *0.1, yPage*0.0,NULL);
  SetROP2 (pd.hDC, nDrawingMode) ;
  Teken_assen(hwnd,pd.hDC,&axcal);
  Teken_logit_data(hwnd,pd.hDC,&axcal);
  teken_graph(hwnd,pd.hDC,&axcal,axcal.X_As,axcal.Y_As);
  SetWindowExtEx   (pd.hDC, xPage, yPage,NULL);
  SetViewportExtEx (pd.hDC, xPage, yPage,NULL);
  SetViewportOrgEx (pd.hDC, xPage *0.05, yPage*0.0,NULL) ;
  filetrue=TRUE;
  regel=2+(yPage*Hoogte_grafiek/cyChar);
  if ((fpin=fopen(Tempfile,"r")) == NULL)   filetrue=FALSE;
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
	fclose(fpin);
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


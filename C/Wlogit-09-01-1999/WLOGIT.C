
/*---------------------------------------
	WLOGIT.C
	 (c) Ed Nieuwenhuys, 1992
  ---------------------------------------*/
#include "wlogit.h"
#include "commdlg.h"
#include "shellapi.h"
#include "direct.h"

#define  WM_menu 67
#define LOTUS1 0X0404
#define LOTUS2 0X0405
//#define MAXROWS 900          // maximale dimensies worksheet 200
#define MAX_COL 4
//#define DATUM __DATE__

//PRINTDLG pd;

int    AUTOMENU = FALSE;
int    MAX_ROW;
int    size;

char   huge **naam;
char   huge **label;
double huge **data;
double huge **temp;
char   huge *ykpunten;
HGLOBAL hglb_naam[MAXROWS+1];
HGLOBAL hglb_label[MAX_COL+1];
HGLOBAL hglb_data[MAX_COL+1];
HGLOBAL hglb_temp[MAX_COL+1];
HGLOBAL hglb_ykpunten;
HGLOBAL hglb_logit;
HWND hedit;

char   szAppName  [] = "Logit" ;
char   szUntitled [] = "(untitled)" ;
short  cxClient,cyClient;
short  cxC,cyC;
short  cxChar,cxCaps,cyChar;
int    rows=0, columns=0;
int    labels,xx,yy,aa,rows_temp,kolom;
int    print,lost,LogXas,LogYas,LogXData,LogYData,berekend;
int    Line_regr,Line_prob,Line_xy;
int	 Ticks,Switchax,JoinResults;
short  StopCalcAll,NoGraph,Regressie;
int    Spearm,Xzero,Yzero;
int	 Numgraph;
int    test_selection[MAX_COL];
double aantal=0;
double slope,intercept,C,D,reslogit;
double corr;
double somx,somy,upx,upy,maxP;
int    PICOPEN;
int    deleted;
char   workingdir[MAXPATH];
long   Pstarted,Fileinput,ManualInput,Rescalcs,PRNfile_auto;

HANDLE 		hInst;
LOGFONT 		lf;
CHOOSEFONT 	cf;
TEXTMETRIC  tm;

FILE *fp;
FILE *fpin;
FILE *fpout;
char szRealFileName[MAXSTRLEN] ;
int  geladen=0;
int  wis=1;
int  HAND;
char Pic_output_file[MAXPATH];	// = "RESLOGIT.PIC";
char Tempfile[MAXPATH];			// = "RESLOGIT.TMP"; zie IDM_AUTOPRN
char Editor[MAXPATH]= "NOTEPAD.EXE";
char Adjustment_file[MAXPATH]="WLOGIT.ADJ";
/****************************  FUNCTION PROTOTYPES ***************/
int PASCAL 			WinMain (HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow);
BOOL FAR PASCAL 	AboutDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL FAR PASCAL 	KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 				   DoCaption (HWND hwnd, char *szFileName);
short 				AskAboutQuit (HWND hwnd, char *szFileName);
short 				Out_of_Memory (HWND hwnd);
LRESULT CALLBACK 	WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);
void   				Display_Bitmap(HANDLE,HANDLE);
void   				read_wks(char *);
void   				skip(FILE *,long );
void 				   clear_data(void);
double 				getd(FILE *);
void   				mallocdata(HWND hwnd);
void   				freemallocdata(void);
void huge ** 		Alloceer_data(HWND hwnd,long Kolom, long Rij,HGLOBAL *Handle, int Sizevan);
short 				Make_Last_Adjustment_file(HWND hwnd);
short 				Read_Last_Adjustment_file(HWND hwnd);
void 				   Make_PRN_output_file(void);
void 				   Make_temp_files(void);
HANDLE   			hDLL;
extern int  		calc(HANDLE,HWND,int , int );
//char 				Prn_output_file[MAXSTRLEN];
int   				DrawPicFile=FALSE;			//Schrijf PIC file naar disk
int   				DrawGraphScreen=TRUE;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D
short 				Yklijninvoer;
short				   EditorActive;
/************************************************************/

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdLine, int nCmdShow)
{
MSG      msg;
HWND     hwnd ;
WNDCLASS wndclass ;

hDLL =   LoadLibrary ("BWCC.DLL");

Regressie= IDC_LOGIT;
lost     	= FALSE;
print    	= FALSE;
LogXData 	= FALSE; //Log data x-as
LogYData 	= FALSE; //Log data y-as
axcal.X_Astrans = TRUE; //Log x-as
axcal.Y_Astrans = TRUE; //Log y-as
berekend 	= FALSE;
PICOPEN  	= FALSE;
Line_regr	= TRUE;
Line_prob	= FALSE;
Line_xy  	= FALSE;
Spearm		= FALSE;
Xzero	   	= FALSE;
Yzero 		= FALSE;
Numgraph 	= FALSE;
Ticks	 		= 0;     // =inside
Switchax 	= FALSE;
StopCalcAll	= FALSE;       // error in calculation go back to menu
NoGraph		= FALSE;
JoinResults = FALSE;
PRNfile_auto= FALSE;
upx=upy		= 1E6;
maxP		   = 1.0;
MAX_ROW		= MAXROWS;
szRealFileName[0]=0;
lpszCmdLine	= lpszCmdLine;

	  if (!hPrevInstance)
	  {
	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;//| CS_SAVEBITS;
	  wndclass.lpfnWndProc   = (WNDPROC)WndProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (hInstance, "WLOGIT") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;
	  wndclass.lpszClassName = szAppName ;
	  RegisterClass (&wndclass) ;

	  }
//	  mallocdata(hwnd);
	  hInst = hInstance;
	  nCmdShow  = SW_SHOWNORMAL;//MAXIMIZED;
	  hwnd = CreateWindow (szAppName,"Logit",
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;

	  mallocdata(hwnd);

	  ShowWindow (hwnd, nCmdShow) ;
	  UpdateWindow (hwnd);
	  while (GetMessage (&msg, NULL, 0, 0))
			 {
					TranslateMessage (&msg) ;
					DispatchMessage (&msg) ;
			 }
	  return msg.wParam ;
	  }

BOOL FAR PASCAL AboutDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  switch (message)
	  {
	  case WM_INITDIALOG:							return TRUE ;
	  case WM_COMMAND:
			 switch (wParam)
			 {
			 case IDOK:	EndDialog (hwnd, 0);		return TRUE ;
			 }
			 break ;
	  }
	  lParam=lParam;
	  return FALSE ;
	  }

BOOL FAR PASCAL KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  char text[280];
  lParam=lParam;
  switch (message)
	  {
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg,Regressie)
					,BM_SETCHECK,TRUE			,0L);
//			SendMessage(GetDlgItem(hDlg,IDM_LINEREGR)
//				,BM_SETCHECK,Line_regr	,0L);
			SendMessage(GetDlgItem(hDlg,IDM_XLOGas)
				  ,BM_SETCHECK,axcal.X_Astrans,0L);
			SendMessage(GetDlgItem(hDlg,IDM_YLOGas)
				  ,BM_SETCHECK,axcal.Y_Astrans,0L);
			SendMessage(GetDlgItem(hDlg,IDM_XZERO)
				  ,BM_SETCHECK,Xzero			,0L);
			SendMessage(GetDlgItem(hDlg,IDM_YZERO)
				  ,BM_SETCHECK,Yzero			,0L);
			SendMessage(GetDlgItem(hDlg,IDC_JOINRESULTS)
				,BM_SETCHECK,JoinResults	,0L);
			SendMessage(GetDlgItem(hDlg,IDC_AUTOPRN)
				,BM_SETCHECK,(WPARAM)PRNfile_auto	,0L);
			CheckRadioButton(hDlg,IDM_Ticksin,IDM_Ticksout,Ticks?IDM_Ticksout:IDM_Ticksin);
			SetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file);
			SetDlgItemText(hDlg,IDM_PRNFILENAME,Tempfile);
			SetDlgItemText(hDlg,IDM_RESFILE,Editor);
			strcpy(text,"abcdefghijklmnop");
			sprintf(text,"%0.6G",upy);
			SetDlgItemText(hDlg,IDM_MAXY,text);
			sprintf(text,"%0.6G",upx);
			SetDlgItemText(hDlg,IDM_MAXX,text);
			sprintf(text,"%d",polynoomgraad);
			SetDlgItemText(hDlg,IDC_POLYDEGREE,text);
					 return TRUE ;
		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
			 case IDC_POLYNOOM :
								SendMessage(GetDlgItem(hDlg,Regressie)
										,BM_SETCHECK,FALSE			,0L);
								Regressie=IDC_POLYNOOM;
								return 0;
			 case IDC_LINEAIR :
								SendMessage(GetDlgItem(hDlg,Regressie)
										,BM_SETCHECK,FALSE			,0L);
								Regressie=IDC_LINEAIR;
								axcal.X_Astrans = FALSE;
								axcal.Y_Astrans = FALSE;
								SendMessage(GetDlgItem(hDlg,IDM_XLOGas)
									  ,BM_SETCHECK,axcal.X_Astrans,0L);
								SendMessage(GetDlgItem(hDlg,IDM_YLOGas)
									  ,BM_SETCHECK,axcal.Y_Astrans,0L);
								return 0;
			 case IDC_RID :
								SendMessage(GetDlgItem(hDlg,Regressie)
										,BM_SETCHECK,FALSE			,0L);
								Regressie=IDC_RID;
								axcal.X_Astrans = FALSE;
								axcal.Y_Astrans = FALSE;
								Xzero=Yzero=TRUE;
								SendMessage(GetDlgItem(hDlg,IDM_XLOGas)
									  ,BM_SETCHECK,axcal.X_Astrans,0L);
								SendMessage(GetDlgItem(hDlg,IDM_YLOGas)
									  ,BM_SETCHECK,axcal.Y_Astrans,0L);
								SendMessage(GetDlgItem(hDlg,IDM_XZERO)
									  ,BM_SETCHECK,Xzero			,0L);
								SendMessage(GetDlgItem(hDlg,IDM_YZERO)
									  ,BM_SETCHECK,Yzero			,0L);
								return 0;
			 case IDC_LOGIT :
								SendMessage(GetDlgItem(hDlg,Regressie)
										,BM_SETCHECK,FALSE			,0L);
								Regressie=IDC_LOGIT;
								axcal.X_Astrans = TRUE;
								axcal.Y_Astrans = TRUE;
								Xzero=Yzero=FALSE;
								SendMessage(GetDlgItem(hDlg,IDM_XLOGas)
									  ,BM_SETCHECK,axcal.X_Astrans,0L);
								SendMessage(GetDlgItem(hDlg,IDM_YLOGas)
									  ,BM_SETCHECK,axcal.Y_Astrans,0L);
								SendMessage(GetDlgItem(hDlg,IDM_XZERO)
									  ,BM_SETCHECK,Xzero			,0L);
								SendMessage(GetDlgItem(hDlg,IDM_YZERO)
									  ,BM_SETCHECK,Yzero			,0L);
								return 0;
//			 case IDM_LINEREGR :
//								Line_regr=1-Line_regr;
//								return 0 ;
			 case IDM_XLOGas:
								axcal.X_Astrans = 1-axcal.X_Astrans;
								return 0 ;
			 case IDM_YLOGas:
								axcal.Y_Astrans=1-axcal.Y_Astrans;
			 case IDM_XZERO:
								Xzero=1-Xzero;
								return 0 ;
			 case IDM_YZERO:
								Yzero=1-Yzero;
								return 0 ;
			 case IDM_Ticksin:
			 case IDM_Ticksout:
								CheckRadioButton(hDlg,IDM_Ticksin,IDM_Ticksout,wParam);
								Ticks= (BYTE)(wParam==IDM_Ticksin?0:2);
								return 0 ;
			 case  IDM_PICFILE:
								GetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file,MAXPATH-1);
								return 0;
			 case  IDM_PRNFILENAME:
								GetDlgItemText(hDlg,IDM_PRNFILENAME,Tempfile,MAXPATH-1);
								return 0;
			 case  IDM_RESFILE:
								GetDlgItemText(hDlg,IDM_RESFILE,Editor,MAXPATH-1);
								return 0;
			 case  IDM_MAXX:
								GetDlgItemText(hDlg,IDM_MAXX,text,20);
								upx=atof(text);
								return 0;
			 case  IDM_MAXY:
								GetDlgItemText(hDlg,IDM_MAXY,text,20);
								upy=atof(text);
								return 0;
			 case IDC_POLYDEGREE:
								GetDlgItemText(hDlg,IDC_POLYDEGREE,text,2);
								polynoomgraad=atoi(text);
								return 0;
			 case  IDC_AUTOPRN:
								PRNfile_auto=1-PRNfile_auto;
								if(geladen)	Make_PRN_output_file();
//								else 	  	Make_temp_files();
								SetDlgItemText(hDlg,IDM_PRNFILENAME,Tempfile);
								SetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file);
								return 0;
			 case  IDC_JOINRESULTS:
								 JoinResults=1-JoinResults;
								 SendMessage(GetDlgItem(hDlg,IDC_JOINRESULTS)
											,BM_SETCHECK,JoinResults	,0L);
								return 0;
			 default:
								EndDialog (hDlg, wParam) ;
								return TRUE ;
				 }
	  }
	  return FALSE ;
	  }


void DoCaption (HWND hwnd, char *szFileName)
	  {
	  char szCaption [256] ;

	  wsprintf (szCaption, "File:%s Version:%s  S%ld,F%ld,M%ld,C%ld",
			 (LPSTR) (szFileName [0] ? szFileName : szUntitled), DATUM,Pstarted,Fileinput,ManualInput,Rescalcs) ;
	  SetWindowText (hwnd, szCaption) ;
	  }

short AskAboutQuit (HWND hwnd, char *szFileName)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;
	  szFileName=szFileName;

	  wsprintf (szBuffer, "Quit Logit ?");
		nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_YESNO | MB_ICONQUESTION);
	 return nReturn ;
	  }

short AreYouSure (HWND hwnd)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;

	  wsprintf (szBuffer, "Logitate All ?");
		nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
	  return nReturn ;
	  }

short Out_of_Memory (HWND hwnd)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;

	  wsprintf (szBuffer, "Out of Memory");
	  nReturn = MessageBox (hwnd, szBuffer, szAppName,MB_OK | MB_ICONSTOP);
	  SendMessage(hwnd,WM_CLOSE,NULL,NULL);
	  return nReturn ;
	  }

LRESULT CALLBACK WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  static BOOL    bNeedSave = FALSE ;
	  static FARPROC lpfnAboutDlgProc;
	  static HANDLE  hInst,hInstKnop,hInstBitmap,hInstgraph,hInstSelect,hInstInput;
	  static FARPROC lpfnKnop;
	  HDC	    	 	  hdc;
	  int            i,j;
	  char			  text[256];
	  WORD 			  WMpaint=TRUE;
	  char 			  szMsg[80];
	  HCURSOR 		  hcurSave;

	  switch (message)
	  {
	  case WM_CREATE:
			hdc=GetDC(hwnd);
			hInstKnop 			= ((LPCREATESTRUCT) lParam)->hInstance ;
			lpfnKnop 			= MakeProcInstance (KnoppenProc, hInstKnop) ;
			hInstBitmap 		= ((LPCREATESTRUCT) lParam)->hInstance ;
			hInst 				= ((LPCREATESTRUCT) lParam)->hInstance ;
			lpfnAboutDlgProc  = MakeProcInstance (AboutDlgProc, hInst) ;
			hInstgraph  		= ((LPCREATESTRUCT) lParam)->hInstance ;
			hInstSelect 		= ((LPCREATESTRUCT) lParam)->hInstance ;
			hInstInput  		= ((LPCREATESTRUCT) lParam)->hInstance ;
			SelectObject (hdc, GetStockObject (DEVICE_DEFAULT_FONT));//SYSTEM_FONT)) ;
			GetTextMetrics (hdc, &tm) ;
			cxChar = tm.tmAveCharWidth ;
			cyChar = tm.tmHeight + tm.tmExternalLeading ;
			ReleaseDC (hwnd, hdc) ;
			getcwd(text, MAXPATH-15);
			strcat(text,"\\");
			strcat(text,Tempfile);
			strcpy(Tempfile,text);
			Read_Last_Adjustment_file(hwnd);
			getcwd(workingdir, MAXPATH);
			strupr(workingdir);
			Make_PRN_output_file();
			Pstarted++;
			PostMessage(hwnd,WM_menu,NULL,NULL);
			return 0 ;

	  case WM_SIZE:
			cxClient= LOWORD(lParam);
			cyClient= HIWORD(lParam);
			cxC= LOWORD(lParam);//voor bitmap
			cyC= HIWORD(lParam);
			return 0 ;

	  case WM_SETFOCUS:
			SetFocus(hwnd);
			if(EditorActive)
			{	EditorActive=FALSE; PostMessage(hwnd,WM_menu,NULL,NULL);}
			return 0;

	  case WM_PAINT :
			if(!PICOPEN && WMpaint)
			{
			BeginPaint(hwnd,&ps);
			hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
			Display_Bitmap(hwnd,hInstBitmap);
			ShowWindow (hwnd, SW_SHOWNORMAL);//MAXIMIZED) ;
			DoCaption (hwnd, szRealFileName) ;
			SetCursor(hcurSave);
			EndPaint(hwnd,&ps);
			}
		  return 0;
	  case WM_menu :
			wParam=DialogBox(hInstKnop,"KNOPPEN",hwnd,lpfnKnop);
			PostMessage(hwnd,WM_COMMAND,wParam,NULL);
			return 0;

	  case WM_COMMAND :
			StopCalcAll=FALSE;
			switch (wParam)
			{
			 case IDM_LOAD:
				  lost = FALSE;
				  fcloseall();           // voor de zekerheid
				  if(CMUFileOpen(hwnd,0))
					{
					if(Loaded_file_name[0]!=0)
						{
						lstrcpy (szRealFileName,Loaded_file_name) ;
						DoCaption (hwnd, szRealFileName) ;
						bNeedSave = FALSE ;
						if (strlen(szRealFileName) < 2) break;
						berekend = 0; 	rows_temp = 0;	geladen = 1;
						clear_data();
						read_wks(szRealFileName);
						Make_PRN_output_file();
						Fileinput++;
						}
					}
				  HAND=FALSE;
				  PostMessage(hwnd,WM_menu,NULL,NULL);
				  return(0);
			 case IDM_BEREKEN:
				  Yklijninvoer=FALSE;
				  if(geladen)
					{
					Make_PRN_output_file();
					hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
					berekend=FALSE;
					if(Switchax) berekend=calc(hInstgraph,hwnd,1,0);//not implemented
					else		 berekend=calc(hInstgraph,hwnd,0,1);
					if(berekend&&!StopCalcAll)
						{
						SetCursor(hcurSave);
						if((fpout=fopen(Tempfile,"wt")) == NULL)
							{
							MessageBox (hwnd, "Can not open outputfile","Disk full?",MB_OK | MB_ICONQUESTION);
							PostMessage(hwnd,WM_menu,NULL,NULL);
							return 0;
							}
						else
							{
							if(berekend) DoCalcResultDlg(hInstInput,hwnd);
							fclose(fpout);
							Rescalcs+=rows;
							}
						 }
				  }
				  else MessageBeep(1);
				  PostMessage(hwnd,WM_menu,NULL,NULL);
				  return(0);
			 case IDM_MANINPUT:  //Manual input
				  InvalidateRect(hwnd,NULL,TRUE);
				  StopCalcAll=FALSE;
				  HAND = TRUE;
				  lost = FALSE;
				  berekend  = 0;
				  rows_temp = 0;
//				  clear_data();
				  for(j=0;j<MAX_YK_PNT;j++)
					{
					for(i=0;i<2;i++)  temp[i][j]=MISSING;
//					data[2][j]=1.0;
					ykpunten[j]=FALSE;
					}
				  Yklijninvoer=TRUE;
				  ManualInput++;
				  Make_PRN_output_file();
				  DoCalcResultDlg(hInstInput,hwnd);
				  PostMessage(hwnd,WM_menu,NULL,NULL);
				  return 0 ;
			 case IDM_PRINT:
				  if(geladen & berekend) PrintMyPage(hwnd);
				  else        MessageBeep(1);
				  PostMessage(hwnd,WM_menu,NULL,NULL);
				  return 0 ;
			 case IDM_ABOUTP:
				  DialogBox (hInst, "AboutBox", hwnd,lpfnAboutDlgProc) ;
				  PostMessage(hwnd,WM_menu,NULL,NULL);
				  return 0 ;
			 case IDM_CLOSE:
				  SendMessage(hwnd,WM_CLOSE,NULL,NULL);
				  return 0;
			 case IDM_RESET:
				  return 0 ;

			 case IDM_FURELISA:
				  clear_data();
				  if(Elisa(hwnd))
					{
					Fileinput++;
					lstrcpy (szRealFileName,Loaded_file_name) ;
					DoCaption (hwnd, szRealFileName) ;
					Make_PRN_output_file();
					HAND=FALSE;
					}
				 setdisk(workingdir[0]-'A');
				 chdir(workingdir);
				 PostMessage(hwnd,WM_menu,NULL,NULL);
				 return 0;
			case IDM_EDITOR:
				 hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
				 strcpy(text,Editor);
				 strcat(text," ");
				 strcat(text,Tempfile);
				 WMpaint=FALSE;
				 hedit=ShellExecute(hwnd,"open",Editor,Tempfile,"c:\windows",SW_SHOWMAXIMIZED);
				 if (hedit < 32)
					{
					sprintf(szMsg, "Exec failed; error code = %d", hedit);
					MessageBox(hwnd, szMsg, "Error", MB_ICONSTOP);
					PostMessage(hwnd,WM_menu,NULL,NULL);
					}
				 EditorActive=TRUE;
				 BringWindowToTop(hedit);
				 SetCursor(hcurSave);
				 WMpaint=TRUE;
				 return 0;
			 case 1:
				 PostMessage(hwnd,WM_menu,NULL,NULL);
				 return 0;
			 default:
				 MessageBeep(0);
				 SendMessage(hwnd,WM_CLOSE,0L,0L);
				 return 0;
			}
  return 0;

  case WM_CLOSE:
		if (IDYES== AskAboutQuit (hwnd, szRealFileName))
		DestroyWindow (hwnd) ;
		else PostMessage(hwnd,WM_menu,NULL,NULL);
		return 0 ;

  case WM_QUERYENDSESSION:
		if (!bNeedSave || IDCANCEL != AskAboutQuit (hwnd, szRealFileName))
		return 1L ;

  case WM_DESTROY:
		if (hDLL) FreeLibrary (hDLL);
		if((fpout=fopen("RESLOGIT.TMP","wt")) == NULL)
			MessageBox (hwnd, "Can not open outputfile",szAppName,MB_OK | MB_ICONQUESTION);
		else
			{
			fprintf(fpout,"%s","Nothing calculated\n");
			fclose(fpout);
			}
		PostQuitMessage (0) ;
		freemallocdata();
		Make_Last_Adjustment_file(hwnd);
		return 0 ;
  }
 return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//-------------------------------------------------------------------------------------------
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap)
{
 HDC 	    hdcMemory, hdcbm;
 HBITMAP    hbmpMyBitmap, hbmpOld;
 BITMAP     bm;
 hbmpMyBitmap = LoadBitmap(hInstBitmap, "WCORRBMP");
 GetObject(hbmpMyBitmap, sizeof(BITMAP), &bm);
 hdcbm = GetDC(hwndbm);
 hdcMemory = CreateCompatibleDC(hdcbm);
 hbmpOld = SelectObject(hdcMemory, hbmpMyBitmap);
//	BitBlt(hdcbm, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCCOPY);
 StretchBlt(hdcbm, 0, 0, cxC/3,cyC/1.5, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
 SelectObject(hdcMemory, hbmpOld);
 DeleteDC(hdcMemory);
 ReleaseDC(hwndbm, hdcbm);
 DeleteObject(hbmpMyBitmap);
 return;
 }

/*************** Lees lotus *.wk1 of *.wks in ***************/


void read_wks(char *infile)
{
 enum opcode {
   BOF=0,Eof,CALCMODE,CALCORDER,SPLIT,
   SYNC=5,DIMENSIONS,WINDOW1,COLW1,WINDOW2,
   COLW2=10,NRANGE,BLANK,INTEGER,NUMBER,
   LABEL=15,FORMULA,
   TABLE=24,QRANGE,PRANGE,SRANGE,FRANGE,KRANGE1,
   DRANGE=32,KRANGE2=35,PROTECT,FOOTER,HEADER,SETUP,MARGINS,
   LABELFMT=41,TITLES,GRAPH=45,NGRAPH,CALCOUNT,FORMAT,CURSORW12,
   STRING=51,SNRANGE=71,WKSPASS=75,
   HIDCOL1=100,HIDCOL2,PARSE,RRANGES,MRANGES=105,CPI=150};

 int 	op, formula_size;//,file_type;
 int 	column, row,  end_col, end_row;//,start_col,start_row;
 char   c,pos,format;
 double doubleing,integer;
 long 	count,body_length;

 lost=FALSE;
 count=0;
 labels=0;

 if((fp=fopen(infile,"rb")) == NULL) { exit(1);    }
 op=getw(fp);
 while (op!=Eof)
 {
 body_length=(long)getw(fp);
 if (body_length < 0) {   exit(-1);  }
 count += body_length+4;

 switch(op)
  {
 case INTEGER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 integer=(double)getw(fp);
//	 if (integer < 0 ) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (row > rows) rows=row;
	 data[column][row]=integer;
	 break;
 case NUMBER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 doubleing=getd(fp);
//	 if (doubleing <0 || doubleing > 1e10) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (row > rows) rows=row;
	 data[column][row]= doubleing;
	 break;
 case FORMULA:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 doubleing=(double)getd(fp);
	 formula_size=getw(fp);
	 skip(fp,formula_size);
//	 if (doubleing <0 || doubleing > 1e10) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (row > rows) rows=row;
	 data[column][row]= doubleing;
	 break;
 case BLANK:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 data[column][row]=(double)MISSING;
	 break;
 case LABEL:
	 pos=0;
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 if (row < MAX_ROW && column < MAX_COL)
		{
		if (column > columns) columns=column;
		data[column][row]=(double)MISSING;
		labels=columns;
		}
	 c=getc(fp);
	 while (c)
		{
		  if (column < MAX_COL  && pos<MAX_LEN-1 && row==0)
				label[column][pos++]=c;
		  else if(row < MAX_ROW && pos<MAX_LEN-1 && column==0)
				naam[row][pos++]=c;
		  c=getc(fp);
		 }
	 if (column < MAX_COL && row==0 )	 label[column][pos++]=0;
	 if (row < MAX_ROW && column==0)  	 naam[row][pos++]=0;
	 break;
 case BOF:
	 //file_type=
	 getw(fp);
	 break;
 case DIMENSIONS:
	 //start_col=
	 getw(fp);
	 //start_row=
	 getw(fp);
	 end_col=getw(fp)+1;
	 end_row=getw(fp)+1;
	 rows =   (end_row >= MAX_ROW ) ? MAX_ROW : end_row ;
	 columns= (end_col >= MAX_COL)  ? MAX_COL : end_col ;
	 rows=columns=0;
	  break;
 case CPI:
	  skip(fp,(int)body_length);
	  break;
 default:
		skip(fp,body_length);
	 }
  op=getw(fp);
  }
fclose(fp);
if(row > MAX_ROW-2) lost = TRUE;
op=format;
rows++;
}

void skip(FILE *fp,long noofbytes)
{
 char c;
 while (noofbytes>0)  { c=getc(fp);  noofbytes--; }
 c=c;
}
//-----------------------------------------------------------------------
void clear_data(void)
{
int row,column;
for (row=0;row<MAXROWS;row++)
  {
	for (column=0;column<MAX_COL;column++)  data[column][row]=(double)MISSING;
	naam[row][0]=0;
  }
 for (column=0;column<MAX_COL;column++) strcpy((char*)label[column]," ");
 memset(ykpunten,0,MAX_YK_PNT);
 memset(Logit,0,MAX_ROW * sizeof(RESULTS));
}

double getd(FILE *fp)
{
 double a;
 char *b;
 int i,j;
  b=(char *) &a;
  for (i=0;i<sizeof(double);++i)
	{
	 if((j = getc(fp)) == EOF) return(-1.0);
	 b[i]= j & 0xFF;
	}
return(a);
}

//---------------------------------------------------------------------------------------------
void mallocdata(HWND hwnd)
 {

naam  = (char huge **)Alloceer_data(hwnd,		MAX_ROW+1,	MAX_LEN+1,hglb_naam, sizeof(char));
label = (char huge **)Alloceer_data(hwnd,		MAX_COL+1,	MAX_LEN+1,hglb_label, sizeof(char));
temp  = (double huge **)Alloceer_data(hwnd,	MAX_COL+1,	MAX_ROW+1,hglb_temp, sizeof(double));
data = (double huge **)Alloceer_data(hwnd,	MAX_COL+1,	MAX_ROW+5,hglb_data, sizeof(double));

hglb_ykpunten 	= GlobalAlloc(GPTR,((int)MAX_YK_PNT) * sizeof(char));
ykpunten 		= (char huge*)GlobalLock(hglb_ykpunten);
hglb_logit 		= GlobalAlloc(GPTR,((int)MAX_ROW) * sizeof(RESULTS));
Logit 			= (RESULTS*)GlobalLock(hglb_logit);
}
//---------------------------------------------------------------------------------------------
void freemallocdata(void)
{
 int n;
 for (n=MAX_ROW; n >= 0;n--)
	{GlobalUnlock(hglb_naam[n]);		GlobalFree(hglb_naam[n]); }
 for (n=MAX_COL; n >= 0;n--)
	{GlobalUnlock(hglb_label[n]);		GlobalFree(hglb_label[n]);}
 for (n=MAX_COL; n >= 0;n--)
	{GlobalUnlock(hglb_temp[n]);		GlobalFree(hglb_temp[n]); }
 for (n=(int)MAX_COL; n >= 0;n--)
	{GlobalUnlock(hglb_data[n]);		GlobalFree(hglb_data[n]); }

	 GlobalUnlock(hglb_ykpunten);	  	GlobalFree(hglb_ykpunten);
	 GlobalUnlock(hglb_logit);		 	GlobalFree(hglb_logit);
}

//----------------------------------------------------------------------------------------------
void huge ** Alloceer_data(HWND hwnd,long Kolom, long Rij,HGLOBAL *Handle, int Sizevan)
{
int i,n;
void huge **array_naam=NULL;
 Handle[0] = GlobalAlloc(GPTR, (Kolom+1)  * sizeof(void*));
 if(Handle[0])
  {
	array_naam = (void huge**)GlobalLock(Handle[0]);
	for (n=1;n < Kolom;n++)
		{
		Handle[n] = GlobalAlloc(GPTR, (Rij+1) * Sizevan);
		if(Handle[n]==NULL) break;
		array_naam[n-1] = (void huge*)GlobalLock(Handle[n]);
		}
	if(n<Kolom)  //Als allocatie niet lukt de al gealloceerde pointers freeen
		{
		 for (i=n-1; i >= 0;i--)
			 {	GlobalUnlock(Handle[i]);	GlobalFree(Handle[i]); }
		 array_naam=NULL;
		 Out_of_Memory (hwnd);
		 }
	}
return array_naam;
 }

//------------------------------  Adjustment files ----------------------------------------------
short Make_Last_Adjustment_file(HWND hwnd)
 {
 FILE *stream;
	 if((stream=fopen(Adjustment_file,"wt")) == NULL)
			MessageBox (hwnd, "Can not open outputfile",szAppName,MB_OK | MB_ICONQUESTION);
			 else
			  {
				fprintf(stream,"%s,Editor\r\n",Editor);
				fprintf(stream,"%ld,Programstarted\r\n",Pstarted);
				fprintf(stream,"%ld,Fileinput\r\n",Fileinput);
				fprintf(stream,"%ld,ManualInput\r\n",ManualInput);
				fprintf(stream,"%ld,Rescalcs\r\n",Rescalcs);
				fprintf(stream,"%ld,PRNfile_auto\r\n",PRNfile_auto);
				fclose(stream);
			  }
return(TRUE);
}

short Read_Last_Adjustment_file(HWND hwnd)
 {
 FILE *stream;
 char text[42];

 Pstarted=Fileinput=ManualInput=Rescalcs=0L;
	 if((stream=fopen(Adjustment_file,"rt")) == NULL)
			Make_Last_Adjustment_file(hwnd);
	 else
		{
		fgets(text,40,stream);  strcpy(Editor,strtok(text,","));
		fgets(text,40,stream);  Pstarted=atol(strtok(text,","));
		fgets(text,40,stream);  Fileinput=atol(strtok(text,","));
		fgets(text,40,stream);  ManualInput=atol(strtok(text,","));
		fgets(text,40,stream);  Rescalcs=atol(strtok(text,","));
		fgets(text,40,stream);  PRNfile_auto=atol(strtok(text,","));
		fclose(stream);
		}
return(TRUE);
}

void Make_PRN_output_file(void)
{
char drive[MAXDRIVE];
char dir[MAXDIR];
char file[MAXFILE];
char ext[MAXEXT];

	if(geladen && PRNfile_auto && strlen(szRealFileName)>0)
	 {
		fnsplit(szRealFileName,drive,dir,file,ext);
		if(stricmp(ext,".PRN")==0)  strcpy(ext,"TXT");
		else 						strcpy(ext,"PRN");
		fnmerge(Tempfile,drive,dir,file,ext);
		strcpy(ext,"PIC");
		fnmerge(Pic_output_file,drive,dir,file,ext);
	 }
	else Make_temp_files();
}

void Make_temp_files(void)
{
	sprintf(Tempfile,"%s\\%s",workingdir,"RESLOGIT.TMP");
	sprintf(Pic_output_file,"%s\\%s",workingdir,"RESLOGIT.PIC");

}


// ---------------------------------------
//	 BLAST.C
//	 (c) Ed Nieuwenhuys, feb 1996
// ---------------------------------------
#include "blast.h"
#include "blasto.h"
#include <commdlg.h>
#include <shellapi.h>
//#include <print.h>

#define EDITID 1
#define WM_menu 67
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define LOTUS1 0X0404
#define LOTUS2 0X0406
#define MAX_LEN 40
#define MAX_LEN_NAAM 7        // Lengte van de naam column
#define DATUM __DATE__

extern int Fflag, Logflag, Kader, Header, Barsize, Fixed_distance;
int Max_dagen_oud;
//extern char* y_legend;

long   MAXROWS;
long   MAX_COL=255;

int    AUTOMENU = FALSE;
int    MAX_ROW;
int    size;
char   huge **naam;
char   huge **label;
double huge **data;
double huge **temp;
double huge **PP;
double huge *sortx;
double huge *sorty;
int	 huge *test_selection;
double huge *BSarray;
double huge *Histogram_dev;

HGLOBAL *hglb_naam;
HGLOBAL *hglb_label;
HGLOBAL *hglb_data;
HGLOBAL *hglb_temp;
HGLOBAL *hglb_PP;
HGLOBAL hglb_sortx;
HGLOBAL hglb_sorty;
HGLOBAL hglb_test_selection;

char   szAppName  [] = "BLAST EJN/RCA 08/98" ;
char   szUntitled [] = "(untitled)" ;
char   Resoutputfile[40]="";
short  cxClient,cyClient;
short  cxC,cyC;
short  cxClientOrg;
short  cyClientOrg; //correctiefont in picplib.c
short  cxChar,cxCaps,cyChar;
int    rows=0, columns=0;
int    print,lost,berekend;
int	 EditorOutputParam;
short  NoGraph;
int    PICOPEN;
int    deleted;
char   workingdir[MAXPATH];
long   Pstarted;
int	 Nooffiles;

PRINTDLG pd;
HANDLE 	hInst;
LOGFONT 	lf;
CHOOSEFONT 	cf;
TEXTMETRIC  tm;
HWND		hedit; //voor oproepen editor
HANDLE 	hDLL;
FILE *fp;
FILE *fpin;
FILE *fpout;
FILE *fpout1;
char szRealFileName[MAXSTRLEN] ;
int  geladen=0;
int  wis=1;
int  HAND;
int  Locked=FALSE;
char *Tempfile = "BLAST.TMP";
char Editor[40]= "WRITE.EXE";
char Adjustment_file[MAXPATH]="BLAST.ADJ";
/****************************  FUNCTION PROTOTYPES ***************/
extern void DoSelectOpenDlg(HANDLE hInst,HWND hwnd);

int PASCAL 			WinMain (HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow);
BOOL FAR PASCAL 	AboutDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL FAR PASCAL 	KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 					DoCaption (HWND hwnd, char *szFileName);
short 				AskAboutQuit (HWND hwnd, char *szFileName);
short 				Out_of_Memory (HWND hwnd);
LRESULT CALLBACK 	WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);

int 					sort_function( const void *a, const void *b);
void   				Display_Bitmap(HANDLE,HANDLE);
void   				mallocdata(void);
void   				freemallocdata(void);
void huge ** 		Alloceer_data(long Kolom, long Rij,HGLOBAL *Handle, int Sizevan);
void 					DeAlloceer_data(long Kolom,HGLOBAL *hglb);
short 				Make_Last_Adjustment_file(HWND hwnd);
short 				Read_Last_Adjustment_file(HWND hwnd);

extern BOOL 		CMUFileOpen( HWND);
extern BOOL 		PrintMyPage(HWND) ;
extern char 		Loaded_file_name[MAXSTRLEN];
char 					Pic_output_file[MAXSTRLEN]="temp.pic";
int   				DrawPicFile=FALSE;			//Schrijf PIC file naar disk
int   				DrawGraphScreen=TRUE;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D

/************************************************************/

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdLine, int nCmdShow)
{
MSG      msg;
HWND     hwnd ;
WNDCLASS wndclass ;
hDLL = LoadLibrary ("BWCC.DLL");

lpszCmdLine=lpszCmdLine;
lost     = FALSE;
print    = FALSE;
berekend = FALSE;
PICOPEN  = FALSE;
NoGraph	= FALSE;
Locked	= FALSE;
Logflag	= LOGFLAG;
Kader		= 1;
Header  	= 1;
Barsize	= BARSIZE;
Fixed_distance=FALSE;
Fflag	= FFLAG;
Max_dagen_oud=30;
strcpy(y_legend,"Antistof-eenheden (U/ml)");

//EditorOutputParam=REGRESPARAM;

	  if (!hPrevInstance)
	  {
	  wndclass.style         = CS_BYTEALIGNWINDOW;// |CS_PARENTDC | CS_OWNDC;
	  wndclass.lpfnWndProc   = (WNDPROC)WndProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (hInstance, "Blast") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = szAppName ;

	  RegisterClass (&wndclass) ;
	  }
	  hInst = hInstance;
//	  nCmdShow  = SW_SHOWNORMAL;
	  hwnd = CreateWindow (szAppName, NULL,
			  //WS_OVERLAPPEDWINDOW,
			  WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;
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
	  case WM_INITDIALOG:								return TRUE ;
	  case WM_COMMAND:
			 switch (wParam)
			 {
			 case IDOK:	EndDialog (hwnd, 0) ;		return TRUE ;
			 }
			 break ;
	  }
	  lParam=lParam;
	  return FALSE ;
	  }

BOOL FAR PASCAL KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  lParam=lParam;
  switch (message)
	  {
		char text[20];
		case WM_INITDIALOG:

			SendMessage(GetDlgItem(hDlg,IDC_FFLAG),BM_SETCHECK,Fflag,0L);
			SendMessage(GetDlgItem(hDlg,IDC_LOGFLAG),BM_SETCHECK,Logflag,0L);
			SendMessage(GetDlgItem(hDlg,IDC_KADER),BM_SETCHECK,Kader,0L);
			SendMessage(GetDlgItem(hDlg,IDC_HEADER),BM_SETCHECK,Header,0L);
			SetDlgItemText(hDlg,IDC_YLEGEND,y_legend);
			SetDlgItemText(hDlg,IDC_MAXDAGOUD,itoa(Max_dagen_oud,text,10));
						return TRUE ;

		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
				case IDC_YLEGEND:
					GetDlgItemText(hDlg,IDC_YLEGEND,y_legend,MAXSTRLEN);
					return 0;
				case IDC_MAXDAGOUD:
					GetDlgItemText(hDlg,IDC_MAXDAGOUD,text,18);
					Max_dagen_oud=atoi(text);
					return 0;
				case IDC_FFLAG:
					Fflag=(Fflag)?0:1;
					return 0;
				case IDC_HEADER:
					Header=1;//(Header)?0:1;
					return 0;
				case IDC_KADER:
					Kader=(Kader)?0:1;
					return 0;
				case IDC_LOGFLAG:
					Logflag=(Logflag)?0:1;
					return 0;
/*				case 'D':
				Kyo=(Kyo)?0:1;
				break;
*/
/*				case IDC_FIXEDDIST:
					Fixed_distance=(Fixed_distance)?0:1;
					return 0;
				case IDC_PENSIZE:
			clrscr();
			cprintf("Fill Pen size (1-40) default:%d\r\n: ",Fillpen);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=200) Fillpen=n;
			cprintf("\r\n\r\nContour Pen size (1-40) default:%d\r\n: ",Pen);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=200) Pen=n;
			break;
*/
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

	  wsprintf (szCaption, "File:%s Version:%s  S%ld",
			 (LPSTR) (szFileName [0] ? szFileName : szUntitled), DATUM,Pstarted) ;
	  SetWindowText (hwnd, szCaption) ;
	  }

short AskAboutQuit (HWND hwnd, char *szFileName)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;
	  szFileName=szFileName;

	  wsprintf (szBuffer, "Quit Blaster ?");
		nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_YESNO | MB_ICONQUESTION);
	  return nReturn ;
	  }

short AreYouSure (HWND hwnd)
	  {
     char  szBuffer [40] ;
     short nReturn ;

	  wsprintf (szBuffer, "Correlate All ?");
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
 static HANDLE  hInst , hInstKnop,hInstBitmap,hInstgraph,hInstSelect,hInstViewer;
 static FARPROC lpfnKnop;
 char           szFileName[256] ;
 int            n;
 WORD 			 WMpaint=TRUE;
 HDC	    	    hdc;
 HCURSOR 		hcurSave;

 switch (message)
  {
  case WM_CREATE:
		 hdc=GetDC(hwnd);
		 hInstKnop = ((LPCREATESTRUCT) lParam)->hInstance ;
		 lpfnKnop = MakeProcInstance (KnoppenProc, hInstKnop) ;
		 hInstBitmap = ((LPCREATESTRUCT) lParam)->hInstance ;
		 hInst = ((LPCREATESTRUCT) lParam)->hInstance ;
		 lpfnAboutDlgProc = MakeProcInstance (AboutDlgProc, hInst) ;
		 hInstgraph  = ((LPCREATESTRUCT) lParam)->hInstance ;
		 hInstSelect = ((LPCREATESTRUCT) lParam)->hInstance ;
		 hInstViewer = ((LPCREATESTRUCT) lParam)->hInstance ;
		 SelectObject (hdc, GetStockObject (DEVICE_DEFAULT_FONT));//SYSTEM_FONT)) ;
		 GetTextMetrics (hdc, &tm) ;
		 cxChar = tm.tmAveCharWidth ;
		 cyChar = tm.tmHeight + tm.tmExternalLeading ;


		 lf.lfHeight=21;
		 lf.lfWidth=0;
		 lf.lfWeight=FW_BOLD;
//		 lf.lfCharSet = ANSI_CHARSET;
//		 lf.lfOutPrecision= OUT_STRING_PRECIS;//OUT_DEFAULT_PRECIS;
//		 lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//		 lf.lfQuality = PROOF_QUALITY;
		 lf.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;//variable
		 strcpy(lf.lfFaceName,"Times New Roman");
		 memset(&cf, 0, sizeof(CHOOSEFONT));
		 cf.lStructSize = sizeof(CHOOSEFONT);
		 cf.hwndOwner = hwnd;
		 cf.lpLogFont = &lf;
		 cf.hDC=pd.hDC;
		 cf.iPointSize=18;
		 cf.Flags = CF_EFFECTS|CF_PRINTERFONTS|CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS;
		 cf.rgbColors = RGB(0, 0, 0);
		 cf.nFontType = PRINTER_FONTTYPE|SCREEN_FONTTYPE;

		 DoCaption (hwnd, szFileName) ;
		 Pic_output_file[0]=0;
		 Read_Last_Adjustment_file(hwnd);
		 getcwd(workingdir, MAXPATH);
		 strupr(workingdir);
		 Pstarted++;
		 mallocdata();
		 UpdateWindow(hwnd);
		 PostMessage(hwnd,WM_menu,NULL,NULL);
		 return 0 ;

  case WM_SIZE:
			 cxClientOrg=cxClient= LOWORD(lParam);
			 cyClientOrg=cyClient= HIWORD(lParam);
			 cxC= LOWORD(lParam);//voor bitmap
			 cyC= HIWORD(lParam);
			 return 0 ;

  case WM_SETFOCUS:
//			 SetFocus(hwnd);
//			PostMessage(hwnd,WM_menu,NULL,NULL);
//		   Display_Bitmap(hwnd,hInstBitmap);
		 return 0;

  case WM_PAINT :
		 if(!PICOPEN && WMpaint)
		  {
			BeginPaint(hwnd,&ps);
			hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
			Display_Bitmap(hwnd,hInstBitmap);
 //			ShowWindow (hwnd, SW_SHOWMAXIMIZED) ;
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
		 switch (wParam)
		 {
		 case IDM_LOAD:
				lost = FALSE;
				for(n=0;n<MAX_COL;n++) test_selection[n]=9999;//MISSING;
				if(CMUFileOpen(hwnd))
					{
					if(Loaded_file_name[0]!=0)
						{
//						if(Locked)freemallocdata();
						lstrcpy (szRealFileName,Loaded_file_name) ;
						lstrcpy (szFileName,Loaded_file_name) ;
						DoCaption (hwnd, szRealFileName) ;
						bNeedSave = FALSE ;
						if (strlen(szFileName) < 2) break;
						berekend = 0;
						// 	rows_temp = 0;
//						if(read_wks(hwnd,szFileName)) geladen=0;
						}
					}
//				setdisk(workingdir[0]-'A');
//				chdir(workingdir);
		 case IDM_SELECT:
				for(n=0;n<MAX_COL;n++) test_selection[n]=9999;//MISSING;
				DoSelectOpenDlg(hInstSelect,hwnd);
				PostMessage(hwnd,WM_menu,NULL,NULL);
				return 0;
		 case IDM_PRINT:
				if(geladen){
								PrintMyPage(hwnd);
								PrintEMF(hwnd);
							  }

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

		 case IDC_FONT:
				ChooseFont(&cf);
//				FontGekozen=TRUE;
				PostMessage(hwnd,WM_menu,NULL,NULL);
				return 0 ;

/*		 case IDM_EDITOR:
				hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
				strcpy(text,Editor);
				strcat(text," ");
				strcat(text,Tempfile);
				WMpaint=FALSE;
				hedit=ShellExecute(hwnd,"open",Editor,Tempfile,"c:\windows",SW_SHOWMAXIMIZED);
				if (hedit < 32)
					{
					sprintf(szMsg, "Exec failed; error code = %d", wReturn);
					MessageBox(hwnd, szMsg, "Error", MB_ICONSTOP);
					PostMessage(hwnd,WM_menu,NULL,NULL);
					}
//			 	EditorActive=TRUE;
				BringWindowToTop(hedit);
				SetCursor(hcurSave);
				WMpaint=TRUE;
				PostMessage(hwnd,WM_menu,NULL,NULL);
				return 0;
*/
		 case WM_KEYDOWN:
				switch (wParam)
					{
					case VK_RETURN: return 0;
					}
				default:
					MessageBeep(0);
					SendMessage(hwnd,WM_CLOSE,0L,0L);
//					PostMessage(hwnd,WM_menu,NULL,NULL);
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
/*		 if((fpout=fopen(Tempfile,"wt")) == NULL)
				MessageBox (hwnd, "Can not open outputfile",szAppName,MB_OK | MB_ICONQUESTION);
		 else
			  {
				fprintf(fpout,"%s","Nothing calculated");
				fclose(fpout);
			  }
*/		 if (hDLL) FreeLibrary (hDLL);
		 if(Locked)freemallocdata();
		 setdisk(workingdir[0]-'A');
		 chdir(workingdir);
		 Make_Last_Adjustment_file(hwnd);
		 PostQuitMessage (0) ;
		 return 0 ;
  }
return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//---------------------------------------------------------------
int sort_function( const void *a, const void *b)
{
	if( *((double*)a) - *((double*)b) <0 ) return -1;
	if( *((double*)a) - *((double*)b) ==0 ) return 0;
	return 1;
}
//-------------------------------------------------------------------------------------------
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap)
{
 HDC 	    hdcMemory, hdcbm;
 HBITMAP    hbmpMyBitmap, hbmpOld;
 BITMAP     bm;
 hbmpMyBitmap = LoadBitmap(hInstBitmap, "BLASTBMP");
 GetObject(hbmpMyBitmap, sizeof(BITMAP), &bm);
 hdcbm = GetDC(hwndbm);
 hdcMemory = CreateCompatibleDC(hdcbm);
 hbmpOld = SelectObject(hdcMemory, hbmpMyBitmap);
 BitBlt(hdcbm, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCCOPY);
//	StretchBlt(hdcbm, 0, 0, cxC/3,cyC/3, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
 SelectObject(hdcMemory, hbmpOld);
 DeleteDC(hdcMemory);
 ReleaseDC(hwndbm, hdcbm);
 DeleteObject(hbmpMyBitmap);
return;
}
//---------------------------------------------------------------------------------------------
void mallocdata(void)
 {
HGLOBAL hglb;

/*hglb = GlobalAlloc(GPTR,(MAX_ROW+2) * sizeof(void*));
hglb_naam = (HGLOBAL*)GlobalLock(hglb);
naam  = (char huge **)Alloceer_data(MAX_ROW+1,MAX_LEN_NAAM+1,hglb_naam, sizeof(char));
*/
hglb = GlobalAlloc(GPTR,(MAX_COL+2) * sizeof(void*));
hglb_label = (HGLOBAL*)GlobalLock(hglb);
label = (char huge **)Alloceer_data(MAX_COL+1,MAX_LEN+1,hglb_label, sizeof(char));
/*
hglb = GlobalAlloc(GPTR,(3) * sizeof(void*));
hglb_temp = (HGLOBAL*)GlobalLock(hglb);
temp  = (double huge **)Alloceer_data(2,MAX_ROW+1,hglb_temp, sizeof(double));

hglb_sortx = GlobalAlloc(GPTR,(2*MAX_ROW+1) * sizeof(double));
sortx = (double huge*)GlobalLock(hglb_sortx);

hglb_sorty = GlobalAlloc(GPTR,(2*MAX_ROW+1) * sizeof(double));
sorty = (double huge*)GlobalLock(hglb_sorty);

hglb = GlobalAlloc(GPTR,(MAX_COL+2) * sizeof(void*));
hglb_data = (HGLOBAL*)GlobalLock(hglb);
data = (double huge **)Alloceer_data(MAX_COL+1,MAX_ROW+1,hglb_data, sizeof(double));

//hglb = GlobalAlloc(GPTR,(MAX_COL+2) * sizeof(void*));
//hglb_PP = (HGLOBAL*)GlobalLock(hglb);
//PP   = (double huge **)Alloceer_data(MAX_COL+1,MAX_COL+1,hglb_PP, sizeof(double));
*/
hglb_test_selection = GlobalAlloc(GPTR,(MAX_COL+1) * sizeof(void*));
test_selection = (int huge*)GlobalLock(hglb_test_selection);
}
//---------------------------------------------------------------------------------------------
void freemallocdata(void)
{
//  DeAlloceer_data(MAX_ROW+2,hglb_naam);
  DeAlloceer_data(MAX_COL+2,hglb_label);
//  DeAlloceer_data(3,hglb_temp);
//  DeAlloceer_data(MAX_COL+2,hglb_data);
//  DeAlloceer_data(MAX_COL+2,hglb_PP);

//  GlobalUnlock(hglb_sortx);         		 GlobalFree(hglb_sortx);
//  GlobalUnlock(hglb_sorty);         		 GlobalFree(hglb_sorty);
  GlobalUnlock(hglb_test_selection);		 GlobalFree(hglb_test_selection);
  Locked=FALSE;
}

//----------------------------------------------------------------------------------------------
void huge ** Alloceer_data(long Kolom, long Rij, HGLOBAL *Handle, int Sizevan)
{
int i,n;
void huge **array_naam=NULL;

 Handle[0] = GlobalAlloc(GPTR, (Kolom+1)  * sizeof(void*));
 if(Handle[0])
  {
	array_naam = (void huge**)GlobalLock(Handle[0]);
	for (n=1;n <= Kolom+1;n++)
		{
		Handle[n] = GlobalAlloc(GPTR, Rij * Sizevan);
		if(Handle[n]==NULL) break;
		array_naam[n-1] = (void huge*)GlobalLock(Handle[n]);
		}
	if(n<=Kolom)  //Als allocatie niet lukt de al gealloceerde pointers freeen
		{
		 for (i=n-1; i >= 0;i--)
			 {
				GlobalUnlock(Handle[i]);
				GlobalFree(Handle[i]);
			 }
		 array_naam=NULL;
//		 Out_of_Memory (hwnd);
		 }
	}
 Locked=TRUE;

return array_naam;
}
//-------------------------------------------------------------------------------------------
void DeAlloceer_data(long Kolom,HGLOBAL *hglb) //obsolete
{
int n;
for (n=(int)Kolom+1; n >= 0;n--)	{GlobalUnlock(hglb[n]);	GlobalFree(hglb[n]);	}
hglb=NULL;
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
				fprintf(stream,"%d,lfHeight\r\n",lf.lfHeight);
				fprintf(stream,"%d,lfWeight\r\n",lf.lfWeight);
				fprintf(stream,"%s,lfFaceName\r\n",lf.lfFaceName);
				fprintf(stream,"%ld,Programstarted\r\n",Pstarted);
//				fprintf(stream,"%s,Y_legend\r\n",y_legend);
				fprintf(stream,"%d,MAX_DAG_OUD\r\n",Max_dagen_oud);
				fclose(stream);
			  }
return(TRUE);
}

short Read_Last_Adjustment_file(HWND hwnd)
 {
 FILE *stream;
 char text[42];

 Pstarted=0L;
	 if((stream=fopen(Adjustment_file,"rt")) == NULL)
			Make_Last_Adjustment_file(hwnd);
	 else
		{
		fgets(text,40,stream);  strcpy(Editor,strtok(text,","));
		fgets(text,40,stream);  lf.lfHeight=atoi(strtok(text,","));
		fgets(text,40,stream);  lf.lfWeight=atoi(strtok(text,","));
		fgets(text,40,stream);  strcpy(lf.lfFaceName,strtok(text,","));
		fgets(text,40,stream);  Pstarted=atol(strtok(text,","));
//		fgets(text,40,stream);  strcpy(y_legend,strtok(text,","));
		fgets(text,40,stream);  Max_dagen_oud=atoi(strtok(text,","));
		fclose(stream);
		}
return(TRUE);
}


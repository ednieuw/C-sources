/*---------------------------------------
	picdump.c
	 (c) Ed Nieuwenhuys, 1994
  ---------------------------------------*/

#include "3dview.h"
#include "commdlg.h"
#include "direct.h"
#include "shellapi.h"

#define  EDITID 1
#define  WM_menu 67
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define DATUM __DATE__

extern void do_open(HWND hWnd,char *info);

extern void DoSelectOpenDlg(HANDLE hInst,HWND hwnd);

EdCOLORREF huge **P;
HGLOBAL hglb;
HGLOBAL *hglbP;
long   stroken,breedte,hoogte;
short  SourceBMPloaded,BkgdBMPloaded=FALSE;
short  diepte,Knip,Testhalf,Testsize;
int    AUTOMENU = FALSE;
char   szAppName  [] = "3dview EJN 01/95" ;
int	 test_selection[256];
char   szUntitled [] = "" ;
char   Resoutputfile[40]="";
short  cxClient,cyClient;
short  cxC,cyC;
short  cxChar,cxCaps,cyChar;
int    print,lost,berekend;
short  Page_orientation;
int	 Numgraph=0;
int    PICOPEN;
int    deleted;
short  FontGekozen;
short	 cxCharPrn, cyCharPrn, cxCharScreen, cyCharScreen; // maten voor default font
short  cxClientOrg;
short  cyClientOrg; //correctiefont in dmpicplib.c
char 	 szRealFileName[MAXSTRLEN] ;
int  	 geladen=0;
int  	 wis=1;
int  	 HAND;
int  	 Locked=FALSE;
char 	 *Adjustment_file="3dview.SET";
short EditorActive=FALSE;
LOGFONT 		lf;
CHOOSEFONT 	cf;
TEXTMETRIC  tm;
HWND			hedit; //voor oproepen editor
HANDLE 		hDLL;
FILE 			*fp;
FILE 			*fpin;
FILE 			*fpout;
HANDLE  hInstBitmap;
RECT 			rc;
/****************************  FUNCTION PROTOTYPES ***************/
void huge ** Alloceer_data(long Kolom, long Rij, HGLOBAL *Handle, int Sizevan);
void AllocP(int stroken,int x, int y);
void DeAllocP(void);
int PASCAL 			WinMain (HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow);
BOOL FAR PASCAL 	AboutDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL FAR PASCAL 	KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 					DoCaption (HWND hwnd, char *szFileName);
short 				AskAboutQuit (HWND hwnd, char *szFileName);
short 				Out_of_Memory (HWND hwnd);
//int 					set_lowest_drive(void);
LRESULT CALLBACK 	WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);
void 					LoadSourceBitmap(HWND hwnd);
void 					LoadBackgroundBitmap(HWND hwnd);
extern BOOL 		CMUFileOpen( HWND);
extern BOOL 		PrintMyPage(HWND) ;
extern char 		Loaded_file_name[MAXSTRLEN];
char 					Pic_output_file[MAXSTRLEN];
int   				DrawPicFile=FALSE;			//Schrijf PIC file naar disk
int   				DrawGraphScreen=TRUE;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D
char 					workingdir[MAXPATH];

//************************************************************
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

void AllocP(int stroken,int x, int y)
{
if(P!=NULL) return;
hglb = GlobalAlloc(GPTR,(stroken+1) * sizeof(void*));
hglbP = (HGLOBAL*)GlobalLock(hglb);
P = (EdCOLORREF huge **)Alloceer_data(stroken+1,(breedte+10)*hoogte+10,hglbP, sizeof(EdCOLORREF));
x=x;
y=y;
}

void DeAllocP(void)
{
int n;
if(P==NULL) return;
for (n=(int)stroken+1; n >= 0;n--)
	{
		GlobalUnlock(hglbP[n]);
		GlobalFree(hglbP[n]);
	}
P=NULL;
}

//*****************************************************************
int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdLine, int nCmdShow)
{
MSG      msg;
HWND     hwnd ;
WNDCLASS wndclass ;
hDLL = LoadLibrary ("BWCC.DLL");
lpszCmdLine=lpszCmdLine;

print    = FALSE;
berekend = FALSE;
PICOPEN  = FALSE;
Locked	= FALSE;
Knip		= FALSE;
Testhalf = FALSE;
Testsize	= 100;
diepte	=-5;
stroken	= 8;

	  if (!hPrevInstance)
	  {
	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc   = (WNDPROC)WndProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (hInstance, "3dview") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = szAppName ;
	  RegisterClass (&wndclass) ;
	  }
	  nCmdShow  = SW_SHOWMAXIMIZED;

	  hwnd = CreateWindow (szAppName,"3dview",
			  WS_POPUP,
			  //WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;
	  SetWindowText (hwnd, "3dview") ;
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
			 switch (wParam)	 {
			 case IDOK:	EndDialog (hwnd, 0) ;   return TRUE ;
			 }
			 break ;
	  }
	  lParam=lParam;
	  return FALSE ;
	  }
/*
BOOL FAR PASCAL InputDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  switch (message)
	  {
	  case WM_INITDIALOG:
//							SetDlgItemText(hwnd,IDM_INPUT,selected_files[254]);
								return TRUE ;
	  case WM_COMMAND:
			 switch (wParam)
			 {
//			 unsigned drive;
			 case IDOK:
//							GetDlgItemText(hwnd,IDM_INPUT,selected_files[254],11);
//							if(strlen(selected_files[254])>0)
							 {
//							  _dos_getdrive(&drive);
//								Set_Diskvolume(hwnd,drive,selected_files[254]);
							 }
							EndDialog (hwnd, 0) ;
							return TRUE ;
			 case IDCANCEL:
							EndDialog (hwnd, 0) ;
							return TRUE ;
			 }
			 break ;
	  }
	  lParam=lParam;
	  return FALSE ;
	  }
*/
BOOL FAR PASCAL KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  BOOL error;
  switch (message)
	  {
		case WM_INITDIALOG:
			CheckDlgButton(hDlg,IDC_KNIP,Knip);
			CheckDlgButton(hDlg,IDC_TESTHALF,Testhalf);
			SetDlgItemInt(hDlg,IDC_TESTSIZE,Testsize,TRUE);
			SetDlgItemInt(hDlg,IDC_COLS,(int)stroken,TRUE);
			SetDlgItemInt(hDlg,IDC_DEPTH,diepte,TRUE);
					 return TRUE ;
		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
			case IDC_KNIP:
								CheckDlgButton(hDlg,IDC_KNIP,wParam);
								Knip=1-Knip;
								return 0;
			case IDC_TESTHALF:
								CheckDlgButton(hDlg,IDC_TESTHALF,wParam);
								Testhalf=1-Testhalf;
								if(Testsize<10 ||Testsize>100)
								{
									Testsize=100;
									SetDlgItemInt(hDlg,IDC_TESTSIZE,100,TRUE);
								}
								return 0;
			case  IDC_TESTSIZE:
								Testsize=GetDlgItemInt(hDlg,IDC_TESTSIZE,&error,TRUE);
								return 0;
			case  IDC_COLS:
								stroken=GetDlgItemInt(hDlg,IDC_COLS,&error,TRUE);
								if(stroken>0)  breedte=cxClient/stroken;
								return 0;
			case  IDC_DEPTH:
								diepte=GetDlgItemInt(hDlg,IDC_DEPTH,&error,TRUE);
								return 0;
/*			case  IDM_RESFILE:
								GetDlgItemText(hDlg,IDM_RESFILE,Editor,40);
								return 0;
*/			 default:
								GetClientRect(GetDesktopWindow(),&rc);
								if(Testhalf)
								{
									cxClient=cxC=cxClientOrg=
											(int)((float)rc.right*(float)Testsize/100.0);
									cyClient=cyC=cyClientOrg=
											(int)((float)rc.bottom*(float)Testsize/100.0);
								}
								else
								{
									cxClient=cxC=cxClientOrg=rc.right;
									cyClient=cyC=cyClientOrg=rc.bottom;
								}
								breedte=cxClient/stroken;
								hoogte=cyClient+2;
								EndDialog (hDlg, wParam) ;
								return TRUE ;
				 }
	  }
	  lParam=lParam;
	  return FALSE ;
	  }


void DoCaption (HWND hwnd, char *szFileName)
	  {
	  char szCaption [256] ;

	  wsprintf (szCaption, "%s   (%ld Kb free memory) Release:%9s",
			 (LPSTR) (szFileName [0] ? szFileName : szUntitled),GetFreeSpace(0)/1000,DATUM) ;
//	  strcat(szCaption,"   Disk name: ");
//	  selected_files[254][0]=0;
//	  drive=getdisk()+1;
//	  Read_Diskvolume(drive,selected_files[254]);
//	  strcat(szCaption,selected_files[254]);
	  SetWindowText (hwnd, szCaption) ;
	  }

short AskAboutQuit (HWND hwnd, char *szFileName)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;
	  szFileName=szFileName;

	  wsprintf (szBuffer, "Quit 3dview ?");
		nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_YESNO | MB_ICONQUESTION);
	  return nReturn ;
	  }
/*
short Out_of_Memory (HWND hwnd)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;

	  wsprintf (szBuffer, "Out of Memory");
	  nReturn = MessageBox (hwnd, szBuffer, szAppName,MB_OK | MB_ICONSTOP);
	  SendMessage(hwnd,WM_CLOSE,NULL,NULL);
	  return nReturn ;
	  }
*/
LRESULT CALLBACK WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  static BOOL    bNeedSave = FALSE ;
	  static FARPROC lpfnAboutDlgProc;//,lpfnInputDlgProc;
	  static HANDLE  hInstKnop,hInstgraph,hInstAbout;
	  static FARPROC lpfnKnop;
	  char           szFileName[256];
//	  int            n;
//	  unsigned		  startupdrive,noofdrives;
	  WORD 			  WMpaint=TRUE;
	  HCURSOR hcurSave;
	  switch (message)
	  {
	  case WM_CREATE:
			 hInstKnop = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnKnop = MakeProcInstance (KnoppenProc, hInstKnop) ;
			 hInstAbout = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnAboutDlgProc = MakeProcInstance (AboutDlgProc, hInstAbout) ;
			 hInstgraph  = ((LPCREATESTRUCT) lParam)->hInstance ;
			 Pic_output_file[0]=0;
			 GetClientRect(GetDesktopWindow(),&rc);
			 cxClient=cxC=cxClientOrg=rc.right;
			 cyClient=cyC=cyClientOrg=rc.bottom;
			 breedte=cxClient/stroken;
			 hoogte=cyClient+2;

			 lf.lfHeight=21;
			 lf.lfWidth=0;
			 lf.lfWeight=FW_BOLD;
			 lf.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;//variable
			 strcpy(lf.lfFaceName,"Times New Roman");

			 memset(&cf, 0, sizeof(CHOOSEFONT));
			 cf.lStructSize = sizeof(CHOOSEFONT);
			 cf.hwndOwner = hwnd;
			 cf.lpLogFont = &lf;
//			 cf.hDC=pd.hDC;
			 cf.iPointSize=18;
			 cf.Flags = CF_EFFECTS|CF_PRINTERFONTS|CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS;
			 cf.rgbColors = RGB(0, 0, 0);
			 cf.nFontType = PRINTER_FONTTYPE|SCREEN_FONTTYPE;
			 FontGekozen=FALSE;
			 UpdateWindow(hwnd);
			 DoCaption (hwnd, szFileName) ;
			 getcwd(workingdir, MAXPATH);
			 strupr(workingdir);
			 PostMessage(hwnd,WM_menu,NULL,NULL);
			 return 0 ;

/*	  case WM_SIZE:
			 cxClientOrg=cxClient= LOWORD(lParam)-4;
			 cyClientOrg=cyClient= HIWORD(lParam)-4;
			 cxC= LOWORD(lParam);//voor bitmap
			 cyC= HIWORD(lParam);
			 breedte=cxClient/stroken;
			 hoogte=cyClient;
			 return 0 ;
*/
	  case WM_SETFOCUS:
			 SetFocus(hwnd);
			 if(EditorActive) {EditorActive=FALSE; PostMessage(hwnd,WM_menu,NULL,NULL);}
			 return 0;

	  case WM_PAINT :
			 if(!PICOPEN && WMpaint)
			  {
				BeginPaint(hwnd,&ps);
				hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
				ShowWindow (hwnd, SW_SHOWMAXIMIZED) ;
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
			 case IDM_DRIVEA: if(!chdir("a:\\"))setdisk(0);	PostMessage(hwnd,WM_menu,NULL,NULL); return 0 ;
			 case IDM_DRIVEB: if(!chdir("b:\\"))setdisk(1);	PostMessage(hwnd,WM_menu,NULL,NULL); return 0 ;

			 case IDC_BMPS:
								BkgdBMPloaded=SourceBMPloaded=FALSE;
								LoadBackgroundBitmap(hwnd);
								if(BkgdBMPloaded)
									{
									LoadSourceBitmap(hwnd);
									if(SourceBMPloaded)	DoGraphDrawOpenDlg(hInstgraph,hwnd);
									}
								DeAllocP();
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;
			 case IDM_ABOUTP:
								DialogBox (hInstAbout, "AboutBox", hwnd,lpfnAboutDlgProc) ;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;
			 case IDM_CLOSE:
								SendMessage(hwnd,WM_CLOSE,NULL,NULL);
								return 0;
			 case IDM_FONT:
								ChooseFont(&cf);
								FontGekozen=TRUE;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;

			 case IDM_VIEW:
								hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
								BkgdBMPloaded=SourceBMPloaded=FALSE;
								DeAllocP();
								AllocP((int)stroken,(int)breedte,(int)hoogte);
								lost=FALSE;
								randomize();
								{
								int n,t1,t2;
								EdCOLORREF gpixel;
								for(t1=0;t1<breedte;t1++) //intinialiseer random achtergrond in kleur
									for(t2=0;t2<hoogte;t2++)
										{
										gpixel=(EdCOLORREF)dwColor[random(WHITE+1)];
										for(n=0;n<stroken;n++)		pokeP(n,t1,t2,gpixel);
										}
								}
								SetCursor(hcurSave);
								LoadSourceBitmap(hwnd);
								DoGraphDrawOpenDlg(hInstgraph,hwnd);
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0;
			case WM_KEYDOWN:
								switch (wParam)
									{
									case VK_RETURN: return 0;
									}
			 default:
								MessageBeep(0);
								SendMessage(hwnd,WM_CLOSE,0L,0L);
								return 0;
			 }

  case WM_CLOSE:
					 if (IDYES== AskAboutQuit (hwnd, szRealFileName))
								DestroyWindow (hwnd) ;
					 else PostMessage(hwnd,WM_menu,NULL,NULL);
					 return 0 ;

  case WM_QUERYENDSESSION:
					 FontGekozen=FALSE;
					 if (!bNeedSave || IDCANCEL != AskAboutQuit (hwnd, szRealFileName))
					 return 1L ;

  case WM_DESTROY:
					 if (hDLL) FreeLibrary (hDLL);
					 setdisk(workingdir[0]-'A');
					 chdir(workingdir);
					 DeAllocP();
					 PostQuitMessage (0) ;
					 return 0 ;
  }
	  return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//-------------------------------------------------------------------------------------------
void LoadSourceBitmap(HWND hwnd)
{
HCURSOR hcurSave;
int n,t1,t2;
EdCOLORREF gpixel;
HDC hdc;

	do_open(hwnd,"Select source BMP");
	if(strlen(bmpfile_name)<1)  return;
	hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
	do_paint_source(hwnd);
	hdc=GetDC(hwnd);
	for(n=0;n<stroken-1;n++)
		{
		int posx,kolrechts;

		if(!Knip)
			{
			for(t1=0;t1<breedte;t1++)
				for(t2=0;t2<hoogte;t2++)
					pokeP(n+1,t1,(int)t2,peekP(n,t1,t2));
			}
		for(t1=0;t1<breedte;t1++)
			for(t2=0;t2<hoogte;t2++)
				{
				 gpixel=(EdCOLORREF)GetPixel(hdc,(int)(n*breedte+t1),(int)t2);
				 if(gpixel!=(EdCOLORREF)dwColor[WHITE])
					{
					posx=t1+diepte;    kolrechts=1;
					if(posx>breedte)	{ posx-=(int)breedte; kolrechts=2;}
					if(posx<0)			{ posx+=(int)breedte; kolrechts=0;}
					pokeP(n+kolrechts,posx,(int)t2,peekP(n,t1,t2));
//					SetPixel(hdc,(int)(n*breedte+t1),(int)t2,(EdCOLORREF)dwColor[GREEN]);
					}
				SetPixel(hdc,(int)(n*breedte+t1),(int)5,(COLORREF)dwColor[GREEN]);
				}
		 }
	SourceBMPloaded=TRUE;
	ReleaseDC(hwnd,hdc);
	SetCursor(hcurSave);
}

//---------------------------------------------------------------------
void LoadBackgroundBitmap(HWND hwnd)
{
HCURSOR hcurSave;
EdCOLORREF gpixel;
int n,t1,t2;
HDC hdc;
	hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
	DeAllocP();
	AllocP((int)stroken,(int)breedte,(int)hoogte);
	SetCursor(hcurSave);
	do_open(hwnd,"Select Background BMP");
	if(strlen(bmpfile_name)<1)  return;
	hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
	do_paint_background(hwnd);
	hdc=GetDC(hwnd);
	for(t1=0;t1<breedte;t1++)
		for(t2=0;t2<hoogte;t2++)
			{
			gpixel=(EdCOLORREF)GetPixel(hdc,(int)t1+1,(int)t2);
			for(n=0;n<stroken;n++)	pokeP(n,(int)t1,(int)t2,gpixel);
			SetPixel(hdc,(int)(breedte+t1),(int)t2,peekP(0,(int)t1,(int)t2));
			}
	SetCursor(hcurSave);
	ReleaseDC(hwnd,hdc);
	BkgdBMPloaded=TRUE;
}


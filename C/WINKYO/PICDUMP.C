/*---------------------------------------
	picdump.c
	 (c) Ed Nieuwenhuys, 1994
  ---------------------------------------*/

#include "picdump.h"
#include "commdlg.h"
#include "direct.h"
#include "shellapi.h"

#define  EDITID 1
#define  WM_menu 67
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define DATUM __DATE__

extern void DoSelectOpenDlg(HANDLE hInst,HWND hwnd);
extern PRINTDLG pd;

int    AUTOMENU = FALSE;
char   szAppName  [] = "Picdump" ;
int	 test_selection[256];
char   szUntitled [] = "" ;
char   Resoutputfile[40]="";
short  cxClient,cyClient;
short  cxC,cyC;
short  cxChar,cxCaps,cyChar;
int    print,lost,berekend;
int 	 noofgr1,noofgr2,noofgr6;
short  noofgrpage,Page_orientation;
int	 Numgraph=0;
int    PICOPEN;
int    deleted;
int 	 LastGrButton=IDM_NOOFGR1;
int 	 LastLTButton=IDM_LINETHICKNESS2;
int	 LastOrButton=IDM_PORTRAIT;
short  FontGekozen;
short	 cxCharPrn, cyCharPrn, cxCharScreen, cyCharScreen; // maten voor default font
short  cxClientOrg;
short  cyClientOrg; //correctiefont in dmpicplib.c
char 	 szRealFileName[MAXSTRLEN] ;
int  	 geladen=0;
int  	 wis=1;
int  	 HAND;
int  	 Locked=FALSE;
char 	 *Tempfile = "PICDUMP.TMP";
char 	 Editor[40]= "WRITE.EXE";
char 	 *Adjustment_file="PICDUMP.SET";
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
/****************************  FUNCTION PROTOTYPES ***************/
int PASCAL 			WinMain (HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow);
BOOL FAR PASCAL 	AboutDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL FAR PASCAL 	KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 					DoCaption (HWND hwnd, char *szFileName);
short 				AskAboutQuit (HWND hwnd, char *szFileName);
short 				Out_of_Memory (HWND hwnd);
int 					set_lowest_drive(void);
LRESULT CALLBACK 	WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);
void   				Display_Bitmap(HANDLE,HANDLE);
extern BOOL 		CMUFileOpen( HWND);
extern BOOL 		PrintMyPage(HWND) ;
extern char 		Loaded_file_name[MAXSTRLEN];
char 					Pic_output_file[MAXSTRLEN];
int   				DrawPicFile=FALSE;			//Schrijf PIC file naar disk
int   				DrawGraphScreen=TRUE;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D
short 				Make_Last_Adjustment_file(HWND hwnd);
short 				Read_Last_Adjustment_file(HWND hwnd);
char 					workingdir[MAXPATH];

/************************************************************/

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdLine, int nCmdShow)
{
MSG      msg;
HWND     hwnd ;
WNDCLASS wndclass ;
hDLL = LoadLibrary ("BWCC.DLL");


print    = FALSE;
berekend = FALSE;
PICOPEN  = FALSE;
noofgr1	= TRUE;
noofgr2	= FALSE;
noofgr6  = FALSE;
Locked	= FALSE;

	  if (!hPrevInstance)
	  {
	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc   = (WNDPROC)WndProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = hInstance ;
	  wndclass.hIcon         = LoadIcon (hInstance, "Wcorr") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = szAppName ;
	  RegisterClass (&wndclass) ;
	  }
	  nCmdShow  = SW_SHOWMINIMIZED;//NORMAL;//MAXIMIZED;

	  hwnd = CreateWindow (szAppName,"PicDump",
			  WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  NULL, NULL, hInstance, NULL) ;
	  SetWindowText (hwnd, "PICDUMP") ;
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

BOOL FAR PASCAL InputDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  switch (message)
	  {
	  case WM_INITDIALOG:
							SetDlgItemText(hwnd,IDM_INPUT,selected_files[254]);	return TRUE ;
	  case WM_COMMAND:
			 switch (wParam)
			 {
			 unsigned drive;
			 case IDOK:
							GetDlgItemText(hwnd,IDM_INPUT,selected_files[254],11);
							if(strlen(selected_files[254])>0)
							 {
							  _dos_getdrive(&drive);
								Set_Diskvolume(hwnd,drive,selected_files[254]);
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

BOOL FAR PASCAL KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  switch (message)
	  {
		case WM_INITDIALOG:
			CheckRadioButton(hDlg,IDM_NOOFGR1,IDM_NOOFGR12,LastGrButton);
			CheckRadioButton(hDlg,IDM_PORTRAIT,IDM_LANDSCAPE,LastOrButton);
			CheckRadioButton(hDlg,IDM_LINETHICKNESS1,IDM_LINETHICKNESS4,LastLTButton);
			SetDlgItemText(hDlg,IDM_RESFILE,Editor);
					 return TRUE ;
		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
			case IDM_NOOFGR1 :
			case IDM_NOOFGR2 :
			case IDM_NOOFGR6 :
			case IDM_NOOFGR8 :
			case IDM_NOOFGR9 :
			case IDM_NOOFGR12 :
								CheckRadioButton(hDlg,IDM_NOOFGR1,IDM_NOOFGR12,wParam);
								noofgrpage=wParam-600;
								if(noofgrpage>100) noofgrpage-=100;
								LastGrButton=wParam;
								return 0 ;
			case IDM_LINETHICKNESS1:
			case IDM_LINETHICKNESS2:
			case IDM_LINETHICKNESS3:
			case IDM_LINETHICKNESS4:
								CheckRadioButton(hDlg,IDM_LINETHICKNESS1,IDM_LINETHICKNESS4,wParam);
								LastLTButton=wParam;
								// dikte defined in headerfile + 200 wordt gebruikt in readpic.c
								return 0;
			case IDM_PORTRAIT:
			case IDM_LANDSCAPE:
								CheckRadioButton(hDlg,IDM_PORTRAIT,IDM_LANDSCAPE,wParam);
								LastOrButton=wParam;
								return 0;

			case  IDM_RESFILE:
								GetDlgItemText(hDlg,IDM_RESFILE,Editor,40);
								return 0;
			 default:
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
	  int drive;

	  wsprintf (szCaption, "%s   (%ld Kb free memory) Release:%9s",
			 (LPSTR) (szFileName [0] ? szFileName : szUntitled),GetFreeSpace(0)/1000,DATUM) ;
	  strcat(szCaption,"   Disk name: ");
	  selected_files[254][0]=0;
	  drive=getdisk()+1;
	  Read_Diskvolume(drive,selected_files[254]);
	  strcat(szCaption,selected_files[254]);
	  SetWindowText (hwnd, szCaption) ;
	  }

short AskAboutQuit (HWND hwnd, char *szFileName)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;
	  szFileName=szFileName;

	  wsprintf (szBuffer, "Quit Picdump ?");
		nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_YESNO | MB_ICONQUESTION);
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
	  static FARPROC lpfnAboutDlgProc,lpfnInputDlgProc;
	  static HANDLE  hInstKnop,hInstgraph,hInstSelect,hInstAbout,hInstInput;
	  static FARPROC lpfnKnop;
	  char           szFileName[256],text[80] ;
	  int            n;
//	  unsigned		  startupdrive,noofdrives;
	  WORD 			  WMpaint=TRUE;
	  HCURSOR hcurSave;

	  switch (message)
	  {
	  case WM_CREATE:

			 hInstKnop = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnKnop = MakeProcInstance (KnoppenProc, hInstKnop) ;
			 hInstBitmap = ((LPCREATESTRUCT) lParam)->hInstance ;
			 hInstAbout = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnAboutDlgProc = MakeProcInstance (AboutDlgProc, hInstAbout) ;
			 hInstInput = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnInputDlgProc = MakeProcInstance (InputDlgProc, hInstInput) ;
			 hInstgraph  = ((LPCREATESTRUCT) lParam)->hInstance ;
			 hInstSelect = ((LPCREATESTRUCT) lParam)->hInstance ;
			 Pic_output_file[0]=0;

			 lf.lfHeight=21;
			 lf.lfWidth=0;
			 lf.lfWeight=FW_BOLD;
//			 lf.lfCharSet = ANSI_CHARSET;
//			 lf.lfOutPrecision= OUT_STRING_PRECIS;//OUT_DEFAULT_PRECIS;
//			 lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//			 lf.lfQuality = PROOF_QUALITY;
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
			 FontGekozen=FALSE;
			 UpdateWindow(hwnd);
			 DoCaption (hwnd, szFileName) ;
			 Read_Last_Adjustment_file(hwnd);
			 //			 Set_lowest_drive(&startupdrive);
			 getcwd(workingdir, MAXPATH);
			 strupr(workingdir);
			 cxClientOrg = GetDeviceCaps (GetDC(hwnd), HORZRES);
			 cyClientOrg = GetDeviceCaps (GetDC(hwnd), VERTRES);
			 PostMessage(hwnd,WM_menu,NULL,NULL);
			 return 0 ;

	  case WM_SIZE:
//			 cxClientOrg=cxClient= LOWORD(lParam);
//			 cyClientOrg=cyClient= HIWORD(lParam);
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 cxC= LOWORD(lParam);//voor bitmap
			 cyC= HIWORD(lParam);
			 return 0 ;

	  case WM_SETFOCUS:
			 SetFocus(hwnd);
			 if(EditorActive) {EditorActive=FALSE; PostMessage(hwnd,WM_menu,NULL,NULL);}
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
			 switch (wParam)
			 {
			 case IDM_DRIVEA: if(!chdir("a:\\"))setdisk(0);	PostMessage(hwnd,WM_menu,NULL,NULL); return 0 ;
			 case IDM_DRIVEB: if(!chdir("b:\\"))setdisk(1);	PostMessage(hwnd,WM_menu,NULL,NULL); return 0 ;
			 case IDM_PRINT:
								hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
								if(geladen)	PrintMyPage(hwnd);
								else  MessageBox (hwnd, "No File(s) Selected", "Printer ",MB_OK | MB_ICONHAND);
								SetCursor(hcurSave);
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;
			 case IDM_ABOUTP:
								DialogBox (hInstAbout, "AboutBox", hwnd,lpfnAboutDlgProc) ;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;
			 case IDM_VOLUMENAAM:
								DialogBox (hInstInput, "INPUTDIALOG", hwnd,lpfnInputDlgProc) ;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;
			 case IDM_CLOSE:
								SendMessage(hwnd,WM_CLOSE,NULL,NULL);
								return 0;
			 case IDM_DRIVEDIR:
								noofpicfiles=0;
								for(n=0;n<256;n++) test_selection[n]=0;
								if(CMUFileOpen(hwnd))
								 {
								 if(Loaded_file_name[0]!=0)
								  {
									char drive[MAXDRIVE];
									char dir[MAXDIR];
									char file[MAXFILE];
									char ext[MAXEXT];
									int flags;
									flags=fnsplit(Loaded_file_name,drive,dir,file,ext);
									sprintf(Loaded_file_name,"%s%s",file,ext);
									lstrcpy (szRealFileName,Loaded_file_name) ;
									lstrcpy (selected_files[0],Loaded_file_name) ;
									DoCaption (hwnd, szRealFileName) ;
									bNeedSave = FALSE ;
									geladen = 1;
									noofpicfiles=1;
									LastGrButton=IDM_NOOFGR1;
									noofgrpage=1;
									flags=flags;
								  }
								 }
								DoCaption (hwnd, szFileName) ;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return(0);

			 case IDM_FONT:
								ChooseFont(&cf);
								FontGekozen=TRUE;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0 ;

			 case IDM_VIEW:
								 lost=FALSE;
								 if(TRUE)
								  {
									char drive[MAXDRIVE];
									char dir[MAXDIR];
									char file[MAXFILE];
									char ext[MAXEXT];
									int flags;
									flags=fnsplit(selected_files[test_selection[0]],drive,dir,file,ext);

								  if(strcmp(ext,".BMP")==0 || strcmp(ext,".RLE")==0)
										DoGraphDrawOpenDlg(hInstgraph,hwnd);  //print pics op scherm
								  else if(strcmp(ext,".PIC")==0 || strcmp(ext,".EMF")==0 || strcmp(ext,".WMF")==0)
									{
									 for(n=0;n<noofpicfiles;n++)
									  {
										strcpy(selected_files[255],selected_files[test_selection[n]]);
										DoGraphDrawOpenDlg(hInstgraph,hwnd);  //print pics op scherm
										if(lost) break;
									  }
									}
								  }
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0;
			case IDM_PIC2PRN:
								hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
								if(pic2prn(hwnd,selected_files[test_selection[0]],"PIC.TXT"))
								 {
									WMpaint=FALSE;
									hedit=ShellExecute(hwnd,"open",Editor,"PIC.TXT",NULL,SW_SHOWMAXIMIZED);
//									hedit=ShellExecute(hwnd,"open",Editor,"PIC.TXT","c:\windows",SW_SHOWMAXIMIZED);
									if (hedit < 32)
										{
										sprintf(text, "Exec failed; error code = %d", hedit);
										MessageBox(hwnd, text, "Error", MB_ICONSTOP);
										PostMessage(hwnd,WM_menu,NULL,NULL);
										}
									BringWindowToTop(hedit);
									EditorActive=TRUE;
								 }
								SetCursor(hcurSave);
								WMpaint=TRUE;
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0;
			 case IDM_PRN2PIC:
								hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
								if(prn2pic(hwnd,"PIC.TXT","PIC.PIC"))
										strcpy(selected_files[test_selection[0]],"PIC.PIC");
								SetCursor(hcurSave);
								PostMessage(hwnd,WM_menu,NULL,NULL);
								return 0;
			 case IDM_SELECT:
								noofpicfiles=0;
								for(n=0;n<256;n++) test_selection[n]=0;
								 szRealFileName[0]=0;
								 DoSelectOpenDlg(hInstSelect,hwnd);
								 if(noofpicfiles) geladen=TRUE;
								 if(noofpicfiles==1) {LastGrButton=IDM_NOOFGR1; noofgrpage=1;}
								 if(noofpicfiles==2) {LastGrButton=IDM_NOOFGR2; noofgrpage=2;}
								 if(noofpicfiles >2) {LastGrButton=IDM_NOOFGR6; noofgrpage=6;}
								 if(noofpicfiles >6) {LastGrButton=IDM_NOOFGR12; noofgrpage=12;}
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
//					 _dos_setdrive(startupdrive,&noofdrives);
					 setdisk(workingdir[0]-'A');
					 chdir(workingdir);
					 Make_Last_Adjustment_file(hwnd);
					 PostQuitMessage (0) ;
					 return 0 ;
  }
	  return DefWindowProc (hwnd, message, wParam, lParam) ;
	  }
//-------------------------------------------------------------------------------------------
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap)
{
}
/*
 HDC 	    hdcMemory, hdcbm;
 HBITMAP    hbmpMyBitmap, hbmpOld;
 BITMAP     bm;
 float ratio;

 int Xw,Yw;
	hbmpMyBitmap = LoadBitmap(hInstBitmap, "WCORRBMP");
	GetObject(hbmpMyBitmap, sizeof(BITMAP), &bm);
	hdcbm = GetDC(hwndbm);
	hdcMemory = CreateCompatibleDC(hdcbm);
	hbmpOld = SelectObject(hdcMemory, hbmpMyBitmap);
	ratio = (float)cyC / (float)bm.bmHeight;
	Xw = (int)(ratio * bm.bmWidth);
	Yw = (int)(ratio * bm.bmHeight);

// BitBlt(hdcbm, 0, 0, 1.5*bm.bmWidth, 1.5*bm.bmHeight, hdcMemory, 0, 0, SRCCOPY);
//	StretchBlt(hdcbm, 0, 0, cxC,cyC, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
	StretchBlt(hdcbm, 0, 0, Xw,Yw, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
	SelectObject(hdcMemory, hbmpOld);
	DeleteDC(hdcMemory);
	ReleaseDC(hwndbm, hdcbm);
	DeleteObject(hbmpMyBitmap);

  return;

 }
 */
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
				fprintf(stream,"%d,lfWeight\r\n",FW_BOLD);
				fprintf(stream,"%s,lfFaceName\r\n",lf.lfFaceName);
				fprintf(stream,"%d,LINETHICKNESS\r\n",LastLTButton);
				fclose(stream);
			  }
return(TRUE);
}

short Read_Last_Adjustment_file(HWND hwnd)
 {
 FILE *stream;
 char text[42];
	 if((stream=fopen(Adjustment_file,"rt")) == NULL)
			Make_Last_Adjustment_file(hwnd);
	 else
		{
		fgets(text,40,stream);  strcpy(Editor,strtok(text,","));
		fgets(text,40,stream);  lf.lfHeight=atoi(strtok(text,","));
		fgets(text,40,stream);  lf.lfWeight=atoi(strtok(text,","));
		fgets(text,40,stream);  strcpy(lf.lfFaceName,strtok(text,","));
		fgets(text,40,stream);  LastLTButton=atoi(strtok(text,","));
		fclose(stream);
		}
return(TRUE);
}

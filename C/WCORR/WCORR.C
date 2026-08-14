
/*---------------------------------------
	WCORR.C
	 (c) Ed Nieuwenhuys, 1992
  ---------------------------------------*/
#include "wcorr.h"
#include "commdlg.h"
#include "shellapi.h"

#define  EDITID 1
#define  WM_menu 67
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define BELL 7
//#define MAXROWS 500          // maximale dimensies worksheet 200
//#define MAX_COL 100
//#define MAX_YK_PNT 50        // Maximaal 50 ijklijnpunten
#define MAX_LEN 40
#define MAX_LEN_NAAM 7        // Lengte van de naam column
#define MAX_SSITER 2500       // maximaal shoestrap iterations
#define DATUM __DATE__
extern void DoSelectOpenDlg(HANDLE hInst,HWND hwnd);
extern BOOL ViewerMain (HANDLE , HWND);
extern double VC,VCn;

long MAXROWS;
long MAX_COL;

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
int	   huge *test_selection;
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

char   szAppName  [] = "Correlatie EJN 09/98" ;
char   szUntitled [] = "(untitled)" ;
char   Resoutputfile[40]="";
short  cxClient,cyClient;
short  cxC,cyC;
short  cxChar,cxCaps,cyChar;
int    rows=0, columns=0;
int    labels,xx,yy,aa,rows_temp,kolom;
int    print,lost,LogXas,LogYas,LogXData,LogYData,berekend;
int	   EditorOutputParam;
int    Line_regr,Line_prob,Line_xy;
int	   Ticks,Switchax;
short  StopCalcAll,NoGraph;
int    Spearm,Xzero,Yzero;
int	   Numgraph,Autosave;
double aantal=0;
double slope,intercept;
double corr;
double somx,somy,upx,upy,maxP;
int    Shoestrap_iterations;
double Shoestrap_percentage;
int    PICOPEN,SHOESTRAP;
int    deleted;
double LAMBDA;
char   workingdir[MAXPATH];
long   Pstarted;

HANDLE 		hInst;
LOGFONT 	lf;
CHOOSEFONT 	cf;
TEXTMETRIC  tm;
HWND		hedit; //voor oproepen editor
FILE 		*fp;
FILE 		*fpin;
FILE 		*fpout;
FILE 		*fpout1;
char 		szRealFileName[MAXSTRLEN] ;
int  		geladen=0;
int  		wis=1;
int  		HAND;
int  		Locked=FALSE;
char 		*Tempfile = "RESCORR.TMP";
char 		Editor[40]= "WRITE.EXE";
char 		Adjustment_file[MAXPATH]="WCORR.ADJ";
/****************************  FUNCTION PROTOTYPES ***************/
int PASCAL 			WinMain (HANDLE hInstance, HANDLE hPrevInstance,LPSTR lpszCmdLine, int nCmdShow);
BOOL FAR PASCAL 	AboutDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL FAR PASCAL 	KnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 				DoCaption (HWND hwnd, char *szFileName);
short 				AskAboutQuit (HWND hwnd, char *szFileName);
short 				Out_of_Memory (HWND hwnd);
LRESULT CALLBACK 	WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);
HANDLE hDLL;

int 				sort_function( const void *a, const void *b);
int 				Open_result_file(HWND hwnd);
void				Print_results_infile(HWND hwnd,int i, int j);
void   				Display_Bitmap(HANDLE,HANDLE);
void   				read_wks(char *);
void   				skip(FILE *,long );
double 				getd(FILE *);
void   				mallocdata(void);
void   				freemallocdata(void);
void huge ** 		Alloceer_data(long Kolom, long Rij,HGLOBAL *Handle, int Sizevan);
void 					DeAlloceer_data(long Kolom,HGLOBAL *hglb);
short 				Make_Last_Adjustment_file(HWND hwnd);
short 				Read_Last_Adjustment_file(HWND hwnd);

extern int  		calc(HANDLE,HWND,int , int );
extern BOOL 		CMUFileOpen( HWND);
extern BOOL 		PrintMyPage(HWND) ;
extern char 		Loaded_file_name[MAXSTRLEN];
char 					Pic_output_file[MAXSTRLEN];
int   				DrawPicFile=FALSE;			//Schrijf PIC file naar disk
int   				DrawGraphScreen=TRUE;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D

/************************************************************/

int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
			 LPSTR lpszCmdLine, int nCmdShow)
{
MSG      msg;
HWND     hwnd ;
WNDCLASS wndclass ;

hDLL 		= LoadLibrary ("BWCC.DLL");
lpszCmdLine	= lpszCmdLine;
lost     	= FALSE;
print    	= FALSE;
LogXData 	= FALSE; //Log data x-as
LogYData 	= FALSE; //Log data y-as
LogXas	    = FALSE; //Log x-as
LogYas   	= FALSE; //Log y-as
berekend 	= FALSE;
PICOPEN  	= FALSE;
Line_regr	= TRUE;
Line_prob	= FALSE;
Line_xy  	= TRUE;
Spearm		= FALSE;
Xzero		= FALSE;
Yzero 		= FALSE;
Numgraph 	= FALSE;
Autosave	= FALSE;
Switchax 	= FALSE;
StopCalcAll	= FALSE;
NoGraph		= FALSE;
Locked		= FALSE;
Ticks		= 0;     // =inside
LAMBDA		= 1.00;
upx=upy		= 1E6;
maxP		= 1.0;
Shoestrap_iterations = 200;
Shoestrap_percentage = 10;
EditorOutputParam	 = REGRESPARAM;

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
	  hInst = hInstance;
	  nCmdShow  = SW_SHOWMAXIMIZED;
	  hwnd = CreateWindow (szAppName, NULL,
			  WS_OVERLAPPED|WS_THICKFRAME|WS_SYSMENU|WS_CAPTION,
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
	  case WM_INITDIALOG:							return TRUE ;
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
  char text[80];
  lParam=lParam;
  switch (message)
	  {
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg,IDM_LINEREGR),BM_SETCHECK,Line_regr,0L);
			SendMessage(GetDlgItem(hDlg,IDM_LINEPROB),BM_SETCHECK,Line_prob,0L);
			SendMessage(GetDlgItem(hDlg,IDM_LINEXY),BM_SETCHECK,Line_xy,0L);
			SendMessage(GetDlgItem(hDlg,IDM_XLOGas),BM_SETCHECK,LogXas,0L);
			SendMessage(GetDlgItem(hDlg,IDM_YLOGas),BM_SETCHECK,LogYas,0L);
			SendMessage(GetDlgItem(hDlg,IDM_XLOGdata),BM_SETCHECK,LogXData,0L);
			SendMessage(GetDlgItem(hDlg,IDM_YLOGdata),BM_SETCHECK,LogYData,0L);
			SendMessage(GetDlgItem(hDlg,IDM_SPEARM),BM_SETCHECK,Spearm,0L);
			SendMessage(GetDlgItem(hDlg,IDM_XZERO),BM_SETCHECK,Xzero,0L);
			SendMessage(GetDlgItem(hDlg,IDM_YZERO),BM_SETCHECK,Yzero,0L);
			SendMessage(GetDlgItem(hDlg,IDM_NOGRAPH),BM_SETCHECK,NoGraph,0L);
			SendMessage(GetDlgItem(hDlg,IDM_AUTOSAVE),BM_SETCHECK,Autosave,0L);
			SendMessage(GetDlgItem(hDlg,IDM_NUMGRAPH),BM_SETCHECK,Numgraph,0L);
			SendMessage(GetDlgItem(hDlg,IDM_axswitch),BM_SETCHECK,Switchax,0L);
			CheckRadioButton(hDlg,IDC_PAIRED,IDC_REGRESP,IDC_PAIRED+EditorOutputParam);
			CheckRadioButton(hDlg,IDM_Ticksin,IDM_Ticksout,Ticks?IDM_Ticksout:IDM_Ticksin);
			SetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file);
			SetDlgItemText(hDlg,IDM_RESFILE,Editor);
			sprintf(text,"%0.1f",Shoestrap_percentage);
			SetDlgItemText(hDlg,IDC_SSPERC,text);
			sprintf(text,"%d",Shoestrap_iterations);
			SetDlgItemText(hDlg,IDC_SSITER,text);
			sprintf(text,"%6.6G",upx);
			SetDlgItemText(hDlg,IDM_MAXX,text);
			sprintf(text,"%0.6G",upy);
			SetDlgItemText(hDlg,IDM_MAXY,text);
			sprintf(text,"%0.6G",maxP);
			SetDlgItemText(hDlg,IDM_MAXP,text);
			sprintf(text,"%0.6G",LAMBDA);
			SetDlgItemText(hDlg,IDM_LAMBDA,text);
			return TRUE ;

		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
			case IDM_LINEREGR :
								Line_regr=1-Line_regr;
								return 0 ;
			case IDM_LINEPROB :
								Line_prob=1-Line_prob;
								return 0 ;
			case IDM_LINEXY   :
								Line_xy=1-Line_xy;
								return 0 ;
			 case IDM_XLOGas:
								LogXas = 1-LogXas;
								return 0 ;
			 case IDM_YLOGas:
								LogYas=1-LogYas;
				 				return 0 ;
			 case IDM_XLOGdata:
								LogXData=1-LogXData;
								return 0 ;
			 case IDM_YLOGdata:
								LogYData=1-LogYData;
				 				return 0 ;
			 case IDM_SPEARM:
								Spearm=1-Spearm;
				 				return 0 ;
			 case IDM_XZERO:
								Xzero=1-Xzero;
								return 0 ;
			 case IDM_YZERO:
								Yzero=1-Yzero;
								return 0 ;
			 case IDM_NOGRAPH:
								NoGraph=1-NoGraph;
								return 0 ;
			 case IDC_PAIRED:
			 case IDC_NONPAIRED:
			 case IDC_REGRESP:
								CheckRadioButton(hDlg,IDC_PAIRED,IDC_REGRESP,wParam);
								EditorOutputParam=wParam-IDC_PAIRED;
								return 0;
			 case IDM_Ticksin:
			 case IDM_Ticksout:
								CheckRadioButton(hDlg,IDM_Ticksin,IDM_Ticksout,wParam);
								Ticks= (BYTE)(wParam==IDM_Ticksin?0:2);
								return 0 ;
			 case IDM_axswitch:
								Switchax=1-Switchax;
								return 0;
			 case  IDM_PICFILE:
								GetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file,40);
								return 0;
			 case  IDM_RESFILE:
								GetDlgItemText(hDlg,IDM_RESFILE,Editor,40);
								return 0;
			 case  IDC_SSITER:
								GetDlgItemText(hDlg,IDC_SSITER,text,20);
								Shoestrap_iterations=atoi(text);
								if(Shoestrap_iterations>MAX_SSITER)
									Shoestrap_iterations=MAX_SSITER;
								if(Shoestrap_iterations<21)
									Shoestrap_iterations=21;
								return 0;
			 case  IDC_SSPERC:
								GetDlgItemText(hDlg,IDC_SSPERC,text,20);
								Shoestrap_percentage=atof(text);
								return 0;
			 case  IDM_MAXX:
								GetDlgItemText(hDlg,IDM_MAXX,text,20);
								upx=atof(text);
								return 0;
			 case  IDM_MAXY:
								GetDlgItemText(hDlg,IDM_MAXY,text,20);
								upy=atof(text);
								return 0;
			 case  IDM_MAXP:
								GetDlgItemText(hDlg,IDM_MAXP,text,20);
								maxP=atof(text);
								return 0;
			 case  IDM_LAMBDA:
								GetDlgItemText(hDlg,IDM_LAMBDA,text,20);
								LAMBDA=atof(text);
								if(LAMBDA<=0) LAMBDA=1E-10;
								return 0;
			 case  IDM_AUTOSAVE:
								Autosave=1-Autosave;
								if(!Autosave)
								{
								 Pic_output_file[0]=0;
								 SetDlgItemText(hDlg,IDM_PICFILE,Pic_output_file);
                        }
								return 0;
			 case  IDM_NUMGRAPH:
								Numgraph=1-Numgraph;
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

	  wsprintf (szCaption, "File:%s Version:%s  S%ld",
			 (LPSTR) (szFileName [0] ? szFileName : szUntitled), DATUM,Pstarted) ;
	  SetWindowText (hwnd, szCaption) ;
	  }

short AskAboutQuit (HWND hwnd, char *szFileName)
	  {
	  char  szBuffer [40] ;
	  short nReturn ;
	  szFileName=szFileName;

	  wsprintf (szBuffer, "Quit Correlatie ?");
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
	  nReturn = MessageBox (hwnd, szBuffer, szAppName,
					 MB_OK | MB_ICONSTOP);
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
 char 			szMsg[80];
 char			text[512];
 double			nop;
 int            i,j,k,n;
 WORD 			WMpaint=TRUE;
 HDC	    	hdc;
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
		 ReleaseDC (hwnd, hdc) ;
		 DoCaption (hwnd, szFileName) ;
		 Pic_output_file[0]=0;
		 Read_Last_Adjustment_file(hwnd);
		 getcwd(workingdir, MAXPATH);
		 strupr(workingdir);
		 Pstarted++;
		 UpdateWindow(hwnd);
		 PostMessage(hwnd,WM_menu,NULL,NULL);
		 return 0 ;

  case WM_SIZE:
		 cxClient= LOWORD(lParam);
		 cyClient= HIWORD(lParam);
		 cxC= LOWORD(lParam);//voor bitmap
		 cyC= HIWORD(lParam);
		 return 0 ;

  case WM_SETFOCUS:
		 return 0;

  case WM_PAINT :
		 if(!PICOPEN && WMpaint)
		  {
			BeginPaint(hwnd,&ps);
			hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
			Display_Bitmap(hwnd,hInstBitmap);
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
		 case IDM_LOAD:
				if((fpout=fopen(Tempfile,"wt")) == NULL)
				  {
					MessageBox (hwnd, "Can not open outputfile","Disk full?",MB_OK | MB_ICONQUESTION);
					PostMessage(hwnd,WM_menu,NULL,NULL);
					return 0;
				  }
				else
				  {
				  fprintf(fpout,"%s","Nothing calculated");
				  fclose(fpout);
				  }
				lost = FALSE;
				if(CMUFileOpen(hwnd))
					{
					if(Loaded_file_name[0]!=0)
						{
						if(Locked)freemallocdata();
						lstrcpy (szRealFileName,Loaded_file_name) ;
						lstrcpy (szFileName,Loaded_file_name) ;
						DoCaption (hwnd, szRealFileName) ;
						bNeedSave = FALSE ;
						if (strlen(szFileName) < 2) break;
						berekend = 0; 	rows_temp = 0;	geladen = 1;
						read_wks(szFileName);
						}
					}
				setdisk(workingdir[0]-'A');
				chdir(workingdir);
				PostMessage(hwnd,WM_menu,NULL,NULL);
				return(0);
		case IDM_BEREKEN:
				if(geladen)
					{
					if(Autosave && !Pic_output_file[0])
						{
						MessageBox (hwnd, "No PIC output drive/directory", szAppName,
													MB_OK | MB_ICONSTOP);
						PostMessage(hwnd,WM_menu,NULL,NULL);
						return 0 ;
						}
					 axcal.Graphtype=REGRESSION;
					 hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
					 if(test_selection[1]==0) DoSelectOpenDlg(hInstSelect,hwnd);
					 if(Switchax)
							calc(hInstgraph,hwnd,test_selection[1],test_selection[0]);
					 else
							calc(hInstgraph,hwnd,test_selection[0],test_selection[1]);
					 SetCursor(hcurSave);
					 }
				 else MessageBeep(1);
//           for(n=0;n<MAX_COL;n++) test_selection[n]=0;
				 PostMessage(hwnd,WM_menu,NULL,NULL);
				 return(0);
		case IDC_SHOESTRAP:
				 if(geladen)
					{
					int logx,logy;
					double huge *BSslope;
					double huge *BSintercept;
					double huge *BScorr;
					double huge *BSspear;
					HGLOBAL Hsl,Hin,Hco,Hspear,Hist;
               VC=VCn=0;

					Hsl=GlobalAlloc(GPTR,(MAX_SSITER+1) * sizeof(double));
					BSslope = (double huge*)GlobalLock(Hsl);
					Hin=GlobalAlloc(GPTR,(MAX_SSITER+1) * sizeof(double));
					BSintercept = (double huge*)GlobalLock(Hin);
					Hco=GlobalAlloc(GPTR,(MAX_SSITER+1) * sizeof(double));
					BScorr = (double huge*)GlobalLock(Hco);
					Hspear=GlobalAlloc(GPTR,(MAX_SSITER+1) * sizeof(double));
					BSspear = (double huge*)GlobalLock(Hspear);
					Hist=GlobalAlloc(GPTR,(MAX_HIS_SIZE+5) * sizeof(double));
					Histogram_dev = (double huge*)GlobalLock(Hist);

					DoSelectOpenDlg(hInstSelect,hwnd);
					if(test_selection[1]==0) break;
					if(!Open_result_file(hwnd)) break;
					axcal.Graphtype=REGRESSION;
					if(Switchax)
						calc(hInstgraph,hwnd,test_selection[1],test_selection[0]);
					else
						calc(hInstgraph,hwnd,test_selection[0],test_selection[1]);
					hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
					SHOESTRAP = TRUE;
					EditorOutputParam = REGRESPARAM;
					NoGraph = TRUE;
					i=0; j=1;
					logx=LogXas;
					logy=LogYas;
					LogXas=LogYas=0;
					Display_Bitmap(hwnd,hInstBitmap);
					for(k=0;k<Shoestrap_iterations;k++)
						{
						if(Switchax)
							calc(hInstgraph,hwnd,test_selection[j],test_selection[i]);
						else
							calc(hInstgraph,hwnd,test_selection[i],test_selection[j]);
						Print_results_infile(hwnd,i,j);
						sprintf(text,"Iterations to go: %d",Shoestrap_iterations-k);
						SetWindowText (hwnd, text) ;
						BSslope[k]=slope;
						BSintercept[k]=intercept;
						BScorr[k]=corr;
						BSspear[k]=rspearman;
						}

					qsort(BSslope, k, sizeof(BSslope[0]), sort_function);
					SetCursor(hcurSave);
					BSarray=BSslope;
					axcal.Y_Min=BSarray[0];
					axcal.Y_Max=BSarray[k-1];
					axcal.Noof_DataPoints=k-1;
					axcal.Graphtype=SS_SLOPE;
					DoGraphDrawOpenDlg (hInstgraph,hwnd);
					GlobalUnlock(Hsl); GlobalFree(Hsl);

					qsort(BSintercept, k, sizeof(BSintercept[0]), sort_function);
					BSarray=BSintercept;
//					axcal.Y_Min=BSarray[0];
//					axcal.Y_Max=BSarray[k-1];
					axcal.Noof_DataPoints=k-1;
					axcal.Graphtype=SS_INTERCEPT;
					DoGraphDrawOpenDlg (hInstgraph,hwnd);

					qsort(BScorr, k, sizeof(BScorr[0]), sort_function);
					BSarray=BScorr;
//					axcal.Y_Min=BSarray[0];
//					axcal.Y_Max=BSarray[k-1]/1.05;
					axcal.Noof_DataPoints=k-1;
					axcal.Graphtype=SS_CORR;
					DoGraphDrawOpenDlg (hInstgraph,hwnd);

					qsort(BSspear, k, sizeof(BSspear[0]), sort_function);
					BSarray=BSspear;
//					axcal.Y_Min=BSarray[0];
//					axcal.Y_Max=BSarray[k-1]/1.05;
					axcal.Noof_DataPoints=k-1;
					axcal.Graphtype=SS_SCORR;
					DoGraphDrawOpenDlg (hInstgraph,hwnd);
					fclose(fpout);
					GlobalUnlock(Hin); GlobalFree(Hin);
					GlobalUnlock(Hco); GlobalFree(Hco);
					GlobalUnlock(Hspear); GlobalFree(Hspear);
					LogXas=logx;
					LogYas=logy;

					SetCursor(hcurSave);
					BSarray=Histogram_dev;
//					axcal.Y_Min=0;
 //					axcal.Y_Max=800;
					axcal.Noof_DataPoints=MAX_HIS_SIZE;
					axcal.Graphtype=SS_HISTOGRAM;
					DoGraphDrawOpenDlg (hInstgraph,hwnd);
					GlobalUnlock(Hist); GlobalFree(Hist);

					}
				 axcal.Graphtype=REGRESSION;
				 SHOESTRAP=FALSE;
				 NoGraph = FALSE;
				 PostMessage(hwnd,WM_menu,NULL,NULL);
				 return(0);

		 case IDM_CALCALL:
				if(geladen)//&&AreYouSure (hwnd)==IDYES)
					{
					if(Autosave && !Pic_output_file[0])
						{
						MessageBox (hwnd, "No PIC output drive/directory", szAppName,
												MB_OK | MB_ICONSTOP);
						PostMessage(hwnd,WM_menu,NULL,NULL);
						return 0 ;
						}
					if(Autosave && NoGraph)
						{
						sprintf(text,"Depending on maxP & selection\n Max: %d Graphs. Are you sure?",
											(labels+1)*labels/2);
						i=MessageBox (hwnd, text, "Graph drawing can NOT be stopped",
											MB_YESNO | MB_ICONQUESTION);
						if(i==IDNO)
							{
							PostMessage(hwnd,WM_menu,NULL,NULL);
							return 0 ;
							}
						}
					 hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
					 axcal.Graphtype=REGRESSION;
					 StopCalcAll=FALSE;
					 if(!Open_result_file(hwnd)) break;

					 if(test_selection[1]==0)
						  for(i=0;i<=labels;i++) test_selection[i]=i;
					 for(n=0;n<MAX_COL-1;n++)
						  if(test_selection[n+1]==0) break;
					 for(i=0;i<=n;i++)
						for(j=i+1;j<=n;j++)
							{
							if(!StopCalcAll&&!Switchax)
									calc(hInstgraph,hwnd,test_selection[i],test_selection[j]);
							if(!StopCalcAll&& Switchax)
									calc(hInstgraph,hwnd,test_selection[j],test_selection[i]);
							if (Spearm)  nop = probrs;
							else			 nop = P;
							if (aantal>3 && nop <= maxP)
								Print_results_infile(hwnd,i,j);
							}
					 berekend=TRUE;
					 SetCursor(hcurSave);
					}
				 else MessageBeep(1);
				 fclose(fpout);
//				 for(n=0;n<MAX_COL;n++) test_selection[n]=0;
				 PostMessage(hwnd,WM_menu,NULL,NULL);
				 return 0 ;

		 case IDM_PRINT:
				if(geladen)	PrintMyPage(hwnd);
				else        MessageBeep(1);
				if(axcal.Graphtype==REGRESSION)
					if(test_selection[1]==0) DoSelectOpenDlg(hInstSelect,hwnd);
				else
				  {
				  if(Switchax)
						calc(hInstgraph,hwnd,test_selection[1],test_selection[0]);
				  else
						calc(hInstgraph,hwnd,test_selection[0],test_selection[1]);
				  }
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
				// Set all structure fields to zero.
/*		 		memset(&cf, 0, sizeof(CHOOSEFONT));

				cf.lStructSize = sizeof(CHOOSEFONT);
				cf.hwndOwner = hwnd;
				cf.lpLogFont = &lf;
				cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
				cf.rgbColors = RGB(0, 255, 255); // light blue
				cf.nFontType = SCREEN_FONTTYPE;
				ChooseFont(&cf);
				PostMessage(hwnd,WM_menu,NULL,NULL);
  */	 		return 0 ;

		 case IDM_SELECT:
				for(n=0;n<MAX_COL;n++) test_selection[n]=0;
				if(geladen)	 DoSelectOpenDlg(hInstSelect,hwnd);
				else 		  	 MessageBeep(1);
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
//			 	EditorActive=TRUE;
				BringWindowToTop(hedit);
				SetCursor(hcurSave);
				WMpaint=TRUE;
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
		 if((fpout=fopen(Tempfile,"wt")) == NULL)
				MessageBox (hwnd, "Can not open outputfile",szAppName,MB_OK | MB_ICONQUESTION);
		 else
			  {
				fprintf(fpout,"%s","Nothing calculated");
				fclose(fpout);
			  }
		 if (hDLL) FreeLibrary (hDLL);
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
	if( *((double*)a) - *((double*)b) <0 )  return -1;
	if( *((double*)a) - *((double*)b) ==0 ) return 0;
	return 1;
}
//---------------------------------------------------------------
 int Open_result_file(HWND hwnd)
 {
 char text[256];
 char datum[10];

	if((fpout=fopen(Tempfile,"wt")) == NULL)
	  {
		MessageBox (hwnd, "Can not open outputfile",szAppName,MB_OK | MB_ICONQUESTION);
		return FALSE;
	  }
	_strdate(datum);
	sprintf(text,"Printed: %s\nVersion: %s, Program:WCORR.EXE for Windows, Ed Nieuwenhuys",datum,DATUM);
	fprintf(fpout,"%s\n",text);
	sprintf(text,"Pearson correlation, R and significance");
	fprintf(fpout,"%s\n",text);
	sprintf(text,"Spearman rank correlation, R and tie corrected significance");
	fprintf(fpout,"%s\n",text);
	sprintf(text,"T-test for paired samples, two tailed significance");
	if(EditorOutputParam==PAIRED) fprintf(fpout,"%s\n",text);
	sprintf(text,"Wilcoxon matched pairs signed rank test, tie corrected, two tailed significance");
	if(EditorOutputParam==PAIRED)fprintf(fpout,"%s\n",text);
	sprintf(text,"T-test for unpaired samples, two tailed significance");
	if(EditorOutputParam==NONPAIRED)fprintf(fpout,"%s\n",text);
	sprintf(text,"Mann-Whitney U rank sum test, tie corrected, two tailed significance");
	if(EditorOutputParam==NONPAIRED)fprintf(fpout,"%s\n\n",text);

	switch (EditorOutputParam)
	 {
	  case PAIRED:
		  sprintf(text,
"                  Pearson          Spearman                                       Paired Ttest        Wilcoxon");
		  fprintf(fpout,"%s\n",text);
								  sprintf(text,
"          n      R        P       R         P        X-axis   <->   Y-axis          T        P        Z         P");
								  fprintf(fpout,"%s\n",text);
		  break;
	  case NONPAIRED:
		  sprintf(text,
"                  Pearson          Spearman                                       Unpaired Ttest     Mann-Whitney");
		  fprintf(fpout,"%s\n",text);
								  sprintf(text,
"          n      R        P       R         P        X-axis   <->   Y-axis          T         P      Z          P     nx    ny");
								  fprintf(fpout,"%s\n",text);
		  break;
	  case REGRESPARAM:
		  sprintf(text,
"                  Pearson          Spearman                                       slope    intercept");
		  fprintf(fpout,"%s\n",text);
								  sprintf(text,
"          n      R        P       R         P        X-axis   <->   Y-axis          a         b");
								  fprintf(fpout,"%s\n",text);
		  break;
	}
 return TRUE;
}
//-------------------------------------------------------------------
void Print_results_infile(HWND hwnd,int i, int j)
{
  char text[256];
  hwnd=hwnd;

	switch (EditorOutputParam)
	 {
	  case PAIRED:
		  sprintf(text,
"(%2.0d,%2.0d) %4.0f % 6.04f % 9.02G % 6.04f % 9.02G %12.11s <-> %-12.11s % 7.03f % 9.02G  % 6.03f % 9.02G",
				test_selection[i]+1,
				test_selection[j]+1,
				aantal,corr,P,rspearman,probrs,
				&label[test_selection[i]][1],
				&label[test_selection[j]][1],
				tpt,tpprob,zsrt,probsrt);
		  fprintf(fpout,"%s\n",text);
		  break;
	  case NONPAIRED:
		  sprintf(text,
"(%2.0d,%2.0d) %4.0f % 6.04f % 9.02G % 6.04f % 9.02G %12.11s <-> %-12.11s % 7.03f % 9.02G  % 6.03f % 9.02G  %4.0d  %4.0d",
				test_selection[i]+1,
				test_selection[j]+1,
				aantal,corr,P,rspearman,probrs,
				&label[test_selection[i]][1],
				&label[test_selection[j]][1],
				tut,tuprob,zmw,probsmw,
				nx,ny);
		  fprintf(fpout,"%s\n",text);
		  break;
	  case REGRESPARAM:
		  sprintf(text,
"(%2.0d,%2.0d) %4.0f % 6.04f % 9.02G % 6.04f % 9.02G %12.11s <-> %-12.11s % 9.04f % 9.04f",
				test_selection[i]+1,
				test_selection[j]+1,
				aantal,corr,P,rspearman,probrs,
				&label[test_selection[i]][1],
				&label[test_selection[j]][1],
				slope,intercept);
		  fprintf(fpout,"%s\n",text);
		  break;
	  }
}

//-------------------------------------------------------------------------------------------
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap)
{
 HDC 	    	hdcMemory, hdcbm;
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

int 	op, formula_size,file_type;
int 	column, row,  end_col, end_row;//,start_col,start_row;
char   	c,pos,format;
double 	doubleing,integer;
long 	count,body_length;

lost=FALSE;
count=0;
labels=0;

if((fp=fopen(infile,"rb")) == NULL)	 {MessageBeep(MB_ICONHAND); exit(1);    }
op=getw(fp);
while (op!=Eof)
{
 body_length=(long)getw(fp);
 if (body_length < 0) {  MessageBeep(MB_ICONHAND); exit(-1);  }
 count += body_length+4;

 switch(op)
  {
case INTEGER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 integer=(double)getw(fp);
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (row > rows) rows=row;
	 data[column][row]=integer;
	 break;
case NUMBER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 doubleing=getd(fp);
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
//		  if(row==0) data[column][row]=(double)MISSING+1+labels;
//		   else
			data[column][row]=(double)MISSING;
			labels=columns;
		 }
	 c=getc(fp);
	 while (c)
		{
		  if ((column < MAX_COL) && (pos<22) && (row==0))
			{
			label[column][pos++]=c;
			label[column][pos]=0;
			}
		  else if(row < MAX_ROW && pos<22 && column==0 && row>0)
			{
			naam[row][pos++]=c;
			naam[row][pos]=0;
			}
		  c=getc(fp);
		 }
//	 if ((column < MAX_COL) && row==0) label[column][pos]=0;
//	 if(row<MAX_ROW&& column==0) 		  naam[row][pos]=0;
	 break;
case BOF:
	 file_type= getw(fp);
	 break;
case DIMENSIONS:
	 //start_col=
	 getw(fp);
	 //start_row=
	 getw(fp);
	 end_col=getw(fp)+1;
	 end_row=getw(fp)+1;
	 MAXROWS= end_row;
	 MAX_COL = end_col;
	 MAX_ROW =(int) MAXROWS;
	 mallocdata();

	 rows =   (int) (end_row >= MAX_ROW ) ? MAX_ROW : end_row ;
	 columns =(int) (end_col >= MAX_COL)  ? MAX_COL : end_col ;

//	 columns=3;
	 for (row=0;row<=rows;row++)
	  {
		for (column=0;column<=columns;column++) 	 data[column][row]=(double)MISSING;
		strcpy((char*)naam[row]," Noname");
	  }
	 for (column=0;column<=columns;column++) 	 strcpy((char*)label[column]," Noname");
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
}

void skip(FILE *fp,long noofbytes)
{char c;
while (noofbytes>0)
   {
     c=getc(fp);
     noofbytes--;
	}
c=c;
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
void mallocdata(void)
 {
HGLOBAL hglb;

hglb = GlobalAlloc(GPTR,(MAX_ROW+2) * sizeof(void*));
hglb_naam = (HGLOBAL*)GlobalLock(hglb);
naam  = (char huge **)Alloceer_data(MAX_ROW+1,MAX_LEN_NAAM+1,hglb_naam, sizeof(char));

hglb = GlobalAlloc(GPTR,(MAX_COL+2) * sizeof(void*));
hglb_label = (HGLOBAL*)GlobalLock(hglb);
label = (char huge **)Alloceer_data(MAX_COL+1,MAX_LEN+1,hglb_label, sizeof(char));

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

hglb_test_selection = GlobalAlloc(GPTR,(MAX_COL+1) * sizeof(void*));
test_selection = (int huge*)GlobalLock(hglb_test_selection);
}
//---------------------------------------------------------------------------------------------
void freemallocdata(void)
{
  DeAlloceer_data(MAX_ROW+2,hglb_naam);
  DeAlloceer_data(MAX_COL+2,hglb_label);
  DeAlloceer_data(3,hglb_temp);
  DeAlloceer_data(MAX_COL+2,hglb_data);
//  DeAlloceer_data(MAX_COL+2,hglb_PP);

  GlobalUnlock(hglb_sortx);         		 GlobalFree(hglb_sortx);
  GlobalUnlock(hglb_sorty);         		 GlobalFree(hglb_sorty);
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
				fprintf(stream,"%ld,Programstarted\r\n",Pstarted);
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
		fgets(text,40,stream);  Pstarted=atol(strtok(text,","));
		fclose(stream);
		}
return(TRUE);
}

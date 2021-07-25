
// window logit uitrekenen wlorek.c  Ed Nieuwenhuys dec 1993

#include "wlogit.h"

#define NONE 0
#define DILUTION 1
#define MEAS 2

short    nVscrollPos,nPaintBeg, nPaintEnd, nVscrollInc, nHscrollInc ;
short    nMaxWidth, nVscrollMax, nHscrollPos, nHscrollMax, xCaret,yCaret=0 ;
/*
typedef struct {
			float input;
			float dilution;
			double result;
			double result_upper;
			int result_dev;
			char overflow;
			char monsternaam[MAX_LEN_NAAM];
			int pointer
			} RESULTS;
*/
RESULTS *Logit;
char 	text[180];
char	Dtext[20]; //windowtext in Dialogbox
char	Dlgtext[20]; //text in Dialogbox
char	Rtext[42];  // return text in Dialogbox
double	Dresult;   //resultaat van Dialogbox
char	Experimentname[42];
int 	tel=0,lastyk;
short   maxregel=0,startregel,lastregel=1;  //voor caret
short	INPUT=FALSE,Foerin=FALSE,Correction=FALSE;
short	DilStartPos=13,DilEndPos=27,MeasStartPos=29,MeasEndPos=46; //posities cols voor invoer
short   YesNo=0,YesNoTemp,Nop;
double 	Conc=1000,Dilfactor=2;
int    	Replicas=1;
double	Dilution;
char	YN,ConstantDil;

extern 	RECT  recttext;		//Rectangle for drawtext
extern	char  Loaded_file_name[MAXSTRLEN];
extern	short Yklijninvoer;

FARPROC lpfnCalcResult;
HANDLE 	hInstCalcResult;
HGDIOBJ	Hfont;
HANDLE  hInstDlgInput;
FARPROC lpfnDlgInput;
FARPROC lpfnIKnop;
HANDLE 	hInstIKnop;

//--------------------------- function prototypes -----------------------------------------
void 	print_regel(HDC hdc,int i);
void 	print_regel_logit(HDC hdc,int i);
void 	print_regel_yklijn(HDC hdc,int i);
int 	DoCalcResultDlg (HANDLE Inst, HWND hwndparent);
LRESULT CALLBACK CalcResultDlgProc (HWND, WORD, WORD, LONG);
BOOL 	FAR PASCAL CalcInputDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
BOOL 	FAR PASCAL DilKnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
void 	Voerin(HWND hwnd,int regel);
void 	Voerin_yklijn(HWND hwnd,int regel);
void 	Voerin_logit(HWND hwnd,int regel);
void   	Manual_input(HWND hwndEdit);

// ---------------------------- print_regel ----------------------------------------
void 	print_regel(HDC hdc,int i)
{
 if(Yklijninvoer) print_regel_yklijn(hdc,i);
 else             print_regel_logit(hdc,i);
}

//--------------------------- print_regel_yklijn -------------------------------------
void 	print_regel_yklijn(HDC hdc,int i)
{
 if(i<0) return;
 Hfont=SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
 SetTextColor(hdc,RGB(0,0,255));
 startregel = max (0,nVscrollPos + ps.rcPaint.top / cyChar ) ;
 lastregel = min(maxregel,startregel+(cyClient / cyChar)) ;
 if(i==0 && Regressie==IDC_LOGIT) sprintf(text,"%c   | %13s | %13.3f ",
					'@',"BLANK",temp[1][i]);
 else	sprintf(text,"%c   | %13.3f | %13.3f ",
					'@'+i,temp[0][i],temp[1][i]);
 SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
 TextOut (hdc, cxChar, ( - nVscrollPos + i)*cyChar,text,lstrlen(text)) ;
 DeleteObject(Hfont);
}
//-------------------------- print_regel_logit ---------------------------------------
void print_regel_logit(HDC hdc,int i)
{
 Hfont=SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
 SetTextColor(hdc,RGB(255,0,0));
 startregel = max (0,nVscrollPos + ps.rcPaint.top / cyChar ) ;
 lastregel = min(maxregel,startregel+(cyClient / cyChar)) ;
 if(i>-1)
  {
	if(Logit[i].overflow)
		sprintf(text,"%7.0d   | %7.1f | %13.3f |  %12s | %-20s",
			i+1,Logit[i].dilution,Logit[i].input,"  <  > ",Logit[i].monsternaam+1);
	else
		sprintf(text,"%7.0d   | %7.1f | %13.3f | %13.3f | %-20s",
			i+1,Logit[i].dilution,Logit[i].input,Logit[i].result,Logit[i].monsternaam+1);
  }
 SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
 TextOut (hdc, cxChar, ( - nVscrollPos + i)*cyChar,text,lstrlen(text)) ;
 DeleteObject(Hfont);
}
//---------------------------- DoCalcResultDlg -----------------------------------------------
int DoCalcResultDlg (HANDLE Inst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;
	  static char szAppName[] = "CalcResult";

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc	 = (WNDPROC)CalcResultDlgProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = Inst;
	  wndclass.hIcon         = NULL;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;
	  wndclass.lpszClassName = szAppName ;

	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow (szAppName ,NULL,
			  WS_POPUP||WS_THICKFRAME,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  hwndparent, NULL, Inst, NULL);
	  ShowWindow (hwnd,  SW_SHOWMAXIMIZED);
	  UpdateWindow (hwnd);
	  while (GetMessage (&msg, NULL, 0, 0))
			 {
			 TranslateMessage (&msg) ;
			 DispatchMessage (&msg) ;
			 }
	  return msg.wParam ;
	  }
//------------------------------- CalcResultDlgProc --------------------------------------------

LRESULT CALLBACK CalcResultDlgProc (HWND hwnd, WORD message,WORD wParam, LONG lParam)
{
char          text[20] ;
double		  nop;
HDC           hdc;
short         i,j;
PAINTSTRUCT   ps;
TEXTMETRIC    tm;

 switch (message)
	 {
	 double result,n;

	 case WM_CREATE:
		 hdc = GetDC (hwnd) ;
		 hInstDlgInput = ((LPCREATESTRUCT) lParam)->hInstance ;
		 hInstIKnop = ((LPCREATESTRUCT) lParam)->hInstance ;
		 GetTextMetrics (hdc, &tm) ;
		 cxChar = tm.tmAveCharWidth ;
		 cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
		 cyChar = tm.tmHeight + tm.tmExternalLeading ;
		 ReleaseDC (hwnd, hdc) ;
		 nMaxWidth = 40 * cxChar + 18 * cxCaps ;
		 startregel = nVscrollPos=0;          	// eerste regel in geprint scherm
		 xCaret = DilStartPos ;
		 for(n=0;n<MAX_ROW;n++)
			  if(data[0][n]!=MISSING) lastyk=n;
		 if(Yklijninvoer)
			{
			SetWindowText(hwnd,"Conc        Meas               Ins=Start input     Esc=Dilution/Quit");
			Manual_input(hwnd);
			if(YN=='Y') PostMessage(hwnd,WM_KEYDOWN,VK_INSERT,0);
			}
		 if(!Yklijninvoer)
			{
			SetWindowText(hwnd,"Result calculation   Ins=Start input     Esc=Dilution/Quit");
			sprintf(Dtext,"Experiment name?");
			lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
			DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
			FreeProcInstance((FARPROC)lpfnDlgInput);
			strcpy(Experimentname,Rtext);
			if (geladen)  //als een file geladen
			 {
			  j=0;
			  for(i=0;i<=rows;i++)
			  if(data[0][i]==MISSING && data[1][i]!=MISSING)
				{
				strncpy((char*)Logit[j].monsternaam,(char*)naam[i],15);
				Logit[j].pointer=i;
				switch (Regressie)
				 {
				case IDC_POLYNOOM :
					 Logit[j].input=data[1][i];
					 if(data[2][i]>0) Logit[j].dilution=data[2][i];
					 else Logit[j].dilution=1;
					 Logit[j].result=Poly(data[1][i])*Logit[j].dilution;
					 j++;
					 break ;
				case IDC_LINEAIR :
					 Logit[j].input=data[1][i];
					 if(data[2][i]>0) Logit[j].dilution=data[2][i];
					 else Logit[j].dilution=1;
					 Logit[j].result=((data[1][i]-intercept)/slope)*Logit[j].dilution;
					 j++;
					 break ;
				case IDC_RID :
					 Logit[j].input=data[1][i];
					 if(data[2][i]>0) Logit[j].dilution=data[2][i];
					 else Logit[j].dilution=1;
					 Logit[j].result=((data[1][i]*data[1][i]-intercept)/slope)*Logit[j].dilution;
					 j++;
					 break ;
				case IDC_LOGIT :
					 nop=logit(data[1][i]);
					 Logit[j].input=data[1][i];
					 if(data[2][i]>0) Logit[j].dilution=data[2][i];
					 else Logit[j].dilution=1;
					 if(nop==MISSING)
						{
						Logit[j].overflow=TRUE;
						Logit[j].result=MISSING;
						}
					 else
						{
						Logit[j].overflow=FALSE;
						Logit[j].result=exp((nop-intercept)/slope)*Logit[j].dilution;
						}
					 j++;
					 break ;
				  }
			  }
			 }
			maxregel=j;
			YesNo=IDYES;
			Dilution=1;
			if(HAND) PostMessage(hwnd,WM_CHAR,VK_ESCAPE,1);
			}
		 if (hwnd == GetFocus ())	SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
		 return 0;

	 case WM_SIZE:
		cyClient = HIWORD (lParam)-(1*cyChar);
		cxClient = LOWORD (lParam);

		nVscrollMax = max (0, maxregel - cyClient / cyChar);
		nVscrollPos = min (nVscrollPos, nVscrollMax) ;
		SetScrollRange (hwnd, SB_VERT, 0, nVscrollMax, FALSE) ;
		SetScrollPos   (hwnd, SB_VERT, nVscrollPos, TRUE) ;

		nHscrollMax = max (0, 2 + (nMaxWidth - cxClient) / cxChar) ;
		nHscrollPos = min (nHscrollPos, nHscrollMax) ;
		SetScrollRange (hwnd, SB_HORZ, 0, nHscrollMax, FALSE) ;
		SetScrollPos   (hwnd, SB_HORZ, nHscrollPos, TRUE) ;
		return 0 ;

	 case WM_SETFOCUS:
		CreateCaret(hwnd,NULL,cxChar,cyChar);
		SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
		ShowCaret(hwnd);
		return 0;
	 case WM_KILLFOCUS:
		HideCaret(hwnd);
		DestroyCaret();
		return 0;
	 case WM_VSCROLL:
		switch (wParam)
			  {
			  case SB_TOP:
					 nVscrollInc = -nVscrollPos ;
					 break ;

			  case SB_BOTTOM:
					 nVscrollInc = nVscrollMax - nVscrollPos ;
					 break ;

			  case SB_LINEUP:
					 nVscrollInc = -1 ;
					 break ;

			  case SB_LINEDOWN:
					 nVscrollInc = 1 ;
					 break ;

			  case SB_PAGEUP:
					 nVscrollInc = min (-1, -cyClient / cyChar) ;
					 break ;

			  case SB_PAGEDOWN:
					 nVscrollInc = max (1, cyClient / cyChar ) ;
					 break ;

			  case SB_THUMBTRACK:
					 nVscrollInc = LOWORD (lParam) - nVscrollPos ;
					 break ;

			  default:
					 nVscrollInc = 0 ;
			  }
		if ((nVscrollInc =(short)(max (-nVscrollPos,min (nVscrollInc, nVscrollMax - nVscrollPos))))!=0)
			  {
			  nVscrollPos += nVscrollInc ;
			  ScrollWindow (hwnd, 0, -cyChar * nVscrollInc, NULL, NULL) ;
			  SetScrollPos (hwnd, SB_VERT, nVscrollPos, TRUE) ;
			  UpdateWindow (hwnd) ;
			  }
			return 0 ;

	 case WM_HSCROLL:
					switch (wParam)
						  {
						  case SB_LINEUP:
								 nHscrollInc = -1 ;
								 break ;

						  case SB_LINEDOWN:
								 nHscrollInc = 1 ;
								 break ;

						  case SB_PAGEUP:
								 nHscrollInc = -8 ;
								 break ;

						  case SB_PAGEDOWN:
								 nHscrollInc = 8 ;
								 break ;

						  case SB_THUMBPOSITION:
								 nHscrollInc = LOWORD (lParam) - nHscrollPos ;
								 break ;

						  default:
								 nHscrollInc = 0 ;
						  }
					if ((nHscrollInc =(short)(max(-nHscrollPos,min (nHscrollInc, nHscrollMax - nHscrollPos))))!=0)
						  {
						  nHscrollPos += nHscrollInc ;
						  ScrollWindow (hwnd, -cxChar * nHscrollInc, 0, NULL, NULL) ;
						  SetScrollPos (hwnd, SB_HORZ, nHscrollPos, TRUE) ;
						  }
					return 0 ;
	 case WM_CHAR:
		for (i = 0 ; i < LOWORD (lParam) ; i++)
			{
			switch (wParam)
			 {
			 case VK_ESCAPE :                  // escape
				if(Yklijninvoer)
				 {
				 if(maxregel<4)
					 {
					  MessageBox(hwnd,"Less than 4 datapoints","No Go",MB_OK);
					  SendMessage (hwnd, WM_CLOSE, NULL, 0L) ;
					  return 0;
					 }
				  YesNo=MessageBox(hwnd,"Quit Calibrationline input?","Yes = Quit",MB_YESNOCANCEL);
				  if(YesNo==IDYES || YesNo==IDCANCEL)
					 {
					 rows=0;
					 for(n=2;n<MAX_ROW;n++) if(data[1][n]!=MISSING) rows=n;
//	  				 rows_temp=rows;
					 aantal=0;
					 geladen=TRUE;
					 SendMessage (hwnd, WM_CLOSE, NULL, 0L) ;
					 return 0;
					 }
				 }
				 else
					 {
					  YesNo=MessageBox(hwnd,"Constant dilution?","Cancel = Quit",MB_YESNOCANCEL);
					  if(YesNo==IDCANCEL){SendMessage (hwnd, WM_CLOSE, NULL, 0L); break;}
					  if(YesNo==IDYES)
						{
						strcpy(Dtext,"Which dil?");
						lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
						DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
						FreeProcInstance((FARPROC)lpfnDlgInput);
						Dilution=Dresult;
						yCaret=maxregel;
						}
					 }
			case VK_RETURN:  //'\x0D' :                  // CR
						if(yCaret>=maxregel)
							{
							SendMessage(hwnd,WM_KEYDOWN,VK_INSERT,0);
							break ;
							}
			case ' ' :
			case 'C' :
			case 'c' :
						 Nop=xCaret;
						 xCaret=-1;
						 Foerin=TRUE;
						 YesNoTemp=YesNo;
						 YesNo=IDNO;
						 Correction=TRUE;
						 Voerin(hwnd,startregel+yCaret);
						 if(!Foerin) break;
						 hdc=GetDC(hwnd);
						 print_regel(hdc,startregel+yCaret);
						 ReleaseDC(hwnd,hdc);
						 xCaret=Nop;
						 Correction=FALSE;
						 YesNo=YesNoTemp;
						 Foerin=FALSE;
						 SendMessage(hwnd,WM_KEYDOWN, VK_DOWN, 0L);
						 break ;
			default:                       // character codes
						 break ;
			 }
			}
		  return 0;

	 case WM_KEYDOWN:
			switch (wParam)
			  {
			  case VK_INSERT:
					 Foerin=TRUE;
					 Nop=xCaret;
					 xCaret=-1;
					 while(Foerin)
					 {
					  SendMessage(hwnd,WM_VSCROLL, SB_BOTTOM, 0L);
					  Voerin(hwnd,maxregel);
					  if(!Foerin) break;
					  yCaret = lastregel;
					  hdc=GetDC(hwnd);
					  print_regel(hdc,maxregel);
					  ReleaseDC(hwnd,hdc);
					  maxregel++;
					  nVscrollMax = max (0, maxregel + 1 - cyClient / cyChar) ;
					  nVscrollPos = min (nVscrollPos, nVscrollMax) ;
					  SendMessage(hwnd,WM_KEYDOWN, VK_DOWN, 0L);
					 }
					 xCaret=Nop;
					 SendMessage(hwnd,WM_KEYDOWN,VK_END,0);
					 SendMessage(hwnd,WM_KEYDOWN, VK_DOWN, 0L);
					 break;
			  case VK_HOME:
					 SendMessage(hwnd,WM_VSCROLL, SB_TOP, 0L);
					 yCaret = 0;
					 break ;

			  case VK_END:
					 lastregel = min(maxregel,startregel+(cyClient / cyChar)) ;
					 SendMessage(hwnd,WM_VSCROLL, SB_BOTTOM, 0L);
					 yCaret = lastregel-startregel-1;
					 break ;

			  case VK_PRIOR:
					 SendMessage(hwnd,WM_VSCROLL, SB_PAGEUP, 0L);
					 yCaret = 0;
					 break ;

			  case VK_NEXT:
					 SendMessage(hwnd,WM_VSCROLL, SB_PAGEDOWN, 0L);
					 yCaret = 1;
					 break ;

			  case VK_LEFT:
					 xCaret = max (xCaret - 1, 0) ;
					 break ;

			  case VK_RIGHT:
					 xCaret = min (xCaret + 1, strlen(text)-1);
					 break ;

			  case VK_UP:
					 yCaret--;
					 if(yCaret<0)
					  {
						SendMessage(hwnd,WM_VSCROLL, SB_LINEUP, 0L);
						yCaret=0;
					  }
					 break ;

			  case VK_DOWN:
					 yCaret++;
					 if(yCaret>lastregel-startregel)
					  {
						 SendMessage(hwnd,WM_VSCROLL, SB_LINEDOWN, 0L);
						 yCaret = lastregel-startregel;
					  }
					 break ;

			  case VK_DELETE:
								 break ;
			}
		 SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
		 return 0 ;


	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps) ;
		nPaintBeg = max (0,nVscrollPos + ps.rcPaint.top / cyChar ) ;
		nPaintEnd = min (maxregel,nVscrollPos + ps.rcPaint.bottom -5/ cyChar  ) ;
		for (i = nPaintBeg ; i < nPaintEnd ; i++)	  print_regel(hdc,i);
		EndPaint (hwnd, &ps) ;
		return 0 ;

	case WM_CLOSE:

		if(!Yklijninvoer)
		  {
			char text[30],LastName[30];
			int maxtel,tel,aantaltel;

			strcpy(LastName,Logit[0].monsternaam+1);
			if((fpout=fopen(Tempfile,"w+"))==NULL)
				 MessageBox(hwnd,"Error","Error opening outputfile",MB_OK);
			else
			 {
				fprintf(fpout,"\"Wlogit version %s EJN\"\n",DATUM);
				fprintf(fpout,"\"Experiment name: %s\"\n",Experimentname);
				fprintf(fpout,"\"Filename: %s\"\n\n",Loaded_file_name);
				switch (Regressie)
				{
				 int n;
				 case IDC_POLYNOOM :
						fprintf(fpout,"\"R=%6.3f              %d degree polynoom\n X=\"",corr,polynoomgraad);
						for(n=polynoomgraad;n>1;n--)
							fprintf(fpout,"\"%6.6g y^%d + \"\n", polycoeif[n],n);
						fprintf(fpout,"\"%6.6g y +\n %6.6g\"\n", polycoeif[1],polycoeif[0]);
						break;
				 case IDC_LINEAIR :
				 case IDC_RID :
						fprintf(fpout,"\"R=%6.4f\"\n",corr);
						fprintf(fpout,"\"Slope=%6.4f    intercept=%6.4f\"\n",slope,intercept);
						break;
				 case IDC_LOGIT :
						fprintf(fpout,"\"R=%6.4f   slope=%6.4f  intercept=%6.4f\"\n",corr,slope,intercept);
						fprintf(fpout,"\"Blank=%6.4f    Bmax=%6.4f    X(1/2 Bmax)=%6.4f\"\n",C,D,exp(-intercept/slope));
				 }

			  fprintf(fpout,"\n\"%-7s\"|\"%-11s\"|\"%-11s\"|\"%-10s\"|\n",
							 "No","X","Y","Xres");
			  if(Regressie==IDC_LOGIT)
				 fprintf(fpout,"\"%-7s\"|%-13.3f|%-13.3f|%-12.3f|\n","@",0.0,C,0.0);

			  for (i=0;i<rows_temp;i++)
				{
				switch (Regressie)
					{
					case IDC_POLYNOOM :
								result=Poly(temp[1][i]);			break;
					case IDC_LINEAIR :
								result=(temp[1][i]-intercept)/slope; 		break;
					case IDC_RID :
								result=(temp[1][i]*temp[1][i]-intercept)/slope; 		break;
					case IDC_LOGIT :
								result=logit(temp[1][i]);
								if (result!=MISSING)
									result=exp((logit(temp[1][i])-intercept)/slope);	break;
					}
				 if(!ykpunten[i]) strcpy(text,"del "); else strcpy(text,"    ");
				 text[4]=i+'A'; text[5]=0;
				 if(result==MISSING)
					fprintf(fpout,"\"%-7s\"|%-13.3f|%-13.3f|\" <  >\"     |\n",
									text,temp[0][i],temp[1][i]);
				 else
					fprintf(fpout,"\"%-7s\"|%-13.3f|%-13.3f|%-12.3f|\n",
								text,temp[0][i],temp[1][i],result);
				  }

				fprintf(fpout,"\r\n\"%-5s\"|\"%-11s\"|\"%-11s\"|\"%-11s\"|\n",
							 "No","Dil","Y","Xres");

				aantaltel=maxtel=tel=0; //tel aantal samen te voegen regels;
				if(JoinResults)
				{
				 for(i=0;i<maxregel;i++)
					{
					if(strlen(Logit[i].monsternaam+1)<2) continue;
					if(strcmp(Logit[i+1].monsternaam+1,Logit[i].monsternaam+1)==0
								 && JoinResults)
						  tel++;
					else {maxtel=tel; tel=0; aantaltel++;}
					}
				 if(maxtel<2 || aantaltel<2)
					  if(IDNO==MessageBox(hwnd,
						"Hardly anything to join\nStill want to join results?",
						"Join question",
						MB_YESNO|MB_ICONQUESTION)) JoinResults=FALSE;
             else JoinResults=TRUE;
				}
				for(i=0;i<maxregel;i++)
					{
					if(strcmp(LastName,Logit[i].monsternaam+1)!=0 && JoinResults)
						fprintf(fpout,"\n");
					if(Logit[i].dilution>0)// && Logit[i].input!=MISSING )
						{
						if(Logit[i].overflow)
							fprintf(fpout,"%-7.0d|%-13.1f|%-13.3f|\"%-11s\"|\"%-20s\"\n",
								i+1,Logit[i].dilution,Logit[i].input," <  >",Logit[i].monsternaam+1);
						else
							fprintf(fpout,"%-7.0d|%-13.1f|%-13.3f|%-13.3f|\"%-20s\"\n",
								i+1,Logit[i].dilution,Logit[i].input,Logit[i].result,Logit[i].monsternaam+1);
						}
					strcpy(LastName,Logit[i].monsternaam+1);
					}
			}
//			rows=rows_temp+maxregel+2;
			fclose(fpout);
		  }
		  DestroyWindow (hwnd) ;
		  return 0 ;

	 case WM_DESTROY:
			PostQuitMessage (0) ;
			return 0 ;
	 }
 return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//--------------------------------- CalcInputDlgProc ---------------------------------------
BOOL FAR PASCAL CalcInputDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
// char text[80];
 lParam=lParam;

  switch (message)
	  {
		case WM_INITDIALOG:
				SetWindowText(hDlg,Dtext);
				if(Correction) SetDlgItemText(hDlg,IDM_INPUTDLG,Dlgtext);
				return TRUE ;

		case WM_COMMAND :
		 switch (wParam)
				{
			case  IDM_INPUTDLG:
					return 0;
			default:
					GetDlgItemText(hDlg,IDM_INPUTDLG,Rtext,40);
					if(strlen(Rtext)<1) {Foerin=FALSE;}
					Dresult=atof(Rtext);
					EndDialog (hDlg, wParam) ;
					return TRUE ;
				 }
	  }
	  return FALSE ;
	  }
//--------------------------------- DilKnoppenProc ----------------------------------------
BOOL FAR PASCAL DilKnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  char text[80];
  lParam=lParam;

  switch (message)
	  {
		case WM_INITDIALOG:
			sprintf(text,"%6.3f",Conc);
			SetDlgItemText(hDlg,IDM_HIGHCONC,text);
			sprintf(text,"%6.1f",Dilfactor);
			SetDlgItemText(hDlg,IDM_DILFACTOR,text);
			sprintf(text,"%2d",Replicas);
			SetDlgItemText(hDlg,IDM_NOOFREPL,text);
					 return TRUE ;
		case WM_KEYDOWN:
			if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (wParam)
				{
			 case  IDM_HIGHCONC:
								GetDlgItemText(hDlg,IDM_HIGHCONC,text,20);
								Conc=atof(text);
								if(Conc==0) ConstantDil=FALSE;
								return 0;
			 case  IDM_DILFACTOR:
								GetDlgItemText(hDlg,IDM_DILFACTOR,text,20);
								Dilfactor=atof(text);
								if(Dilfactor<1) Dilfactor=2;
								return 0;
			 case  IDM_NOOFREPL:
								GetDlgItemText(hDlg,IDM_NOOFREPL,text,20);
								Replicas=atof(text);
								if(Replicas<1||Replicas>5) Replicas=1;
								return 0;
			 default:
								EndDialog (hDlg, wParam) ;
								return TRUE ;
				 }
	  }
	  return FALSE ;
	  }
//-------------------------------------- Voerin ------------------------------------------
void Voerin(HWND hwnd,int regel)
{
  if(Yklijninvoer) Voerin_yklijn(hwnd,regel);
  else	           Voerin_logit(hwnd,regel);
}
//-------------------------------------- Voerin_yklijn --------------------------------------
void Voerin_yklijn(HWND hwnd,int regel)
{
double vx,vy;
int n;

 if(regel<0) return;
 vx=temp[0][regel];
 vy=temp[1][regel];
 if(!ConstantDil || Correction)
 if( regel>0 || Regressie!=IDC_LOGIT)
  {
	sprintf(Dtext,"%d ",regel);
	strcat(Dtext,"Conc?");
	if(Correction) sprintf(Dlgtext,"%1.3f",temp[0][regel]);
	lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
	DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
	FreeProcInstance((FARPROC)lpfnDlgInput);
	if(!Foerin) return;
	temp[0][regel]=Dresult;
	if(Correction)
		{
		for(n=0;n<rows;n++)
		 if(data[0][n]==vx && data[1][n]==vy) data[0][n]=Dresult;
		}
	else data[0][regel]=Dresult;
  }

if(!Foerin) return;
if(regel==0 && Regressie==IDC_LOGIT)
	{
	sprintf(Dtext," Blank : ");
	temp[0][0]=0;
	data[0][0]=0;
	}
else sprintf(Dtext,"Conc=%11.3f :",temp[0][regel]);
 strcat(Dtext," Meas?");
 if(Correction) sprintf(Dlgtext,"%1.3f",temp[1][regel]);
 lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
 DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
 FreeProcInstance((FARPROC)lpfnDlgInput);
 if(!Foerin) return;
 temp[1][regel]=Dresult;
 if(Correction)
	{
	if(regel==0 && Regressie==IDC_LOGIT)
		{
		for(n=0;n<rows;n++)   if(data[0][n]==0)  data[1][n]=Dresult;
		}
	else
		{
		for(n=0;n<rows;n++)
			if(data[0][n]==temp[0][regel] && data[1][n]==vy) data[1][n]=Dresult;
		}
	}
 else data[1][regel]=Dresult;
}
//-------------------------------------- Voerin_logit ---------------------------------------
void Voerin_logit(HWND hwnd,int regel)
{
double nop;
char t1[5];
			// if(YesNo==7) return;
 if(YesNo==IDNO)
  {
	sprintf(Dtext,"%d ",regel+1);
	strcat(Dtext,"Dil?");
	if(Correction) sprintf(Dlgtext,"%1.1f",Logit[regel].dilution);
	lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
	DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
	FreeProcInstance((FARPROC)lpfnDlgInput);
	if(!Foerin) return;
	Logit[regel].dilution=Dresult;
  }
 else Logit[regel].dilution=Dilution;

 if(Foerin==FALSE) return;
 sprintf(Dtext,"%d ",regel+1);
 strcat(Dtext,"Meas?");
 if(Correction) sprintf(Dlgtext,"%1.3f",Logit[regel].input);
 lpfnDlgInput  = MakeProcInstance (CalcInputDlgProc,hInstDlgInput );
 DialogBox(hInstDlgInput,"INPUTDLG",hwnd,lpfnDlgInput);
 FreeProcInstance((FARPROC)lpfnDlgInput);
 if(!Foerin) return;
 if(strlen((char*)naam[Logit[regel].pointer])<1)
	{
	strcpy((char*)Logit[regel].monsternaam," Sample ");
	itoa(regel+1,t1,10);
	strcat((char*)Logit[regel].monsternaam,t1);
	}

 switch (Regressie)
	{
 case IDC_POLYNOOM :
			Logit[regel].input=Dresult;
			Logit[regel].result=Poly(Dresult)*Logit[regel].dilution;
			break ;
 case IDC_LINEAIR :
			Logit[regel].input=Dresult;
			Logit[regel].result=((Dresult-intercept)/slope)*Logit[regel].dilution;
			break ;
 case IDC_RID :
			Logit[regel].input=Dresult;
			Dresult*=Dresult;
			Logit[regel].result=((Dresult-intercept)/slope)*Logit[regel].dilution;
			break ;
 case IDC_LOGIT :
			Logit[regel].input=Dresult;
			nop=logit(Dresult);
			if(nop==MISSING)
				{ Logit[regel].overflow=TRUE;
				  Logit[regel].result=MISSING;
				}
			else
				{
				  Logit[regel].overflow=FALSE;
				  Logit[regel].result=exp((nop-intercept)/slope)*Logit[regel].dilution;
				}
			break ;
	}
	if(regel==maxregel)	 Logit[regel].pointer=++rows;
	data[0][Logit[regel].pointer]=MISSING;
	data[1][Logit[regel].pointer]=Logit[regel].input;
	data[2][Logit[regel].pointer]=Logit[regel].dilution;
	strcpy((char*)naam[Logit[regel].pointer],Logit[regel].monsternaam);
	if(rows> MAXROWS-10)
	 {
	  MessageBox(hwnd,"Stop, maximum inputlines reached","STOP",MB_ICONHAND);
	  regel--;
	 }
}
//--------------------------------------- Manual_input -----------------------------------
void   Manual_input(HWND hwndEdit)
{
int    i,j,m,n;
WORD   wParam;

 if((rows_temp<3 || aantal<3) && geladen) fill_temp(hwndEdit,0,1);
 YN='Y';
 if(ykpunten[2]==TRUE)
	{
	 YN=MessageBox(hwndEdit,"Correction ?","No = New dataset",MB_YESNO);
	 if(YN==IDYES)
	  {
		ConstantDil=FALSE;
		YN='N';
		if(Regressie==IDC_LOGIT)
		{
		 for(n=rows_temp+1;n>0;n--)
		 {
		 temp[0][n]=temp[0][n-1];
		 temp[1][n]=temp[1][n-1];
		 }
		 temp[0][0]=0;
		 temp[1][0]=C;
		 maxregel=rows_temp+1;
		}
		else maxregel=rows_temp;
		yCaret=1;
		return;
	  }
	 }
 clear_data();
 rows_temp=0;
 YN='Y';
 Conc=1000;
 Dilfactor=2;
 Replicas=1;
 maxregel=0;
 Loaded_file_name[0]=0;
 szRealFileName[0]=0;
 lpfnIKnop = MakeProcInstance (DilKnoppenProc, hInstIKnop) ;
 wParam=DialogBox(hInstIKnop,"AUTOCONC",hwndEdit,lpfnIKnop);
 FreeProcInstance((FARPROC)lpfnIKnop);
 if(wParam==IDM_CANCEL||wParam==2 )  ConstantDil=FALSE;
 else 			       					 ConstantDil=TRUE;
 if(Dilfactor>1)
	{
	geladen=FALSE;
	for(j=0;j<MAX_YK_PNT;j++)
		{
		 for(i=0;i<2;i++)  temp[i][j]=MISSING;
		 ykpunten[j]=FALSE;
		}
	C=0;
	Conc *= Dilfactor;
	if(Regressie==IDC_LOGIT) m=1; else m=0;
	while(m<MAX_YK_PNT)
		{
		 Conc /= Dilfactor;
		for(n=0;n<Replicas;n++) {data[0][m]=Conc;  temp[0][m++] = Conc;}
		}
	}
 }


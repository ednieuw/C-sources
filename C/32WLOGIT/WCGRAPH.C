/*-----------------------------------------------
	WCGRAPH.C -- Ed Nieuwenhuys   1994
  -----------------------------------------------*/

#include "wlogit.h"

int 		DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
LRESULT 	CALLBACK GraphDrawDlgProc (HWND, WORD, WORD, LONG);
void		Print_grafiek(HWND hwnd,HDC hdc);
int		DelInsDataPoint (HWND hwnd,long lParam);
char 		Teken_logit_data(HWND hwndas,HDC hdc,struct ASCAL *ascal);

int DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;
	  static char szAppName[] = "Graph";

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc	 = (WNDPROC) GraphDrawDlgProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = grInst;
	  wndclass.hIcon         = NULL;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;
	  wndclass.lpszClassName = szAppName ;

	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow (szAppName ,NULL,
			  WS_POPUP,//WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  hwndparent, NULL, grInst, NULL);
	  ShowWindow (hwnd,  SW_SHOWMAXIMIZED);
	  UpdateWindow (hwnd);
	  while (GetMessage (&msg, NULL, 0, 0))
			 {
			 TranslateMessage (&msg) ;
			 DispatchMessage (&msg) ;
			 }
	  return msg.wParam ;
	  }


LRESULT CALLBACK GraphDrawDlgProc (HWND hwnd, WORD message,
											WORD wParam, LONG lParam)
	  {
	  HDC hdc;
	  switch (message)
		  {
	  case WM_CREATE:
			 return 0 ;
	  case WM_SIZE:
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 return 0 ;

	  case WM_PAINT :
			 hdc = BeginPaint (hwnd, &ps) ;
			 Print_grafiek(hwnd,hdc);
			 EndPaint(hwnd,&ps);
			 return 0;

	  case WM_LBUTTONDOWN:
								DestroyWindow (hwnd) ;
								return 0;
	  case WM_KEYDOWN:
							switch(wParam)
							 {
								case VK_ESCAPE:
								  StopCalcAll=TRUE;
								  berekend=FALSE;
								case VK_RETURN:
								case VK_TAB:
								case VK_SPACE:
											 DestroyWindow (hwnd);
											 break;

								default:
								if(wParam>='A' && wParam<='A'+axcal.Noof_DataPoints)
								 {
									ykpunten[wParam-'A']=1-ykpunten[wParam-'A'];
									berekend=FALSE;
									regressie(hwnd,axcal.X_As,axcal.Y_As);
									InvalidateRect(hwnd,NULL,TRUE);
							 return 0;
								  }
							 }
	  case WM_RBUTTONDOWN:
	  case WM_MBUTTONDOWN:
								if(DelInsDataPoint (hwnd,lParam))
									{
									regressie(hwnd,axcal.X_As,axcal.Y_As);
									InvalidateRect(hwnd,NULL,TRUE);
									}
								return 0;
	  case WM_CLOSE:
								DestroyWindow (hwnd) ;
								return 0 ;
	  case WM_DESTROY:
								PostQuitMessage (0) ;
								return 0 ;
	  case WM_QUIT:
								PostQuitMessage (0) ;
								return 0 ;
	  }
	  return DefWindowProc (hwnd, message, wParam, lParam);
  }

void	Print_grafiek(HWND hwnd,HDC hdc)
 {
//  MSG msg;
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char file[MAXFILE];
  char ext[MAXEXT];
//  char text[10];

	SelectObject (hdc, GetStockObject (SYSTEM_FONT)) ;
	GetTextMetrics (hdc, &tm) ;
	cxChar = tm.tmAveCharWidth ;
	cyChar = tm.tmHeight + tm.tmExternalLeading ;
	cxCaps = (tm.tmPitchAndFamily &1 ? 3: 2) * cxChar / 2;
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	GetClientRect (hwnd, &rect) ;
	SetWindowExtEx(hdc,rect.right, -rect.bottom,NULL);
	SetViewportExtEx (hdc, rect.right,-rect.bottom,NULL) ;
	SetViewportOrgEx(hdc,0,0,NULL);
	SetROP2 (hdc, nDrawingMode) ;
	MaxX = cxClient;
	MaxY = cyClient;					// size of screen
	if(Pic_output_file[0]!=0)
	{
	 fnsplit(Pic_output_file,drive,dir,file,ext);
	 strcpy(ext,"PIC");
/*	 if(Autosave)
	  {
		strcpy(file,itoa(axcal.X_As,text,10));
		strcat(file,"_");
		strcat(file,itoa(axcal.Y_As,text,10));
	  }
*/	 fnmerge(Pic_output_file,drive,dir,file,ext);
	 DrawPicFile=TRUE;
	 if(!Pic_Open(hwnd))
	 {StopCalcAll=TRUE; return;}
	}
	Teken_assen(hwnd,hdc,&axcal);
	Teken_logit_data(hwnd,hdc,&axcal);
	teken_graph(hwnd,hdc,&axcal,axcal.X_As,axcal.Y_As);
	DeleteObject(SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
	if(Pic_output_file[0]!=0)
	{
	 Pic_End(hwnd);
	 DrawPicFile=FALSE;
//	 if(!Autosave) 			Pic_output_file[0]=0;
	}
	if(NoGraph)	PostMessage(hwnd,WM_CLOSE,0,0L);
 }
//------------------------------------------------------------------------------------------------
int	DelInsDataPoint (HWND hwnd, long lParam)
{
 double xPos,yPos;
 double x,y,xp,yp,varx,vary;
// char text[80];
 int i,j;
 double xi,xa,yi,ya,devx,devy;
 double devbest=999;
// double 	maxx,maxy;
// double 	minx,miny;
 hwnd=hwnd;
 xPos = (double)LOWORD(lParam);    /* horizontal position of cursor */
 yPos = (double)HIWORD(lParam);    /* vertical position of cursor   */

 xp= (xPos/((double)MaxX )* ScreenX);
 yp= (yPos/((double)MaxY )* ScreenY);

 if(axcal.X_Astrans==1)
 	x = exp(((xp-axcal.X_Offset)/axcal.X_Length)*(log(axcal.X_Max) - log(axcal.X_Min))+ log(axcal.X_Min));
 else
	x =((xp-axcal.X_Offset)/axcal.X_Length)*(axcal.X_Max - axcal.X_Min)+ axcal.X_Min;
 if(axcal.Y_Astrans==1)
	y= exp(log(axcal.Y_Max)-(-log(axcal.Y_Min)+(((yp-axcal.Y_Offset)/axcal.Y_Length)*(log(axcal.Y_Max)-log(axcal.Y_Min))+log(axcal.Y_Min))));
 else
	y=(axcal.Y_Max-axcal.Y_Min) -  ((yp-axcal.Y_Offset)/axcal.Y_Length)*(axcal.Y_Max - axcal.Y_Min)+ axcal.Y_Min;

 varx=(axcal.X_Max - axcal.X_Min)/200;
 vary=(axcal.Y_Max - axcal.Y_Min)/200;
 if(axcal.X_Astrans==1)
 varx=exp(log(x)+((log(axcal.X_Max)-log(axcal.X_Min))/200))-x;
 if(axcal.Y_Astrans==1)
 vary=exp(log(y)+((log(axcal.Y_Max)-log(axcal.Y_Min))/200))-y;

 xi=x-varx;
 xa=x+varx;
 yi=y-vary;
 ya=y+vary;

 for (i=0,j=-1;i<rows_temp;i++)
	{
		 if (temp[0][i]>xi&&temp[0][i]<xa)
				if (temp[1][i]>yi&&temp[1][i]<ya)
					{
					 devx=fabs(temp[0][i]-x);
					 devy=fabs(temp[1][i]-y);
					 if((devx+devy)<devbest) {devbest=devx+devy; j=i;}
					}
	}
if(j==-1) return FALSE;
//sprintf(text,"      %6.1f %6.1f",temp[0][j],temp[1][j]);
//MessageBox(hwnd,text,"Point will be deleted",MB_OK);
berekend=FALSE;
ykpunten[j]=FALSE;

return TRUE;
}
//--------------------------------------------------------------
char Teken_logit_data(HWND hwndas,HDC hdc,struct ASCAL *ascal)
{
  LOGPEN lp;
  int 	n;
  float xpos,ypos;
  char text[80];
  hwndas=hwndas;
  lp.lopnStyle   = PS_SOLID;
  lp.lopnWidth.x = ascal->Pointthickness;
  lp.lopnWidth.y = 0;              // y-dimension not used
  if(ascal->Color!=255)	lp.lopnColor = dwColor[ascal->PointColor];
  else						lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
  hpen = SelectObject(hdc,CreatePenIndirect(&lp) );
  ascal->X_Astrans=(BYTE)Ascal(&ascal->X_NoofTicks,&ascal->X_Min,&ascal->X_Max,(int)(ascal->X_Astrans),1);
  if(Regressie==IDC_LOGIT &&ascal->X_Astrans)
	 {
	 xpos=ascal->X_Offset-tm.tmHeight*(ScreenY/cyClient);
	 if(C>=ascal->Y_Min ) ypos=Data_Y_2screen(C,ascal); //Blanko waarde
	 if(ascal->Y_Astrans)
		{
		  if(C>ascal->Y_Min ) ypos=Data_Y_2screen(C,ascal); //Blanko waarde
		  else ypos=ascal->Y_Offset-tm.tmHeight*(ScreenY/cyClient);
		}
	 Point(hdc,xpos,ypos,ascal->X_Length/200);
	 }
  if(Regressie==IDC_LOGIT && !ascal->X_Astrans)
	 {
	 xpos=ascal->X_Offset;
	 if(C>=ascal->Y_Min ) ypos=Data_Y_2screen(C,ascal); //Blanko waarde
	 if(ascal->Y_Astrans)
		{
		  if(C>ascal->Y_Min ) ypos=Data_Y_2screen(C,ascal); //Blanko waarde
		  else ypos=ascal->Y_Offset-tm.tmHeight*(ScreenY/cyClient);
		}
	 Point(hdc,xpos,ypos,ascal->X_Length/200);
	 }
  for(n=0;n<ascal->Noof_DataPoints;n++)
	{
	 if(Regressie==IDC_RID) ypos=Data_Y_2screen(temp[1][n]*temp[1][n],ascal);
	 else						   ypos=Data_Y_2screen(temp[1][n],ascal);
	 if(temp[0][n]==0 &&ascal->X_Astrans==1)	 xpos=ascal->X_Offset-tm.tmHeight*(ScreenY/cyClient);
	 else 					   xpos=Data_X_2screen(temp[0][n],ascal);
	 if(ykpunten[n]==FALSE) Printc(hdc,xpos,ypos,1,0,"X");//itoa(n,text,10));
	 else		               Point(hdc,xpos,ypos,ascal->X_Length/200);
	 text[0]=(char)n+65;
	 text[1]=0;
	 Printc(hdc,xpos,ypos+ascal->Y_Length/20,1,0,text);
	}
  DeleteObject(SelectObject(hdc, hpen));
 return 0;
}

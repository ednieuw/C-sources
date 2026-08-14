/*-----------------------------------------------
   WCGRAPH.C -- Open and Close File Dialog Boxes
  -----------------------------------------------*/

#include "wcorr.h"

int 	DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
LRESULT CALLBACK GraphDrawDlgProc (HWND, WORD, WORD, LONG);
void	Print_grafiek(HWND hwnd,HDC hdc);
void	DelInsDataPoint (HWND hwnd,long lParam);

int DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;
	  static char szAppName[] = "Graph";//"Wcorr" ;

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;//| CS_SAVEBITS;
	  wndclass.lpfnWndProc	 = (WNDPROC)GraphDrawDlgProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = grInst;
	  wndclass.hIcon         = NULL;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = szAppName ;//"Graph";

	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow (szAppName ,NULL,
			  WS_POPUP,
			  //WS_OVERLAPPEDWINDOW,
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


LRESULT CALLBACK GraphDrawDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  HDC hdc;
	  switch (message)
		  {
	  case WM_CREATE:
//			 SetWindowText (hwnd, "Graph Weighted Correlation") ;
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

	  //case WM_RBUTTONDOWN:
	  case WM_KEYDOWN:
//								StopCalcAll=TRUE;
								if(wParam==VK_ESCAPE)     StopCalcAll=TRUE;
	  case WM_LBUTTONDOWN:
								DestroyWindow (hwnd) ;
								return 0;
	  case WM_RBUTTONDOWN:
								DelInsDataPoint (hwnd,lParam) ;
								reken(hwnd,axcal.X_As,axcal.Y_As);
								InvalidateRect(hwnd,NULL,TRUE);
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
     return DefWindowProc (hwnd, message, wParam, lParam) ;
  }

void	Print_grafiek(HWND hwnd,HDC hdc)
 {
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char file[MAXFILE+9];
  char ext[MAXEXT];
  char text[12];

	SelectObject (hdc, GetStockObject (SYSTEM_FONT)) ;
	GetTextMetrics (hdc, &tm) ;
	cxChar = tm.tmAveCharWidth ;
	cyChar = tm.tmHeight + tm.tmExternalLeading ;
	cxCaps = (tm.tmPitchAndFamily &1 ? 3: 2) * cxChar / 2;
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	GetClientRect (hwnd, &rect) ;
	SetWindowExt(hdc,rect.right, -rect.bottom);
	SetViewportExt (hdc, rect.right,-rect.bottom) ;
	SetViewportOrg(hdc,0,0);
	SetROP2 (hdc, nDrawingMode) ;
	MaxX = cxClient;
	MaxY = cyClient;					// size of screen
	if(Pic_output_file[0]!=0)
	{
	 fnsplit(Pic_output_file,drive,dir,file,ext);
		 strcpy(ext,"PIC");
	 if(Autosave)
	  {
		strcpy(file,itoa(axcal.X_As,text,10));
		strcat(file,"_");
		strcat(file,itoa(axcal.Y_As,text,10));
	  }
	if(axcal.Graphtype!=REGRESSION) 		strcat(file,"________");

	switch (axcal.Graphtype)
	 {
		case REGRESSION:										break;
		case SS_SLOPE:		 file[7]='S'; file[8]=0;	break;
		case SS_INTERCEPT: file[7]='I'; file[8]=0;	break;
		case SS_CORR:      file[7]='R'; file[8]=0;	break;
		case SS_SCORR:     file[7]='N'; file[8]=0;	break;
	 }
	 if(strlen(file)==0)
	 {
		 strcpy(file,"noname");
		 MessageBox (hwnd,"Graph file had no name\nSaved as NONAME.PIC",
								"Graph saving oblivion",
								MB_OK | MB_ICONHAND);
	 }
	 fnmerge(Pic_output_file,drive,dir,file,ext);
	 DrawPicFile=TRUE;
	 if(!Pic_Open(hwnd)) {StopCalcAll=TRUE; return;}
	}
	if(SHOESTRAP) 	teken_shoestrap_graph(hwnd,hdc);
	else
		{
		Teken_assen(hwnd,hdc);
		Teken_data(hwnd,hdc);
		teken_graph(hwnd,hdc,axcal.X_As,axcal.Y_As);
		}
	DeleteObject(SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
	if(Pic_output_file[0]!=0)
	{
	 Pic_End(hwnd);
	 DrawPicFile=FALSE;
//if(!Autosave) 			Pic_output_file[0]=0;
	}
	if(Autosave&&NoGraph)	PostMessage(hwnd,WM_CLOSE,0,0L);
 }
//------------------------------------------------------------------------------------------------
void	DelInsDataPoint (HWND hwnd, long lParam)
{
 double xPos,yPos;
 double x,y,xp,yp,varx,vary;
 char 	text[80];
 int 	i,j;
 double xi,xa,yi,ya,devx,devy;
 double devbest=999;
 double maxx,maxy;
 double minx,miny;

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
if(j>-1)
	{
	sprintf(text,"%6.1f %6.1f ",temp[0][j],temp[1][j]);
	MessageBox(hwnd,text,"Point will be deleted",MB_OK);
	for(i=j;i<rows_temp-1;i++)
		{
		temp[0][i]=temp[0][i+1];
		temp[1][i]=temp[1][i+1];
		}
	rows_temp--;
	}
/*else {
		sprintf(text,"%6.1f %6.1f  %s",x,y,"Out of Range");
		MessageBox(hwnd,text,"Out of Range",MB_OK|MB_ICONEXCLAMATION);
	  }
*/
maxx=maxy=-10E20;
minx=miny=+10e20;

for (i=0;i<rows_temp;i++)
   {
       if (temp[0][i]>maxx)      maxx=temp[0][i];
       if (temp[0][i]<minx)      minx=temp[0][i];
       if (temp[1][i]>maxy) 		maxy=temp[1][i];
		 if (temp[1][i]<miny)		miny=temp[1][i];
	}
if(Xzero && !LogXas) {minx=0; axcal.X_Zeroforced=Xzero;}
if(Yzero && !LogYas) {miny=0; axcal.Y_Zeroforced=Yzero;}

axcal.X_Min=minx;
axcal.X_Max=maxx*1.05;
axcal.Y_Min=miny;
axcal.Y_Max=maxy*1.05;

}

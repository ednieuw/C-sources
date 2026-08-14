/*------------------------------------------
   Fireflies FIREFLY.C
					  (c) Ed Nieuwenhuys 1993
  ------------------------------------------*/

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define TRUE 	1
#define FALSE	0
#define ENERGIE 110
#define THRESHOLDDIV 4
#define CELLAROUND 4
long FAR PASCAL WndProc (HWND, WORD, WORD, LONG) ;
void DrawFlies (HWND) ;
void CalcFlash(HWND);
int  Init(HWND hwnd);
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap);
//  waves
int 	energie=ENERGIE;
int	thresholddiv=THRESHOLDDIV;
int 	cellaround=CELLAROUND;
short nooffliesx=40;
short nooffliesy=30;
/*
int 	energie=5;
int	thresholddiv=40;
int 	cellaround=4;
short nooffliesx=40;
short nooffliesy=30;
*/
struct FLIES {
					int x;
					int y;
					unsigned int energie;
					unsigned int genergie;
					char flash;
               char status;
				 } flies[4400];



short 	cxClient, cyClient ;
short 	nooffx,nooffy;
short 	nRed=255, nGreen=255, nBlue=255 ;
int 		threshold=1000;
char 		text[120],rectangle,Glow=TRUE;
char 		*p;
HDC 	   hdcMemory, hdcbm;
HBRUSH 	hBrushg,hBrushw,hBrushy ;
HBITMAP  FlyLGBitmap,FlyDGBitmap,FlyBLBitmap, hbmpOld;
BITMAP   bm;
HANDLE	hInst;


int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow)
     {
     static char szAppName[] = "Firefly" ;
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASS    wndclass ;
	  if (!hPrevInstance)
          {
          wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
			 wndclass.lpfnWndProc   = (WNDPROC)WndProc ;
          wndclass.cbClsExtra    = 0 ;
          wndclass.cbWndExtra    = 0 ;
			 wndclass.hInstance     = hInstance ;
			 wndclass.hIcon         = LoadIcon (hInstance, "firefly"); ;//NULL ;
          wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
			 wndclass.hbrBackground = GetStockObject (BLACK_BRUSH) ;
          wndclass.lpszMenuName  = NULL ;
          wndclass.lpszClassName = szAppName ;
          RegisterClass (&wndclass);
          }
	 hInst = hInstance ;
	 nCmdShow  = SW_SHOWMAXIMIZED;
	 lstrcpy(text,lpszCmdLine);
	 p = strtok(text, " ");
	 if (p) {  nooffx=atoi(p);
	 p = strtok(NULL, " ");
		if (p)  nooffy=atoi(p);}
	 p = strtok(NULL, " ");
		if (*p=='R' ||*p=='r')  rectangle=TRUE; else rectangle=FALSE;

	 if(nooffx*nooffy<=5400&&nooffx*nooffy>10)     {		nooffliesx=nooffx;		nooffliesy=nooffy;     }

    hwnd = CreateWindow (szAppName, "Firefly",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

    ShowWindow (hwnd, nCmdShow) ;
	 sprintf(text,"Cellsaround %d Energie %d%% Thresholddiv %d %s FIREFLY",
						cellaround,energie,thresholddiv,Glow?"Glow":"");
	 SetWindowText(hwnd,text);
	 UpdateWindow (hwnd) ;
	 Init(hwnd);
	 DrawFlies (hwnd) ;
		  while (TRUE)
          {
          if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
               {
               if (msg.message == WM_QUIT)
                    break ;

               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
               }
			 else {

					CalcFlash(hwnd);
					DrawFlies (hwnd) ;
               }
			 }
	 lpszCmdLine=lpszCmdLine;
     return msg.wParam ;
     }
//---------------------------------- wndproc -------------------------------------------------
long FAR PASCAL WndProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
{
int m,n;
switch (message)
			 {
			 case WM_SIZE:
					for(n=0;n<nooffliesx;n++)
						for(m=0;m<nooffliesy;m++)
						  flies[n+nooffliesx*m].status=-1;
					cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
               return 0 ;

			 case WM_CHAR:
					for(n=0;n<LOWORD(lParam); n++)
					 {
					  switch(wParam)
						{
						 case 'c': cellaround--;if(cellaround<1) cellaround=1;	break;
						 case 'C': cellaround++; 	break;
						 case 'e': energie--; if(energie<1) energie=1;	break;
						 case 'E': energie++;		break;
						 case 't': thresholddiv--;	if(thresholddiv<1) thresholddiv=1; break;
						 case 'T': thresholddiv++;	break;
						 case 'r':
						 case 'R':
										energie=ENERGIE;
										thresholddiv=THRESHOLDDIV;
										cellaround=CELLAROUND;
                              break;

						 case 'g': Glow=FALSE; break;
						 case 'G': Glow=TRUE;  break;
						 }
					  }
//					  sprintf(text,"CellsX %d CellsY %d Cellsaround %d Energie %d%% Thresholddiv %d %s FIREFLY",
//									nooffliesx,nooffliesy,cellaround,energie,thresholddiv,Glow?"Glow":"");
					  sprintf(text,"Cellsaround %d Energie %d%% Thresholddiv %d %s FIREFLY",
									cellaround,energie,thresholddiv,Glow?"Glow":"");
					  SetWindowText(hwnd,text);
                return 0;
			 case WM_DESTROY:
					DeleteObject (SelectObject(hwnd,hBrushw)) ;
					DeleteObject (SelectObject(hwnd,hBrushg)) ;
					DeleteObject (SelectObject(hwnd,hBrushy)) ;
					SelectObject(hdcMemory, hbmpOld);
					DeleteDC(hdcMemory);
					DeleteObject(FlyLGBitmap);
					DeleteObject(FlyDGBitmap);
					DeleteObject(FlyBLBitmap);
					PostQuitMessage (0) ;
               return 0 ;
			 }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//------------------------------------ drawflies ------------------------------------------------
void DrawFlies (HWND hwnd)
{
HBRUSH hBrush;
HDC    hdc ;
short  xLeft, xRight, yTop, yBottom;
int m,n;

hdc = GetDC (hwnd) ;
 for(m=0;m<nooffliesy;m++)
for(n=0;n<nooffliesx;n++)
  {
		xLeft   = flies[n+nooffliesx*m].x ;
		xRight  = xLeft+cxClient/(nooffliesx);
		yTop    = flies[n+nooffliesx*m].y;
		yBottom = yTop+cyClient/(nooffliesy);

		if(flies[n+nooffliesx*m].flash==0)
		 {
		   hBrush = hBrushw;
			if(flies[n+nooffliesx*m].status==0) continue;
			else flies[n+nooffliesx*m].status=0;
			if(rectangle)
			{
				SelectObject (hdc, hBrush) ;
				Rectangle (hdc, xLeft,yTop, xRight, yBottom) ;
			}
			else
		   {
				SelectObject(hdcMemory, FlyBLBitmap);
				StretchBlt(hdcbm,xLeft,yTop, cxClient/nooffliesx, cyClient/nooffliesy, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
		   }
 }
		if(flies[n+nooffliesx*m].flash==1)
		 {
	      hBrush = hBrushy;
			flies[n+nooffliesx*m].flash=0;
//			flies[n+nooffliesx*m].energie=0;
			if(flies[n+nooffliesx*m].status==1) continue;
			else flies[n+nooffliesx*m].status=1;
			if(rectangle)
			{
				SelectObject (hdc, hBrush) ;
				Rectangle (hdc, xLeft,yTop, xRight, yBottom) ;
			}
			else
		   {
				SelectObject(hdcMemory, FlyDGBitmap);
				StretchBlt(hdcbm,xLeft,yTop, cxClient/nooffliesx, cyClient/nooffliesy, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
		   }
	 }

	if(flies[n+nooffliesx*m].flash==2)
		 {
	      hBrush = hBrushg;
			flies[n+nooffliesx*m].flash=Glow;
			if(flies[n+nooffliesx*m].status==2) continue;
			else flies[n+nooffliesx*m].status=2;
			if(rectangle)
			{
				SelectObject (hdc, hBrush) ;
				Rectangle (hdc, xLeft,yTop, xRight, yBottom) ;
			}
			else
		   {
				SelectObject(hdcMemory, FlyLGBitmap);
				StretchBlt(hdcbm,xLeft,yTop, cxClient/nooffliesx, cyClient/nooffliesy, hdcMemory, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
		   }
	 }
  }
ReleaseDC (hwnd, hdc) ;
}
//-----------------------calc flash-------------------------------------------------------------
void CalcFlash(HWND hwnd)
{

 int m,n,i,j,u,v,k,l,km,lm;
 int distance,maxdistance;
// HCURSOR hcurSave;

//hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
 maxdistance=2*(cellaround)*(cellaround);
 for(n=0;n<nooffliesx;n++)
	for(m=0;m<nooffliesy;m++)
	 {
	  u=n+nooffliesx*m;
	    if(flies[u].energie>=threshold)   // &&flies[u].flash<1&&flies[u].status<1)
       {
		  k=max(0,n-cellaround);
		  l=max(0,m-cellaround);
		  km=min(nooffliesx-1,n+cellaround);
		  lm=min(nooffliesy-1,m+cellaround);
		  for(i=k;i<=km;i++)
			for(j=l;j<=lm;j++)
    		 {
			  if(i==n && j==m) continue;
			  distance=(i-n)*(i-n)+(j-m)*(j-m);
			  flies[i+nooffliesx*j].genergie+=
(int)(0.5+0.5*((float)(threshold)*((float)maxdistance/(float)distance/(float)maxdistance)))/thresholddiv;
			}
		  flies[u].flash=2;
		  }
	  else   flies[u].genergie+=10;//((energie*threshold/100)-flies[u].energie);  //energie;//2;//random(2);

	  }
// SetCursor(hcurSave);
 for(n=0;n<nooffliesx;n++)
	for(m=0;m<nooffliesy;m++)
	{
	 flies[n+nooffliesx*m].energie+=flies[n+nooffliesx*m].genergie;
	 flies[n+nooffliesx*m].genergie=0;
	  if(flies[n+nooffliesx*m].flash==2)		  flies[n+nooffliesx*m].energie=0;

    }
 hwnd=hwnd;
 }
//----------------------------------- init --------------------------------------------------
int Init(HWND hwnd)
  {
	int m,n,i;
	RECT rc;
	PAINTSTRUCT ps;

	Display_Bitmap(hwnd,hInst);
	GetClientRect(hwnd,&rc);
	cxClient = rc.right ;
   cyClient = rc.bottom;
	hBrushg = CreateSolidBrush (RGB (0, nGreen,0)) ;
	hBrushy = CreateSolidBrush (RGB (0,48,0));//nRed-48, nGreen, nBlue-48)) ;
	hBrushw = CreateSolidBrush (RGB (0,0,0));//nRed, nGreen, nBlue)) ;
	for(n=0;n<nooffliesx;n++)
		for(m=0;m<nooffliesy;m++)
			 {
			  flies[n+nooffliesx*m].x=(cxClient/(nooffliesx))*(n);
			  flies[n+nooffliesx*m].y=(cyClient/(nooffliesy))*(m);
//			  i=threshold*.80-n*m;
//           if(i<0) i=0;
			  flies[n+nooffliesx*m].energie=random(threshold);//random(99);
			  flies[n+nooffliesx*m].status=0;
			  flies[n+nooffliesx*m].flash=0;
			  }
/*			  n=nooffliesx/3;
			  m=nooffliesy/3;
			  flies[n+nooffliesx*m].energie=3200;
			  flies[n+nooffliesx*m].status=-1;
			  flies[n+nooffliesx*m].flash=2;
			  n=nooffliesx*2/3;
			  m=nooffliesy*2/3;
			  flies[n+nooffliesx*m].energie=3200;
			  flies[n+nooffliesx*m].status=-1;
			  flies[n+nooffliesx*m].flash=2;
*/	  return 0;
 }
//--------------------------------------------------------------------------------------------
void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap)
{
	FlyLGBitmap = LoadBitmap(hInstBitmap, "FLYBMPLG");
	FlyDGBitmap = LoadBitmap(hInstBitmap, "FLYBMPDG");
	FlyBLBitmap = LoadBitmap(hInstBitmap, "FLYBMPBL");
	GetObject(FlyLGBitmap, sizeof(BITMAP), &bm);
	hdcbm = GetDC(hwndbm);
	hdcMemory = CreateCompatibleDC(hdcbm);
	ReleaseDC(hwndbm, hdcbm);
  return;
 }

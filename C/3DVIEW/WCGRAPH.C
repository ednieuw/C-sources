/*-----------------------------------------------
	WCGRAPH.C -- Open and Close File Dialog Boxes
  -----------------------------------------------*/

#include "3dview.h"

int 	DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
LRESULT	FAR PASCAL GraphDrawDlgProc (HWND, WORD, WORD, LONG);
void	Print_grafiek(HWND hwnd,HDC hdc,char *PICfilenaam);
EdCOLORREF peekP(long strook,long x,long y);
void pokeP(long strook,long x,long y,EdCOLORREF value);
void ddinit(HWND hwnd,HDC hdc);

extern int geladen;
extern short Knip;
char *PICnaam;

int DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;
	  static char szAppName[] = "Graph";

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	  wndclass.lpfnWndProc	 = (WNDPROC)GraphDrawDlgProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = grInst;
	  wndclass.hIcon         = NULL;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
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


LRESULT FAR PASCAL GraphDrawDlgProc (HWND hwnd, WORD message,
											WORD wParam, LONG lParam)
	  {
	  HDC hdc;
	  short Init;
	  switch (message)
		  {
	  case WM_CREATE:
				  Init=TRUE;
				  geladen=TRUE;
			 return 0 ;
	  case WM_SIZE:
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 return 0 ;

	  case WM_PAINT :
			 hdc = BeginPaint (hwnd, &ps) ;
			 if(Init && !BkgdBMPloaded) {ddinit(hwnd,hdc); Init=FALSE;}
			 Print_grafiek(hwnd,hdc,"magweg");
			 EndPaint(hwnd,&ps);
			 return 0;

	  case WM_KEYDOWN:
	  case WM_RBUTTONDOWN:
								lost=TRUE; //stop showen volgende graphs
	  case WM_LBUTTONDOWN:
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

void	Print_grafiek(HWND hwnd,HDC hdc, char *PICfilenaam)
 {
short t1,t2,n;
//char kleur;

	PICfilenaam=PICfilenaam;

	MaxX = cxClient;
	MaxY = cyClient;					// size of screen
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	GetClientRect (hwnd, &rect) ;
	SetWindowExt(hdc,rect.right, -rect.bottom);
	SetViewportExt (hdc, rect.right,-rect.bottom) ;
	SetViewportOrg(hdc,0,0);
	SetROP2 (hdc, nDrawingMode) ;
	 for(n=0;n<stroken;n++)
		for(t1=0;t1<breedte;t1++)
			for(t2=0;t2<hoogte;t2++)
				{
//					kleur=peekP(n,t1,t2);
//					if(kleur>11) kleur=0; else kleur=15;
//					SetPixel(hdc,(int)(n*breedte)+t1,t2,(EdCOLORREF)dwColor[kleur]);
					SetPixel(hdc,(int)(n*breedte)+t1,t2,peekP(n,t1,t2));
				}
 }
//**************************************************
EdCOLORREF peekP(long strook,long x,long y)
{
 return( P[(int)strook][(long)((breedte*y)+x)] );
}

void pokeP(long strook,long x,long y,EdCOLORREF value)
{
  if(strook>stroken) return;
  P[(int)strook][(long)((breedte*y)+x)] = value;
}

//****************************************************

void ddinit(HWND hwnd,HDC hdc)
{
short t1,t2,n,posx,kolrechts;
EdCOLORREF gpixel;
HCURSOR hcurSave;

hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
MaxX = cxClient;
MaxY = cyClient;					// size of screen
SetMapMode (hdc, MM_ANISOTROPIC) ;
GetClientRect (hwnd, &rect) ;
SetWindowExt(hdc,rect.right, -rect.bottom);
SetViewportExt (hdc, rect.right,-rect.bottom) ;
SetViewportOrg(hdc,0,0);
SetROP2 (hdc, nDrawingMode) ;
//Display_Bitmap(hwnd,hInstBitmap);

if(!SourceBMPloaded)
	{
	Printc(hdc,ScreenX/2,ScreenY/2,8,0,"T E S T");
// Doe verplaatsingen in overige stroken
	 for(n=0;n<stroken-1;n++)
		{
		if(!Knip)
			{
			for(t1=0;t1<breedte;t1++)
				for(t2=0;t2<hoogte;t2++)
					pokeP(n+1,t1,(int)t2,peekP(n,t1,t2));
			}

		for(t1=0;t1<breedte;t1++)
			for(t2=0;t2<hoogte;t2++)
				{
				 gpixel=GetPixel(hdc,(int)(n*breedte+t1),(int)t2);
				 if(gpixel!=(EdCOLORREF)dwColor[WHITE])
					{
					posx=t1+diepte;    kolrechts=1;
					if(posx>breedte)	{ posx-=(int)breedte; kolrechts=2;}
					if(posx<0)			{ posx+=(int)breedte; kolrechts=0;}
					pokeP(n+kolrechts,posx,(int)t2,peekP(n,t1,t2));
					SetPixel(hdc,(int)(n*breedte+t1),(int)t2,(EdCOLORREF)dwColor[BLUE]);
					}
//				 else	SetPixel(hdc,(int)(n),(int)t2,(EdCOLORREF)peekP(n,t1,t2));
				}
		 }
	}
SetCursor(hcurSave);
}


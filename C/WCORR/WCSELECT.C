/*-----------------------------------------------
	WCSELECT.C -- Select two tests
  -----------------------------------------------*/

#include "wcorr.h"
#define ID_MYLISTBOX 101

int 	DoSelectOpenDlg (HANDLE grInst, HWND hwndparent);
LRESULT CALLBACK SelectDlgProc (HWND, WORD, WORD, LONG);
void	SelectTests(HWND hwnd,HDC hdc);

BOOL FAR PASCAL SelectListDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
 {
 WORD m,n;
 char text[50];
	  switch (message)
	  {
	  case WM_INITDIALOG:
		 for(n=0;n<=labels;n++)
			{
			itoa(n+1,text,10);
			strcat(text,"    ");
			text[3]=0;
			strcat(text,(char*)&label[n][1]);
			text[35]=0;
			if(text[3]!=0)
			SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING, 0, (LPARAM) ((LPSTR) text));
			}
	   return TRUE ;

	  case WM_COMMAND:
			 switch (wParam)
				{
				case IDOK:
					m=0;
					for(n=0;n<=labels;n++) // registreer geselecteerde testen
					if(SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_GETSEL,n, 0L))
					test_selection[m++]=n;
					EndDialog (hwnd, 0) ;
					return TRUE ;
				}
		   break ;
	  }
	 lParam=lParam;
	 return FALSE ;
	 }

int DoSelectOpenDlg (HANDLE selInst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;//| CS_SAVEBITS;
	  wndclass.lpfnWndProc   = (long (FAR PASCAL*)())SelectDlgProc;	// Function to retrieve messages for
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = selInst ;
	  wndclass.hIcon         = LoadIcon (selInst, "Wcorr") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = COLOR_WINDOW+1;//GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;		 //szAppName ;
	  wndclass.lpszClassName = "Select";
//     hinst=selInst;
	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow ("Select", NULL,
			  WS_OVERLAPPEDWINDOW,//|WS_THICKFRAME|WS_SYSMENU|WS_CAPTION|WS_DLGFRAME,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  hwndparent, NULL, selInst, NULL) ;
	  ShowWindow (hwnd,  SW_SHOWMAXIMIZED) ;
	 UpdateWindow (hwnd);


	  while (GetMessage (&msg, NULL, 0, 0))
	       {
	       TranslateMessage (&msg) ;
	       DispatchMessage (&msg) ;
	       }
     return msg.wParam ;
	  }

 LRESULT CALLBACK SelectDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
  {
  HDC hdc;
  TEXTMETRIC  tm;
  HANDLE hSelect;
  static FARPROC lpfnSelect;

  switch (message)
		{
	  case WM_CREATE:
			 hdc=GetDC(hwnd);
			 GetTextMetrics(hdc,&tm);
			 ReleaseDC(hwnd,hdc);
			 hSelect = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnSelect = MakeProcInstance (SelectListDlgProc, hSelect) ;
			 wParam=DialogBox(hSelect,"ListSelect",hwnd,lpfnSelect);
			 FreeProcInstance(lpfnSelect);
			 SendMessage(hwnd,WM_CLOSE,0L,0L);
										   return 0 ;
	  case WM_SIZE:
		   cxClient= LOWORD(lParam);
		   cyClient= HIWORD(lParam);       return 0 ;

	  case WM_KEYDOWN:
	  case WM_LBUTTONDOWN:
	  case WM_CLOSE:
		   DestroyWindow (hwnd) ;		   return 0 ;
	  case WM_DESTROY:
		   PostQuitMessage (0) ;		   return 0 ;
	  case WM_QUIT:
		   PostQuitMessage (0) ;		   return 0 ;
	  }
  return DefWindowProc (hwnd, message, wParam, lParam);
  }

void SelectTests(HWND hwnd,HDC hdc)
 {
   SelectObject (hdc, GetStockObject (SYSTEM_FONT)) ;
   GetTextMetrics (hdc, &tm) ;
   cxChar = tm.tmAveCharWidth ;
   cyChar = tm.tmHeight + tm.tmExternalLeading ;
   cxCaps = (tm.tmPitchAndFamily &1 ? 3: 2) * cxChar / 2;
   SetMapMode (hdc, MM_ANISOTROPIC) ;
   GetClientRect (hwnd, &rect) ;
   SetViewportExt (hdc, rect.right, -rect.bottom) ;
   SetViewportOrg(hdc,0,0);					//rect.bottom);
   SetWindowExt(hdc,rect.right, -rect.bottom);
   SetROP2 (hdc, nDrawingMode) ;
   MaxX = cxClient;
   MaxY = cyClient;							// size of screen
  DeleteObject(SelectObject (hdc, GetStockObject (SYSTEM_FONT)));
 }


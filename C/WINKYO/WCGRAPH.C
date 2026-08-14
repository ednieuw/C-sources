/*-----------------------------------------------
	WCGRAPH.C -- Open and Close File Dialog Boxes
  -----------------------------------------------*/

#include "picdump.h"
#define DATUM __DATE__

int 	DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
LRESULT	FAR PASCAL GraphDrawDlgProc (HWND, WORD, WORD, LONG);
void	Print_grafiek(HWND hwnd,HDC hdc,char *PICfilenaam);
void	Print_metafile(HWND hwnd,HDC hdc, char *PICfilenaam);

char *PICnaam;
char drive[MAXDRIVE];
char dir[MAXDIR];
char file[MAXFILE];
char ext[MAXEXT];
int flags;

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
			  WS_OVERLAPPEDWINDOW,
			  //WS_POPUP,
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
	  char szCaption [256] ;
	  switch (message)
		  {
	  case WM_CREATE:
			 wsprintf (szCaption, "%s Release:%9s",selected_files[255],DATUM) ;
			 SetWindowText (hwnd,szCaption ) ;
			 return 0 ;
	  case WM_SIZE:
//			 cxClientOrg=cxClient= LOWORD(lParam);
//			 cyClientOrg=cyClient= HIWORD(lParam);
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 return 0 ;
	  case WM_PAINT :
			 hdc = BeginPaint (hwnd, &ps) ;
			 flags=fnsplit(selected_files[test_selection[0]],drive,dir,file,ext);
			 if(strcmp(ext,".BMP")==0 || strcmp(ext,".RLE")==0)
					do_paint_bmps(hwnd);
			 if (strcmp(ext,".WMF")==0 || strcmp(ext,".EMF")==0)
					Print_metafile(hwnd,hdc,selected_files[255]);
			 else Print_grafiek(hwnd,hdc,selected_files[255]);
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
	 HDC      hDCMeta;
	 HANDLE   hMeta;
	 char s[MAXPATH+20];
	 int disk;

	MaxX = cxClient;
	MaxY = cyClient;					// size of screen
	SetMapMode 		(hdc, MM_ANISOTROPIC) ;
	GetClientRect 	(hwnd, &rect) ;
	SetWindowExt	(hdc,rect.right, -rect.bottom);
	SetViewportExt (hdc, rect.right,-rect.bottom) ;
	SetViewportOrg	(hdc,0,0);
	SetROP2 			(hdc, nDrawingMode) ;
//	flags=fnsplit(selected_files[test_selection[0]],drive,dir,file,ext);
	flags=fnsplit(PICfilenaam,drive,dir,file,ext);
	if (strcmp(ext,".WMF")==0 || strcmp(ext,".EMF")==0)
		{  // als geen Pic file maar een metafile print dan deze
		Print_metafile(hwnd,hdc,PICfilenaam);
		return;
		}
	else ReadPic(hdc,PICfilenaam);

	//disk=getdisk()+'A';
	drive[0]=0;
	getcwd(dir, MAXPATH);
	strcpy(ext,"wmf");
	fnmerge(s,drive,dir,file,ext);
	hDCMeta = CreateMetaFile(s);// (LPSTR) "sample.wmf");
	SetMapMode (hDCMeta, MM_ANISOTROPIC) ;
	GetClientRect (hwnd, &rect) ;
	SetWindowExt(hDCMeta,rect.right, rect.bottom);
	SetROP2 (hDCMeta, nDrawingMode) ;
	ReadPic(hDCMeta,PICfilenaam);
	CloseMetaFile(hDCMeta);
  }


 void	Print_metafile(HWND hwnd,HDC hdc, char *PICfilenaam)
 {
	HANDLE hmf;
	MaxX = cxClient;
	MaxY = cyClient;
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	GetClientRect (hwnd, &rect) ;
	SetWindowExt(hdc,rect.right, rect.bottom);
	SetViewportExt (hdc, rect.right,rect.bottom) ;
	SetViewportOrg(hdc,0,0);
	SetROP2 (hdc, nDrawingMode) ;

	hmf = GetMetaFile(PICfilenaam);
	PlayMetaFile(hdc, hmf);
	DeleteMetaFile(hmf);
 }

/*-----------------------------------------------
	WCSELECT.C -- Select two tests
  -----------------------------------------------*/

#include "Blast.h"
#define ID_MYLISTBOX 101

void	Makedate(int *m,int *d,int *y,unsigned total);
int 	Sort_function( const void *a, const void *b);
BOOL 	FAR PASCAL SelectListDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);
int 	DoSelectOpenDlg (HANDLE selInst, HWND hwndparent);
LRESULT CALLBACK SelectDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam);

struct 	ffblk ffblk;

void Makedate(int *m,int *d,int *y,unsigned total)
{
	*m=(total>>5)&0XF;
	*d=total & 0X1F;
	*y=(total>>9)&0X3F;
	*y+=1980;
}

int Sort_function( const void *a, const void *b)
{
	return(0-strcmp( (char *)a, (char *)b));
}

#define MAX_FILES 150
BOOL FAR PASCAL SelectListDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
 {
 WORD m,n;
 int done,year,month,day;
 char nopfile[MAX_FILES+2][25];
 struct date d;
 long Today,Filedate;

 getdate(&d);
 Today=(d.da_year)*365+d.da_mon*30+d.da_day;

 switch (message)
	  {
	  case WM_INITDIALOG:
			n=0;
			done = findfirst("*.WK1",&ffblk,0); //FA_ARCH
			while (!done||n>MAX_FILES)
			{
			Makedate(&month, &day, &year, (unsigned)ffblk.ff_fdate);
			Filedate=year*365+month*30+day;
			if((Today-Filedate)<Max_dagen_oud)  // alleen files die de laatste maand veranderd zijn
				sprintf((char*)nopfile[n++],"%2.2d-%2.2d%2.2d:%s",
										   year,month,day,ffblk.ff_name);
			done = findnext(&ffblk);
			}
		   Nooffiles=n; //quicksort  werkt niet goed met (char huge*)
		   qsort(nopfile[0], Nooffiles, sizeof(nopfile[0]), Sort_function);
			for(n=0;n<Nooffiles;n++) strcpy((char*)label[n],nopfile[n]+10);
			for(n=0;n<Nooffiles;n++) SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
								0, (LPARAM) ((LPSTR)nopfile[n]+5)); //label[n]));
			return TRUE ;

	  case WM_COMMAND:
		   switch (wParam)
			{
			 case IDOK:
				 m=0;
				 geladen=0;
				 for(n=0;n<Nooffiles;n++) // registreer geselecteerde testen
					if(SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_GETSEL,n, 0L))
						test_selection[m++]=n;
					EndDialog (hwnd, 0) ;
					geladen=TRUE;
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
//	  FARPROC lpfnSelectDlgProc ;

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;//| CS_SAVEBITS;
	  wndclass.lpfnWndProc   = (long (FAR PASCAL*)())SelectDlgProc;	// Function to retrieve messages for
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = selInst ;
	  wndclass.hIcon         = LoadIcon (selInst, "Blast") ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = COLOR_WINDOW+1;//GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = "Select";
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
  HANDLE hSelect;
  static FARPROC lpfnSelect;

  switch (message)
		{
	  case WM_CREATE:
		   hSelect = ((LPCREATESTRUCT) lParam)->hInstance ;
		   lpfnSelect = MakeProcInstance (SelectListDlgProc, hSelect) ;
		   wParam=DialogBox(hSelect,"ListSelect",hwnd,lpfnSelect);
		   FreeProcInstance(lpfnSelect);
		   SendMessage(hwnd,WM_CLOSE,0L,0L);
		   return 0 ;
	  case WM_SIZE:
		   cxClient= LOWORD(lParam);
		   cyClient= HIWORD(lParam);
		   return 0 ;

	  case WM_KEYDOWN:
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
//     default:
	  }
	  return DefWindowProc (hwnd, message, wParam, lParam) ;
  }


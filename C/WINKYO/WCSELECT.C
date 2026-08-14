/*-----------------------------------------------
	WCSELECT.C -- Select Picfiles
  -----------------------------------------------*/
#include "picdump.h"
#define ID_MYLISTBOX 101

LRESULT 	FAR PASCAL SelectDlgProc (HWND, WORD, WORD, LONG);
void     Print_files_indialog(HWND hwnd);
int 		sort_function( const void *a, const void *b);

int 		DoSelectOpenDlg (HANDLE grInst, HWND hwndparent);
char 		selected_files[256][14];
int 		noofpicfiles,nooffiles;

BOOL FAR PASCAL SelectListDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)

 {
 WORD 		m,n;
 struct 		ffblk ffblk;
 int 			done;
 char 		text[20];
 // unsigned 	drive;

	  switch (message)
	  {
	  case WM_INITDIALOG:
 							done = findfirst("*.?MF",&ffblk,0);
							while (!done)
								{
//								SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
//											 0, (LPARAM) ((LPSTR) ffblk.ff_name));
								strcpy(selected_files[nooffiles++],ffblk.ff_name);
								if(nooffiles>254) break;
								done = findnext(&ffblk);
								}
							done = findfirst("*.PIC",&ffblk,0);
							while (!done)
								{
//								SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
//											 0, (LPARAM) ((LPSTR) ffblk.ff_name));
								strcpy(selected_files[nooffiles++],ffblk.ff_name);
								if(nooffiles>254) break;
								done = findnext(&ffblk);
								}
							done = findfirst("*.BMP",&ffblk,0);
							while (!done)
								{
//								SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
//											 0, (LPARAM) ((LPSTR) ffblk.ff_name));
								strcpy(selected_files[nooffiles++],ffblk.ff_name);
								if(nooffiles>254) break;
								done = findnext(&ffblk);
								}
							done = findfirst("*.RLE",&ffblk,0);
							while (!done)
								{
//								SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
//											 0, (LPARAM) ((LPSTR) ffblk.ff_name));
								strcpy(selected_files[nooffiles++],ffblk.ff_name);
								if(nooffiles>254) break;
								done = findnext(&ffblk);
								}
							qsort(selected_files, nooffiles, sizeof(selected_files[0]),
									 sort_function);
							Print_files_indialog(hwnd);
							return TRUE ;

	  case WM_COMMAND:
			 switch (wParam)
			 {

			 case IDC_MOVE:
							if(nooffiles>254)
								{
									MessageBox (hwnd,
									"Maximum of 255 files reached\n No more files will be copied",
									"Maximum exceeded",	MB_OK | MB_ICONSTOP);
                           return 0;
								}
							for(n=0;n<nooffiles;n++) // registreer geselecteerde test
							  if(SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_GETSEL,n, 0L))
									{
									strcpy(text,selected_files[n]);
									break;
									}
								SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_INSERTSTRING,
										0, (LPARAM) ((LPSTR) text));
								nooffiles++;
								for(n=nooffiles;n>0;n--)
									strcpy(selected_files[n],selected_files[n-1]);
								strcpy(selected_files[0],text);
							return 0;

			 case IDOK:
							m=0;
							for(n=0;n<nooffiles;n++) // registreer geselecteerde testen
								if(SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_GETSEL,n, 0L))
									test_selection[m++]=n;
							noofpicfiles=m;
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

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	  wndclass.lpfnWndProc   = (WNDPROC)SelectDlgProc;	// Function to retrieve messages for
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = selInst ;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = COLOR_WINDOW+1;//GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = "Select";
	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow ("Select", NULL,
									WS_OVERLAPPEDWINDOW,
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

LRESULT FAR PASCAL SelectDlgProc (HWND hwnd, WORD message,WORD wParam, LONG lParam)
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
			 nooffiles=0;
			 SendMessage(hwnd,WM_CLOSE,0L,0L);
			 return 0 ;
	  case WM_SIZE:
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 return 0 ;

//	  case WM_KEYDOWN:
//	  case WM_LBUTTONDOWN:
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
	  return  DefWindowProc (hwnd, message, wParam, lParam) ;
  }
// ----------------------------------------------------------------------
void     Print_files_indialog(HWND hwnd)
{
int n;
//char text[80];

	for(n=0;n<nooffiles;n++)
		{
/*		itoa(n+1,text,10);
		strcat(text," ");
		strcat(text,selected_files[n]);
		SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
							 0, (LPARAM) ((LPSTR) text));
*/
		SendDlgItemMessage(hwnd, ID_MYLISTBOX, LB_ADDSTRING,
							 0, (LPARAM) ((LPSTR) selected_files[n]));
		}

}

int sort_function( const void *a, const void *b)
{
	return( strcmp((char *)a,(char *)b) );
}


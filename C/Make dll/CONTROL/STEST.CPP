/********************************************************
 * stest -- test a spin button				*
 ********************************************************/
#define STRICT

#include <windows.h>
#include <bwcc.h>
#include <time.h>

#include "spin.h"
#include "stest.h"

/********************************************************
 * main_window_proc -- procedure to handle main window	*
 *							*
 * Parameters (windows usual)				*
 *	hWnd -- dialog window handle			*
 *	wMessage -- message to be handled		*
 *	wParam, lParam -- parameters to the message	*
 *							*
 * Return						*
 *	Message dependent				*
 ********************************************************/
BOOL CALLBACK main_window_proc(HWND hWnd, 
	UINT wMessage, WPARAM wParam, LPARAM lParam)
{
    (void)hWnd;	// Avoid warning
    (void)lParam; // Avoid warning

    switch(wMessage) {
       case WM_INITDIALOG:
	   SendDlgItemMessage(hWnd, TEST_BUTTON, 
	   	SP_ADD_TEXT, 0, (LPARAM)"First");
	   SendDlgItemMessage(hWnd, TEST_BUTTON, 
	   	SP_ADD_TEXT, 0, (LPARAM)"Second");
	   SendDlgItemMessage(hWnd, TEST_BUTTON, 
	   	SP_ADD_TEXT, 0, (LPARAM)"Third");
	   return TRUE;

       case WM_COMMAND :
             switch(wParam) {
		 case IDOK:
		     PostQuitMessage(0);
		     return TRUE;
		 default:
		     break;
             }

       case WM_SYSCOMMAND:
          switch(wParam & 0xFFF0) {
              case SC_CLOSE:
                PostQuitMessage(0);
                return TRUE;
          }
          break;

       case WM_DESTROY:
          PostQuitMessage(0);
          break;

       default:
          return FALSE;
   }
   return FALSE;
}

/********************************************************
 * WinMain -- main window				*
 *							*
 * Parameters (windows usual)				*
 *	hInstance -- instance data for the application	*
 *	hPrevInstance -- pointer to previous instance	*
 *	lpszCmdLine -- command line			*
 *	nCmdShow -- show window option			*
 *							*
 * Returns						*
 *	exit status					*
 ********************************************************/
int PASCAL WinMain(HINSTANCE hInstance,         
                   HINSTANCE hPrevInstance,    
                   LPSTR  lpszCmdLine,        
                   int    nCmdShow)          
{
    MSG      msg;		// Current message 
    HWND     main_window_handle;	// Main window handle
    HMODULE borland_library_handle;// Borland library handle
    HMODULE spin_library_handle; // Handle for spin library

    (void)hPrevInstance;	// Avoid warning
    (void)lpszCmdLine;

    borland_library_handle = LoadLibrary("bwcc.dll");
    if((UINT)borland_library_handle <= 32) {
	MessageBox(NULL, 
	    "Unable to load Borland Controls", 
	    "System Error", MB_OK | MB_ICONHAND);
	return (FALSE);
    }

    spin_library_handle = LoadLibrary("spin.dll");
    if((UINT)spin_library_handle <= 32) {
	MessageBox(NULL, 
	    "Unable to load Spin Controls", 
	    "System Error", MB_OK | MB_ICONHAND);
	FreeLibrary(borland_library_handle);
	return (FALSE);
    }

    main_window_handle = CreateDialog(hInstance,
		   (LPCSTR)"MAIN_DIALOG",
		   NULL,
		   main_window_proc);

    if(main_window_handle == NULL) {
	MessageBox(NULL, 
	    "Unable to display main dialog", 
	    "System Error", MB_OK);
	return FALSE;
    }

    ShowWindow(main_window_handle, nCmdShow);
    UpdateWindow(main_window_handle);

    while(GetMessage(&msg, NULL, 0, 0))
	if(!IsDialogMessage(main_window_handle, &msg)) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

    FreeLibrary(borland_library_handle);
    FreeLibrary(spin_library_handle);
    return msg.wParam;
}

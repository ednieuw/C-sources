#include "commdlg.h"
#include "wcorr.h"

char szName[256];
char Loaded_file_name[256];
/*********************************************************************
Using the OPENFILENAME structure and the Windows 3.1 API call
GetOpenFileName() eases the selection of files for the programmer and for
the user.  The WINHELP.EXE help file WIN31WH.HLP (found in the BORLANDC\BIN
directory) contains a detailed description of the function call and its
associated structure.  The Flags field of the structure is particularly
useful when custimization is required.
**********************************************************************/
BOOL CMUFileOpen( HWND hWnd )
{
	OPENFILENAME ofnTemp;
	DWORD Errval;	// Error value
	char buf[5];	// Error buffer
	char Errstr[50]="GetOpenFileName returned Error #";
	char szTemp[] = "Lotus Files (*.WK1)\0*.WK1\0All Files (*.WK?)\0*.WK?\0";
	BOOL gelukt=TRUE;
/*
Note the initialization method of the above string.  The GetOpenFileName()
function expects to find a string in the OPENFILENAME structure that has
a '\0' terminator between strings and an extra '\0' that terminates the
entire filter data set.  Using the technique shown below will fail because
"X" is really 'X' '\0' '\0' '\0' in memory.  When the GetOpenFileName()
function scans szTemp it will stop after finding the extra trailing '\0'
characters.

	char szTemp[][4] = { "X", "*.*", "ABC", "*.*", "" };

The string should be "X\0*.*\0ABC\0*.*\0".

Remember that in C or C++ a quoted string is automatically terminated with
a '\0' character so   char "X\0";   would result in 'X' '\0' '\0' which
provides the extra '\0' that GetOpenFileName() needs to see in order to
terminate the scan of the string.  Just 'char ch "X";' would result in 'X'
'\0' and GetOpenFileName() would wander off in memory until it lucked into
a '\0' '\0' character sequence.
*/

/*
Some Windows structures require the size of themselves in an effort to
provide backward compatibility with future versions of Windows.  If the
lStructSize member is not set the call to GetOpenFileName() will fail.
*/
	ofnTemp.lStructSize = sizeof( OPENFILENAME );
	ofnTemp.hwndOwner = hWnd;	// An invalid hWnd causes non-modality
	ofnTemp.hInstance = 0;
	ofnTemp.lpstrFilter = (LPSTR)szTemp;	// See previous note concerning string
	ofnTemp.lpstrCustomFilter = NULL;
	ofnTemp.nMaxCustFilter = 0;
	ofnTemp.nFilterIndex = 1;
	ofnTemp.lpstrFile = (LPSTR)szName;	// Stores the result in this variable
	ofnTemp.nMaxFile = sizeof( szName );
	ofnTemp.lpstrFileTitle = NULL;
	ofnTemp.nMaxFileTitle = 0;
	ofnTemp.lpstrInitialDir = NULL;
	ofnTemp.lpstrTitle = "OPENFILENAME";	// Title for dialog
	ofnTemp.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofnTemp.nFileOffset = 0;
	ofnTemp.nFileExtension = 0;
	ofnTemp.lpstrDefExt = "WK?";
	ofnTemp.lCustData = NULL;
	ofnTemp.lpfnHook = NULL;
	ofnTemp.lpTemplateName = NULL;
/*
If the call to GetOpenFileName() fails you can call CommDlgExtendedError()
to retrieve the type of error that occured.
*/
	if(GetOpenFileName( &ofnTemp ) != TRUE)
	{
		Errval=CommDlgExtendedError();
		if(Errval!=0)	// 0 value means user selected Cancel
		{
			sprintf(buf,"%ld",Errval);
			strcat(Errstr,buf);
			MessageBox(hWnd,Errstr,"WARNING",MB_OK|MB_ICONSTOP);
         gelukt=FALSE;
		}

	}
	InvalidateRect( hWnd, NULL, TRUE );	// Repaint to display the new name
 lstrcpy(Loaded_file_name,szName);
 if(Errval==0) gelukt=FALSE;
 return (gelukt);
}



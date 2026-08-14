#include "commdlg.h"
#include "picdump.h"
#include "direct.h"
#include "shellapi.h"

static struct
	{
	unsigned char ExtCode;
	char Res1[5];
	unsigned char Code;
	unsigned char OldDrive;
	char OldName[11];
	char Res2[5];
	char NewdName[11];
	char Res3[9];
	}
//DirFcb = {'\xFF',"",'\x10'},
VolFcb = {'\xFF',"",'\x08','\0',"???????????"};

static struct
	{
	char Res1[8];
	char OldVolid[11];
	char Res2[5];
	char NewVolid[11];
	char Res3[9];
	} Dta;
// ------------------------------ function prototypes ------------------------------
char *FilGetVolid(int Drive);
int  FilSetVolid (HWND hwnd, int Drive, char *Volid);
void Read_Diskvolume(int drive,char* diskname);
void Set_Diskvolume(HWND hwnd, int drive,char* diskname);
BOOL CMUFileOpen( HWND hWnd );
int  Set_lowest_drive(unsigned* olddrive);

char szName[256];
char Loaded_file_name[256];


//------------------------------- Read_Diskvolume ------------------------------------------
void Read_Diskvolume(int drive,char* diskname)
{
 char *volnaam;

 volnaam=FilGetVolid( drive);
 if(volnaam!=NULL) strncpy(diskname,volnaam,11);
}

//------------------------------- Set_Diskvolume ------------------------------------------

void Set_Diskvolume(HWND hwnd, int drive,char* diskname)
{
	FilSetVolid (hwnd,drive, diskname);
}
//------------------------------- Read_Diskvolume ------------------------------------------

char *FilGetVolid(int Drive)
{
 int ErrorCode;

 _DX = (unsigned int) &Dta;
 _AH = 0x1a;
 geninterrupt(0x21);

 VolFcb.OldDrive = Drive;
 _DX = (unsigned int) &VolFcb;
 _AH = 0x11;
 geninterrupt(0x21);
 ErrorCode = _AL;
 if(ErrorCode == 0) {Dta.OldVolid[12]=0;	return (Dta.OldVolid);}
 else 					return(NULL);
}

//------------------------------- Set_Diskvolume ------------------------------------------
int FilSetVolid (HWND hwnd,int Drive, char *Volid)
{
 int i;
 int ErrorCode;

 if(FilGetVolid(Drive)==NULL)
	{

	char text[20];
	sprintf(text,"%c: %s",Drive+'A'-1,Volid);
	MessageBox (hwnd, "Do not panic\nscreen will be blackened", "Disk labeling ",MB_OK | MB_ICONHAND);
	if (ShellExecute(hwnd,"open","LABEL",text,"c:\dos",SW_SHOWMINIMIZED) < 32)
			 {
			 sprintf(text, "Volumelabel failed, program LABEL not found?");
			 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
			 }
/*	for(i=0;i<11;++i)
		if(*Volid == '\0')			VolFcb.OldName[i] = ' ';
		else              			VolFcb.OldName[i] = *Volid++;
	_DX = (unsigned int) &VolFcb;
	_AH = 0x16;
	geninterrupt(0x21);
	ErrorCode = _AL;

	for(i=0;i<11;++i)
		if(*Volid == '\0')			Dta.NewVolid[i] = ' ';
		else              			Dta.NewVolid[i] = *Volid++;
	_DX = (unsigned int) &Dta;
	_AH = 0x16;
	geninterrupt(0x21);
	ErrorCode = _AL;
*/
	}
 else
	{
	for(i=0;i<11;++i)
		if(*Volid == '\0')			Dta.NewVolid[i] = ' ';
		else              			Dta.NewVolid[i] = *Volid++;
	_DX = (unsigned int) &Dta;
	_AH = 0x17;
	geninterrupt(0x21);
	ErrorCode = _AL;
	}

 return(0);

 }


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
	char szTemp[] = "PIC Files (*.PIC)\0*.PIC;*.BMP;*.BMP\0All Files (*.*)\0*.*\0";
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
	ofnTemp.lpstrTitle = "Use SELECT in main menu for multiple selection";	// Title for dialog
	ofnTemp.Flags =  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;//OFN_FILEMUSTEXIST |
	ofnTemp.nFileOffset = 0;
	ofnTemp.nFileExtension = 0;
	ofnTemp.lpstrDefExt = "PIC";
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


int Set_lowest_drive(unsigned *olddrive)
{
 int save, disk,disks;

	/* save original drive */
	save = getdisk();

	/* print number of logic drives */
	//disks =
	setdisk(save);
	/* print the drive letters available */
	for (disk = 0;disk < 2;++disk)
	{
		setdisk(disk);
		if (disk == getdisk()) break;
	}
//	setdisk(save);
	olddrive=(unsigned*)(disk+1);
	return 1;
	}


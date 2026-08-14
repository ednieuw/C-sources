/********************************************************
 * bmp -- view and print bitmaps			*
 ********************************************************/
//#define STRICT
#include "picdump.h"
#include <bwcc.h>
#include <commdlg.h>
#include <mem.h>
//#include "bmp.h"
//#include "clip.h"

HINSTANCE Instance;	// Our data instance

// Status type
enum X_STATUS {X_ERROR, X_OK};
/*
 * Bitmap variables
 *	We store our bitmap in two parts
 *
 *	The BITMAPFILEHEADER and
 *		the rest of the data. (raw_global_handle)
 *
 * The rest of the data consists of the following sections.
 * Each has it's own pointer that points to them
 *
 *	BITMAPINFOHEADER		(bitmap_info_header_ptr)
 *	RGBQUAD (n_colors long)		(color_map_ptr)
 *	BITS				(bitmap_data_ptr)
 *
 * Additionally, the combination of the BITMAPINFOHEADER and
 *	RGBQUAD is called the BITMAPINFO (bitmap_info_ptr).
 *
 * Warning: These pointers are not valid unless the memory has
 *	been locked with the "lock_bitmap" call.
 */
// File header
BITMAPFILEHEADER   bitmap_file_header;
// Rest of the data packed together
HGLOBAL raw_global_handle = NULL;
BYTE FAR *raw_ptr = NULL;

// Pointer to header (points into raw_global_handle's data)
static BITMAPINFOHEADER *bitmap_info_header_ptr = NULL;
// Pointer to color table (points into raw_global_handle's data)
static RGBQUAD *color_map_ptr = NULL;
// Pointer to bit array (points into raw_global_handle's data)
static BYTE FAR *bitmap_data_ptr = NULL;

// Pointer to bitmap header information
#define bitmap_info_ptr ((BITMAPINFO *) bitmap_info_header_ptr)

/*
 * Number of colors in the color map
 */
static DWORD n_colors;

// File name of the current file we are working on
char bmpfile_name[512] = "";

// The BM that begins each bitmap file
#define DIB_MAGIC ('B' | ('M' << 8))

// Information about the print dialog
//static HICON hour_glass_cursor; // handle to hourglass cursor

//-------------------------------------------------------------------



/********************************************************
 *  free_old_bmp -- free the old bitmap if there was	*
 *		one allocated				*
 ********************************************************/
static void free_old_bmp(void)
{
	 if (raw_global_handle != NULL) {
		  GlobalFree(raw_global_handle);
	raw_global_handle = NULL;
	bitmap_info_header_ptr = NULL;
	color_map_ptr = NULL;
	bitmap_data_ptr = NULL;
	raw_ptr = NULL;
	 }
	 memset(&bitmap_file_header, '\0', sizeof(bitmap_file_header));
}

/********************************************************
 * lock_bitmap -- lock the bitmap's memory and setup	*
 *			pointers.			*
 *							*
 * Computes the pointers into this array and		*
 *	checks the for validity.			*
 *							*
 * Returns						*
 *	OK -- this looks like a device independent 	*
 *			bitmap				*
 *	ERROR -- this is not a bitmap (message done)	*
 ********************************************************/
//static X_STATUS lock_bitmap(void)
static enum X_STATUS lock_bitmap(void)
{
	 BYTE FAR *cur_ptr;	// What we're pointing to now
	 if(raw_ptr==NULL) return(X_ERROR);
	 cur_ptr = raw_ptr;

	 bitmap_info_header_ptr = (BITMAPINFOHEADER *)cur_ptr;
	 cur_ptr += sizeof(BITMAPINFOHEADER);

	 // Check for correct structure size
	 //   Older style OS/2 bitmaps will have a different value in
	 //		in this field.
	 //		(We don't do OS/2, we do windows)
	 if (bitmap_info_header_ptr->biSize !=
		sizeof(BITMAPINFOHEADER)) {
		  MessageBox(NULL,
		 "Error: Illegal structure size "
		"found in info header",
		 "File error", MB_ICONSTOP | MB_OK);
	return(X_ERROR);
	 }

	 n_colors = bitmap_info_header_ptr->biClrUsed;

	 if (n_colors == 0)
		{
	// 24 bit colors need no table, all others do
		if (bitmap_info_header_ptr->biBitCount != 24)
		 {
		 // Number of colors is 2**<number of bits per color>
//		 n_colors = 1 << bitmap_info_header_ptr->biBitCount;
		 n_colors = 2* bitmap_info_header_ptr->biBitCount;
		 }
	 }

	 /*	fill in some default values if they are zero */
	 if (bitmap_info_header_ptr->biClrUsed == 0)
			 bitmap_info_header_ptr->biClrUsed = n_colors;

	 if (bitmap_info_header_ptr->biSizeImage == 0) {
	  /* Compute the number of bytes in the image
		*
		*  biWidth * biBitCount ==
		*		number of bits per scan line
		*  Next adjust because we want to round
		*  to next 4 byte word and convert to words
		*     (biWidth * biBitCount + 31) / 32
		*  Convert #4 bytes words to bytes
		*		*4
		*  Finally, multiply scanline byte
		*  size by # scan lines
		*/
	  bitmap_info_header_ptr->biSizeImage =
		  (((bitmap_info_header_ptr->biWidth *
			(DWORD)bitmap_info_header_ptr->biBitCount +
				31L) / 32) * 4)
			 *   bitmap_info_header_ptr->biHeight;
	 }

	 // Read in the color map
	 color_map_ptr = (RGBQUAD *)cur_ptr;
	 cur_ptr += (sizeof(RGBQUAD) * n_colors);

	 if (bitmap_file_header.bfOffBits != 0L)
		  cur_ptr = raw_ptr + (int)(bitmap_file_header.bfOffBits -
		  sizeof(bitmap_file_header));

	 bitmap_data_ptr = cur_ptr;

	 return (X_OK);
}

/********************************************************
 * unlock_bitmap -- unlock data locked by lock_bitmap	*
 ********************************************************/
static void unlock_bitmap(void)
{
	 GlobalUnlock(raw_global_handle);
//	GlobalUnlock(raw_mem_handle);

	 bitmap_info_header_ptr = NULL;
	 color_map_ptr = NULL;
	 bitmap_data_ptr = NULL;
	 raw_ptr = NULL;
}

/********************************************************
 * read_bmp -- read in a bitmap				*
 *							*
 * Parameters						*
 *	hWnd -- handle of the main window		*
 *	in_file -- the file to read			*
 *							*
 * Returns						*
 *	X_OK -- file read in				*
 *	X_ERROR -- problem with the file		*
 *		(error message issued)			*
 ********************************************************/
static enum X_STATUS read_bmp(HWND hWnd, int in_file)
{
	 DWORD data_size;	// Size of the guts of the file

	 // Now read the file
	 if (_hread(in_file, &bitmap_file_header,
			sizeof(bitmap_file_header)) !=
			sizeof(bitmap_file_header)) {
		  MessageBox(hWnd,
		 "Error: Unable to read bitmap file header",
		 "Read error", MB_ICONSTOP | MB_OK);
	return(X_ERROR);
	 }

	 if (bitmap_file_header.bfType != DIB_MAGIC) {
	// Error string for MessageBox
	char error_msg[100];

	wsprintf(error_msg,
		 "Bad magic number (0x%04x) in bitmap",
		 bitmap_file_header.bfType);
		  MessageBox(hWnd, error_msg,
			"Read error", MB_ICONSTOP|MB_OK);
	return(X_ERROR);
	 }

	 data_size = bitmap_file_header.bfSize - sizeof(BITMAPFILEHEADER);
	 raw_global_handle = GlobalAlloc(GMEM_MOVEABLE, data_size);
	 if (raw_global_handle == NULL)
	 {
		  MessageBox(hWnd,
		 "Error: Unable to read bitmap info header",
		 "Read error", MB_ICONSTOP | MB_OK);
		  return (X_ERROR);
	 }
	// Handle to our raw bitmap
	 raw_ptr = (BYTE FAR*) GlobalLock(raw_global_handle);
	//	class GlobalMemoryData raw_mem(raw_global_handle);

	if (_hread(in_file, raw_ptr, data_size) != data_size)
	 {
		 MessageBox(hWnd,
		"Error: Unable to allocate space for bitmap",
		"Read error", MB_ICONSTOP | MB_OK);
		 return(X_ERROR);
	 }
  if (lock_bitmap() == X_ERROR) {	unlock_bitmap();	return(X_ERROR);	 }
//  unlock_bitmap();
 return (X_OK);
}

/********************************************************
 * do_open -- handle the open command			*
 *							*
 * Get a file name, then get the file data		*
 *							*
 * Parameters						*
 *	hWnd -- window handle to use for the open	*
 ********************************************************/
void do_open(HWND hWnd,char *info)
{
	 // Structure for the open information
	 OPENFILENAME open_file_name;

	 int in_file;		// FD of input file

	 bmpfile_name[0] = '\0';
	 memset(&open_file_name, '\0', sizeof(open_file_name));
	 open_file_name.lStructSize = sizeof(open_file_name);
	 open_file_name.hwndOwner = hWnd;
	 open_file_name.hInstance = NULL;
	 open_file_name.lpstrFilter =
		"Bitmaps (*.bmp)\0*.bmp\0";
	 open_file_name.lpstrCustomFilter = NULL;
	 open_file_name.nMaxCustFilter = 0;
	 open_file_name.nFilterIndex = 0;
	 open_file_name.lpstrFile = bmpfile_name;
	 open_file_name.nMaxFile = sizeof(bmpfile_name);
	 open_file_name.lpstrFileTitle = NULL;
	 open_file_name.nMaxFileTitle = 0;
	 open_file_name.lpstrInitialDir = NULL;
	 open_file_name.lpstrTitle = info;//"Select Bitmap File";
	 open_file_name.Flags = OFN_PATHMUSTEXIST|
	OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_SHOWHELP;
	 open_file_name.nFileOffset = 0;
	 open_file_name.nFileExtension = 0;
	 open_file_name.lpstrDefExt = "BMP";
	 open_file_name.lCustData = NULL;
	 open_file_name.lpfnHook = NULL;
	 open_file_name.lpTemplateName = NULL;

	if (GetOpenFileName(&open_file_name) == 0)
	 {
		// Character version of error message
		char error_msg[200];
		DWORD error;	// Error from common dialog

		error = CommDlgExtendedError();
		if (error == 0L)	    return;
		wsprintf(error_msg,"GetOpenFileName error 0x%lx", error);
		MessageBox(hWnd, error_msg,"Open error", MB_ICONSTOP | MB_OK);
		return;
	 }

	free_old_bmp();
	in_file = _lopen(bmpfile_name,READ);

	 if (in_file < 0)
	  {
		char error_msg[100];  // Message for error

		wsprintf(error_msg,"Unable to open file '%s'", bmpfile_name);
		MessageBox(hWnd, error_msg,"Open error", MB_ICONSTOP | MB_OK);
		return;
	 }

	 if (read_bmp(hWnd, in_file) == X_ERROR) {free_old_bmp();}

	 _lclose(in_file);
	 InvalidateRect (hWnd, NULL, TRUE);
}

/********************************************************
 * do_paint -- paint the bitmap on the window		*
 *							*
 * Parameters						*
 *	hWnd -- windows handle				*
 ********************************************************/
void do_paint_bmps(HWND hWnd)
{
// PAINTSTRUCT paint_struct;	// What to paint
 int in_file;		// FD of input file
 int n;
 int xorg=0,yorg=0;
 int xdiv,ydiv,incx=0,incy=0;

// BeginPaint(hWnd, &paint_struct);
// InvalidateRect (hWnd, NULL, TRUE);

for(n=0;n<noofgrpage;n++)
 {
	if(LastGrButton==IDM_NOOFGR1) {xdiv=1;ydiv=1;}
	if(LastGrButton==IDM_NOOFGR2) {xdiv=2;ydiv=1;}
	if(LastGrButton==IDM_NOOFGR6) {xdiv=3;ydiv=2;}
	if(LastGrButton==IDM_NOOFGR8) {xdiv=4;ydiv=2;}
	if(LastGrButton==IDM_NOOFGR9) {xdiv=3;ydiv=3;}
	if(LastGrButton==IDM_NOOFGR12) {xdiv=4;ydiv=3;}

//	free_old_bmp();
	in_file = _lopen(selected_files[test_selection[n]],READ);
	 if (in_file < 0)
	  {
		char error_msg[100];  // Message for error
		wsprintf(error_msg,"Unable to open file '%s'", bmpfile_name);
		MessageBox(hWnd, error_msg,"Open error", MB_ICONSTOP | MB_OK);
		return;
	 }
	 if (read_bmp(hWnd, in_file) == X_ERROR) {free_old_bmp();}
	 _lclose(in_file);

 if (raw_global_handle != NULL)
  {
	lock_bitmap();
		SetStretchBltMode(hWnd,STRETCH_DELETESCANS);
		StretchDIBits(ps.hdc,
			xorg				,yorg,
			cxClient/xdiv	,cyClient/ydiv,
			0					, 0,
			(int)bitmap_info_header_ptr->biWidth,
			(int)bitmap_info_header_ptr->biHeight,
			bitmap_data_ptr,
			(LPBITMAPINFO)bitmap_info_ptr, DIB_RGB_COLORS,SRCCOPY);

	unlock_bitmap();
	}
	xorg+=cxClient/xdiv;
	 if(++incx>=xdiv)
		 {
		  incy++;
		  xorg=0;
		  yorg+=cyClient/ydiv;
		  incx=0;
		 }

  }
}


#define STRICT
#include <windows.h>
#include "clip.h"

/********************************************************
 * GlobalMemoryData -- initialize the global memory 	*
 *			data class			*
 *							*
 * Parameters						*
 *	handle -- handle to the global memory		*
 ********************************************************/
GlobalMemoryData::GlobalMemoryData(HGLOBAL our_handle) :
    handle(our_handle)
{
    locked = FALSE;
}
/********************************************************
 * ptr -- return a pointer to the global memory		*
 *		(automatically locks memory)		*
 ********************************************************/
BYTE *GlobalMemoryData::ptr(void)
{
    if (locked == FALSE) {
        data_ptr = (BYTE *) GlobalLock(handle);
	locked = TRUE;
    }
    return (data_ptr);
}
/********************************************************
 * size -- return the size of the global data		*
 ********************************************************/
int GlobalMemoryData::size(void)
{
    return ((int)GlobalSize(handle));
}
/********************************************************
 * Unlock -- unlock the global memory			*
 ********************************************************/
void GlobalMemoryData::Unlock(void)
{
    if (locked) {
        GlobalUnlock(handle);
	locked = FALSE;
    }
}
/********************************************************
 * ~GlobalMemoryData -- unlock the global memory	*
 ********************************************************/
GlobalMemoryData::~GlobalMemoryData(void)
{
    Unlock();
}
/********************************************************
 * Clipboard -- initialize the clipboard class		*
 ********************************************************/
Clipboard::Clipboard(void)
{
    clipboard_open = FALSE;
}
/********************************************************
 * open_check -- check to see if the clipboard is open	*
 *		and open it if needed			*
 ********************************************************/
void Clipboard::open_check(void) {
    if (!clipboard_open) {
	clipboard_open = OpenClipboard(main_window_handle);
    }
}
/********************************************************
 * OpenClipboard -- open the clipboard			*
 *	Note: We do automatic opening in the other calls*
 *	so this is redundant.				*
 *							*
 * Parameters						*
 *	hWnd -- handle of the main window		*
 *							*
 * Returns						*
 *	TRUE -- opened					*
 *	FALSE -- not opened				*
 ********************************************************/
BOOL Clipboard::OpenClipboard(HWND hWnd)
{
   if (clipboard_open)
       return (TRUE);
   clipboard_open = ::OpenClipboard(hWnd);
   return (clipboard_open);
}
/********************************************************
 * GetClipboardData -- grab data from the clipboard	*
 *							*
 * Parameters						*
 *	format -- format to read			*
 *							*
 * Returns						*
 *	Handle to global memory of data			*
 *	or NULL if no data gotten			*
 ********************************************************/
HGLOBAL Clipboard::GetClipboardData(WORD format)
{
    open_check();
    return (::GetClipboardData(format));
}
/********************************************************
 *  EmptyClipboard -- clear the clipboard data		*
 *							*
 * Returns						*
 *	TRUE -- clipboard empty				*
 *	FALSE -- problem				*
 ********************************************************/
BOOL Clipboard::EmptyClipboard(void)
{
    open_check();
    return (::EmptyClipboard());
}
/********************************************************
 * SetClipboardData -- dump data to the clipboard	*
 *							*
 * Parameters						*
 *	format -- the format of the data		*
 *	data -- handle of the data			*
 *							*
 * Returns						*
 *	TRUE -- data in the clipboard			*
 *	FALSE -- problem				*
 ********************************************************/
HGLOBAL Clipboard::SetClipboardData(WORD format, HGLOBAL data)
{
    open_check();
    return (::SetClipboardData(format, data));
}
/********************************************************
 * IsClipboardFormatAvailable -- test to see if format	*
 *		is in the clipboard			*
 *							*
 * Parameters						*
 *	format -- format to check			*
 *							*
 * Returns						*
 *	TRUE -- data of type "format" is in clipboard	*
 *	FALSE -- no data available			*
 ********************************************************/
BOOL Clipboard::IsClipboardFormatAvailable(WORD format)
{
    open_check();
    return (::IsClipboardFormatAvailable(format));
}
/********************************************************
 * ~Clipboard -- destroy clipboard class, close 	*
 *	clipboard.					*
 ********************************************************/
Clipboard::~Clipboard(void)
{
    CloseClipboard();
}

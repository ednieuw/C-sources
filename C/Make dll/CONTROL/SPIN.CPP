#define STRICT

#include <windows.h>
#include <custcntl.h>
#include <string.h>

#include "spin-i.h"
#include "spin.h"

// Class name for this control
#define SPIN_NAME	"SpinButton"

// Property for edit information
#define EDIT_PROP	"spin_edit"

// Our data instance
static HINSTANCE instance;

// The bitmap for the spin button
static HBITMAP spin_bitmap;

/********************************************************
 * Structure for communicating with edit dialog proc	*
 ********************************************************/
struct edit_param {
    // Global memory containing style information
    HGLOBAL 	style_global;
    // Pointer to the String To Id routine
    LPFNSTRTOID StrToId;
    // Pointer to the Id To String routine
    LPFNIDTOSTR	IdToStr;
};
/********************************************************
 * This structure contains a list of names 		*
 *	for the spin button.				*
 *							*
 * Note: This would be more efficient and flexible	*
 *	if it were a dynamic structure, but this 	*
 *	version is very simple and easy to use.		*
 ********************************************************/

#define NAME_SIZE	30	// Length of longest name
// Max number of names in the list
#define LIST_MAX	20	

struct spin_data {
   int n_names;		// Number of names
   int current;		// Current number 
   char list[LIST_MAX][NAME_SIZE];
};

// Where we put the spin data in  our window extra bytes
#define SPIN_DATA	0	

/********************************************************
 * spin_button_proc -- procedure to handle spin		*
 *		button messages				*
 *							*
 * Parameters (windows usual)				*
 *	hWnd -- handle to the window			*
 *	wMessage -- message to handle			*
 *	wParam, lParam -- message parameters		*
 *							*
 * Return value depends on message			*
 ********************************************************/
LRESULT _export CALLBACK spin_button_proc(HWND hWnd, 
	UINT wMessage,
        WPARAM wParam, LPARAM lParam)
{
    HGLOBAL spin_global;	// Global memory for spin data
    struct spin_data *spin_ptr; // Pointer to current data
    int result;			// Result of SP_GET_SELECTION

    switch (wMessage) {
    	case SP_ADD_TEXT:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    if (spin_global == NULL)
	        return (-1);
	    spin_ptr = (spin_data *)GlobalLock(spin_global);
	    if (spin_ptr->n_names < (LIST_MAX-1)) {
	        strncpy(spin_ptr->list[spin_ptr->n_names],
			(const char *)lParam, NAME_SIZE-1);
		spin_ptr->list[spin_ptr->n_names]
			      [NAME_SIZE-1] = '\0';
		spin_ptr->n_names++;
	    }
	    GlobalUnlock(spin_global);
	    return (0);

	case SP_SELECT:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    if (spin_global == NULL)
	        return (-1);
	    spin_ptr = (spin_data *)GlobalLock(spin_global);
	    spin_ptr->current = wParam;
	    GlobalUnlock(spin_global);
	    return (0);

	case SP_GET_SELECTION:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    if (spin_global == NULL)
	        return (-1);
	    result = spin_ptr->current;
	    GlobalUnlock(spin_global);
	    return (result);

	case WM_CREATE:
	    spin_global = GlobalAlloc(sizeof(spin_data), GHND);
	    SetWindowWord(hWnd, SPIN_DATA, 
				(unsigned short)spin_global);
	    break;

	case WM_DESTROY:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    GlobalFree(spin_global);
	    break;

	case WM_PAINT:
	    {
		PAINTSTRUCT paint_struct; // What to paint 
		RECT client_rect;	// Where to paint it 
		int width, height;	// Bitmap size
		BITMAP bitmap;		// Bitmap itself
		HDC hMem;		// Memory device

		BeginPaint(hWnd, &paint_struct);
		GetClientRect(hWnd, &client_rect);
		Rectangle(paint_struct.hdc, 
		    client_rect.left, client_rect.top, 
		    client_rect.right, client_rect.bottom);

		GetObject(spin_bitmap, sizeof(bitmap), 
			(char *)&bitmap);

		hMem = CreateCompatibleDC(paint_struct.hdc);
		SelectObject(hMem, spin_bitmap);

		height = client_rect.bottom - client_rect.top;
		width = height;

		StretchBlt(paint_struct.hdc, 
			0, 0, width, height, hMem,
			0, 0, bitmap.bmWidth, bitmap.bmHeight, 
			SRCCOPY);
	        DeleteDC(hMem);

		client_rect.left += width;
		if (client_rect.left > client_rect.right)
		    break;

		client_rect.top += 2;
		client_rect.bottom -= 2;
		client_rect.left += 2;
		client_rect.right -= 2;
		spin_global = 
		    (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
		if (spin_global != NULL) 
		    spin_ptr = 
			(spin_data *)GlobalLock(spin_global);

		if ((spin_global == NULL) && 
		    (spin_ptr->n_names != 0))
		    DrawText(paint_struct.hdc, "-spin-",
			    -1, &client_rect, 
			    DT_NOPREFIX|DT_SINGLELINE);
		else {
		    DrawText(paint_struct.hdc, 
		        spin_ptr->list[spin_ptr->current],
			    -1, &client_rect, 
			    DT_NOPREFIX|DT_SINGLELINE);
		    GlobalUnlock(spin_global);
		}
		EndPaint(hWnd, &paint_struct);
	    }
	    break;

	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    if (spin_global == NULL)
	        return (-1);
	    spin_ptr = (spin_data *)GlobalLock(spin_global);
	    spin_ptr->current++;
	    if (spin_ptr->current >= spin_ptr->n_names)
	    	spin_ptr->current = 0;
	    GlobalUnlock(spin_global);
	    InvalidateRect(hWnd, NULL, TRUE);
	    return (0);
		
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	    spin_global = 
	        (HGLOBAL)GetWindowWord(hWnd, SPIN_DATA);
	    if (spin_global == NULL)
	        return (-1);
	    spin_ptr = (spin_data *)GlobalLock(spin_global);
	    spin_ptr->current--;
	    if (spin_ptr->current < 0)
	    	spin_ptr->current = spin_ptr->n_names-1;
	    GlobalUnlock(spin_global);
	    InvalidateRect(hWnd, NULL, TRUE);
	    return (0);
		
	default:
	    return (DefWindowProc(hWnd, 
		    wMessage, wParam, lParam));
    }

    return (0);
}

/********************************************************
 * spin_info -- returns information about the class	*
 *							*
 * Returns						*
 *	 global memory handle containg RWCTLINFO	*
 *		data					*
 ********************************************************/
HGLOBAL _export CALLBACK spin_info(void)
{
    HGLOBAL info_handle = GlobalAlloc(GHND, sizeof(RWCTLINFO));

    if (info_handle != NULL) {
	LPRWCTLINFO info_ptr = 
	    (LPRWCTLINFO) GlobalLock(info_handle);

	info_ptr->wVersion  = 0x0100;		// Version 1.00
	info_ptr->wCtlTypes = 1;
	lstrcpy(info_ptr->szClass, SPIN_NAME);
	lstrcpy(info_ptr->szTitle, "Spin Button");

	// Size of the control (default)
	info_ptr->Type[0].wWidth  = 64 | 0x8000;
	info_ptr->Type[0].wHeight = 24 | 0x8000;

	lstrcpy(info_ptr->Type[0].szDescr, "Spin Button");

	info_ptr->Type[0].dwStyle = BS_PUSHBUTTON | WS_TABSTOP;
	info_ptr->Type[0].hToolBit = spin_bitmap;

	info_ptr->Type[0].hDropCurs = LoadCursor(instance,
		"TOOL_CURSOR");
	if (info_ptr->Type[0].hDropCurs == NULL) {
	    MessageBox(NULL,
	      "Warning: Could not load cursor for spin button",
		    "Spin error", MB_ICONSTOP | MB_OK);
	}

	GlobalUnlock(info_handle);
    }

    return info_handle;
}
/********************************************************
 * init_edit -- initialize the edit dialog		*
 *							*
 * Parameters						*
 *	hWnd -- window handle				*
 *	lParam -- lParm of the INITDIALOG message	*
 ********************************************************/
static void pascal init_edit(HWND hWnd, LPARAM lParam)
{
    // Global memory for parameters
    HGLOBAL edit_global = (HGLOBAL)LOWORD(lParam);

    // Pointer to edit parameters
    struct edit_param *edit_ptr	= 
	(struct edit_param *) GlobalLock(edit_global);


    // Pointer to current style information
    LPCTLSTYLE style_ptr = 
	(LPCTLSTYLE) GlobalLock(edit_ptr->style_global);

    char str[100];	// Random string (for editing of id)

    // Save edit_global for future use in other messages.
    SetProp(hWnd, EDIT_PROP, edit_global);

    // Set control id
    (*edit_ptr->IdToStr)(style_ptr->wId, str, sizeof(str));
    SetDlgItemText(hWnd, CONTROL_BLANK, str);

    // Clean-up scope
    GlobalUnlock(edit_ptr->style_global);
    GlobalUnlock(edit_global);
}

/********************************************************
 * edit_cmd -- handle WM_COMMAND messages for editing	*
 *		the spin button				*
 *							*
 * Parameters						*
 *	hWnd -- window handle				*
 *	wParam, lParam -- message parameters		*
 *							*
 * Returns						*
 *	TRUE -- message handled.			*
 *	FALSE -- not handled				*
 ********************************************************/
static int pascal edit_cmd(HWND hWnd, 
	WPARAM wParam, LPARAM lParam)
{
    (void)lParam;	// Avoid warning

    switch(wParam) {
	case CANCEL_BUTTON:
	    EndDialog(hWnd, FALSE);
	    return (TRUE);
	case OK_BUTTON:
	    {
		// Handle of global memory containing edit data
		HGLOBAL edit_global = GetProp(hWnd, EDIT_PROP);

		// Pointer to edit data for this window
		struct edit_param *edit_ptr = 
		    (struct edit_param *) 
			GlobalLock(edit_global);

		// Pointer to style info for the window
		LPCTLSTYLE style_ptr =	
		    (LPCTLSTYLE) 
			GlobalLock(edit_ptr->style_global);

		// General purpose string
		char str[81];

		// Get control id
		GetDlgItemText(hWnd, CONTROL_BLANK, 
			str, sizeof(str));
		style_ptr->wId = 
			HIWORD( (*edit_ptr->StrToId)(str));

		// Clean-up scope
		GlobalUnlock(edit_ptr->style_global);
		GlobalUnlock(edit_global);
		EndDialog(hWnd, TRUE);

		return (TRUE);
	    }

	default:
	    return (FALSE);
    }
}
/********************************************************
 * spin_edit_dlg -- handle dialog box to set spin #	*
 *							*
 * Parameters (windows usual)				*
 *	hWnd -- window handle for dialog		*
 *	wMessage -- message to handle			*
 *	wParam, lParam -- message parameters		*
 *							*
 * Returns						*
 *	message dependent				*
 ********************************************************/
BOOL FAR PASCAL _export spin_edit_dlg(HWND hWnd, UINT wMessage,
	WPARAM wParam, LPARAM lParam)
{
    switch( wMessage ) {
	case WM_INITDIALOG:
	    init_edit(hWnd, lParam);
	    return (TRUE);

	case WM_COMMAND:
	    return (edit_cmd(hWnd, wParam, lParam));

	    case WM_DESTROY:
		RemoveProp(hWnd, EDIT_PROP);
		return (FALSE);

	    default:
	        return (FALSE);
    }
}

/********************************************************
 * spin_edit -- edit the spin button			*
 *							*
 * Display a dialog and allow editing of the button	*
 *							*
 * Paramters (workshop standard)			*
 *	hWnd -- handle of parent window			*
 *	hCtlStyle -- handle of a RWCTLSTYLE memory blk	*
 *	StrToId  -- Function that turns string into id#	*
 *	IdToStr -- Function that turns id number into	*
 *			string.				*
 *							*
 * Returns:						*
 *	True if the user changes something		*
 ********************************************************/
BOOL _export CALLBACK spin_edit(HWND hWnd, HGLOBAL hCtlStyle,
	LPFNSTRTOID StrToId, LPFNIDTOSTR IdToStr)
{
    // True if we changed something
    BOOL     result 	= FALSE;

    // Memory used for passing parameters to dialog proc
    HGLOBAL  edit_global = 
        GlobalAlloc(GHND, sizeof(struct edit_param));

    if (edit_global) {
	// Pointer to edit parameters
	struct edit_param *edit_ptr = 
	    (struct edit_param *) GlobalLock(edit_global);

	// Get window that has the current focus
	HWND old_focus  = GetFocus();

	// Setup the parameter record
	edit_ptr->IdToStr = IdToStr;
	edit_ptr->StrToId = StrToId;
	edit_ptr->style_global = hCtlStyle;
	GlobalUnlock(edit_global);

	// Invoke the dialog box
	result = DialogBoxParam(instance, "EDIT_DIALOG",
		hWnd, spin_edit_dlg, (DWORD) edit_global);

	// Restore focused window since Windows does not do it
	if (old_focus != NULL)
		SetFocus(old_focus);

	GlobalFree(edit_global);
    }
    return result;
}

/********************************************************
 *  spin_flags_to_str -- turn a set of flags into	*
 *			a string.			*
 * Since we use no flag bits, this call is very simple	*
 *							*
 * Parameters (workshop standard)			*
 *	Flags -- flags from the workshop		*
 *	Buffer -- buffer to store the flags in		*
 *	BufferLength -- max number of charcters in the	*
 *			buffer.				*
 *							*
 * Returns						*
 *	0 -- worked					*
 *	xxx -- failed					*
 ********************************************************/
UINT _export CALLBACK spin_flags_to_str(DWORD flags, 
	LPSTR buffer, UINT buffer_length)
{
    (void)flags;  // Avoid warnings
    (void)buffer;
    (void)buffer_length;

    buffer[0] = '\0';
    return 0;
}
/********************************************************
 * Note: This little trick must be used to make sure	*
 * 	that C++ teats ListClasses as a C procedure	*
 *	and does not mangle the procedure name		*
 ********************************************************/
extern "C" {
HGLOBAL CALLBACK _export ListClasses(LPSTR szAppName,
	UINT wVersion, LPFNLOADRES fnLoad, LPFNEDITRES fnEdit);
}

/********************************************************
 * ListClasses -- list the classes in the DLL		*
 *		called by the workshop			*
 *							*
 * Parameters (workshop standard)			*
 *	szAppName -- name of the calling application	*
 *	wVersion -- version number of applications	*
 *	fnLoad -- function to load resource from 	*
 *		RC file being edited.			*
 *	fnEdit -- function to edit a resource from	*
 *		the RC file being edited.		*
 *							*
 * Returns						*
 *	Global memory handle for the class list		*
 ********************************************************/
HGLOBAL CALLBACK _export ListClasses(LPSTR szAppName,
	UINT wVersion, LPFNLOADRES fnLoad, LPFNEDITRES fnEdit)
{
    // Memory handle for our class
    /*
     * 	int -- number of classes that follow (1)
     *  RWCTLCLASS -- space for the class
     */
    HGLOBAL class_global = GlobalAlloc(GHND,
	    sizeof(int) + sizeof(RWCTLCLASS));

    (void)szAppName;	// Avoid warning
    (void)wVersion;
    (void)fnLoad;
    (void)fnEdit;

    if (class_global != NULL) {
	// Pointer to the class info we are building
	LPCTLCLASSLIST	class_ptr = 
	    (LPCTLCLASSLIST) GlobalLock(class_global);

	class_ptr->nClasses = 1;
	class_ptr->Classes[0].fnRWInfo  = spin_info;
	class_ptr->Classes[0].fnRWStyle = spin_edit;
	class_ptr->Classes[0].fnFlags   = spin_flags_to_str;
	lstrcpy(class_ptr->Classes[0].szClass, SPIN_NAME);
	GlobalUnlock(class_global);
    }

    return (class_global);
}

static WNDCLASS Class = {
    // Window style
    CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS,
    spin_button_proc, 	// lpfnWndProc
    0, 			// cbClsExtra
    sizeof(HGLOBAL),	// cbWndExtra
    0, 		 	// hInstance (set later)
    NULL, 		// No icon
    NULL, 		// hCursor
    NULL,	 	// hbrBackground
    NULL,	 	// lpszMenuName
    SPIN_NAME	 	// lpszClassName
};

/********************************************************
 * LibMain -- initialize the libaray			*
 *							*
 * Parameters (windows standard)			*
 *	hInst -- Instance for the DLL			*
 *	wDataSeg -- data segement for this DLL		*
 *	cbHeapSize -- heap size for this DLL		*
 *	lpszCmdLine -- command line			*
 *							*
 * Returns						*
 *	1 -- initializtion worked			*
 *		(it can't fail)				*
 *	0 -- failed					*
 ********************************************************/
int FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSeg, 
	WORD cbHeapSize, LPSTR lpszCmdLine)
{
	(void)wDataSeg;	// Avoid warning
	(void)cbHeapSize;
	(void)lpszCmdLine;
	instance = hInst;

	Class.hInstance = instance;
	RegisterClass(&Class);
	if (spin_bitmap == NULL) {
	    spin_bitmap = LoadBitmap(instance,
		                     "TOOL_BITMAP");

	    if (spin_bitmap== NULL) {
		MessageBox(NULL,
	      "Warning: Could not load bitmap for spin button",
			"Spin error", MB_ICONSTOP | MB_OK);
	    }
	}

	return 1;
}

/********************************************************
 * Clipboard class					*
 *							*
 * Members:						*
 *	OpenClipboard -- open the clipboard		*
 *		(done automatically by other routines)	*
 *	GetClipboardData -- get data from clipboard	*
 *		(automatically opens if needed)		*
 *	EmptyClipboard -- clear the clipboard		*
 *		(automatically opens if needed)		*
 *	SetClipboardData -- get data from clipboard	*
 *		(automatically opens if needed)		*
 *	IsClipboardFormatAvailable -- test for format	*
 *		(automatically opens if needed)		*
 ********************************************************/
class Clipboard {
    private:
        BOOL clipboard_open;
	void open_check(void);
    public:
	Clipboard(void); 
        BOOL OpenClipboard(HWND hWnd);
	HGLOBAL GetClipboardData(WORD format);
	BOOL EmptyClipboard(void);
	HGLOBAL SetClipboardData(WORD format, HGLOBAL data);
	BOOL IsClipboardFormatAvailable(WORD format);
	~Clipboard(void);
};
/********************************************************
 * GlobalMemoryData -- data for global memory		*
 *							*
 * 	GlobalMemoryData(HGLOBAL) -- setup class	*
 *	ptr -- get the pointer to the data		*
 *	size -- get the size of the data		*
 *	Unlock -- unlock the data			*
 *	~GlobalMemoryData -- also unlock the data	*
 ********************************************************/
class GlobalMemoryData {
    private:
        const HGLOBAL handle;	// Data handle
	BOOL locked;		// True if we've locked the data
	BYTE *data_ptr;		// Pointer to the data
    public:
        GlobalMemoryData(HGLOBAL handle);
	BYTE *ptr(void);
	int size(void);
	void Unlock(void);
	~GlobalMemoryData(void);
};

extern HWND main_window_handle;	// Handle of the main window

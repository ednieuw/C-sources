/********************************************************
 * SP_ADD_TEXT -- add an item to the list 		*
 *		used by a spin button.			*
 *							*
 * Parameters						*
 *	wParam -- not used				*
 *	lParam -- pointer to string to add		*
 ********************************************************/
#define SP_ADD_TEXT	(WM_USER+1)
/********************************************************
 * SP_SELECT -- select an item in the list 		*
 *							*
 * Parameters						*
 *	wParam -- item number for "current" item	*
 *	lParam -- not used				*
 ********************************************************/
#define SP_SELECT	(WM_USER+2)
/********************************************************
 * SP_GET_SELECTION -- read selection from a spin	*
 *		button.					*
 *							*
 * Parameters						*
 *	none						*
 * Returns index of current item			*
 ********************************************************/
#define SP_GET_SELECTION (WM_USER+3)

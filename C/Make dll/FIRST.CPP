#define STRICT

#include <windows.h>
 
// Our data instance
static HINSTANCE instance;

//int FAR PASCAL_stdcall calc(double *row, int rowaantal,double *res1  ,double *res2,double *res3);
 extern "C" {
int FAR PASCAL _export calc(double *row, int rowaantal,double *res1  ,double *res2,double *res3);
  }

int FAR PASCAL _export calc(double *row, int rowaantal,double *res1  ,double *res2,double *res3)
{
 int n;
 *res1=*res2=*res3=0 ;
	for (n=0;n<rowaantal;n++)
		{
		 *res1 += row[n];
		 *res2++;
		 }
	*res3=(*res1) /(*res2);
 return (123);
 }

 static WNDCLASS Class = {
	 // Window style
	 CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS,
	 NULL, 	// lpfnWndProc
	 0, 			// cbClsExtra
	 sizeof(HGLOBAL),	// cbWndExtra
	 0, 		 	// hInstance (set later)
	 NULL, 		// No icon
	 NULL, 		// hCursor
	 NULL,	 	// hbrBackground
	 NULL,	 	// lpszMenuName
	 NULL	 	// lpszClassName
};

 int FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSeg,
	WORD cbHeapSize, LPSTR lpszCmdLine)
{
	(void)wDataSeg;	// Avoid warning
	(void)cbHeapSize;
	(void)lpszCmdLine;
	instance = hInst;

	Class.hInstance = instance;
	RegisterClass(&Class);

	return 1;
}

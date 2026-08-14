//---------------------------------------
//	POPPADP.C for PICDUMP
//  ---------------------------------------

#include "print.h"
#include "picdump.h"
#include "COMMDLG.H"

extern 	CHOOSEFONT cf;
extern 	HANDLE  hInstBitmap;
BOOL 		PrintMyPage (HWND hwnd);
void 		Init_Page(HWND hwnd);
void 		PictextSmall(HDC hdc, int x,int y,char* s);

PRINTDLG pd;
int 		PicSize;
int      xPage, yPage ;
int		PicXsize,PicYsize;
int 		xdiv,ydiv,xorg,yorg,incx,incy;
int		printedpic;
int		SmallCharHeight;

//-------------------------------------------------------------------------
BOOL PrintMyPage (HWND hwnd)
{
static char szSpMsg [] = "PICDUMP JOB" ;
BOOL        bError = FALSE ;
BOOL   		bUserAbort ;
char 			drive[MAXDRIVE];
char 			dir[MAXDIR];
char 			file[MAXFILE];
char 			ext[MAXEXT];
int 			flags;
HANDLE 		hmf;

memset(&pd, 0, sizeof(PRINTDLG));
pd.lStructSize = sizeof(PRINTDLG);
pd.hwndOwner = hwnd;
pd.Flags = PD_RETURNDC|PD_NOPAGENUMS|PD_NOSELECTION;

if(LastOrButton==IDM_LANDSCAPE)
 MessageBox (hwnd, "Choose SETUP in dialogbox\nSelect LANDSCAPE orientation", "Selection ",MB_OK);

if(PrintDlg(&pd) !=0)
{
 short n;
 SetCursor(LoadCursor(NULL, IDC_WAIT));
 //Display_Bitmap(hwnd,hInstBitmap);
 Escape (pd.hDC, STARTDOC, sizeof szSpMsg - 1, szSpMsg, NULL);
 xdiv=1;ydiv=2;printedpic=0;

 if(LastOrButton==IDM_PORTRAIT)
	{
	if(LastGrButton==IDM_NOOFGR1) {xdiv=1;ydiv=1;}
	if(LastGrButton==IDM_NOOFGR2) {xdiv=1;ydiv=2;}
	if(LastGrButton==IDM_NOOFGR6) {xdiv=2;ydiv=3;}
	if(LastGrButton==IDM_NOOFGR8) {xdiv=2;ydiv=4;}
	if(LastGrButton==IDM_NOOFGR9) {xdiv=3;ydiv=3;}
	if(LastGrButton==IDM_NOOFGR12){xdiv=3;ydiv=4;}
	}
 else
	{
	if(LastGrButton==IDM_NOOFGR1) {xdiv=1;ydiv=1;}
	if(LastGrButton==IDM_NOOFGR2) {xdiv=2;ydiv=1;}
	if(LastGrButton==IDM_NOOFGR6) {xdiv=3;ydiv=2;}
	if(LastGrButton==IDM_NOOFGR8) {xdiv=4;ydiv=2;}
	if(LastGrButton==IDM_NOOFGR9) {xdiv=3;ydiv=3;}
	if(LastGrButton==IDM_NOOFGR12){xdiv=4;ydiv=3;}
	}
	Init_Page(hwnd);
	for(n=0;n<noofpicfiles;n++)
		{

		PicXsize = (int) (0.85 * (float)xPage / (float)xdiv);
		PicYsize = (int)((float)PicXsize * 2300.0/3200.0);
		if(PicYsize>0.85*(float)yPage / (float)ydiv)
			{
			PicYsize = (int) (0.85 * (float)yPage / (float)ydiv);
			PicXsize = (int)((float)PicYsize * 3200.0/2300.0);
			}

		MaxX = cxClient = PicXsize;
		MaxY = cyClient = PicYsize;

		SetMapMode 		(pd.hDC, MM_ANISOTROPIC) ;
		SetWindowExt   (pd.hDC, xPage,yPage);//PicXsize, PicYsize);
		SetViewportExt (pd.hDC, xPage,yPage);//PicXsize, PicYsize) ;
		SetViewportOrg (pd.hDC, xorg, yorg) ;
		SetROP2 (pd.hDC, nDrawingMode) ;

		flags=fnsplit(selected_files[test_selection[n]],drive,dir,file,ext);
		if(strcmp(ext,".BMP")==0 || strcmp(ext,".RLE")==0)
				do_paint_bmps(hwnd);
		else if (strcmp(ext,".WMF")==0 || strcmp(ext,".EMF")==0)
			{
		SetWindowExt   (pd.hDC, 2*cxClient/xdiv,3*cyClient/ydiv);
		SetViewportExt (pd.hDC, 2*cxClient/xdiv,3*cyClient/ydiv);
				hmf = GetMetaFile(selected_files[test_selection[n]]);
				PlayMetaFile(pd.hDC, hmf);
				DeleteMetaFile(hmf);
			}
		else
				ReadPic(pd.hDC,selected_files[test_selection[n]]);
		xorg+=(int)(0.9 * xPage / xdiv);
		if(++incx>=xdiv)
		 {
			if(LastOrButton==IDM_PORTRAIT)  	xorg=xPage/10;
			else	 							  	   xorg=yPage/10;
			yorg+=0.9*yPage/ydiv;
			incx=0;
			if(++incy>=ydiv)
				{
				 Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
				 Init_Page(hwnd);
				}
		  }
	  }
  if(((noofpicfiles/noofgrpage)*noofgrpage)!=noofpicfiles)
			 Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
  Escape (pd.hDC, ENDDOC, 0, NULL, NULL) ;
  if (pd.hDevMode != NULL)		GlobalFree(pd.hDevMode);
  if (pd.hDevNames != NULL)	GlobalFree(pd.hDevNames);
  }
 else bUserAbort=TRUE;
 flags=flags;
 return bError || bUserAbort ;
}

void Init_Page(HWND hwnd)
{
char		text[80];
int 		n,txo,tyo;
FILE 		*stream;
struct	ftime ft;


	MaxX =  cxClient = xPage = GetDeviceCaps (pd.hDC, HORZRES);
	MaxY =  cyClient = yPage = GetDeviceCaps (pd.hDC, VERTRES);
	if(LastOrButton==IDM_PORTRAIT) 	{	xorg=xPage/10;	yorg=yPage/25; SmallCharHeight=yPage/100;}
	else	 							  		{  xorg=yPage/10;	yorg=xPage/25; SmallCharHeight=xPage/100;}
	incx=incy=0;
	SetMapMode(pd.hDC,MM_ANISOTROPIC);
	GetClientRect (hwnd, &rect) ;
	SetWindowExt   (pd.hDC, xPage, yPage*1.02);
	SetViewportExt (pd.hDC, xPage, yPage) ;
//	SetWindowExt(pd.hDC,rect.right, -rect.bottom);
//	SetViewportExt (pd.hDC, rect.right,-rect.bottom) ;
	SetROP2 (pd.hDC, nDrawingMode) ;

	sprintf(text,"Disk:%s",selected_files[254]);
	SetTextAlign(pd.hDC,TA_LEFT|TA_BASELINE);
	PictextSmall(pd.hDC,0, 0, text);		  // print volumelabel disk
	txo=0;
	tyo=ydiv-1;
	for(n=printedpic;n<printedpic+xdiv*ydiv;n++)
	{
	if(n>0&&test_selection[n]==0)  continue;
		stream = fopen(selected_files[test_selection[n]],"r");
		getftime(fileno(stream), &ft);
		sprintf(text,"%s %02u/%02u/%02u  %02u:%02u:%02u",selected_files[test_selection[n]],
											ft.ft_day ,ft.ft_month, ft.ft_year+1980,
											ft.ft_hour,ft.ft_min, ft.ft_tsec * 2);
		fclose(stream);
		SetTextAlign(pd.hDC,TA_RIGHT|TA_BASELINE);
		PictextSmall(pd.hDC,++txo*(xPage-xorg)/xdiv,
		tyo*SmallCharHeight,//30,//(short)((float)lf.lfHeight*(float)cxClient/(float)cxClientOrg),
		text);
		if(txo==xdiv) {txo=0;tyo--;}
	}
  printedpic=n;
}

void PictextSmall(HDC hdc, int x,int y,char* s)
 {
 HFONT 		hfont, hfontOld;

 int nop,nop1;
//	SetMapMode(pd.hDC,MM_LOENGLISH);
	nop=lf.lfHeight;
	nop1=lf.lfWidth;
	lf.lfHeight=(short)SmallCharHeight;
	lf.lfWidth=0;
//	 lf.lfHeight=(short)((float)lf.lfHeight*(float)cxClient/(float)cxClientOrg);
//	 lf.lfWidth=(short)(lf.lfWidth*(float)cyClient/(float)cyClientOrg);
	lf.lfEscapement=0;//direction * 10;
	hfont = CreateFontIndirect(cf.lpLogFont);
	lf.lfHeight=nop;
	lf.lfWidth=nop1;
//	SetMapMode(pd.hDC,MM_ANISOTROPIC);
	hfontOld = SelectObject(hdc, hfont);
	TextOut(hdc,x,yPage-y,s,strlen(s));
	DeleteObject(SelectObject(hdc, hfontOld));
 }



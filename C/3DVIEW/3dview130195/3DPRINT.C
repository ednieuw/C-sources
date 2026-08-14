//---------------------------------------
//	POPPADP.C for PICDUMP
//  ---------------------------------------

#include "print.h"
#include "3dview.h"
#include "COMMDLG.H"

extern CHOOSEFONT cf;
extern HANDLE  hInstBitmap;
BOOL PrintMyPage (HWND hwnd);
void Init_Page(void);
void PictextSmall(HDC hdc, int x,int y,char* s);

PRINTDLG 	pd;
int 			PicSize;
int         xPage, yPage ;
int			PicXsize,PicYsize;
int 			xdiv,ydiv,xorg,yorg,incx,incy;
int			printedpic;
int			SmallCharHeight;

BOOL PrintMyPage (HWND hwnd)
{
static char szSpMsg [] = "PICDUMP JOB" ;
BOOL        bError = FALSE ;
BOOL   		bUserAbort ;
//DOCINFO 		lpdi;
memset(&pd, 0, sizeof(PRINTDLG));
pd.lStructSize = sizeof(PRINTDLG);
pd.hwndOwner = hwnd;
pd.Flags = PD_RETURNDC|PD_NOPAGENUMS|PD_NOSELECTION;
/*
memset(&lpdi,0,sizeof(DOCINFO));
lpdi.cbSize=sizeof(DOCINFO);
lpdi.lpszDocName=szSpMsg;
lpdi.lpszOutput=NULL;
*/
if(LastOrButton==IDM_LANDSCAPE)
 MessageBox (hwnd, "Choose SETUP in dialogbox\nSelect LANDSCAPE orientation", "Selection ",MB_OK);

if(PrintDlg(&pd) !=0)
{
// short n;
SetCursor(LoadCursor(NULL, IDC_WAIT));
//Display_Bitmap(hwnd,hInstBitmap);

 //StartDoc(pd.hDC,&lpdi);//
 Escape (pd.hDC, STARTDOC, sizeof szSpMsg - 1, szSpMsg, NULL);
 xdiv=1;ydiv=1;printedpic=0;

	Init_Page();
	SetMapMode(pd.hDC,MM_ANISOTROPIC);
	SetWindowExt   (pd.hDC, xPage,yPage);
	SetViewportExt (pd.hDC, xPage,yPage);
	SetViewportOrg (pd.hDC, xorg, yorg) ;
	SetROP2 (pd.hDC, nDrawingMode) ;
	Print_grafiek(hwnd,pd.hDC,"nop");
/*
	//	StartPage(pd.hDC);
//	for(n=0;n<noofpicfiles;n++)
		{
		PicXsize = (int) (0.85 * (float)xPage / (float)xdiv);
		PicYsize = (int)((float)PicXsize * 2300.0/3200.0);
		if(PicYsize>0.85*(float)yPage / (float)ydiv)
		{
		PicYsize = (int) (0.85 * (float)yPage / (float)ydiv);
		PicXsize = (int)((float)PicYsize * 3200.0/2300.0);
		}
		MaxX = cxClient = PicXsize = xPage;
		MaxY = cyClient = PicYsize = yPage;

		SetWindowExt   (pd.hDC, xPage,yPage);//PicXsize, PicYsize);
		SetViewportExt (pd.hDC, xPage,yPage);//PicXsize, PicYsize) ;
		SetViewportOrg (pd.hDC, xorg, yorg) ;
		SetROP2 (pd.hDC, nDrawingMode) ;
		PrintGrafiek(hwnd,pd.hDC,"nop");
		//		ReadPic(pd.hDC,selected_files[test_selection[n]]);
		xorg+=(int)(0.9 * xPage / xdiv);
		if(++incx>=xdiv)
		 {
	if(LastOrButton==IDM_PORTRAIT)  	xorg=xPage/10;
	else	 							  	   xorg=yPage/10;
			yorg+=0.9*yPage/ydiv;
			incx=0;
			if(++incy>=ydiv)
				{
				 //EndPage(pd.hDC);//
				 Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
				 Init_Page();
				 //	StartPage(pd.hDC);
				}
		  }

		}
*/
		  //EndPage(pd.hDC);//
 Escape (pd.hDC, NEWFRAME, 0, NULL, NULL) ;
  //EndDoc(pd.hDC);//
  Escape (pd.hDC, ENDDOC, 0, NULL, NULL) ;
  if (pd.hDevMode != NULL)		GlobalFree(pd.hDevMode);
  if (pd.hDevNames != NULL)	GlobalFree(pd.hDevNames);
  }
 else bUserAbort=TRUE;

 return bError || bUserAbort ;
}

void Init_Page(void)
{
//char		text[80];
//int 		n,txo,tyo;
//FILE 		*stream;
//struct	ftime ft;

//	SetMapMode(pd.hDC,MM_ANISOTROPIC);
	MaxX = cxClient = xPage = GetDeviceCaps (pd.hDC, HORZRES);
	MaxY = cyClient = yPage = GetDeviceCaps (pd.hDC, VERTRES);
	if(LastOrButton==IDM_PORTRAIT) 	{	xorg=xPage;	yorg=yPage; SmallCharHeight=yPage/100;}
	else	 							  		{  xorg=yPage;	yorg=xPage; SmallCharHeight=xPage/100;}
/*	incx=incy=0;
	SetWindowExt   (pd.hDC, xPage, yPage*1.02);
	SetViewportExt (pd.hDC, xPage, yPage) ;
	SetViewportOrg (pd.hDC, 0,0) ;
	MoveTo(pd.hDC, 0,0) ;
	LineTo(pd.hDC, 0,yPage) ;
	LineTo(pd.hDC, xPage,yPage) ;
	LineTo(pd.hDC, xPage,0) ;
	LineTo(pd.hDC, 0,0) ;
*/
//	sprintf(text,"Disk:%s",selected_files[254]);
/*	SetTextAlign(pd.hDC,TA_LEFT|TA_BASELINE);
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
*/
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



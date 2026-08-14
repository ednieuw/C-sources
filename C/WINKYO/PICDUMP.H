#include "windows.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "fcntl.h"
#include "dos.h"
#include "stdio.h"
#include "time.h"
#include <ctype.h>
#include <io.h>
#include <dir.h>


#define IDM_INPUT	101
#define IDM_SELECT   100
#define IDC_MOVE	102
#define IDM_RESET   	107
#define IDM_PRT     	108
#define IDM_NOOFGR1	601
#define IDM_NOOFGR2	602
#define IDM_NOOFGR6	606
#define IDM_NOOFGR8	608
#define IDM_NOOFGR9	609
#define IDM_NOOFGR12	612
#define IDM_PRN     	113
#define IDM_CLOSE   	114

#define IDM_RESFILE  122
#define IDM_FONT		107
#define IDM_VIEW		108
#define IDM_DRIVEDIR	103
#define IDM_LINETHICKNESS1 201
#define IDM_LINETHICKNESS2 202
#define IDM_LINETHICKNESS3 204
#define IDM_LINETHICKNESS4 206
#define IDC_GROUPBOX1	101
#define IDM_PORTRAIT	102
#define IDM_LANDSCAPE	104
#define IDM_VOLUMENAAM	105
#define IDM_DRIVEA	109
#define IDM_DRIVEB	111
#define IDM_PRN2PIC	112
#define IDM_PIC2PRN	110
#define MISSING -107374176.0    /* &CCCCCCCC */
/*#define IDD_FNAME   0x10
#define IDD_FPATH   0x11
#define IDD_FLIST   0x12
*/
#define IDS_NAME      1
#define IDM_PRINT   5
#define IDM_ABOUTP  7

enum COLORS {
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
	 BROWN,
    LIGHTGRAY,
    DARKGRAY,       /* light colors */
	 LIGHTBLUE,
	 LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
	 LIGHTMAGENTA,
	 YELLOW,
	 WHITE
};

extern DWORD dwColor[16];
//							BLACK,    		BLUE,     		GREEN,    			CYAN,
//							RED,    			MAGENTA,    	BROWN,     			LIGHTGRAY,
//							DARKGRAY,  		LIGHTBLUE,    	LIGHTGREEN,    	LIGHTCYAN,
//							LIGHTRED,  		LIGHTMAGENTA,  YELLOW,    			WHITE

#define CENTER 		0
#define CENTERLEFT	1
#define CENTERTOP		2
#define CENTERRIGHT	3
#define CENTERBOTTOM 4
#define TOPLEFT		5
#define TOPRIGHT 		6
#define BOTTOMLEFT 	7
#define BOTTOMRIGHT 	8
#define MOVE    	160
#define DRAW    	162
#define FILL     	 48
#define FILLO   	208
#define _TEXT   	168
#define _FONT   	167
#define _SIZE   	172
#define _END       96
#define COLOR0  	176
#define COLOR1  	177
#define COLOR2  	178
#define COLOR3  	179
#define COLOR4  	180
#define COLOR5  	181
#define COLOR6  	182
#define COLOR7  	183
#define COLOR8  	184
#define COLOR9  	185
#define COLOR10 	186
#define COLOR11 	187
#define COLOR12 	188
#define COLOR13 	189
#define COLOR14 	190
#define COLOR15 	191
#define COLOR16 	198
//NEW FOR PRINTGRAPH
#define RECTANGLE	192
#define CHORD	 	193
#define ELLIPSE 	194
#define POLYLINE 	195
#define FLOODFILL 196
#define HATCH	  	197
#define NORMAL		  0
#define BDIAGONAL   1
#define CROSS	     2
#define DIAGCROSS   3
#define FDIAGONAL   4
#define HORIZONTAL  5
#define VERTICAL 	  6

#define MAXSTRLEN 256

// ------------------- picdump.c ------------------------------------------------------
extern int   	print,lost,berekend;
extern BOOL 	ViewerMain (HANDLE , HWND);
extern LOGFONT lf;
extern short 	FontGekozen;
extern int 		PicSize;
extern void Display_Bitmap(HANDLE hwndbm,HANDLE  hInstBitmap);

// ------------------- WCGRAPHG.C ------------------------------------------------------
extern int 	DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
extern 		LRESULT	FAR PASCAL GraphDrawDlgProc (HWND, WORD, WORD, LONG);
extern void	Print_grafiek(HWND hwnd,HDC hdc,char *PICfilenaam);

// ------------------- Winpiclib.c ------------------------------------------------------
extern void 	ConvertPicXY(double x,double y);
extern void 	Draw(HDC hdc,double xp,double yp);
extern void		DrawPolygon(HDC hdc,POINT *drawpos, int vertices, int fill);
extern void 	DrawEllipse(HDC hdc,double left,double top,double right,double bottom);
extern void 	DrawFillEllipse(HDC hdc,double left,double top,double right,double bottom);
extern void 	Move(HDC hdc,double xp,double yp);
extern void 	Point(HDC hdc,double xp,double yp, double box);
extern void 	Numm(HDC hdc,double xp,double yp,int number);
extern void 	Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst);
extern int  	Pic_Open(HWND hwnd);
extern void 	Pic_End(HWND hwnd);
extern void 	Picfont(char font);
extern void 	Piccolor(HDC hdc,int color,int thickness);
extern void 	PicRGBcolor(HDC hdc,int Rcolor,int Gcolor,int Bcolor,int thickness);
extern void 	Picsize(int x, int y);
extern void 	Pictext(HDC hdc, int direction,int position,char* s,float size);
extern void 	Picxy(int x,int y);
extern void 	Picbox(HDC hdc,int ox,int oy,int lx,int ly);
//extern float 	Data_X_2screen(float number,struct ASCAL *ascal);
//extern float 	Data_Y_2screen(float number,struct ASCAL *ascal);

struct ASCAL{
					BYTE		X_As;   				//data op Xas
					BYTE		Y_As;             //data op Yas
					float 	X_Min;
					float 	X_Max;
					float 	Y_Min;
					float 	Y_Max;
					BYTE		X_Astrans ;   		//Lineair=0, Log=1
					BYTE		Y_Astrans ;   		//Lineair=0, Log=1
					BYTE    	Red;
					BYTE		Green;
					BYTE		Blue;   					// RGB colors line
					BYTE     Color;				// Als Color ==255 dan RGB values
					BYTE     PointColor;			// Als Color ==255 dan RGB values
					BYTE		Linethickness;
					BYTE		Pointthickness;
					int		X_Offset;
					int 		Y_Offset;
					int 		X_Length;			// length axis in units (10000)
					int 		Y_Length;			// length axis in units (10000)
					BYTE		X_Ticks;				// 0=inside 1=through 2=outside
					BYTE		Y_Ticks;				// 0=inside 1=through 2=outside
					int		X_NoofTicks;
					int		Y_NoofTicks;
					BYTE		X_Zeroforced;
					BYTE		Y_Zeroforced;
					long		Noof_DataPoints;
				 };

extern short  cxClientOrg;
extern short  cyClientOrg;
extern int	  test_selection[256]; //no of selected file from list in wselect.c

extern char   Picoutputfile[];
extern char   Resoutputfile[];
extern int    MAX_ROW;
extern int    PICOPEN;
extern int    HAND;
extern short  noofgrpage,Page_orientation;

extern int    	lost,berekend;
extern int	  	Numgraph;
extern int    	deleted;
extern short  	cxClient,cyClient;
extern short  	cxChar,cxCaps,cyChar;
//extern short	cxCharPrn, PrncyChar,cyCharPrn, cxCharScreen, cyCharScreen;
extern int 		LastGrButton;
extern int 		LastLTButton;
extern int		LastOrButton;
extern TEXTMETRIC    tm;
extern PAINTSTRUCT 	ps;

// Uit Winpiclib
extern const char 		header_vector[17];
extern const double 		LotusPICX;
extern const double 		LotusPICY;
extern double 				ScreenX  ;
extern double 				ScreenY  ;
extern const float  		Pic_default_size;
extern FILE 		*fpout;
extern int   		DrawPicFile;			//Schrijf PIC file naar disk
extern int   		DrawGraphScreen;		//Teken op het geinitialiseerde graphscherm, alleen bij 3D
extern short 		cxClient,cyClient;	// size of screen
extern int			PICOPEN;
extern short 		cxChar,cxCaps,cyChar;
extern char 		Pic_output_file[MAXSTRLEN];
extern double  	PicX,PicY,GraphX,GraphY;
extern int   		MaxX,MaxY; 				//screen coordinates
extern short 		nDrawingMode;
extern int			Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
extern RECT 		rect;
extern HPEN       hpen, hpenOld;
extern LOGPEN 		lp;

// uit Wcorrtek
extern struct ASCAL  axcal;

//uit wselect.c
extern char selected_files[256][14];   // name of selected file see slectedfiles
extern int noofpicfiles;               // no of slected picfiles

// uit readpic.c
extern int ReadPic(HDC hdc,char* filenaam);

// uit poppadp.c
BOOL PrintMyPage (HWND hwnd);

// uit fileio.c
extern BOOL CMUFileOpen( HWND hWnd );
extern char *FilGetVolid(int Drive);
extern int  FilSetVolid (HWND hwnd, int Drive, char *Volid);
extern void Read_Diskvolume(int drive,char* diskname);
extern void Set_Diskvolume(HWND hwnd, int drive,char* diskname);
extern int  Set_lowest_drive(unsigned* olddrive);

// uit pictrans.c
int	pic2prn(HWND hwnd,char *inputfile,char *outputfile);
int	prn2pic(HWND hwnd,char *inputfile, char* outputfile);

//uit viewbmps.c
extern void do_paint_bmps(HWND hWnd);
//extern void do_paint_source(HWND hWnd);
//extern char bmpfile_name[];

// uit wcgraph.c
extern void	Print_metafile(HWND hwnd,HDC hdc, char *PICfilenaam);

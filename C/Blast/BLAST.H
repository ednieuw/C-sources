// Include file blast.h  E.J. Nieuwenhuys 1996

#include "windows.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "fcntl.h"
#include "dos.h"
#include "stdio.h"
#include "time.h"
#include <bios.h>
#include <ctype.h>
#include <io.h>
#include <dir.h>


#define IDM_SELECT   100
#define IDM_LOAD     101       /* hoofdmenu items */
#define IDC_MAXDAGOUD	108
#define IDM_PRT      108
#define IDM_LINEREGR 109
#define IDM_PRN      113
#define IDM_CLOSE    114
#define IDC_LOGFLAG	104
#define IDC_HEADER	105
#define IDC_YLEGEND	106
#define IDC_FONT		107
#define IDC_KADER		103
#define IDC_FFLAG		102
#define MISSING 		-107374176.0    /* &CCCCCCCC */
#define IDD_FPATH   	0x11
#define IDD_FLIST   	0x12

// string table definitions
#define IDS_NAME    	1
#define IDM_PRINT   	5
#define IDM_ABOUTP  	7

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
//	 		BLACK,    	BLUE,     		GREEN,    		CYAN,
//	 		RED,    		MAGENTA,    	BROWN,     		LIGHTGRAY,
//	 		DARKGRAY,  	LIGHTBLUE,    	LIGHTGREEN,    LIGHTCYAN,
//	 		LIGHTRED,  	LIGHTMAGENTA,  YELLOW,    		WHITE

#define CENTER 		0
#define CENTERLEFT	1
#define CENTERTOP	 	2
#define CENTERRIGHT	3
#define CENTERBOTTOM 4
#define TOPLEFT		5
#define TOPRIGHT 	 	6
#define BOTTOMLEFT 	7
#define BOTTOMRIGHT  8

#define MOVE    160
#define DRAW    162
#define FILL     48
#define FILLO   208
#define COLOR0  176
#define COLOR1  177
#define COLOR2  178
#define COLOR3  179
#define COLOR4  180
#define COLOR5  181
#define COLOR6  182
#define COLOR7  183
#define COLOR8  184
#define COLOR9  185
#define COLOR10 186
#define COLOR11 187
#define COLOR12 188
#define COLOR13 189
#define COLOR14 190
#define COLOR15 191
#define _TEXT    168
#define _FONT    167
#define _SIZE    172
#define _END      96

#define MAXSTRLEN 256
// ------------------- Winpiclib.c ------------------------------------------------------
extern void 	ConvertPicXY(double x,double y);
extern void 	Draw(HDC hdc,double xp,double yp);
extern void 	Move(HDC hdc,double xp,double yp);
extern void 	Point(HDC hdc,double xp,double yp, double box);
extern void 	Numm(HDC hdc,double xp,double yp,int number);
extern void 	Printis(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void  	PrintTop(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void  	PrintBottom(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst);
extern int  	Pic_Open(HWND hwnd);
extern void 	Pic_End(HWND hwnd);
extern void 	Picfont(char font);
extern void 	Piccolor(HDC hdc,int color,int thickness);
extern void 	Picsize(int x, int y);
extern void 	Pictext(HDC hdc, int direction,int position,char* s,float size);
extern void 	Picxy(int x,int y);
extern void 	Picbox(HDC hdc,int ox,int oy,int lx,int ly);
extern float 	Data_X_2screen(float number);
extern float 	Data_Y_2screen(float number);
extern char 	Teken_assen(HWND hwndas,HDC hdc);
extern char 	Teken_data(HWND hwndas,HDC hdc);

extern int 		Ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
extern double 	scientific2(double d, double *base, int *exp);
extern double 	val(double mantissa, int exponent);
extern int 		round(double d);
extern double 	pow10d(double exponent);
struct ASCAL{
					BYTE		Graphtype;			//type grafiek
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

//--------------- blast.c ----------------------
//extern char   huge **naam;
//extern double huge **data;
extern char   huge **label;
//extern double huge **temp;
//extern double huge **PP;
//extern double huge *sortx;
//extern double huge *sorty;
extern int	  huge *test_selection;

extern char    Picoutputfile[];
extern char    Resoutputfile[];
extern int     MAX_ROW;
extern int     PICOPEN;
extern int     geladen;
extern short   cxClient,cyClient;
extern short   cxClientOrg;
extern short   cyClientOrg; //correctiefont in dmpicplib.c
extern int	   Nooffiles;

extern short   cxChar,cxCaps,cyChar;
extern TEXTMETRIC   tm;
extern PAINTSTRUCT 	ps;
extern LOGFONT 		lf;
//extern CHOOSEFONT 	cf;

// Uit Winpiclib

extern const char 	header_vector[17];
extern const double LotusPICX;
extern const double LotusPICY;
extern double 		ScreenX  ;
extern double 		ScreenY  ;
extern const float  Pic_default_size;
extern FILE 		*fpout;
extern int   		DrawPicFile;			//Schrijf PIC file naar disk
extern int   		DrawGraphScreen;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D
extern short 		cxClient,cyClient;					// size of screen
extern int			PICOPEN;
extern short 		cxChar,cxCaps,cyChar;
extern char 		Pic_output_file[MAXSTRLEN];
extern double  	PicX,PicY,GraphX,GraphY;
extern int   		MaxX,MaxY; 					//screen coordinates
extern short 		nDrawingMode;
extern int			Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
extern RECT 		rect;
extern HPEN       hpen, hpenOld;
extern LOGPEN 		lp;

// uit Wcorrtek
extern struct ASCAL axcal;
// uit blaster

extern char y_legend[MAXSTRLEN+1];
extern int Max_dagen_oud;
extern int 	Logflag, Kader, Barsize, Fixed_distance, Fflag;

extern int  	read_wks(HWND hwnd, char *filename);
extern void 	print_letter(HDC hdc);

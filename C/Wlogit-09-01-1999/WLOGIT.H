// Header file Wlogit.h Ed nieuwenhuys 1992

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

#define IDM_INPUTDLG	301
#define IDM_FURELISA   	100
#define IDM_LOAD     	101       /* hoofdmenu items */
#define IDM_BEREKEN  	102
#define IDM_BEREKEN1 	192
#define IDM_XLOGas   	103
#define IDM_YLOGas  	104
#define IDM_RESET   	107
#define IDM_PRT     	108
#define IDM_EDITOR   	110
#define IDM_LINEPROB 	111
#define IDM_LINEXY   	112
#define IDM_PRN     	113
#define IDM_SPEARM   	115
#define IDM_XZERO   	116
#define IDM_YZERO   	117
#define IDM_Ticksin 	118
#define IDM_Ticksout 	119

#define IDM_PICFILE 	121
#define IDC_AUTOPRN		106
#define IDM_PRNFILENAME	105
#define IDM_RESFILE  	122
#define IDM_MANINPUT 	123
#define IDM_MAXX		124
#define IDM_MAXY     	125
#define IDM_MAXP    	126
#define IDM_LAMBDA   	128
#define IDC_POLYNOOM	107
#define IDC_POLYDEGREE	136
#define IDC_JOINRESULTS	115
#define IDC_LOGIT		111
#define IDC_LINEAIR	 	112
#define IDC_RID		 	114

#define IDM_CLOSE	 	201
#define IDM_OK		 	202
#define IDM_CANCEL   	203
#define IDM_HIGHCONC 	204
#define IDM_DILFACTOR 	205
#define IDM_NOOFREPL 	206

#define DATUM __DATE__
#define MISSING 		-107374176.0    /* &CCCCCCCC */
#define MAXROWS 		900
#define IDD_FNAME   	0x10
#define IDD_FPATH   	0x11
#define IDD_FLIST   	0x12

// wfile.h

// string table definitions
#define IDS_NAME    	1
#define IDM_PRINT	   	5
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
//			BLACK,    		BLUE,     		GREEN,    		CYAN,
//			RED,    		MAGENTA,    	BROWN,     		LIGHTGRAY,
//			DARKGRAY,  		LIGHTBLUE,    	LIGHTGREEN,    	LIGHTCYAN,
//			LIGHTRED,  		LIGHTMAGENTA,   YELLOW,    		WHITE

#define CENTER 		0
#define CENTERLEFT	1
#define CENTERTOP	2
#define CENTERRIGHT	3
#define CENTERBOTTOM 4
#define TOPLEFT		5
#define TOPRIGHT 	6
#define BOTTOMLEFT 	7
#define BOTTOMRIGHT 8



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
#define _TEXT   168
#define _FONT   167
#define _SIZE   172
#define _END     96

#define MAXSTRLEN 256
// ------------------- WCGRAPHG.C ------------------------------------------------------
extern int 		DoGraphDrawOpenDlg (HANDLE grInst, HWND hwndparent);
extern LRESULT 	CALLBACK  GraphDrawDlgProc (HWND, WORD, WORD, LONG);
extern void		Print_grafiek(HWND hwnd, HDC hdc);
extern int		DelInsDataPoint (HWND hwnd,long lParam);
extern char 	Teken_logit_data(HWND hwndas,HDC hdc,struct ASCAL *ascal);

// ------------------- Winpiclib.c ------------------------------------------------------
extern void 	ConvertPicXY(double x,double y);
extern void 	Draw(HDC hdc,double xp,double yp);
extern void 	Move(HDC hdc,double xp,double yp);
extern void 	Point(HDC hdc,double xp,double yp, double box);
extern void 	Numm(HDC hdc,double xp,double yp,int number);
extern void 	Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	PrintTop(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	PrintBottom(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern void 	Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst);
extern void 	Printis(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
extern int  	Pic_Open(HWND hwnd);
extern void 	Pic_End(HWND hwnd);
extern void 	Picfont(char font);
extern void 	Piccolor(HDC hdc,int color,int thickness);
extern void 	Picsize(int x, int y);
extern void 	Pictext(HDC hdc, int direction,int position,char* s,float size);
extern void 	Picxy(int x,int y);
extern void 	Picbox(HDC hdc,int ox,int oy,int lx,int ly);
extern float 	Data_X_2screen(float number,struct ASCAL *ascal);
extern float 	Data_Y_2screen(float number,struct ASCAL *ascal);
extern char 	Teken_assen(HWND hwndas,HDC hdc,struct ASCAL *ascal);
extern char 	Teken_data(HWND hwndas,HDC hdc,struct ASCAL *ascal);
extern int 		Ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
extern double 	scientific2(double d, double *base, int *exp);
extern double 	val(double mantissa, int exponent);
extern int 		round(double d);
extern double 	pow10d(double exponent);

//----------------------------- wlorek.c ------------------------------------------------
extern short  	maxregel,startregel,lastregel;
extern void  	print_regel(HDC hdc,int i);
extern int 	 	DoCalcResultDlg (HANDLE Inst, HWND hwndparent);
extern LRESULT 	CALLBACK CalcResultDlgProc (HWND, WORD, WORD, LONG);
extern BOOL  	FAR PASCAL CalcInputDlgProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
extern void  	Voerin(HWND hwnd,int regel);

 //---------------------------- wlogcalc.c -----------------------------------------------
extern void   	teken_graph(HWND hwnd,HDC hdc,struct ASCAL *ascal,int rowx,int rowy);
extern double 	logit(double meet);
extern double 	rechte(double meet);
extern int    	Check_Log(double check,int err);
extern void   	regressie(HWND hwnd,int i, int j);
extern int 	  	calc(HANDLE hgraph, HWND hwndgraph, int rowx, int rowy);
extern void   	fill_temp(HWND,int ,int );
extern int 	  	linaire_regressie(HWND hwnd, int i, int j);
extern void	  	Bereken_spreiding(double sx,double sy);

extern double 	Sres,SEslope,SEintercept,slopehigh,slopelow;
extern double  intercepthigh,interceptlow,corrlow,corrhigh;
extern double	LowYvalue, HighYvalue;       // min en max Y waarde invoer

struct ASCAL{
			BYTE		X_As;   		//data op Xas
			BYTE		Y_As;           //data op Yas
			float 	X_Min;
			float 	X_Max;
			float 	Y_Min;
			float 	Y_Max;
			BYTE	X_Astrans ;   		//Lineair=0, Log=1
			BYTE	Y_Astrans ;   		//Lineair=0, Log=1
			BYTE    Red;
			BYTE	Green;
			BYTE	Blue;   			// RGB colors line
			BYTE    Color;				// Als Color ==255 dan RGB values
			BYTE    PointColor;			// Als Color ==255 dan RGB values
			BYTE	Linethickness;
			BYTE	Pointthickness;
			int		X_Offset;
			int 	Y_Offset;
			int 	X_Length;			// length axis in units (10000)
			int 	Y_Length;			// length axis in units (10000)
			BYTE	X_Ticks;		  	// 0=inside 1=through 2=outside
			BYTE	Y_Ticks;		  	// 0=inside 1=through 2=outside
			int		X_NoofTicks;
			int		Y_NoofTicks;
			BYTE	X_Zeroforced;
			BYTE	Y_Zeroforced;
			long	Noof_DataPoints;
			};


// ---------------------- elisa.c  -----------------------------------
int Elisa(HWND hwnd);

// ---------------------- wlogit.c  -----------------------------------
extern void   clear_data(void);
extern BOOL   CMUFileOpen( HWND,int);
extern BOOL   PrintMyPage(HWND) ;
extern char   Loaded_file_name[MAXSTRLEN];

extern double huge **data;
extern char   huge **label;
extern double huge **temp;
extern char   huge **naam;  // NOG AANPASSEN --------------------------------
#define MAX_YK_PNT 150      // Maximaal 50 ijklijnpunten
#define MAX_LEN 12          // Lengte van de naam en label column

extern char   huge *ykpunten;   /*string voor actieve ijklijnpunten */

extern char   Tempfile[MAXPATH];
extern int    MAX_ROW;
extern int    PICOPEN;
extern int    HAND;
extern int	  test_selection[];
extern int    rows, columns;
extern int    labels,xx,yy,aa,rows_temp,kolom;
extern int    lost,LogXas,LogYas,LogXdata,LogYdata,berekend,geladen;
extern int 	  Line_regr,Line_prob,Line_xy;
extern int	  Ticks,Switchax,JoinResults;
extern short  StopCalcAll,NoGraph,Regressie;
extern int 	  Spearm,Xzero,Yzero;
extern int	  Numgraph;
extern double aantal;
extern double slope,intercept,C,D,reslogit;
extern double corr;
extern double somx,somy,upx,upy,maxP;
extern int    deleted;
extern char   szRealFileName[MAXSTRLEN] ;
extern short  cxClient,cyClient;
extern short  cxChar,cxCaps,cyChar;
extern int    Manualinput,Invoerscherm;
extern 		  TEXTMETRIC    tm;
extern 		  PAINTSTRUCT 	ps;

// ----------------------- Uit Winpiclib -----------------------------------------------

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
extern char 		Pic_output_file[MAXPATH];
extern double  		PicX,PicY,GraphX,GraphY;
extern int   		MaxX,MaxY; 					//screen coordinates
extern short 		nDrawingMode;
extern int			Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
extern RECT 		rect;
extern HPEN       	hpen, hpenOld;
extern LOGPEN 		lp;

// uit Wlortek
extern struct ASCAL axcal;

// uit polynoom.c
#define MAX_POLYNOOM 10
extern int Polynoom(HWND hwnd,int i, int j);
extern double Poly(double y);

extern int polynoomgraad;
extern double coeifpolynoom[MAX_POLYNOOM+2];
extern double polycoeif[500];
typedef struct {
			float 	input;
			float 	dilution;
			double 	result;
			double 	result_upper;
			int 	result_dev;
			char 	overflow;
			char 	monsternaam[MAX_LEN];
			int 	pointer;
			} RESULTS;
extern RESULTS *Logit;




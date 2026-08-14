#include "include\stdio.h"
#include "include\stdlib.h"
#include "include\alloc.h"
#include "include\process.h"
#include "include\math.h"
#include "include\io.h"
#include "include\string.h"
#include "include\ctype.h"
#include "include\conio.h"
#include "include\fcntl.h"
#include "include\dos.h"
#include "include\dir.h"
#include "include\bios.h"
#define TRUE            1
#define FALSE           0

typedef unsigned char       Byte;
typedef unsigned short      Word;
typedef unsigned long       Dword;
typedef unsigned int        Uint;
typedef unsigned short      Ushort;
#define __COLORS                      /* avoid duplication in conio.h */
//#define LINECOLOR(x)   rgb[lcolorstring[x%strlen(lcolorstring)]-'A']
//#define SYMBOLCOLOR(x) rgb[scolorstring[x%strlen(scolorstring)]-'A']
#define LINESTYLE(x)   linetypestring[x%strlen(linetypestring)]
#define SYMBOLSTYLE(x) symbolstring[x%strlen(symbolstring)]
#define BLACK   rgb[27]
#define RED     0X6060FFL
#define GREEN   0X60FF80L
#define BLUE    0XFF6060L
#define MAGENTA 0XFF60FFL
#define YELLOW  0X60FFFFL
#define CYAN    0XFFFF60L
#define CYAN2   0X7F7F00L
#define PURPLE  0X7F007FL
#define PURPLE2 0X3F007FL
#define PURPLE3 0XFF7FFFL
#define ORANGE  0X3F7FFFL
#define BROWN   0X00007FL
#define GREEN2  0X007F7FL
#define GREEN3  0X7FFF7FL
#define GREEN4  0X3FFF7FL
#define BLUE2   0XFF7F7FL
#define BLUE3   0XFF7F00L
#define BLUE4   0XFF3F00L
#define PINK    0X7F00FFL
#define PINK2   0X3F00FFL
#define PINK3   0X3F7FFFL
#define GREY    0X7F7F7FL
#define GREY2   0X3F3F3FL
#define GREY3   0X3F7F3FL
#define WHITE   rgb[26]
/* licht blauw:
#define PICCOLOR 0XE0E0C0L
#define PAPERCOLOR 0XE0E0D0L
#define PICCOLOR   0XE0D0D0L
#define PAPERCOLOR 0XE0A0A0L
#define PICCOLOR   0X2C0040L
#define PAPERCOLOR 0X000060L
*/
#define PICCOLOR   0X880000L
#define PAPERCOLOR 0X680000L
#define RGB0  PICCOLOR
#define RGB1  BLUE
#define RGB2  CYAN
#define RGB3  BROWN
#define RGB4  GREEN2
#define RGB5  GREEN3
#define RGB6  GREEN
#define RGB7  BLUE2
#define RGB8  BLUE3
#define RGB9  PINK
#define RGB10 GREY
#define RGB11 PURPLE2
#define RGB12 MAGENTA
#define RGB13 PURPLE3
#define RGB14 ORANGE
#define RGB15 PURPLE
#define RGB16 GREEN4
#define RGB17 RED
#define RGB18 BLUE4
#define RGB19 PINK2
#define RGB20 CYAN2
#define RGB21 PINK3
#define RGB22 GREY2
#define RGB23 GREY3
#define RGB24 YELLOW
#define RGB25 PAPERCOLOR
#define RGB26 BLACK
#define RGB27 WHITE
#define MAXLEN 81
#define PENWIDTH 12
#define PENSTYLE 0
#define BOLD 0
#define ITALIC 0
#define UNDERLINE 0
#define CHARSET 0
#define PENCOLOR   (Color?0XFFFFFFL:0L)       /* 0=black, FF-red, FF00=green, FF0000=blue*/
#define BRUSHCOLOR (Color?0XFFFFFFL:0L)
#define TEXTCOLOR  (Color?0XFFFFFFL:0L)
#define BRUSHSTYLE 0

//#define HEIGHT 24566
//#define WIDTH 32766

//#define HEIGHT 12283
//#define WIDTH  16383

//#define HEIGHT 5397
//#define WIDTH 6985

#define HEIGHT 19000
#define WIDTH 25200
#define MISSING     -999999.000
#define TEXTSIZE        15
#define VSIZE           (TEXTSIZE*9)
#define HSIZE           (TEXTSIZE*8)
#define X_ORIGIN        (HSIZE/2)
//#define X_LENGTH        (3200-HSIZE)
#define X_LENGTH        3198
#define LOG0            240 /* 0-position on logarithmic abscissa */
#define Y_ORIGIN        VSIZE
//#define Y_LENGTH        (2300 - VSIZE)
#define Y_LENGTH        2298
//#define WFACTOR         (0.95*(float)HEIGHT/(float)Y_LENGTH)
#define WFACTOR         (0.9*(float)HEIGHT/(float)Y_LENGTH)
#define PICTOP          Y_LENGTH
#define RIM             100 /* tick-mark size */
#define EXTRA_RIM       0 /* space on x-axis < xmin */
#define INTRA           16 /* space between bars in PIC-units */
#define FILL            TRUE
#define HGRID           FALSE
#define VGRID           FALSE
#define BOX             FALSE /* Draw box surrounding legends */
#define LITERAL         FALSE
#define XFORMAT         0
#define YFORMAT         0
#define XY              FALSE   /* draw x=y line  */
#define Y1LINE          TRUE
#define CAP             50
#define LEGEND_SYMBOL_SIZE 64
#define MISSING_CALIBRATOR -999
#define MARGIN              50
#define SYMBOL_SIZE 40
#define XSIZE           160 /* horizontal size of connecting line in legend block */
#define MAXNOOFCALIBRATORS  10
#define SD 0.01
#define VC 1
#define RSLOPEMIN 0.65
#define OUTFILES 1111       /* WMF,PLA,PR0,PIC */
#define WMFVSIZE 40
#define WMFHSIZE 24
#define WMFCENTER 130
#define WMFCENTERLEFT 124
#define WMFCENTERTOP 6
#define WMFCENTERRIGHT 126
#define WMFCENTERBOTTOM 30
#define WMFTOPLEFT 0
#define WMFTOPRIGHT 2
#define WMFBOTTOMLEFT 24
#define WMFBOTTOMRIGHT 26
#define OBJECTS 20
#define PEN 0
#define BRUSH 1
#define FONT 2
/* pic.h: header for pic-files 280288 */

#define MOVE 160
#define DRAW 162
/*
#define FILL 48
#define FILLO 208
*/
#define COLORS 28     /* number of colors*/
#define COLOR0 176
#define TEXT 168
#define PICFONT 167
#define SIZE 172
#define END 0X60
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8
#define PIXELS_PER_LINE 2
#define MARGIN              50
#define X_LEGEND        "units/ml"
#define Y_LEGEND        "% radioactivity bound"
/*
#define PRNFILE         "a:line.prn"
#define PICFILE     "a:line.pic"
*/
#define RETURN 13
#define CR 13
#define LARGE 0
#define MEDIUM 1
#define SMALL  2
#define LINEAR 3
#define SHOW            TRUE
#define LITERAL         FALSE
#define XFORMAT         0
#define YFORMAT         0
#define XY              FALSE   /* draw x=y line  */
#define Y1LINE          TRUE
#define CAP             50
#define LEGEND_SYMBOL_SIZE 64
#define MAX_LEN            240

enum {
STRING_ARRAY=1,
SINGLE_STRING,
INTEG,
FLOATING,
INT_ARRAY,
FLOAT_ARRAY,
RGB_ARRAY
};
extern int    *Int_array;                /* [NOOFINTS+1] */
extern int    *valid_data;               /* [SERIES] */
extern int    *range_size;               /* [RANGES+1] */
extern int    *range_start;              /* [RANGES+1] */
extern int    *range_startpos;           /* [RANGES+1] */
extern int    *range_type;               /* [RANGES+1]: axis-connection: left=0 right=1 */


/* STRING_ARRAY */
#define NOOFSTRING_ARRAYS 1
#define DA_extra_text 0

/*STRING*/
#define NOOFSTRINGS 8
enum {
DS_linetypestring,
DS_symbolstring,
DS_lcolorstring,
DS_scolorstring,
DS_input_path,
DS_output_path,
DS_input_ext,
DS_output_ext,
DS_logfiledir,
DS_logfile,
DS_WMFviewer
};

#define NOOFINTS 39
#define Cap         Int_array[0]
#define Hgrid       Int_array[1]
#define Vgrid       Int_array[2]
#define Y1line      Int_array[3]
#define Xformat     Int_array[4]
#define Yformat     Int_array[5]
#define Legend      Int_array[6]
#define Legendpos   Int_array[7]
#define Logxflag    Int_array[8]
#define Logyflag    Int_array[9]
#define Xy          Int_array[10]
#define Rim         Int_array[11]
#define Extra_rim   Int_array[12]
#define Symbol_size Int_array[13]
#define Line        Int_array[14]
#define Literal     Int_array[15]
#define Box         Int_array[16]
#define Color       Int_array[17]
#define Logy2flag   Int_array[18] /* used to be:Plotter */
#define Xcal        Int_array[19]
#define Ycal        Int_array[20]
#define X_length    Int_array[21]
#define Y_length    Int_array[22]
#define Textsize    Int_array[23]
#define Statflag    Int_array[24]
#define Range_check Int_array[25]
#define X_origin    Int_array[26]
#define Xet         Int_array[27] /* x-pos extra text */
#define Yet         Int_array[28]
#define Bare        Int_array[29]
#define X_start     Int_array[30]
#define Y_start     Int_array[31]
#define Show        Int_array[32]
#define Readmode    Int_array[33]
#define Bar_size    Int_array[34]
#define Intra       Int_array[35]
#define Showstat    Int_array[36]
#define Outfiles    Int_array[37]
#define Replica     Int_array[38]

#define NOOFFLOATS 11
#define Xmax Float_array[0]
#define Xmin Float_array[1]
#define Ymax Float_array[2]
#define Ymin Float_array[3]
#define Ylo  Float_array[4]
#define Yhi  Float_array[5]
#define Y2max Float_array[6]
#define Y2min Float_array[7]
#define Sd Float_array[8]
#define Vc Float_array[9]
#define Rslopemin Float_array[10]


extern Word brush(Word style, Word hatch, Dword color);
extern void center(int lines, float *cx, float *cy, float *cz, float *min, float *max);
extern char *colormenu(char *s);
extern int count_char(char *s, char c);
extern void dotted_h_line(int x0, int x1, int y0);
extern void dotted_v_line(int x0, int y0, int y1);
extern void draw_line(int n, float xmin, double scale_xfactor, float ymin, double scale_yfactor, int wmf_flag);
extern void draw_regression(int n);
extern void draw_xy(int x_origin, float xmin, float xmax, double x_factor, int y_origin, float ymin, float ymax, double y_factor);
extern void init_colors(void);
extern Word CreateFont(Word orientation, Word Font, Word HsizeWMF, Word VsizeWMF);
extern Word CreateObject(Word obj);
extern int get_legendlines(void);
extern int getbarsize(int dataline);
extern char *keycopy(char *target,char *source,int maxlen);
extern void openout(char *filename);
extern char *find_monthname(char *name, int month, double size_per_month);
extern Word pen(Word style, Word width, Dword color);
extern void pic0draw(int x,int y);
extern void pic0move(int x,int y);
extern int  picbottomline(int n);
extern void piccolor(int color);
extern int  piccopy(char *source, char *target);
extern void piccorner();
extern int  picdot(int x,int y, int type, int size, int dataline);
extern int  picdotdot(double step1,double step2,double xfactor,double yfactor,double *xpos,double *ypos,double xlim,double ylim);
extern void picdotted_line(int x0, int y0, int x, int y, int type, Dword color, int wmf_flag);
extern void picdraw(int x,int y);
extern void picend(void);
extern void picextra_text(int x_origin, int y_origin, int lx, int ly);
extern void picfont(char font);
extern void picheaders(int headerlines);
extern void piclegend(int legends_per_line);
extern void picmove(int x,int y);
extern void picopen(char *s);
extern void picrectangle(int xpos,int ybase,int xpos2,int y, int fill);
extern void picsize(int x,int y);
extern void pictext(int direction,int position, char *s);
extern int  pictextlines(int xpos, int ypos, char *text, int direction, int position, int picflag);
extern int  pictopline(int n);
extern int  picx(float x);
extern int  picx_cal(int logflag, int picflag);
extern void picxy(int x,int y);
extern int  picy(float y, int range);
extern int  picy_cal(int logflag, int picflag);
extern int  picy_axis_title(char *y_axis_string, int y_pos, int picflag);
extern int  picy2_cal(int logflag, int picflag);
extern int  picy2_axis_title(char *y2_axis_string, int y2_pos, int picflag);
extern void plot_point(int column, int range_number, int bar);
extern void polyline(int r);
extern void putbyte(Byte b);
extern void putdword(Dword dw);
extern void putword(Word w);
extern int round(double d);
extern double val(double mantissa, int exponent);
extern double scientific2(double d, double *base, int *exp);
extern void wmftextcolor(Dword color);
extern void wmfdot(Word x, Word y, Word size, Word style, Dword color);
extern void wmfdraw(Word x,Word y, Word style, Word width, Dword color);
extern void wmfend(void);
extern void wmfline(Word x0,Word y0,Word x1,Word y1, Word style, Word width, Dword color);
extern void wmfmove(Word x,Word y);
extern void wmfopen(void);
extern void wmfpolylinestart(Word x, Word y, Word points, Word style, Word width, Dword color);
extern void wmfpolylinestep(Word x,Word y);
extern void wmfrectangle(Word x0, Word y0, Word x1, Word y1, Word brushstyle, Dword pencolor, Dword brushcolor);
extern void wmfsquare(Word x, Word y, Word size, Word style, Dword color);
extern Word wmftext(Word direction, Word alignment, Dword color, char *s);
extern void wmftriangle(Word x, Word y, Word size, Word style, Dword color);
extern void wmfdiamond(Word x, Word y, Word size, Word style, Dword color);
extern void write_wmf(void);
extern long LINECOLOR(int pos);
extern long SYMBOLCOLOR(int pos);
extern long rgb[28];

extern int Pen,Brush,Font, Filltype, VsizeWMF, HsizeWMF;
extern int Wmftextsize;
extern int FontStyle, BrushStyle, Bold, Italic, Underline, Charset;
extern Dword BrushColor;
extern Dword PenColor;
extern Dword TextColor;
extern Byte NewPenColor, NewBrushColor;
extern Word PenWidth, PenStyle, NoAsk;
extern int object[OBJECTS];
extern Word noofobjects, Orientation;
extern Word Alignment;
/* no vertical central alignment! Only: Base, Bottom and Top
 0  6  2       1  7  3
 8 14 10       9 15 11
24 30 26      25 31 27
introduced: 125, 131 and 127 for vertical central alignment
*/
extern Word vertices,alignment,direction,position;
extern Word currentx, currenty;
extern Dword maxrecordsize, filelen;
extern Dword nullvalue;
extern Dword *nullp;
extern char *first_title;                /* [MAX_LEN+1] */
extern char *second_title;               /* [MAX_LEN+1] */
extern char *Input_path;                 /* [41] ="A:" */
extern char *Input_ext;                  /* [5] =".PRN" */
extern char *Output_ext;                 /* [5] =".PRN" */
extern char *Output_path;                /* [41]="A:" */
extern char *x_legend;                   /* [MAX_LEN+1] */
extern char *y_legend;                   /* [MAX_LEN+1] */
extern char *y2_legend;                  /* [MAX_LEN+1] */
extern char *symbolstring;               /* [80]="0123456789" */
extern char *linetypestring;             /* [20]="012345678"*/
extern char *lcolorstring;               /* linecolors: [40]="012345678"*/
extern char *scolorstring;               /* symbolcolors: [80]="012345678"*/
extern char *l0colorstring;               /* backup linecolors: [40]="012345678"*/
extern char *s0colorstring;               /* backup symbolcolors: [80]="012345678"*/
extern float xmin, xmax, ymin, ymax,y2min, y2max, units;
extern int result, ranges, headerlines, Trace;
extern int data_columns, Data_columns, dual_axis;
extern double scale_xfactor, scale_yfactor, scale_y2factor, slope, r2;
extern int maxlegend, legendblocksize;
extern int xll, xul, yll, yul;                 /* xll=x_lower_limit in screen units */
extern int xllflag, xulflag, yllflag, yulflag; /* xulflag: data_points higher than Xmax found */
extern int logxflag, logyflag, logy2flag;
extern int x_origin, x_length, log0;
extern int y_origin, y_length;
extern float upperlimit;
/*
te definieren in eigen header, bv pla.h
extern float xx1;
extern float xx2;
extern float xx3;
extern float yy1;
extern float yy2;
extern float yy3;
*/
extern int DX;
extern int DY;
extern int Vsize;
extern int Hsize;
extern int YSIZE;
//extern int Show;
extern char *first_title;                /* [MAX_LEN+1] */
extern char *second_title;               /* [MAX_LEN+1] */
extern char *Input_path;                 /* [41] ="A:" */
extern char *Output_path;                /* [41]="A:" */
extern char *x_legend;                   /* [MAX_LEN+1] */
extern char *y_legend;                   /* [MAX_LEN+1] */
extern char *y2_legend;                  /* [MAX_LEN+1] */
extern char *symbolstring;               /* [80]="0123456789" */
extern char *linetypestring;             /* [20]="012345678"*/
extern char *lcolorstring;               /* linecolors: [40]="012345678"*/
extern char *scolorstring;               /* symbolcolors: [80]="012345678"*/
extern char *l0colorstring;               /* backup linecolors: [40]="012345678"*/
extern char *s0colorstring;               /* backup symbolcolors: [80]="012345678"*/
extern char *datapoint_code;
extern char *prnfile;                    /* [41] */
extern char *outfile;                    /* [41] */
extern char *fln_filename;               /* [41] */
extern char *buffer;                     /* [MAX_LEN] */
extern char *picfile;                    /* [20]= "C:LINE.PIC" */
extern char *wmffile;                    /* [20]= "C:LINE.WMF" */
extern char *plafile;                    /* [20]= "C:LINE.PLA" */
extern char *prnfile;                    /* [20]= "C:LINE.PRN" */
extern char **name;
extern char **extra_text;                /* [10][MAX_LEN+1] */
extern char **filename;                  /* [MAXFILES][LENFILENAME+1] */
extern char **range_name;                /* [RANGES+1][MAX_LEN+1] */
extern char **calibratorstring;          /* [MAXNOOFCALIBRATORS][12] */
extern char **headerline;                /* [HEADERLINES][MAX_LEN] */
extern char **stringline;                /* [HEADERLINES+2][MAX_LEN] extra lines: x- and y-legend */

#ifdef TRACE
   extern int Trace;
#endif

extern FILE *fpin;
extern FILE *fpout;
extern FILE *fpwmf;
extern FILE *fppla;
extern FILE *tracefile;

extern int result, calcflag;
extern int maxrange;
extern unsigned columns;
extern char monthname[12][10];
extern int calbase[4][4];
extern int noofcalibrators;
extern int startup_drive;
extern float  *Float_array;              /* [NOOFFLOATS+1] */
extern float  *x1;
extern float  *x2;
extern float  *x3;
extern float  *y1;
extern float  *y2;
extern float  *y3;
extern float  *intercept;                /* [RANGES+1] */

extern double *islope;                   /* [SERIES] */
extern double *meanx;                    /* [SERIES] */
extern double *meany;                    /* [SERIES] */
extern double *value_of_calibrator;      /* [MAXNOOFCALIBRATORS] */

extern char header_vector[17];



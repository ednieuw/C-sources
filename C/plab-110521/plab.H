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

#define VERSION "1.1b1"
#define SEGMENTS 100
#define MAXREPLICA 20
#define TRACE 0
#define INPUT_PATH "C:"
#define INPUT_EXT ".PR?"
#define OUTPUT_EXT ".PR0"
#define OUTPUT_PATH "C:"
#define COLUMNS            512    /* LINE: aantal datapunten */
#define SERIES              24     /* max nr of curves for PLA-analysis */
#define RANGES         COLUMNS
#define DATA_COLUMNS         11
#define HEADERLINES         20
#define NAME_LEN            40     /* data-label length (after removal of dubbelquotes, if present */
#define BUFFERLEN          242
#define LENFILENAME         12
#define SCREENSTARTROW       4
#define NOOFLINES          (25-SCREENSTARTROW)
#define SCREENCOLUMNS       80
#define TEXTCOLUMNS          4
#define MAXFILES           (TEXTCOLUMNS*NOOFLINES)

#define LOGXFLAG    TRUE
#define LOGYFLAG    FALSE
#define LOGY2FLAG   FALSE
#define LINE        FALSE   /* connect points */
#define SYMBOL_STRING "135724689"
#define STARTUP_STRING "pla43.EXE"
#define XCAL        3      /* 3=closed: type of axes drawn 0-3 */
#define YCAL        3      /* type of axes drawn 0-3 */
#define Eover2 1.3591409


#define FLNCODE "\\/RCAFLN03"
#define ENDCODE "#\\#"
#define LEGEND          2   /* number of lines for legends */
#define LEGENDPOS       1   /* legends below the graph */
//#define LOGFLAG         FALSE
#define OFF ' '
#define ON  '#'
#define HLON   {textcolor(0); textbackground(7);}
#define HLOFF  {textcolor(7); textbackground(0);}
#define CURSOR(a,b) gotoxy(a,b)
#define MORE   gotoxy(70,25);HLON;cprintf(" MORE --> ");HLOFF;getch();CLS
#define QUOTE 34

/* keyboard.h 230288 */
#define QUOTE 34
#define SPECIAL_KEY 0
#define UP 328
#define DOWN 336
#define HOME 327
#define SHIFT_HOME 382
#define LEFT 331
#define RIGHT 333
#define PgUp 329
#define PgDn 337
#define EXEC 335
#define ESC 27
#define CANCEL 366
#define INSERT 338
#define DELETE 339
#define F1 315
#define F2 316
#define F3 317
#define F4 318
#define F5 319
#define F6 320
#define F7 321
#define F8 322
#define F9 323
#define F10 324
#define HELP 367 /* F1 toets */
#define SPACE 32
#define BACKSPACE 8
#define TAB 9
#define BACKTAB 271
#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_Z 26

extern void startup_message(void);
extern void allocate(void);
extern int get_options(void);
extern void no_room(void);
extern void minmax_menu(void);
extern void legend_menu(void);
extern void position_menu(void);
extern void format_menu(void);
extern void range_menu(void);
extern void extra_text_menu(void);
extern int isdecimal(char c);
extern int isdelimiter(char c);
extern int isnumber(char *s);
extern int isvoid(char *s);
extern void printstring(char c,int n);
extern void frame(int hor,int vert);
extern void down(char c,int n);
extern int strmenu(int current_pos, int rows, int startrow,int nooflines);
extern char *strleft(char *target,char *source, char *mask);
extern char *getmask(char *default_mask);
extern int getdir(char *s);
extern void makedate(int *m,int *d,int *y,unsigned total);
extern void maketime(int *h,int *m,int *s,unsigned total);
extern char *getfile(char *mask);
extern int read_data(char *filename, char *infostring, int show);
extern void openin(char *filename);
extern int  fgetline(char *line,int maxlen, FILE *fpin);       /* returns no of non_white chars */
extern int parse_line(char *buffer, int dataline);
extern void prepare_pic(void);
extern int numbers_per_line(char *buffer);
extern char *stripwhites(char *s);
extern int check_output_file(char *new_name, char *default_name);
extern int check_dosname(char *s);
extern void legend_menu();
extern void column_menu();
extern int getkey(int flag);
extern int ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag, int format, int xflag);
extern double pow10d(double exponent);
extern double pla(int startS, int NSd, int startT, int NTd);
extern void edit_data(void);
extern int picsave(char *source, char *target, char *ext);
extern float getfloat(int *key, int field);
extern char *makefilename(char *full_filename, char *default_path, char *default_ext, int forced_ext_flag);
extern char *strnleft2(char *buffer, char *s1, char c, int n);
extern int strpos(char *s,char c,int pos);
extern int strpos2(char *s,char c,int pos);
extern int getmaxlegend(void);
extern void  yesno(int flag, char *yesstring, char *nostring);
extern void initialise_arrays(void);
extern void write_fln(char *s);
extern char *ftoa(float f);
extern int check_drive(int drive);
extern void err0(char *s, int line);
extern double transform(float y, float sd, float vc);
extern double backtransform(float y, float sd, float vc);
extern double getmeany(float y1, float y2, float sd, float vc);
extern char *flag;                       /* [COLUMNS] */
extern char *startchars;                 /* [MAXFILES] */
extern char *charpointer;
extern char *infostring1;
extern char *infostring2;
extern float *x;
extern float *y[MAXREPLICA+2];
extern int *ny;
extern float xx1;
extern float xx2;
extern float xx3;
extern float yy1;
extern float yy2;
extern float yy3;


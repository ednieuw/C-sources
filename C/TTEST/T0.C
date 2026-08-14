/* t0 = readwks Eco-C: 040388 */
/* turbo-C version: 220388 */
/* update: 200488 Turbo 1.5 */
/* most recent modifications: 110688,060788,250788, 310888, 261188 */
/* SE (rather than 95% confidence intervals) centile 030788 */
/* 050589 floats ipv longs: protocol=2; MAX_LEN 80 */

/* compile; -C nested comments allowed:
if exist error del error
if exist t0.exe del t0.exe
tcc -mh -C t0 >error
if exist t0.exe del t0.obj
type error
*/

/* compile t.b.v. debugger (dc.bat)
if exist error del error
if exist t0.exe del t0.exe
tcc -mh -v -C t0 >error
if exist t0.exe del t0.obj
type error
*/

/* compile met stack-overflow check:
if exist error del error
if exist t0.exe del t0.exe
tcc -mh -N -C t0 >error
if exist t0.exe del t0.obj
type error
*/

/* wksdata.inf = info-file (ASCII)                    */
/* wksdata.dat = data-file (float)                    */
/* wksdata.u   = test-units = row 2                   */
/* wksdata.prn = testname, (n), ile25, ile50, ile75       */
/* wksdata.ids = sample-identification    = col 1     */

#define STARTUP_MESSAGE1 cprintf("Programma \"Ttest deel 1: T0.EXE\" dd %s",__DATE__)
#define STARTUP_MESSAGE2 cprintf("Inlichtingen en fouten-melding: Rob Aalberse")
#define LOTUS1 0X0404
#define LOTUS2 0X0406
#define MAX_ROW 512
#define MIN_ROW 10
#define MAX_COL 48
#define MIN_COL 1
#define MAX_LEN 80
#define ID_LEN 20
#define NAMEROW 0
#define UNITROW 1
#define STARTROW 2
#define CODECOLUMN 0
#define STARTCOLUMN 1
#define MISSING -107374176.0    /* &CCCCCCCC */
#define MAXVALUE 999999999.0
#define MINVALUE MISSING
#define PROTOCOL_CODE 3         /* data: floats; MISSING= (float) &CCCCCCCC */
#define CENTILE 0.0
#define SE 1.0

#define MAXNOOFITEMS 80
#define HLON   textcolor(BLACK); textbackground(LIGHTGRAY)
#define HLOFF  textcolor(LIGHTGRAY); textbackground(BLACK)
#define CLS    clrscr()
#define CURSOR(a,b) gotoxy(a,b)
#define MORE   gotoxy(70,25);HLON;cprintf(" MORE --> ");HLOFF;getch();CLS
/*
#define HLON  cprintf("\033[7m")
#define NORM  cprintf("\033[0m")
#define CLS   cprintf("\033[2J")
#define CURSOR(a,b) cprintf("\033[%d;%dH",a,b)
#define MORE CURSOR(25,70);HLON;cprintf(" MORE --> ");NORM;getch();CLS
*/
#define LENFILENAME 80
#define SCREENSTARTROW 4
#define NOOFLINES (25-SCREENSTARTROW)
#define SCREENCOLUMNS 80
#define TEXTCOLUMNS 4
#define MAXFILES (TEXTCOLUMNS*NOOFLINES)

#define M0 cprintf("\r\nNo filename entered; press RETURN to re-try\r\n")
#define M1 cprintf("reading file %s\r\n",filename)
#define M2 cprintf("output-files:\r\n\r\nwksdata.inf: file-information\r\n")
#define M4 cprintf("wksdata.u  : test-units\r\n")
#define M5 cprintf("wksdata.ids: sample-identifications (N=%d)\r\n",rows)
#define M6 cprintf("wksdata.dat: actual data (numeric)\r\n")
#define M7 cprintf("wksdata.prn: 25-, 50-, and 75- percentiles\r\n")
#define M1_getfile cprintf("No files in directory %s\r\ntry again ... \r\n\r\n",s)
#define M2_getfile cprintf("Enter directory search mask ( press Esc to exit):\r\n%s ",mask)
#define M3_getfile cprintf("Mask: %s. Select file and press RETURN (press Esc to exit)",s)
#define M4_getfile cprintf("File Name      Size   Date    Time")
#define M5_getfile cprintf("%-12s%7ld %2.2d/%2.2d/%2.2d %2.2d:%2.2d\r\n",filenames[n],ffblk.ff_fsize,month,day,year,hour,minute)
#define M0_strmenu cprintf("%-13s",array[n])
#define E0 cprintf("cannot open input file %s; press RETURN to re-try, Esc to quit\r\n",filename)
#define E1 cprintf("read error at byte count %ld",counter)
#define E2 cprintf("file-type: %4X ipv %4X of %4X: dus geen LOTUS-worksheet",file_type,LOTUS1,LOTUS2)
#define E3 cprintf("cannot open \"%s\" output file\r\n",f)
#define E4 cprintf("error writing \"%s\" output file\r\n",f)
#define E5 cprintf("\r\nError: No tests found\r\n\r\nThe first row MUST contain test-names\r\npress RETURN to re-try\r\npress ESC to quit")
#define E6 cprintf("\r\nError: No samples found\r\n\r\nThe first column MUST contain sample-identification-namesn\r\npress RETURN to re-try\r\npress ESC to quit")
#define E7 cprintf("\r\nError: Out of memory\r\n")

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
#define RETURN 13
#define BACKSPACE 8
#define CR 13
#define TAB 9
#define BACKTAB 271
#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_Z 26

#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <mem.h>

/* function prototypes: */
void print_extract(void);
void startup_message(void);
void printstring(char c,int n);
void frame(int hor,int vert);
void down(char c,int n);
void no_room(void);
int strmenu(int current_pos, int rows, int startrow,int nooflines);
char *strleft(char *target,char *source, char *mask);
char *keycopy(char *target,char *source,int maxlen);
char *getmask(char *default_mask);
/* int getdir(char *s,char filenames[][]); */
int getdir(char *s);
void makedate(int *m,int *d,int *y,unsigned total);
void maketime(int *h,int *m,int *s,unsigned total);
char *getfile(char *filename, char *mask);
int getfilename(int argc,char **argv);
char *makefilename(char *full_filename, char *default_path, char *default_ext, int forced_ext_flag);
void read_wks(void);
int lotustonumber(char *s);
char *numbertolotus(int i, char *s);
int islabelfield (int row, int column, char *s);
int isname( char *s);
int islimits(int row, int column);
int strpos(char *s,char c,int pos);
void make_label(int labelfield, int row,int column,char *stringbuffer);
void read_environment(void);
float set_env_variable(char env_string[], float default_value);
void clear_array(void);
char *clear_string(char *s);
void skip(FILE *fp,int noofbytes);
void openout(char *s);
void dump_block( unsigned char *pointer, int noofbytes);
float minimum_value(int column);
float maximum_value(int column);
void dump_info(char *f);
void dump_data(char *f);
int count_testnames(void);
void dump_testunits(char *f);
void dump_ile(char *f);
char *quoted(char *buffer, char *s);
void dump_minmax(char *f);
int dump_sample_ids(char *f);
void dump_ranks(char *f);
int check_wks(void);
void rank_data(double centile);
void qs(int column, int rows);		/* quicksort prelude */
void qs0(int left, int right);		/* quicksort */
int get_column(void);
print_ranks(void);
int get_row(int column,int score);
float get_ile(int column,double ile);
void no_room(void);
void make_ruler(int x, int y, int units, int unitsize);
char *strnleft2(char *buffer, char *s1, char c, int n);
int getkey(int flag);
void farewell(int exit_code);
void write_pointers(void);
double ilevalue(int n, double level, double centile);
void clear_hugearray(void);

/********* global variables *********/

union lofl {
   long  lo;
   float fl;
};

char buffer[256];
int result;
int startup_drive, data_drive=0;
char filename[LENFILENAME+1];
char shortfilename[13];
char filenames[MAXFILES][LENFILENAME+1];
char drive[]="A:";
char default_drive[]="A:";
char dir[MAXDIR];
char default_dir[MAXDIR];
char ext[]=".WK1";
char codestring[]="1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()-+=,<.>/?|`~\\";

FILE *fpin;
FILE *fpout, *tracefile;
char Input_path[80]	="A:";
unsigned int rows, columns;
int Trace, Show, Startrow, Max_row, Namerow, Unitrow, Min_row, Startcolumn, Max_col, Codecolumn, Min_col;
double Centile=CENTILE, Se=SE;
                /* Centile    0: 25,50,75 centiles
				   Centile 0.75: 75 centile (SE)
				   Centile 0.50: 50 centile (SE)
*/

float huge *datapointer;     /* [MAX_ROW][MAX_COL];          */
float huge * huge *data;
int  far *rankpointer;		/* [MAX_ROW][MAX_COL];			*/
int far **rank;
int  far *valid_data;		/* [MAX_COL];				*/
float far *i25;			/* [MAX_COL];				*/
float far *i50;
float far *i75;
int temprank[MAX_ROW];
float tempdata[MAX_ROW];
char testname[MAX_COL+1][MAX_LEN+1];
char testunit[MAX_COL+1][MAX_LEN+1];
char samplename[MAX_ROW+1][ID_LEN+1];
char header1[MAX_LEN+1];
char header2[MAX_LEN+1];
char startchars[MAXFILES];

enum labelfieldtype {NOLABEL, HEADER1, HEADER2, TESTNAME, TESTUNIT, SAMPLENAME }


/***************** START OF MAIN ************************/

main(int argc,char *argv[]) {
   int i;
   CLS;
   setcbrk(1);
   if (argc>1) {
      data_drive=argv[1][0]-'A';
      if (data_drive<0 || data_drive>5) data_drive=0;
   }
   read_environment();
   if (Trace && (tracefile=fopen("trace","wt"))==0) {
      cprintf("cannot open \"trace\"");
      farewell(-1);
   }
   if ((datapointer = farcalloc( (long) MAX_ROW * MAX_COL, (long) sizeof(float))) == 0 ) no_room();
   if ((rankpointer = farcalloc( (long) MAX_ROW * MAX_COL, (long) sizeof(int)))==0) no_room();
   if ((data		= farcalloc( (long) MAX_ROW , (long) sizeof(data)))==0) no_room();
   if ((rank		= farcalloc( (long) MAX_ROW , (long) sizeof(rank)))==0) no_room();
   if ((valid_data	= calloc( MAX_COL  , sizeof(int)))==0) no_room();
   if ((i25 		= calloc( MAX_COL  , sizeof(float)))==0) no_room();
   if ((i50		= calloc( MAX_COL  , sizeof(float)))==0) no_room();
   if ((i75		= calloc( MAX_COL  , sizeof(float)))==0) no_room();
   if (Trace) write_pointers();
   do {
      i=-1;
      do {
        i=getfilename(argc,argv);
        if (i<=0) {
           CLS;
           E0;
           if (getch()==ESC) farewell(-1);
        }
      } while (i<=0);
      do {
         clear_array();
         setdisk(0);
         read_wks();
         setdisk(startup_drive);
         columns=count_testnames();
         dump_testunits("wksdata.u");
         rows=dump_sample_ids("wksdata.ids");
         if (Show) result=check_wks();
         else result=1;
         if (Trace) fprintf(tracefile, "\r\n%4d rows=%d columns=%d Startrow=%d Startcolumn=%d",
            __LINE__,rows, columns, Startrow, Startcolumn);
      } while (!result);
      if (result==-1) continue;
      rank_data(Centile);
      dump_ile("wksdata.prn");
      dump_data("wksdata.dat");
      dump_info("wksdata.inf");
      dump_ranks("wksdata.rnk");
   } while (result<1);
   farewell(1);
   return 1;
}

/************************* END OF MAIN ************************/

void farewell(int exit_code) {
   setdisk(startup_drive);
   exit(exit_code);
}

void startup_message(void) {
   clrscr();
   frame(52,4);
   gotoxy(3,2);
   STARTUP_MESSAGE1;
   gotoxy(3,3);
   STARTUP_MESSAGE2;
   gotoxy(1,6);
}

void frame(int hor,int vert) {
   int x,y;
   x=wherex();
   y=wherey();
   putch(201);
   printstring(205,hor-2);
   putch(187);
   down(186,vert-2);
   putch(188);
   gotoxy(x,y);
   down(186,vert-2);
   putch(200);
   printstring(205,hor-2);
   gotoxy(x,y+vert+1);
}   

void printstring(char c,int n) {
   int i;
   for (i=0;i<n;i++) putch(c);
}

void down(char c,int n) {
   int i;
   for (i=0;i<n;i++) {
      putch('\n');
      putch('\b');
      putch(c);
   }
   putch('\n');
   putch('\b');
}

void print_extract(void) {
   union lofl {
      long  lo;
      float fl;
   };
   union lofl missing;
   int h1=20,h2=20,h3=20, i;
   char buffer1[19], buffer2[19];
   float d1,d2;
   missing.fl=MISSING;
   clrscr();
   for (i=0;i<19;i++) buffer1[i]='\0';
   for (i=0;i<19;i++) buffer2[i]='\0';
   cprintf("Partial extract of worksheet:\r\n\r\n");
   putch(201);
   printstring(205,h1);
   putch(203);
   printstring(205,h2);
   putch(203);
   putch(205);
   cprintf(" .. ");
   putch(205);
   putch(203);
   printstring(205,h3);
   putch(187);
   putch('\n');
   putch('\r');
   for (i=0;i<19;i++) buffer1[i]='\0';
   for (i=0;i<19;i++) buffer2[i]='\0';                                               
   strncpy(buffer1,testname[0],18);
   strncpy(buffer2,testname[columns-1],18);
   cprintf("%c%-19s %c%19s %c  ..  %c%19s %c\r\n",186,"LOTUS column",186,numbertolotus(Startcolumn,"  "),186,186,numbertolotus(Startcolumn+columns-1,"  "),186);
   cprintf("%c%-19s %c%19s %c  ..  %c%19s %c\r\n",186,"testname:",  186,buffer1,186,186,buffer2,186);
   for (i=0;i<19;i++) buffer1[i]='\0';
   for (i=0;i<19;i++) buffer2[i]='\0';                               
   strncpy(buffer1,testunit[0],18);                                                    
   strncpy(buffer2,testunit[columns-1],18);
   cprintf("%c%-19s %c%19s %c  ..  %c%19s %c\r\n",186,"testunit:",  186, buffer1, 186,186,buffer2,186);
   putch(204);
   printstring(205,h1);
   putch(206);
   printstring(205,h2);
   putch(206);
   putch(205);
   cprintf(" .. ");
   putch(205);
   putch(206);
   printstring(205,h3);
   putch(185);
   putch('\n');
   putch('\r');
   d1=data[0][0];
   d2=data[0][columns-1];
   for (i=0;i<19;i++) buffer1[i]='\0';
   strncpy(buffer1,samplename[0],18);
   cprintf("%c%19s %c",186,buffer1,186);
   if (d1==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d1);
   cprintf("%c  ..  %c",186,186);
   if (d2==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d2);
   cprintf("%c\r\n",186);
   d1=data[1][0];
   d2=data[1][columns-1];                             
   for (i=0;i<19;i++) buffer1[i]='\0';
   strncpy(buffer1,samplename[1],18);
   cprintf("%c%19s %c",186,buffer1,186);
   if (d1==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d1);
   cprintf("%c  ..  %c",186,186);
   if (d2==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d2);
   cprintf("%c\r\n",186);
   printstring('.',71);
   putch('\n');
   putch('\r');
   printstring('.',71);
   putch('\n');
   putch('\r');
   d1=data[rows-2][0];
   d2=data[rows-2][columns-1];
   for (i=0;i<19;i++) buffer1[i]='\0';                
   strncpy(buffer1,samplename[rows-2],18);
   cprintf("%c%19s %c",186,buffer1,186);
   if (d1==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d1);
   cprintf("%c  ..  %c",186,186);
   if (d2==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d2);
   cprintf("%c\r\n",186);
   d1=data[(rows-1)][0];
   d2=data[rows-1][columns-1];
   for (i=0;i<19;i++) buffer1[i]='\0';                
   strncpy(buffer1,samplename[rows-1],18);
   cprintf("%c%19s %c",186,buffer1,186);
   if (d1==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d1);
   cprintf("%c  ..  %c",186,186);
   if (d2==MISSING) cprintf("%19s ","-----");
   else cprintf("%19.3f ", d2);
   cprintf("%c\r\n",186);
   for (i=0;i<19;i++) buffer1[i]='\0';
   putch(200);
   printstring(205,h1);
   putch(202);
   printstring(205,h2);
   putch(202);
   putch(205);
   cprintf(" .. ");
   putch(205);
   putch(202);
   printstring(205,h3);
   putch(188);
   putch('\n');
   putch('\r');
}

void no_room(void){
   E7;
   farewell(-1);
}

void read_environment(void) {
   startup_drive=getdisk();
   Trace	=(int)set_env_variable("TRACE",		0);
   Show		=(int)set_env_variable("SHOW",		1);
   Startrow	=(int)set_env_variable("STARTROW",	STARTROW);
   Max_row	=(int)set_env_variable("MAX_ROW",	MAX_ROW);
   Startcolumn	=(int)set_env_variable("STARTCOLUMN",	STARTCOLUMN);
   Max_col	=(int)set_env_variable("MAX_COL",	MAX_COL);
   Min_row	=(int)set_env_variable("ROWS",		MIN_ROW);
   Min_col	=(int)set_env_variable("MIN_COL",	MIN_COL);
   Namerow	=(int)set_env_variable("NAMEROW",	NAMEROW);
   Unitrow	=(int)set_env_variable("UNITROW",	UNITROW);
   Codecolumn   =(int)set_env_variable("CODECOLUMN", CODECOLUMN);
   Centile  = (double)set_env_variable("CENTILE", CENTILE);
   Se       = (double)set_env_variable("SE", SE);
   if (Centile>1) Centile/=100;
   if (Centile<0.05 || Centile>0.95) Centile=0;
   if (Trace) fprintf(tracefile,"%4d read_environment\r\n"
      "Trace      =%d\r\n"
      "Show       =%d\r\n"
      "Startrow   =%d\r\n"
      "Min_row    =%d\r\n"
      "Startcolumn=%d\r\n"
      "Min_col    =%d\r\n"
      "Centile    =%5.2lf\r\n"
      "Se         =%5.2lf\r\n",
    __LINE__, Trace, Show, Startrow, Min_row, Startcolumn, Min_col, Centile, Se);
}

/********/

float set_env_variable(char *env_string, float default_value) {
   char *s;
   float result;
   s=getenv(env_string);
   if (!s)				result= default_value;
   else {
      if (!stricmp(s,"AUTO"))		result= MISSING;
      if (!stricmp(s,"OFF"))		result= 0;
      if (!stricmp(s,"FALSE"))		result= 0;
      if (!stricmp(s,"ON"))		result= 1;
      if (!stricmp(s,"TRUE"))		result= 1;
      else result= atof(s);
   }
   if (Trace) fprintf(tracefile,"%4d set_env_variable %12s \"%s\" default=%7.2f result=%7.2f\r\n",
      __LINE__, env_string, s, default_value, result);
   return result;
}

/********/

void write_pointers(void) {
      fprintf(tracefile,"\r\n readwks %s %4d\r\n"
      "data-blocks:\r\n"
      "data      =%p\r\n"
      "rank      =%p\r\n"
      "valid_data=%p\r\n"
      "i25       =%p\r\n"
      "i50       =%p\r\n"
      "i75                            =%p\r\n"
      "temprank[MAX_ROW]              =%p\r\n"
      "tempdata[MAX_ROW]              =%p\r\n"
      "testname[MAX_COL+1][MAX_LEN+1] =%p\r\n"
      "testunit[MAX_COL+1][MAX_LEN+1] =%p\r\n"
      "samplename[MAX_ROW+1][ID_LEN+1]=%p\r\n"
      "header1[MAX_LEN+1]             =%p\r\n"
      "header2[MAX_LEN+1]             =%p\r\n"
      " coreleft =%8lX = %ld\r\n",
      __DATE__,__LINE__,data,rank, valid_data,i25,i50,i75,
      temprank, tempdata, testname[0], testunit[0], samplename[0], header1, header2,
      coreleft(), coreleft());
}

void clear_array(void) {
register row,column;
/*
   int *ip;
   ip=(int*) datapointer;
*/
   clrscr();
   if (Trace) fprintf(tracefile,"\r\n%4d clear_array",__LINE__);
   for (row=0;row<MAX_ROW;row++) {
      data[row]=datapointer + row * MAX_COL;
      rank[row]=rankpointer + row * MAX_COL;
   }
   clear_hugearray();
/*
   setmem(ip,0XFFFF,0XCC);
   (unsigned)((unsigned)MAX_ROW*(unsigned)MAX_COL*sizeof(float)), 0XCC);
   setmem(ip+(unsigned)0XFFFF+1,(unsigned)((long)(MAX_ROW*(long)MAX_COL)-(long)0XFFFF)*sizeof(float), 0XCC);
   for (row=0;row<Max_row;row++) {
      putch('.');
      for (column=0;column<Max_col;column++) data[row][ column] = MISSING;
   }
*/
   for (column=0;column< Max_col;column++) clear_string( testname[column]);
   putch('.');
   for (column=0;column< Max_col;column++) clear_string( testunit[column]);
   putch('.');
   for (row   =0;row   < Max_row;row++   ) clear_string( samplename[row]);
   putch('.');
   clear_string(header1);
   putch('.');
   clear_string(header2);
   rows=MIN_ROW;
   columns=MIN_COL;
}

void clear_hugearray(void) {
   unsigned size;
   unsigned long start, jobsize;
   char huge *cp;
   size=0XFFFF;
   jobsize=(long)MAX_ROW * (long)MAX_COL * (long)sizeof(float);
   for (start=0, cp=(char*) datapointer; start+size<jobsize; start+=size, cp+=size) {
      setmem(cp,size,0XCC);
   }
   if (jobsize-start>0) setmem(cp, (unsigned)(jobsize-start), 0XCC);
}

/********/

int getfilename(int argc,char **argv) {
   int result=-1;
   if (argc==1) {
       getfile(filename, "A:*.wk1");
       while (filename==NULL) {
             clrscr();
             M0;
             if (getch()!=CR) farewell(-1);
             clrscr();
	     getfile(filename, "A:*.wk1");
       }
   }
   else strncpy(filename,argv[1],LENFILENAME);
   if ((fpin=fopen(filename,"rb")) == NULL) result=0;
   else result=1;
   fclose(fpin);
   if (Trace) fprintf(tracefile,"\r\n%4d filename:%s",__LINE__,filename);
   return result;
}

void read_wks(void) {
enum opcode {
   BOF=0,Eof,CALCMODE,CALCORDER,SPLIT,
   SYNC=5,DIMENSIONS,WINDOW1,COLW1,WINDOW2,
   COLW2=10,NRANGE,BLANK,INTEGER,NUMBER,
   LABELCODE=15,FORMULA,
   TABLE=24,QRANGE,PRANGE,SRANGE,FRANGE,KRANGE1,
   DRANGE=32,KRANGE2=35,PROTECT,FOOTER,HEADER,SETUP,MARGINS,
   LABELFMT=41,TITLES,GRAPH=45,NGRAPH,CALCOUNT,FORMAT,CURSORW12,
   STRING=51,SNRANGE=71,WKSPASS=75,
   HIDCOL1=100,HIDCOL2,PARSE,RRANGES,MRANGES=105,CPI=150
};
   int integer, op, body_length, formula_size, file_type,
       column, row, end_col, end_row, max_len;
   long counter, filesize;
   double step;
   char nop;
   int mark;
   double lotusnumber;
   unsigned char c, pos;
/*
   unsigned char buffer[439];  /* to read graph data */
*/
   char stringbuffer[MAX_LEN+1];
   if (Trace) fprintf(tracefile,"\r\n%4d reading %s; startrow=%d startcolumn=%d\r\n",
      __LINE__,filename,Startrow,Startcolumn);
   fpin=fopen(filename,"rb");
   filesize=filelength(fileno(fpin));
   clrscr();
   make_ruler(23,1,10,5);
   cprintf("\r\n%12s %6ld : ", shortfilename, filesize);
   step = filesize/50.0;
   counter=0;
   mark=0;
   op=getw(fpin);
   while (op!=Eof){
      while ( (mark * step) < counter && mark < 50) {
         putch(220);
         mark++;
      }
      *stringbuffer='\0';
      body_length=getw(fpin);
      if (body_length < 1) {
         E1;
         cprintf("\r\n%4d %s: body_length=%d",__LINE__,__FILE__, body_length);
         if (getch()==27) farewell(-1);
         putch('\n');
      }
      counter += body_length+4;
      switch(op){
           case BOF:
           if (Trace) fprintf(tracefile,"bytecount %5ld: BOF\r\n",counter-body_length-4);
           file_type=getw(fpin);
               if (file_type!=LOTUS1 && file_type!=LOTUS2){
                   E2;
                   if (getch()==27) farewell(-1);
                   putch('\n');
               }
               break;
           case DIMENSIONS:
               getw(fpin);			/* start_col */
               getw(fpin);			/* start_row */
               end_col=getw(fpin);
               end_row=getw(fpin);
               rows = (end_row > Max_row) ?
                  Max_row :
                  end_row;
               columns = (end_col > Max_col) ?
                  Max_col :
                  end_col;
               if (Trace) fprintf(tracefile,"bytecount %5ld: DIMENSIONS; end_row=%d end_col=%d\r\n",counter-body_length-4, end_row, end_col);
               break;
           case INTEGER:
               nop=getc(fpin);      /* format */
               column=getw(fpin);
               row=getw(fpin);
               integer=getw(fpin);
               sprintf(stringbuffer,"%d",integer);
               islabelfield(row,column,stringbuffer);
/*
               if (Trace) fprintf(tracefile,"bytecount %5ld: INTEGER; row=%d column=%d value=%d\r\n",counter-body_length-4, row, column, integer);
*/
               if (islimits(row,column)) data[row-Startrow][column-Startcolumn] = (float) integer;
               break;
           case NUMBER:
               nop=getc(fpin);      /* format */
               column=getw(fpin);
               row=getw(fpin);
               fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               if (lotusnumber<MISSING || lotusnumber>MAXVALUE) lotusnumber=MISSING;
               if (lotusnumber==MISSING) clear_string(stringbuffer);
               else sprintf(stringbuffer,"%9.3lf",lotusnumber);
               islabelfield(row,column,stringbuffer);
               if (islimits(row,column)) data[row-Startrow][column-Startcolumn] = (float) lotusnumber;
/*
               if (Trace) fprintf(tracefile,"bytecount %5ld: NUMBER; row=%d column=%d value=%9.3lf\r\n",counter-body_length-4, row, column, lotusnumber);
*/
               break;
	   case FORMULA:
               nop=getc(fpin);      /* format */
               column=getw(fpin);
               row=getw(fpin);
               fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               if (lotusnumber<MISSING || lotusnumber>MAXVALUE) lotusnumber=MISSING;
               if (lotusnumber==MISSING) clear_string(stringbuffer);
               else sprintf(stringbuffer,"%9.3lf",lotusnumber);
               islabelfield(row,column,stringbuffer);
               if (islimits(row,column)) data[row-Startrow][column-Startcolumn] = (float) lotusnumber;
               formula_size=getw(fpin);
               skip(fpin,formula_size);
               if (Trace) fprintf(tracefile,"bytecount %5ld: FORMULA; row=%d column=%d value=%9.3lf\r\n",counter-body_length-4, row, column, lotusnumber);
               break;
           case BLANK:
               nop=getc(fpin);      /* format */
               column=getw(fpin);
               row=getw(fpin);
               clear_string(stringbuffer);
               islabelfield(row,column,stringbuffer);
               if (islimits(row,column)) data[row-Startrow][column-Startcolumn] = MISSING;
/*
               if (Trace) fprintf(tracefile,"bytecount %5ld: BLANK; row=%d column=%d\r\n",counter-body_length-4, row, column);
*/
               break;
           case LABELCODE:
               pos=0;
               stringbuffer[0]='\0';
               nop=getc(fpin);      /* format */
               column=getw(fpin);
               row=getw(fpin);
               max_len= (islabelfield(row, column, " ")  == SAMPLENAME) ? ID_LEN:MAX_LEN;
               if (islimits(row,column)) data[row-Startrow][column-Startcolumn] = (float) MISSING;
               while ((c=getc(fpin))!=0) {
                 if (pos<max_len-1) {
                    if (pos>0) stringbuffer[pos-1] = c;
                    pos++;
                 }
               }
               stringbuffer[pos-1]='\0';
               if (*stringbuffer=='\0') clear_string(stringbuffer);
               islabelfield(row,column,stringbuffer);
               if (Trace) fprintf(tracefile,"bytecount %5ld: LABEL; row=%d column=%d \"%s\"\r\n",counter-body_length-4, row, column, stringbuffer);
               break;
/*
	   case CPI:
	       skip(fpin,body_length);
	       break;
	   case PRANGE:
	   case SRANGE:
	   case FRANGE:
	   if (Trace) fprintf(tracefile,"bytecount %5ld: PSF-RANGE\r\n",counter-body_length-4);
	       getw(fpin);		/* start_col */
	       getw(fpin);		/* start_row */
	       end_col=getw(fpin);
	       end_row=getw(fpin);
		   break;
	   case KRANGE1:
	   case KRANGE2:
	   if (Trace) fprintf(tracefile,"bytecount %5ld: KRANGE\r\n",counter-body_length-4);
	       getw(fpin);		/* start_col */
	       getw(fpin);		/* start_row */
	       end_col=getw(fpin);
	       end_row=getw(fpin);
	       nop=getc(fpin);      /* order */
	       break;
	   case NGRAPH:
	   if (Trace) fprintf(tracefile,"bytecount %5ld: NGRAPH\r\n",counter-body_length-4);
	       for (pos=0;pos<16;pos++) nop=getc(fpin);
	   case GRAPH:
	   if (Trace) fprintf(tracefile,"bytecount %5ld: GRAPH\r\n",counter-body_length-4);
	       fread(buffer,1,439,fpin);
	       break;
*/
	   default:
	   if (Trace) fprintf(tracefile,"bytecount %5ld: other code: %3d\r\n",counter-body_length-4, op);
	       skip(fpin,body_length);
	 }
      op=getw(fpin);
   }
   fclose(fpin);
   while (mark++<50) putch(220);
   putch('\n');
   if (Trace) {
      for (row=0;row<rows;row++) {
	 fprintf(tracefile,"\r\nrow %3d:", row);
	 for (column=0; column<columns; column++)
	    fprintf(tracefile,"%12.2f",data[row][column]);
      }
   }
}

int islimits(int row, int column) {
   if (
	 row >= Startrow
      &&
	 row < Startrow + Max_row
      &&
	 column >= Startcolumn
      &&
	 column < Startcolumn + Max_col
   )
   return 1;
   else return 0;
}

int islabelfield( int row, int column, char *stringbuffer) {
   int result=NOLABEL;
   if (row==Namerow && column==Codecolumn) {
      strncpy(header1, stringbuffer,MAX_LEN);
      result=HEADER1;
   }
   if (row==Unitrow && column==Codecolumn) {
      strncpy(header2, stringbuffer,MAX_LEN);
      result=HEADER2;
   }
   if (row==Namerow && column>=Startcolumn && column<Startcolumn + Max_col) {
      strncpy(testname[column - Startcolumn],stringbuffer,MAX_LEN);
      result=TESTNAME;
   }
   if (row==Unitrow && column>=Startcolumn && column<Startcolumn + Max_col) {
      strncpy(testunit[column - Startcolumn],stringbuffer,MAX_LEN);
      result=TESTUNIT;
   }
   if (column==Codecolumn && row>=Startrow && row<Startrow + Max_row ) {
      strncpy(samplename[row  - Startrow],   stringbuffer,ID_LEN);
      result=SAMPLENAME;
   }
   return result;
}

void skip(FILE *fp,int noofbytes) {
   char nop;
   while (noofbytes>0) {
       nop=getc(fp);
       noofbytes--;
   }
}

/********/

void openout(char *f) {
   if ((fpout=fopen(f,"wb")) == NULL) {
       E3;
       farewell(-1);
   }
}

/********/

void dump_block(unsigned char *pointer, int noofbytes) {
int n, nn;
unsigned char c;
	for (n=-32; n<noofbytes; n+=16) {
		fprintf(tracefile,"%4d ",n);
		for (nn=0;nn<16;nn++){
			c=*(pointer+n+nn);
			fprintf(tracefile,"%2X ",c%256);
		}
		for (nn=0;nn<16;nn++){
			c=*(pointer+n+nn);
			putc((c>31 && c<127)?c:'.', tracefile);
		}
	putc('\n', tracefile);
	}
}

float minimum_value(int column) {
   int row;
   float d,m=MAXVALUE;
   for (row=0;row<rows;row++) {
      d=data[row][ column];
      if ( d>MISSING && m>d ) m=d;
   }
   return (m);
}

/********/

float maximum_value(int column) {
   int row;
   float d,m=MINVALUE;
   for (row=0;row<rows;row++) {
      d=data[row][ column];
      if ( d>MISSING && m<d ) m=d;
   }
   return (m);
}

/********/

void dump_info(char *f) {
   if (Trace) fprintf(tracefile, "%4u dump_info\r\n",__LINE__);
   openout(f);
   fprintf(fpout,"\"%s\"\r\n\"%s\"\r\n\"%s\"\r\n",filename,header1,header2);
   fprintf(fpout,"%d\r\n%d\r\n",rows,columns);
   fprintf(fpout,"%d\r\n",PROTOCOL_CODE);
   fprintf(fpout,"centile-option: %5.2lf\r\n",Centile);
   fprintf(fpout,"start-row: %d\r\n",Startrow);
   fprintf(fpout,"start-column: %d\r\n",Startcolumn);
   fclose(fpout);
}

/********/

void dump_data(char *f) {
int row,column;
float value;
   openout(f);
   for (row=0;row<rows;row++) {
      if (Trace) fprintf(tracefile,"\r\n%3d",row);
      for (column=0; column<columns; column++) {
         value= data[row][ column];
/*EJN fwrite kan niet kleiner dan 0*/
	 if (fwrite(&value,sizeof(float),1,fpout)==0) {
	    E4;
	    farewell(-1);
	 }
	 if (Trace) {
	    if (value>MISSING) fprintf(tracefile," %3.0f",value);
	    else fprintf(tracefile," ...",value);
	 }
      }
   }
   fclose(fpout);
   if (Trace) fprintf(tracefile, "\r\n%4u dump_data",__LINE__);
}

/********/

int count_testnames(void) {
   int column;
   for (column=0; isname(testname[column]) || column<Min_col; column++) {
      if (Trace) fprintf(tracefile,"\r\n%4d column %2d testname:%s",__LINE__,column,testname[column]);
   }
   if (Trace) fprintf(tracefile, "\r\n%4u count_testnames: column=%d Min_col=%d",__LINE__,column, Min_col);
   if (column<Min_col || column<1) column=Min_col;
/*
   if (column<columns-Startcolumn+1) {
      clrscr();
      cprintf("WARNING: only %d data columns (with column name) used\r\n"
	      "Apart from %d non-data-columns the worksheet contains %d data columns\r\n\r\n"
	      "Press any key to continue ", column, Startcolumn, columns-Startcolumn+1);
      if (getch()==ESC) farewell(-1);
   }
*/
   return column;
}

/********/

void dump_testunits(char *f) {
   int column;
   openout(f);
   for (column=0; strlen(testname[column]) && column<Max_col; column++) {
      if (fprintf(fpout,"%s\r\n",testunit[column])==-1) {
         E4;
         farewell(-1);
      }
      if (Trace) fprintf(tracefile,"\r\ncolumn %2d:\"%s\"",column,testunit[column]);
   }
   fclose(fpout);
   if (Trace) fprintf(tracefile, "\r\n%4u dump_test_units\r\n",__LINE__);
}

/********/

void dump_minmax(char *f) {
   int column;
   openout(f);
   for (column=0;column<columns;column++) {
      if (fprintf(fpout,"%12.4f %12.4f\r\n",
	    minimum_value(column),
            maximum_value(column))==-1
         ) {
         E4;
         farewell(-1);
      }
   }
   fclose(fpout);
   if (Trace) fprintf(tracefile, "%4u dump_minmax\r\n",__LINE__);
}

void dump_ile(char *f) {
   int column;
   char buffer[MAX_LEN+3];
   openout(f);
   if (fprintf(fpout,"\"%s\"\r\n\"%s\"\r\n\"%s\"\r\n",header1,header2,testunit[0])==-1) {
      E4;
      farewell(-1);
   }
   for (column=0;column<columns;column++) {
      if (fprintf(fpout,"%19s %3d %12.3f %12.3f %12.3f\r\n",
            quoted(buffer, testname[column]),
            valid_data[column],
            i25[column],
            i50[column],
            i75[column])==-1) {
         E4;
         farewell(-1);
      }
   }
   fclose(fpout);
   if (Trace) fprintf(tracefile, "%4u dump_ile\r\n",__LINE__);
}

char *quoted(char *buffer, char *s) {
   sprintf(buffer,"\"%s\"",s);
   return buffer;
}

/********/

int dump_sample_ids(char *f) {
   int row, r, n;
   n=strlen(codestring);
   openout(f);
   for (row=r=0; isname(samplename[row]) || row < Min_row; row++) {
         if (samplename[row][0]==' ' && samplename[row][1]=='\0') sprintf(samplename[row],"%c", codestring[row%n]);
         else r++;
         if (fprintf(fpout,"%s\r\n",samplename[row])==-1) {
         E4;
         farewell(-1);
      }
      if (Trace) fprintf(tracefile,"\r\nrow %3d:r=%d \"%s\"",row,r,samplename[row]);
   }
   fclose(fpout);
   if (!r) {
      cprintf("\r\nWARNING: no sample codes found in column A of worksheet\r\nCheck number of rows!\r\n\r\nPress any key to continue ");
      if (getch()==27) exit(-1);
   }
   if (row<Min_row || row<1) row=Min_row;
   if (Trace) fprintf(tracefile, "\r\n%4u dump_sample_ids row=%d r=%d ",__LINE__,row, r);
   return row;
} /* ^^^ dump_sample_ids ^^^ */

/********/

void dump_ranks(char *f) {
int row,column;
int i;
   openout(f);
   for (row=0;row<rows;row++) {
      for (column=0;column<columns;column++) {
         i = rank[row][ column];
         if (putw(i,fpout)==EOF && ferror(fpout)) {
            E4;
            farewell(-1);
         }
      }
   }
   fclose(fpout);
   if (Trace) fprintf(tracefile, "%4u dump_ranks\r\n",__LINE__);
} /* ^^^ dump_ranks ^^^ */

/********/

void rank_data(double centile) {
   int row, column;
   float percentile;
   if (Trace) fprintf(tracefile, "\r\n%4u rank_data; centile=%ld",__LINE__,centile);
   cprintf("\r\nranking test ");
   for (column=0;column<columns;column++) {
      valid_data[column]=0;
      putch(column+'A');
      for (row=0;row<rows;row++) rank[row][ column]=-1;
      qs(column, rows);
      for (row=0; row<rows; row++) if (data[row][column] >MISSING) valid_data[column]++;
   }
   CLS;
   if (Show) {
      if (Centile) cprintf("%.1lf centile +/- SE\r\n     N    ile-SE   %.1lf-ile    ile+SE\r\n\r\n",Centile*100, Centile*100);
      else         cprintf("     N    25-ile    median     75-ile\r\n\r\n");
   }
   for (column=0; column < columns ; column++) {
      i25[column] = get_ile( column, ilevalue(valid_data[column], 0.25, centile));
      i50[column] = get_ile( column, ilevalue(valid_data[column], 0.50, centile));
      i75[column] = get_ile( column, ilevalue(valid_data[column], 0.75, centile));
      if (Show) {
         cprintf("%2s %3d",numbertolotus(Startcolumn + column,"  "),valid_data[column]);
         if ((percentile = i25[column]) == MISSING) cprintf("%10s","..."); else cprintf("%10.2f",percentile);
         if ((percentile = i50[column]) == MISSING) cprintf("%10s","..."); else cprintf("%10.2f",percentile);
         if ((percentile = i75[column]) == MISSING) cprintf("%10s","...\r\n"); else cprintf("%10.2f\r\n",percentile);
         if (column%16==15 && columns-column>5) {
            MORE;
            if (Centile) cprintf("     N    ile-SE   %2.0-ile    ile+SE\r\n\r\n",Centile);
            else cprintf("     N    25-ile    median     75-ile\r\n\r\n");
         }
      }
   }
   if (Trace) fprintf(tracefile, "%4u rank_data\r\n",__LINE__);
} /* ^^^ rank_data ^^^ */

void qs(int column, int rows) {
   int row;
   float d;
   for (row=0;row<rows; row++) {
      temprank[row]=row;
      d=data[row][column];
      tempdata[row]=(d>MISSING)?d:-MISSING;
   }
   qs0(0,rows-1);
   for (row=0;row<rows; row++) {
      rank[row][column]=temprank[row];
      if (Trace) fprintf(tracefile,"\r\n%4d column=%2d n=%3d row=%3d rank=%3d data=%12.2f",
         __LINE__, column, valid_data[row], row, temprank[row], data[temprank[row]][ column]);
   }
}

   
void qs0(int left, int right) {
   register int i, j;
   int r;
   float x,y;
   i=left;   j=right;
   x=tempdata[(left+right)/2];
   do {
      while (tempdata[i] <  x  && i < right) i++;
      while (x < tempdata[j]   && j > left ) j--;
      if (i<=j) {
         y= tempdata[i];               r=temprank[i];
         tempdata[i] = tempdata[j];    temprank[i]=temprank[j];
         tempdata[j] = y;              temprank[j]=r;
         i++;j--;
      }
   } while (i<=j);
   if (left < j) qs0(left,j);
   if (i < right) qs0(i,right);
}


/********/

/*
int get_row(int column,int score) {
   int row, result=-1;
   for (row=0;row<rows;row++) {
      if (rank[row][ column]==score) {
         result=row;
         break;
      }
   }
   if (Trace) fprintf(tracefile, "%4u getrow: %3d\r\n",__LINE__,result);
   return result;
}
*/

float get_ile(int column, double ile) {
   float value, low_value, high_value;
   double factor;
   int n, score;
   n= valid_data[column];
   if (n==0) return MISSING;
   if (n<3 || ile<0.01 || ile>0.99) ile=(Centile)?Centile:0.5;
   score=(int)((n+1)*ile);
   factor=(n+1)*ile - (double)score;
   if (!score) value= data[rank[0][column]][column];
   else {
      low_value=(score-1<n)
         ? data[rank[score-1][column]][column]
         : data[rank[n-1][column]][column];
      high_value= (score<n)
         ? data[rank[score][column]][column]
         : data[rank[n-1][column]][column];
      value= low_value + (factor * (high_value - low_value));
   }
   return value;
}

/*
float get_ile(int column,double ile) {
   float low_value, high_value, result;
   double factor;
   int n,score;
   n=valid_data[column];
   if (n==0) return MISSING;
   if (n<5 || ile>0.99) ile=(Centile)?Centile:0.5;
   score=(int)((n+1)*ile);
   factor=(n+1)*ile - (double)score;
   low_value   = data[rank[score-1][column]][column];
   high_value  = data[rank[score  ][column]][column];
   result = low_value + (factor * (high_value - low_value));
   if (Trace) fprintf(tracefile,"%4u get_ile column=%u ile=%4.2lf factor=%7.3lf (score-1)=%d low=%8.2f score=%d high=%8.2f -> %8.2f\r\n",
      __LINE__,column, ile, factor, score-1, low_value, score, high_value, result);
   return result;
}
*/
int check_wks(void) {
   int c;
   int accepted=0, result=1;;
   print_extract();
   do {
         window(1,15,80,25);
         clrscr();
         cprintf("1:  Testnames    from row    %d",Namerow+1);
         cprintf("\r\n2:  Testunits    from row    %d",Unitrow+1);
         cprintf("\r\n3:  Data         from row    %d .. %d (at least %d .. %d, max %d .. %d)",
	    Startrow+1,
            Startrow + rows,
            Startrow+1,
            Startrow+Min_row,
            Startrow+1,
            Startrow+Max_row);
         cprintf("\r\n4:  Sample codes from column %s", numbertolotus(Codecolumn,"  "));
         cprintf("\r\n5:  Data         from column %s .. %s (at least %s .. %s, max %s .. %s)",
            numbertolotus(Startcolumn, "  "),
            numbertolotus(Startcolumn + columns-1, "  "),
            numbertolotus(Startcolumn, "  "),
            numbertolotus(Startcolumn + Min_col-1, "  "),
            numbertolotus(Startcolumn, "  "),
            numbertolotus(Startcolumn + Max_col-1, "  "));
/*
         if (!Centile)cprintf("\r\n6:  Statistics: 25-, 50- 75 percentiles");
         else         cprintf("\r\n6:  Statistics: %d percentile +/- SE", (int)(Centile*100));
*/
         cprintf("\r\n\r\nPress RETURN to accept\r\nPress 1-5 to change option: ");
         do c=getkey(0); while (!(c==RETURN || c==ESC || (c>'0' && c <'7')));
         window(1,1,80,25);
      switch(c) {
         case RETURN:
            accepted = 1; 
            break;
         case ESC:
            accepted = -1;
            result=-1;
            break;
         case '1':
            window(1,22,80,25);
            clrscr();
            cprintf("In which row are the testnames (default: %d) :",Namerow+1);
            gets(buffer);
            if (*buffer) Namerow=atoi(buffer)-1;
            if (Namerow<=0 || Namerow > 255) Namerow=NAMEROW;
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
         case '2':
            window(1,22,80,25);
            clrscr();
            cprintf("In which row are the descriptions of the test-units (default: %d) :",Unitrow+1);
	    gets(buffer);
            if (*buffer) Unitrow=atoi(buffer)-1;
            if (Unitrow<0 || Unitrow > 255) Unitrow=UNITROW;
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
         case '3':
            window(1,22,80,25);
            clrscr();
            cprintf("Enter first data-row (default: %d) :",Startrow+1);
            gets(buffer);
            if (*buffer) Startrow=atoi(buffer)-1;
            if (Startrow<0 || Startrow > 255) Startrow=STARTROW;
            cprintf("Enter the minimum number of rows to read (default: %d, maximum: %d)",MIN_ROW,MAX_ROW);
            gets(buffer);
            if (*buffer) Min_row=atoi(buffer);
            if (Min_row<=0 || Min_row > MAX_ROW) Min_row=MIN_ROW;
/*
            cprintf("Enter the maximum number of rows to read (default: %d, maximum: %d) :",Max_row, MAX_ROW);
            gets(buffer);
            if (*buffer) Max_row=atoi(buffer);
            if (Max_row<=0 || Max_row > MAX_ROW) Max_row=MAX_ROW;
*/
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
         case '4':
            window(1,22,80,25);
            clrscr();
            cprintf("In which column are the sample-codes (default: %s) :",numbertolotus(Codecolumn,"  "));
            gets(buffer);
            if (*buffer) {
               if (atoi(buffer)>0) Codecolumn=atoi(buffer)-1;
               else Codecolumn=lotustonumber(buffer);
            }
            if (Codecolumn<0 || Codecolumn > 255) Codecolumn=CODECOLUMN;
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
	 case '5':
            window(1,22,80,25);
            clrscr();
            cprintf("Enter the first data-column (default: %s) :", numbertolotus(Startcolumn,"  "));
            gets(buffer);
            if (*buffer) {
               if (atoi(buffer)>0) Startcolumn=atoi(buffer)-1;
               else Startcolumn=lotustonumber(buffer);
            }
            if (Startcolumn<0 || Startcolumn > 255) Startcolumn=STARTCOLUMN;
            cprintf("Enter the minimum number of columns to read (default: %d, maximum: %d) :",MIN_COL, MAX_COL);
            gets(buffer);
            if (*buffer) Min_col=atoi(buffer);
            if (Min_col<=0 || Min_col > MAX_COL) Min_col=MIN_COL;
/*
            cprintf("Enter the maximum number of columns to read (default: %d, maximum: %d) :",Max_col, MAX_COL);
            gets(buffer);
            if (*buffer) Max_col=atoi(buffer);
            if (Max_col<=0 || Max_col > MAX_COL) Max_col=MAX_COL;
*/
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
         case '6':
            window(1,22,80,25);
            clrscr();
            cprintf("Statistics:\r\n0 = 25-, 50- and 75- percentiles\r\n1 = percentile +/- SE\r\nEnter 0 or 1: ");
            do c=getch(); while (!(c=='0' || c=='1' || c==RETURN));
            if (c=='0'||c==RETURN) Centile=0;
            else if (c=='1'){
               clrscr();
               cprintf("Enter percentile (10-90, default 50): ");
               gets(buffer);
               if (*buffer) Centile=atof(buffer);
               else Centile=0.5;
               if (Centile>10) Centile/=100;
               if (Centile<=0.1 && Centile > 0.9) Centile=CENTILE;
            }
            accepted = 0;
            result = 0;
            clrscr();
            window(1,1,80,25);
            break;
      }
   } while (!accepted);
   if (Trace) fprintf(tracefile,"\r\n%4d Startcolumn=%d Min_col=%d Max_col=%d Startrow=%d Min_row=%d Max_row=%d",
      __LINE__, Startcolumn, Min_col, Max_col, Startrow, Min_row, Max_row);
   return result;
}

int lotustonumber(char *s) {
   int result;
   if (strlen(s)==1) result=toupper(s[0])-'A';
   else result=26 * (toupper(s[0])-'A'+1) + toupper(s[1])-'A';
   if (result<0 || result >256) result =-1;
   if (Trace) fprintf(tracefile,"\r\n%4d lotustonumber %s -> %i",__LINE__, s, result);
   return result;
}

char *numbertolotus(int i, char *s) {
   if (i<26) sprintf(s, "%c",i+'A');
   else      sprintf(s, "%c%c",i/26+'A'-1,i%26+'A');
   return s;
}

int strmenu(int current_pos, int rows, int startrow,int nooflines) {
     int pos, key;
     int n, result, row, col, maxnoofitems;
     maxnoofitems=TEXTCOLUMNS*nooflines;
     if (rows>maxnoofitems) rows=maxnoofitems;
     if (rows>MAXFILES) rows=MAXFILES;
     for (n=0;n<rows;n++) startchars[n]=toupper(*(filenames[n]));
     startchars[rows]='\0';
     n = current_pos;
     do {
        row=startrow + n % nooflines;
        col=1+SCREENCOLUMNS/TEXTCOLUMNS*(n/nooflines);
        gotoxy(col, row);
        HLON;
        cprintf("%-8s",strnleft2(buffer,filenames[n],'.',8));
        HLOFF;
        key=getkey(0);
        gotoxy(col, row);
        cprintf("%-8s",strnleft2(buffer,filenames[n],'.',8));
        if (key<=SPACE || key>256) {
           switch (key) {
              case UP:
                   n--;
		   if (n<0) n=rows-1;
                 break;
              case DOWN:
              case SPACE:
                   n++;
                   if (n > rows-1) n=0;
                 break;
              case TAB:
              case RIGHT:
                   if (n<rows-nooflines) n+=nooflines;
                 break;
              case BACKTAB:
              case LEFT:
                   if (n>nooflines-1) n-=nooflines;
                 break;
              case PgUp:
                   n=(n>nooflines-1 && rows>nooflines) ?
                        nooflines:
                        0;
                 break;
              case HOME:
                   n=0;
                 break;
              case PgDn:
                   n=(n>nooflines-1 || nooflines>rows) ?
                        rows-1:
                        nooflines-1;
                   if (n>rows-1) n=rows-1;
                 break;
              case SHIFT_HOME:
              case EXEC:
                 n=rows-1;
                 break;
              case RETURN:
                 result = RETURN;
                 break;
              case ESC:
              case CANCEL:
                   n=-1;
                   break;
              case CTRL_C:
                  farewell(0);
              default:
                  break;
           }
       } else {
               pos=strpos(startchars,toupper(key),n);
               n=(pos<0) ? n : pos;
       }
     }  while (result!=RETURN && n>=0);
   return n;
}

int strpos(char *s,char c,int pos)

/* finds next occurrence of c in s starting at pos */

{
int p;
     for (p=pos+1;s[p];p++) if (s[p]==c) return p;
     for (p=0;p<=pos;p++) if (s[p]==c) return p;
     return -1;
}

char *keycopy(char *target,char *source,int maxlen) {
int t,tt,s,insert=0;
int c;
result=0;
for (t=0;t<maxlen;t++) target[t]='\0';
maxlen--;
c=0;
     for (t=0,s=0;t<maxlen && c!=RETURN;) {
          c=getkey(1);
          if (c>31 && c<127) {
               putch(c);
               target[t++]=c;
               if (insert==0 && (source[s+1])!='\0') s++;
          } else {
               switch (c) {
               case RETURN:
                    if (!target[0]) {
                       strcpy(target,source);
                       result=RETURN;
                    }
                    else target[t]='\0';
                    break;
               case EXEC:
               case ESC:
               case CANCEL:
               case BACKTAB:
		    target[0]=0;
                    c=RETURN;
                    break;
               case BACKSPACE:
                    insert=0;
                    if (t>0) {
                         cprintf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
                    break;
               case RIGHT:
                    insert=0;
                    if (t<maxlen && source[s]){
                         putch(source[s]);
                         target[t++]=source[s++];
                    }
                    break;
               case DELETE:
                    source++;
                    insert=0;
                    break;
               case INSERT:
                    insert=1;
                    break;
               case LEFT:
                    insert=0;
                    if (t>0) {
                         cprintf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
                    break;
               case F3:
                    insert=0;
                    while (t<maxlen && source[s]) {
                         putch(source[s]);
                         target[t++]=source[s++];
                    }    
                    break;
               case F5:
                    insert=0;
                    for (tt=0;tt<t;tt++) source[tt]=target[tt];
                    for (t=0;t<maxlen;t++) target[t]='\0';
                    putch('@');
		    putch('\n');
                    s=t=0;
                    break;
               case CTRL_C:
                    farewell(0);
                    break;
               }
          }
     }
     return target;
}

char *strleft(char *s1,char *s2,char *s3) {
     int i,ii;
     if (strlen(s2)==0 || strlen(s3)==0 ) {
          *s1='\0';
          return s1;
     }
     strcpy(s2,s1);
     for (i=1;i<=strlen(s3);i++) {
          for (ii=strlen(s2);ii>0;ii--) {
	       if (s3[i-1]==s2[ii-1]) {
		    s1[ii]='\0';
		    return s1;
	       }
	  }
     }
     *s1='\0';
     return s1;
}

char *getfile(char *path, char *s) {
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char ext[MAXEXT];
/*
   char path[120];
*/
   int nooffiles,selected_file;
   do {
		strcpy(path,getmask(s));
		if (!(*path)) return NULL;
	makefilename(path,path,".wk1",0);
	fnsplit(path, drive, dir, shortfilename, ext);
		fnmerge(Input_path, drive, dir, "*", ext);
	nooffiles=getdir(path);
		if (!nooffiles) {
	    clrscr();
	    cprintf("No files in directory %s\n\rtry again ... \n\r\n\r",s);
		}
	} while (!nooffiles);
	selected_file= strmenu(
	   0,
	   nooffiles,
	   SCREENSTARTROW,
	   NOOFLINES
	);
	if (selected_file==-1) return NULL;
    strcpy(path,filenames[selected_file]);
//EJN kon niet van c: ophalen
//    fnsplit(filenames[selected_file],drive,dir,shortfilename,ext);
//    if (!*drive) sprintf(drive,"%c:",data_drive+'A');
//    fnmerge(path,drive,dir,shortfilename,ext);
    fnmerge(path,drive,dir,buffer,ext);
    return path;
}
/*
char *getfile(char *s) {
    char sink[5];
    int nooffiles,selected_file;
	do {
		strcpy(s,getmask(s));
		if (!(*s)) return NULL;
		nooffiles=getdir(s,filenames);
		if (!nooffiles) {
			CLS;
			M1_getfile;
		}
	} while (!nooffiles);
	selected_file = strmenu(
	   0,
	   nooffiles,
	   SCREENSTARTROW,
	   NOOFLINES
	);
	if (selected_file==-1) return NULL;
	strcpy(s, filenames[selected_file]);
	fnsplit(s, sink, sink, shortfilename, ext);
	fnmerge(s, drive, dir, shortfilename, ext);
	if (Trace) fprintf(tracefile, "\r\n%4d : drive=%s dir=%s ext=%s shortfilename=%s fullname=%s",
	   __LINE__, drive, dir, ext, shortfilename, s);
	return s;
}
*/
char *getmask(char *mask) {
	char buffer[80];
	int flag;
    clrscr();
    startup_message();
    M2_getfile;
	keycopy(buffer,mask,80);
	strcpy(mask,buffer);
	flag = fnsplit(mask, drive, dir, shortfilename, ext);
        if (!(flag & DRIVE))      strcpy( drive, default_drive);
        if (!(flag & DIRECTORY))  strcpy( dir, default_dir);
   if (Trace) fprintf(tracefile, "\r\n%4d : mask=%s drive=%s dir=%s ext=%s shortfilename=%s",
      __LINE__, mask, drive, dir, ext, shortfilename);
	return mask;
}
/*
int getdir(char *s, char filenames[][LENFILENAME+1]) {
	int done,n,row,col,month,day,year,hour,minute,second;
	struct ffblk ffblk;
	CLS;
	M3_getfile;
	for (col=0; col<TEXTCOLUMNS; col++) {
	   gotoxy(1+col*SCREENCOLUMNS/TEXTCOLUMNS, SCREENSTARTROW-1);
           cprintf("Filename YY/MM/DD");
        }
	done=findfirst(s,&ffblk,0);
        if (done) return 0;
	for(n=0,done=0;!done && n<MAXFILES;n++) {
		row=SCREENSTARTROW + n%NOOFLINES;
                col=1+SCREENCOLUMNS/TEXTCOLUMNS*(n/NOOFLINES);
		gotoxy(col, row);
		strcpy(filenames[n],ffblk.ff_name);
		maketime(&hour,&minute,&second,ffblk.ff_ftime);
		makedate(&month,&day,&year,ffblk.ff_fdate);
        cprintf("%-8s %2.2d/%2.2d/%2.2d\r\n",strnleft2(buffer, filenames[n],'.',8), year, month, day);
                done=findnext(&ffblk);
	}
	return(n);
}
*/

int getdir(char *s) {
	int done,n,row,col,month,day,year,hour,minute,second;
	struct ffblk ffblk;
    clrscr();
    cprintf("Mask: %s. Select file and press RETURN (press Esc to exit)",s);
	for (col=0; col<TEXTCOLUMNS; col++) {
	   gotoxy(1+col*SCREENCOLUMNS/TEXTCOLUMNS, SCREENSTARTROW-1);
       cprintf("Filename YY/MM/DD");
    }
    done=findfirst(s,&ffblk,0);
    if (done) return 0;
    for(n=0,done=0;!done && n<MAXFILES;n++) {
       row=SCREENSTARTROW + n%NOOFLINES;
       col=1+SCREENCOLUMNS/TEXTCOLUMNS*(n/NOOFLINES);
       gotoxy(col, row);
       strcpy(filenames[n],ffblk.ff_name);
       maketime(&hour,&minute,&second,ffblk.ff_ftime);
       makedate(&month,&day,&year,ffblk.ff_fdate);
       cprintf("%-8s %2.2d/%2.2d/%2.2d\n\r",strnleft2(buffer, filenames[n],'.',8), year, month, day);
       done=findnext(&ffblk);
   }
   return(n);
}

void makedate(int *m,int *d,int *y,unsigned total) {
	*m=(total>>5)&0XF;
	*d=total & 0X1F;
	*y=(total>>9)&0X3F;
	*y+=80;
}

void maketime(int *h,int *m,int *s,unsigned total) {
	*h=(total>>11) & 0X1F;
	*m=(total>>5) & 0X3F;
	*s=total & 0X1F;
}

char *strnleft2(char *buffer, char *s1, char stop, int n) {
   int i=0;
   do buffer[i]=s1[i++];
   while  (i && i<n && s1[i]!=stop);
   buffer[i]='\0';
   return buffer;
}

char *clear_string(char *s) {
   *s=' ';
   *(s+1)='\0';
   return s;
}

int isname( char *s) {
   if (!*s) return 0;
   if (strlen(s)==1 && s[0]==' ') return 0;
   return 1;
}

void make_ruler(int x, int y, int units, int unitsize) {
   int i, ii;
   gotoxy(x, y); putch(213); 
   for (i=0;i<units-1;i++) {
      for (ii=0; ii<unitsize-1;ii++) putch(205);
      putch(209);
   }
   for (ii=0; ii<unitsize-1;ii++) putch(205);
   putch(184);
}

int getkey(int flag) {
char c;
   c=getch();
   if (c==SPECIAL_KEY) {
      return 256+getch();
   }
   else if (c==CTRL_C) farewell(0);
   else if (!flag) return toupper(c);
   return c;
}

double ilevalue(int n, double level, double centile) {
   double result;
   n+=1;
   if (centile>0) {
      if (level==0.25) result= (n * centile - ( Se * sqrt(n*centile*(1-centile))))/n;
      if (level==0.50) result= centile; 
      if (level==0.75) result= (1 + n * centile + ( Se * sqrt(n*centile*(1-centile))))/n;
      }
   else result = level;
   return result;
}

char *makefilename(char *fullname, char *default_path, char *default_ext, int forced_ext_flag) {
   int flag;
   char drive[MAXDRIVE];
   char default_drive[MAXDRIVE];
   char dir[MAXDIR];
   char default_dir[MAXDIR];
   char shortname[MAXFILE];
   char ext[MAXEXT];
   flag= fnsplit(default_path, default_drive, default_dir, shortname, ext);
   flag= fnsplit(fullname, drive, dir, shortname, ext);
   if (!(flag & DRIVE))      strcpy( drive, default_drive);
   if (!(flag & DIRECTORY))  strcpy( dir, default_dir);
   if (!(flag & EXTENSION) || forced_ext_flag)  strcpy( ext, default_ext);
   fnmerge(fullname,drive,dir,shortname,ext);
   fnmerge(default_path,drive,dir,"*",ext);
   return fullname;
}



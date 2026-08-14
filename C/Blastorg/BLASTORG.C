/* BLAST: print blokkerende antistoffen-uitslag+bar-graph vanuit LOTUS naar Kyocera */

#define TRUE    1
#define FALSE 	0
#define SHOW    TRUE
#define KYO     TRUE
#define LOTUS1 0X0404
#define LOTUS2 0X0406
#define BARSIZE 100
#define LOGFLAG TRUE
#define FFLAG TRUE
#define VFILL1 20
#define VFILL2 8
#define XFILL1 80
#define XFILL2 40
#define RATIO_LIMIET 2.5
#define FIRSTTEXTROW 5
#define MAXTEXTROWS 15
#define LASTTEXTROW (FIRSTTEXTROW + MAXTEXTROWS-1)
#define FIRSTDATAROW 21
#define MAXDATAROWS 20
#define LASTDATAROW (FIRSTDATAROW + MAXDATAROWS-1)
#define FIRSTDATACOLUMN 2
#define MAXDATACOLUMNS 4
#define LASTDATACOLUMN (FIRSTDATAROW + MAXDATAROWS-1)
#define MAXNOOFCALIBRATORS  10
#define MISSING_CALIBRATOR -999
#define LARGE 0
#define MEDIUM 1
#define SMALL  2
#define LINEAR 3
#define RIM 100

#define BOF 0
#define Eof 1
#define CALCMODE 2
#define CALCORDER 3
#define SPLIT 4
#define SYNC 5
#define DIMENSIONS 6
#define WINDOW1 7
#define COLW1 8
#define WINDOW2 9
#define COLW2 10
#define NRANGE 11
#define BLANK 12
#define INTEGER 13
#define NUMBER 14
#define LABELCODE 15
#define FORMULA 16
#define TABLE 24
#define QRANGE 25
#define PRANGE 26
#define SRANGE 27
#define FRANGE 28
#define KRANGE1 29
#define DRANGE 32
#define KRANGE2 35
#define PROTECT 36
#define FOOTER 37
/*
#define HEADER 38
*/
#define SETUP 39
#define MARGINS 40
#define LABELFMT 41
#define TITLES 42
#define GRAPH 45
#define NGRAPH 46
#define CALCOUNT 47
#define FORMAT 48
#define CURSORW12 49
#define STRING 51
#define SNRANGE 71
#define WKSPASS 75
#define HIDCOL1 100
#define HIDCOL2 101
#define PARSE 102
#define RRANGES 103
#define MRANGES 105
#define CPI 150

#define HEADER FALSE
#define LOTUS FALSE
#define PAPERLENGTH     3300
#define BOTTOM_OF_PAGE  120
#define KYO_X           24 /* KYO CORRECTIE */
/*
#define KYO_Y           -96 /* KYO CORRECTIE */
*/
#define KYO_Y           -106 /* KYO CORRECTIE */
#define LETTERSHIFT     -10
#define X_FACTOR        0.54
#define Y_FACTOR        0.5
#define X_ORIGIN        300
#define X_LENGTH        3200
#define Y_ORIGIN        0
#define Y_ORIGIN1       1300
#define Y_ORIGIN2       2800
#define Y_LENGTH        2400
#define POS0 100
#define POS1 20
#define POS2 -140
#define POS3 -30

#include <stdio.h>
#include <bios.h>
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

#define OFF ' '
#define ON  '#'
#define GROUP 8
#define LENFILENAME 80
#define MAXNOOFITEMS 320
#define NOOFLINES 20
#define MAXFILES MAXNOOFITEMS
#define MAX_LEN 40
#define MORE gotoxy(70,25);highvideo();printf(" MORE --> ");normvideo();getch();clrscr()
#define MISSING -999999.0
/*
#define HLON   textcolor(YELLOW); textbackground(RED)
#define HLON1  textcolor(BLUE);   textbackground(LIGHTRED)
#define HLON2  textcolor(YELLOW); textbackground(GREEN)
#define HLON3  textcolor(YELLOW); textbackground(BLUE)
#define HLON4  textcolor(GREEN);  textbackground(BLACK)
#define HLON5  textcolor(YELLOW); textbackground(BROWN)
#define HLOFF  textcolor(LIGHTGRAY);  textbackground(BLACK)
#define HLON  highvideo()
#define HLOFF normvideo()
*/
#define HLON  printf("\033[7m")
#define HLOFF printf("\033[0m")
#define CURSOR(a,b) printf("\033[%d;%dH",a,b)

/* pic.h: header for pic-files 280288 */
#define PAPERWIDTH      2240
#define LINE_HEIGHT 20
#define KYOBOTTOM	1000
#define PEN             5   /* pen diameter in dots */
#define FILLPEN         1   /* pen diameter in dots */
#define FONT1 10 /* 14-point bold proportional */
#define FONT6 15 /* 9-point */
#define MOVE 160
#define DRAW 162
#define FILL 48
#define FILLO 208
#define COLOR 1		/* number of colors */
#define COLOR0 176
#define COLOR15 191
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

#define W_SPECIAL_KEY 31
#define W_UP 448
#define W_DOWN 450
#define W_HOME 452
#define W_ENDKEY 455
#define W_LEFT 451
#define W_RIGHT 449
#define W_PgUp 456
#define W_PgDn 457
#define W_EXEC 453
#define W_CANCEL 480	/* = annuleer = laat vervallen = cancel */
#define W_BACKTAB 461
#define W_INSERT 454
#define W_DELETE 455
#define W_F1 384
#define W_F2 385
#define W_F3 386
#define W_F4 387
#define W_F5 388
#define W_F6 389
#define W_F7 390
#define W_F8 391
#define W_F9 392
#define W_F10 393
#define W_HELP 481
#define MAXLEN 80

#define S1 printf("Programma \"BLAST.EXE\" dd %s",__DATE__)
#define S2 printf("Inlichtingen en fouten-melding: Rob Aalberse")
#define S3 printf("Reading A:*.wks directory ...");
#define M0 printf("\nNo *.wks-file found\n\nInsert another disc and press RETURN to re-try\n")
#define M14  printf("%c%12s",flag[n],filename[n])
#define M14a printf("%c",flag[n])
#define M0_STRMENU printf("%-13s",array[n])
#define E0 printf("\ncannot open input file %s; press RETURN to re-try\n",filename[0])
#define E1 printf("\nNo wks-files found; press RETURN to re-try\n");
#define E2 printf("\nNon-printable character \" %c \" (ASCII %d): wrong input-file \"%s\"?\nprogramm aborted\n",c,c,filename[0])
#define E3 printf("\nError writing file to disc: Disc full?")
#define M0_CHECK_OUTPUT_FILE printf("\n\noutput-file: \"%s\" \n",s)
#define E1_CHECK_OUTPUT_FILE printf("\n\noutput file %s allready exists\nenter new name or press RETURN to overwrite\noutput-wks-file :",s)
#define E2_CHECK_OUTPUT_FILE printf("\nerror %d: cannot open \"%s\" for output\nenter new name or press ESC to quit\noutput-wks-file :",ferror(fpout),s)

void read_wks(char *filename);
void startup_message(void);
int initialise_printer(void);
void get_charsize(void);
void kyostart(void);
void kyoend(void);
void extra_text_menu(void);
void farewell(void);
void read_environment(void);
float set_env_variable(char env_string[], float default_value);
void printstring(char c,int n);
void make_ruler(int x, int y, int units, int unitsize);
void frame(int hor,int vert);
void down(char c,int n);
int get_inputfiles(int argc, char *argv1);
void makedate(int *m,int *d,int *y,unsigned total);
void maketime(int *h,int *m,int *s,unsigned total);
void get_outputfile(char *filename, int argc, char * argv2);
int select_files(void);
int check_output_file(char *filename);
int strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines);
void write_screen(int startrow, int rows, int nooflines, int new) ;
char *strleft(char *buffer, char *s, int n);
int strpos(char *s,char c,int pos);
char *keycopy(char *target,char *source,int maxlen);
int getkey(int flag);
char *make_kyofile_name(char *kyofile, char *wksfile);
int getfile(char *path, char filename[][]);
void error(int n, long count);
void translate_wksfile(char *wksfile);
int dots(char c);
int isvoid(char *s);
void skip(FILE *fp,int noofbytes);
char *clear_string(char *s);
void assign_number(int row, int column, float value, int formattype);
void clear_arrays(void);
void print_letter(void);
void kyomove(int x, int y);
void kyodraw(int x, int y);
void kyobar(int filltype, int x0, int y0, int x1, int y1);
void kyo62tabel(int tabelcolumn, int tabelrow, int xtab1, int xtab2, int xtab3, int xtab4, int xtab5, int xtab6, int ytab1, int ytab2);
void kyofill(int filltype, int x0, int y0, int lx, int ly);
void kyotext(int direction,int position, int x, int y, char *text);
int dots_per_text(char *text);
int  ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
void kyoy_axis_title(char *y_axis_string, int y_pos);
int  kyoy_cal(int logflag);
double scientific2(double d, double *base, int *exp);
int round(double d);
double val(double mantissa, int exponent);
double pow10d(double exponent);
char *datestring(char *buffer, float date);
float datenumber(int jaar, int maand, int dag);
int yearnumber(float datenumber);
void yesno(int flag, char *yesstring, char *nostring);
int isdate(int formatbyte);
char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char    kyofile[80];

int Logflag=LOGFLAG, Kader=1, Barsize=BARSIZE, Fixed_distance=FALSE, Fflag=FFLAG;
char y_legend[MAX_LEN+1]="Antistof-eenheden (U/ml)";
int tabelfont=1;
int tabelcolumn=200;
int tabelrow=2700;
int font[][4]={
      0,17,30,54,
      1,17,30,54,
      2,19,25,45,
      3,20,25,45,
      4,21,25,45,
      5,22,18,36,
      6,23,25,45,
      7,25,18,32,
      8,26,25,54,
	  9,28,25,54,
     10,29,40,64,
     11,30,35,54,
	 12,31,30,45,
     13,32,18,36,
     14,33,16,27,
     15,34,18,40,
     16,36,14,31,
      1,18,30,54,
      6,24,25,45,
      8,27,25,45,
     15,35,18,40 };
char font10[] = {
   17,20,20,46,34,52,42,16,22,22,30,69,17,20,17,19,34,34,34,34,34,34,34,34,34,34,18,18,69,69,69,34,
   65,41,42,45,44,39,36,46,45,18,33,42,35,53,44,46,40,46,43,40,36,43,38,57,38,37,36,23,30,23,64,30,
   69,34,37,33,37,34,19,37,37,16,16,33,17,54,43,43,37,37,23,33,19,43,32,46,32,32,30,30,30,30,69 };

/*
    ,  ,! ," , #, $, %, &, ', (, ), *, +, ,, -, ., /, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?,
    @, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _,
    `, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, {, |, }, ~ 
*/
int totalfiles, nooffiles, nop;
int Ycal=0;
int calibratorstring[MAXNOOFCALIBRATORS][16];
double value_of_calibrator[MAXNOOFCALIBRATORS];
int noofcalibrators=MAXNOOFCALIBRATORS;
int calbase[4][4]=   {1 , 0, 0, 0,
					  1 , 3, 0, 0,
					  1 , 2, 5, 0,
					  10,20,25,50
                     };
int exponent[MAXNOOFCALIBRATORS];
int startup_drive, fileno, Startrow, Startcolumn, rows, columns;
int x, y, result, Header, Show, Trace, Kyo, Group, Lotus;
float X_factor=X_FACTOR, Y_factor=Y_FACTOR;
int Paperlength=PAPERLENGTH, Pen=PEN, Font=FONT1, Fillpen=FILLPEN;
int Lettershift=LETTERSHIFT;
int n, count, size, Hsize=24, Vsize=32;
FILE *fpin, *fpout, *tracefile;
char filename[MAXFILES][LENFILENAME+1];
char fileinfo[MAXFILES][80];
long filesize[MAXFILES];
int file[MAXFILES];        /* files in order of disc directory */
char flag[MAXFILES];
char startchars[MAXFILES];
char volumelabel[20]="no disc name";
char shortfilename[13];
int charsize[256];
char buffer[81];
long currentfilesize;
int x_origin, x_length, y_origin, y_length;
int linefont[MAXTEXTROWS], linerow[MAXTEXTROWS], linecolumn[MAXTEXTROWS];
float xdata[MAXDATAROWS];
float ydata[MAXDATAROWS][MAXDATACOLUMNS];
float xmin, xmax, ymin=0.01, ymax=10000;
char textline[MAXTEXTROWS][MAXLEN];
char default_text[MAXTEXTROWS][MAXLEN];
char volgnr[MAXDATAROWS][10];
double xfactor, yfactor;

int main(int argc, char* argv[]) {
   int i;
   read_environment();
   if (argc>1) strcpy(filename[0],argv[1]);
   if (argc>2) strcpy(kyofile, argv[2]);
   get_charsize();
   if (argc>1) Show=0;
   if (argc>2) Kyo=0;
   clrscr();
   if (Show) startup_message();
   setdisk(0);
   while ( (nooffiles=get_inputfiles(argc,filename[0]))==0) {
      E1;
      if (getch()!=RETURN) exit(-1);
   }
   clrscr();
   if (Kyo) if (!initialise_printer()) Kyo=0;
   if (!Kyo && argc<3) get_outputfile(filename[file[0]], argc, kyofile);
   kyostart();
   clrscr();
   make_ruler(24,1,10,5);
   for (i=0;i<nooffiles;i++) {
      clear_arrays();
      if (Header && !Lotus) {
         fprintf(fpout, "FONT 16;\r\nMAP 0,%d;\r\nTEXT'Disc: %s';\r\n",
            PAPERLENGTH-BOTTOM_OF_PAGE,
            volumelabel);
         fprintf(fpout,"MAP 0,%d;\r\nTEXT'%12s %18s';\r\n",
            PAPERLENGTH - BOTTOM_OF_PAGE + 40,
            filename[file[i]],
            fileinfo[file[i]]);
      }
	  fprintf(fpout,"cmnt \"----\";\r\ncmnt FILE: \"%s\";\r\n",filename[file[i]]);
      read_wks(filename[file[i]]);
      print_letter();
      if (Lotus) fprintf(fpout,"\"PAGE;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n");
      else fprintf(fpout,"PAGE;\r\n");
   }
   kyoend();
   fclose(fpout);
   if (!Kyo) cprintf("\r\noutput-file: A:%s\r\n",kyofile);
   farewell();
   return 1;
}

void clear_arrays(void) {
   int i, j;
   x_origin=y_origin=100;
   x_length=y_length=2000;
   for (i=0; i<MAXTEXTROWS; i++) {
     linefont[i]=10;
     linerow[i]=100*i;
     linecolumn[i]=50;
     textline[i][0]='\0';
   }
   for (i=0; i<MAXDATAROWS; i++) {
     xdata[i]=MISSING;
     for (j=0; j<MAXDATACOLUMNS; j++) ydata[i][j]=MISSING;
   }
   xmin=32000.0;
   xmax=33000.0;
   ymin=0.1;
   ymax=100;
}

void print_letter(void) {
   int i, x, y, rim=RIM, datarows=0, year, yrs, textpos;
   double d, value, inter, min;
   double IgG, IgG4, ratio, ff;    /* ff=foezelfactor: (IgG in tabel)/(aanvankelijk bepaalde IgG) */
   int xtab1=250;                                                           
   int xtab2=300;
   int xtab3=350;
   int xtab4=300;
   int xtab5=350;
   int xtab6=500;
   int ytab1=140;
   int ytab2;
   x_length-=x_origin;
   if (!Fixed_distance) {
     y_origin-=1.5*Vsize;
   }
   y_length=y_origin-y_length;
/*
   if (Logflag && ymin>0 && ymax>0) {
      ymin=log(ymin);
      ymax=log(ymax);
   }
   yfactor=(double)y_length/(ymax-ymin);
*/
   for (i=datarows=0; i<MAXDATAROWS; i++) if (xdata[i]!=MISSING) datarows++;
   for (i=0; i<MAXTEXTROWS; i++) {
     if(*default_text[i]) {
		fprintf(fpout, "FONT %d;\r\nMAP %d,%d;\r\nTEXT'%s';\r\n",
		   linefont[i], linecolumn[i], linerow[i], default_text[i]);
		if (*textline[i]) fprintf(fpout, "MAP %d,%d;\r\nTEXT': %s';\r\n",
			 linecolumn[i]+450, linerow[i], textline[i]);
	 }
   }
   fprintf(fpout, "SPD %d;\r\n", Pen);
   min=ymin;
   Logflag = kyoy_cal(Logflag);
   if (Logflag && ymin>0 && ymax>0) {
      ymin=log(ymin);
      ymax=log(ymax);
      fprintf(fpout, "SPD 1;\r\n");
      for (value=0.01; value<=10000; value*=10) {
        d=log(value);
        if (d>=ymin && d<=ymax) {
          kyomove(x_origin-RIM,y_origin-(d-ymin)*yfactor);
          kyodraw(x_origin+x_length+RIM, y_origin-(d-ymin)*yfactor);
        }
      }
      fprintf(fpout, "SPD %d;\r\n", Pen);
   }
   if (Fixed_distance) {
      x_length-=2*rim;
      x_origin+=rim;
   }
   xfactor=(xmax>xmin)?(double)x_length/(xmax-xmin):1;
   inter=(datarows)?(double)x_length/(datarows-1):0;
   ff=1.0;
   for (i=MAXDATAROWS-1; i>=0; i--) {
     if (xdata[i]==MISSING) continue;
     IgG= (ydata[i][3]==MISSING)? ydata[i][1]:ydata[i][3]*ff;
     IgG4=(ydata[i][2]==MISSING)? ydata[i][0]:ydata[i][2];
     ratio=(ydata[i][3]==MISSING)? IgG4/ydata[i][1]:IgG4/ydata[i][3];
     IgG4=ratio*IgG;
/*
     if (ydata[i][1]!=MISSING && ydata[i][3]!=MISSING && Fflag) IgG4*=IgG/ydata[i][3];
*/
     ff=(ydata[i][1]==MISSING || !Fflag)?1.0:IgG/ydata[i][1];
	 fprintf(fpout,"FONT 1;\r\nMAP %d,%d;\r\nTEXT'   %2d   %9s   %9s';\r\n",
        tabelcolumn, tabelrow+i*70+ytab1+60, i+1, datestring(buffer,xdata[i]), volgnr[i]);
     if (IgG4>=.99*min) sprintf(buffer, "%.0f",IgG4);
     else if (IgG4==MISSING) sprintf(buffer, "---");
     else sprintf(buffer, "< %.0f",min);
     fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%6s';\r\n",
        tabelcolumn+xtab1+xtab2+xtab3, tabelrow+i*70+ytab1+60, buffer);
     if (IgG>=.99*min) sprintf(buffer, "%.0f",IgG);
     else if (IgG==MISSING) sprintf(buffer, "---");
     else sprintf(buffer, "< %.0f",min);
     fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%7s';\r\n",
        tabelcolumn+xtab1+xtab2+xtab3+xtab4, tabelrow+i*70+ytab1+60, buffer);
     if (IgG>RATIO_LIMIET && IgG4!=MISSING) {
        if (IgG4>=.99*min) sprintf(buffer, "%6.0f",100*IgG4/IgG);
        else sprintf(buffer,"< %4.0f",100*min/IgG);
     } else sprintf(buffer, "---");
     fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%7s';\r\n",
        tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+60, tabelrow+i*70+ytab1+60, buffer);
     cprintf("\r\n%d %10s %12.3f %12.3f %12.3f", i+1, datestring(buffer,xdata[i]), IgG4, IgG, ff);
     x=(Fixed_distance)?
          (x_origin+(
             (inter)?
                i*inter:
                x_length/2)):
          (x_origin+(int)((xdata[i]-xmin)*xfactor));
     if (IgG4>MISSING) {
        if (IgG4<0.99*min) y=y_origin+20;
        else if (Logflag) {
           if (IgG4 >0) y=y_origin-(log(IgG4)-ymin)*yfactor;
           else y=y_origin+20;
		} else y=y_origin-(IgG4-ymin)*yfactor;
     } else continue;
     kyobar(6, x-((Fixed_distance)?Barsize:4*Barsize/10), y_origin, x, y);
     y=y_origin-(IgG-ymin)*yfactor;
     if (IgG>MISSING) {
        if (IgG<0.99*min) y=y_origin+20;
        else if (Logflag) {
            if (IgG >0) y=y_origin-(log(IgG)-ymin)*yfactor;
            else y=y_origin+20;
        } else y=y_origin-(IgG-ymin)*yfactor;
     } else continue;
     kyobar(8, x, y_origin, x+((Fixed_distance)?Barsize:4*Barsize/10), y);
     fprintf(fpout, "FONT %d;\r\nMAP %d,%d;\r\nTEXT'%d';\r\n",10, x-14, y_origin+80, i+1);
   }
   if (!Fixed_distance) {
      yrs=yearnumber(xmax)-yearnumber(xmin);
/*
      fprintf(fpout, "SPD 2;\r\nFONT %d;\r\nMAP %d,%d;\r\nDAP %d,%d;\r\n",
         yrs>6?15:10, x_origin-3*rim/2, y_origin+200, x_origin+x_length+7*rim/4, y_origin+200);
*/
      fprintf(fpout, "SPD 2;\r\nFONT %d;\r\nMAP %d,%d;\r\nDAP %d,%d;\r\n",
         yrs>6?15:10, x_origin-rim/2, y_origin+200, x_origin+x_length+3*rim/4, y_origin+200);
      year=yearnumber(xmin);
      do {
         x=(x_origin+(int)((datenumber(year,1,1)-xmin)*xfactor));
         textpos=x+(datenumber(year+1,1,1)-datenumber(year,1,1))*xfactor/2;
         if (yrs>6) textpos+=20;
         if (x>x_origin-rim) {
/*
            if (textpos<=x_origin+x_length) {
				kyomove(x, y_origin + 160);
                kyodraw(x, y_origin + 200);
                sprintf(buffer,"%d",1900+year);
                kyotext( 0,CENTERTOP, textpos, y_origin+120, buffer);
            } else if (x<=x_origin+x_length+rim) {
                kyomove(x, y_origin + 160);
                kyodraw(x, y_origin + 200);
            }
*/
            if (textpos<=x_origin+x_length+rim) {
                kyomove(x, y_origin + 160);
                kyodraw(x, y_origin + 200);
                sprintf(buffer,"%d",1900+year);
                kyotext( 0,CENTERTOP, textpos, y_origin+120, buffer);
            }
         }
         year++;
      } while (x<x_origin+x_length);
      fprintf(fpout, "SPD %d;\r\n", Pen);
   }
   ytab2=datarows*70+40;
   if (Kader) kyo62tabel(tabelcolumn, tabelrow, xtab1, xtab2, xtab3, xtab4, xtab5, xtab6, ytab1, ytab2);
   y=tabelrow+ytab1-50;
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'volg-nr';\r\n",
      tabelcolumn+20, y);
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'  datum';\r\n",
      tabelcolumn+xtab1+20, y);
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'  CLB-nr';\r\n",
      tabelcolumn+xtab1+xtab2+20, y);
   kyobar(6, tabelcolumn+xtab1+xtab2+xtab3+20,y-60,tabelcolumn+xtab1+xtab2+xtab3+130,y+20);
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+150, y);
   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'4';\r\n",
	 tabelcolumn+xtab1+xtab2+xtab3+150+90, y+20);
   kyobar(8, tabelcolumn+xtab1+xtab2+xtab3+xtab4+20,y-60,tabelcolumn+xtab1+xtab2+xtab3+xtab4+130,y+20);
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+xtab4+140, y);
   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'totaal';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+xtab4+140+100, y+20);
   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG /IgG    (%)';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20, y);
   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'4';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+90, y+20);
   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'totaal';\r\n",
      tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+250, y+20);
}

void read_wks(char *filename) {
   int integer, op, body_length, formula_size, file_type, formatbyte,
       column, row, end_col, end_row, max_len=MAX_LEN;
   long counter, filesize;
   double step;
   int mark;
   double lotusnumber;
   unsigned char c, pos;
   char stringbuffer[MAX_LEN+1];
   fpin=fopen(filename,"rb");
   filesize=filelength(fileno(fpin));
   clrscr();
   make_ruler(23,1,10,5);
   strleft(shortfilename, filename, MAXLEN);
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
         printf("\n%4d %s: body_length=%d",__LINE__,__FILE__, body_length);
         if (getch()==27) farewell();
         putch('\n');
      }
      counter += body_length+4;
      switch(op){
           case BOF:
           if (!Kyo) fprintf(fpout,"bytecount %5ld: BOF\n",counter-body_length-4);
           file_type=getw(fpin);
               if (file_type!=LOTUS1 && file_type!=LOTUS2){
                   cprintf("file-type: %4X ipv %4X of %4X: dus geen LOTUS-worksheet",file_type,LOTUS1,LOTUS2);
                   farewell();
               }
               break;
           case DIMENSIONS:
			   getw(fpin);          /* start_col */
               getw(fpin);          /* start_row */
               end_col=getw(fpin);
               end_row=getw(fpin);
               if (!Kyo) fprintf(fpout,"bytecount %5ld: DIMENSIONS; end_row=%d end_col=%d\n",counter-body_length-4, end_row, end_col);
               break;
           case INTEGER:
               formatbyte=isdate(getc(fpin));      /* format */
               column=getw(fpin);
               row=getw(fpin);
               integer=getw(fpin);
               assign_number(row, column, (float)integer, formatbyte);
               if (!Kyo) fprintf(fpout,
                  "bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%d\n",
                  counter-body_length-4, "INTEGER", formatbyte, row, column, integer);
               break;
           case NUMBER:
               formatbyte=isdate(getc(fpin));      /* format */
               column=getw(fpin);
               row=getw(fpin);
               fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               assign_number(row, column, lotusnumber, formatbyte);
               if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%9.3lf\n",counter-body_length-4, "NUMBER", formatbyte, row, column, lotusnumber);
               break;
           case FORMULA:
               formatbyte=isdate(getc(fpin));      /* format */
               column=getw(fpin);
               row=getw(fpin);
               fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               if (row==FIRSTTEXTROW && column==4) strcpy(textline[0],datestring(buffer,lotusnumber));
			   else assign_number(row, column, lotusnumber, formatbyte);
			   formula_size=getw(fpin);
			   skip(fpin,formula_size);
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%9.3lf\n",counter-body_length-4, "FORMULA", formatbyte, row, column, lotusnumber);
			   break;
		   case BLANK:
			   formatbyte=isdate(getc(fpin));      /* format */
			   column=getw(fpin);
			   row=getw(fpin);
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; row=%3d column=%3d\n",counter-body_length-4, "BLANK", row, column);
			   break;
		   case LABELCODE:
			   pos=0;
			   clear_string(stringbuffer);
			   formatbyte=isdate(getc(fpin));      /* format */
			   column=getw(fpin);
			   row=getw(fpin);
			   while ((c=getc(fpin))!=0 && pos<MAX_LEN) {
				 if (pos<max_len-1) {
					if (pos>0) stringbuffer[pos-1] = c;
					pos++;
				 }
			   }
			   stringbuffer[pos-1]='\0';
			   if (row==0 && column==0 && strcmp(stringbuffer,"BLAST 1")!=0) {
				   clrscr();
                   cprintf("Not a BLAST-1 worksheet\r\n"
                   "Press any key ");
                   farewell();
               } else if (column==4 && row>=FIRSTTEXTROW && row<=LASTTEXTROW) strcpy(textline[row-FIRSTTEXTROW],stringbuffer);
               else if (column==3 && row>=FIRSTTEXTROW && row<=LASTTEXTROW) strcpy(default_text[row-FIRSTTEXTROW],stringbuffer);
               else if (row>=FIRSTDATAROW && row<=LASTDATAROW) {
                 if (column==1) {
                    strncpy(volgnr[row-FIRSTDATAROW],stringbuffer,9);
                    volgnr[row-FIRSTDATAROW][9]='\0';
                 }
                 else assign_number(row, column, ymin/2, formatbyte);
               }
               if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d \"%s\"\n",counter-body_length-4, "LABEL", formatbyte, row, column, stringbuffer);
               break;
           default:
               skip(fpin,body_length);
         }
      op=getw(fpin);
   }
   fclose(fpin);
   while (mark++<50) putch(220);
   putch('\n');
}

void assign_number(int row, int column, float value, int formattype){
   if (row==1) {
      if (column==1) x_origin=(int)value;
      else if (column==2) x_length=(int)(value+0.49);
      else if (column==3) y_origin=(int)(value+0.49);
      else if (column==4) y_length=(int)(value+0.49);
   } else if (row==2) {
      if (column==1) xmin=value;
      else if (column==2) xmax=value;
      else if (column==3) ymin=value;
      else if (column==4) ymax=value;
   } else if (row==FIRSTTEXTROW-1) {
      if (column==0) tabelfont=(int)(value+0.49);
      else if (column==1) tabelrow=(int)(value+0.49);
      else if (column==2) tabelcolumn=(int)(value+0.49);
   } else if (row>=FIRSTTEXTROW && row<=LASTTEXTROW) {
      if (column==0) linefont[row-FIRSTTEXTROW]=(int)(value+0.49);
      else if (column==1) linerow[row-FIRSTTEXTROW]=(int)(value+0.49);
      else if (column==2) linecolumn[row-FIRSTTEXTROW]=(int)(value+0.49);
      else if (column==3) {
         if (formattype==-1)     datestring(buffer,value);
         else if (formattype>-1) sprintf(buffer,"%.*f",formattype, value);
         else                    sprintf(buffer,"%.0f",value);
         strcpy(default_text[row-FIRSTTEXTROW],buffer);
      }
      else if (column==4) {
         if (formattype==-1)     datestring(buffer,value);
         else if (formattype>-1) sprintf(buffer,"%.*f", formattype, value);
         else                    sprintf(buffer,"%.0f",value);
         strcpy(textline[row-FIRSTTEXTROW],buffer);
      }
    } else if (row>=FIRSTDATAROW && row<=LASTDATAROW) {
      if (column==0) xdata[row-FIRSTDATAROW]=value;
      else if (column>=FIRSTDATACOLUMN && column<=LASTDATACOLUMN)
         ydata[row-FIRSTDATAROW][column-FIRSTDATACOLUMN]=(value>=ymin)?value:ymin/2;
   }
}

void kyo62tabel(
   int tabelcolumn,
   int tabelrow,
   int xtab1,
   int xtab2,
   int xtab3,
   int xtab4,
   int xtab5,
   int xtab6,
   int ytab1,
   int ytab2) {
   int x_length, y_length;
   x_length= xtab1+xtab2+xtab3+xtab4+xtab5+xtab6;
   y_length= ytab1+ytab2;
   kyomove(tabelcolumn,          tabelrow);
   kyodraw(tabelcolumn+x_length, tabelrow);
   kyomove(tabelcolumn,          tabelrow+ytab1);
   kyodraw(tabelcolumn+x_length, tabelrow+ytab1);
   kyomove(tabelcolumn,          tabelrow+ytab1+ytab2);
   kyodraw(tabelcolumn+x_length, tabelrow+ytab1+ytab2);
   kyomove(tabelcolumn,                                     tabelrow);
   kyodraw(tabelcolumn,                                     tabelrow+y_length);
   kyomove(tabelcolumn+xtab1,                               tabelrow);
   kyodraw(tabelcolumn+xtab1,                               tabelrow+y_length);
   kyomove(tabelcolumn+xtab1+xtab2,                         tabelrow);
   kyodraw(tabelcolumn+xtab1+xtab2,                         tabelrow+y_length);
   kyomove(tabelcolumn+xtab1+xtab2+xtab3,                   tabelrow);
   kyodraw(tabelcolumn+xtab1+xtab2+xtab3,                   tabelrow+y_length);
   kyomove(tabelcolumn+xtab1+xtab2+xtab3+xtab4,             tabelrow);
   kyodraw(tabelcolumn+xtab1+xtab2+xtab3+xtab4,             tabelrow+y_length);
   kyomove(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5,       tabelrow);
   kyodraw(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5,       tabelrow+y_length);
   kyomove(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+xtab6, tabelrow);
   kyodraw(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+xtab6, tabelrow+y_length);
}

int initialise_printer(void) {
   int i;
   char c=0;
/*
   char errorcode[5];
*/
   do {
     fpout=stdprn;	 /**** output directly to Kyocera ****/
/*
     printf("Printing .... ");
*/
     i=biosprint(1,0,0);	/* initialises LPT1 */
     if (Trace) fprintf(tracefile,"\n%4d biosprint: %d",__LINE__, i);
     if (i!=16 && i!=144) {
        if (i==1) {
           clrscr();
           printf("Kyocera niet goed geinitialiseerd\nZet Kyocera aan en re-start computer");
           farewell();
        }
        printf("\nPrinter error %d\nFix and press RETURN\n: ",i);
/*
        if (i&8) printf("Switch printer ON and press RETURN\n: ");
        if (i&32) printf("Printer out of paper? Fix and press RETURN\n: ");
        *buffer='\0';
        itoa(i, errorcode, 10);
        keycopy(buffer, errorcode, 80);
        if (itoa(buffer,"              ",10)==i && !result) Kyo=1;
        if (!*buffer) farewell();
*/
        c=getkey(0);
     }
   } while (i!=16 && i!=144 || !Kyo || c==ESC);
   if (c==ESC) return 0;
   return 1;
}

void error(int n, long count) {
   clrscr();
   if (!Kyo) printf("Write error %d (ferror()=%d) at bytecount %ld\ndisc full?", n, ferror(fpout), count);
   else {
      printf("Write error %d (ferror()=%d) at bytecount %ld", n, ferror(fpout), count);
      printf("\nPress ESC to quit\nPress any other key to continue ");
      if (getch()==27) farewell();
   }
putch('\n');
}

int get_options(void) {
   int n, option;
   for (;;) {
      do {
         clrscr();
         printf("B - Bar dikte               : ");
            if (Fixed_distance) HLON;
            printf("%d",Barsize);
            if (Fixed_distance) HLOFF;
            putch(' ');
            if (!Fixed_distance) HLON;
            printf("(%d)\r\n", 4*Barsize/10);
            if (!Fixed_distance) HLOFF;
         cprintf("C - Continuiteits-correctie : ");
         yesno(Fflag,"YES","NO");
         cprintf("F - Font                    : %d\r\n", Font);
         cprintf("I - print file-Information  : ");
         yesno(Header,"YES","NO");
         cprintf("K - Kader                   : ");
         yesno(Kader,"YES","NO");
         cprintf("L - Log-transformatie       : ");
         yesno(Logflag,"YES","NO");
         cprintf("P - Pen dikte               : %d (fill)/%d (contour)\r\n", Fillpen, Pen);
         cprintf("T - Tijds-afhankelijke x-as : ");
         yesno(!Fixed_distance,"YES","NO");
         cprintf("Y - Y-legende               : %s\r\n", y_legend);
         cprintf("\r\nD - naar Disc? %s\r\n",
          (Kyo)?"Nu gaan gegevens naar de KYOCERA i.p.v. naar een floppie":
                "Nu gaan gegevens naar een floppie i.p.v. naar de Kyocera");
         cprintf("\r\nRETURN = accept options: ");
         if ((option=getkey(0))==RETURN || option==EXEC) return 0;
         if (option==ESC || option==CANCEL)             return -1;
      } while (strpos("BCDFIKLPTY",option,0)==-1);
      switch (option) {
         case 'B':
            clrscr();
            cprintf("Bar size (1-1000) default:%d\r\n: ",Barsize);
            gets(buffer);
            n=atoi(buffer);
            if (n>0 && n<=1000) Barsize=n;
            break;
         case 'C':
            Fflag=(Fflag)?0:1;
            break;
         case 'I':
            Header=(Header)?0:1;
            break;
         case 'K':
            Kader=(Kader)?0:1;
            break;
         case 'L':
            Logflag=(Logflag)?0:1;
            break;
         case 'D':
            Kyo=(Kyo)?0:1;
            break;
         case 'T':
            Fixed_distance=(Fixed_distance)?0:1;
            break;
         case 'P':
            clrscr();
            cprintf("Fill Pen size (1-40) default:%d\r\n: ",Fillpen);
            gets(buffer);
            n=atoi(buffer);
            if (n>0 && n<=200) Fillpen=n;
            cprintf("\r\n\r\nContour Pen size (1-40) default:%d\r\n: ",Pen);
            gets(buffer);
            n=atoi(buffer);
            if (n>0 && n<=200) Pen=n;
            break;
         case 'F':
            clrscr();
            cprintf("Fonts:\r\n\n\n"
                   " 7, 16  = very small  ( 7-point)\r\n"
                   "15      = small       ( 9-point)\r\n"
                   " 6      = Elite       (10-point)\r\n"
                   " 8      = Gothic      (12-point)\r\n"
                   " 9      = Gothic bold (12-point)\r\n"
                   " 1      = Courier     (12-point)\r\n"
                   "10      = Helve Bold  (14.4-point)\r\n\r\n"
                   " default:%d\r\n: ",Font);
            gets(buffer);
            n=atoi(buffer);
            if (n>0 && n<=16) Font=n;
            break;
         case 'Y':
            clrscr();
            printf("\nLegend y-axis \"%s\": ",y_legend);
            strcpy(y_legend, keycopy(buffer, y_legend, MAX_LEN));
            break;
      }
   }
}

void get_charsize(void) {
   int i;
   for (i=0;i<256;i++) charsize[i]=1;
}

int dots(char c) {
   if (Font!=10 || c<32 || c>127) return font[Font][2];
   return font10[c-32];
}

void kyostart(void){
   int w;
   if (Lotus) w=fprintf(fpout,"\"!R! RES;\r\nUNIT D; SPD %d;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n",Pen);
   else w=fprintf(fpout,"!R! RES;\r\nUNIT D; SPD %d;\r\n",Pen);
   if (w==EOF || ferror(fpout)) error(w,count);
}

void kyoend(void) {
   int w;
   w=fprintf(fpout,"RES;\r\nEXIT;\r\n");
   if (w==EOF || ferror(fpout)) error(w,count);
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

void farewell(void) {
   fclose(fpout);
   fclose(fpin);
   setdisk(startup_drive);
   exit(0);
}

int get_inputfiles(int argc, char *argv1) {
   int i,c;
   if (argc>1) {
      strncpy(filename[0],argv1,sizeof(filename[0]));
      while ((fpin=fopen(filename[0],"rb")) == NULL) {
          fclose(fpin);
          clrscr();
          E0;
          if (getkey(0)!=CR) farewell();
      }
      fclose(fpin);
      nooffiles=getfile(filename[0],filename);
   } else {
      do {
         totalfiles=getfile("a:*.wk*", filename);
         while (!totalfiles) {
           clrscr();
           M0;
           if (getkey(0)!=CR) farewell();
           clrscr();
           totalfiles=getfile("a:*.wk*",filename);
         }
         if (Trace) fprintf(tracefile,"\n%4d total_files=%d",__LINE__, totalfiles);
         nooffiles=select_files();
         if (nooffiles<1) {
            clrscr();
            printf("no files selected\npress RETURN to re-try\npress ESC to quit\n: ");
            c=getkey(0);
            if (c==CTRL_C ||c==ESC || c==CANCEL) farewell();
            c=ESC;
         } else {
            clrscr();
            printf("selected wk*-files:\n\n");
            for (i=c=0;i<nooffiles;i++,c++) {
               gotoxy(1+c/15*20, 4+c%15);
               printf("%-12s %6ld\n", filename[file[i]], filesize[file[i]]);
               if (c>=59) {
                  MORE;
                  clrscr();
                  c=-1;
               }
            }
            gotoxy(1,21);
            printf("RETURN to accept\nESC for selection of other files\n");
            if (Kyo) printf("M for option-menu\n: ");
            else     printf("Current output: disc-file; press M for option-menu\n");
            c=getkey(0);
            if (c==CTRL_C) farewell();
         }
         clrscr();
      } while (c==ESC || c==CANCEL);
      if (c=='M') if (get_options()==-1) return -1;
   }
   return nooffiles;
}

void get_outputfile(char *s, int argc, char *argv2) {
  /**** output to *.kyo file (ASCII) ****/
     setmem(kyofile,MAXLEN,'\0');
     if (argc>2) strncpy(kyofile,argv2, sizeof(kyofile));
     else make_kyofile_name(kyofile,s);
     if (!check_output_file(kyofile) || (fpout=fopen(kyofile,"wb")) == NULL) {
        printf("cannot open output file %s\n",kyofile);
        farewell();
     } else cprintf("\r\noutput-file \"%s\" open\r\n",kyofile);
}

int check_output_file(char *s) {
   char buffer[80];
   result=0;
   while ((fpin=fopen(s,"rb"))!=NULL && result!=RETURN) {
     strcpy(buffer,s);
     fclose(fpin);
     E1_CHECK_OUTPUT_FILE;
     if (keycopy(s,buffer,80)==NULL) return 0;
     if (s==NULL || strlen(s)<1) return 0;
     if (Show) M0_CHECK_OUTPUT_FILE;
   }
   return 1;
}

int strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines) {
     int pos, key;
     int maxlen, n, result, row, col, maxnoofitems;
/*
     maxlen=80*nooflines/rows;
*/
     maxlen=80*nooflines/rows-3;
     if (maxlen<1) maxlen=1;
     if (maxlen>12) maxlen=12;
     maxnoofitems=nooflines*20;
     if (rows>maxnoofitems) rows=maxnoofitems;
     if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
     *selected_test=-2;
     startchars[rows]='\0';
     n = current_pos;
/*
     col=(maxlen+3)*(n/nooflines);
     CURSOR(row,col);
*/
     row=startrow + n % nooflines;
     col=(maxlen+2)*(n/nooflines)+1;
     gotoxy(col,row);
     HLON;
     strleft(shortfilename,filename[n],maxlen);
/*
     printf("%c %-*s",flag[n],maxlen+1,shortfilename);
*/
     printf("%c%-*s",flag[n],maxlen,shortfilename);
     HLOFF;
     gotoxy(25,25);
     printf("%s %s (%ld bytes)",filename[n],fileinfo[n],filesize[n]);
     key=getkey(0);
     gotoxy(25,25);
     clreol();
     gotoxy(col,row);
/*
     printf("%c %-*s", flag[n], maxlen+1, strleft(shortfilename, filename[n], maxlen));
*/
     printf("%c%-*s", flag[n], maxlen, strleft(shortfilename, filename[n], maxlen));
     if (key<=SPACE || key>256) {
        switch (key) {
           case UP:
           case W_UP:
                n--;
                if (n<0) n=rows-1;
                result = n;
              break;
           case DOWN:
           case W_DOWN:
                n++;
                if (n > rows-1) n=0;
                result = n;
              break;
           case TAB:
           case RIGHT:
           case W_RIGHT:
                if (n<rows-nooflines) n+=nooflines;
                result = n;
              break;
           case BACKTAB:
           case LEFT:
           case W_LEFT:
                if (n>nooflines-1) n-=nooflines;
                result = n;
              break;
           case PgUp:
           case W_PgUp:
/*
                n=(n>nooflines-1 && rows>nooflines) ?
                     nooflines:
                     0;
*/
                n=(n/nooflines)*nooflines;
                result = n;
              break;
           case HOME:
           case W_HOME:
                n=0;
                result = n;
              break;
           case PgDn:
           case W_PgDn:
/*
                n=(n>nooflines-1 || nooflines>rows) ?
                     rows-1:
                     nooflines-1;
*/
                n=(n/nooflines)*nooflines + nooflines-1;
                if (n>rows-1) n=rows-1;
                result = n;
              break;
           case SHIFT_HOME:
                n=rows-1;
                result = n;
              break;
             case CTRL_A:
                *selected_test= -4;
                result = n;
              break;
             case CTRL_B:
             case CTRL_Z:
                *selected_test= -3;
                result = n;
              break;
             case RETURN:
             case SPACE:
                *selected_test=n;
                result = n;
              break;
             case W_ENDKEY:
             case W_EXEC:
             case EXEC:
             case ESC:
             case W_CANCEL:
             case CANCEL:
                *selected_test= -1;
                result = n;
              break;
           case CTRL_C:
               exit(0);
           default:
               result = n;
               break;
        }
    } else {
            pos=strpos(startchars,toupper(key),n);
            n=(pos<0) ? n : pos;
            result =  n;
    }
    return result;
}

int getkey(int flag) {
int c;
   result=1;
   c=getch();
   if (c==SPECIAL_KEY || c==W_SPECIAL_KEY) {
      c=256+getch();
      if (c>F5 && c<=F10) return c;
   } else {
      if (c==ESC) result=0;
      if (c==CTRL_C) farewell();
      if (!flag) c=toupper(c);
   }
   return c;
}

char *strleft(char *buffer, char *s, int n) {
   int i;
   for (i=0; s[i] && i<n; i++) buffer[i]=s[i];
   buffer[i]='\0';
   return buffer;
}

int getfile(char *path, char filename[][LENFILENAME+1]) {
    int n=0, i=0, done, result;
	int month, day, year, hour, min, sec;
	struct ffblk ffblk;
	done=findfirst("a:*.*",&ffblk,FA_LABEL);
	if (!done) strncpy(volumelabel,ffblk.ff_name,12);
    else {
       i=1;
       printf("No disc name found.\nEnter disc name (RETURN for none): ...........\b\b\b\b\b\b\b\b\b\b\b");
       gets(buffer);
       if (*buffer) {
          strncpy(volumelabel,buffer,11);
          volumelabel[11]='\0';
          sprintf(buffer,"LABEL A:%s",volumelabel);
          i=system(buffer);
          if (i==-1) {
             printf("Unable to perform command \"%s\"\nPress any key to continue",buffer);
             if (getch()==27) farewell();
             strcpy(volumelabel,"no disc name");
          }
       }
       else strcpy(volumelabel,"no disc name");
       clrscr();
       if (!i) printf("Disc name: \"%s\"",volumelabel);
       else puts(volumelabel);
    }
/*
	while (ffblk.ff.attrib != FA_LABEL && !done) findnext(&ffblk);
*/
    gotoxy(28,14);
    S3;
    done=findfirst(path,&ffblk,0);
        if (done) result= 0;
	else for(n=0,done=0;!done && n<MAXFILES;n++) {
		strcpy(filename[n],ffblk.ff_name);
		filesize[n]=ffblk.ff_fsize;
		maketime(&hour, &min, &sec, ffblk.ff_ftime);
		makedate(&month, &day, &year, ffblk.ff_fdate);
		sprintf(fileinfo[n],"%2.2d-%2.2d-19%2.2d, %2.2d:%2.2d",
		     day, month, year,hour, min);
                done=findnext(&ffblk);
	}
    result = n;
	for (i=10;i<20;i++) {
	   gotoxy(1,i);
	   delline();
	}
	return result;
}

int select_files(void) {
   int t, column, nooffiles, group, fileno, startrow, nooflines, selected_file;
   nooffiles=0;
   selected_file=-1;
   startrow=5;
   nooflines=17;
   startchars[totalfiles]='\0';
   for (t=0;t<totalfiles;t++) {
      startchars[t]=toupper(filename[t][0]);
      flag[t]=OFF;
      file[t]=t;
   }
   write_screen(startrow, totalfiles, nooflines, TRUE);
   fileno=0;
   do {
      group=1;
      gotoxy(1, 23);
      printf("\033[7mMove cursor and then select wks-files with RETURN-key\n"
                    "Press UITVOEREN to end selection of wks-files\n"
                    "Press F10 to view file:\033[0m");
      fileno = strmenu(
         fileno,
         &selected_file,
         totalfiles,
         startrow,
         nooflines);
      if ( selected_file == -1) break;
      if ( selected_file == -2) continue;
      if ( selected_file == -3) {
         group=Group;
         if (fileno+group>=totalfiles) {
            group=totalfiles-fileno;
            putch(7);
         }
      }
      if ( selected_file == -4) {
         group=totalfiles-fileno;
      }
      for (t=fileno;t<fileno+group;t++) {
         if (flag[t]==OFF && nooffiles<MAXFILES && nooffiles<totalfiles) {
            flag[t]=ON;
            nooffiles++;
         }
         else if (flag[t]==ON) {
            flag[t]=OFF;
            nooffiles--;
         }
         else {
            fileno--;
            putch(7);
         }
      }
      write_screen( startrow, totalfiles, nooflines, FALSE);
      fileno+=group-1;
      fileno = (++fileno) % totalfiles;
   } while (selected_file != -1);
   for (t=0,column=0;t<totalfiles;t++) if (flag[t]==ON) file[column++]=t;
   return nooffiles;

}

void write_screen(int startrow, int rows, int nooflines, int new) {
int maxlen, n, row, col, maxnoofitems;
/*
  maxlen=80*nooflines/rows;
*/
  maxlen=80*nooflines/rows-3;
  if (maxlen<1) maxlen=1;
  if (maxlen>12) maxlen=12;
  maxnoofitems=nooflines*20;
  if (Trace) fprintf(tracefile,"\n%4d: rows=%d nooflines=%d maxnoofitems=%d maxlen=%d",
     __LINE__, rows, nooflines, maxnoofitems, maxlen);
  if (rows>maxnoofitems) rows=maxnoofitems;
  if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
  for (n=0;n<rows;n++) {
     row=startrow + n % nooflines;
/*
     col=(maxlen+3)*(n/nooflines);
*/
     col=(maxlen+2)*(n/nooflines)+1;
     gotoxy(col,row);
     if (new) {
          strleft(shortfilename,filename[n],maxlen);
/*
          printf("%c %-*s",flag[n],maxlen+1,shortfilename);
*/
          printf("%c%-*s",flag[n],maxlen,shortfilename);
      }
     else putch(flag[n]);
  }
}


char *make_kyofile_name(char *kyofile, char *wksfile) {
   int pos=0;
   char c;
   *kyofile='\0';
   do {
      c=wksfile[pos];
      kyofile[pos]=c;
      pos++;
   } while(c!='.' && c!='\0');
   if (c!='.') {
      kyofile[pos]='.';
      pos++;
   }
   strcat(kyofile,"kyo");
   if (Trace) fprintf(tracefile,"\n%4d kyofile=%s wksfile=%s",__LINE__,kyofile, wksfile);
   return kyofile;
}

void startup_message(void) {
   clrscr();
   frame(65,4);
   gotoxy(3,2);
   S1;
   gotoxy(3,3);
   S2;
   gotoxy(1,5);
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

void read_environment(void) {
   startup_drive=getdisk();
   Trace= (int)set_env_variable("TRACE", 0);
   Header= (int)set_env_variable("HEADER", HEADER);
   Show= (int)set_env_variable("SHOW", SHOW);
   Group= set_env_variable("GROUP", GROUP);
   Kyo= (int)set_env_variable("KYO", KYO);
   Lotus= (int)set_env_variable("LOTUS", LOTUS);
   Lettershift= (int)set_env_variable("SHIFT",LETTERSHIFT);
   Paperlength= (int)set_env_variable("PAPERLENGTH",PAPERLENGTH);
   Pen=(int)set_env_variable("PEN",PEN);
   Font=(int)set_env_variable("FONT",FONT1);
}

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
   if (Trace) fprintf(tracefile,"%4d set_env_variable %12s \"%s\" default=%7.2f result=%7.2f\n",
      __LINE__, env_string, s, default_value, result);
   return result;
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
               case EXEC:
               case RETURN:
               case W_EXEC:
                    if (!target[0]) {
                       strcpy(target,source);
                       result=RETURN;
                    }
                    else target[t]='\0';
                    break;
               case ESC:
               case CANCEL:
               case BACKTAB:
               case W_BACKTAB:
                    target[0]=0;
                    c=RETURN;
                    break;
               case BACKSPACE:
                    insert=0;
                    if (t>0) {
                         printf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
                    break;
               case RIGHT:
               case W_RIGHT:
                    insert=0;
                    if (t<maxlen && source[s]){
                         putch(source[s]);
                         target[t++]=source[s++];
                    }
                    break;
               case DELETE:
               case W_DELETE:
                    source++;
                    insert=0;
                    break;
               case INSERT:
               case W_INSERT:
                    insert=1;
                    break;
               case LEFT:
               case W_LEFT:
                    insert=0;
                    if (t>0) {
                         printf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
                    break;
               case F3:
               case W_F3:
                    insert=0;
                    while (t<maxlen && source[s]) {
                         putch(source[s]);
                         target[t++]=source[s++];
                    }    
                    break;
               case F5:
               case W_F5:
                    insert=0;
                    for (tt=0;tt<t;tt++) source[tt]=target[tt];
                    for (t=0;t<maxlen;t++) target[t]='\0';
                    putch('@');
                    putch('\n');
                    s=t=0;
                    break;
               case CTRL_C:
                    farewell();
                    break;
               }
          }
     }
     return target;
}

int strpos(char *s,char c,int pos) {
/* finds next occurrence of c in s starting at pos */
int p;
     for (p=pos+1;s[p];p++) if (s[p]==c) return p;
     for (p=0;p<=pos;p++) if (s[p]==c) return p;
     return -1;
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

int isvoid(char *s) {
   int i;
   for (i=0;s[i];i++) if (!isspace(s[i])) return 0;
   return 1;
}

void kyoy_axis_title(char *y_axis_string, int y_pos) {
   kyomove(40,y_pos);
   kyotext( 1, TOPRIGHT, x_origin, y_pos, y_axis_string);
}                                  


int kyoy_cal(int logflag) {
   int n, ox, lx, pos, cal, logyflag, maxlen=0, mantissa, lowest_exponent, highest_exponent;
   double calibrator_value, max, min;
   kyoy_axis_title(y_legend, y_origin - y_length);
   x_origin+=Vsize+80;
   if (!Logflag) ymin=0;
   logyflag= ascal(&noofcalibrators, &ymin, &ymax, logflag, TRUE);
   min=(logyflag)?log(ymin):ymin;
   max=(logyflag)?log(ymax):ymax;
   yfactor=(double)y_length/(max - min);
   for (n=0, maxlen=0;n<noofcalibrators;n++) if (maxlen<strlen(calibratorstring[n])) maxlen=strlen(calibratorstring[n]);
   x_origin+= 40 + maxlen * Hsize/2;
   x_length-= x_origin;
   if (x_length<=100) x_length=100;
   ox= x_origin;
   lx= x_length;
   if (Ycal/4 || logyflag) {                    /* bottom line */
      fprintf(fpout,"SPD 1;\r\n");
      kyomove(ox,    y_origin);
      kyodraw(ox+lx, y_origin);
      fprintf(fpout,"SPD %d;\r\n",Pen);
   }
   if (Ycal/4==2) {                 /* top line */
      kyomove(ox,    y_origin+y_length);
      kyodraw(ox+lx, y_origin+y_length);
   }
   if (Ycal>9) {                    /* right line */
      kyomove(ox+lx, y_origin);
      kyodraw(ox+lx, y_origin+y_length);
   }
   for (cal=0; cal<noofcalibrators; cal++) {
      if (value_of_calibrator[cal]<0.99*ymin || value_of_calibrator[cal]>1.01*ymax ) continue;
      calibrator_value=(Logflag && value_of_calibrator[cal]>0)?log(value_of_calibrator[cal]):
                       value_of_calibrator[cal];
      pos= (cal==noofcalibrators-1)?y_origin-y_length: y_origin - yfactor * (calibrator_value-min);
         kyomove(ox,pos);
         kyodraw(ox+RIM/2,pos);
         kyotext(0,CENTERRIGHT,ox-Hsize/2,pos,calibratorstring[cal]);
         if ((Ycal%4)<2) {
            kyomove(ox+lx,pos);
            kyodraw(ox+lx-RIM/2,pos);
         }
   }
   if (Ycal%2) {    /* tick-marks outside */
      ox+=RIM/2;
      lx-=RIM;
   }
      kyomove(ox,y_origin   );
      kyodraw(ox,y_origin-y_length);
      if ((Ycal%4)<2) {
         kyomove(ox+lx,y_origin   );
         kyodraw(ox+lx,y_origin-y_length);
      }
   if (Ycal%2) {    /* tick-marks outside */
      ox-=RIM/4;
      lx+=RIM/4;
   } else lx-=RIM/4;
   if (logyflag) {
      highest_exponent=(int)(max/log(10)+0.001);
      lowest_exponent=(int)(min/log(10)-1.1);
      for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
         for (mantissa=1;mantissa<10;mantissa++) {
            calibrator_value= log(val((double)mantissa,cal));
            if (calibrator_value > min && calibrator_value < max) {
               pos= y_origin - yfactor * (calibrator_value-min);
               if (pos>y_origin || pos<y_origin - y_length) continue;
                  kyomove(ox,pos);
                  kyodraw(ox+RIM/4,pos);
                  if ((Ycal%4)<2) {
                     kyomove(ox+lx,pos);
                     kyodraw(ox+lx+RIM/4,pos);
                  }
               }
         }
      }
   }
   x_origin=ox+RIM;
   x_length=lx-RIM;
   if (Ycal%4<2) x_length-=RIM;       /* right axis */
   return logyflag;
}

int ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag) {
    int n, range, exp, exp0, t, t0, found, negative, precision, extra_decimal=0;
    double unit, value, tempmin, tempmax, ratio, base, lowest_calibrator;
    int calbase[4][4]=   {1 , 0, 0, 0,
                      1 , 3, 0, 0,
                      1 , 2, 5, 0,
                      10,20,25,50
                     };
    int exponent[MAXNOOFCALIBRATORS];
    #ifdef TRACE
       if (Trace==2) {
          tracefile=fopen("TRACE","at");
          fprintf(tracefile,"\n%4d: *min=%.4f *max=%.4f",
             __LINE__, *min, *max);
          fclose(tracefile);
       }
    #endif
    if (logflag) round(scientific2(*min, &base, &exp));
    else round(scientific2(*max, &base, &exp));
    tempmin=*min * 10 +  fabs(*min)/10000;
    tempmax=*max * 10 -  fabs(*max)/10000;
    lowest_calibrator=val(10,exp);
    for (n=0;n<MAXNOOFCALIBRATORS;n++) {
        *calibratorstring[n]='\0';
        value_of_calibrator[n]=MISSING_CALIBRATOR;
    }

    if (tempmax<=0 || tempmin<=0) logflag=FALSE;
    if (logflag) {
        if (tempmin<=0) tempmin= lowest_calibrator;
        ratio= (double)(tempmax / tempmin);
        if (ratio < 51 )       range= SMALL;
        else if (ratio < 501 ) range= MEDIUM;
        else                   range= LARGE;
    } else range=LINEAR;
    t=t0=0;
    found=FALSE;
    do{
       exp0=--exp;
       unit=val(calbase[range][t],exp);
    } while (3*unit>=tempmax-tempmin);
    while (!found) {
       if (! logflag) {
          found= ( 6*unit >= tempmax - tempmin);
          if (!found) {
             t++;
             t%=range+1;
             if (t==0) exp++;
             unit=val(calbase[range][t],exp);
          }
          if (found && t==2) extra_decimal=1;
       } else {
           value=val((double)calbase[range][t],exp+1);
           found= (value >= tempmin);
           if (!found) {
             t0=t;
             exp0=exp;
             t++;
             t%=range+1;
             if (t==0) exp++;
           } else {
             t=t0;
             exp=exp0;
             value=val((double)calbase[range][t],exp+1);
          }
       }
    }

PHASE2: /* find corrected min and max and fill array */
    if (!logflag) {
       if ( tempmin/unit-(int)(tempmin/unit) != 0) {
          negative= ( tempmin < 0);
          tempmin=  unit * (int)(tempmin/unit);
          if (negative) tempmin -= unit;
       }
    } else tempmin=val((double)calbase[range][t],exp+1);
    for (n=0 ; n < MAXNOOFCALIBRATORS ; n++) {
        if ( ! logflag ) {
           value_of_calibrator[n]=(tempmin + n*unit)/10;
           exponent[n]=exp;
        } else {
           exponent[n]= exp;
           value_of_calibrator[n]= val((double)calbase[range][t],exp);
           t++;
           t%=range+1;
           if (t==0) exp++;
        }
        if (value_of_calibrator[n] >= tempmax/10) {
            tempmax= value_of_calibrator[n]*10;
            *noofcalibrators=n+1;
            break;
        }
    }
/*
    tempmin/=10;
    tempmax/=10;

    #ifdef TRACE
      if (Trace==2) {
         tracefile=fopen("TRACE","at");
         fprintf(tracefile,"\n%4d: tempmin=%.4f tempmax=%.4f",
            __LINE__, tempmin, tempmax);
         fclose(tracefile);
      }
    #endif
*/
    if (!calcflag) return logflag;

/* phase 3: print results in minimal-length strings */

    for (n=0; n< *noofcalibrators ; n++) {
        if (value_of_calibrator[n]>(tempmax>0?1.0001* tempmax:0.9999* tempmax)) {
           #ifdef TRACE
              if (Trace==2) {
                 tracefile=fopen("TRACE","at");
                 fprintf(tracefile,"\ncal[%d]=%.4lf tempmax=%.4lf verschil:%lf",
                    n, value_of_calibrator[n], tempmax, value_of_calibrator[n] - tempmax);
                 fclose(tracefile);
              }
           #endif
           break;
        }
        precision= (range==LINEAR) ? (extra_decimal - exponent[n]): -exponent[n];
        if (precision<0) precision=0;
        sprintf(calibratorstring[n],"%.*lf", precision, value_of_calibrator[n]);
    }
/*
   if (logflag) for (n=0; n<*noofcalibrators; n++) value_of_calibrator[n]=log(value_of_calibrator[n]);
*/
    return logflag;
}

void skip(FILE *fp,int noofbytes) {
   while (noofbytes>0) {
       nop=getc(fp);
       noofbytes--;
   }
}

double val(double mantissa, int exponent) {
   return mantissa*pow10(exponent);
}

double scientific2(double d, double *base, int *exp) {
   double logd;
   int neg=FALSE;
   if (d==0) {
      *base=0;
      *exp=-1;
      return *base;
   }
   if (d<0) {
      neg=1;
      d=-d;
   }
   logd=log10(d);
   *exp= (int)logd - (d<1)-1;
   *base= pow10d(logd-*exp);
   if (neg) *base=-*base;
   return *base;
}

int round(double d) {
    int i=(int)d;
    if (d-i<.5 ) return (int)(d+.5);
    return (int)(d+1);
}

double pow10d(double exponent) {
   /* 10^exponent, exponent is double rather than int, as required for pow() */
   return exp(exponent*log(10));
}

char *clear_string(char *s) {
   *s=' ';
   *(s+1)='\0';
   return s;
}

int dots_per_text(char *text) {
   int pos, dotszin;
   for (pos=dotszin=0; pos<strlen(text); pos++)
      dotszin+=
         (Font!=10 || text[pos]<32 || text[pos]>127)?
            font[Font][2]:
            font10[text[pos]-32];
   return dotszin;
}

void kyomove(int x, int y) {
   int w;
   w=fprintf(fpout,"MAP %d, %d;\r\n", x, y);
   count+=4;
   if (w==EOF || ferror(fpout)) error(w,count);
}

void kyodraw(int x, int y) {
   int w;
   w=fprintf(fpout,"DAP %d, %d;\r\n", x, y);
   count+=4;
   if (w==EOF || ferror(fpout)) error(w,count);
}

void kyobar(int filltype, int x0, int y0, int x1, int y1) {
   int lx, ly;
   lx=x1-x0;
   ly=y1-y0;
   kyomove(x0, y0);
   kyodraw(x1, y0);
   kyodraw(x1, y1);
   kyodraw(x0, y1);
   kyodraw(x0, y0);
   fprintf(fpout,"SPD %d;\r\n",Fillpen);
   kyofill(filltype, x0, y0, lx, ly);
   fprintf(fpout,"SPD %d;\r\n",Pen);
}

void kyofill(int t, int x0, int y0, int lx, int ly) {
   int d, dd, direction, dx1, dy1, dx2, dy2;
   if (lx<0) {
      lx=-lx;
      x0-=lx;
   }
   if (ly<0) {
      ly=-ly;
      y0-=ly;
   }
   if (t<=0 || t>8) return;
   if (t==2) {
      kyofill(3, x0, y0, lx, ly);
      kyofill(1, x0, y0, lx, ly);
      return;
   }
   if (t==4) {
      for (d=VFILL1; d<lx; d+=VFILL1) {
         kyomove(x0+d,y0);
         kyodraw(x0+d,y0+ly);
      }
      return;
   }
   if (t==8) {
      for (d=VFILL2; d<lx; d+=VFILL2) {
         kyomove(x0+d,y0);
         kyodraw(x0+d,y0+ly);
      }
      return;
   }
   if (t==7) {
      kyofill(2, x0, y0, lx, ly);
      kyofill(5, x0, y0, lx, ly);
      return;
   }
   if (t==3) {
      dd=XFILL1;
      direction=1;
   }
   if (t==1) {
      dd=XFILL1;
      direction=-1;
      x0 += lx;
   }
   if (t==5) {
      dd=XFILL2;
      direction=1;
   }
   if (t==6) {
      dd=XFILL2;
      direction=-1;
      x0 += lx;
   }
   for (d=dd; d<lx + ly; d+=dd) {
      dx1 = (d<lx) ?  direction*d : direction*lx;
      dx2 = (d<ly) ?  0           : direction*(d-ly);
      dy1 = (d<lx) ?  ly          : ly-(d-lx);
      dy2 = (d<ly) ?  ly-d        : 0; 
      kyomove(x0+dx1,y0+dy1);
      kyodraw(x0+dx2,y0+dy2);
   }
}

void kyotext(int direction,int position, int x, int y, char *text) {
   int lettershift=-10, x1, w;
   int dotszin;
   dotszin=dots_per_text(text);

   if(direction==1 || direction==3) {
                 x1 = PAPERLENGTH - y;
                 y  = x +KYO_Y;
                 x  = x1;
   };
   if (direction==0 || direction==1) {
      switch (position) {
         case CENTER:
            x-=dotszin/2;
            y+=font[Font][3]/2 + lettershift;
        break;
         case CENTERLEFT:
			y+=font[Font][3]/2 + lettershift;
            break;
         case CENTERTOP:
            x-=dotszin/2;
            y+=font[Font][3] + lettershift;
            break;
         case CENTERRIGHT:
            x-=dotszin;
            y+=font[Font][3]/2 + lettershift;
            break;
         case CENTERBOTTOM:
            x-=dotszin/2;
            break;
         case TOPLEFT:
            y+=font[Font][3] + lettershift;
            break;
         case TOPRIGHT:
            x-=dotszin;
            y+=font[Font][3] + lettershift;
            break;
         case BOTTOMLEFT:
            break;
         case BOTTOMRIGHT:
            x-=dotszin;
            break;
      }
   } else if (direction==2 || direction==3) {
      switch (position) {
         case 0:
            x+=dotszin/2;
			y-=font[Font][3]/2 + lettershift;
            break;
         case 1:
            y-=font[Font][3]/2 + lettershift;
            break;
         case 2:
            x+=dotszin/2;
            break;
         case 3:
            x+=dotszin;
            y-=font[Font][3]/2 + lettershift;
            break;
         case 4:
            x+=dotszin/2;
            y-=font[Font][3] + lettershift;
            break;
         case 5:
            x+=dotszin;
            break;
         case 6:
            break;
         case 7:
            x+=dotszin;
            y-=font[Font][3] + lettershift;
            break;
         case 8:
            y-=font[Font][3] + lettershift;
            break;
      }
   }
   if (direction==1 || direction==3) w=fprintf(fpout,"FONT %d;\r\n", font[Font][1]);
   w=fprintf(fpout,"MAP %d, %d;\r\nTEXT '%s';\r\n",x, y, text);
   if (w==EOF || ferror(fpout)) error(w,count);
   if(direction==1 || direction == 3) w=fprintf(fpout,"FONT %d;\r\n", font[Font][0]);
}


char *datestring(char *buffer, float date) {
   int jaar, maand, dag, found=FALSE;
   jaar= (int)(date/365.25);
   dag=date-jaar*365-(jaar-1)/4-1;
   while (!found) {
      if (dag<32) {
         maand=1;
         found=TRUE;
         break;
      }
      dag-=31;
      if (dag<29+((jaar%4)==0)) {
         maand=2;
         found=TRUE;
         break;
      }
      dag-=28+((jaar%4)==0);
      if (dag<32) {
         maand=3;
         found=TRUE;
         break;
      }
      dag-=31;
	  if (dag<31) {
         maand=4;
         found=TRUE;
         break;
      }
      dag-=30;
      if (dag<32) {
         maand=5;
         found=TRUE;
         break;
      }
      dag-=31;
      if (dag<31) {
         maand=6;
         found=TRUE;
         break;
      }
      dag-=30;
      if (dag<32) {
         maand=7;
         found=TRUE;
         break;
      }
      dag-=31;
      if (dag<32) {
         maand=8;
         found=TRUE;
         break;
      }
      dag-=31;
	  if (dag<31) {
         maand=9;
         found=TRUE;
         break;
      }
      dag-=30;
      if (dag<32) {
         maand=10;
         found=TRUE;
         break;
      }
      dag-=31;
	  if (dag<31) {
         maand=11;
         found=TRUE;
         break;
      }
      dag-=30;
      if (dag<32) {
         maand=12;
         found=TRUE;
         break;
      }
      maand=13;
      found=TRUE;
   }
   sprintf(buffer,"%d-%d-%d",dag, maand, jaar);
   return buffer;
}

int yearnumber(float datenumber) {
   return (int)(datenumber/365.25);
}

float datenumber(int jaar, int maand, int dag) {
   float date;
   date= dag + (float)jaar*365.0 + 1;
   if (jaar>3)  date+=(jaar-1)/4;
   if (maand>1) date+=31;                   /* feb */
   if (maand>2) {                           /* mrt */
	  if (jaar>0 && jaar%4==0) date+=29;
	  else date+=28;
   }
   if (maand>3) date+=31;                   /* apr */
   if (maand>4) date+=30;                   /* mei */
   if (maand>5) date+=31;                   /* jun */
   if (maand>6) date+=30;                   /* jul */
   if (maand>7) date+=31;                   /* aug */
   if (maand>8) date+=31;                   /* sep */
   if (maand>9) date+=30;                   /* okt */
   if (maand>10) date+=31;                  /* nov */
   if (maand>11) date+=30;                  /* dec */
   return date;
}


void yesno(int flag, char *yesstring, char *nostring) {
   if (flag) {
      printf(" %s  ", nostring);
      HLON;
      printf("%s\r\n", yesstring);
      HLOFF;
   } else {
      putch(' ');
      HLON;
      printf("%s", nostring);
      HLOFF;
      printf("  %s\r\n", yesstring);
   }
}

int isdate(int formatbyte) {
int hi, lo;
/* returns -1 if date, 0-15 for number of decimal places, -2 for other */
   hi=(formatbyte>>4)&7;
   lo=formatbyte&15;
/*
cprintf("\r\n%s %4d : shift4=%d formatbyte=%d hi=%d lo=%d",
__FILE__, __LINE__, formatbyte>>4, formatbyte, hi, lo);
if (getch()==27) farewell();
putch('\r');
putch('\n');
*/
   if (hi==7) {
	  if ( lo==2 ||
		   lo==3 ||
		   lo==4 ||
		  (lo >6 && lo<10)
	  ) return -1;
	  if (lo==1 || lo==15) return 0;
	  return -2;
   }
   return lo;
}


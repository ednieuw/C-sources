#include "include\wmf.h"
#include "plab.h"
void  allocate(void);
int   check_dosname(char *s);
int   check_drive(int drive);
int   check_output_file(char *new_name, char *default_name);
void check_range(float *y1, float *y2, float *y3, float sd, float vc);
void  column_menu();
int   count_char(char *s, char c);
void  down(char c,int n);
void  edit_data(void);
void err0(char *s, int line);
void  extra_text_menu(void);
//void  exit(int exit_code);
int   fgetline(char *line,int maxlen, FILE *fpin);
int   fgetline(char *line,int maxlen, FILE *fpin);
char *find_monthname(char *name, int month, double size_per_month);
void  format_menu(void);
void  frame(int hor,int vert);
char *ftoa(float f);
int getbarsize(int dataline);
int   getdir(char *s);
char *getfile(char *mask);
float getfloat(int *key, int field);
int   getkey(int flag);
char *getmask(char *default_mask);
int   getmaxlegend(void);
int   get_legendlines(void);
int   get_options(void);
void  initialise_arrays(void);
int   isdecimal(char c);
int   isdelimiter(char c);
int   isnumber(char *s);
int   isvoid(char *s);
char *keycopy(char *target,char *source,int maxlen);
void  legend_menu();
void  legend_menu(void);
void  makedate(int *m,int *d,int *y,unsigned total);
char *makefilename(char *full_filename, char *default_path, char *default_ext, int forced_ext_flag);
void  maketime(int *h,int *m,int *s,unsigned total);
float meansd(float *mean,int dataline,int n);
void  minmax_menu(void);
void  no_room(void);
int   numbers_per_line(char *buffer);
void  openin(char *filename);
void  openout(char *filename);
int   parse_line(char *buffer, int dataline);
int   piccopy(char *source, char *target);
int   picsave(char *source, char *target, char *ext);
void  position_menu(void);
void  printstring(char c,int n);
void  range_menu(void);
int   read_data(char *filename, char *infostring, int show);
int   round(double d);
void  startup_message(void);
char *stripwhites(char *s);
char *strleft(char *target,char *source, char *mask);
int   strmenu(int current_pos, int rows, int startrow,int nooflines);
char *strnleft2(char *buffer, char *s1, char c, int n);
int   strpos(char *s,char c,int pos);
int   strpos2(char *s,char c,int pos);
void  write_fln(char *s);
void  yesno(int flag, char *yesstring, char *nostring);
long LINECOLOR(int pos);
long SYMBOLCOLOR(int pos);
//double pow10d(double exponent);
double scientific2(double d, double *base, int *exp);
double val(double mantissa, int exponent);
char ytransform[3][14]={
"Linear",
"Logarithmic",
"Homoscedastic"};

extern char LOGFILEDIR[]; // "c:\\util"
extern char LOGFILE[];    // "c:\\util\\plab.log"
extern char WMFviewer[];  // "c:\\util\\metacomp.exe"

void allocate(void) {
   int i;
   if ((flag=           (char *)calloc( COLUMNS,   sizeof(char) ))==0) no_room();
   if ((first_title=    (char *)calloc( MAX_LEN+1, sizeof(char) ))==0) no_room();
   if ((second_title=   (char *)calloc( MAX_LEN+1, sizeof(char) ))==0) no_room();
   if ((Input_path=     (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* A: */
   if ((Input_ext=      (char *)calloc( 5,         sizeof(char) ))==0) no_room(); /* A: */
   if ((Output_ext=     (char *)calloc( 5,         sizeof(char) ))==0) no_room(); /* A: */
   if ((Output_path=    (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* A: */
   if ((infostring1=     (char *)calloc( 161,       sizeof(char) ))==0) no_room();
   if ((infostring2=     (char *)calloc( 161,       sizeof(char) ))==0) no_room();
   if ((x_legend=       (char *)calloc( MAX_LEN+1, sizeof(char) ))==0) no_room();
   if ((y_legend=       (char *)calloc( MAX_LEN+1, sizeof(char) ))==0) no_room();
   if ((y2_legend=      (char *)calloc( MAX_LEN+1, sizeof(char) ))==0) no_room();
   if ((symbolstring=   (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* 123456789 */
   if ((linetypestring= (char *)calloc( 41,        sizeof(char) ))==0) no_room(); /* 12345678  */
   if ((scolorstring=   (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* 12345678  */
   if ((lcolorstring=   (char *)calloc( 41,        sizeof(char) ))==0) no_room(); /* 12345678  */
   if ((s0colorstring=   (char *)calloc( 81,       sizeof(char) ))==0) no_room(); /* 12345678  */
   if ((l0colorstring=   (char *)calloc( 41,       sizeof(char) ))==0) no_room(); /* 12345678  */
   if ((datapoint_code= (char *)calloc( COLUMNS,   sizeof(char) ))==0) no_room();
   if ((picfile=        (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* C:LINE.PIC */
   if ((fln_filename=   (char *)calloc( 81,        sizeof(char) ))==0) no_room();
   if ((wmffile=        (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* C:LINE.WMF */
   if ((plafile=        (char *)calloc( 81,        sizeof(char) ))==0) no_room(); /* C:LINE.PLA */
   if ((prnfile=        (char *)calloc( 81,        sizeof(char) ))==0) no_room();
   if ((outfile=        (char *)calloc( 81,        sizeof(char) ))==0) no_room();
   if ((buffer=         (char *)calloc( BUFFERLEN+1, sizeof(char) ))==0) no_room();
   if ((startchars=     (char *)calloc( MAXFILES,  sizeof(char) ))==0) no_room();
   if ((name=             (char **)calloc(COLUMNS,            sizeof(charpointer)))==0) no_room(); /* NAME_LEN+1 */
   if ((extra_text=       (char **)calloc(11,                 sizeof(charpointer)))==0) no_room(); /* MAX_LEN+1 */
   if ((filename=         (char **)calloc(MAXFILES,           sizeof(charpointer)))==0) no_room(); /* LENFILENAME+1 */
   if ((range_name=       (char **)calloc(RANGES+1,           sizeof(charpointer)))==0) no_room(); /* MAX_LEN+1 */
   if ((calibratorstring= (char **)calloc(MAXNOOFCALIBRATORS, sizeof(charpointer)))==0) no_room(); /* 12 */
   if ((headerline=       (char **)calloc(HEADERLINES,        sizeof(charpointer)))==0) no_room(); /* MAX_LEN */
   if ((stringline=       (char **)calloc(HEADERLINES+2,      sizeof(charpointer)))==0) no_room(); /* MAX_LEN;  extra lines: x- and y-legend */
   for (i=0; i<COLUMNS; i++)           if ((name[i]=             (char *)calloc( NAME_LEN+1,   sizeof(char)))==0) no_room();
   for (i=0; i<11; i++)                if ((extra_text[i]=       (char *)calloc( MAX_LEN+1,    sizeof(char)))==0) no_room();
   for (i=0; i<MAXFILES; i++)          if ((filename[i]=         (char *)calloc( 41,           sizeof(char)))==0) no_room();
   for (i=0; i<RANGES+1; i++)          if ((range_name[i]=       (char *)calloc( MAX_LEN+1,    sizeof(char)))==0) no_room();
   for (i=0; i<MAXNOOFCALIBRATORS; i++)if ((calibratorstring[i]= (char *)calloc( 12,           sizeof(char)))==0) no_room();
   for (i=0; i<HEADERLINES; i++)       if ((headerline[i]=       (char *)calloc( MAX_LEN+1,    sizeof(char)))==0) no_room();
   for (i=0; i<HEADERLINES+2; i++)     if ((stringline[i]=       (char *)calloc( MAX_LEN+1,    sizeof(char)))==0) no_room();

   if ((Int_array=      (int *)calloc(NOOFINTS+1, sizeof(int)))==0) no_room();
   if ((valid_data=     (int *)calloc(SERIES,     sizeof(int)))==0) no_room();
   if ((range_size=     (int *)calloc(RANGES+1,   sizeof(int)))==0) no_room();
   if ((range_start=    (int *)calloc(RANGES+1,   sizeof(int)))==0) no_room();
   if ((range_startpos= (int *)calloc(RANGES+1,   sizeof(int)))==0) no_room();
   if ((range_type=     (int *)calloc(RANGES+1,   sizeof(int)))==0) no_room();

   if ((Float_array= (float *)calloc(NOOFFLOATS+1, sizeof(float)))==0) no_room();
   if ((intercept=   (float *)calloc(RANGES+1,     sizeof(float)))==0) no_room();
   if ((x=           (float *)calloc( COLUMNS,     sizeof(float)))==0) no_room();
   if ((ny=          (int *)calloc( COLUMNS,     sizeof(int)))==0) no_room();
// y[0][i]       : sd
// y[1][i]       : min
// y[2][i]       : mean
// y[3][i]       : max
// y[4][i]
//   ..
// y[Replica+3]  : individual datapoints
   for (i=0; i<MAXREPLICA+4; i++) if ((y[i]=(float *)calloc( COLUMNS, sizeof(float)))==0) no_room();
   if ((islope=              (double *) calloc(SERIES,             sizeof(double)))==0) no_room();
   if ((meanx=               (double *) calloc(SERIES,             sizeof(double)))==0) no_room();
   if ((meany=               (double *) calloc(SERIES,             sizeof(double)))==0) no_room();
   if ((value_of_calibrator= (double *) calloc(MAXNOOFCALIBRATORS, sizeof(double)))==0) no_room();
}

void no_room(void){
   cprintf("\n\rError: Out of memory\n\r");
   exit(0);
}

int get_options(void) {
int c, n, option, changes=0, newL, newR;
   for (;;) {
      do {
         clrscr();
         cprintf("0 - x-axis transformation: %s\n\r",(Logxflag?"Logarithmic":"Linear"));
         cprintf("1 - y-axis transformation: %s %s\n\r",ytransform[Logyflag],(dual_axis)?ytransform[Logy2flag]:"");
         cprintf("2 - first title: \"%s\"\n\r",  first_title);
         cprintf("3 - second title: \"%s\"\n\r", second_title);
         cprintf("4 - x-axis title: \"%s\"\n\r", x_legend);
         if (!dual_axis) cprintf("5 - y-axis title: \"%s\"\n\r", y_legend);
         else cprintf("5 - y-axis titles: \"%s\" and \"%s\"\n\r", y_legend,y2_legend);
         if (ranges<=SERIES) {
            cprintf("6 - Parallel-line analysis: ");
            if (Statflag==0) HLON
            cprintf(" NO ");
            if (Statflag==0) HLOFF
            if (Statflag==1) HLON
            cprintf(" Y Linear ");
            if (Statflag==1) HLOFF
            if (Statflag==2) HLON
            cprintf(" homoscedastic ");
            if (Statflag==2) HLOFF
/* if (Statflag==3) HLONcprintf(" Y Logit ");if (Statflag==3) HLOFF*/
            cprintf("\r\n");
         }
         cprintf("7 - cap on error-lines: %d\n\r", Cap);
         cprintf("8 - lines connecting points: %s\n\r",(Line?"yes":"no"));
         cprintf("9 - (X= Y) - line: %s\n\r",(Xy?"yes":"no"));
         cprintf("A - Axis: format X- and Y-axis: %s\n\r", (Bare)?"Unformatted":((Xcal/3)?"Closed":"Exploded"));
         if (Color) cprintf("C - Colors off, or edit palette\r\n");
         else cprintf(      "C - Colors on? Change from B/W to colors\r\n");
/*
         cprintf("E - Edit data: ");
            if (Range_check==0) HLON
            cprintf(" No range-check ");
            if (Range_check==0) HLOFF
            if (Range_check==1) HLON
            cprintf(" Skip low ");
            if (Range_check==1) HLOFF
            if (Range_check==2) HLON
            cprintf(" Skip high ");
            if (Range_check==2) HLOFF
            if (Range_check==3) HLON
            cprintf(" Full range-check");
            if (Range_check==3) HLOFF
            cprintf("\r\n");
*/
         cprintf("F - Format of lines: current line-types: %s\n\r", linetypestring);
         cprintf("G - Grid: Horizontal grid: %s; vertical grid: %s\n\r",Hgrid?"YES":"NO",Vgrid?"YES":"NO");
         if (Logyflag)
           cprintf("H - Horizontal line at y=1: %s\n\r",Y1line?"yes":"no");
         cprintf("L - Legends: modify text of legends\n\r");
         cprintf("M - MinMax: ");
              cprintf("Xmin=%s ",(Xmin==MISSING)?"AUTO":ftoa(Xmin));
              cprintf("Xmax=%s ",(Xmax==MISSING)?"AUTO":ftoa(Xmax));
              cprintf("Ymin=%s ",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
              cprintf("Ymax=%s ",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
              if (dual_axis) {
                 cprintf("Y2min=%s ",(Y2min==MISSING)?"AUTO":ftoa(Y2min));
                 cprintf("Y2max=%s\n\r",(Y2max==MISSING)?"AUTO":ftoa(Y2max));
              } else cprintf("\r\n");
         cprintf("P - Position: position of legends%s: %d\n\r", Statflag?"/PLA-results":"",Legendpos);
         cprintf("S - Symbols: modify type/size of symbols\n\r");
         cprintf("T - Text: change text-size (default: %i)\n\r",Textsize);
         cprintf("V - Variability: SD and Variation Coefficient: (current:%.3f and %.1f\%)\n\r",Sd,100*Vc);
         cprintf("X - X-axis: modify origin and length of x-axis\n\r");
         cprintf("Y - Y-axis: modify origin and length of y-axis\n\r");
         cprintf("\n\rRETURN= accept options (Escape= quit). Selected option:  ");
         if ((option=getkey(0))==RETURN || option==EXEC) {
            cprintf("\r\n");
            return changes;
         }
         if (option==ESC || option==CANCEL) return -1;
      } while (strpos("ACEFGHLMPSTVXY0123456789",option,0)==-1 && option!=RIGHT && option!=LEFT);
      changes++;
      switch (option) {
         case 'A':
            format_menu();
            break;
         case 'C':
            clrscr();
            if (Color) {
               cprintf("B - Colors off? Change from %s to B/W\r\n",scolorstring);
               cprintf("P - Palette editing\r\n");
               if ((option=getkey(0))==RETURN || option==EXEC) break;
               if (option=='B') {
                  strcpy(s0colorstring,scolorstring);
                  strcpy(l0colorstring,lcolorstring);
                  strcpy(scolorstring,"0");
                  strcpy(lcolorstring,"0");
                  Color=0;
                  break;
               }
            } else {
               cprintf("C - Colors on? Change from B/W to Color\r\n");
               if ((option=getkey(0))==RETURN || option==EXEC) break;
               strcpy(scolorstring,s0colorstring);
               strcpy(lcolorstring,l0colorstring);
               Color=3;
            }
            colormenu(0);
            break;
         case 'E':
            edit_data();
            calcflag=FALSE;
            break;
         case 'F':
            clrscr();
            cprintf("\n\r\n\rLine-type sequence used: \"%s\" \n\r"
               "0 = solid line (A)\n\r"
               "1 = ---------- (B)\n\r"
               "2 = .......... (C)\n\r"
               "3 = -.-.-.-.-. (D)\n\r"
               "4 = -...-...-. (E)\n\r"
               "5 = double solid line (F)\n\r"
               "6 = double dashed line (G)\n\r"
               "7 = double dotted line (H)\n\r"
               "8 = double dashdotted line(I)\n\r"
               "9 = no line\n\r", linetypestring);
            strcpy(linetypestring, keycopy(buffer, linetypestring, 19));
            for (n=0; n<strlen(linetypestring); n++) {
               linetypestring[n]=toupper(linetypestring[n]);
               if (linetypestring[n]=='O') linetypestring[n]='0';
            }
            colormenu(lcolorstring);
            break;
         case 'G':
            do {
               clrscr();
               cprintf("H - Horizontal grid: ");
               yesno(Hgrid, "Yes", "No");
               cprintf("V - Vertical   grid: ");
               yesno(Vgrid, "Yes", "No");
               cprintf("\RETURN = accept\n\r");
               c=getkey(0);
               if (c=='H') Hgrid=Hgrid?0:1;
               if (c=='V') Vgrid=Vgrid?0:1;
            } while (c!=RETURN && c!=ESC);
            break;
         case 'H':
            Y1line=(Y1line)?0:1;
            break;
         case 'L':
            legend_menu();
            break;
         case 'M':
            minmax_menu();
            break;
         case 'P':
            position_menu();
            break;
         case 'S':
            clrscr();
            cprintf("Characters or Symbols (C/S) default: %c ",Literal?'C':'S');
            c=getkey(0);
            if (c=='C') Literal=TRUE;
            if (c=='S') Literal=FALSE;
            if (c==SPACE) Literal=(Literal)?0:1;
            if (Literal) break;
            cprintf("\n\r\n\rSymbol sequence used: \"%s\" \n\r"
               "0 = open circle\n\r"
               "1 = filled circle\n\r"
               "2 = open square\n\r"
               "3 = filled square\n\r"
               "4 = open triangle\n\r"
               "5 = filled triangle\n\r"
               "6 = open inverted triangle\n\r"
               "7 = filled inverted triangle\n\r"
               "8 = *\n\r"
               "9 = diamond\n\r\n\r"
               "L   = line without symbols\n\r"
               "B/b = vertical bar (B=filled, b=open)\n\r"
               "D   = datasymbols\n\r"
               "R   = rotated datasymbols\n\r\n\r: ",symbolstring);
            strcpy(symbolstring, keycopy(buffer, symbolstring, 19));
            for (n=0; n<strlen(symbolstring); n++) if (symbolstring[n]!='b') symbolstring[n]=toupper(symbolstring[n]);
            cprintf("\n\rsymbolstring:\"%s\"\n\r\n\r\n\rsymbol size (0-400, default %d)\n\r: ",
               symbolstring, Symbol_size);
            gets(buffer);
            if (*buffer) {
               n=atoi(buffer);
               if (n>=0 && n<=400) Symbol_size=n;
            }
            if (strpos(symbolstring,'B',0)>-1 ||
                strpos(symbolstring,'b',0)>-1) {
                cprintf("Bar width (1-400, default:%d, automatic=0): ",Bar_size);
                gets(buffer);
                if (*buffer) {
                   n=atoi(buffer);
                   if (n>=0 && n<=400) Bar_size=n;
                }
            }
            colormenu(scolorstring);
            break;
         case 'T':
            clrscr();
            cprintf("Text-size: (max=%d,default=%d)", 2*TEXTSIZE, Textsize);
            gets(buffer);
            n=atoi(buffer);
            if (n>0 && n<=2*TEXTSIZE) {
               Textsize=n;
               Vsize=Textsize*9;
               Hsize=Textsize*8;
               VsizeWMF=Textsize*WMFVSIZE;
               HsizeWMF=Textsize*WMFHSIZE;
               YSIZE=0.9*Vsize;   /* vertical size of legend block */
               DY=0.4*YSIZE;
            }
            break;
         case 'V':
            clrscr();
            cprintf("minimal Variation Coefficient (%.3f) ",Vc);
            gets(buffer);
            if (*buffer) Vc=atof(buffer);
            if (Vc>0.5) Vc/=100;
//          if (Vc<0.001) Statflag=1;
            cprintf("minimal Standard Deviation (%.3f) ",Sd);
            gets(buffer);
            if (*buffer) Sd=atof(buffer);
            fpin=fopen(prnfile,"rb");
            columns=read_data(prnfile,0,Show);
            fclose(fpin);
            calcflag=FALSE;
            prepare_pic();
            break;
         case 'X':

            cprintf("\n\rOrigin of x-axis (max=%d,default=%d)",X_LENGTH-X_length, X_start);
            gets(buffer);
            if (strlen(buffer)) {
            n=atoi(buffer);
            if (n>=0 && n<=X_LENGTH-100) X_start=n;
            else X_start=0;
            }
            cprintf("\n\rLength of x-axis (max=%d,default=%d)",X_LENGTH-X_start, X_length);
            gets(buffer);
            if (strlen(buffer)) {
            n=atoi(buffer);
            if (n>1 && n<=X_LENGTH-X_start) X_length=n;
            else X_length=X_LENGTH-X_start;
            }
            cprintf("\n\rLength of extra space on x-axis (max=%d,default=%d)",X_LENGTH/2, Extra_rim);
            gets(buffer);
            if (strlen(buffer)) {
            n=atoi(buffer);
            if (n>=0 && n<=X_LENGTH/2) Extra_rim=n;
            }
            break;
         case 'Y':
            cprintf("\n\rOrigin of y-axis (max=%d,default=%d)",Y_LENGTH-100, Y_start);
            gets(buffer);
            if (strlen(buffer)) {
            n=atoi(buffer);
            if (n>=0 && n<=Y_LENGTH-100) Y_start=n;
            else Y_start=0;
            }
            cprintf("\n\rLength of y-axis (max=%d,default=%d)",Y_LENGTH-Y_start, y_length);
            gets(buffer);
            if (strlen(buffer)) {
            n=atoi(buffer);
            if (n>1 && n<=Y_LENGTH-Y_start) Y_length=n;
            else Y_length=Y_LENGTH-Y_start;
            }
            break;
         case '0':
            Logxflag=(Logxflag)?0:1;
            if (Logxflag && Xmin<=0) Xmin=MISSING;
            calcflag=0;
            break;
         case '1':
            if (dual_axis) {
               clrscr();
               cprintf("0=Linear\r\n1=Logarthimic\r\n2=homoscedastic\r\n\r\n");
               newL=logyflag+1;
               newR=logy2flag+1;
               if (newL>2) newL=0;
               if (newR>2) newR=0;
               cprintf("L=left axis %d to %d\r\nR=swap right axis %d to %d\r\n",Logyflag,newL,Logy2flag,newR);
               c=toupper(getch());
               if (c=='L') Logyflag=newL;
               if (c=='R') Logy2flag=newR;
            } else {
               Logyflag++;
               if (Logyflag>2) Logyflag=0;
            }
            if (Logyflag==1 && Ymin<=0) Ymin=MISSING;
            if (Logy2flag==1 && Y2min<=0) Y2min=MISSING;
            calcflag=0;
            break;
         case '2':
            clrscr();
            cprintf("First title \"%s\": ",first_title);
            strcpy(first_title,keycopy(buffer, first_title, MAX_LEN));
            break;
         case '3':
            clrscr();
            cprintf("\n\rSecond title \"%s\": ",second_title);
            strcpy(second_title,keycopy(buffer, second_title, MAX_LEN));
            break;
         case '4':
            clrscr();
            cprintf("\n\rLegend x-axis \"%s\": ",x_legend);
            strcpy(x_legend, keycopy(buffer, x_legend, MAX_LEN));
            break;
         case '5':
            clrscr();
            cprintf("\n\rLegend y-axis \"%s\": ",y_legend);
            strcpy(y_legend, keycopy(buffer, y_legend, MAX_LEN));
            if (dual_axis) {
               cprintf("<=>\"%s\": ",y2_legend);
               strcpy(y2_legend, keycopy(buffer, y2_legend, MAX_LEN));
            }
            break;
/*
         case '6':
            clrscr();
            cprintf("\n\rnumber of datacolumns (max=%d,default=%d)",DATA_COLUMNS, data_columns);
            gets(buffer);
            if (*buffer) {
               n=atoi(buffer);
               if (n>1 && n<=DATA_COLUMNS) Data_columns=n;
            }
            data_columns=Data_columns;
            break;
*/
         case '6':
         case RIGHT:
            if (!Statflag) Statflag=1;
            else if (Statflag==1) Statflag=2;
            else if (Statflag==2) Statflag=0;
            if (Statflag) Line=FALSE;
            else Line=TRUE;
//            if (Statflag==2 && Vc<=0.001) Statflag=1;
            calcflag=FALSE;
            if (Statflag) {
//                     Logyflag=0;
                     Ylo=MISSING;
                     Yhi=MISSING;
            }
            break;
         case LEFT:
            if (!Statflag) Statflag=2;
            else if (Statflag==1) Statflag=0;
            else if (Statflag==2) Statflag=1;
            if (Statflag) Line=FALSE;
            else Line=TRUE;
//            if (Statflag==2 && Vc<=0.001) Statflag=1;
            calcflag=FALSE;
            if (Statflag) {
//                     Logyflag=0;
                     Ylo=MISSING;
                     Yhi=MISSING;
            }
            break;
         case '7':
            clrscr();
            cprintf("\n\r0      = no cap on error-bars\n\r"
                     "0 - 400= length of cap-line (default:%d): ",Cap);
            gets(buffer);
            if (*buffer) Cap=atof(buffer);
            if (Cap<0) Cap=0;
            if (Cap>400) Cap=400;
            break;
         case '8':
            Line=(Line)?0:1;
            break;
         case '9':
            Xy=(Xy)?0:1;
            break;
      }
   }
}

void minmax_menu(void) {
   clrscr();
   cprintf("M - MinMax: ");
   cprintf("Xmin=%s ",(Xmin==MISSING)?"AUTO":ftoa(Xmin));
   cprintf("Xmax=%s ",(Xmax==MISSING)?"AUTO":ftoa(Xmax));
   cprintf("Ymin=%s ",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
   cprintf("Ymax=%s\n\r\n\r",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
   if (dual_axis) {
      cprintf("Y2min=%s ",(Y2min==MISSING)?"AUTO":ftoa(Y2min));
      cprintf("Y2max=%s\n\r\n\r",(Y2max==MISSING)?"AUTO":ftoa(Y2max));
   }
   cprintf("'?'= AUTOMATIC\n\r\n\r");
   cprintf("Xmin=lowest value x-axis (%s) ",(Xmin==MISSING)?"AUTO":ftoa(Xmin));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Xmin=MISSING;
   else if (*buffer) Xmin=atof(buffer);
   cprintf("\n\rXmax=highest value x-axis (%s) ",(Xmax==MISSING)?"AUTO":ftoa(Xmax));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Xmax=MISSING;
   else if (*buffer) Xmax=atof(buffer);
   cprintf("\n\rYmin=lowest value y-axis (%s) ",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Ymin=MISSING;
   else if (*buffer) Ymin=atof(buffer);
   cprintf("\n\rYmax=highest value y-axis (%s) ",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Ymax=MISSING;
   else if (*buffer) Ymax=atof(buffer);
   if (dual_axis) {
      cprintf("\n\rY2min=lowest value y-axis (%s) ",(Y2min==MISSING)?"AUTO":ftoa(Y2min));
      gets(buffer);
      if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Y2min=MISSING;
      else if (*buffer) Y2min=atof(buffer);
      cprintf("\n\rY2max=highest value y-axis (%s) ",(Y2max==MISSING)?"AUTO":ftoa(Y2max));
      gets(buffer);
      if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Y2max=MISSING;
      else if (*buffer) Y2max=atof(buffer);
   }
}

void edit_data(void) {
   int bar, r, column, option, ok, c, i;
   float value;
   clrscr();
   cprintf("A=Automatic data-check for Parallel Line Analysis\r\n"
           "M=Manual data-check\r\n"
           "default: %c ",Range_check?'A':'M');
   c=getkey(0);
   if (c==ESC) return;
   if (c=='A') Range_check=3;
   if (c=='M') Range_check=0;
   cprintf("%c \r\n\r\n",(Range_check?'A':'M'));
   if (Range_check) {
       cprintf("0=Manual data-check\r\n"
               "1=skip low data  (currently: y-values < %.4f)\r\n"
               "2=skip high data (currently: y-values > %.4f)\r\n"
               "3=skip data with low or high y-values\r\n"
               "default: %d \r\n", Ylo, Yhi, Range_check);
       c=getkey(0);
       if (c==ESC) return;
       if (c>='0' && c<='3') Range_check=c-'0';
   }
   if (Range_check%2) {
      cprintf("\r\nLow cut-off value (%.4f): ",Ylo);
      value=getfloat(&i,14);
      if (value>MISSING) Ylo=value;
   }
   if (Range_check/2) {
      cprintf("\r\nHigh cut-off value (%.4f): ",Yhi);
      value=getfloat(&i,14);
      if (value>MISSING) Yhi=value;
   }
   for (r= 0, column=0;r < ranges; r++) {
      do {
         ok=TRUE;
         clrscr();
         if (ranges>1) cprintf("\n\rrange %d: \"%s\"\r\n\r\n", r+1, range_name[r]);
         for (bar=0; bar<range_size[r]; bar++) {
            if (Show) putch(bar+'A');
            if (Range_check%2 && y[2][column+bar]<Ylo) cprintf(" < ");
            else if (Range_check/2 && y[2][column+bar]>Yhi) cprintf(" > " );
            else if (strpos("(){}[]*", datapoint_code[column+bar],0)==-1) cprintf("   ");
            else cprintf(" %c ",datapoint_code[column+bar]);
            cprintf("%9.2lf %9.2lf %9.2lf %9.2lf\r\n", x[column+bar], y[1][column+bar], y[2][column+bar], y[3][column+bar]);
         }
         cprintf("\r\nRETURN to accept, codeletter 'A' to '%c' to edit: ",'A'+range_size[r]-1);
         option=getkey(0);
         if (option==ESC) return;
         if (option==RETURN || option < 'A') continue;
         option-='A';
         if (option>range_size[r]) continue;
         i=column+option;
         ok=FALSE;
         c=datapoint_code[i];
         if (isdigit(c)) c='@';
         clrscr();
         cprintf("datapoint-code: %c:\r\n\r\nX  =%9.2lf\r\nY1 =%9.2lf\r\nY2 =%9.2lf\r\nY3 =%9.2lf\r\n", c, x[i], y[1][i], y[2][i], y[3][i]);
         cprintf("\r\n\r\n* = %s this datapoint\r\n"
                         "[ = %s this datapoint during PLA-analysis\r\n"
                         "X = change x-value\r\n"
                         "Y = change y-values\r\n"
                         "press RETURN to continue: ", ((c=='*')?"re-include":"exclude"), ((c=='[')?"re-include":"exclude"));
         option=getkey(0);
         if (option==ESC) return;
         switch (option) {
            case '*':
               datapoint_code[i]=(c=='*')?' ':'*';
               break;
            case '[':
               datapoint_code[i]=(c=='[')?' ':'[';
               break;
            case 'X':
               cprintf("X (%.3f) ", x[i]);
               gets(buffer);
               if (*buffer) x[i]=atof(buffer);
               break;
            case 'Y':
               cprintf("Y1 (%.3f) ", y[1][i]);
               gets(buffer);
               if (*buffer) y[1][i]=atof(buffer);
               cprintf("Y2 (%.3f) ", y[2][i]);
               gets(buffer);
               if (*buffer) y[2][i]=atof(buffer);
               cprintf("Y3 (%.3f) ", y[3][i]);
               gets(buffer);
               if (*buffer) y[3][i]=atof(buffer);
               break;
            case RETURN:
               ok=TRUE;
         }
      } while (!ok);
      column+=range_size[r];
   }
}
void legend_menu(void) {
   int option;
   clrscr();
   cprintf("1 - Show/modify legends of ranges\n\r");
   cprintf("2 - Show/modify extra text\n\r");
   cprintf("\n\r\n\rEscape or RETURN= previous menu\n\r"
              "Choose 1 or 2:  \n\r");
   option=getkey(0);
   switch (option) {
      case '1':
         range_menu();
         break;
      case '2':
         extra_text_menu();
         break;
   }
}

void range_menu(void) {
   int t, response;
   for (;;) {
      clrscr();
      HLON
      cprintf("Current range-names:\n\r\n\r");
      HLOFF
      for (t=0;t<ranges;t++) cprintf("\n\r%d=%-16s",t+1,range_name[t]);
      cprintf("\n\r\n\rRETURN= accept\n\rModify which legend (1-%d) : ",ranges);
      do {
         response=getkey(0);
         if (response==ESC || response==CANCEL) return;
         t=response-'1';
      } while (response!=RETURN && (t<0 || t>=ranges));
      if (response==RETURN) break;
      cprintf(" %c\n\r%s: ",response,range_name[t]);
      keycopy(buffer,range_name[t],MAX_LEN);
      if (isvoid(buffer)) range_name[t][0]='\0';
      else strcpy(range_name[t],buffer);
   }
}
void format_menu(void) {
   int i;
   clrscr();
   cprintf("Formatted/Unformatted (F/U), default: %c=%s ", (Bare?'U':'F'),(Bare?"Unformatted":"Formatted"));
   i=toupper(getch());
   if (i=='U') Bare=TRUE;
   if (i=='F') Bare=FALSE;
   if (Bare) {
      Xcal=0;
      Ycal=0;
      return;
    }
   clrscr();
   cprintf("X-axis:\n\r\n\r"
          "0=Numerical format\n\r"
          "1=Exponential format\n\r"
          "2=Month-format\n\r"
          "Default: %i ",Xformat);
   i=toupper(getch());
   if (i!=RETURN && i>='0' && i<='2') Xformat=i-'0';
   clrscr();
   cprintf("Y-axis:\n\r\n\r"
          "0=Numerical format\n\r"
          "1=Exponential format\n\r"
          "Default: %i ",Yformat);
   i=getch();
   if (i!=RETURN && i>='0' && i<='1') Yformat=i-'0';
   clrscr();
   cprintf("Closed or Exploded (C/E, default: %c) ",(Xcal/3)?'C':'E');
   i=toupper(getch());
   if (i=='C')        Xcal=3;
   else if (i=='E')   Xcal=0;
   else if (i==SPACE) Xcal=(Xcal/3)?0:3;
   Ycal=Xcal;
/*
   cprintf("\n\r\n\r0,1,2: \"exploded\"\n\r\n\r"
            "0 - Tick marks on both sides of the figure\n\r"
            "1 - Tick marks at left, only a line at right\n\r"
            "2 - Tick marks at left, empty at right\n\r\n\r"
            "3,4,5: \"closed\"\n\r\n\r"
            "3 - Tick marks on both sides of the figure\n\r"
            "4 - Tick marks at left, only a line at right\n\r"
            "5 - Tick marks at left, empty at right\n\r\n\r"
            "Default: %i ",Xcal);
   i=toupper(getch());
   if (i!=RETURN && i>='0' && i<'6') Xcal=i-'0';
   if (Xformat==2) Logxflag=0;
   cprintf("\n\r\n\r0,1,2: \"exploded\"\n\r\n\r"
            "0 - Tick marks bottom and top\n\r"
            "1 - Tick marks at bottom axis, only a line at top of figure\n\r"
            "2 - Tick marks at bottom axis, empty at top of figure\n\r\n\r"
            "3,4,5: \"closed\"\n\r\n\r"
            "3 - Tick marks bottom and top\n\r"
            "4 - Tick marks at bottom axis, only a line at top of figure\n\r"
            "5 - Tick marks at bottom axis, empty at top of figure\n\r\n\r"
            "Default: %i ",Ycal);
   i=toupper(getch());
   if (i!=RETURN && i>='0' && i<'6') Ycal=i-'0';
*/
}
void position_menu(void) {
   int r;
   clrscr();
   cprintf("Legend at the indicated position (4=no legend, default:%d)\n\r\n\r"
               "%c%c%c%c%c%c%c\n\r"
               "%c3   2%c1\n\r"
               "%c     %c\n\r"
               "%c     %c\n\r"
               "%c%c%c%c%c%c%c\n\r"
               "0\n\r\n\r",Legendpos,218,196,196,196,196,196,191,
                                 179,179,179,179,179,179,
                                 192,196,196,196,196,196,217);
   r=getch();
   if (r>='0' && r<'5') Legendpos=r-'0';
   if (r=='4') {
      Legendpos=0;
      Legend=0;
   } else if (!Legend) Legend=LEGEND;
   if (Legendpos==0 && Legend>0) {
      cprintf("\n\r\n\rLegends printed over %d column(s) ",Legend);
      r=getch();
      if (r>='0' && r<'9') Legend=r-'0';
   }
   if (Statflag) cprintf("\r\nPrint PLA-results in figure: RETURN=%s SPACE=%s\r\n",(Showstat)?"YES":"NO",(Showstat)?"NO":"YES");
   if (getch()!=RETURN) Showstat=(Showstat)?0:1;
}

void extra_text_menu(void) {
   int t, response,i;
   char buffer[MAX_LEN+1];
   for (;;) {
      clrscr();
      cprintf("\Extra text at the indicated positions:\n\r\n\r"
               "0         1\n\r"
               "  2  3  4\n\r\n\r"
               "  5  6  7\n\r"
               "8         9\n\r\n\r"
               "F=free-located text: x=%d y=%d ",Xet, Yet);
      for (t=0;t<11;t++) if (extra_text[t][0]) cprintf("\n\rextra text %d \"%s\"",t,extra_text[t]);
      cprintf("\n\r\n\rRETURN= accept\n\rModify/add which text (0 - 9,F) : ");
      do {
         response=getkey(0);
         t=response-'0';
      } while (response!=RETURN && (t<0 || t>9) && response!='F');
      if (response==RETURN) break;
      if (response=='F') {
        t=10;
        cprintf("\r\nGive x,y coordinates of center of text\r\nLower-left: x=0, y=0\r\nUpper-right=%d %d\r\nnx=%d: (0-%d) ",X_LENGTH,Y_LENGTH,Xet,X_LENGTH);
        gets(buffer);
        i=(*buffer>='0' && *buffer<='9')?atoi(buffer):-1;
        if (i>=0 && i<X_LENGTH) Xet=i;
        cprintf("\r\ny=%d: (0-%d) ",Yet,Y_LENGTH);
        gets(buffer);
        i=(*buffer>='0' && *buffer<='9')?atoi(buffer):-1;
        if (i>=0 && i<Y_LENGTH) Yet=i;
      }
      cprintf("\r\n(%c) %s: ",response,extra_text[t]);
      keycopy(buffer,extra_text[t],MAX_LEN);
      if (isvoid(buffer)) extra_text[t][0]='\0';
      else strcpy(extra_text[t],buffer);
   }
}

int isvoid(char *s) {
   int i;
   for (i=0;s[i];i++) if (!isspace(s[i])) return 0;
   return 1;
}

void picextra_text(int x_origin, int y_origin, int lx, int ly) {
   int i, x, y, position;
   for (i=0; i<11; i++) {
      if (extra_text[i][0]) {
         switch (i) {
            case 0:
               position=TOPLEFT;
               x=X_ORIGIN;
               y=y_length;
               break;
            case 1:
               position=TOPRIGHT;
               x=X_ORIGIN+X_length;
               y=y_length;
               break;
            case 2:
               position=TOPLEFT;
               x=x_origin+12;
               y=y_origin+ly-12-(Xcal/3)*RIM;
               break;
            case 3:
               position=CENTERTOP;
               x=x_origin + lx/2;
               y=y_origin+ly-12-(Xcal/3)*RIM;
               break;
            case 4:
               position=TOPRIGHT;
               x=x_origin+lx-12;
               y=y_origin+ly-12-(Xcal/3)*RIM;
               break;
            case 5:
               position=BOTTOMLEFT;
               x=x_origin+12;
               y=y_origin+12;
               break;
            case 6:
               position=CENTERBOTTOM;
               x=x_origin + lx/2;
               y=y_origin+12;
               break;
            case 7:
               position=BOTTOMRIGHT;
               x=x_origin+lx-12;
               y=y_origin+12;
               break;
            case 8:
               position=BOTTOMLEFT;
               x=X_ORIGIN;
               y=picbottomline(1);
               break;
            case 9:
               position=BOTTOMRIGHT;
               x=X_length;
               y=Y_ORIGIN;
               break;
            case 10:
               position=CENTER;
               x=Xet;
               y=Yet;
               break;
        }
         picmove(x,y);
         pictext(0,position, extra_text[i]);
      }
   }
}

int getkey(int flag) {
int c;
   result=1;
   c=getch();
   if (c==SPECIAL_KEY) {
      c=256+getch();
      if (c>F5 && c<=F10) return c;
   } else {
      if (c==ESC) result=0;
      if (c==CTRL_C) exit(-1);
      if (!flag) c=toupper(c);
   }
   return c;
}

int read_data(char *s, char *infostring, int show) {
   int non_whites, pos, line=0, column=0, n, r, maxlen=BUFFERLEN, i, ii;
   int rangeline, dataline=-1;
   int stringlines=0;
   int ranges_read=0;
   struct ftime ft;
   Dword rgbcolor;
   char buffer2[BUFFERLEN], *pointer;
   int type, item, subitem;
   unsigned int opcode;
   float f;
   if ((fpin=fopen(s,"rb")) == NULL) return 0;
   if (fgetline(buffer2, maxlen, fpin)==EOF) {
      fclose(fpin);
      return 0;
   }
   if (infostring) {
   getftime(fileno(fpin),&ft);
   sprintf(infostring,"%s;%s;%s %02u-%02u-%02u",
      VERSION,__DATE__,s,
      ft.ft_day,
      ft.ft_month,
      ft.ft_year+80);
   }
   if (strcmp(buffer2,FLNCODE)==0) {
      if (fgetline(buffer2, maxlen, fpin)==EOF) {
         fclose(fpin);
         return 0;
      }
      if (show) cprintf("\r\n\r\nExisting format found in %s\r\nRETURN = use pre-saved format\r\nAny other key to clear formats to default values ",s);
      if (!show || getch()==RETURN) do {
         pos=strpos(buffer2,';',0);
         if (pos>5) buffer2[pos]='\0';
         pointer=buffer2;
         if (*pointer=='\\') pointer++;
         else break;
         if (*pointer=='/') pointer++;
         else break;
         if (strcmp(pointer,ENDCODE)==0) break;
         opcode=atoi(pointer);
         type=opcode/1000;
         item=(opcode%1000)/100;
         subitem=opcode%100;
         pointer+=5;
         switch (type) {
            case STRING_ARRAY:
               switch (item) {
               case DA_extra_text:
                  if (subitem>=0 && subitem<11) strcpy(extra_text[subitem], pointer);
                  break;
            }
            break;
            case SINGLE_STRING:
               switch (subitem) {
                  case DS_linetypestring:
                     strncpy(linetypestring, pointer, 40);
                     linetypestring[40]='\0';
                     break;
                  case DS_symbolstring:
                     strncpy(symbolstring, pointer, 80);
                     symbolstring[80]='\0';
                     break;
                  case DS_lcolorstring:
                     strncpy(lcolorstring, pointer, 40);
                     lcolorstring[40]='\0';
                     break;
                  case DS_scolorstring:
                     strncpy(scolorstring, pointer, 80);
                     scolorstring[80]='\0';
                     break;
                  case DS_input_path:
                     strncpy(Input_path, pointer, 80);
                     Input_path[240]='\0';
                     break;

                  case DS_input_ext:
                     strncpy(Input_ext, pointer, 4);
                     Input_ext[4]='\0';
                     break;
                  case DS_output_ext:
                     strncpy(Output_ext, pointer, 4);
                     Output_ext[4]='\0';
                     break;
                  case DS_output_path:
                     strncpy(Output_path, pointer, 80);
                     Output_path[240]='\0';
                     break;
                  case DS_logfiledir:
                     strncpy(LOGFILEDIR, pointer, 240);
                     LOGFILEDIR[240]='\0';
                     break;
                  case DS_logfile:
                     strncpy(LOGFILE, pointer, 240);
                     LOGFILE[240]='\0';
                     break;
                  case DS_WMFviewer:
                     strncpy(WMFviewer, pointer, 240);
                     WMFviewer[240]='\0';
                     break;
               }
               break;
            case INTEG:
               if (subitem>=0 && subitem<NOOFINTS) {
                  if (!
                    ((subitem==32 && Show>-1) ||
                     (subitem==37 && Outfiles>-1))
                    ) Int_array[subitem]=atoi(pointer);
               }
               break;
            case RGB_ARRAY:
               if (subitem>=0 && subitem<28) {
                  sscanf(pointer,"%8lX",&rgbcolor);
                  rgb[subitem]=rgbcolor;
               }
               break;
            case FLOATING:
               if (subitem>=0 && subitem<NOOFFLOATS) {
                  f=atof(pointer);
                  Float_array[subitem]=(f<=-999999.0)?MISSING:f;
               }
         }
      } while (fgetline(buffer2, maxlen, fpin)!=EOF);
      if (Show) {
         putch('\r');
         putch('\n');
      }
   } else {
      fclose(fpin);
      fpin=fopen(s,"rb");
   }
   if (!Color) {
      strcpy(scolorstring,"0");
      strcpy(lcolorstring,"0");
//      TextColor=rgb[27];
//      BrushColor=rgb[26];
      TextColor=0;
      BrushColor=0;
   }
   headerlines=0;
   for (r=0;r<RANGES;r++) range_size[r]=0;
   ymin=-MISSING;
   ymax=MISSING;
   do {
      line ++;
      if (line%10==1 && Show) putch('.');
      if ((non_whites=fgetline(buffer,maxlen,fpin))<1) continue;
      data_columns=parse_line(buffer, column);
      if (data_columns>0) {
         if (stringlines && dataline==-1) ranges_read=1;
         if (ranges_read) range_size[ranges_read-1]++;
         dataline=1;
         column++;
      }
      if (column>=COLUMNS) break;
      if (data_columns==0 && stringlines<HEADERLINES+2 && *buffer!='\0') {
         strcpy(stringline[stringlines],buffer);
         if (dataline==-1) headerlines++;
         if (dataline) stringlines++;
         if (dataline > 0) {
            dataline=0;
            range_size[ranges_read]=0;
            range_start[ranges_read]=column;
            ranges_read++;
         }
      }
      if (ranges_read>=RANGES){
         ranges_read=RANGES;
         break;
      }
      if (stringlines>=HEADERLINES+2){
         stringlines=HEADERLINES+2;
         headerlines--;
         break;
      }
   } while (non_whites!=EOF && column<COLUMNS && ranges_read<RANGES && stringlines<HEADERLINES+2);
   fclose(fpin);
   ranges=ranges_read;
   *x_legend='\0';
   *y_legend='\0';
   *y2_legend='\0';
   if (ranges>1) headerlines--;
   if (dual_axis) headerlines--;
   rangeline=headerlines;
   for (n=0;n<stringlines;n++) stripwhites(stringline[n]);
   if (headerlines) {
      headerlines --;
      if (!*x_legend) strcpy(x_legend,stringline[headerlines]);
   } else if (!*x_legend) strcpy(x_legend,X_LEGEND);
   if (headerlines) {
      headerlines --;
      if (stringline[headerlines][0]=='!') strcpy(y2_legend,stringline[headerlines]);
      else strcpy(y_legend,stringline[headerlines]);
   }
   if (headerlines) {
      headerlines --;
      if (stringline[headerlines][0]=='!') strcpy(y2_legend,stringline[headerlines]);
      else if (!*y_legend) strcpy(y_legend,stringline[headerlines]);
      else headerlines++;
   }
   if (!*y_legend) strcpy(y_legend,Y_LEGEND);
   if (dual_axis && !*y2_legend) strcpy(y2_legend,Y_LEGEND);
   if (ranges>1) for (r=0,n=rangeline;n<stringlines;r++,n++) {
      if (stringline[n][0]=='!') {
         dual_axis=1;
         range_type[r]=1;
      } else range_type[r]=0;
      strcpy(range_name[r],stringline[n]);
      if (r==0) {
         pos=strpos(range_name[r],'\\',0);
         if (pos>-1 && strlen(range_name[r])>pos+1) {
            units=atof(range_name[r]+pos+1);
            if (units==0.0) units=1.0;
         } else units=1.0;
if (Trace) printf("%s %4d : units=%.3f\n",__FILE__, __LINE__,units);
      }
   }
   if (headerlines) for (n=0;n<headerlines;n++) {
      strcpy(headerline[n],stringline[n]);
      if (n==0) if (!*first_title) strcpy (first_title,headerline[0]);
      if (n==1) if (!*second_title) strcpy (second_title,headerline[1]);
   }

   if (Trace && !infostring) {
      printf("\r\n%s Readmode=%d\n",prnfile,Readmode);
      if (*first_title) printf("%s\n",first_title);
      if (*second_title) printf("%s\n",second_title);
      for (i=0; i<column; i++) {
            printf("%2d ",i);
            for (ii=4; ii<ny[i]+4; ii++) printf(" %9.3f",y[ii][i]);
            printf("\n");
      }
      printf("\n");
      for (i=0; i<column; i++) {
            printf("%2d n=%2d x=%12.3f sd=%12.3f %12.3f %12.3f %12.3f\n",
               i, ny[i], x[i], y[0][i], y[1][i], y[2][i], y[3][i]);
      }
   }
   return column;
}

int parse_line(char *buffer, int dataline) {
   int data_columns=-1, textstring=0, digits=0, pos, pos1, pos2, p, n, i, ii;
   char value[20];
   float digit[DATA_COLUMNS], tempdigit, min, max, s;
   char c;
   digits=0;
   if (!Data_columns) data_columns=numbers_per_line(buffer);
   else data_columns=Data_columns;
   if (data_columns>DATA_COLUMNS) data_columns=DATA_COLUMNS;
   if (!data_columns) return 0;
   for (n=data_columns-1,pos2=strlen(buffer)-1;n>=0 && !textstring;n--) {
      do {
         *value='\0';
         c=buffer[pos2];
         if ( !(isdelimiter(c) || isdigit(c))) textstring=TRUE;
         if (isdelimiter(c) ) pos2--;
      } while (!(textstring) && isspace(c) && pos2>-1 );
      if (textstring) return 0;
      pos1=pos2;
      while (pos1>0 && isdecimal(c=buffer[pos1])) pos1--;
      if (isdelimiter(c) || pos1==0) {
         digits ++;
         for (pos=pos1;!isdecimal(buffer[pos]) && pos<pos2+1;pos++)
            ;
         for (p=0;pos<pos2+1;p++,pos++)
            if (p<20) value[p]=buffer[pos];
            else {
               cprintf("\n\rERROR in line %s: number too long: %p\n\r",
                  buffer,p);
               exit(-1);
            }
         value[p]='\0';
         if (!(isnumber(value))) textstring=TRUE;
         else digit[data_columns-digits]= (float)atof(value);
      } else textstring=TRUE;
      pos2=pos1;
      if (textstring) return 0;
   }
   if (data_columns<2) return 0;
   datapoint_code[dataline]=*buffer;
   for (pos1=0;pos1<MAX_LEN && !buffer[pos1];pos1++)
      ;
   if (buffer[pos1]==QUOTE) {
      pos1++;
      for (pos2=0; pos2<NAME_LEN && buffer[pos1+pos2]!=QUOTE && buffer[pos1+pos2];pos2++) name[dataline][pos2]=buffer[pos1+pos2];
      name[dataline][pos2+1]='\0';
   } else {
      for (pos2=0; pos2<NAME_LEN && buffer[pos1+pos2]!=SPACE && buffer[pos1+pos2];pos2++) name[dataline][pos2]=buffer[pos1+pos2];
      name[dataline][pos2+1]='\0';
    }
   x[dataline]=digit[0];
   if (Trace) printf("%s %4d : s=\"%s\"\n",__FILE__, __LINE__,buffer);
   switch(data_columns) {
      case 2:
         ny[dataline]=1;
         y[0][dataline]=0;
         y[1][dataline]=digit[1];
         y[2][dataline]=digit[1];
         y[3][dataline]=digit[1];
         y[4][dataline]=digit[1];
         y[5][dataline]=digit[1];
         y[6][dataline]=digit[1];
         break;
      case 3:
         ny[dataline]=2;
         if (!Readmode) {
            y[0][dataline]=digit[2];
            y[1][dataline]=digit[1]-digit[2];
            y[2][dataline]=digit[1];
            y[3][dataline]=digit[1]+digit[2];
            y[4][dataline]=y[1][dataline];
            y[5][dataline]=y[2][dataline];
            y[6][dataline]=y[3][dataline];
         } else {
            if (digit[1]>digit[2]) {
               tempdigit=digit[1];
               digit[1]=digit[2];
               digit[2]=tempdigit;
            }
            y[0][dataline]=(digit[2]-digit[1])/sqrt(2);
            y[1][dataline]=y[4][dataline]=digit[1];
            y[2][dataline]=y[5][dataline]=(digit[1]+digit[2])/2;
            y[3][dataline]=y[6][dataline]=digit[2];
/*
            if (Statflag==0) y[2][dataline]=(digit[1]+digit[2])/2;
            else y[2][dataline]=getmeany(digit[1],digit[2],Sd,Vc);
*/
         }
         break;
      case 4:
         if (!Readmode) {
            ny[dataline]=2;
            y[4][dataline]=digit[1];
            y[5][dataline]=digit[2];
            y[6][dataline]=digit[3];
            if (digit[1]>digit[3]) {
               tempdigit=digit[1];
               digit[1]=digit[3];
               digit[3]=tempdigit;
            }
            y[1][dataline]=digit[1];
            y[2][dataline]=digit[2];
            y[3][dataline]=digit[3];
            y[2][dataline]=meansd(&(y[0][dataline]),dataline,ny[dataline]);
        } else if (Readmode==1) {
            ny[dataline]=3;
            min=digit[1];
            max=digit[1];
            for (i=2; i<4; i++) {
               if (min>digit[i]) {
                  min=digit[i];
               }
               if (max<digit[i]) {
                  max=digit[i];
               }
            }
            y[1][dataline]=min;
            y[3][dataline]=max;
            for (i=1, ii=4; i<4; i++) {
               y[ii++][dataline]=digit[i];
            }
            y[2][dataline]=meansd(&(y[0][dataline]),dataline,ny[dataline]);
         } else {
            ny[dataline]=(digit[3]<MAXREPLICA)?digit[3]:MAXREPLICA;
            s=(ny[dataline]%2)?digit[2]*sqrt((double)ny[dataline]/((double)ny[dataline]-1.0)):digit[2];
if (Trace) cprintf("%s %4d : s=%.4f\n",__FILE__, __LINE__,s);
            n=ny[dataline]/2;
            y[0][dataline]=digit[2];
            y[2][dataline]=digit[1];
            y[1][dataline]=digit[1]-s;
            y[3][dataline]=digit[1]+s;
            for (i=4; i<n+4; i++) {
               if (i<MAXREPLICA+4) y[i][dataline]=digit[1]-s;
               else {
                  printf("ERROR at line %4d: i=%d\n",__LINE__,i);
                  exit(-1);
               }
            }
            for ( ; i<2*n+4; i++) {
               if (i<MAXREPLICA+4) y[i][dataline]=digit[1]+s;
               else {
                  printf("ERROR at line %4d: i=%d\n",__LINE__,i);
                  exit(-1);
               }
            }
            if (ny[dataline]%2 && i<ny[dataline]+4) {
               if (i<MAXREPLICA+4) y[i][dataline]=digit[1];
               else {
                  printf("ERROR at line %4d: i=%d\n",__LINE__,i);
                  exit(-1);
               }
            }
         }
         break;
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
         ny[dataline]=data_columns-1;
         x[dataline]=digit[0];
            ny[dataline]=data_columns-1;
            min=digit[1];
            max=digit[1];
            for (i=2; i<data_columns; i++) {
               if (min>digit[i]) {
                  min=digit[i];
               }
               if (max<digit[i]) {
                  max=digit[i];
               }
            }
            y[1][dataline]=min;
            y[3][dataline]=max;
            for (i=1, ii=4; i<data_columns && ii<MAXREPLICA+4; i++) {
               y[ii++][dataline]=digit[i];
            }
            y[2][dataline]=meansd(&(y[0][dataline]),dataline,ny[dataline]);
         break;
      default:
         return 0;
   }
/*
   if (y[2][dataline]<=0) {
         y[1][dataline]=MISSING;
         y[2][dataline]=MISSING;
         y[3][dataline]=MISSING;
   } else
*/
   {
      check_range(&(y[1][dataline]),&(y[2][dataline]),&(y[3][dataline]),Sd,Vc);
      if (ymin>y[2][dataline]) ymin=y[2][dataline];
      if (ymax<y[2][dataline]) ymax=y[2][dataline];
   }

   if (Trace) {
      cprintf("%s %4d %2d %2d %10.4f ",__FILE__,__LINE__,dataline,ny[dataline],x[dataline]);
      for (ii=0; ii<ny[dataline]+2; ii++) cprintf("%11.3f ",y[ii][dataline]);
      cprintf("\r\n");
   }

   return data_columns;
}

float meansd(float *sd,int dataline,int n) {
int i;
float x;
double sx, sxx;
   if (n<2) return MISSING;
//cprintf("\r\n%s %4d : dataline=%d n=%d",__FILE__, __LINE__,dataline,n);if (getch()==27) exit(-1);putch('\r');putch('\n');
   sx=y[1][dataline];
   sxx=sx*sx;
   for (i=3; i<n+2; i++) {
      x=y[i][dataline];
      if (x==MISSING) n--;
      else {
         sx+=x;
         sxx+=x*x;
      }
   }
   *sd=(n>1)?sqrt(sxx-sx*sx/n)/(n-1):MISSING;
   return (n>0)?sx/n:MISSING;
}

void check_range(float *y1, float *y2, float *y3, float sd, float vc) {
    float yy1, yy2, yy3, d, f;
    if (vc<0.001) return;
    d=sd/vc;
    f=sqrt(1+vc);
    yy1=*y1+d;
    yy3=*y3+d;
    if (yy1>yy3) {
       yy2=yy1;
       yy1=yy3;
       yy3=yy2;
    }
    yy2=sqrt(yy1*yy3);
//cprintf("\r\n%s %4d : y1=%.3f y3=%.3f d=%.3f\r\nyy1=%.3f yy3=%.3f yy3/yy1=%.3f 1+vc=%.3f\r\n",__FILE__, __LINE__, *y1,*y3,d, yy1,yy3,yy3/yy1,1+vc);
    if (yy3/yy1<1+vc) {
       yy1=yy2/f;
       yy3=yy2*f;
    }
    *y1=yy1-d;
    *y2=yy2-d;
    *y3=yy3-d;
//cprintf("\r\n%s %4d : y1=%.3f y3=%.3f",__FILE__, __LINE__,*y1,*y3);if (getch()==27) exit(-1);putch('\r');putch('\n');
}

int numbers_per_line(char *s) {
   int p, pos=0, len, n=0;
   char number[81];
   len=strlen(s);
   do {
      while (isdelimiter(s[pos]) && pos < len) pos++;
      if (pos >= strlen(s)) break;
      p=0;
      while (!isdelimiter(s[pos]) && pos<strlen(s)) number[p++]=s[pos++];
      number[p]='\0';
      if (isnumber(number)) n++;
      else n=0;
   } while (pos<len);
   return n;
}

int isdecimal(char c) {
   if (isdigit(c) ||
       c=='.'     ||
       c=='+'     ||
       c=='-') return 1;
   else return 0;
}

int isdelimiter(char c) {
   return (isspace(c) || c==',' || c==';');
}

int isnumber(char *s) {
int n, len=strlen(s);
   if (!len)                                        return 0;
   for (n=0; n<len ; n++)
      if (!(isdecimal(s[n])))                       return 0;
   if (s[len]=='.')                                 return 0;
   for (n=1;n<len;n++) if (s[n]=='+' || s[n]=='-')  return 0;
   for (n=0;n<len-1;n++) if (s[n]=='.')  
      for (n++;n<len-1;n++) if (s[n]=='.')          return 0;
   return 1;
}

char *stripwhites(char *s) {
   int p, len;
   char *s2;
   s2=s;
   for (p=0;isspace(s[p]) && p<strlen(s);s2++)
      ;
   for (p=strlen(s2)-1;isspace(s2[p]) && p;p--) s2[p]='\0';
   if (*s2==QUOTE){
      s2++;
      len=strlen(s2);
      if (s2[len-1]==QUOTE) s2[len-1]='\0';
   }
   strcpy(s,s2);
   return s;
}

int fgetline(char *s,int i,FILE *fpin) {
   char c;
   int skip=FALSE, pos=0, non_whites=0;
   while((c=fgetc(fpin))!=EOF && c!='\r' && c!='\n' && pos<i ) {
      if (c==26) {
         c=EOF;
         break;
      }
      if (non_whites==0 && c=='\\') skip=TRUE;
      if (c!=' ' && c!='\t') {
         if (!isprint(c)) {
            return EOF;
/*
            cprintf("\n\rNon-printable character \" %c \" (ASCII %d) \"%s\": wrong input-file \"%s\"?\n\r", c, c, buffer, prnfile);
            exit(-1);
*/
         }
         non_whites ++;
         s[pos]=c;
         pos++;
      } else if (pos) {
         s[pos]=c;
         pos++;
      }
   }
   while((c=fgetc(fpin))!=EOF && (c=='\r' || c=='\n'))
      ;
   ungetc(c,fpin);
   s[pos]='\0';
   if (non_whites==0 && c==EOF) return EOF;
   if (skip) return 0;
   else return non_whites;
}

char *find_monthname(char *name, int month, double size_per_month) {
   int len;
   if (size_per_month>11*HsizeWMF/2) len=10;
   else if (size_per_month>4*HsizeWMF/2) len=3;
   else len=1;
   if (size_per_month<HsizeWMF/2 && (month)%3!=1) return 0;
   strncpy(name, monthname[month%12], len);
   name[len]='\0';
   return name;
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
//   *base= pow10d(logd-*exp);
   *base= pow(10.0,logd-*exp);
   if (neg) *base=-*base;
   return *base;
}

double val(double mantissa, int exponent) {
   return mantissa*pow10(exponent);
}

int round(double d) {
    int i=(int)d;
    if (d-i<.5 ) return (int)(d+.5);
    return (int)(d+1);
}
/*
double pow10d(double exponent) {
   /* 10^exponent, exponent is double rather than int, as required for pow() */
   return pow(10.0,exponent*log10(10));
}
*/

char *keycopy(char *target,char *source,int maxlen) {
int t,tt,s,insert=0;
int c;
result=0;
for (t=0;t<maxlen;t++) target[t]='\0';
// cprintf("\r\n%s %4d : source=\"%s\" target=\"%s\"",__FILE__, __LINE__,source,target);
// er was een bug in line: maxlen 80 met filenamen van 40!
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
                    putch('\n\r');
                    s=t=0;
                    break;
               case CTRL_C:
                    exit(0);
                    break;
               }
          }
     }
     return target;
}

void openin(char *s) {
   if ((fpin=fopen(s,"rb"))== NULL) {
       cprintf("\n\rcannot open \"%s\" for input\n\r",s);
       exit(-1);
   }
}

void openout(char *s) {
   if ((fpout=fopen(s,"wb"))== NULL) {
       cprintf("\n\rcannot open \"%s\" for output\n\r",s);
       exit(-1);
   }
}

int strmenu(int current_pos, int rows, int startrow,int nooflines) {
     int pos, key;
     int n, result, row, col, maxnoofitems;
     maxnoofitems=TEXTCOLUMNS*nooflines;
     if (rows>maxnoofitems) rows=maxnoofitems;
     if (rows>MAXFILES) rows=MAXFILES;
     for (n=0;n<rows;n++) startchars[n]=toupper(*(filename[n]));
     startchars[rows]='\0';
     n= current_pos;
     do {
        row=startrow + n % nooflines;
        col=1+SCREENCOLUMNS/TEXTCOLUMNS*(n/nooflines);
        gotoxy(1,2);
        cprintf("                                  ");
        gotoxy(1,2);
        cprintf("%s",filename[n]);
        gotoxy(col, row);
        HLON
        cprintf("%-8s",strnleft2(buffer,filename[n],'.',8));
        HLOFF
        key=getkey(0);
        gotoxy(col, row);
        cprintf("%-8s",strnleft2(buffer,filename[n],'.',8));
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
                   n=rows-1;
                 break;
                case RETURN:
                case EXEC:
                   result= RETURN;
                 break;
                case ESC:
                case CANCEL:
                   n=-1;
                 break;
              case CTRL_C:
                  exit(0);
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

/* $getfile: GET FILE FROM DISC as a subroutine; first Eco-C version: 081187 */
/* modified 040288,060288,270288,040388: quit with Esc */
/* Turbo-C version 220388 */
/* latest modification: 230388 */

char *getfile(char *s) {
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char shortname[MAXFILE];
   char ext[MAXEXT];
   char path[120];
   int nooffiles,selected_file;
   do {
		strcpy(path,getmask(s));
		if (!(*path)) return NULL;
//        makefilename(path,path,".PRN",0);
		fnsplit(path, drive, dir, shortname, ext);
//        fnmerge(Input_path, drive, dir, "*", ext);
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
    strcpy(s,filename[selected_file]);
    fnmerge(path,drive,dir,filename[selected_file],"");
    strcpy(s,path);
    return s;
}

char *strnleft2(char *buffer, char *s1, char stop, int n) {
   int i=0;
   do buffer[i]=s1[i++];
   while  (i && i<n && s1[i]!=stop);
   buffer[i]='\0';
   return buffer;
}

char *getmask(char *mask) {
	char buffer[80];
    cprintf("Enter directory search mask ( press Esc to exit):\n\r%s ",mask);
	keycopy(buffer,mask,80);
	strcpy(mask,buffer);
	return mask;
}

int getdir(char *mask) {
	int done,n,row,col,month,day,year,hour,minute,second;
	struct ffblk ffblk;
    char s[82];
    strcpy(s,mask);
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
       strcpy(filename[n],ffblk.ff_name);
       maketime(&hour,&minute,&second,ffblk.ff_ftime);
       makedate(&month,&day,&year,ffblk.ff_fdate);
       cprintf("%-8s %2.2d/%2.2d/%2.2d\n\r",strnleft2(buffer, filename[n],'.',8), year, month, day);
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

void startup_message(void) {
   clrscr();
   frame(51,4);
   gotoxy(3,2);
   cprintf("Programma \"%s\" dd %s",STARTUP_STRING,__DATE__);
   gotoxy(3,3);
   cprintf("Inlichtingen en fouten-melding: Rob Aalberse");
   gotoxy(2,8);
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
      putch('\n\r');
      putch('\b');
      putch(c);
   }
   putch('\n\r');
   putch('\b');
}

int picsave(char *source, char *target, char *ext) {
   int i, files_written;
   if (Show) {
      clrscr();
      cprintf("RETURN = save as %s_file\n\r"
             "SPACE  = do NOT save, go back to Options menu\n\r"
             "C      = do NOT save, go to Calculations menu\n\r"
             "Esc    = do NOT save, quit program\n\r", ext);
      i=getkey(0);
   } else i=RETURN;
   switch (i) {
      case 'C':
         return 0;
      default:
      case SPACE:
      case TAB:
         return 1;
      case ESC:
      case CANCEL:
      case CTRL_C:
         exit(0);
         break;
      case RETURN:
      case EXEC:
         if (Show) {
               if (strpos(target,'.',0) >-1) target[strpos(target,'.',0)]='\0';
               cprintf("default file-name: %s ",target);
               keycopy(buffer,target,MAX_LEN);
               if (!result) {
                  strcpy(buffer, target);
                  return 1;
               }
            if (!check_output_file(target,buffer)) return 1;
         }
         if (strpos(target,'.',0) >-1) target[strpos(target,'.',0)]='\0';
         strcat(target,ext);
         do {
            files_written=piccopy(source,target);
            if (files_written<=0) {
               HLON
               cprintf("\r\n\r%s\r\n\rProblem encounterd while saving \"%s\"\r\n\r", target);
               HLOFF
               cprintf("\r\n\r\r\n\rRETURN = re-try\r\n\r"
                       "Esc    = do NOT save, go back to main menu\r\n\r");
               i=getkey(0);
               if (!i) return 0;
               switch (i) {
                  default:
                  case RETURN:
                  case EXEC:
                    break;
                  case ESC:
                  case CANCEL:
                     return 0;
               }
         }
      } while (!files_written);
      if (!Show) return 1;
      else return 0;
      /* end case RETURN */
   }
   return -1; /* should not occur */
}

int check_output_file(char *new_name, char *default_name) {
   result=0;
   strcpy(new_name,default_name);
   while ((fpin=fopen(new_name,"rb"))!=NULL && result!=RETURN) {
     fclose(fpin);
     cprintf("\n\r\n\routput file %s already exists\n\renter new name or press RETURN to overwrite\n\routput-PIC-file: %s ",default_name,default_name);
     if (keycopy(new_name, default_name, 40)==NULL) return 0;
     if (*new_name==NULL) {
        strcpy(new_name,default_name);
        return 0;
     }
     cprintf("\r\n");
   }
   return 1;
}

char *makefilename(char *fullname, char *default_path, char *default_ext, int forced_ext_flag) {
   int flag;
   char drive[MAXDRIVE];
   char path[81];
   char default_drive[MAXDRIVE];
   char dir[MAXDIR];
   char default_dir[MAXDIR];
   char shortname[MAXFILE];
   char ext[MAXEXT];
   strcpy(path,default_path);
//cprintf("\r\n%s %4d : fullname=%s default_path=%s",__FILE__, __LINE__,fullname, default_path);
   flag= fnsplit(path, default_drive, default_dir, shortname, ext);
   flag= fnsplit(fullname, drive, dir, shortname, ext);
   if (!(flag & DRIVE))      strcpy( drive, default_drive);
   if (!(flag & DIRECTORY))  strcpy( dir, default_dir);
   if (!(flag & EXTENSION) || forced_ext_flag)  strcpy( ext, default_ext);
   fnmerge(fullname,default_drive,default_dir,shortname,ext);
   fnmerge(default_path,default_drive,default_dir,"*",ext);
//cprintf("\r\n%s %4d : fullname=%s default_path=%s",__FILE__, __LINE__,fullname, default_path);if (getch()==27) exit(-1);putch('\r');putch('\n');
   return fullname;
}

int check_dosname(char *s) {
char c;
int i, namestart=0, extlen, len, dotpos=0;
   if (strpos2(s,'?',0) !=-1) return 0;
   if (strpos2(s,'*',0) !=-1) return 0;
   if (strpos2(s,'/',0) !=-1) return 0;
   if (s[1]==':' && (c=toupper(*s))>'@' && c<'F') namestart=2;
   if (strpos2(s,':',namestart) !=-1) return 0;
   if (strlen(s)-namestart>1 && strpos2(s,' ',namestart+1) !=-1) return 0;
   do {
      i=strpos2(s, '\\', namestart)+1;
      if (i) namestart=i;
   } while (i);
   i=strpos2(s,'.',namestart);
   if (i>-1) {
      dotpos=++i;
      if (i<strlen(s) && strpos2(s,'.',i)!=-1) {
         return 0;
      }
      extlen=strlen(s)-dotpos+1;
      if (extlen<2 || extlen>4) return 0;
   } else extlen=0;
   if ((len=strlen(s)-extlen-namestart)>8) return 0;
   return len;
}

int strpos(char *s,char c,int pos) {
/* finds next occurrence of c in s starting at pos */
int p;
     for (p=pos+1;s[p];p++) if (s[p]==c) return p;
     for (p=0;p<=pos;p++) if (s[p]==c) return p;
     return -1;
}

int strpos2(char *s,char c,int pos) {
/* finds occurrence of c in s starting at pos */
int p;
     if (pos>strlen(s)) cprintf("\n\rerror: position %d in \"%s\" not found\n\r", pos, s);
     for (p=pos;s[p];p++) if (s[p]==c) return p;
     return -1;
}

int getmaxlegend(void) {
   int i, len, maxlen=0,pos1,pos2,done;
   for (i=0,pos1=0,done=0; i<ranges; i++) {
         len=strlen(range_name[i]);
         while (!done) {
//          len=strcspn(p+pos,"\\");
            pos2=strpos(range_name[i],'\\',pos1);
            if (pos2<pos1) pos2=len;
            if ( maxlen < pos2-pos1) maxlen=pos2-pos1;
            if (pos2<len-1) pos1=pos2+1;
            else done=1;
         }
   }
   return maxlen;
}

int get_legendlines(void) {
   int i, n;
   for (i=0, n=0; i<ranges; i++)  n+=count_char(range_name[i],'\\')+1;
   return n;
}

int count_char(char *s, char c) {
   int n=0, result=0;
   while(s[n]) if (s[n++]==c) result++;
   return result;
}

char *ftoa(float f) {
   sprintf(buffer,"%.3f",f);
   return buffer;
}

void yesno(int flag, char *yesstring, char *nostring) {
   if (flag) {
      cprintf(" %s  ", nostring);
      HLON
      cprintf("%s\n\r", yesstring);
      HLOFF
   } else {
      putch(' ');
      HLON
      cprintf("%s", nostring);
      HLOFF
      cprintf("  %s\n\r", yesstring);
   }
}

void initialise_arrays(void) {
   int i,ii;
   Box=         BOX;
   Color=       1;
   Hgrid=       HGRID;
   Legend=      LEGEND;
   Legendpos=   LEGENDPOS;
   Line=        LINE;
   Literal=     LITERAL;
   Logxflag=    1;
   Logyflag=    LOGYFLAG;
   Logy2flag=   LOGY2FLAG;
   Rim=         RIM;
   Extra_rim=   EXTRA_RIM;
   Show=        -1;
   Symbol_size= SYMBOL_SIZE;
   Showstat=    1;
   Bar_size=    0;
   Intra=       INTRA;
   Textsize=    TEXTSIZE;
   Vgrid=       VGRID;
   Xcal=        XCAL;
   Xformat=     XFORMAT;
   Xy=          XY;
   X_start=     0;
   X_length=    X_LENGTH-X_start;
   X_origin=    -1;
   Y1line=      Y1LINE;
   Ycal=        YCAL;
   Yformat=     YFORMAT;
   Y_start=     0;
   Y_length=    Y_LENGTH-Y_start;
   Statflag=    1;
   Outfiles=    OUTFILES;
   Range_check= 3;      /* for PLA-calculation: 0=none, 1=break if low, 2=break if high, 3=full check */
   Ylo= MISSING;        /* cut-off for PLA */
   Yhi= MISSING;
   Xmax=MISSING;
   Xmin=MISSING;
   Ymax=MISSING;
   Ymin=MISSING;
   Y2max=MISSING;
   Y2min=MISSING;
   Sd=SD;
   Vc=VC;
   Rslopemin=RSLOPEMIN;
   Readmode=0;   /* Int_array[33] mode=0: 2 data = duplo's; mode=1: gemiddelde, sd */
   units=1.0;
   Xet=X_LENGTH/2;       /* position free extra text */
   Yet=Y_LENGTH/2;
   Bare=FALSE;
   for (i=0; i<11; i++) extra_text[i][0]='\0';
   strcpy(Input_path,INPUT_PATH);
   strcpy(Input_ext,INPUT_EXT);
   strcpy(Output_ext,OUTPUT_EXT);
   strcpy(Output_path,OUTPUT_PATH);
   strcpy(symbolstring,SYMBOL_STRING);
   strcpy(linetypestring,"0");
   init_colors();
   strcpy(picfile,"PLAB.PIC");
   strcpy(wmffile,"PLAB.WMF");
   strcpy(plafile,"PLAB.PLA");
   strcpy(prnfile,"PLAB.PRN");
   for (i=0; i<COLUMNS; i++) {
      x[i]=MISSING;
      ny[i]=0;
      for (ii=0; ii<MAXREPLICA+2; ii++) y[ii][i]=MISSING;
   }
// Wmftextsize=-1;
}

void write_fln(char *s) {
   int ii, t, bar, r, column;
   unsigned int opcode;
   if (Show) cprintf("Updating PRN-file \"%s\"\r\n", s);
   if ((fpout=fopen(s,"wb")) == NULL) return;
   clearerr(fpout);
   fprintf(fpout,"%s\r\n", FLNCODE);
/*
   opcode=1000*STRING_ARRAY+100*DA_range_name;
   for (t=0; t<RANGES; t++) if (range_name[t][0]) fprintf(fpout,"%u %s\r\n",opcode+t, range_name[t]);
*/
   opcode=1000*STRING_ARRAY+100*DA_extra_text;
   for (t=0; t<11; t++) if (extra_text[t][0]) fprintf(fpout,"\\/%u %s\r\n",opcode+t, extra_text[t]);
   opcode=1000*INTEG;
   for (ii=0;ii<NOOFINTS;ii++)  if (ii!=32 && ii!=37) fprintf(fpout,"\\/%u %d\r\n",opcode+ii, Int_array[ii]);
   opcode=1000*FLOATING;
   for (ii=0;ii<NOOFFLOATS;ii++) if (Float_array[ii]!=MISSING) fprintf(fpout,"\\/%u %.3f\r\n",opcode+ii, Float_array[ii]);
   opcode=1000*SINGLE_STRING;
   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_linetypestring, linetypestring,"line type");
   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_symbolstring, symbolstring,"symbol type");
   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_lcolorstring, lcolorstring,"line color");
   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_scolorstring, scolorstring,"symbol color");
//   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_input_path, Input_path,"input path");
//   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_output_path, Output_path,"output path");
//   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_input_ext, Input_ext,"input extension");
//   fprintf(fpout, "\\/%u %s; %s\r\n", opcode+DS_output_ext, Output_ext,"output extension");
   opcode=1000*RGB_ARRAY;
   fprintf(fpout,"\\/%u 0X%06lX\r\n",opcode, rgb[0]);
   for (ii=1;ii<COLORS-3;ii++) {
      if (strpos(lcolorstring,'A'+ii,0)>-1 ||
          strpos(scolorstring,'A'+ii,0)>-1)
      fprintf(fpout,"\\/%u 0X%06lX\r\n",opcode+ii, rgb[ii]);
   }
   fprintf(fpout,"\\/%u 0X%06lX\r\n",opcode+COLORS-2, rgb[COLORS-2]);
   fprintf(fpout,"\\/%u 0X%06lX\r\n",opcode+COLORS-1, rgb[COLORS-1]);
   fprintf(fpout,"\\/%s\r\n",ENDCODE);
   if (*first_title)  fprintf(fpout, "\"%s\"\r\n", first_title);
   if (*second_title) fprintf(fpout, "\"%s\"\r\n", second_title);
   fprintf(fpout, "\"%s\"\r\n", y_legend);
   if (dual_axis)   fprintf(fpout, "\"%s\"\r\n", y2_legend);
   fprintf(fpout, "\"%s\"\r\n", x_legend);
   column=0;
   for (r= 0;r < ranges; r++) {
   if (ranges>1) fprintf(fpout,"\"%s\"\r\n",range_name[r]);
     for (bar=0; bar<range_size[r]; bar++) {
        sprintf(buffer,"\"%s\"",name[column]);
        fprintf(fpout,"%-19s",buffer);
        fprintf(fpout,"%13.4f %14.5f %14.5f %14.5f\r\n", x[column],y[1][column], y[2][column], y[3][column]);
        column++;
      }
   }
   fclose(fpout);
}

float getfloat(int *key, int field) {
   int i, pos;
   char b[15];
   *key=0;
   for (pos=0; pos>=0 && pos<14; pos++) {
      b[pos]='\0';
      i=getkey(0);
      if (!i) {
         *key=ESC;
         return MISSING;
      }
      if (!isdigit(i) && i!='.' && i!='-' && i!=BACKSPACE) {
         if (!(pos && i==RETURN)) *key=i;
         if (!pos) return MISSING;
         else {
            while (field-- > -1) putch(' ');
            return atof(b);
         }
      } else if (i==BACKSPACE) {
         pos--;
         field++;
         b[pos]='\0';
         pos--;
         putch('\b');
         putch(' ');
         putch('\b');
      } else {
         b[pos]=i;
         putch(i);
         field--;
      }
   }
   b[pos]=0;
   return atof(b);
}

int getbarsize(int dataline) {
int range, start, stop;
float dx,xx0,xx1,xx2;
   for (range=0; range<ranges-1 && dataline>=range_start[range+1] ; range++);
     ;
   start=range_start[range];
   stop=start+range_size[range];
   if (stop-start<2) return 100;
      if (dataline==start) {
         xx1=x[dataline];
         xx2=x[dataline+1];
         dx=(xx2-xx1)*0.25;
         xx1-=dx;
         xx2=xx1+2*dx;
      } else if (dataline==stop-1) {
         xx1=x[dataline-1];
         xx2=x[dataline];
         dx=(xx2-xx1)*0.25;
         xx2+=dx;
         xx1=xx2-2*dx;
      } else {
         xx1=x[dataline-1];
         xx0=x[dataline];
         xx2=x[dataline+1];
         xx1+=(xx0-xx1)*0.75;
         xx2+=(xx0-xx2)*0.75;
      }
   if (Logxflag) {
      xx1=log10(xx1);
      xx2=log10(xx2);
   }
   dx=fabs(xx2-xx1);
   return scale_xfactor*dx-Intra;
}

/*
int check_drive(int drive) {
   int result;
   char buffer[512];
   result=biosdisk(4,drive,0,0,0,1,buffer);
// cprintf("drive %d:result=%d ",drive, result);
   if (result!=128) return 1;
   else return 0;
}
*/
/*
void err0(char *s, int line) {
   if (nullvalue!=*nullp) {
      cprintf("\r\n%s %4d : nullpointer assigned!",s,line);
      exit(-1);
   } else if (Trace) cprintf("\r\n%s %4d : nullpointer ok",s,line);
}
*/

/*
   if ((Sd>0.0 || Vc>0.0) && x[dataline]>0.0) {
      vc=1.0+Vc;
      y=y[2][dataline];
      ll=(y[1][dataline]<y[3][dataline])?y[1][dataline]:y[3][dataline];
      ul=(y[1][dataline]>y[3][dataline])?y[1][dataline]:y[3][dataline];
      y[1][dataline]=(ll>(y-Sd)/vc)?(y-Sd)/vc:ll;
      if (y[1][dataline]<=0.0) y[1][dataline]=(Vc>0.01)?y*Vc:0.2*y;
      y[3][dataline]=(ul<y*vc+Sd)?y*vc+Sd:ul;
   }
*/

/*
      dy1=dy2=0;
//      if (Sd>0.0 && y*Vc<fabs(y-y[1][dataline]) && (fabs(y-y[1][dataline])<Sd || fabs(y[3][dataline]-y)<Sd)) dy=(y>Sd)?Sd:0.8*y;
      if (Sd>0.0 && (fabs(y-y[1][dataline])<Sd || fabs(y[3][dataline]-y)<Sd)) dy1=fabs(((y>Sd)?Sd:0.8*y));
      if (Vc>0.0 && (fabs(y/y[1][dataline])<vc || fabs(y[3][dataline]/y)<vc)) dy2=fabs(y*Vc);
      if (dy1 || dy2) {
         dy=(dy1>dy2)?dy1:dy2;
         y[1][dataline]=y-dy;
         y[3][dataline]=y+dy;
      }
*/


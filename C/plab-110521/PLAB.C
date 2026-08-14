// teken line-graph + error-bar vanuit PRN-file */
// extern unsigned _stklen=10000; */
#include "plab.h"
#include "include\wmf.h"
#include "include\assert.h"
char LOGFILEDIR[242]; // "c:\\util\\"
char LOGFILE[242];    // "c:\\util\\plab.log"
char WMFviewer[242];  // "c:\\util\\metacomp.exe"

//DS_logfiledir,
//DS_logfile,
//DS_WMFviewer

void prepare_pic(void);
int find_range(int *startS,int *sizeS, int *startT,int *sizeT);
double pla(int startS, int NSd, int startT, int NTd);
void plot_result(void);
void plot_info(char *infostring1, char *infostring2, int size);
void read_environment(void);
void draw_regression(int n);
double transform(float y, float sd, float vc);
double backtransform(float y, float sd, float vc);
double getmeany(float y1, float y2, float sd, float vc);
double getmidresponse(int start, int size);
double getslope(int start, int size);

Dword nullvalue;
Dword *nullp;
char *flag;                       /* [COLUMNS] */
char *first_title;                /* [MAX_LEN+1] */
char *second_title;               /* [MAX_LEN+1] */
char *Input_path;                 /* [241] ="C:" */
char *Input_ext;                  /* [5] =".PRN" */
char *Output_ext;                  /* [5] =".PRN" */
char *Output_path;                /* [241]="C:" */
char *x_legend;                   /* [MAX_LEN+1] */
char *y_legend;                   /* [MAX_LEN+1] */
char *y2_legend;                  /* [MAX_LEN+1] */
char *symbolstring;               /* [80]="0123456789" */
char *linetypestring;             /* [20]="012345678"*/
char *lcolorstring;               /* linecolors   [40]="012345678"*/
char *scolorstring;               /* symbolcolors [80]="012345678"*/
char *l0colorstring;              /* backup linecolors   [40]="012345678"*/
char *s0colorstring;              /* backup symbolcolors [80]="012345678"*/
char *datapoint_code;
char *prnfile;                    /* [241] */
char *outfile;                    /* [241] */
char *fln_filename;               /* [241] */
char *buffer;                     /* [MAX_LEN] */
char *startchars;                 /* [MAXFILES] */
char *picfile;                    /* [241]= "C:LINE.PIC" */
char *wmffile;                    /* [241]= "C:LINE.WMF" */
char *plafile;                    /* [241]= "C:LINE.PLA" */
char *prnfile;                    /* [241]= "C:LINE.PRN" */
char *infostring1;
char *infostring2;
char **name;                      /* [COLUMNS][NAME_LEN+1] */
char **extra_text;                /* [10][MAX_LEN+1] */
char **filename;                  /* [MAXFILES][LENFILENAME+1] */
char **range_name;                /* [RANGES+1][MAX_LEN+1] */
char **calibratorstring;          /* [MAXNOOFCALIBRATORS][12] */
char **headerline;                /* [HEADERLINES][MAX_LEN] */
char **stringline;                /* [HEADERLINES+2][MAX_LEN] extra lines: x- and y-legend */

int    *Int_array;                /* [NOOFINTS+1] */
int    *valid_data;               /* [SERIES] */
int    *range_size;               /* [RANGES+1] */
int    *range_start;              /* [RANGES+1] */
int    *range_startpos;           /* [RANGES+1] */
int    *range_type;               /* [RANGES+1]: axis-connection: left=0 right=1 */

float  *Float_array;              /* [NOOFFLOATS+1] */

int    *ny;
float  *x;
float  *y[MAXREPLICA+2];
float  *intercept;                /* [RANGES+1] */

double *islope;                   /* [SERIES] */
double *meanx;                    /* [SERIES] */
double *meany;                    /* [SERIES] */
double *value_of_calibrator;      /* [MAXNOOFCALIBRATORS] */
char header_vector[]= {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
int Trace=TRACE;

FILE *fpin, *fpout, *fpwmf, *fppla, *tracefile;
char *charpointer;
int result=0, ranges, headerlines;
int data_columns, Data_columns=0, dual_axis=0;
double scale_xfactor, scale_yfactor, scale_y2factor, slope, slope0, slope2, r2, g, upper, potency, lower, corrcoeff;
int maxlegend, legendblocksize;
int xll, xul, yll, yul;                 /* xll=x_lower_limit in screen units */
int xllflag, xulflag, yllflag, yulflag; /* xulflag: data_points higher than Xmax found */
int logxflag, logyflag, logy2flag;
int x_origin, x_length=X_LENGTH, log0;
int y_origin, y_length=Y_LENGTH;
float upperlimit;
float xx2;
float yy1;
float yy2;
float yy3;
int DX;
int DY;
int Vsize;
int Hsize;
int YSIZE;
//int Show=SHOW;
float xmin;
float xmax;
float ymin;
float y2min;
float ymax;
float y2max;
float units;

int result;
int calcflag=FALSE;
int maxrange;
unsigned columns;
char Cfg[80];
char monthname[12][10]={
"DECEMBER",
"JANUARI",
"FEBRUARI",
"MARCH",
"APRIL",
"MAY",
"JUNE",
"JULY",
"AUGUST",
"SEPTEMBER",
"OCTOBER",
"NOVEMBER"
};
int calbase[4][4]={{1,0,0,0},{1,3,0,0},{1,2,5,0},{10,20,25,50}};
int noofcalibrators=MAXNOOFCALIBRATORS;
//int startup_drive;
int cal_start[RANGES];

/************************ START OF MAIN ************************/

int main(int argc, char* argv[]) {
   int flag, r, bar, column, key, changes_made;
   int startS, startT, sizeS, sizeT;
   float yy,temp;
   char cfgfile[241];
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char shortname[241]="";
   char ext[MAXEXT];
   char path[242]="";
   long prnlength;
   struct date d;
   struct time t;
   struct ftime ft;
   nullp=0;
   nullvalue=*nullp;
   allocate();
   initialise_arrays();
strcpy(LOGFILEDIR,"c:\\util\\");
strcpy(LOGFILE,"c:\\util\\plab.log");
strcpy(WMFviewer,"c:\\util\\metacomp.exe");


   if (argc>1 && argv[argc-1][0]=='/') strcpy(Cfg,argv[argc-1]+1);
   else Cfg[0]=0;
   read_environment();                /* SET SHOW=0/1 overrides setting in PLAB.CFG */
   if (!Cfg[0]) strcpy(Cfg,"PLAB.CFG");
//   cfgfile=searchpath(Cfg);
   strcpy(cfgfile,Cfg);
   if (cfgfile) read_data(cfgfile,infostring1,0);
   else sprintf(infostring1,"%s;%s;(no cfg)",VERSION,__DATE__);
   if (Vc>=0.5) Vc/=100.0;
   if (Show==-1) Show=SHOW;
   printf("%s %4d :Cfg=\"%s\" cfgfile=\"%s\" Vc=%.3f Sd=%.3f\n",__FILE__, __LINE__,Cfg,cfgfile,Vc,Sd);
   if (argv[1][0]!='/') strcpy(path,argv[1]);
// if (argc>2) Show=(!Show);
   if (argc>1 && argv[1][0]!='/') {
      flag= fnsplit(path, drive, dir, shortname, ext);
      if (flag & DRIVE) strcpy(Input_path,drive);
      if (flag & DIRECTORY)  strcat(Input_path, dir);
      if (flag & EXTENSION) {
         strcat(shortname,ext);
         strcpy(Input_ext,ext);
      }
   }
/*
   else {
      sprintf(shortname,"%s*%s",Input_path,Input_ext);
      strcpy(ext,Input_ext);
   }
*/
   if (Show) {
      clrscr();
      HLOFF
      startup_message();
   }
   do {
      if (argc>2 && argv[argc-1][0]!='/') {
         if (*argv[2] != '-') strcpy(outfile,argv[2]);
         else strcpy(outfile,argv[2]+1);
      }
      if (argc==1 || (argc>1 && (*argv[1]=='-' || *argv[1]=='/'))) {
          sprintf(prnfile,"%s*%s",Input_path,Input_ext);
//          strcpy(prnfile,ext);
//          makefilename(prnfile,Input_path,Input_ext,1);
          while (getfile(prnfile)==NULL) {
                clrscr();
                cprintf("\n\rNo filename entered; press RETURN to re-try\n\r");
                if (getch()!=CR) exit(-1);
                clrscr();
                sprintf(prnfile,"%s*%s",Input_path,Input_ext);
          }
      } else if (argc>1 && *argv[1]!='-' && argv[1][0]!='/') strcpy(prnfile,argv[1]);
//      prnfile[strpos(prnfile,'.',0)]='\0';
//      makefilename(prnfile, Input_path,Input_ext, 0);
      if (argc>1 && *argv[1]=='-') {
         strcpy(outfile,argv[1]+1);
         makefilename(outfile, Output_path, ".PIC",1);
      }
      if (! *outfile) {
         strcpy(outfile,prnfile);
         makefilename(outfile,Output_path,".PIC",1);
      }
      while ((fpin=fopen(prnfile,"rb"))== NULL) {
          clrscr();
          cprintf("\n\rcannot open input file %s; press RETURN to re-try\n\r",prnfile);
          if (getch()!=CR) exit(-1);
     }
     if ((prnlength=filelength(fileno(fpin)))==0) {
         clrscr();
         cprintf("Selected file is empty\n\r"
         "Possible cause: file not closed when printing to file\n\r"
         "e.g. Lotus-123 command sequence:\n\r"
         "Print File Range Go Quit\n\r"
         "Forgetting \"Quit\"  results in empty (non-closed) file\n\r\n\r"
         "Press Esc to quit program\n\r"
         "Any other key to select other file");
         if (getch()==ESC) exit(-1);
      }
   } while (!prnlength);
   if (Show) {
      clrscr();
      cprintf("reading file %s\n\r",prnfile);
   }
//strcpy(LOGFILEDIR,"c:\\util\\");
//strcpy(LOGFILE,   "c:\\util\\pla43.log");
   if ((fpout=fopen(LOGFILE,"a+t"))==NULL) {
       cprintf("\n\rError: Unable to open %s for output\n\rPlease check the directory %s; it may not yet exist", LOGFILE, LOGFILEDIR);
       getch();
       exit(-1);
   }
   getftime(fileno(fpin),&ft);
   getdate(&d);
   gettime(&t);
   sprintf(infostring2,"%s %s; dd-mm-yyyy: %02d-%02d-%04d; %02d:%02d; ",
      prnfile,
      cfgfile,
      d.da_day,
      d.da_mon,
      d.da_year,
      t.ti_hour,
      t.ti_min);
//    t.ti_sec);
   fprintf(fpout,infostring2);
   fprintf(fpout,"%s: Readmode=%d Vc=%.3f Sd=%.3f\n",__FILE__,Readmode,Vc,Sd);
   fclose(fpout);
   columns=read_data(prnfile,0,Show);

   if (Trace) printf("%s %4d :Cfg=\"%s\" cfgfile=\"%s\" Readmode=%d Vc=%.3f Sd=%.3f\n",__FILE__, __LINE__,Cfg,cfgfile,Readmode,Vc,Sd);
   clrscr();
   cprintf("%s\r\n%s\r\n\r\n"
           "CFG-file:   %s\r\n"
           "Sd:         %.3f\r\n"
           "Vc:         %.3f\r\n\r\n"
           "Readmode:   %d\r\n"
           "   Readmode=1: x y1 y2 ... yi\r\n"
           "   Readmode=2: x,y,sd,n\r\n"
           "   Readmode=0: x,y1; x,y1,y2; x,y1,mean,y2\r\n\r\n",
      infostring1,infostring2,(cfgfile)?cfgfile:"no cfgfile",Sd,Vc,Readmode);
cprintf("Press Esc to quit, any other key to continue ...");if (getch()==27) exit(-1);putch('\r');putch('\n');
   Vsize=Textsize*9;
   Hsize=Textsize*8;
   VsizeWMF=Textsize*WMFVSIZE;
   HsizeWMF=Textsize*WMFHSIZE;
   YSIZE=0.9*Vsize;   /* vertical size of legend block */
   if (Xformat==2) Logxflag=0;
   DX=30;
   DY=30;
   do {
      if (Show) {
         clrscr();
         cprintf("Building picture ...\r\n");
      }
      prepare_pic();

/**** write line_graph to PIC-file ****/

     picopen(picfile);
     wmfopen();
     if (!Bare) {
         if (Color) {
            wmfrectangle(0,0,WIDTH,HEIGHT,0, rgb[25],rgb[25]);
            wmfrectangle(x_origin*WFACTOR,y_origin*WFACTOR,(x_origin+x_length)*WFACTOR,(y_origin+y_length)*WFACTOR,0,rgb[0],rgb[0]);
         }
         ascal(&noofcalibrators, &ymin, &ymax, Logyflag, 0, Yformat,0);
         logyflag= picy_cal(Logyflag,1);
         picy_axis_title(y_legend, y_origin + y_length,1);
         if (dual_axis) {
            ascal(&noofcalibrators, &y2min, &y2max, Logy2flag, 0, Yformat,0);
            logy2flag= picy2_cal(Logy2flag,1);
            picy2_axis_title(y2_legend, y_origin + y_length,1);
         }
         ascal(&noofcalibrators, &xmin, &xmax, Logxflag, 0, Xformat,1);
         logxflag= picx_cal(Logxflag,1);
         if (Statflag && !calcflag) {
            find_range(&startS,&sizeS,&startT,&sizeT);
            calcflag=pla(startS,sizeS,startT,sizeT);
            Ylo=y[2][startS];
            Yhi=y[2][startS+sizeS-1];
            if (Ylo>Yhi) {
               temp=Ylo;
               Ylo=Yhi;
               Yhi=temp;
            }
         }
      }
      if (Ylo==MISSING) Ylo=(columns>6)?ymin+0.1*(ymax-ymin):ymin;
      if (Yhi==MISSING) Yhi=(columns>6)?ymin+0.9*(ymax-ymin):ymax;
      if (logxflag) {
         xmin=log10(xmin);
         xmax=log10(xmax);
      }
      if (logyflag==1) {
         ymin=log10(ymin);
         ymax=log10(ymax);
      }
      if (logyflag==2) {
         ymin=transform(ymin,Sd,Vc);
         ymax=transform(ymax,Sd,Vc);
      }
      if (logy2flag==1 && dual_axis) {
         ymin=log10(ymin);
         ymax=log10(ymax);
      }
      if (logy2flag==2 && dual_axis) {
         y2min=transform(y2min,Sd,Vc);
         y2max=transform(y2max,Sd,Vc);
      }
      if (!Bare) {
      picheaders(headerlines);
      if (!dual_axis) {
         if (xllflag && !Extra_rim) dotted_v_line(xll,y_origin,y_origin+y_length);
         if (xulflag) dotted_v_line(xul,y_origin,y_origin+y_length);
         if (yllflag) dotted_h_line(x_origin-log0,x_origin+x_length,yll);
         if (yulflag) dotted_h_line(x_origin-log0,x_origin+x_length,yul);
      }
      picextra_text(x_origin, y_origin, x_length, y_length);
        if (xmin < 0 && xmax>0 && !logxflag) {
           picmove(x_origin - xmin * scale_xfactor, y_origin);
           picdraw(x_origin - xmin * scale_xfactor, y_origin+y_length);
        }
        if (!dual_axis) {
           if (ymin < 0 && ymax > 0 && (!logyflag || (logyflag && Y1line))) {
              picmove(x_origin-log0, y_origin - ymin * scale_yfactor);
              picdraw(x_origin-log0 + x_length+log0, y_origin - ymin * scale_yfactor);
           }
        }
      }
      if (Statflag &&!dual_axis && Showstat) {
         if (Ylo>MISSING) {
            if (logyflag==0) yy=Ylo;
            else if (logyflag==1) yy=log10(Ylo);
            else yy=transform(Ylo,Sd,Vc);
            picmove(x_origin-log0,                 y_origin+(yy - ymin) * scale_yfactor);
            picdraw(x_origin-log0 + x_length+log0, y_origin+(yy - ymin) * scale_yfactor);
         }
         if (Yhi>MISSING) {
            if (logyflag==0) yy=Yhi;
            else if (logyflag==1) yy=log10(Yhi);
            else yy=transform(Yhi,Sd,Vc);
            picmove(x_origin-log0,                 y_origin+(yy - ymin) * scale_yfactor);
            picdraw(x_origin-log0 + x_length+log0, y_origin+(yy - ymin) * scale_yfactor);
         }
      }
      for (r=0;r<ranges;r++) {
         if ((Line || symbolstring[r]=='L' || Statflag) && toupper(symbolstring[r])!='B') {
            if (Statflag) {
               if (!calcflag) calcflag=pla(0,range_size[0],range_start[1],range_size[1]);
               if (calcflag) draw_regression(r);
            }
            if (Line || symbolstring[r]=='L') {
               polyline(r);
               if (range_type[r]) {
                  draw_line(r, xmin, scale_xfactor, y2min, scale_y2factor, 0);
               } else {
                  draw_line(r, xmin, scale_xfactor, ymin, scale_yfactor, 0);
               }
            }
         }
      }
      if (Statflag && Showstat) {
         plot_info(infostring1,infostring2,3);
         plot_result();
      }
      column=0;
      for (r= 0;r < ranges; r++) {
         if (Show) putch('.');
         for (bar=0; bar<range_size[r]; bar++) {
            if (!(y[2][column]==MISSING||x[column]==MISSING || datapoint_code[column]=='*')) {
               if ((range_type[r] && logy2flag)||(range_type[r]==0 && logyflag)) {
               if ((range_type[r] && logy2flag==1)||(range_type[r]==0 && logyflag==1)) {
                  yy1=(y[1][column]>0)?log10(y[1][column]):MISSING;
                  yy2=(y[2][column]>0)?log10(y[2][column]):MISSING;
                  yy3=(y[3][column]>0)?log10(y[3][column]):MISSING;
               } else {
                  yy1=(y[1][column]>0)?transform(y[1][column],Sd,Vc):MISSING;
                  yy2=(y[2][column]>0)?transform(y[2][column],Sd,Vc):MISSING;
                  yy3=(y[3][column]>0)?transform(y[3][column],Sd,Vc):MISSING;
               }
               } else {
                  yy1=y[1][column];
                  yy2=y[2][column];
                  yy3=y[3][column];
               }
               if (logxflag && x[column]) xx2=(x[column]>0)?log10(x[column]):MISSING;
               else xx2=x[column];
               plot_point(column,r,bar);
               column++;
            }
         }
      }
      if (Xy)  {
         draw_xy ( x_origin, xmin, xmax, scale_xfactor, y_origin, ymin, ymax, scale_yfactor);
         if (dual_axis) draw_xy ( x_origin, xmin, xmax, scale_xfactor, y_origin, y2min, y2max, scale_y2factor);
      }
      if (Legend && ranges>1 && !Bare) piclegend(Legend);
      fputc(END,fpout);
      if (fclose(fpout)==EOF) {
        cprintf("\n\rError writing file to disc: Disc full?");
        exit(-1);
      }
   wmfend();
   filelen=((ftell(fpwmf))>>1)-11;  /* size in Words */
   fseek(fpwmf,28,SEEK_SET);
   fwrite(&filelen,sizeof(Dword),1,fpwmf);
   fwrite(&noofobjects,sizeof(Word),1,fpwmf);
   fwrite(&maxrecordsize,sizeof(Dword),1,fpwmf);
   fclose(fpwmf);
      if (!Show) {
        if (Outfiles%2) picsave(picfile, outfile, ".PIC");
        if ((Outfiles/10)%2) {
           write_fln(prnfile);
           outfile[strpos(outfile,'.',0)]='\0';
           strcat(outfile,Output_ext);
           piccopy(prnfile, outfile);
        }
        if (Statflag && calcflag && (Outfiles/100)%2) {
            outfile[strpos(outfile,'.',0)]='\0';
            strcat(outfile,".PLA");
            piccopy(plafile, outfile);
        }
        if (Outfiles/1000) {
           outfile[strpos(outfile,'.',0)]='\0';
           strcat(outfile,".WMF");
           piccopy(wmffile, outfile);
        }
        exit(1);
      }
      if (logxflag) {
         xmin=pow(10.0,xmin);
         xmax=pow(10.0,xmax);
      }
      if (logyflag) {
         ymin=pow(10.0,ymin);
         ymax=pow(10.0,ymax);
      }
      if (Show) {
/*
         if (spawnlp(P_WAIT,"drawpic.exe","drawpic.exe",picfile,NULL)==-1) {
            cprintf("\n\rspawn-error\n\r");
            exit(-1);
         }
*/
         if (spawnlp(P_WAIT,WMFviewer,WMFviewer,wmffile,NULL)==-1) {
            cprintf("\n\runable to show %s: spawn-error\n\r",wmffile);
            exit(-1);
         }

         changes_made=get_options();
      } else changes_made=0;
      if (changes_made== -1) break;
      if (changes_made== 0) {
         if (Outfiles%2) changes_made=picsave(picfile, outfile, ".PIC");
         if ((Outfiles/10)%2) {
           outfile[strpos(outfile,'.',0)]='\0';
           strcpy(fln_filename,outfile);
           strcat(fln_filename,Output_ext);
           write_fln(fln_filename);
         }
         if (Statflag && calcflag && (Outfiles/100)%2) {
            outfile[strpos(outfile,'.',0)]='\0';
            strcat(outfile,".PLA");
            piccopy(plafile, outfile);
         }
         if (Outfiles/1000) {
           outfile[strpos(outfile,'.',0)]='\0';
           strcat(outfile,".WMF");
           piccopy(wmffile, outfile);
         }
         if (changes_made) if(get_options()==-1) break;
      }
      if (!changes_made && Show) {
         cprintf("\r\n\r\nRETURN = change options\r\n"
                 "Q      = Quit\r\n");
         key=getkey(0);
         if (key=='Q' || key==ESC) exit(1);
         if (key==RETURN) changes_made= 1;
      }
   } while (changes_made>0 && Show);
   return 1;
}

void prepare_pic(void) {
   int r, n, legend, column, maxylen, maxlen2, dox, dx, rightmargin;

      if (*first_title && *first_title !=' ') {
         strcpy(headerline[0], first_title);
         if (headerlines<1)  headerlines=1;
      }
      if (*second_title && *second_title !=' ') {
         strcpy(headerline[1], second_title);
         if (headerlines<2) headerlines=2;
      }

/**** find min/max values for graph */

      xmin=-MISSING;
      xmax=MISSING;
      log0=0;
      for (column=0;column<columns;column++) {
         if (datapoint_code[column]=='*') continue;
         if (Logxflag && x[column]<=0) log0=LOG0;
         else if (xmin >x[column]) xmin=x[column];
         if (xmax <x[column]) xmax=x[column];
      }
      if (Xmax !=MISSING) xmax= Xmax;
      if (Xmin !=MISSING) xmin= Xmin;
      if (xmin <0 ) Logxflag=0;
      if (Xmin==MISSING && !Logxflag && xmin>0 && Xformat<2) xmin=0;
      if (Logxflag && xmax / xmin <6) {
         xmin=sqrt(xmax * xmin)/2.5;
         xmax=xmin*6;
      }
      ymin=-MISSING;
      ymax=MISSING;
      y2min=-MISSING;
      y2max=MISSING;
      for (r=0,column=0;column<columns;column++) {
         if (r<ranges-1 && column>=range_start[r+1]) r++;
         if (datapoint_code[column]=='*') continue;
         if (range_type[r]) {
            if (y2min > y[1][column]) y2min=y[1][column];
            if (y2max < y[3][column]) y2max=y[3][column];
         } else {
            if (ymin > y[1][column]) ymin=y[1][column];
            if (ymax < y[3][column]) ymax=y[3][column];
         }
      }
      if (Ymax > MISSING) ymax= Ymax;
      if (Ymin > MISSING) ymin= Ymin;
      if (ymin <=0) Logyflag=0;
      if (Y2max > MISSING) y2max= Y2max;
      if (Y2min > MISSING) y2min= Y2min;
      if (y2min <=0) Logy2flag=0;
      if (Ymin==MISSING && !Logyflag && ymin>0) ymin=0;
      if ( Logyflag && ymax / ymin <6) {
         ymin=sqrt(ymax * ymin)/2.5;
         ymax=ymin*6;
      }
      if (dual_axis &&  Logy2flag && y2max / y2min <6) {
         y2min=sqrt(y2max * y2min)/2.5;
         y2max=y2min*6;
      }
      if (Y2min==MISSING && !Logy2flag && y2min>0) y2min=0;
//    else y_length -= Ycal==0?Rim:0;
      xllflag=yllflag=xulflag=yulflag=0;
      for (r=0,column=0;column<columns;column++) {
         if (column<=range_start[r]) r++;
         if (datapoint_code[column]=='*') continue;
         if (log0 && x[column]<=0) continue;
         if (x[column]<xmin && x[column]!=MISSING) xllflag=1;
         if (x[column]>xmax) xulflag=1;
         if (range_type[r]) {
            if (y[2][column]<y2min && y[2][column]!=MISSING) yllflag=1;
            if (y[2][column]>y2max) yulflag=1;
         } else {
            if (y[2][column]<ymin && y[2][column]!=MISSING) yllflag=1;
            if (y[2][column]>ymax) yulflag=1;
         }
      }
      if (Legend<2 && ranges==1) Legend=0;
      legend=(Legend==0 || Legendpos !=0)?1:1+ceil((double)ranges/Legend);
      logxflag= picx_cal(Logxflag,0);
      logyflag= picy_cal(Logyflag,0);
      if (dual_axis) logy2flag= picy2_cal(Logy2flag,0);

/**** determine x/y-origin and x/y-length ****/

      x_length=X_length;
      if (Bare) {
         x_origin=X_origin;
         y_origin=Y_ORIGIN;
         y_length=Y_LENGTH-headerlines*Vsize-y_origin;
         scale_xfactor=xmax-xmin>0.00001?(double)x_length/(xmax - xmin):1.0;
         scale_yfactor=ymax-ymin>0.00001?(double)y_length/(ymax - ymin):1.0;
         if (dual_axis) scale_y2factor=(double)y_length/(y2max - y2min);
      } else {

/**** y-origin/y-length ****/

//       y_origin=(Legend==0 || Legendpos !=0)? 20:picbottomline(legend+1)+20;
         y_origin=(Legend==0 || Legendpos !=0)? picbottomline(2)+20:picbottomline(legend+2);
         if (Xformat==1 && (Xcal==0 || Xcal==2)) y_origin+=VSIZE;
//       y_length-= headerlines*Vsize;
         y_length=Y_LENGTH-headerlines*Vsize-y_origin;
         if (Xcal/3==0) {
            y_origin+=Rim;
            y_length-=Rim+((Xcal==0)?Rim:0);
         }

/**** x-origin/x-length ****/

        rightmargin=0;
        dox=picy_axis_title(y_legend, y_origin + y_length,0);
        x_origin= (X_origin==-1) ? X_start + dox:X_start;
        ascal(&noofcalibrators, &ymin, &ymax, Logyflag, 1, Yformat,0);
        for (n=0, maxylen=0; n<noofcalibrators;n++) if (maxylen<strlen(calibratorstring[n])) maxylen=strlen(calibratorstring[n]);
        if (X_origin==-1) x_origin+= maxylen * Hsize;
        ascal(&noofcalibrators, &xmin, &xmax, Logxflag, 1, Xformat,1);
/*
        for (n=0, maxxlen=0; n<noofcalibrators;n++) if (maxxlen<strlen(calibratorstring[n])) maxxlen=strlen(calibratorstring[n]);
        if (X_origin==-1 && (maxxlen+1)/2>maxylen) {
           dx=((maxxlen+1)/2-maxylen) * Hsize/2;
           x_origin+= dx;
           rightmargin+=dx;
        }
*/
        dx=(strlen(calibratorstring[0])*Hsize)/2;
        if (x_origin<dx+Rim && X_origin==-1) x_origin=dx+Rim;
        dx=(strlen(calibratorstring[noofcalibrators-1])*Hsize)/2;
        if (rightmargin<dx+Rim) rightmargin=dx+Rim;
        maxlegend= getmaxlegend();
        if (Statflag && Showstat && maxlegend<8) maxlegend=8;
        legendblocksize= maxlegend*((float)Hsize) + 3*DX + LEGEND_SYMBOL_SIZE;
        if (Line || (Statflag && calcflag)) legendblocksize+=LEGEND_SYMBOL_SIZE + DX;
        if (Legendpos==1 && ranges>1) {
           dx=legendblocksize+2*Rim;
           if (rightmargin<dx) rightmargin=dx;
        } else if (Statflag && Showstat) {
           dx=14*Hsize;
           if (rightmargin<dx) rightmargin=dx;
        }
        if (Ycal/3==0) {
          if (X_origin==-1) {
             x_origin+=Rim;
             rightmargin+=Rim;
          }
          rightmargin+=Rim+((Ycal==0)?Rim:0);
        }
        x_origin+=log0+Extra_rim;
        rightmargin+=Extra_rim;
        if (dual_axis) {
           ascal(&noofcalibrators, &y2min, &y2max, Logy2flag, 0, Yformat,0);
           for (n=0, maxlen2=0; n<noofcalibrators;n++) if (maxlen2<strlen(calibratorstring[n])) maxlen2=strlen(calibratorstring[n]);
           rightmargin+=maxlen2 * Hsize;
           dox=picy2_axis_title(y2_legend, y_origin + y_length,0)+Vsize;
           rightmargin+=dox;
        }
        x_length=X_length-rightmargin-log0-Rim;
        if (x_length<=100) x_length=100;

/**** scaling factors ****/

        scale_xfactor=(double)x_length/(xmax - xmin);
        scale_yfactor=(double)y_length/(ymax - ymin);
        if (dual_axis) scale_y2factor=(double)y_length/(y2max - y2min);
        xll=x_origin-log0;
        xul=x_origin+x_length;
        yll=y_origin;
        yul=y_origin+y_length;
     }
}

void read_environment(void) {
   char *s;
   s=getenv("TRACE");
   if (s) Trace=atoi(s);
   else Trace=TRACE;
   s=getenv("SHOW");
   if (s) Show=atoi(s);
   else Show=-1;
   s=getenv("OUTFILES");
   if (s) Outfiles=atoi(s);
   else Outfiles=-1;
   if (!Cfg) {
      s=getenv("CFG");
      if (s) strcpy(Cfg,s);
      else strcpy(Cfg,"PLAB.CFG");
   }
}

double pla(int startS, int NSd, int startT, int NTd) {
int N, NS, NT, start, n, df, i, ii, iii, repl;
double t7, cf, G;// G=grand_total=som of y's; cf=correctionfactor G^2/N
// startS, startT: array-position of standard/test
// NSd, NTd aantal standaard/test-concentraties
// NSi, NTi aantal replica's , 2 voor duplo, 3 voor triplo etc
// NS, NT totaal aantal standaarden/tests
// N=NS+NT

// N totaal aantal monsters
   double xx0,yy0,sx, sy, sxx, sxy, ssxx, ssxy, totaly, sst, ss, mean, syi;
//   double syb[MAXREPLICA]; was bedoeld voor complete block designs, echter: de replica's staan (partieel) gesorteerd in array
   double x_intercept, x_intercept0, y_intercept, temp;
   double YS=0.0; // som van alle standaard responses,  n=NSi*NSd=8
   double YT=0.0; // som van alle test responses,       n=NTi*NTd=6
   double YY=0.0; // kwadratensom van alle responses,   n=NSi+NTi=14
   double XS=0.0; // som van alle standaard doses,      n=NSi*NSd=8
   double XT=0.0; // som van alle test doses,           n=NTi*NTd=6
   double XXS=0.0; // kwadratensom van alle standaard doses,  n=NSi*NSd=8
   double XXT=0.0; // kwadratensom van alle test doses,       n=NTi*NTd=6
   double YYS=0.0; // kwadratensom van alle standaard responses,  n=NSi*NSd=8
   double YYT=0.0; // kwadratensom van alle test responses,       n=NTi*NTd=6
   double XYS=0.0; // produktsom van alle standaard responses,    n=NSi+NSd=8
   double XYT=0.0; // produktsom van alle test responses,         n=NTi*NTd=6
//   double YSi; // som van de Nsi standaard responses, n=NSi=2
//   double YTi; // som van de Nti test responses,      n=NTi=2
   double YSgem; // gemiddelde standaard repsons
   double YTgem; // gemiddelde test repsons
   double XSgem; // gemiddelde standaard dosis
   double XTgem; // gemiddelde test dosis
   double Sxxs, Sxxt, Sxys, Sxyt, Syys, Syyt;
double M, Mup, Mlow, lim, s;
//double potency;
double SSprep, SSreg, SSpar, SSlins, SSlint, SSbetw, SStot, SSres, MSres;
// SSblock,
double Fprep, Fpar, Flins, Flint;
double lodose, middose, hidose, ll, ul;

/**/ double lim0,lim1,lim2,lim3,lim4, lim10,lim11,lim12;

   int dataline, range, r, totaln=0, doublelog;
//   doublelog=(Statflag==2 && Vc>0.001);
if (NSd<2 || NTd<2) {
   cprintf("Too few datapoints: NSd=%d NTd=%d\r\n",NSd,NTd);
   exit(-1);
}
   doublelog=(Statflag==2);
   for (i=0,NS=0;i<NSd;i++) NS+=ny[startS+i];
   for (i=0,NT=0;i<NTd;i++) NT+=ny[startT+i];
   N=NS+NT; df=N-NSd-NTd;
   if (Trace==2) printf("%s %4d : units=%.3f NS=%d NT=%d N=%d df=%d doublelog=%d\n",__FILE__, __LINE__,units, NS,NT,N,df,doublelog);
   if (df==4) t7=2.776;
   if (df==5) t7=2.5706;
   if (df==6) t7=2.447;
   if (df==7) t7=2.3646;
   if (df==8) t7=2.306;
   if (df==9) t7=2.262;
   if (df==10) t7=2.228;
   if (df==11) t7=2.201;
   if (df==12) t7=2.179;
   if (df==13) t7=2.160;
   if (df==14) t7=2.145;
   if (df==15) t7=2.131;
   if (df==16) t7=2.120;
   if (df==17) t7=2.110;
   if (df==18) t7=2.101;
   if (df==19) t7=2.093;
   if (df==20) t7=2.086;
   if (df==21) t7=2.080;
   if (df==22) t7=2.074;
   if (df==23) t7=2.069;
   if (df==24) t7=2.064;
   if (df==25) t7=2.060;
   if (df==26) t7=2.056;
   if (df==27) t7=2.052;
   if (df==28) t7=2.048;
   if (df==29) t7=2.045;
   if (df>=30 && df<40) t7=2.03;
   if (df>=40 && df<60) t7=2.01;
   if (df>=60 && df<90) t7=1.99;
   if (df>=100 && df<120) t7=1.97;
   if (df>=120)  t7=1.960;


// bereken XSgem en XTgem, YSgem en YTgem
   XS=XT=YS=YT=YY=XXS=XXT=YYS=YYT=XYS=XYT=YSgem=YTgem=XSgem=XTgem=SSbetw=0.0;
   repl=(NSd==NTd)?ny[startS]:0;
//   for (i=0; i<MAXREPLICA; i++) syb[i]=0.0;
      for (r=0; r<NSd; r++) {
         dataline=startS+r;
         if (x[dataline]<=0) continue;
         xx0=log10(x[dataline]);
         XS+=ny[dataline]*xx0;
         XXS+=ny[dataline]*xx0*xx0;
//         XS+=xx0;
         for (i=0, syi=0.0; i<ny[dataline]; i++) {
            yy0=(i==0)?y[1][dataline]:y[i+2][dataline];
            if (yy0==MISSING) continue;
            if (repl!=ny[dataline]) repl=0;
            if (doublelog) yy0=transform(yy0,Sd,Vc);
            YS+=yy0;
//          syb[i]+=yy0;
            syi+=yy0;
            YYS+=yy0*yy0;
            XYS+=xx0*yy0;
            YY+=yy0*yy0;
if (Trace) printf("%s %4d: [%d,%d]: y=%7.3f YS=%12.4lf YY=%12.4lf YYS=%12.4lf\n",__FILE__, __LINE__,r,i,yy0,YS,YY,YYS);
         }
         SSbetw+=syi*syi/ny[dataline];
      }
      for (r=0; r<NTd; r++) {
         dataline=startT+r;
         if (x[dataline]<=0) continue;
         xx0=log10(x[dataline]);
         XT+=ny[dataline]*xx0;
         XXT+=ny[dataline]*xx0*xx0;
         for (i=0, syi=0.0; i<ny[dataline]; i++) {
            yy0=(i==0)?y[1][dataline]:y[i+2][dataline];
            if (yy0==MISSING) continue;
            if (doublelog) yy0=transform(yy0,Sd,Vc);
            YT+=yy0;
//          syb[i]+=yy0;
            syi+=yy0;
            YYT+=yy0*yy0;
            XYT+=xx0*yy0;
            YY+=yy0*yy0;
if (Trace) printf("%s %4d: [%d,%d]: y=%7.3f YT=%12.4lf YY=%12.4lf YYT=%12.4lf\n",__FILE__, __LINE__,r,i,yy0,YT,YY,YYT);
         }
         SSbetw+=syi*syi/ny[dataline];
      }
      G=YS+YT; cf=G*G/N; SStot=YY-cf; SSbetw-=cf;
/*
      SSblock=0.0;
      if (NSd==NTd && repl) {
         for (i=0; i<repl; i++) {
            if (Trace) printf("%s %4d: block[%d]: y=%7.3lf\n",__FILE__, __LINE__,i,syb[i]);
            SSblock+=syb[i]*syb[i]/(NSd+NTd);
         }
         SSblock-=cf;
      }
*/
//    SSres=SStot-SSbetw-SSblock;
      SSres=SStot-SSbetw;
if (Trace) printf("YS=%.4lf YT=%.4lf XS=%.4lf XT=%.4lf SStot=%.6lf SSbetw=%.6lf SSres=%.6lf cf=%.4lf\n",YS,YT,XS,XT,SStot,SSbetw,SSres,cf);
if (Trace) printf("YY=%.4lf YYS=%.4lf+YYT=%.4lf=%.4lf XXS=%.4lf XXT=%.4lf XYS=%.4lf XYT=%.4lf\n",YY,YYS,YYT,YYS+YYT,XXS,XXT,XYS,XYT);
if (Trace==2) printf("NSd=%d NTd=%d NS=%d NT=%d\n",NSd,NTd,NS,NT);
      XSgem=XS/NS;
      XTgem=XT/NT;
      YSgem=YS/NS;
      YTgem=YT/NT;
if (Trace) printf("XSgem=%8.3lf XTgem=%8.3lf YSgem=%8.3lf YTgem=%8.3lf\n",XSgem, XTgem, YSgem, YTgem);

// bereken sigma's
   Sxxs=Sxys=Syys=0.0;
      for (r=0; r<NSd; r++) {
         dataline=startS+r;
if (Trace) printf("x[%2d]=%12.4f y[%2d]=%12.4f\n",dataline,x[dataline],dataline,y[2][dataline]);
         xx0=log10(x[dataline]);
         yy0=y[2][dataline];
         if (doublelog) yy0=transform(yy0,Sd,Vc);
//       SSbetw+=yy0*yy0*ny[dataline];
         xx0-=XSgem;
         yy0-=YSgem;
         Sxxs+=(double)ny[dataline]*xx0*xx0;
         Syys+=(double)ny[dataline]*yy0*yy0;
         Sxys+=(double)ny[dataline]*xx0*yy0;
      }
if (Trace) printf("%s %4d : Sxxs=%.4lf Syys=%.4lf Sxys=%.4lf SSbetw=%.6lf\n",__FILE__, __LINE__,Sxxs,Syys,Sxys,SSbetw);
   Sxxt=Sxyt=Syyt=0.0;
      for (r=0; r<NTd; r++) {
         dataline=startT+r;
if (Trace) printf("x[%2d]=%12.4f y[%2d]=%12.4f\n",dataline,x[dataline],dataline,y[2][dataline]);
         xx0=log10(x[dataline]);
         yy0=y[2][dataline];
         if (doublelog) yy0=transform(yy0,Sd,Vc);
//       SSbetw+=yy0*yy0*ny[dataline];
         xx0-=XTgem;
         yy0-=YTgem;
         Sxxt+=(double)ny[dataline]*xx0*xx0;
         Syyt+=(double)ny[dataline]*yy0*yy0;
         Sxyt+=(double)ny[dataline]*xx0*yy0;
      }
//    if (Trace) printf("SSbetw=%.6lf (YS+YT)*(YS+YT)/N=%.6lf\n",SSbetw,(YS+YT)*(YS+YT)/N);
//    SSbetw-=(YS+YT)*(YS+YT)/(double)(NS+NT);
if (Trace) {
   printf("%s %4d : Sxxt=%.6lf Syyt=%.6lf Sxyt=%.6lf SSbetw=%.6lf\n",__FILE__, __LINE__,Sxxt,Syyt,Sxyt,SSbetw);
   printf("Sxxs=XXS-XS*XS/NS=%.4lf %.4lf\n",XXS-(XS*XS)/(double)NS,Sxxs);
   printf("Sxxt=XXT-XT*XT/NT=%.4lf %.4lf\n",XXT-(XT*XT)/(double)NT,Sxxt);
   printf("Sxys=XYS-XS*YS/NS=%.4lf %.4lf\n",XYS-(XS*YS)/(double)NS,Sxys);
   printf("Sxyt=XYT-XT*YT/NT=%.4lf %.4lf\n",XYT-(XT*YT)/(double)NT,Sxyt);
   printf("Syys=YYS-YS*YS/NS=%.4lf %.4lf %.4lf\n",YYS-(YS*YS)/(double)NS,Syys,(Sxys*Sxys)/Sxxs);
   printf("Syyt=YYT-YT*YT/NT=%.4lf %.4lf %.4lf\n",YYT-(YT*YT)/(double)NT,Syyt,(Sxyt*Sxyt)/Sxxt);
}
      if (Sxxs+Sxxt<=0.0) {
         cprintf("ERROR: dose range = 0\r\n");
         exit(-1);
      }
      slope=(Sxys+Sxyt)/(Sxxs+Sxxt);
      if (slope==0.0) {
         cprintf("PROBLEM: slope=0.0; cannot calculate potency\r\n");
         exit(-1);
      }
      intercept[0]=YSgem-slope*XSgem;
      intercept[1]=YTgem-slope*XTgem;
      M=(intercept[1]-intercept[0])/slope;
/*
      intercept[0]=-XSgem+YSgem/slope;
      intercept[1]=-XTgem+YTgem/slope;
      M=(intercept[1]-intercept[0]);
*/
      potency=pow(10.0,M);
if (Trace) printf("%s %4d : slope=%.4lf intercept[0]=%.4lf intercept[1]=%.4lf M=%.4lf potency=%.4lf\n",__FILE__, __LINE__,slope,intercept[0],intercept[1],M,potency);
      potency*=units;
if (Trace) printf("%s %4d : potency*units=%.4lf\n",__FILE__, __LINE__,potency);
      if (NSd%2) middose=log10(x[startS+(NSd-1)/2]);
      else {
         dataline=startS+NSd/2-1;
         middose=(log10(x[dataline])+log10(x[dataline+1]))/2;
if (Trace) printf("x[%d]=%.3f x[%d+1]=%.3f middose=%.4lf\n",dataline,x[dataline],dataline+1,x[dataline+1],pow(10.0,middose));
      }
      lodose= middose-log10(2)/2;
      hidose= middose+log10(2)/2;
//      y_intercept=-slope*intercept[0];
      y_intercept=intercept[0];
      ll= y_intercept+slope*lodose;
      ul= y_intercept+slope*hidose;
if (Trace) printf("y_intercept=%.5lf ll=%.5lf ul=%.5lf\n",y_intercept,ll,ul);
      if (doublelog) {
         ll= backtransform(ll, Sd, Vc);
         ul= backtransform(ul, Sd, Vc);
      }
      slope2=(ul/ll)-1.0;
if (Trace){
printf("%s %4d : lodose=%.5lf middose=%.5lf hidose=%.5lf ll=%.5lf ul=%.5lf slope2=%.5lf\n",
__FILE__, __LINE__,
lodose,
middose,
hidose,
ll,
ul,
slope2);
}
if (Trace) printf("SSprep: YS*YS/NS+YT*YT/NT=%.4lf (YS+YT)*(YS*YT))/N=%.4lf\n", YS*YS/NS+YT*YT/NT,((YS+YT)*(YS+YT))/N);
if (Trace) printf("SSreg : Sxys+Sxyt=%.4lf Sxxs+Sxxt=%.4lf\n",Sxys+Sxyt,Sxxs+Sxxt);
if (Trace) printf("SSpar : Sxys*Sxys/Sxxs=%.4lf Sxyt*Sxyt/Sxxt=%.4lf\n", (Sxys*Sxys)/Sxxs, (Sxyt*Sxyt)/Sxxt);
if (Trace==2) printf("%s %4d: slope2=%.4lf slope=%.4lf intercept[0]=%.4lf intercept[1]=%.4lf M=%.4lf Sxxs=%.4lf Sxxt=%.4lf Sxys=%.4lf Sxyt=%.4lf Syys=%.4lf Syyt=%.4lf\n",
 __FILE__,__LINE__,slope2, slope,intercept[0],intercept[1], M,Sxxs, Sxxt, Sxys, Sxyt, Syys, Syyt);
      SSprep=YS*YS/NS+YT*YT/NT-((YS+YT)*(YS+YT))/N;
      SSreg= ((Sxys+Sxyt)*(Sxys+Sxyt))/(Sxxs+Sxxt);
      SSpar= (Sxys*Sxys)/Sxxs + (Sxyt*Sxyt)/Sxxt - SSreg;
      SSlins= Syys - (Sxys*Sxys)/Sxxs;
      SSlint= Syyt - (Sxyt*Sxyt)/Sxxt;
//    SSbetw= SSprep+SSreg+SSpar+SSlins+SSlint;

//    SStot=YY-(YS+YT)*(YS+YT)/(double)N;
//    SSres=SStot-SSbetw;
      MSres=(df>0)?SSres/df:SSres;
      if (SSres<0.0) {
         printf("ERROR!: SStot=%.4lf SSbetw=%.4lf SSres=%.4lf\n",SStot,SSbetw,SSres);
         SSres=0.0;
      }
//      MSres=SSres/(NSd+NTd);
if (Trace) printf("%s %4d df=%d:\n"
              "SSprep = %12.6lf\n"
              "SSreg  = %12.6lf\n"
              "SSpar  = %12.6lf\n"
              "SSlins = %12.6lf\n"
              "SSlint = %12.6lf\n"
              "SSbetw = %12.6lf (%.6lf)\n"
              "SStot  = %12.6lf\n"
              "SSres  = %12.6lf\n"
              "MSres  = %12.6lf\n",
        __FILE__,__LINE__,df,SSprep,SSreg,SSpar,SSlins,SSlint,SSbetw,SSprep+SSreg+SSpar+SSlins+SSlint,SStot, SSres, MSres);
      Fprep=(MSres==0)?MISSING:SSprep/MSres;
//    Freg=(MSres==0)?MISSING:SSreg/MSres;
      Fpar=(MSres==0)?MISSING:SSpar/MSres;
      Flins=(MSres<=0 || NSd<3)?MISSING:(SSlins/(NSd-2))/MSres;
      Flint=(MSres<=0 || NTd<3)?MISSING:(SSlint/(NTd-2))/MSres;
      s=(MSres<0)?MISSING:sqrt(MSres);
if (Trace) printf("s=%.6lf slope=%.6lf Sxxs=%.6lf Sxxt=%.6lf\n",s,slope,Sxxs,Sxxt);
      g = (MSres<0 || Sxxs+Sxxt==0)?MISSING:t7 * t7 * MSres / (slope * slope * (Sxxs+Sxxt));
if (Trace) printf("g=%.6lf MSres=%.6lf Sxxs=%.6lf Sxxt=%.6lf\n",g,s*s,Sxxs,Sxxt);
if (Trace) printf("1/NS +1/NT=%12.6lf\n",(1.0/(double)NS +1.0/(double)NT));
if (Trace) printf("s*t7/slope=%.6lf 1-g=%.6lf (M-XSgem+XTgem) * (M-XSgem+XTgem)=%.6lf\n",s*t7/slope,1-g,(M-XSgem+XTgem) * (M-XSgem+XTgem));
/**/
      lim1=1-g;
      lim2=1.0/(double)NS +1.0/(double)NT;
      lim3=M-XSgem+XTgem;
      lim4=Sxxs+Sxxt;
      lim0=(s*t7)/slope;
      lim10=(lim4==0.0)?0:lim1 * lim2 + lim3*lim3/lim4;
      lim11=(lim10<0)?0:sqrt(lim10);
      lim12=lim0*lim11;
      if (Trace) printf("lim0=%.6lf lim1=%.6lf lim2=%.6lf lim3=%.6lf lim4=%.6lf\n",lim0,lim1,lim2,lim3,lim4);
      if (Trace) printf("lim10=%.6lf lim11=%.6lf lim12=%.6lf\n",lim10,lim11,lim12);
/**/
      lim=(Sxxs+Sxxt>0 && slope && (1-g>=0) )?((s*t7)/slope) * sqrt((1-g) * (1.0/(double)NS +1.0/(double)NT)+((M-XSgem+XTgem) * (M-XSgem+XTgem))/(Sxxs+Sxxt)):MISSING;
if (Trace)    printf("slope=%.6lf t7=%.6lf s=%.6lf g=%.6lf lim=%.6lf\n",slope,t7,s,g,lim);
//      if (g>1.0 || g<0.0 || lim<0)
      if (g>1.0 || g<0.0)      {
         Mup=M;
         Mlow=M;
         printf("\nWARNING: g=%.4lf lim=%.4lf, invalid confidence interval!\n",g,lim);
      } else {
         Mup= XSgem-XTgem+(M-XSgem+XTgem+lim)/(1-g);
         Mlow=XSgem-XTgem+(M-XSgem+XTgem-lim)/(1-g);
      }
      upper=pow(10.0,Mup)*units;
      lower=pow(10.0,Mlow)*units;
if (Trace)    printf("Mup=%.6lf Mlow=%.6lf upper=%.6lf lower=%.6lf\n",Mup,Mlow,upper,lower);
      if (lower>upper) {
         temp=lower;
         lower=upper;
         upper=temp;
      }
if (Trace) printf("                           Fprep=%12.6lf\n",Fprep);
if (Trace) printf("Non-parallelism          : Fpar= %12.6lf (5.59)\n",Fpar);
if (Trace) printf("Non-linearity standard   : Flins=%12.6lf (4.74)\n",Flins);
if (Trace) printf("Non-linearity test sample: Flint=%12.6lf (5.59)\n",Flint);
   ssxx=ssxy=totaly=0.0;
      for (range=0 ;range< 2; range++) {
      sx=sy=sxx=sxy=0;
      if (range==0) {
         n=NSd;
         start=startS;
      } else {
         n=NTd;
         start=startT;
      }
      for (r=0; r<n; r++) {
         dataline=start+r;
         if (x[dataline]<=0) continue;
         xx0=log10(x[dataline]);
         yy0=y[2][dataline];
         if (doublelog) yy0=transform(yy0,Sd,Vc);
         sx+=xx0;
         sy+=yy0;
         sxx+=xx0*xx0;
         sxy+=xx0*yy0;
      }
      ssxy+=sxy-sx*sy/(double)n;
      ssxx+=sxx-sx*sx/(double)n;
      meanx[range]= sx/(double)n;
      meany[range]= sy/(double)n;
      islope[range]= (sxy-sx*sy/(double)n)/(sxx-sx*sx/(double)n);
      if (Trace) printf("%s %4d : islope[%d]=%12.6lf \n",__FILE__, __LINE__,range,islope[range]);

      totaly+=sy;
      totaln+=n;
   }
   if (Trace) printf("%s %4d : ssxy=%.4lf ssxx=%.4lf totaly=%.4lf totaln=%d\n",__FILE__, __LINE__,ssxy,ssxx,totaly, totaln);
   slope=ssxx>0?ssxy/ssxx:0;
   for (r=0; r<ranges; r++) intercept[r]=meany[r]-meanx[r] * slope;
   mean=totaly/(double)totaln;
   sst=ss=0;
   for (r=0; r<NSd; r++) {
      dataline=startS+r;
      xx0=log10(x[dataline]);
      yy0=y[2][dataline];
      if (doublelog) yy0=transform(yy0,Sd,Vc);
      sst+=(yy0-mean)*(yy0-mean);
      ss+=(yy0-(intercept[0] + slope * xx0)) * (yy0-(intercept[0]+ slope * xx0));
   }
   for (r=0; r<NTd; r++) {
      dataline=startT+r;
      xx0=log10(x[dataline]);
      yy0=    y[2][dataline];
      if (doublelog) yy0=transform(yy0,Sd,Vc);
      sst+=(yy0-mean)*(yy0-mean);
      ss+= (yy0-(intercept[1] + slope * xx0)) * (yy0-(intercept[1]+ slope * xx0));
   }
   if (Trace) printf("%s %4d : sst=%.4lf ss=%.4lf\n",__FILE__, __LINE__,sst,ss);
   r2=(sst>0)?(sst-ss)/sst:0;
   corrcoeff=sqrt(r2);
   x_intercept0=(slope)?pow(10.0,-intercept[0]/slope):-999.0;
   slope0=islope[0];
   x_intercept=(slope)?pow(10.0,-intercept[1]/slope):-999.0;
//   potency=(x_intercept && x_intercept!=-999.0)?units*x_intercept0/x_intercept:-999.0;
   printf("%12.4lf %12.4lf %12.4lf %8.4lf %2d %2d %2d %2d %6.4lf %12.4lf %12.4lf %12.4lf %12.4lf %12.4lf %.4lf %.4f %.4f \"%s\" \"%s\"\n",
         lower,
         potency,
         upper,
         lower==0?MISSING:upper/lower,
         startS+1,
         startS+NSd,
         startT+1-range_start[1],
         startT+NTd-range_start[1],
         corrcoeff,
//         (doublelog)?slope:slope/Eover2,
//         (doublelog)?slope:slope/5.0,
         slope2,
         (slope0==0)?MISSING:islope[1]/slope0,
         x_intercept0,
         x_intercept,
         g,
         lim,
         Sd,
         Vc,
         prnfile,
         range_name[1]);
   if ((fppla=fopen(plafile,"wt")) == NULL) {
      cprintf("Unable to write to %s\r\nPress Esc to quit, any other key to continue ",plafile);
      if (getch()==ESC) exit(-1);
      return slope2;
   }
   clearerr(fppla);
   for (i=0; i<2; i++) {
      for (ii=0; ii<range_size[i]; ii++) {
         for (iii=0; iii<ny[i+ii]+2; iii++) fprintf(fppla,"%12.4f",y[iii][i+ii]);
         fprintf(fppla,"\n");
      }
      fprintf(fppla,"\n");
   }
   x_intercept0=valid_data[0]?(slope==0)?MISSING:pow(10.0,-intercept[0]/slope):0.0;
   slope0=islope[0];
   fprintf(fppla,"\"PLA-mode: %s\" SD=%.4f Vc=%.4f\n",(Statflag==1)?"LinLog":"LogLog",Sd,Vc);
   fprintf(fppla,"\"Common slope =\" %12.4lf\n"
                 "\"slope2       =\" %12.4lf\n"
"         \"r^2 =\" %12.4lf\n",slope,slope2,r2);
   fprintf(fppla,"\"%24s\" \"%12s\" \"%11s\" \"%11s\"\n",
   "testname","rel.slope","x-intercept","potency");
   for (r=0; r<2; r++) {
//    if (valid_data[r]) {
         x_intercept=(slope==0)?MISSING:pow(10.0,-intercept[r]/slope);
//         potency=(x_intercept)?units*x_intercept0/x_intercept:-999.0;
//         y_intercept=(Statflag==2)?pow(10.0,intercept[r]):intercept[r];
         fprintf(fppla,"\"%24s\"  %12.4lf  %12.6lf  %12.4lf\n",
         range_name[r],
         slope0==0?MISSING:islope[r]/slope0,
         x_intercept,
         (r)?potency:units);
//    }
   }
   if (g!=MISSING) {
if (!potency) potency=1.0;
fprintf(fppla,"\"potency:\" %10.4lf %10.4lf %10.4lf (percent deviation: %.2lf %.2lf)\n\n", lower, potency, upper, (potency==0)?MISSING:100.0*lower/potency-100.0, (potency==0)?MISSING:100.0*upper/potency-100.0);
fprintf(fppla,"\"slope calibrator:\" %8.4lf; slope test sample: %8.4lf\n",slope0,islope[1]);
fprintf(fppla,"\"                           Fprep=\"%12.4lf\n",Fprep);
fprintf(fppla,"\"Non-parallelism          : Fpar= \"%12.4lf (5.59)\n",Fpar);
fprintf(fppla,"\"Non-linearity standard   : Flins=\"%12.4lf (4.74)\n",Flins);
fprintf(fppla,"\"Non-linearity test sample: Flint=\"%12.4lf (5.59)\n",Flint);
   }
   fclose(fppla);
   return 1;
}

int find_range(int *startS,int *sizeS, int *startT,int *sizeT) {
double slope, maxslope=MISSING, meany, diff, mindiff=-MISSING;
int i, imax, imin, doublelog;
//   doublelog=(Statflag==2 && Vc>0.001);
   doublelog=(Statflag==2);
   if (range_size[0]<2 || range_size[1]<2) return 0;
   *startS=0;*startT=range_start[1];
   if (range_size[0]>3) {
      *sizeS=4;
      for (i=0; *startS + *sizeS + i < range_size[0]; i++) {
         slope=getslope(i,*sizeS);
         if (slope==MISSING) continue;
         slope=fabs(slope);
         if (maxslope<slope) {
            imax=i;
            maxslope=slope;
         }
if (Trace) printf("n%s %4d : i=%d slope=%.4lf imax=%d maxslope=%.4lf\n",__FILE__, __LINE__,i,slope,imax, maxslope);
      }
if (Trace) printf("n%s %4d : imax=%d maxslope=%.4lf\n",__FILE__, __LINE__,imax, maxslope);
      *startS=imax;
      *sizeS=4;
   } else if (range_size[0]==3) {
      *startS=0;
      *sizeS=3;
   } else if (range_size[0]==2) {
      *startS=0;
      *sizeS=2;
   }
   meany=getmidresponse(*startS,*sizeS);
   if (doublelog) meany=transform(meany,Sd,Vc);
   *sizeT=(range_size[1]>2)?3:2;
   for (i=0; i<range_size[1]; i++) {
      diff=fabs(
         (doublelog?
//            transform(y[2][range_start[1]+i],Sd,Vc)-transform(meany,Sd,Vc):
            transform(y[2][range_start[1]+i],Sd,Vc)-meany:
            y[2][range_start[1]+i]-meany));
      if (mindiff>diff) {
         imin=i;
         mindiff=diff;
      }
if (Trace) printf("%s %4d : i=%d+%d meany=%.4lf diff=%.4lf imin=%d+%d mindiff=%.4lf\n",__FILE__, __LINE__,range_start[1],i,meany,diff,range_start[1],imin, mindiff);
   }
if (Trace) printf("\n%s %4d : imin=%d+%d mindiff=%.4lf\n",__FILE__, __LINE__,range_start[1],imin, mindiff);
   if (imin<2) *startT=range_start[1];
   else if (imin>range_size[1]-*sizeT) *startT=range_start[1]+range_size[1]-*sizeT;
   else if (
      *sizeT==2 && (
         fabs(
            (doublelog?
               transform(y[2][range_start[1]+imin-1],Sd,Vc):
               y[2][range_start[1]+imin-1]
            )
         )
         <
         fabs(
            (doublelog?
               transform(y[2][range_start[1]+imin+1],Sd,Vc):
               y[2][range_start[1]+imin+1]
            )
         )
      )
   ) *startT=range_start[1]+imin-1;
   else *startT=range_start[1]+imin;
   if (Trace) printf("%s %4d: range_start[1]=%d imin=%d startS=%d sizeS=%d startT=%d sizeT=%d\n",
      __FILE__,__LINE__,range_start[1], imin, *startS,*sizeS,*startT,*sizeT);
   return 1;
}

double getmidresponse(int start, int size) {
   int i, doublelog;
   double meany=0.0;
//   doublelog=(Statflag==2 && Vc>0.001);
   doublelog=(Statflag==2);
   for (i=start; i<start+size; i++) {
      meany+=
         (doublelog)?
            transform(y[2][i],Sd,Vc):
            y[2][i];
   }
   meany/=size;
   return (doublelog)?backtransform(meany,Sd,Vc):meany;
}

double getslope(int start, int size) {
   int i, doublelog;
   float xvalue, yvalue;
   double sx=0.0, sxx=0.0, sxy=0.0, sy=0.0, syy=0.0, slope;
//   doublelog=(Statflag==2 && Vc>0.001);
   doublelog=(Statflag==2);
   for (i=start; i<start+size; i++) {
      if (x[i]<=0) return MISSING;
      xvalue=log(x[i]);
      sx+=xvalue;
      sxx+=xvalue*xvalue;
      yvalue=(doublelog)?transform(y[2][i],Sd,Vc):y[2][i];
//   if (Trace) printf("%s %4d: getslope: i=%2d y=%12.4f yvalue=%12.4lf\n",__FILE__,__LINE__,i,y[2][i],yvalue);
      sy+=yvalue;
      sxy+=xvalue*yvalue;
      syy+=yvalue*yvalue;
   }
   slope= (size*sxy-sx*sy)/(size*sxx-sx*sx);
//   if (Trace) printf("%s %4d: getslope: start=%d size=%d slope=%12.4lf\n",__FILE__,__LINE__,start, size, slope);
   return slope;
}

void plot_info(char *infostring1, char *infostring2, int size) {
   int xpos;
   xpos= x_origin + x_length + ((Ycal==0)?Rim:0) + 64;
   picsize(size*9, size*8);
   Font=CreateFont(0,Font,size*WMFHSIZE,size*WMFVSIZE);
   picmove(xpos,90);
   pictext(0,BOTTOMLEFT,infostring2);
   picmove(xpos,50);
   sprintf(buffer,"sd=%.3f;cv=%.1f%%;%s",Sd,100*Vc,(Statflag==2)?"Log(Y+sd/cv)":"LinY");
   pictext(0,BOTTOMLEFT,buffer);
   picmove(xpos,10);
   pictext(0,BOTTOMLEFT,infostring1);
   picsize(Hsize,Vsize);
   Font=CreateFont(0,Font,HsizeWMF,VsizeWMF);
}

void plot_result(void) {
int xpos, dxpos, ypos;
char buffer[40];

      xpos= x_origin + x_length + ((Ycal==0)?Rim:0) + 64;
      dxpos=250;
//    xpos= X_length-legendblocksize-2*Rim+50;
      ypos= y_origin+y_length - 0.4*Vsize-(Xcal/3)*RIM-5*YSIZE;
      picmove(xpos, ypos);
      pictext(0, CENTERLEFT,"s");
      picmove(xpos+dxpos, ypos);
      sprintf(buffer,"=%6.3lf", slope2);
      pictext(0, CENTERLEFT, buffer);

      ypos-=YSIZE;
      picmove(xpos, ypos);
      pictext(0, CENTERLEFT,"rs");
      picmove(xpos+dxpos, ypos);
      sprintf(buffer,"=%6.3lf",islope[1]/slope0);
      pictext(0, CENTERLEFT, buffer);
      ypos-=YSIZE;
//      sprintf(buffer,"  2");
//      picmove(xpos, ypos);
//      pictext(0, CENTERLEFT, buffer);
//      ypos-=(int)((float)YSIZE*0.3);

      picmove(xpos, ypos);
      pictext(0, CENTERLEFT,"r");
      sprintf(buffer,"=%6.3lf",corrcoeff);
      picmove(xpos+dxpos, ypos);
      pictext(0, CENTERLEFT, buffer);
/*
      ypos-=YSIZE;
      picmove(xpos, ypos);
      if (g==MISSING) {
      pictext(0, CENTERLEFT,"g ????");
      } else {
         pictext(0, CENTERLEFT,"g");
         picmove(xpos+dxpos, ypos);
         sprintf(buffer,"=%6.3lf",g);
         pictext(0, CENTERLEFT, buffer);
      }
*/
      ypos-=YSIZE;
      picmove(xpos, ypos);
      pictext(0, CENTERLEFT,"conc");
      if (potency<0.1)      sprintf(buffer,"= %.4lf",potency);
      else if (potency<1.0) sprintf(buffer,"= %.3lf",potency);
      else if (potency<10)  sprintf(buffer,"= %.2lf",potency);
      else if (potency<100) sprintf(buffer,"= %.1lf",potency);
      else                  sprintf(buffer,"= %li",(long)potency);
      picmove(xpos+dxpos, ypos);
      pictext(0, CENTERLEFT, buffer);
      if (g!=MISSING) {
      ypos-=YSIZE;
      if (potency<0.1)      sprintf(buffer,"  (%.4lf /",lower);
      else if (potency<1.0) sprintf(buffer,"  (%.3lf /",lower);
      else if (potency<10)  sprintf(buffer,"  (%.2lf /",lower);
      else if (potency<100) sprintf(buffer,"  (%.1lf /",lower);
      else                  sprintf(buffer,"  (%li  /",(long)lower);
      picmove(xpos, ypos);
      pictext(0, CENTERLEFT, buffer);
      ypos-=YSIZE;
      if (potency<0.1)      sprintf(buffer,"%.4lf)",upper);
      else if (potency<1.0) sprintf(buffer,"%.3lf)",upper);
      else if (potency<10)  sprintf(buffer,"%.2lf)",upper);
      else if (potency<100) sprintf(buffer,"%.1lf)",upper);
      else                  sprintf(buffer,"%li)",(long)upper);
      picmove(xpos+dxpos, ypos);
      pictext(0, CENTERLEFT, buffer);
      }
}

void draw_regression(int n) {
 int i, x, y, xxx, yyy;
 Dword color;
 float min,max,yfactor;
 double a, yy2;
 int yas2,logflag;
// column=range_start[n];
 yas2=range_type[n];
 logflag=(yas2)?logy2flag:logyflag;
 min=(yas2)?y2min:ymin;
 max=(yas2)?y2max:ymax;
 yfactor=(yas2)?scale_y2factor:scale_yfactor;
 color=LINECOLOR(n);
 if (color!=PenColor) Pen=pen(PenStyle,PenWidth,color);
   a=(xmax-xmin)/SEGMENTS;
   if (logxflag) {
      x=x_origin;
      xx2=xmin;
   } else {
      xx2=(xmin>0)?xmin:xmax/100;
      x=x_origin + (xx2-xmin) * scale_xfactor;
      xx2=xx2>0?log10(xx2):xmin;
 }
   yy2=intercept[n] + slope*xx2;
   if (Statflag==2) yy2=backtransform(yy2,Sd,Vc);
   if (logflag==1) yy2=yy2>0?log10(yy2):min;
   if (logflag==2) yy2=yy2>0?transform(yy2,Sd,Vc):min;
   if (yy2<ymin) yy2=ymin;
   if (yy2>ymax) yy2=ymax;
   y= y_origin + (yy2-min) * yfactor;
   for (i=1; i<=SEGMENTS; i++) {
      xx2=xmin+i*a;
      xxx=x_origin + (xx2-xmin) * scale_xfactor;
      if (!logxflag) xx2=log10(xx2);
      yy2=intercept[n]+slope*xx2;
      if (Statflag==2) yy2=backtransform(yy2,Sd,Vc);
      if (logflag==1) yy2=yy2>0?log10(yy2):min;
      if (logflag==2) yy2=yy2>0?transform(yy2,Sd,Vc):min;
      if (yy2<min) yy2=min;
      if (yy2>max) yy2=max;
      yyy=y_origin + (yy2-min) * yfactor;
      if(x!=xxx && y!=yyy) {
         picdotted_line(x, y, xxx, yyy, n, color, 0); /* PIC-file */
         picdotted_line(x, y, xxx, yyy, n, color, 1); /* WMF-file */
      }
      x=xxx;
      y=yyy;
   }
}

double transform(float y, float sd, float vc) {
   double result;
   result=(vc<0.001)?y:y+sd/vc;
   if (result<=0) return MISSING;
/*
      cprintf("Error: Unable to log-transform y: %.3f*%.3f+%.3f=%.3lf\r\n",y,vc*100,sd,result);
      exit(-1);
*/
   return log10(result);
}

double backtransform(float y, float sd, float vc) {
   if (vc<0.001) return (Logyflag)?pow(10.0,y):y;
   return pow(10.0,y)-sd/vc;
}

double getmeany(float y1, float y2, float sd, float vc) {
   double q;
   if (vc<0.001) return (y1+y2)/2;
   q=(double)sd/vc;
   return (double) sqrt((y1+q)*(y2+q))-q;
}

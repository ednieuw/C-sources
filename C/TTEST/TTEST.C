/* TTest = uitgebreide t1: T-testplaatje uit lotus 123 via datafile */

#define STARTUP_MESSAGE1 cprintf("Ttest.EXE 2.63 dd %s",__DATE__)
#define STARTUP_MESSAGE2 cprintf("Inlichtingen en fouten-melding:")
#define STARTUP_MESSAGE3 cprintf("Rob Aalberse / Ed Nieuwenhuys")
#define STARTUP_MESSAGE4 cprintf("CLB 020 - 512 3158")

extern unsigned _stklen=10000;
#include "ttest.h"

/********************** GLOBAL VARIABLES *****************/
char **columntext;
char **defaultstring;       /* 6 default eval-strings */
char **evalstring;          /* = textstring + 2+2*MAX_COL */
char **evaltext;            /* (expanded evalstring) textstring + 2+3*MAX_COL */
char **extra_text;          /* textstring + 2+4*MAX_COL+SYMBOLS (10*extra-text)*/
char **rangetext;
char **symbol_legend;       /* textstring + 2+4*MAX_COL */
char **testname;            /* = textstring + 2 */
char **testunit;            /* = textstring + 2+MAX_COL */
char **textstring;          /* pointer_array for textstrings in textpointer_block */
char **pstring;             /* probability-strings: PSTRING */
char *buffer;
char *evalpointer;
char *samplecode_string;    /* = textstring + 1 */
char *samplenumber_string;  /* = textstring */
char *textpointer;          /* one block for most of the string_arrays; (2+4*MAX_COL+SYMBOLS+10)*[MAX_LEN+1] */

char Cut_off1[10]="M";
char Cut_off2[10]="M";
char First_title[MAX_LEN+1];
char Output_path[81]= "A:";
char Second_title[MAX_LEN+1];
char Symbol_string[20]="0123456789";
char Y_axis_title[MAX_LEN+1];

char calibratorstring[MAXNOOFCALIBRATORS][40];
char central_value_string[]= "median";
char completestring[MAX_COL+3];
char error_string[]= "(no range)";
char errstring[40];
char filename[41];
char ftt_filename[41];
char ftt_name[41]="NONE";
char flag[MAX_COL];
char graph_name[NOOFGRAPHS][31] = {
      "T-test",
      "Cumulative frequency curve",
      "Bar/Box",
      "Matrix of data dots",
      "Pearson correlation bar graph",
      "Spearman correlation bar graph",
      "Frequency positive bar graph",
      "Relative risk bar graph",
      "Odds ratio bar graph",
      "Dendrogram",
      "2D-clusterplot"};

char selectstring[]="@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu";
char header_vector[]= {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char info1[MAX_LEN+1];
char info2[MAX_LEN+1];
char operator_type;
char optiontext[NOOFOPTIONS][30] = {
         "Quit program",
         "Format file selection",
         "Mode of test selection",
         "Test selection",
         "Graph-type selection",
         "View graph",
         "Options for graph",
         "Legend modification",
         "Save graph",
         "Write statistics",
         "2nd-test selection"};
char outfile[MAX_LEN+1];
char range_string[40]= "inter-quartile range";
char rowlabel[MAX_ROW+1];
char startchars[MAXNOOFITEMS+1];
char stat_string[]= "median / iqr";
char statmode_string[4][24]= {
   "Paired, one-tailed",
   "Paired, two-tailed",
   "Non-paired, one-tailed",
   "Non-paired, two-tailed"};
char symbolstring[20]="0123456789";
char Symbol_color[20]="4926501378";
char selected_symbolstring[20]="1111111111";
char symbol_in_use[20];
char used_symbolstring[20];
char teststring[MAX_COL+3]="";
char token[80];
char tok_type;
char ttlog[]="{tt}.prn";
char ttpic[]="{tt}.pic";
char tt_file[81]   = "";
char y_axis_title[MAX_LEN+1];

unsigned char *symbol;

signed char test0[MAX_COL+1];
signed char test[MAX_COL+1];

int **casnum;            /* pointer to subselected data  */
int **extra_rank;
int **rank0;             /* voor Spearman: tijdelijke ranks */
int **rank;

int *medianrank;           /* used for quicksort ; [MAX_ROW]    */
int *N;
int *temprank;             /* used for quicksort ; [MAX_ROW]    */
int *valid_data0;
int *valid_data;
int *filltype;
int *pstart;
int *pend;
int Trace;
int Target_drive=TARGET_DRIVE;
int bottomlines;
int calc_flag;
int column_width;
int cormatrix_flag=0;
int error_status;
int ftt_flag;
int headerlines;
int layout_flag=FALSE;
int logflag;
int maxlegend;
int maxlegendsize;
int new_graph;
int new_stat;
int new_tests;
int noofcalibrators=MAXNOOFCALIBRATORS;
int nooftests;
int n_diff;
int option;
int ranges;
int picture_ready_flag=FALSE;
int prob_text_flag=FALSE;
int prob_pos_flag=FALSE;
int protocol_code;
int result=0;
int save_flag;
int sort_flag;
int sort_required;
int startup_drive;
int success=0;
int swapstat=0;
int symbols;
int symbol_size;
int symbolstart=-1;
int tests_per_range[MAX_COL+1];
int xpos;
int x_length;
int x_origin;
int yll;
int yul;                        /*yulflag: data_points found higher than Ymax*/
int y_length;
int y_origin;
int zeroflag[MAX_COL+1];

int columns;
int pic_no=0;
int rows;
int saveerror=0;
int true_rows;
int true_columns;

long low_pointer;
long high_pointer;
long bytecounter=0;

float **data0;             /* pointer to original data     */
float *cormatrix;          /* linearized matrix of correlation coefficients */
float *dist;               /* linearized matrix of distance coefficients */

float *cutoff_value;       /* MAX_COL */
float *i25;                /* 25-percentiles */
float *i50;                /* 50-percentiles */
float *i75;                /* 75-percentiles */
float *lower_limit;
float *max;
float *min;                /* MAX_COL */
float *tempdata;           /* used for quicksort ; [MAX_ROW]    */
float *upper_limit;
float *fill_min;
float *fill_max;
float *pvalue;
float lower_limit_diff;
float true_max;
float true_min;
float ttmax;
float ttmin;
float upper_limit_diff;
float vars[MAX_COL+1];
float ymax;
float ymin;

double *mean;
double *var;
double *sd;
double class_unit;
double lower_deviation;
double central_value;
double upper_deviation;
double maximum_value;
double mean_diff;
double minimum_value;
double scale_yfactor;
double inter;
double interval;
double sd_diff;
double value_of_calibrator[MAXNOOFCALIBRATORS];
double var_diff;

FILE *fpin;
FILE *fpout;
FILE *logfile;
FILE *fptemp;
#ifdef TRACE
   FILE *tracefile;
#endif

unsigned int t_value[][31]={
1000,  308, 1886, 1638,1533,1476,1440,1415,1397,1383,1372,1363,1356,1350,1345,1341,1337,1333,1330,1328,1325,1323,1321,1319,1318,1316,1315,1314,1313,1311,1282,
500,   631, 2920, 2353,2132,2015,1943,1895,1860,1833,1812,1796,1782,1771,1761,1753,1746,1740,1734,1729,1725,1721,1717,1714,1711,1708,1706,1703,1701,1699,1645,
250,  1271, 4303, 3182,2776,2571,2447,2365,2306,2262,2228,2201,2179,2160,2145,2131,2120,2110,2101,2093,2086,2080,2074,2069,2064,2060,2056,2052,2048,2045,1960,
100,  3182, 6965, 4541,3747,3365,3143,2998,2896,2821,2764,2718,2681,2650,2624,2602,2583,2567,2552,2539,2528,2518,2508,2500,2492,2485,2479,2473,2467,2462,2326,
50,   6366, 9925, 5841,4604,4032,3707,3499,3355,3250,3169,3106,3055,3012,2977,2947,2921,2898,2878,2861,2845,2831,2819,2807,2797,2787,2779,2771,2763,2756,2576,
25,  12732,14089, 7453,5598,4773,4317,4020,3833,3690,3581,3497,3428,3372,3326,3286,3252,3222,3197,3174,3153,3135,3119,3104,3091,3078,3067,3057,3047,3038,2807,
10,  31831,22326,10213,7173,5893,5208,4785,4501,4297,4144,4025,3930,3852,3787,3733,3686,3646,3610,3579,3552,3527,3505,3485,3467,3450,3435,3421,3408,3396,3090,
5,  0XFFFF,31598,12924,8610,6869,5959,5408,5041,4781,4537,4437,4318,4221,4140,4073,4015,3965,3922,3883,3850,3819,3792,3767,3745,3725,3707,3690,3674,3659,3291};

int   Int_array[NOOFINTS+1];
float Float_array[NOOFFLOATS+1];

/* local routines: */
void startup_message(int x, int y, int n, int len);
void down(char c,int n);
void frame(int hor,int vert);
void printstring(char c,int n);
void make_fttname(void);
void show_status(void);
/* locals: */
#ifdef TRACE
   void dump_address(void);
#endif
   /****************** graphic parameter values:
   	maxlen		:maximum length of string in screen-units
   	max_n		:maximum number of points in string
   	intra		:screen_units between points in string
    inter       :screen_units between tests
	class_unit	:screen_units between strings
	interval	:data_units between strings
	factor		:conversion data-units -> screen units
    ******************/

int main() {
   int t, n, xpos, ypos, result;
   int quit=0;
   char cli[MAX_LEN+1];
   char default_ftt[15];
   HLOFF;
   clrscr();
   HLON3; startup_message(1,20,4,40); HLOFF;
   gotoxy(1,1);
   Color=COLOR;
   layout_flag=TRUE;
   new_tests=TRUE;
   new_graph=TRUE;
   picture_ready_flag=FALSE;
   save_flag=FALSE;
   calc_flag=FALSE;
   sort_flag=FALSE;
   ftt_flag=FALSE;
   sort_required=TRUE;
   Se_flag=-1;
   Cutoff_flag=FALSE;
   logflag=Logflag=0;
   option=-1;
   startup_drive=getdisk();
   read_info("wksdata.inf");  /* also allocates arrays and initialises variables */
   cprintf("reading worksheet %s\r\nwksdata.inf: general information on worksheet\r\n",filename);
   Vsize=Textsize*9;
   Hsize=Textsize*8;
   read_environment();
   #ifdef TRACE
      if (Trace) open_tracefile("trace");
   #endif
/* Phase 1: read data */

   read_wksdata(); /* setdisk(startup_drive); */
   make_fttname();
   strcpy(default_ftt, ftt_filename);
   symbols=find_symbols_in_use(-2);
   for (t=0;t<columns;t++) test[t]=test0[t]=t;
   test[t]=test0[t]=-99;

/* Phase 2: organize data */

   clrscr();
   HLON3; startup_message(1,20,4,40); HLOFF;
   gotoxy(1,1);
   if (check_dosname(ftt_filename) && check_drive(ftt_filename)  ) ftt_flag=read_ftt(ftt_filename);
   gotoxy(1,1);clreol();
   make_subselection(nooftests?FALSE:TRUE);
   gotoxy(1,1);clreol();
   write_stat_strings();
   #ifdef TRACE
      if (Trace) {
         dump_address();
         dump_arrays();
      }
   #endif

/* Phase 3: start main loop */

   do {
      if (nooftests<1) {
         for (t=0; t<columns; t++) test[t]=test0[t]=t;
         test[t]=test0[t]=-99;
         nooftests=columns;
      }
      show_status();
      saveerror=0;
      cormatrix_flag=(Outcome_test==-1 && (Graph_type==PEARSON || Graph_type==SPEARMAN));
      xpos=48;
      ypos=13;
      gotoxy(xpos,ypos);
      frame(26,NOOFOPTIONS+2-!(Graph_type==DOTMATRIX || Graph_type==PEARSON || Graph_type==SPEARMAN || Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO)-(Graph_type==OGIVE));
      xpos+=2;
      ypos++;
      for (n=0;n<MAXNOOFITEMS && n<NOOFOPTIONS-!(Graph_type==DOTMATRIX || Graph_type==PEARSON || Graph_type==SPEARMAN || Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO)-(Graph_type==OGIVE);n++) {
         gotoxy(xpos,ypos+n);
         cprintf("%s\r\n",optiontext[n]);
         startchars[n]=optiontext[n][0];
      }
      startchars[NOOFOPTIONS]='\0';            
      option=(option==VIEW_GRAPH)?SAVE_GRAPH:VIEW_GRAPH;
      while ((result= strmenu1(
          optiontext[option],
          &option,
          NOOFOPTIONS-!(Graph_type==DOTMATRIX || Graph_type==PEARSON || Graph_type==SPEARMAN || Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO)-(Graph_type==OGIVE),
          xpos,
          ypos+option))==0);
      if ( result== -1) break;
      if (!(Graph_type==BAR_GRAPH || Graph_type==TTEST) && Median_flag==2) Median_flag=1;
      if (Median_flag==2) Prob_flag=FALSE;
      clrscr();
      fclose(fpout);
      fclose(logfile);
      fflush(stdin);
      switch (option) {
         case FORMAT_FILE_SELECTION:
            ftt_flag=TRUE;
            initialise_arrays();
            write_stat_strings();
            layout_flag=TRUE;
            new_tests=TRUE;
            new_graph=TRUE;
            picture_ready_flag=FALSE;
            save_flag=FALSE;
            calc_flag=FALSE;
            sort_flag=FALSE;
            sort_required=TRUE;
            for (t=0;t<columns;t++) test[t]=test0[t]=t;
            test[t]=test0[t]=-99;
            nooftests=columns;
            clrscr();
            fflush(stdin);
            cprintf("RETURN = use a pre-existing format file\r\n"
                    "ESC    = do NOT use format file, use default values instead\r\n");
            if (getch()!=ESC) {
               setdisk(0);
               strcpy(buffer,"*.FTT");
               clrscr();
               if (getfile(buffer)) {
                  strcpy(ftt_filename,buffer);
                  ftt_flag=read_ftt(ftt_filename);
               }
               setdisk(startup_drive);
            } else ftt_flag=FALSE;
            clrscr();
            nooftests= make_subselection(nooftests?FALSE:TRUE);
            break;
         case SELECTION_MODE:
            Subselect=getselectionmode();
            if (Subselect<0) break;
         case TEST_SELECTION:
            nooftests=make_subselection(TRUE);
            sort_flag=FALSE;
            calc_flag=FALSE;
            picture_ready_flag=FALSE;
            save_flag=FALSE;
            User_legend_flag=FALSE;
            write_stat_strings();
            if (Prob_flag) write_prob_strings(FALSE);
            break;
         case OUTCOME_TEST_SELECTION:
            Outcome_test=select_outcome_test();
            clrscr();
            fflush(stdin);
            layout_flag=TRUE;
            calc_flag=FALSE;
            picture_ready_flag=FALSE;
            save_flag=FALSE;
            User_legend_flag=FALSE;
            write_stat_strings();
            break;
         case GRAPH_SELECTION:
            Graph_type=select_graph(Graph_type);
            clrscr();
            fflush(stdin);
            if (
               Graph_type==PEARSON ||
               Graph_type==SPEARMAN ||
               Graph_type==RELATIVE_RISK ||
               Graph_type==ODDS_RATIO
            ) Outcome_test=select_outcome_test();
            layout_flag=TRUE;
            calc_flag=FALSE;
            layout_flag=FALSE;
            picture_ready_flag=FALSE;
            save_flag=FALSE;
            new_graph=TRUE;
/*
            if (Prob_flag && (Graph_type==TTEST || Graph_type==BAR_GRAPH)) write_prob_strings(FALSE);
*/
            write_stat_strings();
            break;
         case VIEW_GRAPH:
         case SAVE_GRAPH:
         case WRITE_STAT:
            swapstat=0;
            if (!layout_flag) write_stat_strings();
            if (Median_flag && sort_flag) make_centiles(nooftests,Outliers_flag);
            if (!(Graph_type==TTEST || Graph_type==BAR_GRAPH) && Median_flag==2) Median_flag=1;
            if (!picture_ready_flag) {
               switch (Graph_type) {
                  case TTEST:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     tt();
                     break;
                  case OGIVE:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     ogive();
                     break;
                  case BAR_GRAPH:
                     if (Stat==-1) if (!stat_menu()) break;
                     sort_required=(Median_flag);
                     if (!calc_flag) process_data();
                     hg(BAR_GRAPH);
                     break;
                  case DOTMATRIX:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     dotmatrix();
                     break;
                  case PEARSON:
                     sort_required=FALSE;
                     if (!calc_flag) process_data();
                     hg(PEARSON);
                     break;
                  case SPEARMAN:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     hg(SPEARMAN);
                     break;
                  case PERCENTAGE_POSITIVE:
                     sort_required=FALSE;
                     if (!calc_flag) process_data();
                     write_stat_strings();
                     hg(PERCENTAGE_POSITIVE);
                     break;
                  case RELATIVE_RISK:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     write_stat_strings();
                     hg(RELATIVE_RISK);
                     break;
                  case ODDS_RATIO:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     write_stat_strings();
                     hg(ODDS_RATIO);
                     break;
                  case DENDROGRAM:
                  case XYCLUSTER:
                     sort_required=TRUE;
                     if (!calc_flag) process_data();
                     write_stat_strings();
                     if (Mean_flag) hg(PEARSON);
                     else hg(SPEARMAN);
                     break;
               }
            }
            fflush(stdin);
            if (option==VIEW_GRAPH) {
               if (spawnlp(P_WAIT,"drawpic.exe","drawpic.exe",ttpic,NULL)==-1) {
                  cprintf("\r\nProblem showing PICture\r\nProbable causes:\r\n"
                     "- not enough memory\n\r"
                     "- file DRAWPIC.EXE not found\n\r\n\r"
                     "Press any key to continue\n\r");
                  if (getch()==ESC) farewell(pic_no);
               }
               fflush(stdin);
            }
            if (option==SAVE_GRAPH) {
               clrscr();
               cprintf("\r\nUpdating format-file \"%s\"\r\n",default_ftt);
               write_ftt(default_ftt);
               ftt_flag=TRUE;
               saveerror=0;
               if (!save_flag) make_ttname(tt_file, outfile, pic_no);
               if(!picsave(ttpic,tt_file,".PIC"))  saveerror=1;
               if(saveerror) break;
               pic_no++;
               strcpy(buffer, tt_file);
               strtok(buffer,".");
               strcat(buffer,".FTT");
               if (check_dosname(buffer)) {
                  sprintf(cli,"copy %s %s > {tt}.log", default_ftt, buffer);
                  system(cli);
               }
            }
            if (saveerror) break;  /* if no files saved break */
            if (option==SAVE_GRAPH || option==WRITE_STAT) {
               if (
                     !(
                        Stat==-1 &&
                           (
                              Graph_type==TTEST ||
                              (
                                 Graph_type==BAR_GRAPH &&
                                 Median_flag!=2
                              )
                           ) || (
                              Graph_type==OGIVE ||
                              Graph_type==DOTMATRIX
                           )
                      )
                  ) {
                  display_logfile();
                  gotoxy(1,23);
                  picsave(ttlog,tt_file,".PRN");
               }
            }
            if (save_flag) {
               n=strpos(tt_file,'.',0);
               if (n==-1) n=strlen(tt_file);
               strncpy(outfile,tt_file, n>0?n:MAX_LEN);
               if (n>=0 && n<MAX_LEN) outfile[n]='\0';
            }
            fflush(stdin);
            break;
         case OPTIONS:
            option=get_options(Graph_type);
            fflush(stdin);
            write_stat_strings();
            layout_flag=TRUE;
            save_flag=FALSE;
            picture_ready_flag=FALSE;
            break;
         case CHANGE_LEGEND:
            modify_text();
            fflush(stdin);
            picture_ready_flag=FALSE;
            save_flag=FALSE;
            break;
         case QUIT:
            quit=TRUE;
     }
     clrscr();
   } while (!quit);
   setdisk(startup_drive);
   farewell(pic_no);
   return 0;
}

#ifdef TRACE
void dump_address(void) {
   fprintf(tracefile,"%5ld columntext\n",getaddress(columntext));
   fprintf(tracefile,"%5ld defaultstring\n",getaddress(defaultstring));
   fprintf(tracefile,"%5ld evalstring\n",getaddress(evalstring));
   fprintf(tracefile,"%5ld evaltext\n",getaddress(evaltext));
   fprintf(tracefile,"%5ld extra_text\n",getaddress(extra_text));
   fprintf(tracefile,"%5ld rangetext\n",getaddress(rangetext));
   fprintf(tracefile,"%5ld symbol_legend\n",getaddress(symbol_legend));
   fprintf(tracefile,"%5ld testname\n",getaddress(testname));
   fprintf(tracefile,"%5ld testunit\n",getaddress(testunit));
   fprintf(tracefile,"%5ld textstring\n\n",getaddress(textstring));

   fprintf(tracefile,"%5ld casnum\n",getaddress(casnum));
   fprintf(tracefile,"%5ld extra_rank\n",getaddress(extra_rank));
   fprintf(tracefile,"%5ld rank0\n",getaddress(rank0));
   fprintf(tracefile,"%5ld rank\n\n",getaddress(rank));
   fprintf(tracefile,"%5ld data0\n\n",getaddress(data0));

   fprintf(tracefile,"%5ld textpointer\n",getaddress(textpointer));
   fprintf(tracefile,"%5ld textstring\n",getaddress(textstring));
   fprintf(tracefile,"%5ld textstring[0]\n",getaddress(textstring[0]));
   fprintf(tracefile,"%5ld defaultstring\n",getaddress(defaultstring));
   fprintf(tracefile,"%5ld defaultstring[0]\n",getaddress(defaultstring[0]));
   fprintf(tracefile,"%5ld defaultstring[5]\n\n",getaddress(defaultstring[5]));

   fprintf(tracefile,"%5ld Cut_off1\n",getaddress(Cut_off1));
   fprintf(tracefile,"%5ld calibratorstring\n",getaddress(calibratorstring));
   fprintf(tracefile,"%5ld graphname\n",getaddress(graph_name));
   fprintf(tracefile,"%5ld optiontext\n",getaddress(optiontext));
   fprintf(tracefile,"%5ld test\n",getaddress(test));
   
   fprintf(tracefile,"%5ld &y_origin\n",getaddress(&y_origin));
   fprintf(tracefile,"%5ld zeroflag\n",getaddress(zeroflag));
   fprintf(tracefile,"%5ld tracefile\n",getaddress(tracefile));
   fprintf(tracefile,"%5ld t_value\n",getaddress(t_value));
   fprintf(tracefile,"%5ld t_value[0]\n",getaddress(t_value[0]));
   fprintf(tracefile,"%5ld t_value[7]\n",getaddress(t_value[7]));
   fprintf(tracefile,"%5ld Int_array\n",getaddress(Int_array));
   fprintf(tracefile,"%5ld Float_array\nEnd of DUMP\n\n",getaddress(Float_array));
}
#endif

void startup_message(int x, int y, int n, int len) {
   gotoxy(x,y);
   frame(len,n+2);
   gotoxy(x+2,y+1);
   STARTUP_MESSAGE1;
   gotoxy(x+2,y+2);
   STARTUP_MESSAGE2;
   if (n>2) {
      gotoxy(x+2,y+3);
      STARTUP_MESSAGE3;
   }
   if (n>3) {
      gotoxy(x+2,y+4);
      STARTUP_MESSAGE4;
   }
}

/* local procedures */

void down(char c,int n) {
   int i;
   for (i=0;i<n;i++) {
      putch(c);
      putch('\n');
      putch('\b');
   }
}

void frame(int hor,int vert) {
   int x,y;
   textcolor(CYAN);
   textbackground(GREEN);
   x=wherex();
   y=wherey();
   putch(201);
   printstring(205,hor-2);
   putch(187);
   gotoxy(x+hor-1,y+1);
   down(186,vert-2);
   putch(188);
   gotoxy(x,y+1);
   down(186,vert-2);
   gotoxy(x,y+vert-1);
   putch(200);
   printstring(205,hor-2);
   gotoxy(x,y+vert+1);
   HLOFF;
}   

void printstring(char c,int n) {
   int i;
   for (i=0;i<n;i++) putch(c);
}

void make_fttname(void) {
   strcpy(outfile,filename);
   sprintf(ftt_filename,"%c:",Target_drive+'A');
   strcat(ftt_filename, filename+2);
   strtok(ftt_filename, ".");
   strcat(ftt_filename,".FTT");
}

void show_status(void) {
   int t;
   gotoxy(1,1);
   cprintf("Current worksheet         : ");
   HLON3; cprintf("\"%s\" \"%s\" \"%s\"\r\n",filename,info1,info2); HLOFF;
   cprintf("Dimensions                : ");
   HLON3; cprintf("%d data-rows, %d data-columns\r\n",rows,columns); HLOFF;
   cprintf("Current format-file       : ");
   HLON3; cprintf("%s\r\n", ftt_flag?ftt_filename:"---"); HLOFF;
   if (Cutoff_flag) cprintf("Cut-off values in 1st data-row\r\n");
   cprintf("Worksheet columns selected: ");
   HLON3; for (t=0; test0[t]!=-99 && t<nooftests;t++) cprintf(" %s",test0[t]>-1?numbertolotus(test0[t]+1,"  "):" "); HLOFF;
   cprintf("\r\n");
   if (Graph_type==PEARSON || Graph_type==SPEARMAN || Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO) {
      cprintf("2nd (outcome-) test       : ");
      HLON3;
      if (Outcome_test!=-1) cprintf("column %s \"%s\"", numbertolotus(Outcome_test+1,"  " ), testname[Outcome_test]);
      else if (Graph_type==PEARSON || Graph_type==SPEARMAN) cprintf("All %d tests", nooftests);
      else cprintf("Codes %s in column A", used_symbolstring);
      HLOFF;
      cprintf("\r\n");
   }
   if (Graph_type==DOTMATRIX && Rank_column) {
      cprintf("2nd (ranking-) test       : ");
      HLON3;
      if (Outcome_test!=-1) cprintf("column %s \"%s\"", numbertolotus(Outcome_test+1,"  " ), testname[Outcome_test]);
      else cprintf("Automatic");
      HLOFF;
      cprintf("\r\n");
   }
   cprintf("Sub-selection of data     : ");
   HLON3; cprintf("%s\r\n",Subselect?"YES":"NO"); HLOFF;
   if (*used_symbolstring) {
      cprintf("Symbols found             : ");
      HLON3; cprintf("%s\r\n",used_symbolstring); HLOFF;
   }
   cprintf("Graph type                : ");
   HLON; cprintf("%s%s\r\n",graph_name[Graph_type],(Graph_type==DENDROGRAM||Graph_type==XYCLUSTER)?(Mean_flag)?" (Pearson)":" (Spearman)":""); HLOFF;
   if (Graph_type==TTEST || Graph_type==BAR_GRAPH || Graph_type==OGIVE) {
      cprintf("Statistic                 : ");
      HLON3; cprintf("%s",(Stat==-1)?"none":range_string);
      if (Mean_flag && Prob_flag) cprintf(" %s",statmode_string[Mean_flag-1]);
      cprintf("\r\n"); HLOFF;
      cprintf("Log-transformation        : ", ymin);
      HLON3; cprintf("%s\r\n",Logflag?"YES":"NO"); HLOFF;
      cprintf("Legend for y-axis         : ");
      HLON3; cprintf("%s %s\r\n", y_axis_title, (User_legend_flag)?"(USER)":""); HLOFF;
   }
   cprintf("Free memory/PIC-file size : ");
   HLON3; cprintf("%ld/%ld\r\n",coreleft(), bytecounter); HLOFF;
   if(saveerror){
      gotoxy(1,19);
      HLON; cprintf("File(s) not saved"); HLOFF;
   }
}

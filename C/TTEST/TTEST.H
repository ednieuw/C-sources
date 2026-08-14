/* unsigned _stklen=10000; */

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

#define E1(s) cprintf("\r\nError writing file \"%s\" to disc: disc full?",s)
#define E2 cprintf("error reading file %s",f)
#define E3 cprintf("\r\nError: Out of memory: % columns and % rows\r\nPress any key ...", columns, rows)
#define E4 cprintf("cannot open input file \"%s\"\r\n",s)
#define E5 cprintf("cannot open output file \"%s\"\r\n",s)
/*
#define TRACE 1
*/
#define PROTOCOL_CODE 3         /* data: floats; MISSING= (float) &CCCCCCCC */
#define TRUE 1
#define FALSE 0
#define TARGET_DRIVE 0
#define MISSING -107374176.0    /* &CCCCCCCC */
#define MISSING_CALIBRATOR -999
#define MAX_ROW 512
#define MAX_COL 32
#define FILL_COL MAX_COL       /* number of columns for which block-fill is available */
#define PSTRING 10             /* number of probability-strings */
#define MAX_LEN 79
#define BUFFERSIZE 512
#define LEGEND 2              /* number of legends per line*/
#define LEGENDPOS 1           /* legends to the right of the graph */
#define SYMBOLS 10
#define NOOFOPTIONS 11
#define NOOFGRAPHS 11
#define MAXNOOFCALIBRATORS 8
#define MAXNOOFCLASSES 101
#define DEFAULTNOOFCLASSES 40
#define MARGIN 50
#define CENTILE 0.0
#define COMBI 50
#define CUT_OFF 2.0
#define CUT_OFF1 "Median"
#define CUT_OFF2 "Median"
#define YCAL 0                  /* type of axes drawn 0-3 */
#define TIP 1.5                 /* Size of tip in % of symbol_size */
#define CAP 0.3                 /* Cap size in % of bar-size */
#define STAT 0.8                /* length of stat-lines in tt-graph */
#define INTER_T 0.2             /* inter-tt distance in % of tt-size */
#define INTER_BAR 0.5           /* inter-bar distance in % of bar-size */
#define INTRA 16
#define MIN_SYMBOL_SIZE 8
#define MAX_SYMBOL_SIZE 48
#define PIXELS_PER_LINE 2
#define FILLTYPE 2
#define PLOTTER 1
#define RANGES_IN_HEADER TRUE
#define STAGGER FALSE
#define SWAP FALSE              /* range-names BELOW/ABOVE column-names */
#define MAXNOOFITEMS 80
#define OFF ' '
#define ON  '#'
#define RIM 100
#define XSIZE 150 /* horizontal size of legend block */
#define TEXTSIZE 15
#define LEGEND_SYMBOL_SIZE 64
#define VSIZE (TEXTSIZE*9)
#define HSIZE (TEXTSIZE*8)
#define X_ORIGIN (HSIZE/2)
#define X_LENGTH (3200-HSIZE)
#define Y_ORIGIN (VSIZE/2)
#define Y_LENGTH (2300 - VSIZE)
#define PICTOP 2300

enum {
ONE_TAILED_PAIRED=1,
TWO_TAILED_PAIRED,
ONE_TAILED_UNPAIRED,
TWO_TAILED_UNPAIRED
};

enum {
STRING_ARRAY=1,
SINGLE_STRING,
INTEG,
FLOATING,
INT_ARRAY,
FLOAT_ARRAY
};

/* STRING_ARRAY */
enum {
DA_evalstring,
DA_evaltext,
DA_extra_text,
DA_symbol_legend,
DA_testname,
DA_testunit,
DA_defaultstring,
DA_pstring,
DA_END
};

/*STRING*/
enum {
DS_cut_off1,
DS_cut_off2,
DS_first_title,
DS_second_title,
DS_symbol_string,
DS_y_axis_title,
DS_selected_symbolstring,
DS_symbol_color,
DS_END
};

/* INTEG */
enum {
DI_color,
DI_combi,
DI_cutoff_flag,
DI_datasymbol,
DI_graph_type,
DI_hsize,
DI_intra,
DI_iqr_flag,
DI_legend,
DI_legendpos,
DI_line,
DI_logflag2,
DI_logflag,
DI_mean_flag,
DI_median_flag,
DI_nn,
DI_noofclasses,
DI_no_ranges,
DI_outcome_test,
DI_outliers_flag,
DI_plotter,
DI_ranges_in_header,
DI_sd_flag,
DI_sem_flag,
DI_se_flag,
DI_showclasses,
DI_skipnegatives,
DI_small,
DI_stagger,
DI_stattext_flag,
DI_subselect,
DI_swap,
DI_symbol_size,
DI_textsize,
DI_user_legend_flag,
DI_vsize,
DI_x_length,
DI_ycal,
DI_zero,
DI_ortho,
DI_clxlength,
DI_fullname,
DI_distance,
DI_legend_symbol_size,
DI_literal,
DI_dotflag,
DI_rank_column,
DI_prob_flag,
DI_x_origin,
DI_END
};

/*FLOAT*/
enum {
DF_cap,
DF_centile,
DF_cut_off,
DF_inter_bar,
DF_inter_t,
DF_stat,
DF_tip,
DF_ymax,
DF_ymin,
DF_END
};

/* INT_ARRAY */
enum {
DIA_filltype,
DIA_pstart,
DIA_pend,
DIA_END
};

/* FLOAT_ARRAY */
enum {
DFA_fill_min,
DFA_fill_max,
DFA_pvalue,
DFA_END
};

#define NOOFINTS          (DI_END)
#define NOOFFLOATS        (DF_END)
#define NOOFSTRING_ARRAYS (DA_END)
#define NOOFSTRINGS       (DS_END)
#define NOOFINT_ARRAYS    (DIA_END)
#define NOOFFLOAT_ARRAYS  (DFA_END)

#define Color             (Int_array[DI_color])
#define Combi             (Int_array[DI_combi])
#define Cutoff_flag       (Int_array[DI_cutoff_flag])
#define Datasymbol        (Int_array[DI_datasymbol])
#define Graph_type        (Int_array[DI_graph_type])
#define Hsize             (Int_array[DI_hsize])
#define Intra             (Int_array[DI_intra])
#define Iqr_flag          (Int_array[DI_iqr_flag])
#define Legend            (Int_array[DI_legend])
#define Legendpos         (Int_array[DI_legendpos])
#define Line              (Int_array[DI_line])
#define Logflag2          (Int_array[DI_logflag2])
#define Logflag           (Int_array[DI_logflag])
#define Mean_flag         (Int_array[DI_mean_flag])
#define Median_flag       (Int_array[DI_median_flag])
#define Nn                (Int_array[DI_nn])
#define Noofclasses       (Int_array[DI_noofclasses])
#define No_ranges         (Int_array[DI_no_ranges])
#define Outcome_test      (Int_array[DI_outcome_test])
#define Outliers_flag     (Int_array[DI_outliers_flag])
#define Plotter           (Int_array[DI_plotter])
#define Prob_flag         (Int_array[DI_prob_flag])
#define Ranges_in_header  (Int_array[DI_ranges_in_header])
#define Sd_flag           (Int_array[DI_sd_flag])
#define Sem_flag          (Int_array[DI_sem_flag])
#define Se_flag           (Int_array[DI_se_flag])
#define Showclasses       (Int_array[DI_showclasses])
#define Skipnegatives     (Int_array[DI_skipnegatives])
#define Small             (Int_array[DI_small])
#define Stagger           (Int_array[DI_stagger])
#define Stattext_flag     (Int_array[DI_stattext_flag])
#define Subselect         (Int_array[DI_subselect])
#define Swap              (Int_array[DI_swap])
#define Symbol_size       (Int_array[DI_symbol_size])
#define Textsize          (Int_array[DI_textsize])
#define User_legend_flag  (Int_array[DI_user_legend_flag])
#define Vsize             (Int_array[DI_vsize])
#define X_length          (Int_array[DI_x_length])
#define X_origin          (Int_array[DI_x_origin])
#define Ycal              (Int_array[DI_ycal])
#define Zero              (Int_array[DI_zero])
#define Ortho             (Int_array[DI_ortho])
#define Clxlength         (Int_array[DI_clxlength])
#define Fullname          (Int_array[DI_fullname])
#define Distance          (Int_array[DI_distance])
#define Legend_symbol_size (Int_array[DI_legend_symbol_size])
#define Literal           (Int_array[DI_literal])
#define Dotflag           (Int_array[DI_dotflag])
#define Rank_column       (Int_array[DI_rank_column])

#define Cap               (Float_array[DF_cap])
#define Centile           (Float_array[DF_centile])
#define Cut_off           (Float_array[DF_cut_off])
#define Inter_bar         (Float_array[DF_inter_bar])
#define Inter_t           (Float_array[DF_inter_t])
#define Stat              (Float_array[DF_stat])
#define Tip               (Float_array[DF_tip])
#define Ymax              (Float_array[DF_ymax])
#define Ymin              (Float_array[DF_ymin])

/* pic.h: header for pic-files 280288 */

#define MOVE 160
#define DRAW 162
#define FILL 48
#define FILLO 208
#define COLOR 1         /* default number of colors */
#define MAXCOLOR 15     /* maximum number of colors */
#define COLOR0 176
#define TEXT 168
#define FONT 167
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

/* DAT.H 210488 */

#define HLON   textcolor(YELLOW); textbackground(RED)
#define HLON1  textcolor(BLUE);   textbackground(LIGHTRED)
#define HLON2  textcolor(YELLOW); textbackground(GREEN)
#define HLON3  textcolor(YELLOW); textbackground(BLUE)
#define HLON4  textcolor(GREEN);  textbackground(BLACK)
#define HLON5  textcolor(YELLOW); textbackground(BROWN)
#define HLOFF  textcolor(BLACK);  textbackground(LIGHTGRAY)
#define CLS    clrscr()
#define CURSOR(a,b) gotoxy(a,b)
#define MORE   gotoxy(70,25);HLON;cprintf(" MORE --> ");HLOFF;getch();CLS

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
#define TAB 9
#define BACKTAB 271
#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_Z 26

#define NOOFVARS MAX_COL+1

enum {
   USER=1,
   PAIRED,
   NON_PAIRED};

enum graph_type {
   TTEST=0,
   OGIVE,
   BAR_GRAPH,
   DOTMATRIX,
   PEARSON,
   SPEARMAN,
   PERCENTAGE_POSITIVE,
   RELATIVE_RISK,
   ODDS_RATIO,
   DENDROGRAM,
   XYCLUSTER
   };

#define GRAPH_TYPE          TTEST

enum program_level {
   QUIT=0,
   FORMAT_FILE_SELECTION,
   SELECTION_MODE,
   TEST_SELECTION,
   GRAPH_SELECTION,
   VIEW_GRAPH,
   OPTIONS,
   CHANGE_LEGEND,
   SAVE_GRAPH,
   WRITE_STAT,
   OUTCOME_TEST_SELECTION
   };

enum range {LARGE=0, MEDIUM, SMALL, LINEAR};

/* $start.c */

extern void allocate(void);
extern void clear_array1(void);
extern void farewell(int pic_no);
extern void initialise_arrays(void);
extern void no_room(void);

/* $select.c */

extern int   getselectionmode(void);
extern int   make_subselection(int flag);
extern int   select_graph(int default_type);
extern int   select_tests(void);
extern void  select_data(void);
extern int   select_outcome_test(void);

/* $data.c */
extern int    accept_data(float d, int warningflag);
extern void   adjust_minmax(void); /* corrects ymin/ymax for deviations extending beyond ttmin/ttmax */
/*
extern void count_excess(void);
*/
extern int    find_minmax(int cycle);
extern int    find_N(void); /* find the number of valid valid-data */
extern int    find_row(int row, int t, int flag);
extern void   find_values(int t, int logflag);
extern double find_prob(double tval, int df);
extern float  getdata(int row, int t, int flag);
extern void   get_diff(int t0, int t1);
extern float  get_ile(int start, int column,double ile, int flag);
extern int    get_mean(int warningflag);
extern double ilevalue(int n, double level, double centile);
extern void   make_centiles(int nooftests, int outliers_flag);
extern int    process_data(void);
extern void   qs(int t, int rows);
extern void   qs0(int left, int right);
extern void   rank_data(int nooftests);
extern void   rank_medians(void);
extern void   pearson(int logflag1, int logflag2);
extern void   spearman(void);
extern double t95(int df);
extern void   tt_minmax(void);
extern double tt_calc(int t0, int t1, int statmode);

/* $varia */
extern int    check_teststring(char *teststring, int columns);
extern int    count_char(char *s, char c);
extern void   dotted_h_line(int x0, int x1, int y0);
#ifdef TRACE
extern void   dump_arrays(void);
extern void   dump_block(unsigned char *pointer, int noofbytes);
#endif
extern int    empty_tests(int first_test, int last_test);
extern void   expand_evalstring(int t);
extern int    fillmode(void);
extern int    find_dx( int point, int n, int intra);
extern int    find_symbols_in_use(int subset);
extern char  *ftoa(float f);
extern long   getaddress(void far *address);
extern int    getkey(int flag);
extern int    get_bottomlines(int *range_type, int *testnamelines, int *evaltextlines);
extern int    get_headerlines(int range_type, int testnamelines, int evaltextlines);
extern int    get_legendlines(void);
extern int    get_maxlegend(void);
extern char   get_symbol(int datapoint, int column, int *row);
extern int    isdecimal(char c);
extern int    isdelimiter(char c);
extern int    isnumber(char *s);
extern int    isvoid(char *s);
extern char  *keycopy(char *target,char *source,int maxlen);
extern int    lettertonumber(char c);
extern int    lotustonumber(char *s);
extern int    not_substring(char *s, char *substring);
extern char   numbertoletter(int n);
extern char  *numbertolotus(int i, char *s);
extern char  *quoted(char *buffer, char *s);
extern void   reset_prob(void);
extern void   show_text(char **text, char *teststring, int startrow, int textlines, int screenlines);
extern char  *strinput(char *s);
extern char  *stripwhites(char *s);
extern char  *strleft(char *buffer, char *s, int n);
extern int    strmenu1(char *name, int *current_pos, int rows, int xpos, int ypos);
extern int    strpos(char *s,char c,int pos);
extern int    symbols_in_use(void);
extern void   toupper_evalstring(char *evalstring, int columns);
extern void   write_screen(int startrow, int rows, int nooflines, int new);
extern void   write_prob_strings(int show_flag);
extern void   write_stat_strings(void);

/* $opt.c */

extern int  get_options(int graph_type);
extern int  modify_text(void);
extern void read_environment(void);
extern int stat_menu(void);

/* $file.c */

extern int  check_dosname(char *s);
extern int  check_drive(char *s);
extern int  check_output_file(char *new_name, char *default_name, char *ext);
extern void display_logfile(void);
extern char *getfile(char *s);
extern char *make_ttname(char *tt_file, char *filename, int pic_no);
extern void openin(char *s);
extern void openout(char *s);
extern void open_logfile(char *s);
extern void open_tracefile(char *s);
extern int  picsave(char *source, char *target, char *ext);
extern void read_casenames(char *f);
extern void read_data(char *f, int protocol_code);
extern int read_ftt(char *s);
extern void read_ile(char *f);
extern void read_info(char *f);
extern void read_ranks(char *f);
extern void read_testunits(char *f);
extern void read_wksdata(void);
extern void write_ftt(char *s);
extern void read_print_ftt_info(int file_sel);

/* $parse.c */

extern int parse(char *evalstring,int row);

/* $tt.C */
extern void tt(void);
extern int write_ttlog(int warningflag);
extern void dotmatrix(void);
extern void draw_prob(void);

/* $hg.c */
extern void hg(int graph_type);
extern void write_bar(int x, int y, int y0, int lower_deviation, int central_value, int upper_deviation, int x_unit, float cap, int tip, int bar, int combi, int tt, int valid_data, int zeroflag);
extern void write_box(int xpos,int y_origin,int p10,int p25,int se_min,int median,int se_max,int p75, int p90,int x_unit, int valid_data);
extern void ogive(void);

/* $pic.c */

extern int  ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
extern void piccolumnlegend(int t, int xpos, int range_type, int testnamelines, int evaltextlines);
extern void picdotted_line(int x0, int y0, int x, int y, int type);
extern void picfill(int x0, int y0, int x, int y, int type);
extern int  picranges(int range_type, int testnamelines, int evaltextlines, double inter);
extern void picopen(char *s);
extern void piccorner(void);
extern void picextra_text(int x_origin, int y_origin, int lx, int ly);
extern void picmove(int x,int y);
extern void picfont(char font);
extern void piccolor(int color);
extern void picsize(int x,int y);
extern void picdraw(int x,int y);
extern void pictext(int direction,int position, char *s);
extern int  pictextlines(int xpos, int ypos, char *textline, int direction, int position);
extern void picend(void);
extern void picxy(int x,int y);
extern void picfirst_title(char *title, int picmidx);
extern void picsecond_title(char *title, int picmidx);
extern int  picy_cal(int logflag);
extern int  picy_axis_title(char *y_axis_string, int y_pos);
extern int  picbottomline(int n);
extern int  pictopline(int n);
extern void piclegend(int legends_per_line);
extern int  picstring(int datapoint, int column, int x, int y, int n, int intra,int size);
extern void picdot(int x,int y, int type, int size);

/* $drawpic.c */

//extern int drawpic(char *argvv);


/********************** GLOBAL VARIABLES *****************/
extern char **columntext;
extern char **defaultstring;
extern char **evalstring;
extern char **evaltext;
extern char **extra_text;
extern char **rangetext;
extern char **symbol_legend;
extern char **testname;
extern char **testunit;
extern char **textstring;
extern char **pstring;
extern char *buffer;
extern char *evalpointer;
extern char *samplecode_string;
extern char *samplenumber_string;
extern char *textpointer;

extern char Cut_off1[];
extern char Cut_off2[];
extern char First_title[];
extern char Output_path[];
extern char Second_title[];
extern char Symbol_string[];
extern char Symbol_color[];
extern char Y_axis_title[];

extern char calibratorstring[][40];
extern char central_value_string[];
extern char completestring[];
extern char error_string[];
extern char errstring[];
extern char filename[];
extern char flag[];
extern char ftt_filename[];
extern char ftt_name[];
extern char graph_name[][31];
extern char header_vector[];
extern char info1[];
extern char info2[];
extern char operator_type;
extern char optiontext[][30];
extern char outfile[];
extern char range_string[];
extern char rowlabel[];
extern char selectstring[];
extern char startchars[];
extern char stat_string[];
extern char statmode_string[][24];
extern char symbolstring[];
extern char symbol_in_use[];
extern char used_symbolstring[];
extern char selected_symbolstring[];
extern char teststring[];
extern char token[];
extern char tok_type;
extern char ttlog[];
extern char ttpic[];
extern char tt_file[];
extern char y_axis_title[];

extern unsigned char *symbol;
extern int Trace, Show, Target_drive;

extern int **casnum;
extern int **extra_rank;
extern int **rank0;
extern int **rank;

extern int *medianrank;
extern int *N;
extern int *temprank;
extern int *valid_data0;
extern int *valid_data;
extern int *filltype;
extern int *pstart;
extern int *pend;

extern int bottomlines;
extern int calc_flag;
extern int column_width;
extern int cormatrix_flag;
extern int data_flag;
extern int error_status;
extern int ftt_flag;
extern int headerlines;
extern int layout_flag;
/*
extern int logflag;
*/
extern int maxlegend;
extern int maxlegendsize;
extern int new_graph;
extern int new_stat;
extern int new_tests;
extern int noofcalibrators;
extern int nooftests;
extern int n_diff;
extern int option;
extern int outcome_test;
extern int picture_ready_flag;
extern int prob_text_flag;
extern int prob_pos_flag;
extern int protocol_code;
extern int ranges;
extern int result;
extern int save_flag;
extern int sort_flag;
extern int sort_required;
extern int startup_drive;
extern int success;
extern int swapstat;
extern int symbols;
extern int symbol_size;
extern int symbolstart;
extern int tests_per_range[];
extern signed char test0[];
extern signed char test[];
extern int xpos;
extern int x_length;
extern int x_origin;
extern int yll;
extern int yul;
extern int y_length;
extern int y_origin;
extern int zeroflag[];

extern int columns;
extern int pic_no;
extern int rows;
extern int true_rows;
extern int true_columns;

extern long low_pointer;
extern long high_pointer;
extern long bytecounter;

extern float **data0;
extern float *cormatrix;
extern float *dist;

extern float *cutoff_value;
extern float *i25;
extern float *i50;
extern float *i75;
extern float *lower_limit;
extern float *max;
extern float *min;
extern float *tempdata;
extern float *upper_limit;
extern float *fill_min;
extern float *fill_max;
extern float *pvalue;
extern float lower_limit_diff;
extern float true_max;
extern float true_min;
extern float ttmax;
extern float ttmin;
extern float upper_limit_diff;
extern float vars[];
extern float ymax;
extern float ymin;

extern double *mean;
extern double *var;
extern double *sd;
extern double class_unit;
extern double lower_deviation;
extern double central_value;
extern double upper_deviation;
extern double maximum_value;
extern double mean_diff;
extern double minimum_value;
extern double scale_yfactor;
extern double class_unit;
extern double inter;
extern double interval;
extern double sd_diff;
extern double value_of_calibrator[];
extern double var_diff;

extern FILE *fpin;
extern FILE *fpout;
extern FILE *logfile;
extern FILE *fptemp;
extern FILE *tracefile;

extern unsigned int t_value[][31];
extern int Int_array[];
extern float Float_array[];

extern char ch_ftt_info[MAX_LEN];
extern int  fl_ftt_info;
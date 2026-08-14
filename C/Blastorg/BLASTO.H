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

#define OFF ' '
#define ON  '#'
#define GROUP 8
#define LENFILENAME 80
#define MAXNOOFITEMS 320
#define NOOFLINES 20
#define MAXFILES MAXNOOFITEMS
#define MAX_LEN 40
//#define MORE gotoxy(70,25);highvideo();printf(" MORE --> ");normvideo();getch();clrscr()
//#define MISSING -999999.0
#define PAPERWIDTH      2240
#define LINE_HEIGHT 20
#define KYOBOTTOM	1000
#define PEN             5   /* pen diameter in dots */
#define FILLPEN         1   /* pen diameter in dots */
#define FONT1 10 /* 14-point bold proportional */
#define FONT6 15 /* 9-point */
#define MAXLEN 80

/*
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
*/

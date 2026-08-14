/* lotus 2 logit   EJN*/

/* l.prj file
l
cga.obj
herc.obj
egavga.obj
litt.obj
Compile in LARGE*/

#include "alloc.h"
#include "graphics.h"
#include "conio.h"
#include "io.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "process.h"
#include "dir.h"
#include "time.h"
#include "dos.h"
#include "math.h"
#include "fcntl.h"
#include "bios.h"
#include "ctype.h"

#define breedte_menu     35     /* Breedte van menu's */
#define MAXDISK   5
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define SPECIAL_KEY  0
#define UP     72
#define DOWN   80
#define HOME   71
#define ENDT   79
#define LEFT   75
#define UP     72
#define EXEC   13
#define ESC    27
#define RIGHT  77
#define PgUp   73
#define PgDn   81
#define DEL    83
#define SPACE  32
#define RETURN 13
#define TAB     9
#define CHBIN 0xF000    /* Blink Invers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */

#define MANUAL 0
#define LOAD    1       /* hoofdmenu items */
#define BEREKEN 2
#define XLINLOG 3
#define YLINLOG 4
#define PRNONOF 5
#define CHADIR  6
#define RESET   7
#define QUIT    8
#define PRTFILE 9
int AUTOMENU = FALSE;
static char titel[QUIT+1][breedte_menu];

#define BARCOLOR 0X2400

#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define BELL 7
#define MAXROWS 1500          /* maximale dimensies worksheet */
#define MAX_COL 3
#define MAX_YK_PNT 150        /* Maximaal 150 ijklijnpunten */
#define MAX_LEN 20
//#define MAX_LEN_NAAM 15        /* Lengte van de naam column */
#define MISSING -999999.0
#define H_WIDTH 6400          /* lengte en breedte mijn graphics screen */
#define V_WIDTH 4000
#define H_OFFSET 1200         /* offset linker onderhoek box plaatje */
#define V_OFFSET 650
#define H_SCALE 0.95          /* factor lengte en breedte box t.o.v. */
#define V_SCALE 0.95          /* overgebleven ruimte na offset */
#define SBOX 15               /* grootte viekant meetpunt in plaatje */
#define AS_AANTAL 28          /* aantal getallen voor as labeling */

#define DATUM __DATE__
int MAX_ROW;

struct ffblk ffblk;
unsigned disk;
char *directory;
char curdir[132];
int size;
int show;

double **data;
char   **label;
double **temp;
char   **naam;

char ykpunten[MAX_YK_PNT];   /*string voor actieve ijklijnpunten */
int rows=0, columns=0;
int labels,xx,yy,aa,rows_temp,kolom,Manualrows=0;
int print,lost,LXG,LYG,berekend,deleted;
double aantal=0;
double slope,intercept,C,D,reslogit;
double corr;
double somx,somy,upx,upy;

FILE *fp;
FILE *fpin;
FILE *fpout;
FILE *printer;
char fileload[MAXDIR+MAXFILE+5];
char outfile[MAXDIR+MAXFILE+5];
int  maindisk;
char maindir[MAXDIR];
char drawdir[MAXDIR];
char dir[MAXDIR];
int  geladen=0;
int  wis=1;
int  HAND;

 /* wordt gebruikt in graph_body en drawregressieline*/
double vxx,vyy;
double maxx,maxy,as_unit;
double minx,miny;
double x1,x2,y1,y2,xminlog,yminlog;
double offpixx,offpixy;
double lx,ly,mx,my,a,b;
double facx,facy;

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;     /* for palette info			*/

int far *SCHERMPTR;
int far *PTR  = (int far *) 0XB800L;
int 	 SIZE,FONT; 		                  /* wordt gebruikt in graphbody() */
int 	 MaxX, MaxY = 0;
double PICXF,PICYF;  		          /*  Scale Factors */
double xnop,ynop;
char   header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
long	 Pstarted,Fileinput,ManualInput,Rescalcs;
char   Adjustment_file[MAXPATH]="LOGIT.ADJ";

/****************************  FUNCTION PROTOTYPES ***************/
void   set_prn_filename(char *);
int    display_menu(int ,int );
int    menuver(int ,int ,int ,int, int );
int    load_menu(void);
void   makedate(int *, int *, int *,unsigned );
char   *filedir(char *);
void   changedir(char *);
int    parse(char *);
void   box(int ,int ,int ,int ,int ,int );
void   swrite(int ,int ,char * , int );
void   twrite(int ,int ,char *,int ,int );
void   initdisplay(void);
void   veeg(int ,int ,int ,int );
void   lveeg(int ,int ,int ,int ,char,unsigned );
void   prntest(void);
char   *remove_lt_space(char *);
double getd(FILE *);
void   read_wks(char *);
void   skip(FILE *,long );
void   display_labels();
int    calc(int , int );
void   fill_temp(int ,int );
double logit(double );
void   regressie(int , int );
void   teken(double,double);
void   moef(double ,double );
void   point(double ,double);
void   numm(double ,double ,int );
void   prnt(double ,double ,int ,char *);
void   prntc(double,double ,int ,char *);
void   screendump(void);
int    prnonoff(void);
void   picopen(void);
void   piccolor(int );
void   picsize(int );
void   pictext(int ,char *);
void   picend(void);
void   graph_body(void);
void   Initialize(void);
int    graph(void);
void   drawregressieline(double);
int    Check_Log(double check,int err);
void   mallocdata(void);
void   freemallocdata(void);
void   readprt(char *);
short  Make_Last_Adjustment_file(void);
short  Read_Last_Adjustment_file(void);
void	 EXIT(int errcode);
//void   sort2(int n,float *ra,float *rb);

extern void Manual_input(void);
extern void Manual_berekenen(void);
extern void krtdans(void);
/************************************************************/
/************************** MAIN **************************/

main(int argc,char **argv)
{
int  i,j,k,n,actie;
char filenop[20];
char drive[MAXDRIVE];
char dir[MAXDIR];
char file[MAXFILE];
char ext[MAXEXT];

printer  = fopen("LPT1","w");
lost     = FALSE;
print    = TRUE;
show     = FALSE;
LXG      = TRUE;
LYG      = FALSE;
berekend = FALSE;
upx=upy=99999999L;
strcpy(outfile,"");
fileload[0] = 0;
filenop[0] = 0;
actie=MANUAL;
MAX_ROW=MAXROWS;

clrscr();
initdisplay();

if (argv[1][0]=='?' ||argv[1][1]=='?' || argv[1][1]=='h')
 {
  swrite(1,0,"Start the program with LOGIT [1] [2], in which [1] is an",0X0700);
  swrite(1,1,"optional filename with the extention WK1, WKS of PRT.     ",0X0700);
  swrite(1,2,"If the program is started with a filename than the results ",0X0700);
  swrite(1,3,"will be calculated and printed without interference.      ",0X0700);
  swrite(1,4,"[1] or [2] may also be the maximum number of data rows that  ",0X0700);
  swrite(1,5,"will be read in the program. this may  be usefull if there",0X0700);
  swrite(1,6,"is a memory shortage. Coreleft is the memory left to the ",0X0700);
  swrite(1,7,"program. The maximum number of rows that can be absorbed",0X0700);
  swrite(1,8,"are printed in the menu.        ",0X0700);
  swrite(1,9,"The FIRST COLUMN must contain the CONCENTRATION of the cali- ",0X0700);
 swrite(1,10,"bration curve. ",0X0700);
 swrite(1,11,"The SECOND COLUMN must contain the MEASUREMENT VALUES.",0X0700);
 swrite(1,12,"The FIRST COLUMN may contain the name of the sample in text",0X0700);
 swrite(1,13,"The THIRD COLUMN may contain the dilution factor of the sample.",0X0700);
 swrite(1,14,"The FIRST ROW may contain text which will be printed along ",0X0700);
 swrite(1,15,"the axis of the graph.	      ",0X0700);
 swrite(1,16,"A PRT file is an ASCII file of this LOTUS file.",0X0700);
 swrite(1,17,"If the graph is show it can be printed on a EPSON printer by",0X0700);
 swrite(1,18,"pressing TAB.                    ",0X0700);
 swrite(1,19,"Delete or reinsert data points in the graph by pressing the ",0X0700);
 swrite(1,20,"corresponding character in the graph.",0X0700);
 swrite(1,22,"The results are saved in a PRN file with a name which is shown",0X0700);
 swrite(1,23,"in the menu. This file can be imported in any speadsheet.",0X0700);
 getch();
 clrscr();
  swrite(1,0,"  Conc     Units    Dilution",0X0700);
  swrite(1,1,"    0        4           ",0X0700);
  swrite(1,2,"    2        6           ",0X0700);
  swrite(1,3,"    4        9           ",0X0700);
  swrite(1,4,"    8       10           ",0X0700);
  swrite(1,5,"  MA01       9.6       1 ",0X0700);
  swrite(1,6,"  MA01       6.3       2 ",0X0700);
  swrite(1,7,"  MA01       3.5       4 ",0X0700);
  swrite(1,8,"  BLANK      -1        1 ",0X0700);
  swrite(1,9,"  MA02       8.6       1 ",0X0700);
  swrite(1,10,"  MA02       6         2 ",0X0700);
  swrite(1,11,"  MA02       3.5       4 ",0X0700);
  swrite(1,13,"Sample of a lotus worksheet or a ASCII PRT file",0X0700);
  swrite(1,17,"  Ed Nieuwenhuys ",0X0700);
  swrite(1,18,"  Jan van Gentstraat 7",0X0700);
  swrite(1,19,"  1171GH Badhoevedorp ",0X0700);
  swrite(1,20,"  Netherlands         ",0X0700);
 getch();
 clrscr();
 exit(1);
 }

if(argc>=2) /* als een file meegegeven in commandline */
	{
	if(argv[1][0]<58) { MAX_ROW=atoi(argv[1]); ext[0]=0;}
	 else
	  {
	  fnsplit(argv[1],drive,dir,file,ext);
	  if(stricmp(ext,".WK1")==0 || stricmp(ext,".WKS")==0)
	 {actie=LOAD; AUTOMENU = TRUE; strcpy(fileload,argv[1]);}
	  else if(stricmp(ext,".PRT")==0)
	 {actie=PRTFILE; AUTOMENU = TRUE; strcpy(fileload,argv[1]);}
	 }
	}
if(argc==3) /* als een MAXROW is meegegeven in commandline */
	 {
	  if(argv[2][0]<58) MAX_ROW=atoi(argv[2]);
	 }

mallocdata();
Read_Last_Adjustment_file();
Pstarted++;
strcpy(curdir,"?:\\");
curdir[0] = 'A'+getdisk();
maindisk = getdisk();
getcurdir(0,curdir+3);
if(curdir[(int)strlen(curdir)-1] != 92) strcat(curdir,"\\");
strcpy(maindir,(char *)curdir);

while(1)
	{
	wis = 1;

	if(!AUTOMENU)
	 actie=display_menu(wis,actie); /* Als geen filenaam in commandline */
	if (actie == -1) actie=QUIT;
	if(actie==99) {actie=MANUAL; continue;}
	switch (actie)
		{
		case PRTFILE:
		  veeg(0,0,25,80);
		  Manualrows=0;
		  lost =FALSE;
		  if(!AUTOMENU) strcpy(fileload,(char *)filedir("*.PRT"));
		  if (strlen(fileload) < 2) break;
		  strcpy(file,fileload);
		  berekend  = 0;
		  rows_temp = 0;
		  for(k=0;k<MAX_YK_PNT;ykpunten[k++]=FALSE);
		  geladen = 3;
		  veeg(0,0,25,80);
		  readprt(fileload);
		  strcpy(outfile,fileload);
		  actie = BEREKEN;
		  Fileinput++;
		  break;

		case MANUAL:
		  Manualrows=0;
		  veeg(0,0,25,80);
		  lost =FALSE;
		  berekend  = 0;
		  rows_temp = 0;
		  Manual_input();
		  if(geladen <2)
			  strcpy(fileload,"Manual.PRN");  // WK? of PRT niet geladen
		  ManualInput++;
		  actie = BEREKEN;
		  break;

		case LOAD:
		  veeg(0,0,25,80);
		  Manualrows=0;
		  lost = FALSE;
		  for(j=0;j<MAX_YK_PNT;j++)
			{
			 for(i=0;i<2;i++)  temp[i][j]=MISSING;
			 data[2][j]=1.0;
			 ykpunten[j]=FALSE;
			}
		  if(!AUTOMENU)  strcpy(fileload,(char *)filedir("*.WK?"));
		  if (strlen(fileload) < 2) break;
		  strcpy(file,fileload);
		  berekend = 0;
		  rows_temp = 0;
		  for(k=0;k < MAX_YK_PNT;ykpunten[k++]=FALSE);
		  geladen = 2;
		  veeg(0,0,25,80);
		  read_wks(fileload);
		  Fileinput++;
		  actie = BEREKEN;
		  break;

		case BEREKEN:
		  if(!geladen) break;
		  veeg(0,0,25,80);
		  strcpy(file,fileload);
		  set_prn_filename(strcat(strtok(file,"."),".PRN"));
		  n=calc(0,1);
		  if(geladen <2 && n!=2) Manual_berekenen();
		  fclose(fpout);
		  AUTOMENU = FALSE;
		  actie=MANUAL;
		  Rescalcs+=rows+Manualrows;
		  break;

		case XLINLOG:
		  if(LXG) LXG = FALSE;
			else  LXG = TRUE;
		  AUTOMENU = FALSE;
		  actie=MANUAL;
		  if(geladen) actie=BEREKEN;
		  break;

		case YLINLOG:
		  if(LYG) LYG = FALSE;
			 else LYG = TRUE;
		  AUTOMENU = FALSE;
		  actie=MANUAL;
		  if(geladen) actie=BEREKEN;
		  break;

	   case CHADIR:
		  veeg(24,0,1,80);
		  swrite(13,22,"RETURN or New directory: \0",0x1300);
		  gotoxy(39,23);
		  gets(dir);
		  if (strlen(dir) >1) changedir(dir);
		  strcpy(curdir,"?:\\");
		  curdir[0]='A'+getdisk();
		  getcurdir(0,curdir+3);
		  if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
		  veeg(23,0,2,80);
		  AUTOMENU = FALSE;
		  actie=MANUAL;
		  if(geladen) actie=BEREKEN;
		  break;

	   case PRNONOF:
		  if(print)           print = FALSE;
		    else {prntest();  print = TRUE;}
		  AUTOMENU = FALSE;
		  actie=MANUAL;
		  if(geladen) actie=BEREKEN;
		  break;

	   case RESET:
		  berekend=FALSE;
		  rows_temp = 0;
		  for(k=0;k<MAX_YK_PNT;ykpunten[k++]=FALSE);
		  actie=BEREKEN;
		  break;

		case QUIT:
		 default:
		  EXIT(1);
		  break;
		}
  }
}
/**********************  EINDE MAIN ***********************/


void set_prn_filename(char *filenaam)
{
/* if(filenaam[1]!=':') strcpy(outfile,"C:"); else strcpy(outfile,"");*/
 strcpy(outfile,filenaam);
/* strcpy(outfile,"C:RESULT.PRN");*/
 if ((fpout=fopen(outfile,"w")) == NULL)
	 {
	  printf("%s","Can not open outputfile");
     getch();
	  EXIT(1);
	 }
}

int display_menu(int wis,int barkeuze)
{
	int i,num,breed,keuze;
	char datum[12],nop[80],nopp[30];
	num=load_menu();                   /* laad tekst menu */
	if(wis) lveeg(0,0,25,80,'±',0x2500);
	strcpy(datum,DATUM);
	sprintf(nop,"Coreleft %lu bytes",farcoreleft());
	sprintf(nopp,"  Maxrows %d \0",MAX_ROW);
	strcat(nop,nopp);
	swrite(2,24,nop,0x2100);
	sprintf(nop,"S=%ld, F=%ld, M=%ld, C=%ld",
			Pstarted,Fileinput,ManualInput,Rescalcs);
	swrite(2,23,nop,0x2100);

	twrite(2,20,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\0",YELLOW,RED);
	twrite(2,21,"ºProblems?  Ed Nieuwenhuys   º\0",YELLOW,RED);
	twrite(2,22,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\0",YELLOW,RED);


	swrite(17,0,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\0",0x0600);
	swrite(17,1,"º    LOGIT                   º\0",0x0600);
	swrite(17,2,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\0",0x0600);
	swrite(34,1,datum,0x0600);
	swrite(17,3,curdir,0x0700);
	swrite(17,4,fileload,0x0700);
	if(lost) swrite(0,22,"*** ROWS MAY BE LOST ***",0X0700);
	if(geladen){
	 fill_temp(0,1);
	 textcolor(BLACK);       textbackground(BROWN);
	 gotoxy(60,3); cprintf("   Blank");
	 gotoxy(70,3); cprintf("%8.4g\r\n",C);
	 for(i=0;i<20;i++)
	 {
	 if(ykpunten[i])
		{
		 gotoxy(60,i+4); cprintf("%8.4g\r\n",temp[0][i]);
		 gotoxy(70,i+4); cprintf("%8.4g\r\n",temp[1][i]);
		}
	 }
		}
	gotoxy(1,1);
	breed=breedte_menu;
	keuze = menuver(num,5,17,breed-5, barkeuze);
   if (keuze==QUIT || keuze==-1) veeg(0,0,20,80);
   return(keuze);

}


int menuver(int num,int ltopy,int ltopx,int breed, int barkeuze)
{
	time_t first, second;
	long secs_now;
	char *str_now;
	int x, y, code, code1, oke, yy, xx, keer,leave,keus;
	int keuze;
   char ch;

   yy = ltopy;
   xx = ltopx;
   oke = 0;
   keuze = barkeuze;
   box(yy,xx,num,breed,1,0);


   while (1)
	 {
	 yy=ltopy;
	 for (y=0; y<num; y++)
	  {
	   yy++;
		xx=ltopx+1;
	   for (x=0; x<strlen(titel[y]); x++)
		{
		xx++;
		ch = titel[y][x];
		if (y==keuze)   *(SCHERMPTR + yy*80 + xx) = ch | 0x7000;
		else
		  if (x==0)     *(SCHERMPTR + yy*80 + xx) = ch | 0x1300;
			else    *(SCHERMPTR + yy*80 + xx) = ch | 0x4700;
		 }
		}
	  if(oke == 1)		return(keuze);


	  first = time(NULL);  /* Gets system time */
	  while(bioskey(1)==0)
		{
		 second = time(NULL); /* Gets system time again */
		 if(difftime(second,first)>60)
		    {
		     krtdans();
			  return(99);
			 }

		 time(&secs_now);
		 str_now=ctime(&secs_now);
		 xx=55;
		 str_now[24]='\0';
		 swrite(xx,0,str_now,0x1300);
		 }
	   code = getch();
	   code1=SPECIAL_KEY;
	   if(code==SPACE) {code1=SPACE; code=SPECIAL_KEY;}
	   if(code == SPECIAL_KEY)
		 {
	     if(code1 !=SPACE)  code1 = getch();
	     switch (code1)
	       {
			   case UP:
			   case LEFT:    --keuze;         break;
			   case SPACE:
			   case DOWN:
			   case RIGHT:   ++keuze;         break;
			   case HOME:
				case PgUp:      keuze = 0;     break;
			   case ENDT:
			   case PgDn:      keuze = num-1; break;
	       }
				if(keuze < 0)   keuze = num-1;
				if(keuze > num-1)   keuze = 0;
		  }
		else
		{
		  ch = code;  keus=keuze;  leave=0;   keer=-1;
		  while(keer<2)
			{
			 keer++;
			 if(keer==1) { keus=-1;}
			 for(y=keus+1; y<num; y++)
			  {
				if(strnicmp(&titel[y][0],&ch,1) == 0)
			{ keuze = y; oke = 0; leave=1;  keer=2;  }
				if(leave) break;
			  }
			 }
		  oke = 1;
		  if(ch == ESC) {    keuze = -1;   oke = 1;   }
		}
	}
}

/********************** LOAD MENU ********************************/
int load_menu()
{
	int nr=QUIT+1;
	  strcpy(titel[0],"Manual input              \0");
	  strcpy(titel[1],"Load LOTUS file           \0");
	  strcpy(titel[2],"Calculate                 \0");
	if(LXG)
	  strcpy(titel[3],"X-axis LOG/lin            \0");
	 else
	  strcpy(titel[3],"X-axis log/LIN            \0");
	if(LYG)
	  strcpy(titel[4],"Y-axis LOG/lin            \0");
	 else
	  strcpy(titel[4],"Y-axis log/LIN            \0");
	if (print)
	  strcpy(titel[5],"Print ON/off              \0");
	else
	  strcpy(titel[5],"Print on/OFF              \0");
	  strcpy(titel[6],"Other drive / directory   \0");
	  strcpy(titel[7],"Reset datapoints          \0");
	  strcpy(titel[8],"Quit                      \0");
  return(nr);
 }



/***************** haal datum uit ffblk.ff_fdate ************************/
void makedate(int *d, int *m, int *y,unsigned dosdate)
{
 *m = (dosdate >>5)&0XF;
 *d = dosdate & 0x1F;
 *y = (dosdate >>9) & 0X3F;
 *y += 80;
 }


/* Directory listen op scherm en keuze maken ****************************** */
/*                                                                          */
/* char dir 			zoek sleutel directory                      */
/*                                                                          */
/* ************************************************************************ */

char *filedir(char *filein)
{
   char nop[30];
   int y,leave;
   static char filenaam[250][30];// nr,breedte filenaam + datum
	char ch;			 // te printen char
   int begin = 0;                // 1e file uit blok van 25 dat geprint word
   int keuze = 0;		 // nummer gekozen file
   int num = 0;                  // aantal files
   int oke;                      // 0 = geen keuze ESC 1 = keuze RETURN
   int toets;			 // toetsaanslag via getch()
   int pos = 0;			 // positie binnen filenaam
   int veld;			 // nummer van filenaam
   int x = 0;			 // teller
   int xx =0;                    // row waar geprint moet worden
   int yy = 0;                   // colom waar geprint moet worden
/*   int maxaantal = 110; */     // aantal files per beeldscherm
   int keus=0;
   int keer=0;
   int day,month,year;

	oke = findfirst((char *)filein,&ffblk,0);
	while (!oke)
      {
       sscanf(ffblk.ff_name,"%s", &nop);
       makedate(&day,&month,&year,ffblk.ff_fdate);
		 sprintf(filenaam[x],"%12.12s %2.2d-%2.2d-%2.2d",nop,day,month,year);
       if(++x == 250)
	{
	 sound(100);
	 nosound();
	 sound(1000);
	 swrite(10,10,"Niet alle files ingelezen\0",0x0200);
	 swrite(10,11,"Max. is 250 files\0",0x0200);
	 delay(50);
	 nosound();
	 sleep(3);
	 break;
	}
      oke = findnext(&ffblk);
      }
   num = x;
	oke = 0;

   box(0,6,(num/3)+1,69,2,1);

   while (1)
    {
    for(veld = begin; veld < begin + num; veld++)
     {
		yy = 1 + (veld - begin) / 3;
      xx = 8 + (((veld - begin) % 3) * 22);
      if(veld < num)
       {
       for(pos = 0; pos < 21; pos++, xx++)
	{
	 if(pos < strlen((char *)filenaam[veld]) )
	       ch = (filenaam[veld][pos]);
	 else  ch = SPACE;
		 if (veld == keuze) *(SCHERMPTR + yy * 80 + xx) = ch | CHHIG;
		 else               *(SCHERMPTR + yy * 80 + xx) = ch | CHINV;
	 }
	}
	 else
	   for(pos = 0; pos < 21; pos++, xx++)
					 *(SCHERMPTR + yy * 80 + xx) = SPACE | CHINV;
     }
    if(oke > 0)
      {
	veeg(0,6,25,69);
	if(oke==1) return((char*)"");
	 else
	  {
	   filenaam[keuze][12]=0;
	   return((char*)remove_lt_space(filenaam[keuze]));
	  }
      }
	 toets = getch();
    if(toets == SPECIAL_KEY)
     {
      toets = getch();
      switch (toets)
	{
	   case UP:      keuze -= 3;      break;
	   case LEFT:  --keuze;           break;
	   case DOWN:    keuze += 3;      break;
	   case RIGHT: ++keuze;           break;
		case HOME:    keuze = 0;       break;
		case ENDT:    keuze = num - 1; break;
	}
		if(keuze < 0)          keuze = num - 1;
      if(keuze > num - 1)    keuze = 0;
/*    if(keuze > num - 1)    keuze = num - 1;
*/    if(keuze < 25)         begin = 0;
      else                   begin = ((keuze - 20) / 3 ) * 3;
	  }
     else
      {
       ch = toets;
       keus=keuze;
       leave=0;
       keer=-1;
       while(keer<2)
	 {
	   keer++;
	   if(keer==1) { keus=-1;}
	     for(y=keus+1; y<num; y++)
	      {
	       if(strnicmp(&filenaam[y][0],&ch,1) == 0)
		  {
		    keuze = y;  oke = 0; leave=1;  keer=2;
		    if(keuze < 25)  begin = 0;
		    else  begin = ((keuze - 20) / 3 ) * 3;
		  }
	       if(leave) break;
	       }
	  }

    if(ch == RETURN)   oke = 2;
    if(ch == ESC) { keuze=-1;   oke = 1;}
   }
  }
}

/************************** CHANGE DIR *****************************/
void changedir(char *filein)
{
if ( parse((char *)filein) )
     {
      swrite(5,23,"Illegal drive input ",0x1300);
      sleep(1);
     }
else setdisk(disk);
if(chdir(directory))
     {
       swrite(5,23,"Illegal dir input ",0x1300);
       sleep(1);
     }
}

int parse(char *ptr)
{
 if ( ptr[1] == ':')
   {
	disk = (unsigned) toupper( ptr[0]) - 'A';
	if (disk > MAXDISK)         return(-1);
    directory = ptr+ 2;
   }
 else    {   disk=getdisk();     directory=ptr;     }
if (*directory=='\0') directory=".";
return(0);
}

/* printen box op het scherm ********************************************** */
/*                                                                          */
/* int topy			row waar box geprint moet worden            */
/* int topx			col waar box geprint moet worden            */
/* int lang			lengte box                                  */
/* int breed		        breedte box                                 */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur		        0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

void box(int topy,int topx,int lang,int breed,int type,int kleur)
{
   int x;			/* teller */
	int y;                       /* teller */
   int z;                       /* teller */
   unsigned int ch;		/* horizontale streep */
   unsigned int ck;		/* kleur die geprint moet worden */
   unsigned int clb;		/* hoek links boven */
   unsigned int crb;            /* hoek rechts boven */
   unsigned int clo;            /* hoek links onder */
   unsigned int cro;            /* hoek rechts onder */
   unsigned int cv;             /* verticale streep */

	switch (type)
		{
		case 1:
	 clb = 'Ú'; crb = '¿';
			clo = 'À'; cro = 'Ù';
	 ch  = 'Ä';  cv = '³';
	 break;
		case 2:
	 clb = 'É'; crb = '»';
	 clo = 'È'; cro = '¼';
	 ch  = 'Í';  cv = 'º';
	 break;
		}
	switch(kleur)
		{
		case 0: ck = CHNOR; break;
		case 1: ck = CHINV; break;
		case 2: ck = CHHIG; break;
		}

	z = 0;
	PTR = SCHERMPTR + (topy + z) * 80 + topx ;
						 *(PTR)               = clb | ck;
	for(x = 1; x < breed - 1; x++)      *(PTR + x)           = ch  | ck;
						 *(PTR + (breed - 1)) = crb | ck;
	for(x = 0; x < lang; x++)
      {
      z = x + 1;
	  PTR = SCHERMPTR + (topy + z) * 80 + topx ;
					   *(PTR)               = cv | ck;
	  for(y = 1; y < breed - 1; y++)   *(PTR + y)           = SPACE | ck;
					   *(PTR + (breed - 1)) = cv | ck;
      }
	z = lang + 1 ;
	PTR = SCHERMPTR + (topy + z) * 80 + topx ;
						 *(PTR)               = clo | ck;
   for(x = 1; x < breed - 1; x++)      *(PTR + x)           = ch  | ck;
				       *(PTR + (breed - 1)) = cro | ck;
}


/***********************  write text  ****************/

void swrite(int posx,int posy,char * tekst,int kleur)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
	int x=0;
   for(;tekst[x];*(nop+x) = tekst[x++] | kleur);
}

void twrite(int posx,int posy,char * tekst,int forground,int background)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
	int x=0;
   unsigned int kleur = (background<<12) + (forground<<8);
   for(;tekst[x]; *(nop+x) = tekst[x++] | kleur);
}


void initdisplay(void)
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int  *far)( (reg.h.al == 7) ? 0XB0000000L : 0XB8000000L);
}

void veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
	for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
		 *(SCHERMPTR + y*80 + x) = SPACE | 0x0700;
}

void lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur)
{
	int y, x;
	unsigned k1 = 0x0000 + kleur;
	unsigned k2 = 0x0100;
	for(y=topy; y<topy+totl; y++)
	{
//	  k1+=0x0100;
	  kleur = k1 + k2;
	  for(x=topx; x<topx+totb; x++)
		{
//	   kleur += k2;
//		  if (((kleur& 0x8000)-0x8000)==0)  kleur = 0x0000;
		 *(SCHERMPTR + y*80 + x) = kar | kleur;
	   }
	 }
}


/**************** PRINTER TEST   EJN 040388   ********************
Dit programma test of de printer op printen staat en blijft dit
testen totdat vier pogingen zijn ondernomen
*/
void prntest(void)
{
int n,count;

count=0;
while(count<4)
 {
  count++;
  n= biosprint(2,0,0);
  if( n == 0x90) break;
  veeg(0,0,25,80);
  if (count>3)
     {
       printf("\n**********************************\n"
	      "*       It does not work        *\n"
	      "*        CALL FOR HELP          *\n"
	      "*********************************\n");
	  nosound();
	  }
  printf("\n\n\n\nERROR %x ",n);
  if ((n & 0x01) ==0x01)
	     puts("\n         Printer Device time out error");
  if ((n & 0x08) ==0x08)
	     puts("\n         Printer I/O error");
  if ((n & 0x10) == 0x10)
	     puts("\n         Printer not ONLINE");
  if ((n & 0x20) == 0x20)
		  puts("\n         Printer Out of paper");
  if ((n & 0x40) == 0x40)
	     puts("\n         Printer is OFF");
  if ((n & 0x80) == 0x80)
	     puts("\n         Printer NOT BUSY");
  puts("Press a key after solving the error");
  while(bioskey(1)==0);
  getch();
  nosound();
  }
 }

/************  remove kleiner dan spatie ***************/
char *remove_lt_space(char *inputline)
{
int n;
char nop[255];
strcpy(nop,strrev(inputline));
for(n=strlen(nop);n>=0;n--)	{ if(nop[n]<=' ') nop[n]=0; else break;}
strcpy(inputline,strrev(nop));
for(n=strlen(inputline);n>=0;n--)
	 { if(inputline[n] <= ' ') inputline[n]=0; else break;}
return(inputline);
}

double getd(FILE *fp)
{
 double a;
 char *b;
 int i,j;
  b=(char *) &a;
  for (i=0;i<sizeof(double);++i)
	{
	 if((j = getc(fp)) == EOF) return(-1.0);
	 b[i]= j & 0xFF;
	}
return(a);
}

/*************** Lees lotus *.wk1 of *.wks in ***************/
void read_wks(char *infile)
{
enum opcode {
	BOF=0,Eof,CALCMODE,CALCORDER,SPLIT,
	SYNC=5,DIMENSIONS,WINDOW1,COLW1,WINDOW2,
	COLW2=10,NRANGE,BLANK,INTEGER,NUMBER,
	LABEL=15,FORMULA,
	TABLE=24,QRANGE,PRANGE,SRANGE,FRANGE,KRANGE1,
	DRANGE=32,KRANGE2=35,PROTECT,FOOTER,HEADER,SETUP,MARGINS,
	LABELFMT=41,TITLES,GRAPH=45,NGRAPH,CALCOUNT,FORMAT,CURSORW12,
	STRING=51,SNRANGE=71,WKSPASS=75,
	HIDCOL1=100,HIDCOL2,PARSE,RRANGES,MRANGES=105,CPI=150};

int op, formula_size, file_type;
int column, row, start_col, start_row, end_col, end_row;
char  c, pos,format,order=0;
double doubleing,integer;
long count,body_length;
//unsigned char buffer[439];
long bars;
char nopp[80];
lost=FALSE;
count=0;
labels=0;

if ((fp=fopen(infile,"rb")) == NULL)
	 {
		printf("Can't find file %s",infile);
      getch();
		EXIT(1);
	 }
printf("\n\n\n\n                   LOADING %s\n\n",infile);
if(!show)
  {
  swrite(20,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(20,11,"³                                         ³",0x1700);
  swrite(20,12,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(20,13,"0%                  50%                  100%",0x2700);
  strcpy(nopp,"                                         ");
  bars = (long)filelength(fileno(fp)) /41L;
  }
 op=getw(fp);
 while (op!=Eof)
	{
		 body_length=(long)getw(fp);
		 if (body_length < 0) {
			  printf("\nDeze worksheet kan ik niet verwerken\n");
			  printf("Maak een printerfile van de data met: / printer file enz\n");
			  printf("en lees deze weer in, in een LEGE worksheet met:\n");
			  printf("/ file import numbers\n");
           getch();
			  EXIT(-1);
		 }

 count += body_length+4;
 if (!show)
	{
	 strnset(nopp,176,(int)((long)count / (long)bars));
	 swrite(21,11,nopp,BARCOLOR);
	 }

 if (show) printf("count: %ld ",count);
 switch(op)
  {
case INTEGER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 integer=(double)getw(fp);
	 if (integer < 0 ) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (show)
		 printf("integer: col %d, row %d, read:%8.3f\n",column,row,integer);
	 if (row > rows) rows=row;
	 data[column][row]=integer;
	 break;
case NUMBER:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 doubleing=getd(fp);
	 if (doubleing <0 || doubleing > 1e10) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (show)
	  printf("number: col %d, row %d, read:%8.3f\n",column,row,doubleing);
	 if (row > rows) rows=row;
	 data[column][row]= doubleing;
	 break;
case FORMULA:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 doubleing=(double)getd(fp);
	 formula_size=getw(fp);
	 skip(fp,formula_size);
	 if (doubleing <0 || doubleing > 1e10) break;
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (show)
	  printf("FORMULA: col %d, row %d, read:%8.3f\n",column,row,doubleing);
	 if (row > rows) rows=row;
	 data[column][row]= doubleing;
	 break;
case BLANK:
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 if (row >= MAX_ROW || column >= MAX_COL) break;
	 if (show) printf("BLANK: col %d, row %d\n",column,row);
	 data[column][row]=(double)MISSING;
	 break;
case LABEL:
	 pos=0;
	 format=getc(fp);
	 column=getw(fp);
	 row=getw(fp);
	 if (show) printf("LABEL: col %4d , row %4d ",column,row);
	 if (row < MAX_ROW && column < MAX_COL)
		{
		  if (column > columns) columns=column;
		  if(row==0) data[column][row]=(double)MISSING+1+labels;
			else      data[column][row]=(double)MISSING;

		 }
	 c=getc(fp);
	 c=getc(fp);
	 while (c)
		{
//		  if(c!=39 && c!=34 &&c!=96);
		  {
			if (show) putchar(c);
			if ((column < MAX_COL) && (pos<MAX_LEN-1) && (row==0))
				 label[column][pos++]=c;
			if(row < MAX_ROW && column==0 &&(pos<MAX_LEN-1))
				  naam[row][pos++]=c;
			}
		  c=getc(fp);
		 }
	 if (row < MAX_ROW && column==0)		  naam[row][pos++]=0;
	 if (column < MAX_COL && row==0 )	  label[column][pos++]=0;
	 if (show) putchar('\n');
	 break;
case BOF:
	 file_type=getw(fp);
	 if (show)              printf("file-type %X\n",file_type);
	 break;
case DIMENSIONS:
	 start_col=getw(fp);
	 start_row=getw(fp);
	 end_col=getw(fp);
	 end_row=getw(fp);
	 if (show) 	   printf("dimensions col;row: %d;%d en %d;%d\n"
				 ,start_col,start_row,end_col,end_row);
	 rows =   (end_row >= MAX_ROW ) ? MAX_ROW : end_row ;
	 columns= (end_col >= MAX_COL)  ? MAX_COL : end_col ;
	 columns=3;
	 for (row=0;row<=rows;row++)
	  {
		for (column=0;column<columns;column++)  data[column][row]=MISSING;
		strcpy(naam[row],"Noname");
	  }
	  rows=columns=0;
	  break;
 case CPI:
	  if (show) printf("CPI=%d, dus %d actieve kolommen\n"
				,body_length,body_length/6);
	  skip(fp,body_length);
	  break;
  default:
		if (show) printf("opcode: %d lengte: %d bytes \n",op,body_length);
		skip(fp,body_length);
	 }
  op=getw(fp);
  }
fclose(fp);
if(row > MAX_ROW-2) lost = TRUE;
op=order;
op=format;
}

void skip(FILE *fp,long noofbytes)
{char c;
while (noofbytes>0)
	{
	  c=getc(fp);
	  noofbytes--;
	  c=c;
	}
}

/*************** Print labels ***************/
void display_labels(void)
{
int label_no;
	puts("\n\n");
		 for (label_no=0;label_no<labels;label_no++)
	{
	printf("#%d %s\n",label_no,&label[label_no][1]);
	if(print) fprintf(printer,"#%d %s\n",label_no,&label[label_no][1]);
	}
}

/*************** Bereken corr,a,b ***************/
/*************** Bereken ijklijn  ****************/

int calc(int row_gehalte, int row_meet)

{
 int n,i,k,rows_gevuld;
 double resultt,endresult,dil;
 double nop;
 char datum[12];
 struct date d;
 char s[80];

 while(TRUE)                   /* verwijder punten,screendump of stop */
 {
	regressie(row_gehalte,row_meet);
	nop=graph();
	picend();
	if(nop==FALSE) break;         /* plaatje goedgekeurd, ga rekenen */
	if(nop==2) { return(2);} /* escape gedrukt */
 }
 kolom=row_meet;
 n=0;
 if(print )
  {
  while(!prnonoff())
	{
	 puts("Turn on the printer & Press any key");
	 getch();
	 n++;
	 if(n==3) {print = FALSE; break;}
	}
  }

/* if(print) fputc(15,printer);*/
 getdate(&d);
 sprintf(datum,"%2.2d-%2.2d-%2.2d",d.da_day,d.da_mon,d.da_year);

 printf("Filenaam : %s\n   Datum : %s\n\n",fileload,datum);
 if(print)
	 fprintf(printer,"\n Filename : %s\n     Date : %s\n\n",fileload,datum);

 printf("    slope = %0.3f\nintercept = %0.3f\n   corr R = %0.4f\n    Blank = %0.3f\n     Bmax = %0.3f\n[1/2Bmax] = %0.3f\n\n"
					,slope,intercept,corr,C,D,exp(-intercept/slope));
 if (print) fprintf(printer,"    slope = %0.3f\nintercept = %0.3f\n        R = %0.4f\n    Blank = %0.3f\n     Bmax = %0.3f\n[1/2Bmax] = %0.3f\n\n"
					,slope,intercept,corr,C,D,exp(-intercept/slope));
 if(print) {  printf("Experiment name : ");
			gets(s);
			fprintf(printer," Exp name : %s\n",s);
			fputs("\n\n",printer);
		 }

 printf("            Conc     Meas     result      dil     endresult\n");
 if(print)
  fprintf(printer,"	   Conc     Meas     result      dil     endresult\n");

 printf("\n");
 if (print) fprintf(printer,"\n");

 for (k=rows;data[row_meet][k]<0;k--);
 rows_gevuld=k;
 i=1;
 if(data[row_meet][0] >= 0) i=0;  /*Als toch eerste rij gevuld*/
 for(k=i;k<=rows_gevuld;k++)
  {
	nop = logit(data[row_meet][k]);
	resultt=exp((nop-intercept)/slope);
	if (data[2][k] > 0) { dil = data[2][k]; 	endresult=resultt * dil; }
	 else               { dil = 1;      		endresult=resultt; }

												printf("%4d ",k);
	if (data[row_gehalte][k]<0 ) /* als geen conc */
												printf("%9.9s",naam[k]);
	else       								printf("%9.3f",data[row_gehalte][k]);
	if (data[row_meet][k]<0)     /* als conc is MISSING */
												printf("      -\n");
	else if (nop == MISSING)     /* Als resultaat NB */
												printf(" %12.3f \n",data[row_meet][k]);
	else                      			printf(" %12.3f %9.3f %9.1f %12.3f \n"
																	,data[row_meet][k],resultt,dil,endresult);

	/**        Als printer aan */
	if(print)
	{
												fprintf(printer,"%4d ",k);
	if (data[row_gehalte][k]<0 )     fprintf(printer,"%9.9s",naam[k]);
		else    								fprintf(printer,"%9.3f",data[row_gehalte][k]);
	if (data[row_meet][k]<0)         fprintf(printer,"      -\n");
	else if (nop == MISSING)         fprintf(printer," %12.3f\n",data[row_meet][k]);
		else              				fprintf(printer," %12.3f %9.3f %9.1f %12.3f\n"
																	,data[row_meet][k],resultt,dil,endresult);
	}

	 /*       schrijf in output file */
												fprintf(fpout,"%4d ",k);
	if (data[row_gehalte][k] < 0 )   fprintf(fpout,"\"%9.9s\"",naam[k]);
	  else     							   fprintf(fpout,"%9.3f",data[row_gehalte][k]);
	if (data[row_meet][k] < 0)       fprintf(fpout,"\n");
	else if (nop == MISSING)         fprintf(fpout," %12.3f\n",data[row_meet][k]);
	  else              				   fprintf(fpout," %12.3f %9.3f %9.1f %12.3f\n"
																	,data[row_meet][k],resultt,dil,endresult);
	if(bioskey(1)!=0)
	 {
	  if(getch()==27) break;
	 }
  }
/*if(print){ fputc(27,printer);  fputc(64,printer);}*/
return(0);
}
/**************************  fill temp  *******************************/
void fill_temp(int i,int j)
{
 int k,n;
 double vx,vy;
 double cc[50];
 int cteller=0;
 rows_temp=0;
 for(k=0;k<=rows;k++)
	{
	 vx=data[i][k];
	 vy=data[j][k];
	 if (vx>0 && vy>=0)
	  {
		temp[0][rows_temp]=vx;
		temp[1][rows_temp]=vy;
		ykpunten[rows_temp]=TRUE;
		if (show) printf("x=%0.3f  y=%0.3f\n",vx,vy);
		if(++rows_temp >=MAX_YK_PNT)
		{
		printf("Too many datapoints; max: %d \n",MAX_YK_PNT);
      getch();
		EXIT(-1);
		}
	  }
	 if(vx==0.0 && cteller<50) { cc[cteller++]=vy; }
	}
// if(!HAND)
 if(cteller)  //als wk? of prt file geladen
  {
	C=0;
	for (n=0;n<cteller;C+=cc[n++]);
	C/=(cteller);
  }
}

/*************************  LOGIT ********************************/

double logit(double meet)
{
 if((meet-C)/(D-meet)<=0) return(MISSING);
 return(log((double)((meet-C)/(D-meet))));
}

/*************************** REGRESSIE ***************************/
void regressie(int i, int j)
{
register int k;
register double vy;
double   somxx,somxy,somyy,sx,sy,sxy;
double   vx[MAX_YK_PNT+1];
double   xmax,ymax,yc;
double   factor=1.1,maalfactor=1.01;
double   corroud = 0;
int      loop=1;
int      toggle,ycc;
char     text[60],ii[3];

picopen();
if(!berekend)
{
 Iteratie:
 factor=1.1;
 maalfactor=1.01;
 corroud = 0;
 xmax=ymax=D=yc=0.0;
 deleted=FALSE;
 corr=1.0;
 if(rows_temp<3 || aantal<3) fill_temp(i,j);
 somx=0;    somy=0;    somxx=0;    somyy=0;    somxy=0;    aantal=0;

 for(k=0;k<rows_temp;k++)
   {
    if(ykpunten[k]==TRUE)
      {
	 vy=temp[1][k];
	 if (fabs(yc)<fabs(vy-C))
	  {
	    yc=vy-C;  /* Bepaal verhogen of verlagen */
	    ycc=k;
	  }
      }
	 }
 if(yc>0)   /* D>C */
 {
 for (k=0;k<rows_temp;k++)
   {
    if(ykpunten[k]==TRUE)
      {
       if (temp[0][k] > xmax)  xmax=temp[0][k];
       if (temp[1][k] > ymax)  ymax=temp[1][k];
       if (temp[1][k] <= C    ) {ykpunten[k]=FALSE; deleted=TRUE;}/*gooi punt eruit */
      }
   }
 }
 if(yc<0)   /* D<C */
 {
 for (k=0;k<rows_temp;k++)
   {
    if(ykpunten[k]==TRUE)
		{
       if (temp[0][k] > xmax)  xmax=temp[0][k];
       if (temp[1][k] > ymax)  ymax=temp[1][k];
       if (temp[1][k] > C    ) {ykpunten[k]=FALSE; deleted=TRUE;} /*gooi punt eruit */
      }
   }
  }
 for(k=0;k<rows_temp;k++)
   {
    if(ykpunten[k]==TRUE)
      {
	 if(Check_Log(temp[0][k],1)) return;
	 vx[k]=log(temp[0][k]);
	 vy=temp[1][k];
	 somx+= vx[k];	 somxx+=vx[k]*vx[k];	 aantal++;
      }
    }
  if ( yc < 0.0 )   D=temp[1][ycc] / factor;
  else              D=temp[1][ycc] * factor;

  cleardevice();
  graph_body();
  setcolor(LIGHTGREEN);
//  prnt(H_WIDTH/3.0,V_WIDTH*0.9,0," Press CODE to (un)delete, ESC to stop");
  prnt(H_WIDTH/3.0,V_WIDTH*0.9,0," Calculating");
  toggle=0;
  while (loop>0)
	{
	toggle=1-toggle;
	  somy=0;    somyy=0;    somxy=0;
	 for(k=0;k<rows_temp;k++)
	  {
		if(ykpunten[k]==TRUE)
		{
		 vy = logit(temp[1][k]);
		 somy += vy; somyy+=vy*vy; somxy+=vx[k]*vy;
		}
	  }
	 sxy=(somxy-(somx*somy / aantal));
	 sx= (somxx-(somx*somx / aantal));
	 sy= (somyy-(somy*somy / aantal));
	 corr=(sxy / (sqrt( sx * sy)));

	 slope = sxy / sx;
	 intercept= ( somy / aantal ) - ( somx / aantal ) * slope;
//	 sprintf(text,"R = %0.5f",corr);
//	 setviewport(0,0,MaxX/2.5,MaxY/25,0);
//	 clearviewport();
//	 setviewport(0,0,MaxX,MaxY,0);
//	 prnt(H_OFFSET*1.0,V_WIDTH*1.0,0,text);

	 setcolor(WHITE);
//	 if(toggle) drawregressieline(15.0);

	if(bioskey(1)!=0)
	{
	 ii[0]=getch();
	 ii[1]=0;
	 strupr(ii);
//	 if(ii[0]=='S' ||ii[0]==27) break;
	 if(ii[0]==27) break;
	 if (ii[0]>='A' && ii[0]<=('A'+rows_temp))
		{
		  ykpunten[ii[0]-'A']=1-ykpunten[ii[0]-'A'];
		  berekend=FALSE;
		  goto Iteratie;
		}
	}
	if(corr<0) corr=0-corr;           /*maak R positief */
	factor*=maalfactor;
//	ic =  corr * 1000000;              /* stop itt als corr niet meer dan 0.00001 verandert */
//	ico = corroud * 1000000;
	if (corr<=corroud )
	  {
		factor/=maalfactor;
		maalfactor=1;
		loop++;          /*verhoog de loop teler*/
		if (loop==2)
			{
			 factor=(((factor-1)/10)+1); /* Ga nog max 10 keer terug */
			 yc=-yc;		  /* In veranderde richting */
			}
		if (loop==3)
			{
			 factor=(((factor-1)/5)+1);  /* Ga nog max 5 keer terug */
			 yc=-yc;		  /* In veranderde richting */
			}
		if (loop==4)
			{			     /* Ga nog 1 keer terug */
			 yc=-yc;		  /* In veranderde richting */
			}
	  }
	if(loop <= 4 )  corroud=corr;
	else 	{
			 berekend = TRUE;
			 break;
			} 				  /* Als loop>3 verlaat iteratie*/
	setcolor(BLACK);
//	if(toggle) drawregressieline(15.0);
	setcolor(WHITE);

	if (yc > 0 )   D *= factor;        /* D verlagen of verhogen ? */
	else 				D /= factor;
	if(D>C)
	  { if(D<=ymax)	 D=ymax*1.01; }  /* D niet < ymax */
	}							/*  einde if berekend */
 corr = sxy / sqrt(sx*sy);
 slope = sxy / sx;
 intercept= ( somy / aantal ) - ( somx / aantal ) * slope;
}
cleardevice();
setcolor(GREEN);
graph_body();
drawregressieline(100.0);
setcolor(LIGHTRED);
sprintf(text,"R = %0.5f  Bmax = %0.3f  Blank = %0.3f ",corr,D,C);
prnt(H_OFFSET*1.0,V_WIDTH*1.0,0,text);

return;
}
/*************** Subroutines tekenen en schrijven PIC file ***************/

void teken(double xp,double yp)
	{ lineto((int)(xp*PICXF),MaxY-(int)(yp*PICYF) );  }

void moef(double xp,double yp)
  { moveto((int)(xp*PICXF),MaxY-(int)(yp*PICYF) );  }

void point(double xp,double yp)
 {
  prntc(xp,yp,0,"@");
/* teken (xp-SBOX,yp+SBOX);
 teken (xp-SBOX,yp-SBOX);
 teken (xp+SBOX,yp-SBOX);
 teken (xp+SBOX,yp+SBOX);*/
 }

void numm(double xp,double yp,int number)
{
 unsigned char tekst[4];
 tekst[0]=64+number;
 tekst[1]='\0';
 prntc(xp,yp,0,(char *)tekst);
}

void prnt(double xpos,double ypos,int richting,char *tekst)
 {
  settextjustify(LEFT_TEXT,TOP_TEXT);
  moef(xpos,ypos);
  pictext(((int)(richting/90)),tekst);
 }

void prntc(double xpos,double ypos,int richting,char *tekst)
 {
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  moef(xpos,ypos);
  pictext(((int)(richting/90)),tekst);
 }


/******************** TEST OF PRINTER AAN OF UIT ************************/
int prnonoff(void)
{
 if(biosprint(2,0,0)  != 0x90)
 {
  sound(2200);
  delay(13);
  nosound();
  sound(200);
  delay(300);
  nosound();
  return(FALSE); /* printer UIT */
 }
return(TRUE);    /* printer AAN */
 }

/*************** openen en verwerken PIC file ***************/

void picopen(void)
{
	Initialize() ;  	/* Set system into Graphics mode */
/*
	picsize(1);
	FONT=0;
	piccolor(0);
*/
}

void piccolor(int color){ setcolor(color);}
void picsize(int x) {   SIZE = x;  }
void pictext(int direction,char *s)
{
 settextstyle(FONT,direction,SIZE);
 outtext(s);
}
void picend(void)   {closegraph();}
/*************** Tekenen van plaatje ***************/
void graph_body(void)
 {
 char text[40];
 int i;
 double vx,vy,asx,asy;
 double as[AS_AANTAL+2]={0.001,0.002,0.005,0.01,0.02,0.05,0.1,0.2,0.5,1.0,2.0,
			5.0,10.0,20.0,50.0,100.0,250.0,500.0,1000.0,2500.0,
			5000.0,10000.0,20000.0,50000.0,1E5,1E6,1E7,1E8,0};
maxx=maxy=0.0;
minx=999999.0;
miny=999999.0;

for (i=0;i<rows_temp;i++)
   {
       if (temp[0][i]>maxx)                  maxx=temp[0][i];
       if (temp[0][i]<minx && temp[0][i]>0)  minx=temp[0][i];
       if (temp[1][i]>maxy) 		     maxy=temp[1][i];
       if (temp[1][i]<miny && temp[1][i]>0)  miny=temp[1][i];
	}
 maxx*=1.1;
 maxy*=1.1;
 if(Check_Log(minx,2)) return;
 xminlog=log(minx/1.1);
 if(Check_Log(miny,3)) return;
 yminlog=log(miny/1.1);
 
 for (i=0;i<=AS_AANTAL;i++)
  {
   asx=as[i];
	if (10*asx > maxx )   break;
  }

 for (i=0;i<=AS_AANTAL;i++)
  {
   asy=as[i];
   if (20*asy> maxy )  break;
  }

 offpixx=H_OFFSET;
 offpixy=V_OFFSET;
 lx=(H_WIDTH-H_OFFSET)*H_SCALE;         /*lengte assen*/
 ly=(V_WIDTH-V_OFFSET)*V_SCALE;
 if(Check_Log(maxx,4)) return;
 if (LXG)  facx=lx/(log(maxx)-xminlog); /*factor data*/
 else      facx=lx/maxx;
 if(Check_Log(maxy,5)) return;
 if (LYG)  facy=ly/(log(maxy)-yminlog); /*factor data*/
 else      facy=ly/maxy;

 moef(offpixx,offpixy);                 /*draw box*/
 teken(lx+offpixx,offpixy);
 teken(lx+offpixx,ly+offpixy);
 teken(offpixx,ly+offpixy);
 teken(offpixx,offpixy);
 if(LXG)
  {
   moef(offpixx,offpixy);
   teken(offpixx/3,offpixy);
	prntc(offpixx/3+200,offpixy-10,0,"<<");
   }
 if(LYG)
  {
   moef(offpixx,offpixy);
	teken(offpixx,0);
   prntc(offpixx,200,90,"<<");
  }
 for (i=0;i<rows_temp;i++)
  {
   vx=temp[0][i];
   vy=temp[1][i];
   if(Check_Log(vx,6))     return;
   if(LXG)                 x2=(log(vx)-xminlog)*facx+offpixx;
   else                    x2=vx*facx+offpixx;
   if(Check_Log(vy,7))     return;
	if(LYG)                 y2=(log(vy)-yminlog)*facy+offpixy;
   else                    y2=vy*facy+offpixy;
   if (ykpunten[i]==TRUE)  {setcolor(YELLOW);    numm(x2,y2,i+1);}
   else                    {setcolor(LIGHTRED); point(x2,y2);}
   setcolor(WHITE);
  }

 if(LXG)
    {
     for (i=0;as[i]<(minx/1.1);i++);               /* LOG X as labeling */
     for (i=i;as[i]<=maxx;i++)
       {
	if(Check_Log(as[i],8)) return;
	mx=((log(as[i])-xminlog)*facx+offpixx);
	my=(double)(offpixy);
	moef(mx,my);
	teken(mx,my+100);
	gcvt(as[i],5,text);
	prntc(mx,my-150.0,0,text);
       }
      }
     else
     {
 for (i=0;i<=(int)(maxx/asx);i++)               /* LIN X as labeling */
       {
	mx=(double)((double)i*facx*asx+offpixx);
	my=offpixy;
	moef(mx,my);
	teken(mx,my+100.0);
	gcvt((double)i*asx,5,text);
	prntc(mx,my-150.0,0,text);
       }
      }
   if(LYG)
		{
       for (i=0;as[i]<(miny/1.1);i++);          /* LOG Y as labeling */
       for (;as[i]<=maxy;i++)
       {
	if(Check_Log(as[i],9))	    return;

	mx=offpixx;
	my=((log((double)as[i])-yminlog)*facy+offpixy);
	moef(mx,my);
	teken(mx+100,my);
	gcvt(as[i],5,text);
	prntc(mx-400,my,0,text);
		 }
      }
     else
     {
      for (i=0;i<=(int)(maxy/asy);i++)               /* LIN Y as labeling */
      {
       mx=offpixx;
       my=(double)((double)i*facy*asy+offpixy);
       moef(mx,my);
       teken(mx+100.0,my);
       gcvt((double)i*asy,5,text);
	prntc(mx-400.0,my,0,text);
      }
     }
 setcolor(WHITE);
 prntc(H_WIDTH/2 , 300 , 0 ,"Concentration");
 setcolor(LIGHTGREEN);
 prntc(H_WIDTH/2,100,0,"TAB=Scrdump+Calc  SPACE=Calc  ESC=Menu  CODE=(Un)Delete point");
 setcolor(WHITE);
 prntc(300, V_WIDTH/2, 90 ,"Measurement");       /* print assen */
}

/*************************  INITIALIZE *****************************/

void Initialize(void)
{
  if (registerfarbgidriver(CGA_driver_far)    < 0) EXIT(-1);
  if (registerfarbgidriver(Herc_driver_far)   < 0) EXIT(-1);
  if (registerfarbgidriver(EGAVGA_driver_far) < 0) EXIT(-1);

  GraphDriver = DETECT;			/* Request auto-detection	*/
  initgraph( &GraphDriver, &GraphMode,"");
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk )		/* Error occured during init	*/
   {
	 printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    getch();
	 EXIT( 1 );
   }
  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/
  MaxX = getmaxx();
  MaxY = getmaxy(); /* Read size of screen		*/
  PICXF = (double)MaxX/H_WIDTH; /* correctie factor mijn coord. naar TC coord. */
  PICYF = (double)MaxY/V_WIDTH;
}

/**************************  GRAPH *********************************/
int graph(void)
{
	char j[3];
	char i;

	do { j[0]=getch();	j[1]=0; strupr(j); i=j[0];}
	 while (i!=SPACE && i!=TAB && i!=ESC &&
				 (i<'A'  || i>('A'+rows_temp)) );
	if (i==TAB )
	 {if(!print) return(FALSE);
	  setcolor(BLACK);
	  prntc(H_WIDTH/2,100,0,"TAB=Scrdump+Calc  SPACE=Calc  ESC=Menu  CODE=(Un)Delete point");
	  setcolor(WHITE);
	  if(!prnonoff()){
	    closegraph();
	    prntest();
	    return(TRUE);
	    }
	  screendump();
	  return(FALSE);
	 }
	if (i==SPACE)                 return(FALSE);
	if (i==ESC )                  return(2);
	if (i>='A' && i<=('A'+rows_temp))
	{  ykpunten[i-'A']=1-ykpunten[i-'A'];  berekend=FALSE;	}
return(TRUE);
}

/*************** DRAW REGRESSIELIJN ***********************************/
void drawregressieline(double noofpoints)
{
int go;
double CC,DD;

 if(D<C) { DD=C-1E-10;   CC=D+1E-10; if(DD>maxy) DD=maxy;}
 else    { DD=D-1E-10;   CC=C+1E-10; if(DD>maxy) DD=maxy;}
 go=0;
 for (vyy=CC;vyy<DD;vyy += DD/(noofpoints+0.001))
	{
	   vxx=exp((logit(vyy)-intercept)/slope);
	   if(LXG) {if(Check_Log(vxx,12)) return;}
	   if(LXG)  x2 = (log(vxx) - xminlog) * facx + offpixx;
	     else   x2 =      vxx             * facx + offpixx;

	   if(LYG) {if(Check_Log(vyy,13)) return;}
	   if(LYG)  y2 = (log(vyy) - yminlog) * facy + offpixy;
	      else  y2 =      vyy             * facy + offpixy;

	   if(x2<0) x2=0;
	   if(y2<0) y2=0;
	   moef(x1,y1);
	   x1=x2;
	   y1=y2;
	   if(vxx>maxx||vyy>maxy)continue;
	   if(x1<0 || y1<0) continue;
	   if(!go) { go=1; continue;} /*eerste keer niet tekenen */
	   teken(x2,y2);
	  }
}

/************************  check log <=0 ***************************/
int Check_Log(double check,int err)
{
 if(check<=0)
  {
	printf("Log Error %d: %f :Druk een toets",err,check);
	getch();
	return(1);
  }
	else return(0);
 }

/************************ malloc data ************************/

void mallocdata(void)
 {
	int n;

/* LABELS */
	label = (char  **)malloc((MAX_COL)  * sizeof(char  *) );
	for (n=0;n <= MAX_COL;n++)
	{
	 label[n] = (char *) malloc((MAX_LEN) * sizeof(char));
	  if (label[n]==0)
		{
		 printf("%lu coreleft\n\n",farcoreleft());
		 puts("TOO LESS MEMORY\n");
       getch();
		 EXIT(-1);
		}
	}
/* TEMP */
	temp = (double **)malloc( 2 * sizeof(double  *) );
	for (n=0;n < 2;n++)
	{
	 temp[n] = (double  *) malloc((MAX_YK_PNT) * sizeof(double));
	  if (temp[n]==0)
		{
		 printf("%lu coreleft\n\n",farcoreleft());
		 puts("TOO LESS MEMORY\n");
		 getch();
		 EXIT(-1);
		}
	}
/* DATA */
	data = (double  **)malloc((MAX_ROW)  * sizeof(double  *) );
	for (n=0;n < MAX_COL;n++)
	{
	 data[n] = (double  *) malloc((MAX_ROW) * sizeof(double));
	  if (data[n]==0)
		{
		 printf("%lu coreleft\n\n",farcoreleft());
		 puts("TOO LESS MEMORY\n");
       getch();
		 EXIT(-1);
		}
	}
/* NAAM */
	naam = (char  **)malloc((MAX_ROW)  * sizeof(char  *) );
	for (n=0;n < MAX_ROW;n++)
	{
	 naam[n]= (char *) malloc(MAX_LEN * sizeof(char));

	  if (naam[n]==0)
		{
		 printf("%lu coreleft\n\n",farcoreleft());
		 puts("TOO LESS MEMORY\n");
       getch();
		 EXIT(-1);
		}
	}
}
/***************** DEALLOCEER DATA *************************************/
void freemallocdata(void)
{
  int n;

  for (n=MAX_COL-1; n >= 0;n--)	free(data[n]);	 free(data);
  for (n=1; n >= 0;n--)	        free(temp[n]);   free(temp);
  for (n=MAX_COL-1; n >= 0;n--)	free(label[n]);	 free(label);
  for (n=MAX_ROW-1; n >= 0;n--)	free(naam[n]);	 free(naam);
}
/*************************READ PRT FILE *******************************/
void readprt(char *filein)
{
	int 	fout,row;
	char  inputline[255];
	long int count;
	char 	nop[255],nopp[25];
	double bars;

	printf("\n\nInlezen %s ",filein);

	if ((fpin=fopen(filein,"r")) == NULL)
	{
		printf("cannot open input-file %s\n",filein);
		getch();
		EXIT(1);
	}

  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2700);
  strcpy(nopp,"                     ");

	count=0;
	row=0;
	bars=(double)( filelength(fileno(fpin)) ) / 21;

	while(TRUE)
	{
	  if(fgets(inputline,255,fpin)==NULL)    break;
	  count+=strlen(inputline)+2;
	  strnset(nopp,176,(int)(count/bars));
	  swrite(31,11,nopp,BARCOLOR);
	  strcpy(nop,"  ");
	  strcat(nop,inputline);
	  strcpy(inputline,remove_lt_space(nop));
	  fout=FALSE;
	  if(strlen(inputline)<3)  fout=TRUE;
	  if(!fout)
	  {
		strcpy(inputline,remove_lt_space(inputline));
		strcpy(nop,strtok(inputline," "));
		if(nop[0]>57)
		  {
		   data[0][row]=-1;
		   strcpy(naam[row],nop);
		  }
		else
		  {
		    data[0][row]=atof(nop);
		    strcpy(naam[row],"Noname");
		  }
		naam[row][MAX_LEN-1]=0;
		strcpy(nop,strtok(NULL," "));
		data[1][row]=atof(nop);
		strcpy(nop,strtok(NULL," "));
		data[2][row++]=atof(nop);
		if(row+4 > MAX_ROW) {lost=TRUE; break;}
		}
	  }
data[0][row]=MISSING;
data[1][row]=MISSING;
data[2][row]=MISSING;
naam[row][0]=0;
rows=row;
fclose(fpin);
}

/************************ SCREENDUMP *************************************/
void screendump(void)
{
int printeer;
register int colomn,deler,compensatie,a,b;
register unsigned int pixy , regel , nop;
unsigned char rowsum[1025];
unsigned char str1[] = {27,64,27,65,8};              /* linespacing 8/72 inch */
unsigned char str2[] = {27,76,128,2};                /* ESC K, 640 char */
unsigned char str3[] = {13,10};                      /* CR/LF */
unsigned char str4[] = {13,10,27,65,12};             /* linespacing 1/6 inch */
unsigned char str5[] = {32,32,32,32,32,32,32,32,32,32,32,32};
if (MaxX+1==720) str2[2]=208;                          /*ESC K,720 char */
if (MaxX+1==320) {str2[2]=64; str2[3]=1;}              /* ESC K,320 char */

if(!prnonoff()) return;
printeer = open("PRN" , O_WRONLY | O_BINARY);
write(printeer,str1,5);
nop = 0;

if(MaxY+1<350) deler=8; else deler=16;                  /* als >350 regels */
if(MaxY+1<350) compensatie=1; else compensatie=2;       /* compress hoogte */
 {
 for ( regel = 0 ; regel< ((MaxY+1)/deler) ; regel++)
  {
	  for ( colomn = 0 ; colomn < (MaxX+1) ; colomn++)
     {
      rowsum[colomn] = 0 ;                            /* zet byte op nul */
      for ( pixy = 0 ; pixy < deler ; pixy++ )
	 {
          b = (pixy/2)*2;                             /* pixy even? */
	  if(b==pixy) a=0;                            /* voeg 2 pix samen */
	  if (getpixel( colomn ,nop + pixy ))         /* if screen dot */
	   { putpixel( colomn ,nop + pixy,YELLOW);
            if(deler==16)
				{
             if(b == pixy)
                {
                rowsum[colomn] += (128 >> (pixy/compensatie));
		a=1;
		}
		 else if(!a)    rowsum[colomn] += (128 >> (pixy/compensatie));
	     }
	   else    rowsum[colomn] += (128 >> pixy);
	   }
	 }                                            /* einde loop byte*/
      }                                               /* einde loop kolom */
	 write(printeer,str5,5);                           /* print 5 spaces */
	 write(printeer,str2,4);                           /* ESC K,mod256,div256 */
	 write(printeer,rowsum,MaxX+1);                    /* print bitimage */
	 write(printeer,str3,2);                           /* print CR/LF */
	 nop += deler;
	 }                                                 /* einde regel */
	}
 write(printeer,str4,5);                              /* linespacing 1/6 inch */
 close(printeer);
 }

short Make_Last_Adjustment_file(void)
 {
 FILE *stream;
 if((stream=fopen(Adjustment_file,"wt")) == NULL)
	{
		printf("cannot open input-file %s\n",Adjustment_file);
		getch();
		EXIT(1);
	}
 fprintf(stream,"%ld,Programstarted\r\n",Pstarted);
 fprintf(stream,"%ld,Fileinput\r\n",Fileinput);
 fprintf(stream,"%ld,ManualInput\r\n",ManualInput);
 fprintf(stream,"%ld,Rescalcs\r\n",Rescalcs);
 fclose(stream);
return(TRUE);
}

short Read_Last_Adjustment_file(void)
 {
 FILE *stream;
 char text[42];

 Pstarted=Fileinput=ManualInput=Rescalcs=0L;
	 if((stream=fopen(Adjustment_file,"rt")) == NULL)
			Make_Last_Adjustment_file();
	 else
		{
		fgets(text,40,stream);  Pstarted=atol(strtok(text,","));
		fgets(text,40,stream);  Fileinput=atol(strtok(text,","));
		fgets(text,40,stream);  ManualInput=atol(strtok(text,","));
		fgets(text,40,stream);  Rescalcs=atol(strtok(text,","));
		fclose(stream);
		}
return(TRUE);
}
//-----------------------------------------------------------------
void EXIT(int errcode)
{
setdisk(maindisk);
chdir(maindir);
//printf("%s\n",maindir);
freemallocdata();
Make_Last_Adjustment_file();
exit(errcode);
}
 /* Correlatiematrix turbo C         EJN

	Options : char signed
		  word byte
		  compile large
 */

#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <string.h>
#include <process.h>
#include <dos.h>
#include <bios.h>
#include <dir.h>
#include <stdlib.h>
#include "corr.h"

#define breedte_item 20
#define lang 16

#define ROMAX 25
#define COMAX 80

extern void display_labels();
extern void display_data();
extern void changedir();
extern void teken_graph();
extern int  display_menu(int);
extern char *filedir(char *dir);
extern void veeg(int, int, int, int);
extern void swrite(int ,int ,char * ,int);
//extern void mode(int);
//extern void palette(int);
extern void initdisplay();
extern void read_wks(char *);

void main(int argc,char *argv[]);
void prntest(void);
void beep(unsigned freq, unsigned duration);
void mallocdata(void);
void freemallocdata(void);

unsigned _stklen=18192;                   /* vergroot de stack */
unsigned long MAX_ROW=750L;
unsigned long MAX_COL=50L;

float  **data;            /* DATA BLOK*/
float  **PP;			  /* P TABEL BLOK */
float  *sortx;
float  *sorty;
char   **label;
int    *SCHERMPTR;
float  **temp;

int   actie;
int   show,lost,print,num,spearm;
char  curdir[40];
char  dir[40];
char  fileload[40];
char  maindir[40];
int   maindisk;
float upx,upy,upz;
int   geladen=0;
int   lijn =1,lijnxy =1,conf=1,THREE_D=0;
float maxttest;
char  outfile[40];
int   DrawPicFile=TRUE;     // Schrijf PIC file naar disk
int   DrawGraphScreen=FALSE;// Teken op het geinitialiseerde graphscherm, alleen bij 3D

/************************  MAIN  ************************/

void main(int argc,char *argv[])
{

   unsigned disk;
   int wis=1,n;
   char drawdir[60];
	lost=FALSE;
/* strcpy(file,"Geen");*/
   print=FALSE;
   show=FALSE;
   num=FALSE;
   spearm=FALSE;
   upx=upy=upz=999999L;
   maxttest=1.0;
   clrscr();

   if (argc==4)  show=TRUE;
   if (argc>=3)
       {
	MAX_COL=atol(argv[1]);
	MAX_ROW=atol(argv[2]);
       }

	initdisplay();
   mallocdata();

   textmode(3);
	normvideo();
//   palette(0);

   strcpy(curdir,"?:\\");
   curdir[0]='A'+getdisk();
   maindisk=getdisk();
   getcurdir(0,curdir+3);
   if(curdir[(int)strlen(curdir)-1] != 92) strcat(curdir,"\\");
   strcpy(maindir,(char *)curdir);

  while(1)
   {
   wis=1;
  _setcursortype(_NOCURSOR);
   actie=display_menu(wis);
  _setcursortype(_NORMALCURSOR);
   if(print) prntest();
   if (actie==-1) actie=QUIT;

      switch (actie)

		{
      case LOAD:
	  veeg(0,0,25,80);
	  strcpy(fileload,(char *)filedir("*.wk?"));
	  if (strlen(fileload) < 2) break;
	  geladen=1;
	  clrscr();
	  read_wks(fileload);
	  break;

      case SPEARMAN:
	  if(spearm) spearm=0;
		else spearm=1;
	  wis=0;
	  break;

      case CHADIR:
	    veeg(24,0,1,80);
	    swrite(13,24,"RETURN or New directory: \0",0x1300);
	    gotoxy(39,25);
	    gets(dir);
		 if (strlen(dir) >1) changedir();
	    strcpy(curdir,"?:\\");
	    curdir[0]='A'+getdisk();
	    getcurdir(0,curdir+3);
	    if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
	    veeg(23,0,2,80);
	  break;

       case VIEW:
	  strcpy(drawdir,maindir);
	  strcat(drawdir,"DRAWPIC.EXE");
	  if(spawnlp(P_WAIT,drawdir,drawdir,NULL)==-1)
			 {
			   clrscr();
			   puts("Not enough memory or DRAWPIC not found\n");
			   delay(3000);
			 }
	  break;

       case PRINT:
	  if(!geladen) break;
	  clrscr();
	  display_labels();
	  display_data();
	  break;

       case CALC:
	  if(!geladen) break;
	  clrscr();
	  calc_corr();
	  break;

       case PRNONOF:
	  if(print) print=FALSE;
	     else {prntest();  print=TRUE;}
	  break;

       case LIJN:
	  if(lijn) lijn=FALSE;
	     else   lijn=TRUE;
	  break;

       case CONF:
	  if(conf) conf=FALSE;
	   else   conf=TRUE;
		  break;
       case LIJNXY:
	  if(lijnxy) lijnxy=FALSE;
	   else   lijnxy=TRUE;
		  break;

       case LIMIT:
	  clrscr();
	  gotoxy(1,6);
	  upx=upy=upz=-1;
		  while (upx<=0)
		     {
		       upx=-1;
		       printf("\nUpper X_as?  ");
		       scanf("%f",&upx);
		       fflush(stdin);
		     }
		  while(upy<=0)
		    {
		      upy=-1;
		      printf("Upper Y_as?  ");
		      scanf("%f",&upy);
		      fflush(stdin);
		     }
		  while(upz<=0)
		    {
		      upz=-1;
		      printf("Upper Z_as?  ");
		      scanf("%f",&upz);
		      fflush(stdin);
		     }
	   break;

       case PLAATJE:
	  if(!geladen) break;
	  num=FALSE;
	  teken_graph();
	  if(stricmp(outfile,"0")==0) break;
	  drawpic(outfile);
	  break;

		 case NPLAATJE:
	  if(!geladen) break;
	  num=TRUE;
	  teken_graph();
	  if(stricmp(outfile,"0")==0) break;
	  drawpic(outfile);
	  break;


       case MAXCORR:
	 clrscr();
	 maxttest=0.0;
	 printf("\n Enter min p value to print (bijv 1E-9) : ");
	 scanf("%f",&maxttest);
	 fflush(stdin);
	 break;

       case THREED:
	  if(THREE_D) THREE_D=FALSE;
	     else     THREE_D=TRUE;
	  break;


       case QUIT:
       default:
	  clrscr();
	  maindir[strlen(maindir)-1] =0;
	  setdisk(maindisk);
	  chdir(maindir);
//	  printf("%s\n",maindir);
	  freemallocdata();
	  _setcursortype(_NORMALCURSOR);
	  exit(-1);
      }
     }

  }



/* PRINTER TEST   EJN 040388


Dit programma test of de printer op printen staat en blijft dit
testen totdat negen pogingen zijn ondernomen
*/


void prntest(void)
{

int i,j,n,count;

count=0;

while(count<9)
 {
  count++;
  n= biosprint(2,0,0);

  if( n == 0x90) break;
	  nosound();
	  sound(9000);
/*  beep(200,10);*/
  clrscr();
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


void beep(unsigned freq, unsigned duration)
 {
    sound(freq);
    delay(duration);
    nosound();


 }
void mallocdata(void)
 {
   int n;

   label = (char  **)malloc(((int)MAX_COL)  * sizeof(char  *) );
   for (n=0;n < MAX_COL;n++)
   {
	label[n] = (char *) malloc((MAX_LEN) * sizeof(char));
	  if (label[n]==0)
	   {
		 printf("%u coreleft\n\n",coreleft());
		 puts("TOO LESS MEMORY\n");
		 exit(-1);
		}
	}

   temp = (float **)malloc( 3 * sizeof(float  *) );
   for (n=0;n < 3;n++)
   {
	temp[n] = (float  *) malloc(((int)MAX_ROW) * sizeof(float));
	  if (temp[n]==0)
	   {
		 printf("%u coreleft\n\n",coreleft());
		 puts("TOO LESS MEMORY\n");
		 exit(-1);
		}
	}

	sortx = (float  *) malloc(((int)MAX_ROW) * sizeof(float));
	sorty = (float  *) malloc(((int)MAX_ROW) * sizeof(float));

   data = (float  **)malloc(((int)MAX_ROW+5)  * sizeof(float  *) );
   for (n=0;n < MAX_ROW+5;n++)
   {
	data[n] = (float  *) malloc(((int)MAX_COL) * sizeof(float));
	  if (data[n]==0)
	   {
		 printf("%u coreleft\n\n",coreleft());
		 puts("TOO LESS MEMORY\n");
		 exit(-1);
		}
	}

   PP = (float  **)malloc(((int)MAX_COL+1) *  sizeof(float  *));
   for (n=0;n < MAX_COL;n++)
   {
	PP[n] = (float  *) malloc(((int)MAX_COL) * sizeof(float));
	 if (PP[n]==0)
	  {
		printf("%u coreleft\n\n",coreleft());
		puts("TOO LESS MEMORY\n");
		exit(-1);
	   }
   }
}

void freemallocdata(void)
{
  int n;
		 for (n=(int)MAX_COL-1;n >= 0;n--)	  free(PP[n]);
		 free(PP);

		 for (n=(int)MAX_ROW+5-1; n >= 0;n--)  free(data[n]);
		 free(data);

		 for (n=2; n >= 0;n--)		  free(temp[n]);
		 free(temp);

		 free(sortx);

		 free(sorty);

		 for (n=(int)MAX_COL-1; n >= 0;n--)	  free(label[n]);
		 free(label);

}
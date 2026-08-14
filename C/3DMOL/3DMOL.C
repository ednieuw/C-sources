 /* 3d molecuul   EJN feb 1995

	Options : char signed
		  word byte
		  compile large
 */
#include "corr.h"

#define breedte_item 20
#define lang 16

#define ROMAX 25
#define COMAX 80


void main(int argc,char *argv[]);
void beep(unsigned freq, unsigned duration);
void mallocdata(void);
void freemallocdata(void);

unsigned _stklen=18192;                   /* vergroot de stack */
unsigned long MAX_ROW=2500L;
unsigned long MAX_COL=25L;

float  **data;            /* DATA BLOK*/
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
int   lijn =1,lijnxy =1,conf=1,THREE_D=1;
float maxttest;
char  outfile[40];
int   DrawPicFile=TRUE;     // Schrijf PIC file naar disk
int   DrawGraphScreen=FALSE;// Teken op het geinitialiseerde graphscherm, alleen bij 3D

/************************  MAIN  ************************/

void main(int argc,char *argv[])
{

	int wis=1;
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

		 case PLAATJE:
	  if(!geladen) break;
	  num=FALSE;
	  teken_graph();
	  if(stricmp(outfile,"0")==0) break;
//	  drawpic(outfile);
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

	temp = (float **)malloc( 4 * sizeof(float  *) );
	for (n=0;n < 4;n++)
	{
	temp[n] = (float  *) malloc(((int)MAX_ROW) * sizeof(float));
	  if (temp[n]==0)
		{
		 printf("%u coreleft\n\n",coreleft());
		 puts("TOO LESS MEMORY\n");
		 exit(-1);
		}
	}
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

}

void freemallocdata(void)
{
  int n;

		 for (n=(int)MAX_ROW+5-1; n >= 0;n--)  free(data[n]);
		 free(data);

		 for (n=3; n >= 0;n--)		  free(temp[n]);
		 free(temp);

		 for (n=(int)MAX_COL-1; n >= 0;n--)	  free(label[n]);
		 free(label);

}
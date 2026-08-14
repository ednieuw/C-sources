/* pic2kyo: disassembly lotus.pic file to kyocera F1000 laserprinter
         20/06/1988 EJN

compile met:
	PIC (pic.h)
	transl (pic.h)
	PICMENU (pic.h)
	ENFILE
	VIDEO

   Options : char signed
             word byte
 */

#include "stdio.h"
#include "string.h"
#include "process.h"
#include "dos.h"
#include "PIC.H"
#include "dir.h"

#define breedte_item 20
#define lang 15

#define ROMAX 25
#define COMAX 80
#define MENMAX 10
#define MAXFILES 90
#define MAXLEN 80

extern int far *SCHERMPTR;
extern int display_menu(int);
extern int display_size_menu();
extern box(int ,int ,int ,int ,int ,int );
extern box1(int ,int ,int ,int ,int ,int );
extern linever(int ,int ,int ,int ,int );
extern linehor(int ,int ,int ,int ,int );
extern char *filedir(char *dir);
extern swrite(int ,int ,char * ,int);
extern initdisplay();
extern cursoroff();
extern cursoron();
extern cursor(int ,int );
extern mode(int );
extern palette(int);
extern veeg(int ,int ,int ,int );
extern translate_picfile();

extern float facx,facy;
extern int show;

float facx=1.8; /* deelfactor van lotus coordinaten naar kyocera coordinaten */
float facy=1.8;

int far *SCHERMPTR;
int actie;
int show;
int size = FULL;
char *curdir;
char *dir;
char fileload[40];
char maindir[40];
int maindisk;

/************************  MAIN  ************************/

void main()
{
   unsigned disk;
   int geladen=0;
   int wis=1,n;
   char drawdir[60];

   initdisplay();
   cursoroff();
   mode(3);
   palette(0);
   veeg(0,0,25,80);

   strcpy(curdir,"?:\\");
   curdir[0]='A'+getdisk();
   maindisk=getdisk();
   getcurdir(0,curdir+3);
   if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
   strcpy(maindir,(char *)curdir);

  while(1)
   {
   strcpy(curdir,"?:\\");
   curdir[0]='A'+getdisk();
   getcurdir(0,curdir+3);
   if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
   swrite(5,20,"Directory =                                 \0",0x0600);
   swrite(17,20,(char *)curdir,0x0600);
   cursor(0,0);
   actie=display_menu(wis);
   if (actie==-1) actie=QUIT;
    wis=1;
   switch (actie)
      {
      case LOAD:
        veeg(0,0,25,80);
        strcpy(fileload,filedir("*.pic"));
        if (strlen(fileload) < 2) break;
        translate_picfile();
        geladen=1;
        veeg(0,0,25,80);
 	break;
      case PICSIZE:
        size=display_size_menu();
        switch(size)
          {
           case FULL:
            facx=1.4;
            facy=1.4;
            break;
           case HALF:
            facx=1.8;
            facy=1.8;
            break;
           case THIRD:
            facx=1.4;
            facy=2.1;
            break;
           }
        wis=0;
      	break;
      case SHOW:
      	if(show) show=0;
          else show=1;
          wis=0;
          break;
      case EDIT:
          if(!geladen) break;
          spawnlp(P_WAIT,"PE.EXE","PE.EXE","KYO.PRN",NULL);
          break;
      case DUMP:
          if(!geladen) break;
          system("type kyo.prn >prn");
          veeg(0,0,25,80);
          break;
      case CHADIR:
          veeg(22,0,4,80);
          swrite(5,22,"RETURN or New directory: \0",0x1300);
          cursor(32,22);
          gets(dir);
	  if (strlen(dir) >1) changedir();
          veeg(22,0,4,80);
          break;
       case VIEW:
	  strcpy(drawdir,maindir);
          strcat(drawdir,"DRAWPIC.EXE");
          spawnlp(P_WAIT,drawdir,drawdir,NULL);
          break;
       case QUIT:
          veeg(0,0,25,80);
	  setdisk(maindisk);
	  chdir(maindir);
          for (n=2000;n>200;n-=50)
	  {
	   delay(2);
	   sound(n);
	  }
	  nosound();
          exit(-1);
          break;
        default:
          for (n=1;n<50;n++)
	  {
	   delay(1);
	   sound(2000);
	  }
	  nosound();
          break;
      }
      }

  }



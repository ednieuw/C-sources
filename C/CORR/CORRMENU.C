 /* dispmenu.c Ed Nieuwenhuys*/

#include "alloc.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "process.h"
#include "dir.h"
#include "time.h"
#include "dos.h"
#include "corr.h"
#include "graphics.h"
#include "bios.h"
#include "conio.h"

#define breedte_item 35
#define breedte_size_menu 15
#define lang 10

#define ROMAX 25
#define COMAX 80
#define MENMAX 10
#define MAXFILES 90
#define MAXTL 40

extern void swrite(int ,int ,char * ,int);
extern void Box(int topy,int topx,int leng,int breed,int type,int kleur);

int  display_menu(int wis);
int  display_size_menu();
//int  menuver(menu,num,ltopy,ltopx,breed);
int menuver(char menu[MAXFILES][MAXTL],int num,int ltopy,int ltopx,int breed);
int  load_dir();

extern int  *SCHERMPTR;
extern int  print;
extern int  spearm;
extern int  lost;
extern int  lijn,lijnxy,conf,THREE_D;
extern int  geladen;
extern int  columns,rows;
extern char curdir[40];
extern char fileload[40];
extern float upx,upy,upz,maxttest;
extern long MAX_ROW,MAX_COL;
char titel[MAXFILES][MAXTL];
char sizep[5][MAXTL];

int display_menu(int wis)
{
   int num,breed,keuze;
   char nop[10];
   long memsize;

   num=load_dir();                   /* laad tekst menu */
   if(wis) clrscr();
   swrite(14,1,"CORRELATION MATRIX EJN JULI 1993\0",0x0E00);
   gotoxy(13,1);
   printf("%6ld Cols  %6ld Rows",MAX_COL,MAX_ROW);
   gotoxy(6,3);
   if(geladen) printf(" Read : %6d Cols  %6d Rows",columns,rows);

   if (lost) swrite(13,24,"Rows or columns lost !",0xF400);

   memsize=(long) farcoreleft();
   ltoa(memsize,nop,10);
   swrite(0,0,nop,0x0B00);

   swrite(13,3,"Directory =                                 \0",0x0600);
   swrite(25,3,(char *)curdir,0x0600);
   if (geladen)
      {
       swrite (50,6,"File \0",0x0300);
       swrite (55,6,fileload,0x0300);
      }
   gotoxy(51,13);
   printf("Up X limit : %0.0f",upx);
   gotoxy(51,14);
   printf("Up Y limit : %0.0f",upy);
   gotoxy(51,15);
   printf("Up Z limit : %0.0f",upz);
   gotoxy(51,17);
   printf("Max p limit : %0.3G",maxttest);
   gotoxy(1,1);
   breed=breedte_item;
   keuze = menuver(titel,num,5,15,breed);
   if (keuze==QUIT || keuze==-1) clrscr();

   return((int) keuze);
}

int display_size_menu()
{
   int num,nr,breed,keuze;
   char naam[MAXTL];
   num=load_dir();                   /* laad tekst menu */
   veeg(4,58,10,20);
   breed=breedte_size_menu;
   keuze = menuver(sizep,num,5,60,breed)+1;
   if (keuze==0) keuze=1;
   veeg(4,58,10,20);
   return(keuze);
}

				/* verticaal menu */
int menuver(char menu[MAXFILES][MAXTL],int num,int ltopy,int ltopx,int breed)

//int menuver(menu,num,ltopy,ltopx,breed)
//char menu[MAXFILES][MAXTL];
//int  num, ltopy, ltopx, breed;
{
   long secs_now;
   char *str_now;
   int x, y, code, code1, oke, yy, xx, keer,leave,keus;
   int keuze;
   char ch;

   yy = ltopy;
   xx = ltopx;
   oke = 0;
   keuze = 0;
   Box(yy-1,xx-2,num+2,breed,2,0);

   while (1)
      {
	 yy=ltopy;
	 for (y=0; y<num; y++)
	    {
	      yy++;
	      xx=ltopx+1;
	       for (x=0; x<strlen(menu[y]); x++)
		  {
		     xx++;
		     ch = menu[y][x];
		     if (y==keuze)
			  *(SCHERMPTR + yy*COMAX + xx) = ch | 0x7000;
		     else
			if (x==0)
			  *(SCHERMPTR + yy*COMAX + xx) = ch | 0x4F00;
			else
			  *(SCHERMPTR + yy*COMAX + xx) = ch | 0x4E00;
		  }
	    }
	 if(oke == 1)
	    return(keuze);

	 while(bioskey(1)==0)
	 {
	   time(&secs_now);
	   str_now=ctime(&secs_now);
	   xx=55;
	   str_now[24]='\0';
	   swrite(xx,0,str_now,0x1300);
	  }

	 code = getch();
	 code1=0;
	 if(code==32) {code1=32; code=0;}
	 if(code == 0)
            {
            if(code1 !=32)  code1 = getch();
            switch (code1)
               {
               case 32:
		  ++keuze; break;
	       case 72:
		  --keuze; break;
               case 75:
		  --keuze; break;
               case 80:
                  ++keuze; break;
	       case 77:
                  ++keuze; break;
	       case 71:
		  keuze = 0; break;
               case 79:
                  keuze = num-1; break;
               }

               if(keuze < 0)
                  keuze = num-1;
               if(keuze > num-1)
                  keuze = 0;
            }
         else
	    {
            ch = code;
            keus=keuze;
            leave=0;
	    keer=-1;
	    while(keer<2)
	   {
	   keer++;
	   if(keer==1) { keus=-1;}
	    for(y=keus+1; y<num; y++)
	       {
	       if(strnicmp(&menu[y][0],&ch,1) == 0)
		  {
		  keuze = y;
		  oke = 0;
		  leave=1;
		  keer=2;
		  }
     if(leave) break;
	       }
	  }

/*            if(ch == 13)*/
	       oke = 1;
	    if(ch == 27)
	       {
	       keuze = -1;
	       oke = 1;
	       }
	    }
      }
}


int load_dir()
{
   int nr=16;  /*aantal items*/

      strcpy(titel[0], "Retrieve LOTUS file       \0");
      strcpy(titel[1], "Other drive / directory   \0");
      strcpy(titel[2], "View PIC file             \0");
      strcpy(titel[3], "Data printen              \0");
      strcpy(titel[4], "Calc Correlatie           \0");
     if(print)
      strcpy(titel[5], "Printer             ON/off\0");
 else strcpy(titel[5], "Printer             on/OFF\0");
      strcpy(titel[6], "XY upper limits           \0");
    if (spearm)
      strcpy(titel[7], "Spearman rank       ON/off\0");
 else strcpy(titel[7], "Spearman rank       OFF/on\0");
      strcpy(titel[8], "Graph                     \0");
      strcpy(titel[9], "Numbered graph            \0");
      strcpy(titel[10],"Max limit p to print      \0");
    if (lijn)
      strcpy(titel[11],"Line                ON/off\0");
 else strcpy(titel[11],"Line                on/OFF\0");
    if (lijnxy)
      strcpy(titel[12],"Y=X line            ON/off\0");
 else strcpy(titel[12],"Y=X line            on/OFF\0");
    if (conf)
      strcpy(titel[13],"95% Confidence line ON/off\0");
 else strcpy(titel[13],"95% Confidence line on/OFF\0");
    if (!THREE_D)
      strcpy(titel[14],"3d/2D                     \0");
 else strcpy(titel[14],"3D/2d                     \0");

      strcpy(titel[15],"Quit                      \0");

  return(nr);
 }

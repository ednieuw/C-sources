 /* dispmenu.c Ed Nieuwenhuys*/

#include "alloc.h"
#include"stdlib.h"
#include "string.h"
#include "stdio.h"
#include <process.h>
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
/* corr.H   18/09/88 */

#define TRUE 1
#define FALSE 0

#define LOAD 0
#define CHADIR 1
#define VIEW 2
#define PRINT 3
#define CALC 4
#define PRNONOF 5
#define LIMIT 6
#define SPEARMAN 7
#define PLAATJE 8
#define NPLAATJE 9
#define MAXCORR 10
#define LIJN 11
#define QUIT 12

#define MOVE 160
#define DRAW 162
#define FILL 48
#define FILLO 208
#define COLOR0 176
#define COLOR1 177
#define COLOR2 178
#define COLOR3 179
#define COLOR4 180
#define COLOR5 181
#define COLOR6 182
#define COLOR7 183
#define COLOR8 184
#define COLOR9 185
#define COLOR10 186
#define COLOR11 187
#define COLOR12 188
#define COLOR13 189
#define COLOR14 190
#define COLOR15 191
#define TEXT 168
#define FONT 167
#define SIZE 172
#define END 6
#define LX 3300              /* Lengte papier 11' in dots */

#define SPECIAL_KEY 0
#define UP 72
#define DOWN 80
#define HOME 71
#define ENDT 79
#define LEFT 75
#define UP 72
#define EXEC 13
#define ESC 27
#define RIGHT 77
#define PgUp 73
#define PgDn 81
#define SPACE 32
#define RETURN 13
#define TAB 9


#define ROMAX 25
#define COMAX 80
#define MENMAX 10
#define MAXFILES 90
#define MAXLEN 20
#define MAXTL 40

extern int far *SCHERMPTR;
extern swrite(int ,int ,char * ,int);
extern int print;
extern int LQG;
extern char curdir[40];
extern long MAX_ROW,MAX_COL;
extern int geladen;
extern char fileload[40];
extern float upx,upy;
char titel[MAXFILES][MAXTL];
char sizep[5][MAXTL];

int display_menu(int wis)
{
   int num,breed,keuze;
   char nop[10];
   long memsize;

   num=load_dir();                   /* laad tekst menu */
   if(wis) clrscr();
   swrite(13,0,"LOGIT regressie 05/04/89\0",0x0600);
   cursor(13,1);
   printf("%ld Cols  %ld Rows",MAX_COL,MAX_ROW);
   memsize=(long) farcoreleft();
   ltoa(memsize,nop,10);
   swrite(0,0,nop,0x0300);
   memsize=(long) coreleft();
   ltoa(memsize,nop,10);
   swrite(0,1,nop,0x0300);
   swrite(13,22,"Directory =                                 \0",0x0600);
   swrite(25,22,(char *)curdir,0x0600);
   if (geladen)
      {
       swrite (50,6,"File \0",0x0300);
       swrite (55,6,fileload,0x0300);
      }
   cursor(50,12);
   printf("Up X limiet : %0.0f",upx);
   cursor(50,13);
   printf("Up Y limiet : %0.0f",upy);
   cursor(0,0);
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
int menuver(menu,num,ltopy,ltopx,breed)
char menu[MAXFILES][MAXTL];
int  num, ltopy, ltopx, breed;
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
   box(yy-1,xx-2,num+2,breed,2,0);

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
                           *(SCHERMPTR + yy*COMAX + xx) = ch | 0x1300;
                        else
                           *(SCHERMPTR + yy*COMAX + xx) = ch | 0x4700;
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
   int nr=13;  /*aantal items*/

      strcpy(titel[0],"Retrieve LOTUS file       \0");
      strcpy(titel[1],"Other drive / directory   \0");
      strcpy(titel[2],"View PIC file             \0");
      strcpy(titel[3],"Data printen              \0");
      strcpy(titel[4],"Calc Correlatie           \0");
     if(print)
      strcpy(titel[5],"Printer ON/off            \0");
 else strcpy(titel[5],"Printer on/OFF            \0");
      strcpy(titel[6],"XY Limits                 \0");
      strcpy(titel[8],"Graph                     \0");
      strcpy(titel[9],"Numbered graph            \0");
      strcpy(titel[10],"Minlimit ttest (absoluut) \0");
      strcpy(titel[12],"Quit                      \0");

  return(nr);
 }


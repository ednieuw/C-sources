/* picmenu.c Ed Nieuwenhuys*/

#include "alloc.h"
#include "string.h"
#include "stdio.h"
#include <process.h>
#include "dir.h"
#include "time.h"
#include "dos.h"
#include "pic.h"

#define breedte_item 35
#define breedte_size_menu 15
#define lang 10

#define ROMAX 25
#define COMAX 80
#define MENMAX 10
#define MAXFILES 90
#define MAXLEN 20
#define MAXTL 40

extern int far *SCHERMPTR;
extern swrite(int ,int ,char * ,int);
extern veeg(int, int, int, int);
extern int size;
extern int show;

char titel[MAXFILES][MAXTL];
char sizep[5][MAXTL];


int display_menu(int wis)
{
   int num,nr,breed,keuze;
   char naam[MAXTL];
   num=load_dir();                   /* laad tekst menu */
   if(wis) veeg(0,0,20,80);
   swrite(15,0,"EDOPIC2KYOCERA TRANSLATOR 21/06/88\0",0x0600);
   breed=breedte_item;
   keuze = menuver(titel,num,5,15,breed);
   if (keuze==QUIT || keuze==-1) veeg(0,0,20,80);

   return(keuze);
}

int display_size_menu()
{
   int num,nr,breed,keuze;
   char naam[MAXTL];
   num=load_sizedir();                   /* laad tekst menu */
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
   char s[33];
   int x, y, code, code1, oke, yy, xx, keer,leave,keus;
   int keuze;
   char ch;

   yy = ltopy;
   xx = ltopx;
   oke = 0;
   keuze = 0;
   box(yy-1,xx-1,num+2,breed-3,2,0);

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
   int nr=8;

        strcpy(titel[0],"Load Lotus PIC file       \0");
        switch (size)
         {
       case FULL:
        strcpy(titel[1],"Change size = FULL        \0");
        break;
       case HALF:
        strcpy(titel[1],"Change size = HALF        \0");
	break;
       case THIRD:
        strcpy(titel[1],"Change size = THIRD       \0");
        break;
         }
      strcpy(titel[2],"Dump plaatje naar Kyocera \0");
      strcpy(titel[3],"Edit Pic files            \0");
    if (show)
      strcpy(titel[4],"Show ON/off               \0");
 else strcpy(titel[4],"Show on/OFF               \0");

      strcpy(titel[5],"Other drive / directory   \0");
      strcpy(titel[6],"View PIC file             \0");
      strcpy(titel[7],"Quit                      \0");


  return(nr);
 }

int load_sizedir()
{
   int nr=3;

      strcpy(sizep[0],"Full  \0");
      strcpy(sizep[1],"Half  \0");
      strcpy(sizep[2],"Third \0");


  return(nr);
 }

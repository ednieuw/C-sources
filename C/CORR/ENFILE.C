/* CORR ENFILE.C   11-01-88 */

#include "stdio.h"
#include "string.h"
#include "dir.h"
#include "dos.h"
#include "ctype.h"
#include "corr.h"
#include "conio.h"

#define MAXDISK 5
#define MAXCOL 80
#define CHBIN 0xF000    /* Blink Inevers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */

extern char fileload[];
extern char dir[40];
extern int  *SCHERMPTR;
extern void swrite (int,int,char*,int);

extern struct ffblk ffblk;

void veeg (int, int, int, int );
unsigned parse (char *);

struct ffblk ffblk;
unsigned disk;
char *directory;
char defaultdir[132];

/* Directory listen op scherm en keuze maken ****************************** */
/*                                                                          */
/* char dir 			zoek sleutel directory                      */
/*                                                                          */
/* ************************************************************************ */

char *filedir(char *dir)
{
   char filenaam[300][15];
   int y,leave,keus,keer;
   char ch;			/* te printen char */
   int begin = 0;               /* 1e file uit blok van 25 dat geprint word */
   int keuze = 0;		/* nummer gekozen file */
   int num = 0;                 /* aantal files */
   int oke;                     /* 0 = geen keuze ESC 1 = keuze RETURN */
   int toets;			/* toetsaanslag via getch() */
   int pos = 0;			/* positie binnen filenaam */
   int veld;			/* nummer van filenaam */
   int x = 0;			/* teller */
   int xx =0;                   /* row waar geprint moet worden */
   int yy = 0;                  /* colom waar geprint moet worden */

   oke = findfirst((char *)dir,&ffblk,0);
   while (!oke)
      {
      sscanf(ffblk.ff_name,"%s", &filenaam[x++]);
      if(x == 300)   break;
      oke = findnext(&ffblk);
      }
   num = x;
   oke = 0;

   Box(1,6,15,69,2,1);
   while (1)
      {
      for(veld = begin; veld < begin + 75; veld++)
         {
         yy = 2 + (veld - begin) / 5;
         xx = 8 + (((veld - begin) % 5) * 13);
         if(veld < num)
            {
            for(pos = 0; pos < 14; pos++, xx++)
               {
               if(pos < strlen((char *)filenaam[veld]) )
                  ch = (filenaam[veld][pos]);
               else
                  ch = 32;
               if (veld == keuze)
		  *(SCHERMPTR + yy * MAXCOL + xx) = ch | CHHIG;
               else
                  *(SCHERMPTR + yy * MAXCOL + xx) = ch | CHINV;
               }
            }
	 else
	    for(pos = 0; pos < 14; pos++, xx++)
               *(SCHERMPTR + yy * MAXCOL + xx) = 0x20 | CHINV;
         }
         if(oke > 0)
            {
            veeg(1,6,17,69);
	    if(oke==1) return ((char *)"\0");
	      else return((char *)filenaam[keuze]);
            }
         toets = getch();
         if(toets == 0)
            {
            toets = getch();
            switch (toets)
               {
               case 72: keuze -= 5; break;
               case 75: --keuze; break;
               case 80: keuze += 5; break;
               case 77: ++keuze; break;
               case 71: keuze = 0; break;
               case 79: keuze = num - 1; break;
               }
               if(keuze < 0)
                  keuze = num - 1;
               if(keuze > num + 4)
                  keuze = 0;
	       if(keuze > num - 1)
                  keuze = num - 1;
               if(keuze < 25)
                  begin = 0;
               else
		  begin = ((keuze - 20) / 5) * 5;
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
                  keuze = y;
                  oke = 0;
                  leave=1;
                  keer=2;
                  }
     if(leave) break;
               }
          }

            if(ch == RETURN)   oke = 2;
            if(ch == ESC) { keuze=-1;   oke = 1;}
	    }

      }
}

void changedir()
{
if ( parse(dir) )
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

unsigned parse(char *ptr)
{
 if ( ptr[1] == ':')
   {
    disk = (unsigned) toupper( ptr[0]) - 65;
    if (disk > MAXDISK) return(-1);
    directory = ptr+ 2;
   }
 else
     {
      disk=getdisk();
      directory=ptr;
     }

if (*directory=='\0') directory=".";

  return(0);
}
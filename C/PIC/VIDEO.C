/* VIDEO.C   20-06-88 */

#include "dos.h"

#define CHBIN 0xF000    /* Blink Inevers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */
#define CHLBD 201	/* Grafische characters */
#define CHLBE 218
#define CHRBD 187
#define CHRBE 191
#define CHLOD 200
#define CHLOE 192
#define CHROD 188
#define CHROE 217
#define CHHD  205
#define CHHE  196
#define CHVD  186
#define CHVE  179
#define CHSP  32        /* spatie */
#define CHRY  84	/* chr[CHRY][] */
#define CHRX  81        /* chr[][CHRX] */
#define MAXCOL 80
#define TRUE 1
#define FALSE 0


extern int far *SCHERMPTR;

veeg (int, int, int, int);

/* printen box op het scherm ********************************************** */
/*                                                                          */
/* int topy			row waar box geprint moet worden            */
/* int topx			col waar box geprint moet worden            */
/* int lang			lengte box                                  */
/* int breed			breedte box                                 */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur			0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

box(topy,topx,lang,breed,type,kleur)
{
   int x;			/* teller */
   int y;                       /* teller */
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
         clb = CHLBE; crb = CHRBE;
         clo = CHLOE; cro = CHROE;
         ch  = CHHE;  cv  = CHVE;
         break;
      case 2:
         clb = CHLBD; crb = CHRBD;
         clo = CHLOD; cro = CHROD;
         ch  = CHHD;  cv  = CHVD;
         break;
      }
   switch(kleur)
      {
      case 0: ck = CHNOR; break;
      case 1: ck = CHINV; break;
      case 2: ck = CHHIG; break;
      }
   *(SCHERMPTR + topy * MAXCOL + topx) = clb | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + topy * MAXCOL + topx + x) = ch | ck;
   *(SCHERMPTR + topy * MAXCOL + topx + (breed - 1)) = crb | ck;
   for(x = 0; x < lang; x++)
      {
      *(SCHERMPTR + (topy + x + 1) * MAXCOL + topx) = cv | ck;
      for(y = 1; y < breed - 1; y++)
         *(SCHERMPTR + (topy + x + 1) * MAXCOL + topx + y) = CHSP | ck;
      *(SCHERMPTR + (topy + x + 1) * MAXCOL + topx + (breed - 1)) = cv | ck;
      }
   *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx) = clo | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx + x) = ch | ck;
   *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx + (breed - 1)) = cro | ck;
}

/* printen box op het scherm alleen rand ********************************** */
/*                                                                          */
/* int topy			row waar box geprint moet worden            */
/* int topx			col waar box geprint moet worden            */
/* int lang			lengte box                                  */
/* int breed			breedte box                                 */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur			0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

box1(topy,topx,lang,breed,type,kleur)
{
   int x;			/* teller */
   int y;                       /* teller */
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
         clb = CHLBE; crb = CHRBE;
         clo = CHLOE; cro = CHROE;
         ch  = CHHE;  cv  = CHVE;
         break;
      case 2:
         clb = CHLBD; crb = CHRBD;
         clo = CHLOD; cro = CHROD;
         ch  = CHHD;  cv  = CHVD;
         break;
      }
   switch(kleur)
      {
      case 0: ck = CHNOR; break;
      case 1: ck = CHINV; break;
      case 2: ck = CHHIG; break;
      }
   *(SCHERMPTR + topy * MAXCOL + topx) = clb | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + topy * MAXCOL + topx + x) = ch | ck;
   *(SCHERMPTR + topy * MAXCOL + topx + (breed - 1)) = crb | ck;
   for(x = 0; x < lang; x++)
      {
      *(SCHERMPTR + (topy + x + 1) * MAXCOL + topx) = cv | ck;
      *(SCHERMPTR + (topy + x + 1) * MAXCOL + topx + (breed - 1)) = cv | ck;
      }
   *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx) = clo | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx + x) = ch | ck;
   *(SCHERMPTR + (topy + lang + 1) * MAXCOL + topx + (breed - 1)) = cro | ck;
}



/* printen horizontale lijn *********************************************** */
/*                                                                          */
/* int topy			row waar lijn geprint moet worden           */
/* int topx			col waar lijn geprint moet worden           */
/* int breed			breedte lijn                                */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur			0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

linehor(topy,topx,breed,type,kleur)
{
   unsigned int ch;		/* horizontale streep */
   unsigned int ck;		/* kleur waarin geprint moet worden */
   int x;			/* teller */

   switch(type)
      {
      case 1: ch = CHHE; break;
      case 2: ch = CHHD; break;
      }
   switch(kleur)
      {
      case 0: ck = CHNOR; break;
      case 1: ck = CHINV; break;
      case 2: ck = CHHIG; break;
      }
   for(x = topx; x < topx + breed; x++)
      *(SCHERMPTR + topy * MAXCOL + x) = ch | ck;
}

/* printen verticale lijn ************************************************* */
/*                                                                          */
/* int topy			row waar lijn geprint moet worden           */
/* int topx			col waar lijn geprint moet worden           */
/* int lang			lengte lijn                                 */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur			0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

linever(topy,topx,lang,type,kleur)	
{
   unsigned int cv;		/* verticale streep */
   unsigned int ck;		/* kleur waarin geprint moet worden */
   int y;			/* teller */

   switch(type)
      {
      case 1: cv = CHVE; break;
      case 2: cv = CHVD; break;
      }
   switch(kleur)
      {
      case 0: ck = CHNOR; break;
      case 1: ck = CHINV; break;
      case 2: ck = CHHIG; break;
      }
   for(y = topy; y < topy + lang; y++)
      *(SCHERMPTR + y * MAXCOL + topx) = cv | ck;
}



/***********************  write text  ****************/

swrite(int posx,int posy,char * tekst,int kleur)

{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;

   while(tekst[x] != '\0')
        {
         *(nop+x) = tekst[x] | kleur;
         x++;
         }
}


initdisplay()
{
union REGS reg;

reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int far *)( (reg.h.al != 7) ? 0XB8000000L : 0XB0000000L);

}

cursoroff()
{
union REGS reg;
reg.h.ah=1;
reg.h.ch=31;
reg.h.cl=31;
int86(0X10,&reg,&reg);
}

cursoron()
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
reg.h.ch=( (reg.h.al != 7) ? 6 : 12);
reg.h.cl=( (reg.h.al != 7) ? 7 : 14);
reg.h.ah=1;
int86(0X10,&reg,&reg);
}

cursor(int x ,int y)
{
union REGS reg;
reg.h.ah=2;
reg.h.dh=y;
reg.h.dl=x;
reg.h.bh=0;
int86(0X10,&reg,&reg);
}



mode(int mode_code)
{
 union REGS r;
 r.h.al=mode_code;
 r.h.ah=0;
 int86(0x10,&r,&r);
}

palette(int pnum)
 {
  union REGS r;
  r.h.bh=1;
  r.h.bl=pnum;
  r.h.ah=11;
  int86(0x10,&r,&r);
 }


veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
         *(SCHERMPTR + y*80 + x) = 0x20 | 0x0700;
}


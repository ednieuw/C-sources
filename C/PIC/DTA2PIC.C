/* editpic2 lotus.pic file EJN */


#include "stdio.h"
#include "string.h"
#include "dos.h"
#include "dir.h"
#include "ctype.h"
#include "graphics.h"
#include "io.h"
#include "fcntl.h"
#include "process.h"
#include "stdlib.h"
#include "conio.h"
#include "alloc.h"

#define CHBIN 0xF000    /* Blink Invers */
#define CHBHI 0x3E00	/* Blink Highligted */
#define CHHIG 0x0E00    /* Highligted */
#define CHINV 0x2300    /* Invers */
#define CHNOR 0x7300    /* Normaal */
#define BARCOLOR 0x3700
#define CHLBD 201	/* É */  /*Grafische characters */
#define CHLBE 218   /* Ú */
#define CHRBD 187   /* » */
#define CHRBE 191   /* ¿ */
#define CHLOD 200   /* È */
#define CHLOE 192   /* À */
#define CHROD 188   /* ¼ */
#define CHROE 217   /* Ù */
#define CHHD  205   /* Í */
#define CHHE  196   /* Ä */
#define CHVD  186   /* º */
#define CHVE  179   /* ³ */
#define CHSP  32        /* spatie */
#define CHRY  84	    /* chr[CHRY][] */
#define CHRX  81        /* chr[][CHRX] */


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
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8
#define UP 72
#define DOWN 80
#define HOME 71
#define ENDT 79
#define LEFT 75
#define UP 72
#define EXEC 13
#define ESC 0X1b
#define RIGHT 77
#define PgUp 73
#define PgDn 81
#define SPACE 32
#define RETURN 13
#define TAB 9
#define MAXLEN 90
#define PICXF 3000/4000       /* correctie factor naar LOTUS.PIC file */
#define PICYF 2200/3000
#define SBOX 25               /* grootte viekant meetpunt in plaatje */
#define H_OFFSET 400          /* offset linker onderhoek box plaatje */
#define V_OFFSET 550
#define H_FONT 8
#define V_FONT 8
#define H_SCALE 1.0           /* factor lengte en breedte box t.o.v. */
#define V_SCALE 1.0           /* overgebleven ruimte na offset */
#define H_WIDTH 4000          /* lengte en breedte lotus graphics screen */
#define V_WIDTH 3000
#define MAX_LEN 40
#define MAXTEXT 40
#define MAXTEXTX 40
#define AS_AANTAL 20
#define MAXREGELS 40
#define MAXCOL 24

#define XAS     0
#define YAS     1
#define BESTAND 2
#define BAR2D   3
#define BAR3D   4
#define LINE    5
#define POINT   6
#define DATA    7
#define HEADER  8

#define TRUE	1			/* Define some handy constants	*/
#define FALSE	0
#define ON	1
#define OFF	0

#define NULLPOINTER (char *) 0
#define PIC_KEYWORDS 24
#define POSITION_KEYWORDS 9
#define NOOFKEYWORDS PIC_KEYWORDS + POSITION_KEYWORDS
#define NOOFDTAITEMS 9

struct {
   char keyword[13];
   unsigned char op;
} optable[NOOFKEYWORDS]= {
   "COLOR0",COLOR0,
   "COLOR1",COLOR1,
   "COLOR10",COLOR10,
   "COLOR11",COLOR11,
   "COLOR12",COLOR12,
   "COLOR13",COLOR13,
   "COLOR14",COLOR14,
   "COLOR15",COLOR15,
   "COLOR2",COLOR2,
   "COLOR3",COLOR3,
   "COLOR4",COLOR4,
   "COLOR5",COLOR5,
   "COLOR6",COLOR6,
   "COLOR7",COLOR7,
   "COLOR8",COLOR8,
   "COLOR9",COLOR9,
   "DRAW",DRAW,
   "END",END,
   "FILL",FILL,
   "FILLO",FILLO,
   "FONT",FONT,
   "MOVE",MOVE,
   "SIZE",SIZE,
   "TEXT",TEXT,
   "CENTER",CENTER,
   "CENTERLEFT",CENTERLEFT,
   "CENTERTOP",CENTERTOP,
   "CENTERRIGHT",CENTERRIGHT,
   "CENTERBOTTOM",CENTERBOTTOM,
   "TOPLEFT",TOPLEFT,
   "TOPRIGHT",TOPRIGHT,
   "BOTTOMLEFT",BOTTOMLEFT,
   "BOTTOMRIGHT",BOTTOMRIGHT
};
struct {
   char keyword[13];
   unsigned char dta;
} dtatable[NOOFDTAITEMS]=
 {
   "X-AS"    ,XAS     ,
   "Y-AS"    ,YAS     ,
   "BESTAND",BESTAND ,
   "BAR2D"  ,BAR2D   ,
   "BAR3D"  ,BAR3D   ,
   "LINE"   ,LINE    ,
   "POINT"  ,POINT   ,
   "º"      ,DATA    ,
   "É"      ,HEADER
  };

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;     /* for palette info			*/

/*									*/
/*	Function prototypes						*/
/*									*/

void Initialize(void);
char *filedir();
initdisplay(void);
swrite(int posx,int posy,char * tekst,int kleur);
veeg(int topy,int topx,int totl,int totb);
char *filedir(char *dir);
read_dta_file(char *filein);
void box(int, int, int, int , int , int);
teken(double xp,double yp);
moef(double xp,double yp);
point(double xp,double yp);
numm(double xp,double yp,int number);
prnt(double xpos,double ypos,int size,int richting,char *tekst);
prntc(double xpos,double ypos,int size,int richting, char *tekst);
prntcl(double xpos,double ypos,int size,int richting, char *tekst);
prntct(double xpos,double ypos,int size,int richting, char *tekst);
picopen(char *outfile);
picmove(int x,int y);
picfont(char font);
piccolor(int color);
piccolor1(int color);
picsize(int x,int y);
picdraw(int x,int y);
pictext(int direction,int position,char *s);
picend(void);
picxy(int x,int y);
picbox(float ox,float oy,float lx,float ly,int style);
void teken_graph(void);
char *remove_lt_space(char *inputline);
makedate(int *d, int *m, int *y, unsigned dosdate);


int far *SCHERMPTR;
int *PTR  = (int *) 0XB800L ;
int regel=0,kolom=0,regelh=0;
float getal[MAXCOL][MAXREGELS];  /*array for data */
char header1[MAXCOL+1][20];      /* kolomnamen*/
char header2[MAXCOL+1][20];
char headerx[MAXREGELS+1][20];   /* regelnamen*/
char xas[60];
char yas[60];
char bestand[MAXTEXT+1];
double facx,facy;
int lost = FALSE;

int printer;
int MaxX, MaxY = 0;

int HSF= 6;		/*  Scale Factors */
int VSF= 13;

FILE *fpin;
FILE *fpout;
char *picfilein;
char filenaam[20];
int notused;

char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};

/*									*/
/*	Begin main function				*/
/*									*/

int	main(int argc,char **argv)
{

   clrscr();
   initdisplay();     /* init tekstscherm voor swrite */

   if (argc==2)  strcpy(filenaam,argv[1]);
   else    strcpy(filenaam,filedir("*.dta"));

   read_dta_file((char *)filenaam);
   teken_graph();

   exit(0);
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

/********************** init display  *****************/
initdisplay()
{
   GraphDriver = DETECT;		/* Request auto-detection	*/
   detectgraph(&GraphDriver,&GraphMode);
   SCHERMPTR=(int far *)( (GraphDriver == HERCMONO) ? 0XB0000000L : 0XB8000000L);
}

veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
         *(SCHERMPTR + y*80 + x) = 0x20 | 0x0700;
}

/******** haal datum uit ffblk.ff_fdate *************/
makedate(int *d, int *m, int *y,unsigned dosdate)
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

char *filedir(char *dir)
{
   struct ffblk ffblk;
   char nop[30];
   int y,leave;
   char tfilenaam[250][30];      /* nr,breedte filenaam + datum*/
   char filenaam[250][15];
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
/*   int maxaantal = 110; */         /* aantal files per beeldscherm */
   int keus=0;
   int keer=0;
   int day,month,year;

   oke = findfirst((char *)dir,&ffblk,0);
   while (!oke)
      {
	  sscanf(ffblk.ff_name,"%s", &filenaam[x]);
	  makedate(&day,&month,&year,ffblk.ff_fdate);
	  sprintf(tfilenaam[x],"%12.12s %2.2d-%2.2d-%2.2d",filenaam[x],day,month,year);
	  if(++x == 250)
		{ sound(100);
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
			   if(pos < strlen((char *)tfilenaam[veld]) )
				  ch = (tfilenaam[veld][pos]);
               else
                  ch = 32;
               if (veld == keuze)
                  *(SCHERMPTR + yy * 80 + xx) = ch | CHHIG;
               else
                  *(SCHERMPTR + yy * 80 + xx) = ch | CHINV;
               }
            }
         else
			for(pos = 0; pos < 21; pos++, xx++)
               *(SCHERMPTR + yy * 80 + xx) = 0x20 | CHINV;
         }
         if(oke > 0)
            {
			veeg(0,6,25,69);
            if(oke==1) exit(-1);
			  else return((char*)filenaam[keuze]);
            }
         toets = getch();
         if(toets == 0)
            {
            toets = getch();
            switch (toets)
               {
			   case UP: keuze -= 3; break;
               case LEFT: --keuze; break;
			   case DOWN: keuze += 3; break;
               case RIGHT: ++keuze; break;
               case HOME: keuze = 0; break;
               case ENDT: keuze = num - 1; break;
               }
               if(keuze < 0)
                  keuze = num - 1;
			   if(keuze > num - 1)
                  keuze = 0;
/*			   if(keuze > num - 1)
                  keuze = num - 1;
*/			   if(keuze < 25)
                  begin = 0;
               else
				  begin = ((keuze - 20) / 3 ) * 3;
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
				  if(keuze < 25)
                  begin = 0;
               else
				  begin = ((keuze - 20) / 3 ) * 3;
                  }
     if(leave) break;
               }
          }

            if(ch == RETURN)   oke = 2;
            if(ch == ESC) { keuze=-1;   oke = 1;}
            }

      }
}



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

void box(topy, topx, lang, breed, type, kleur)
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
   *(SCHERMPTR + topy * 80 + topx) = clb | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + topy * 80 + topx + x) = ch | ck;
   *(SCHERMPTR + topy * 80 + topx + (breed - 1)) = crb | ck;
   for(x = 0; x < lang; x++)
      {
      *(SCHERMPTR + (topy + x + 1) * 80 + topx) = cv | ck;
      for(y = 1; y < breed - 1; y++)
         *(SCHERMPTR + (topy + x + 1) * 80 + topx + y) = CHSP | ck;
      *(SCHERMPTR + (topy + x + 1) * 80 + topx + (breed - 1)) = cv | ck;
      }
   *(SCHERMPTR + (topy + lang + 1) * 80 + topx) = clo | ck;
   for(x = 1; x < breed - 1; x++)
      *(SCHERMPTR + (topy + lang + 1) * 80 + topx + x) = ch | ck;
   *(SCHERMPTR + (topy + lang + 1) * 80 + topx + (breed - 1)) = cro | ck;
}



error(n,count)
int n;
{
   printf("write error %d at bytecount %ld\n",n,count);
   exit(-1);
}
/************  remove kleiner dan spatie ***************/
char *remove_lt_space(char *inputline)
{
	int n;
	char nop[255];

	  strcpy(nop,strrev(inputline));
	  for(n=strlen(nop);n>0;n--)
			{ if(nop[n]<=' ') nop[n]=0; else break;}
	  strcpy(inputline,strrev(nop));
	  for(n=strlen(inputline);n>0;n--)
			{ if(inputline[n] <= ' ') inputline[n]=0; else break;}
 return(inputline);
}



/******************** READ DTA **************************/
read_dta_file(char *filein)
{
   int n;
   char  inputline[255];
   unsigned char found,dta;
   long int count;
   char *token,nop[255],nopp[25];
   float bars;

   printf("\n\nConversie %s ",filein);

   if ((fpin=fopen(filein,"rb")) == NULL)
	{
	   printf("cannot open input-file %s\n",filein);
       exit(-1);
	}

  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2700);
  strcpy(nopp,"                     ");

   count=30;
   bars=(float)( filelength(fileno(fpin)) ) / 21;

   do {
	  if(fgets(inputline,255,fpin)==NULL) { found=FALSE; break;}
	   count+=strlen(inputline)+2;
	   strnset(nopp,176,(int)(count/bars));
	   swrite(31,11,nopp,BARCOLOR);

	  dta=999;
	  found=TRUE;
	  strcpy(nop,"  ");
	  strcat(nop,inputline);
	  strcpy(inputline,remove_lt_space(nop));
	  if(inputline[0]=='º')          dta=DATA;
	  else if(inputline[0]=='É')     dta=HEADER;
	  else if(inputline[0]=='Ì')     dta=999;
	  else
	  {
	  token=strtok(inputline,":º");
		  for (n=0;n<NOOFDTAITEMS;n++)
			 if (stricmp(token,dtatable[n].keyword)==0)
				{
				  dta=dtatable[n].dta;
				  break;
				}
	   }


	  switch (dta)
	  {
	  case XAS:		strncpy(xas,strtok(NULLPOINTER,"\rº"),55);		break;
	  case YAS: 	strncpy(yas,strtok(NULLPOINTER,"\rº"),55);	   	break;
	  case BESTAND: strcpy(bestand,strtok(NULLPOINTER,"\rº"));	break;
	  case BAR2D:   break;
	  case BAR3D:   break;
	  case LINE:    break;
	  case POINT:   break;

	  case DATA:
		   if(regel >= MAXREGELS)
			  { printf("Data are lost, OVERFLOW\n"); lost=TRUE; break; }
		   kolom=0;
		   token=strtok(inputline,"º");
		   strncpy(headerx[regel],remove_lt_space(token),17);
			token=strtok(NULLPOINTER,"º");
			kolom++;
			while( token)
			{
			   strcpy(token,remove_lt_space(token));
				getal[kolom++][regel]=atof(token);
				if (kolom>=80-1)
				  {
				   puts("KOLOM OVERFLOW");
				   break;
				  }
				token=strtok(NULLPOINTER,"º");
			}
			regel++;
			break;

	  case HEADER:
	  kolom=0;
	  if(fgets(inputline,255,fpin)==NULL)		   found=FALSE;
	  else
	  {
	  token=strtok(inputline,"º");
	  token=strtok(NULLPOINTER,"º");
	  while( token)
		 {
		  strncpy(header1[kolom++],remove_lt_space(token),20);
		  token=strtok(NULLPOINTER,"º");
		  }
	  }
	  kolom=0;
	  if(fgets(inputline,255,fpin)==NULL)		   found=FALSE;
	  else
	  {
	  token=strtok(inputline,"º");
	  token=strtok(NULLPOINTER,"º");
	  while( token)
		 {
		  strncpy(header2[kolom++],remove_lt_space(token),20);
		  token=strtok(NULLPOINTER,"º");
		  }
	   }
	   break;

	   default:  break;
	  }
  }
	  while (found==TRUE);
  kolom--;
  regel--;           /*verwijder totaal regel */
  xas[MAXTEXTX]=0;
  yas[MAXTEXT]=0;

/*
puts("\n");
for(n=0;n<=regel;n++)
  {
	for(m=0;m<kolom;m++)
	{
	  printf("%6.1f  ",getal[m][n]);
	}
	printf("\n");
   }
*/

fclose(fpin);

}

/*************** Subroutines tekenen en schrijven PIC file ***************/

teken(double xp,double yp)
 {
   picdraw((int)(xp*PICXF),(int)(yp*PICYF+50));
 }

 moef(double xp,double yp)
 {
  picmove((int)(xp*PICXF),(int)(yp*PICYF+50));
 }

 point(double xp,double yp)
   {
   moef (xp+SBOX,yp+SBOX);
   teken (xp-SBOX,yp+SBOX);
   teken (xp-SBOX,yp-SBOX);
   teken (xp+SBOX,yp-SBOX);
   teken (xp+SBOX,yp+SBOX);
   }

numm(double xp,double yp,int number)
  {
   char tekst[5];

   itoa(number,tekst,10);
   prnt(xp,yp,1,0,tekst);
  }

prnt(double xpos,double ypos,int size,int richting,char *tekst)
   {
	moef(xpos,ypos);
	pictext(((int)(richting/90)),BOTTOMLEFT,tekst);
	notused=size;
   }



prntc(double xpos,double ypos,int size,int richting, char *tekst)
 {
  moef(xpos,ypos);
  pictext(((int)(richting/90)),CENTER,tekst);
  notused=size;
 }

prntcl(double xpos,double ypos,int size,int richting, char *tekst)
 {
  moef(xpos,ypos);
  pictext(((int)(richting/90)),CENTERLEFT,tekst);
  notused=size;
 }
prntct(double xpos,double ypos,int size,int richting, char *tekst)
 {
  moef(xpos,ypos);
  pictext(((int)(richting/90)),CENTERTOP,tekst);
  notused=size;
 }
prntbr(double xpos,double ypos,int size,int richting, char *tekst)
 {
  moef(xpos,ypos);
  pictext(((int)(richting/90)),BOTTOMRIGHT,tekst);
  notused=size;
 }



/*************** openen en verwerken PIC file ***************/

picopen(char *outfile)
{
int n;
unsigned char c;


   if ((fpout=fopen(outfile,"wb")) == NULL)
       {
       printf("cannot open output-file %s\n",outfile);
       exit(-1);
       }

   for (n=0;n<17;n++)
      {
      c=header_vector[n];
      fputc(c,fpout);
      }
   picsize(80,80);
   picfont(1);
   piccolor(0);
}

picmove(int x,int y)
 {
   fputc(MOVE,fpout);
   picxy(x,y);
 }

 picfont(char font)
  {
   fputc(FONT,fpout);
   fputc(font,fpout);
  }

piccolor(int color)
 {
   while(color>15) { color-=16; }
	fputc(COLOR0+color,fpout);
 }

piccolor1(int color)
 {
   while(color>14) { color-=15; }
	fputc(COLOR1+color,fpout);
 }

picsize(int x,int y)
 {
   fputc(SIZE,fpout);
   picxy(x,y);
 }

picdraw(int x,int y)
{
   fputc(DRAW,fpout);
   picxy(x,y);
}

pictext(int direction,int position,char *s)
 {
   fputc(TEXT,fpout);
   fputc((unsigned char)direction*16+position,fpout);
   fputs(s,fpout);
   fputc('\0',fpout);
 }

picend()
{
   fputc(96,fpout);    /*END*/
   fclose(fpout);
}

picxy(int x,int y)
 {
   fputc(x/256,fpout);
   fputc(x%256,fpout);
   fputc(y/256,fpout);
   fputc(y%256,fpout);
 }

picbox(float ox,float oy,float lx,float ly,int style)
 {
   double dox,doy,dlx,dly;
   char text[10];
   dox = H_OFFSET+ ox * facx;
   doy = V_OFFSET+ oy * facy;
   dlx = lx * facx + dox;
   dly = ly * facy + doy;
	moef(dox,doy);
   teken(dox,dly);
   teken(dlx,dly);
   teken(dlx,doy);
   teken(dox,doy);
   if(style !=999)
   {
	itoa(style,text,10);
	prntct(dox+(dlx-dox)/2,doy,1,0,text);
   }
   if(!ly)
	{
	 moef(dox+(dlx-dox)/2,doy);
	 teken(dox+(dlx-dox)/2,doy+20);
	}
 }


void teken_graph(void)
 {
 int kopregels;
 char nop[25];
 unsigned char keuze;
 char text[MAX_LEN];
 int i,j,n,m,skolom,toets;
 float maxx,maxy,increment,posx,teller,bars;
 float vy,asx,asy;
 double offpixx,offpixy;
 double lx,ly,mx,my;
 int style,skip;
 char fileout[20];
 unsigned nn;

 float as[AS_AANTAL]={0.01,0.02,0.05,0.1,0.2,0.5,1,2,5,10,20,50,100,250,
					  500,1000,2500,5000,10000,20000};

float *sgetal[MAXCOL];  /*array for data */
char  *sheader1[MAXCOL];
char  *sheader2[MAXCOL];
char kolom_use[MAXCOL];

while(TRUE)
{
  clrscr();
  maxx=maxy=0;
  puts("\n\nGeef file naam zonder PIC extensie");
  puts("Voer spatie in om te eindigen\n");
  puts("RETURN genereert Q.PIC\n");
  printf("PIC filenaam : ");
  gets(nop);
  nop[9]='\0';
  if(nop[0]==SPACE) break;
  if(nop[0]==0) strcpy(nop,"Q");
  strcat(nop,".PIC");
  strcpy(fileout,nop);


  clrscr();
  swrite(55,1,"Selecteer met Spatie    \0",0x7100);
  swrite(55,2,"Cursor om te verplaatsen\0",0x7100);
  swrite(55,3,"Return als OK           \0",0x7100);

  kolom_use[0]=FALSE;
  for (n=1;n<=kolom;n++)
	{
	 if(n>24) m=25; else m=0;
	 swrite(m+3,n,"#\0",0x0700);
	 swrite(m+5,n,header1[n],0x0700);
	 swrite(m+5+strlen(header1[n]),n,header2[n],0x0700);
	 kolom_use[n]=TRUE;
	}
   keuze =1;
   gotoxy(80,24);

   while(TRUE)
	   {
	 for (n=1;n<=kolom;n++)
	 {
	 if(n>24) m=25; else m=0;
	  swrite(m+5,n,header1[n],0x0700);
	  swrite(m+5+strlen(header1[n]),n,header2[n],0x0700);
	 }

	 if(keuze>24) m=25; else m=0;
   if (kolom_use[keuze]==TRUE) swrite(m+3,keuze,"#\0",0x0700);
	  else                 swrite(m+3,keuze," \0",0x0700);
   swrite(m+5,keuze,header1[keuze],0x7000);
   swrite(m+5+strlen(header1[keuze]),keuze,header2[keuze],0x7000);

		 toets = getch();
         if(toets == 0)
            {
            toets = getch();
            switch (toets)
               {
			   case UP:   --keuze; break;
               case LEFT: --keuze; break;
			   case DOWN: ++keuze; break;
			   case RIGHT:++keuze; break;
			   case HOME: keuze = 1; break;
			   case ENDT: keuze = kolom ; break;
               }
			}
         else
            {
			if(toets == RETURN)  break;
			if(toets == ESC) break;
			if(toets == SPACE)
			  {
				  if (kolom_use[keuze]==TRUE)
					{
					 if(keuze>24) m=25; else m=0;
					 swrite(m+3,keuze," \0",0x0700);
					 kolom_use[keuze] = FALSE;
					 keuze++;
					 }
				  else
					 {
					 if(keuze>24) m=25; else m=0;
					  swrite(m+3,keuze,"#\0",0x0700);
					  kolom_use[keuze] = TRUE;
					  }
			   }
			}
			if(keuze < 1)
				  keuze = kolom ;
			   if(keuze > kolom )
				  keuze = 1;

	  }  /*end while */
  if(toets == ESC) break;

  skolom=0;
  for (n=0;n<=kolom;n++)
	{
	 if (kolom_use[n]==TRUE)
	   { sheader1[skolom] = header1[n];
		 sheader2[skolom] = header2[n];
		 sgetal[skolom++] = getal[n];
	   }
	 }
  if(skolom==0) continue;

  picopen(fileout);

  for (i=0;i<regel;i++)            /*vind as vermenigvuldigingsfactor*/
	for(j=0;j<skolom;j++)
	 {
	  vy=sgetal[j][i];
	  if (vy>maxy) maxy=vy;
	  }

	 maxx=skolom;
	 maxy=maxy*1.05;

     for (i=0;i<=AS_AANTAL;i++)
     {
      asy=as[i];
      if (9*asy> maxy)
          break;
      }
	 for (i=6;i<=AS_AANTAL;i++)
     {
	  asx=as[i];
	  if (9*asx> maxx)
          break;
      }

     offpixx=H_OFFSET;
     offpixy=V_OFFSET;
	 kopregels=(int)(((float)(regel-1)/4)+1);
     lx=(H_WIDTH-H_OFFSET)*H_SCALE;               /*lengte assen*/
	 ly=(V_WIDTH-V_OFFSET)*V_SCALE-(float)kopregels*120-50;

     facx=lx/maxx; /*factor data*/
	 facy=ly/maxy;

	 piccolor(15);
     moef(offpixx,offpixy);                      /*draw box*/
     teken(lx+offpixx,offpixy);
     teken(lx+offpixx,ly+offpixy);
     teken(offpixx,ly+offpixy);
     teken(offpixx,offpixy);

/**** kopregels baridentificatie *****/
	 i=0;
	 j=V_WIDTH;
	 for (n=0;n<regel;n++)
	   {
		piccolor1(n);
		if (n<9) strcpy(nop," "); else strcpy(nop,"");
		itoa(n+1,text,10);
		strcat(nop,text);
		strcat(nop," = ");
		strcat(nop,headerx[n]);
		prnt((float)i,(float)j,1,0,(char*)&nop);
		i+=1000;
		if(i>3000) {i=0;j-=120;}
		}
		piccolor(15);


/**** xas labeling ****/
	for (i=0;i<(int)(maxx/asx);i++)
       {
		mx=(double)(((float)i+0.5)*facx*asx+offpixx);
        my=(double)(offpixy);
        moef(mx,my);
		prntc(mx,my-170.0,1,0,(char*)sheader1[i]);
		prntc(mx,my-300.0,1,0,(char*)sheader2[i]);
       }
	prntcl(10,my-170.0,1,0,(char*)header1[0]);
	prntcl(10,my-300.0,1,0,(char*)header2[0]);

/**** yas labeling ****/
   for (i=0;i<=(int)(maxy/asy);i++)
       {
        mx=offpixx;
		my=(double)((float)i*facy*asy+offpixy);
        moef(mx,my);
		teken(mx-100.0,my);
        gcvt((float)i*asy,3,text);
		prnt(mx-400.0,my-50,1,0,text);
       }
	prnt(offpixx+100,0,1,0 ,xas);
	prnt(50,offpixy+ly+50,1,0,yas);  /* print assen */
	prntbr(4000,offpixy+ly+50,1,0,bestand);

  increment=(float)skolom/(skolom*(regel+1)-1);
  skip=0;
  if(skolom*regel > 20) skip=1;
  if(skolom*regel > 30) skip=2;
  if(skolom*regel > 40) skip=3;
  if(skolom*regel > 80) skip=5;
  if(skolom*regel > 160) skip=10;

  swrite(55,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(55,11,"³                     ³",0x1700);
  swrite(55,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(55,13,"0%        50%      100%",0x2700);
  teller=1;
	 strcpy(nop,"                     ");
  bars=(float)(skolom*regel)/21;

  for(j=0;j<skolom;j++)
	{
	  posx=j;
	  for (i=0;i<regel;i++)
		 {
		   strnset(nop,176,(int)(++teller/bars));
		   swrite(56,11,nop,BARCOLOR);
		   piccolor1(i);
		   style=i+1;
		   if (skip)
			 if ((i/skip)*skip!=i) style=999;
		   picbox(posx,0,increment,sgetal[j][i],style);
		   posx+=increment;
		 }
	if(j < skolom-1)
	 {
		piccolor(15);
		moef(offpixx + (regel+0.5) * (j+1) * (increment * facx),offpixy);
		teken(offpixx + (regel+0.5) * (j+1) * (increment * facx),offpixy+ly);
	  }
	}
   picend();
   if(spawnlp(P_WAIT,"drawpic","drawpic",fileout,NULL)==-1)
	  {
	   printf("   Te weinig geheugen of DRAWPIC niet gevonden");
		sleep(2);
	  }

 }/*end while*/
}

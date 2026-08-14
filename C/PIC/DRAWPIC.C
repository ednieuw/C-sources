/* draw en dump lotus.pic file EJN */

/* .prj file
drawpic
cgaf.obj
hercf.obj
egavgaf.obj
littf.obj
*/
// 160789 positionering aangepast
// 210789 Hercules/CGA screendump toegevoegd
// 210789 Grootte size verbeterd
// 150290 EGA/VGA aangepast
// 170793 Test einde file toegevoegd & onbekende opcodemelding verwijderd
// 091094 Fill & fillo verbeterd

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <graphics.h>
#include <io.h>
#include <fcntl.h>
#include <bios.h>
#include <stdlib.h>
#include <\sys\stat.h>

#define CHBIN 0xF000    // Blink Invers
#define CHBHI 0x8F00	// Blink Highligted
#define CHHIG 0x0F00    // Highligted
#define CHINV 0x7000    // Invers
#define CHNOR 0x0700    // Normaal
#define CHLBD 201	// Grafische characters
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
#define END 96
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
#define MAXLEN 80
#define H_OFFSET 0.0
#define V_OFFSET 0.0
#define H_SCREEN 1        /* screen_units per pixel horizontaal */
#define V_SCREEN 1	  /* screen_units per pixel verticaal   */
#define H_FONT 9	  /* pixels horizontaal per char */
#define V_FONT 9	  /* pixels verticaal per char   */

#define TRUE	1
#define FALSE	0
#define ON	1
#define OFF	0


#define M_convert2x (int) (H_OFFSET+ (float)x / HSF)
#define M_convert2y (int) (float)MaxY-(V_OFFSET+(float)y/VSF)
#define M_getxeny   x=256*getc(fp)+getc(fp);\
		  y=256*getc(fp)+getc(fp);\
		  count+=4;

/*typedef struct tagPOINT {
					int x;
					int y;
				 } POINT;
*/
int   drawpos[1000];
int	drawtel;

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
char *filedir(char *);
void screendump(void);
void Initialize(void);
void swrite(int posx,int posy,char * tekst,int kleur);
void initdisplay(void);
void box(int, int, int, int, int, int);
void screendump(void);
void prnonoff(void);

int  far *SCHERMPTR;
int  printer;
int  MaxX, MaxY = 0;

float HSF;		/*  Scale Factors */
float VSF;
float xnop,ynop;
int size_multiplication=3;

char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};

/*									*/
/*	Begin main function						*/
/*									*/

void main(int argc,char **argv)
{
	int op,pos,nop,count,color,key;
	int n,x,y;
   unsigned char charx,chary,chop,c,direction,position,vertices,screenprint;
   unsigned char text[MAXLEN];
   char posstring[40];
	int h_size,v_size;
	float nopx,nopy;

	int font=SMALL_FONT;
	int sizexx=80,sizeyy=80;   /* uitgangsgroote letters */

	int sizex=120 ,sizey=120;    /* SIZE PIC FILE LETTERS VOOR INIT */
	FILE *fp;

	if(argc<1) exit(-1);

	clrscr();
	initdisplay();     /* init tekstscherm voor swrite */
	key='M';
while(key=='M' || key == 'm')
 {
	if ((fp=fopen(argv[1],"rb")) == NULL)
	 {
	 argv[1]=filedir("*.PIC");
	 fp=fopen(argv[1],"rb");
	 }

	Initialize() ;  	/* Set system into Graphics mode	*/

	nop=0;
	for (n=0;n<17;n++){
		c=getc(fp);
		if (c!=header_vector[n]) {
	 printf("%s is geen PIC-file",argv[1]);
	 closegraph();		/* Return the system to text mode	*/
	 exit(-1);
		}
	}
	chop=getc(fp);
	count=17;
	while ((chop>>4) != END && nop<20 ){
		switch(chop){
	  case MOVE:
			M_getxeny
			x=M_convert2x;
			y=M_convert2y;
			moveto(x,y);
		break;
	  case DRAW:
			M_getxeny
			x=M_convert2x;
			y=M_convert2y;
			lineto(x,y);
			break;
	  case FILL:
			vertices=getc(fp)+1;
			for (n=0;n<vertices*2;)
			  {
				 M_getxeny
				 drawpos[n++]=M_convert2x;
				 drawpos[n++]=M_convert2y;
			  }
			drawpos[n++]=drawpos[0];
			drawpos[n++]=drawpos[1];
			fillpoly(vertices+1,drawpos);
			break;
	  case FILLO:
			vertices=getc(fp)+1;
			for (n=0;n<vertices*2;)
			  {
				 M_getxeny
				 drawpos[n++]=M_convert2x;
				 drawpos[n++]=M_convert2y;
			  }
			drawpos[n++]=drawpos[0];
			drawpos[n++]=drawpos[1];
			drawpoly(vertices+1,drawpos);
			break;
/*	  case COLOR0:  if(getbkcolor()==0) color=1; else  color=0; break;
	  case COLOR1:  if(getbkcolor()==1) color=2; else  color=1; break;
	  case COLOR2:  if(getbkcolor()==2) color=3; else  color=2; break;
	  case COLOR3:  if(getbkcolor()==3) color=4; else  color=3; break;
	  case COLOR4:  if(getbkcolor()==4) color=5; else  color=4; break;
	  case COLOR5:  if(getbkcolor()==5) color=6; else  color=5; break;
	  case COLOR6:  if(getbkcolor()==6) color=7; else  color=6; break;
	  case COLOR7:  if(getbkcolor()==7) color=8; else  color=7; break;
	  case COLOR8:  if(getbkcolor()==8) color=9; else  color=8; break;
	  case COLOR9:  if(getbkcolor()==9) color=10;	else  color=9; break;
	  case COLOR10: if(getbkcolor()==10) color=11;	else  color=10; break;
	  case COLOR11: if(getbkcolor()==11) color=12;	else  color=11; break;
	  case COLOR12: if(getbkcolor()==12) color=13;	else  color=12; break;
	  case COLOR13: if(getbkcolor()==13) color=14;	else  color=13; break;
	  case COLOR14: if(getbkcolor()==14) color=15;	else  color=14; break;
	  case COLOR15: if(getbkcolor()==15) color=0; 	else  color=15; break;
*/
	  case COLOR0:
	  case COLOR1:
	  case COLOR2:
	  case COLOR3:
	  case COLOR4:
	  case COLOR5:
	  case COLOR6:
	  case COLOR7:
	  case COLOR8:
	  case COLOR9:
	  case COLOR10:
	  case COLOR11:
	  case COLOR12:
	  case COLOR13:
	  case COLOR14:
	  case COLOR15:
						color=chop-COLOR0;
						if(color==0)
						 color=DARKGRAY;
//						if(color>15) color=0;
						setcolor(color);
						setfillstyle(SOLID_FILL,color);
	  break;

	  case TEXT:
			pos=0;
			text[MAXLEN-1]='\0';
			c=getc(fp);
			count++;
			direction=c>>4;
			position=c%16;
			do {
		 c=getc(fp);
		 count++;
		 text[pos]=c;
		 pos++;
			}  while (c!=0 && pos<MAXLEN);
			pos--;

			nopx=10 * sizex / sizexx;
			nopy=10 * sizey / sizeyy;

		 switch (direction) {
		  case 0:
		  case 2:
			direction=0;
			settextstyle( font, HORIZ_DIR, 0 );
			setusercharsize((int)nopx,10,(int)nopy,10);
			break;
		  case 1:
		  case 3:
			direction=1;
			settextstyle( font, VERT_DIR, 0); /*USER_CHAR_SIZE );*/
			setusercharsize((int)nopy,10,(int)nopx,10);
			position += 10;
			break;
			}

			switch (position) {
		 case 0:
							  /* CENTER */
		    settextjustify( CENTER_TEXT, CENTER_TEXT );
		    break;
		 case 1:
							/* CENTERLEFT */
		    settextjustify( LEFT_TEXT, CENTER_TEXT );
		    break;
		 case 2:
							/* CENTERTOP */
		    settextjustify( CENTER_TEXT,TOP_TEXT );
						  break;
		 case 3:
											/* CENTERRIGHT */
		    settextjustify( RIGHT_TEXT, CENTER_TEXT );
                    break;
					  case 4:
											/* CENTERBOTTOM */
						  settextjustify( CENTER_TEXT, BOTTOM_TEXT );
                    break;
                 case 5:
				                    	/* TOPLEFT */
		    settextjustify( LEFT_TEXT ,TOP_TEXT );
						  break;
					  case 6:
                    					/* TOPRIGHT */
		    settextjustify( RIGHT_TEXT, TOP_TEXT );
                    break;
					  case 7:
							/* BOTTOMLEFT */
		    settextjustify( LEFT_TEXT, BOTTOM_TEXT );
						  break;
                 case 8:
							/* BOTTOMRIGHT */
			 settextjustify( RIGHT_TEXT, BOTTOM_TEXT );
		    break;
		 case 10:
							  /* CENTER */
		    settextjustify( CENTER_TEXT, CENTER_TEXT );
                    break;
		 case 11:
							/* CENTERLEFT */
		    settextjustify(  CENTER_TEXT, LEFT_TEXT );
			 break;
		 case 12:
                    					/* CENTERTOP */
		    settextjustify( CENTER_TEXT,CENTER_TEXT );
/*		    moverel(textheight(text),0);*/
						  break;
		 case 13:
											/* CENTERRIGHT */
		    settextjustify( CENTER_TEXT, RIGHT_TEXT );
                    break;
		 case 14:
                    					/* CENTERBOTTOM */
			 settextjustify( TOP_TEXT ,CENTER_TEXT );
						  break;
		 case 15:
				                    	/* TOPLEFT */
		    settextjustify( BOTTOM_TEXT, LEFT_TEXT );
			 break;
		 case 16:
                    					/* TOPRIGHT */
		    settextjustify( BOTTOM_TEXT, RIGHT_TEXT );
		    break;
		 case 17:
											/* BOTTOMLEFT */
			 settextjustify( TOP_TEXT, LEFT_TEXT );
			 break;
		 case 18:
							/* BOTTOMRIGHT */
			 settextjustify( TOP_TEXT, RIGHT_TEXT );
			 break;
			}


			outtext(text);
			break;
	  case FONT:
			getc(fp);
			count++;
			break;
	  case SIZE:
			sizex=( 256 * getc(fp)+getc(fp)) * size_multiplication /3;
			sizey=( 256 * getc(fp)+getc(fp)) * size_multiplication /3;
			count+=4;
			break;
	  case END: break;
	  default:
/*			sound(700);
			delay(500);
			nosound();
*///			printf("onbekende opcode %d\r",chop);
//			getch();
//			 closegraph();	// Return the system to text mode
//				  exit(-1);
			break;
		}
	if (feof(fp)||chop==END) {      //test einde file
	fclose(fp);
/*	sound(700);
	delay(500);
	nosound();
*/	break;
	  }
	chop=getc(fp);
	count++;
	}
  key=getch();
  if(key == TAB) screendump();
  argv[1]="\0";
  closegraph();			/* Return the system to text mode	*/
 }
}


/*									*/
/*	INITIALIZE: Initializes the graphics system and reports		*/
/*	any errors which occured.					*/
/*									*/

void Initialize(void)
{

  if (registerfarbgidriver(CGA_driver_far)    < 0) exit(-1);
  if (registerfarbgidriver(Herc_driver_far)   < 0) exit(-1);
  if (registerfarbgidriver(EGAVGA_driver_far) < 0) exit(-1);
  if (registerfarbgifont(small_font_far)      < 0) exit(-1);

  GraphDriver = DETECT;			// Request auto-detection

  initgraph( &GraphDriver, &GraphMode,"");
  ErrorCode = graphresult();		// Read result of initialization
  if( ErrorCode != grOk ){		// Error occured during init
	 printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
	 exit( 1 );
  }

  getpalette( &palette );		// Read the palette from board
  MaxColors = getmaxcolor() + 1;	// Read maximum number of colors

  MaxX = getmaxx();
  MaxY = getmaxy(); 			// Read size of screen

  HSF = 3400.0 / MaxX;   		// Horizontal conversionfactor
  VSF = 2400.0 / MaxY;   		// Verical conversionfactor
  setbkcolor(WHITE);
  setcolor(BLACK);

}


/***********************  write text  ****************/

void swrite(int posx,int posy,char * tekst,int kleur)

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
void initdisplay()
{
/*union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int  *far)( (reg.h.al == 7) ? 0XB0000000L : 0XB8000000L);
*/

	GraphDriver = DETECT;		/* Request auto-detection	*/
	detectgraph(&GraphDriver,&GraphMode);
	SCHERMPTR=(int far *)((GraphDriver== HERCMONO) ? 0XB0000000L : 0XB8000000L);
}

void veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
	 *(SCHERMPTR + y*80 + x) = 0x20 | 0x0700;
}



/* Directory listen op scherm en keuze maken ****************************** */
/*                                                                          */
/* char dir 			zoek sleutel directory                      */
/*                                                                          */
/* ************************************************************************ */

char *filedir(char *dir)
{
	struct ffblk ffblk;
   int y,leave;
   char nop[4];
   char filenaam[100][15];      /* nr,breedte*/
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
	int maxaantal = 50;          /* aantal files per beeldscherm */
	int keus=0;
	int keer=0;

	oke = findfirst((char *)dir,&ffblk,0);
   if (oke==-1) { printf("No PIC files found\0"); exit(-1);}
   while (!oke)
      {
      sscanf(ffblk.ff_name,"%s", &filenaam[x++]);
	  if(x == 100)
         break;
      oke = findnext(&ffblk);
      }
   num = x;
   oke = 0;

   box(1,6,10,69,2,1);
	swrite(5,24,"Als plaatje : TAB = screendump op EPSON printer",0x0300);
   swrite(5,20,"Druk RETURN in plaatje voor menu",0x0500);
	while (1)
      {
       swrite(5,22,"1 - 9 voor tekst vergroting Nu:\0",0x0300);
       itoa(size_multiplication,nop,10);
       swrite(37,22,nop,0x0400);

      for(veld = begin; veld < begin + maxaantal; veld++)
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
	    veeg(1,6,12,69);
	    if(oke==1) {clrscr(); exit(-1);}
              else return(filenaam[keuze]);
            }
         toets = getch();
         if(toets == 0)
            {
	    toets = getch();
	    switch (toets)
               {
               case UP: keuze -= 5; break;
               case LEFT: --keuze; break;
               case DOWN: keuze += 5; break;
               case RIGHT: ++keuze; break;
               case HOME: keuze = 0; break;
               case ENDT: keuze = num - 1; break;
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
            switch (toets)
					{
	       case '1': size_multiplication=1; break;
	       case '2': size_multiplication=2; break;
	       case '3': size_multiplication=3; break;
	       case '4': size_multiplication=4; break;
	       case '5': size_multiplication=5; break;
	       case '6': size_multiplication=6; break;
	       case '7': size_multiplication=7; break;
	       case '8': size_multiplication=8; break;
	       case '9': size_multiplication=9; break;
		}
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




/*   screendump 200789  Ed Nieuwenhuys  */



int printer;
void screendump(void)
{
register int colomn ,maxy;
register unsigned int pixy , regel;
unsigned char rowsum[720];
unsigned char str1[] = {27,64,27,65,8};      // reset /linespacing 8/72 inch
unsigned char str2[] = {27,76,128,2};        // ESC K, 640 char
unsigned char str3[] = {13,10};
unsigned char str4[] = {13,10,27,65,12};     // linespacing 1/6 inch
unsigned char str5[] = {32,32,32,32,32,32,32,32,32,32,32,32};
str2[2]=(getmaxx())%256;
str2[3]=(getmaxx())/256;

prnonoff();
printer = open( "PRN" , O_WRONLY|O_BINARY);
write(printer,str1,5);

 for ( regel = 0 ; regel< ((MaxY+5)>>3) ; regel++)
 { for ( colomn = 0 ; colomn < MaxX ; colomn++)
     {  rowsum[colomn] = 0 ;                         // zet byte op nul
	for ( pixy = 0 ; pixy < 8 ; pixy++ )
	 {  if (getpixel( colomn ,(regel<<3)+pixy )) // if screen dot
		 rowsum[colomn] += (128 >> pixy );
	 }                                           // einde loop byte
      }    					    					/* einde loop kolom */
   write(printer,str5,10);		       	     // print 10 spaces
   write(printer,str2,4);			   				/* ESC K,mod256,div256 */
   write(printer,rowsum,MaxX) ;
   write(printer,str3,2);
 }                                                   // einde regel
 write(printer,str4,5);
 close(printer);
 }


void prnonoff(void)
{
 if(biosprint(2,0,0)  != 0x90)
 {
  sound(2200);
  delay(13);
  nosound();
  sound(200);
  delay(300);
  nosound();
  closegraph();
  exit(-1);
 }
/* Verlaat het programma als de printer uitstaat*/
 }

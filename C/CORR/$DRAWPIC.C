/* draw en dump lotus.pic file EJN */

/* .prj file
drawpic
cga.obj
herc.obj
egavga.obj
*/
/* 160789 positionering aangepast*/
/* 210789 Hercules/CGA screendump toegevoegd */
/* 210789 Grootte size verbeterd  */
/* 150290 EGA/VGA aangepast */

#include "stdio.h"
#include "string.h"
#include "conio.h"
#include "dos.h"
#include "dir.h"
#include "ctype.h"
#include "graphics.h"
#include "io.h"
#include "fcntl.h"
#include "bios.h"
#include "\sys\stat.h"

#define CHBIN 0xF000    /* Blink Invers */
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
#define H_SCREEN 1        /* screen_units per pixel horizontaal */
#define V_SCREEN 1	  /* screen_units per pixel verticaal   */
#define H_FONT 9	  /* pixels horizontaal per char */
#define V_FONT 9	  /* pixels verticaal per char   */

#define TRUE	1			/* Define some handy constants	*/
#define FALSE	0			/* Define some handy constants	*/
#define ON	1			/* Define some handy constants	*/
#define OFF	0			/* Define some handy constants	*/

/*
int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;     /* for palette info			*/
*/
extern int    MaxX, MaxY;		/* The maximum resolution of the screen */
/*									*/
/*	Function prototypes						*/
/*									*/

int  drawpic(char *argvv);
extern int  Initialize(void);
void screendump(void);
int  prnonoff(void);

int printer;

float H_OFFSET = 0.0;
float V_OFFSET = 0.0;

extern float HSF;		/*  Scale Factors */
extern float VSF;
float xnop,ynop;
int size_multiplication=3;

extern char    header_vector[];
/* = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};*/

/*									*/
/*	Begin main function						*/
/*									*/

int drawpic(char *argvv)
{
   int pos,nop,count;
   int x,y;
   int chop,c,n,direction,position,vertices;
   char text[MAXLEN];
   float nopx,nopy;

   int font=SMALL_FONT;
   int sizexx=80,sizeyy=80;   /* uitgangsgroote letters */
   int sizex=120 ,sizey=120;    /* SIZE PIC FILE LETTERS VOOR INIT */
   FILE *fp;
   clrscr();

   if ((fp=fopen(argvv,"rb")) == NULL)         return(-1);

   Initialize() ;  	/* Set system into Graphics mode	*/
  setbkcolor(WHITE);
     nop=0;
   for (n=0;n<17;n++){
      c=getc(fp);
      if (c!=header_vector[n]) {
	 cprintf("%s is geen PIC-file",argvv);
	 closegraph();		/* Return the system to text mode	*/
	 return(-1);
      }
   }
   chop=getc(fp);
   count=17;
   while ((chop>>4) != END && nop<20 ){
      switch(chop){
	  case MOVE:
	      x=256*getc(fp)+getc(fp);
	      y=256*getc(fp)+getc(fp);
	      count+=4;
	      x= (int) (H_OFFSET+(float)x / HSF);
	      y= (int) ((float)MaxY-(V_OFFSET+(float)y/VSF));
	      moveto(x,y);
	   break;
	  case DRAW:
	      x=256*getc(fp)+getc(fp);
	      y=256*getc(fp)+getc(fp);
	      count+=4;
	      x= (int) (H_OFFSET+(float)x / HSF);
	      y= (int) ((float)MaxY-(V_OFFSET+(float)y/VSF));
	      lineto(x,y);
	      break;
	  case FILL:
	      vertices=getc(fp);
	      for (c=1;c<vertices;c++) {
		  x=256*getc(fp)+getc(fp);
		  y=256*getc(fp)+getc(fp);
		  count+=4;
	      }
	      x= (int) (H_OFFSET+(float)x / HSF);
	      y= (int) ((float)MaxY-(V_OFFSET+(float)y/VSF));
	      floodfill(x,y,getcolor());
	      break;
	  case FILLO:
	      vertices=getc(fp);
	      for (c=1;c<vertices;c++) {
		  x=256*getc(fp)+getc(fp);
		  y=256*getc(fp)+getc(fp);
		  count+=4;
	      }
	      x= (int) (H_OFFSET+(float)x / HSF);
	      y= (int) ((float)MaxY-(V_OFFSET+(float)y/VSF));
	      floodfill(x,y,getcolor());
	      break;

/*	  case COLOR0:  if(getbkcolor()==0) setcolor(15);
					else  setcolor(1); break;
	  case COLOR15:  if(getbkcolor()==1) setcolor(2);
					else  setcolor(1); break;
	  case COLOR14:  if(getbkcolor()==2) setcolor(3);
					else  setcolor(2); break;
	  case COLOR13:  if(getbkcolor()==3) setcolor(4);
					else  setcolor(3); break;
	  case COLOR12:  if(getbkcolor()==4) setcolor(5);
					else  setcolor(4); break;
	  case COLOR11:  if(getbkcolor()==5) setcolor(6);
					else  setcolor(5); break;
	  case COLOR10:  if(getbkcolor()==6) setcolor(7);
					else  setcolor(6); break;
	  case COLOR9:  if(getbkcolor()==7) setcolor(8);
					else  setcolor(7); break;
	  case COLOR8:  if(getbkcolor()==8) setcolor(9);
					else  setcolor(8); break;
	  case COLOR7:  if(getbkcolor()==9) setcolor(10);
					else  setcolor(9); break;
	  case COLOR6: if(getbkcolor()==10) setcolor(11);
					else  setcolor(10); break;
	  case COLOR5: if(getbkcolor()==11) setcolor(12);
					else  setcolor(11); break;
	  case COLOR4: if(getbkcolor()==12) setcolor(13);
					else  setcolor(12); break;
	  case COLOR3: if(getbkcolor()==13) setcolor(14);
					else  setcolor(13); break;
	  case COLOR2: if(getbkcolor()==14) setcolor(15);
					else  setcolor(14); break;
	  case COLOR1: if(getbkcolor()==15) setcolor(0);
					else  setcolor(15); break;
*/
	  case COLOR0:  setcolor(56); break;
	  case COLOR15: setcolor(62); break;
	  case COLOR14: setcolor(61); break;
	  case COLOR13: setcolor(60); break;
	  case COLOR12: setcolor(59); break;
	  case COLOR11: setcolor(58); break;
	  case COLOR10: setcolor(57); break;
	  case COLOR9:  setcolor(56); break;
	  case COLOR8:  setcolor(20); break;
	  case COLOR7:  setcolor(7); break;
	  case COLOR6:  setcolor(6); break;
	  case COLOR5:  setcolor(5); break;
	  case COLOR4:  setcolor(4); break;
	  case COLOR3:  setcolor(3); break;
	  case COLOR2:  setcolor(2); break;
	  case COLOR1:  setcolor(1); break;

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
              chop=getc(fp);
              count++;
              break;
          case SIZE:
	      sizex=( 256 * getc(fp)+getc(fp)) * size_multiplication /3;
	      sizey=( 256 * getc(fp)+getc(fp)) * size_multiplication /3;
              count+=4;
              break;
          default:
/*	      cprintf("onbekende opcode %d\r\n",chop);
              closegraph();	/* Return the system to text mode	*/
              fclose(fp);
              return(-1);
  */    break;
      }
      chop=getc(fp);
      count++;
   }
  if(getch() == TAB) screendump();
  argvv="\0";
  closegraph();			/* Return the system to text mode	*/
  fclose(fp);
  return(1);
}

/*
/*									*/
/*	INITIALIZE: Initializes the graphics system and reports		*/
/*	any errors which occured.					*/
/*									*/

int Initialize(void)
{

  if (registerbgidriver(CGA_driver)    < 0) return(-1);
  if (registerbgidriver(Herc_driver)   < 0) return(-1);
  if (registerbgidriver(EGAVGA_driver) < 0) return(-1);
  if (registerbgifont(small_font)      < 0) return(-1);

  GraphDriver = DETECT;			/* Request auto-detection	*/

  initgraph( &GraphDriver, &GraphMode,"");
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    cprintf(" Graphics System Error: %s\r\n", grapherrormsg( ErrorCode ) );
    return( 1 );
  }
  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy(); /* Read size of screen		*/

  HSF = 3400.0 / (float)MaxX;   /* Horizontal conversionfactor */
  VSF = 2400.0 / (float)MaxY;   /* Verical conversionfactor */

return(1);
}
*/

/*   screendump 200789  Ed Nieuwenhuys  */

void screendump(void)
{
register int colomn;
register unsigned int pixy , regel;
unsigned char rowsum[720];
unsigned char str1[] = {27,64,27,65,8};      /* reset /linespacing 8/72 inch */
unsigned char str2[] = {27,76,128,2};                     /* ESC K, 640 char */
unsigned char str3[] = {13,10};
unsigned char str4[] = {13,10,27,65,12};             /* linespacing 1/6 inch */
unsigned char str5[] = {32,32,32,32,32,32,32,32,32,32,32,32};
str2[2]=(getmaxx())%256;
str2[3]=(getmaxx())/256;

//prnonoff();
printer = open( "PRN" , O_WRONLY|O_BINARY);
write(printer,str1,5);

 for ( regel = 0 ; regel< ((MaxY+5)>>3) ; regel++)
 {
   for ( colomn = 0 ; colomn < MaxX ; colomn++)
     {
	  rowsum[colomn] = 0 ;                          /* zet byte op nul */
      for ( pixy = 0 ; pixy < 8 ; pixy++ )
         {
		  if (getpixel( colomn ,(regel<<3)+pixy ))  /* if screen dot */
			 rowsum[colomn] += (128 >> pixy );
		 }                                          /* einde loop byte*/
	  }    					    					/* einde loop kolom */
   write(printer,str5,10);		       				/* print 10 spaces */
   write(printer,str2,4);			   				/* ESC K,mod256,div256 */
   write(printer,rowsum,MaxX) ;
   write(printer,str3,2);
 }                                                  /* einde regel */
 write(printer,str4,5);                             /* linespacing 1/6 inch */
 close(printer);
 }


int prnonoff(void)
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
  return(-1);
 }
/* Verlaat het programma als de printer uitstaat*/
 return(1);
 }

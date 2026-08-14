#define TRUE            1
#define FALSE 			0
#define MAXROWS 45
#define MAXCOLUMNS 60
#define ROWS 18
#define COLUMNS 24
#define AANTAL 6
#define SHOW            FALSE
#define TRACE           FALSE
#define X_ORIGIN 20
#define Y_ORIGIN 20
#define XLENGTH 3000
#define YLENGTH 2200
#define E3 printf("\nError writing file to disc: Disc full?")
#define E_OPENIN printf("\ncannot open \"%s\" for input\n",s)
#define E_OPENOUT printf("\ncannot open \"%s\" for output\n",s)

// wmf-header

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned short      USHORT;
typedef unsigned int        UINT;
#define WMFFACTOR 2.3
#define PENCOLOR BLACK     /* 0=black, FF-red, FF00=green, FF0000=blue*/
#define BRUSHCOLOR GREEN     /* 0=black, FF-red, FF00=green, FF0000=blue*/
#define PAPERCOLOR 0X777777L
#define BRUSHSTYLE 0
#define HEIGHT 5397L
#define WIDTH 6985L
#define OBJECTS 20
#define PEN 0
#define BRUSH 1

void wmfopen(void);
void wmfend(void);
WORD CreateObject(WORD obj);
//WORD CreatePen(DWORD color);
WORD pen(WORD style, WORD width, DWORD color);
//WORD CreateFont(WORD Orientation, WORD font, WORD Hsize, WORD Vsize);
//WORD CreateBrush(DWORD color);
WORD brush(WORD style, WORD hatch, DWORD color);
void wmfmove(WORD x,WORD y);
void wmfdraw(WORD x,WORD y);
//void wmfdot(WORD x, WORD y, WORD size, WORD style, DWORD color);
void putbyte(BYTE b);
void putword(WORD w);
void putdword(DWORD dw);


/* pic.h: header for pic-files 280288 */

#define MOVE 160
#define DRAW 162
#define COLOR 1		/* number of colors */
#define COLOR0 176
#define TEXT 168
#define FONT 167
#define SIZE 172
#define END 0X60
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8

/* keyboard.h 230288 */
#define ESC 27
#define SPACE 32
#define RETURN 13
#define BACKSPACE 8
#define CTRL_C 3

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <process.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <fcntl.h>
#include <dos.h>
#include <dir.h>

void openout(char *filename);
void picopen(void);
void piccorner();
void picfont(char font);
void piccolor(int color);
void picsize(int x,int y);
void picxy(int x,int y);
void picmove(int x,int y);
void picdraw(int x,int y);
void pictext(int direction,int position,char *s);
void picdot(int x,int y, int type, int size);
void picend();
void cross(int r, int c);
void plafond(int x, int y);
void rechterwand (int x, int y);
void check(int x, int y, int flag);
FILE *fppic, *fpwmf, *tracefile;
char picfile[]= "kamer00.PIC";
char wmffile[]= "kamer00.wmf";
int columns, rows, DX, DY;

int Pen=-1,Brush=-1,Font=-1, Filltype=-1, BrushStyle, Bold, Italic, Underline, Charset;
DWORD BrushColor=BRUSHCOLOR;
DWORD PenColor=PENCOLOR;
BYTE NewPenColor=1, NewBrushColor=1;
WORD PenWidth, PenStyle;
int object[OBJECTS];
WORD noofobjects, Orientation=0;
/* no vertical central alignment! Only: Base, Bottom and Top
 0  6  2       1  7  3
 8 14 10       9 15 11
24 30 26      25 31 27
introduced: 125, 131 and 127 for vertical central alignment
*/
WORD currentx, currenty;
DWORD maxrecordsize=20;



/* h=horizontale wanden (plafond); v=verticale wanden (rechts); k=kamer */
/* linkerwand en vloer komen van de buren */
char h[MAXROWS+1][MAXCOLUMNS+1], v[MAXROWS+1][MAXCOLUMNS+1], k[MAXROWS+1][MAXCOLUMNS+1];

char header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
/************************ START OF MAIN ************************/

int main(int argc, char *argv[]) {
   int aantal, productie, r, c, q;
   if (argc>1) columns=atoi(argv[1]);
   else columns=COLUMNS;
/*
   else {
      printf("\nHoeveel horizontaal (maximaal %d): ",COLUMNS);
      scanf("%d",&columns);
   }
*/
   if (columns<5) columns=5;
   if (columns>MAXCOLUMNS) columns=MAXCOLUMNS;
   rows=columns*3/4;
//cprintf("\r\n%s %4d : columns=%d rows=%d",__FILE__, __LINE__,columns,rows);if (getch()==27) exit(-1);putch('\r');putch('\n');
   DX=XLENGTH/columns;
   DY=YLENGTH/rows;
   clrscr();
/*
   printf("Hoeveel plaatjes");
   scanf("%d",&aantal);
*/
   randomize();
   aantal=AANTAL;
   for (productie=0;productie<aantal;productie++) {
      sprintf(picfile,"kamer%02d.pic",productie);
      sprintf(wmffile,"kamer%02d.wmf",productie);
      for (r=0;r<rows;r++) {
         for (c=0;c<columns;c++) {
            v[r][c]=0;
            h[r][c]=0;
            k[r][c]=0;
         }
      }
      for (r=0;r<rows;r++) {
         v[r][columns-1]=1;
         k[r][columns-1]=1;
         k[r][0]=1;
      }
      for (c=0;c<columns;c++) {
         h[rows-1][c]=1;
         k[rows-1][c]=1;
         k[0][c]=1;
      }
      k[0][0]= k[rows-1][0]= k[0][columns-1]= k[rows-1][columns-1]= 2;
      for (q=0;q<3*rows*columns;q++) {
         c=rand()%columns;
         r=rand()%rows;
         check(r,c, TRUE);
         c=rand()%columns;
         r=rand()%rows;
         check(r,c, FALSE);
      }
      for (q=r=0  ;r<rows;r++) for (c=0;c<columns;c++) check(r,c, TRUE);
      for (q=1,r=0;r<rows;r++) for (c=0;c<columns;c++) check(r,c, FALSE);
      picopen();
      wmfopen();
      picmove(X_ORIGIN,Y_ORIGIN+rows*DY);
      picdraw(X_ORIGIN,Y_ORIGIN);
      picdraw(X_ORIGIN+columns*DX,Y_ORIGIN);
      wmfmove(X_ORIGIN,Y_ORIGIN+rows*DY);
      wmfdraw(X_ORIGIN,Y_ORIGIN);
      wmfdraw(X_ORIGIN+columns*DX,Y_ORIGIN);
      for (r=0;r<=rows;r++) {
         putch('.');
         for (c=0;c<=columns;c++) {
            cross(r,c);
            }
      }
      for (r=0;r<rows;r++) {
         putch('.');
         for (c=0;c<columns;c++) {
            if (h[r][c]) plafond(r,c);
         }
      }
      for (r=0;r<rows;r++) {
         putch('.');
         for (c=0;c<columns;c++) {
            if (v[r][c]) rechterwand(r,c);
         }
      }
      fputc(END,fppic);
      if (fclose(fppic)==EOF) {
        E3;
        exit(0);
      }
      wmfend();
/*
      if (SHOW) {
         if (spawnlp(P_WAIT,"drawpic.exe","drawpic.exe",picfile,NULL)==-1) {
            printf("\nspawn-error\n");
            exit(-1);
         }
      }
*/
   }
   return 1;
}
   
   /****************************************************************
   *								*
   *			END OF MAIN()				*
   *								*
   ****************************************************************/
   
void check(int r, int c, int horizontaal) {
   if (horizontaal && !h[r][c] && k[r][c]<2 && k[r+1][c]<2) {
      h[r][c]=1;
      k[r][c]++;
      if (r<rows-1) k[r+1][c]++;
   }
   if (!horizontaal && !v[r][c] && k[r][c]<2 && k[r][c+1]<2) {
      v[r][c]=1;
      k[r][c]++;
      if (c<columns-1) k[r][c+1]++;
   }
}

void plafond(int r, int c) {
   r++;
   picmove(X_ORIGIN+c*DX, Y_ORIGIN + r*DY);
   picdraw(X_ORIGIN+c*DX+DX, Y_ORIGIN+ r*DY);
   wmfmove(X_ORIGIN+c*DX, Y_ORIGIN + r*DY);
   wmfdraw(X_ORIGIN+c*DX+DX, Y_ORIGIN+ r*DY);
}

void rechterwand (int r, int c) {
   c++;
   picmove(X_ORIGIN+c*DX, Y_ORIGIN+r*DY);
   picdraw(X_ORIGIN+c*DX, Y_ORIGIN+r*DY+DY);
   wmfmove(X_ORIGIN+c*DX, Y_ORIGIN+r*DY);
   wmfdraw(X_ORIGIN+c*DX, Y_ORIGIN+r*DY+DY);
}

void cross(int r, int c) {
   picmove(X_ORIGIN+c*DX-4, Y_ORIGIN+r*DY);
   picdraw(X_ORIGIN+c*DX+4, Y_ORIGIN+r*DY);
   picmove(X_ORIGIN+c*DX, Y_ORIGIN+r*DY-4);
   picdraw(X_ORIGIN+c*DX, Y_ORIGIN+r*DY+4);
   wmfmove(X_ORIGIN+c*DX-4, Y_ORIGIN+r*DY);
   wmfdraw(X_ORIGIN+c*DX+4, Y_ORIGIN+r*DY);
   wmfmove(X_ORIGIN+c*DX, Y_ORIGIN+r*DY-4);
   wmfdraw(X_ORIGIN+c*DX, Y_ORIGIN+r*DY+4);
}

void picopen(void) {
int n;
unsigned char c;
   openout(picfile);
   for (n=0;n<17;n++){
      c=header_vector[n];
      fputc(c,fppic);
   }
   fputc(COLOR0,fppic);
}

void picxy(int x,int y) {
   fputc(x/256,fppic);
   fputc(x%256,fppic);
   fputc(y/256,fppic);
   fputc(y%256,fppic);
}

void picmove(int x,int y) {
   fputc(MOVE,fppic);
   picxy(x,y);
}
void picdraw(int x,int y) {
   fputc(DRAW,fppic);
   picxy(x,y);
}

void picend(void) {
   fputc(END,fppic);
   fclose(fppic);
   if (fclose(fppic)==EOF) {
     E3;
     exit(-1);
   }
}

void openout(char *s) {
   if ((fppic=fopen(s,"wb")) == NULL) {
       E_OPENOUT;
       exit(-1);
   }
}

// WMF routines

void wmfopen(void) {
   int i;
   WORD checksum;
   for (i=0; i<OBJECTS; i++) object[i]=-1;
   noofobjects=0;
   if ((fpwmf=fopen(wmffile,"wb")) == NULL) {
      cprintf("Unable to open %s for output\r\n",wmffile);
      exit(-1);
   }

/* Placeable metafile pre-header: */
   putword(0XCDD7);
   putword(0X9AC6); /* Placeable metafile: CDD7h,9AC6h */
   checksum= 0XCDD7;
   checksum^=0X9AC6;
   putword(0);      /*  handle=0 */
   putword(0);      /*  left */
   putword(0);      /*  top */
   putword(WIDTH);  /*  bottom */
   checksum^=WIDTH;
   putword(HEIGHT); /*  right */
   checksum^=HEIGHT;
/*
   putword(576);    /*  pixels per inch */
   checksum^=576;
*/
   putword(2540);    /*  pixels per inch */
   checksum^=2540;
   putword(0);      /*  reserved 1 */
   putword(0);      /*  reserved 2 */
   putword(checksum); /* checksum =0X53F1*/

/* start of regular WMF-file: */
   putword(1);      /* = 1 */
   putword(9);      /* Headersize = 9 */
   putword(768);    /* Windows-version= 0300h */
   putdword(-1L);   /* Filesize */
   putword(3);      /* NumOfObjects */
   putdword(20L);   /* MaxRecordSize */
   putword(0);      /* 0 */
   putdword(4L);
   putword(0X0103); /* SetMapMode */
   putword(8);
   putdword(5L);    /* Size */
   putword(0X020B); /* SetWindowOrg */
   putword(0);
   putword(0);
   putdword(5L); /*Size */
   putword(0X020C); /*SetWindowExt x,y=bottom */
   putword(HEIGHT);   /*  right */
   putword(WIDTH);   /*  bottom */
   putdword(4L); /*Size */
   putword(0X0102); /*SetBkMode */
   putword(1);      /* 2=opaque; 1=transparant */
   putdword(5L); /*Size */
   putword(0X0201); /*SetBkColor */
   putdword(PAPERCOLOR); /* BkColor w1 0000=zwart 00ff=rood FF00=groen FFFF=geel*/
   putdword(4L); /*Size */
   putword(0X0104); /*SetROP2 */
   putword(13);   /* 9: rood+groen => zwart; default: 13 */
//   putdword(4L);
//   putword(0X012E); /* SetTextAlign */
//   putword(Alignment>100?Alignment-100:Alignment);
//   if (Pen==-1) Pen=CreatePen(0,0,0);
}

void wmfend(void) {
   DWORD filelen;
   putdword(16L);   /* Size*/
   putword(0X02FB); /* CreateFontIndirect */
   putword(16);     /* Height */
   putword(7);      /* Width*/
   putword(0);      /* Escapement =  rotation angle * 10 */
   putword(0);      /* Orientation: not used */
   putword(0);      /* Wheight */
   putword(0X0000); /* Italic,Underline */
   putword(0X0000); /* StrikeOut,CharSet */
   putword(0X0201); /* OutPrecision,ClipPrecision */
   putword(0X2202); /* Quality,PitchAndFamily */
   putword(0X7953); /* Sy*/
   putword(0X7473); /* st*/
   putword(0X6D65); /* em*/
   putword(0X6E00); /* .n" System.n"*/
   CreateObject(FONT);
   putdword(3L);
   putword(0);
   filelen=((ftell(fpwmf))>>1)-11;  /* size in WORDS */
   fseek(fpwmf,28,SEEK_SET);
   fwrite(&filelen,sizeof(DWORD),1,fpwmf);
   fwrite(&noofobjects,sizeof(WORD),1,fpwmf);
   fwrite(&maxrecordsize,sizeof(DWORD),1,fpwmf);
   fclose(fpwmf);
   cprintf("\r\n%s (size:%lu)\r\n",wmffile,(filelen+11)<<1);
}

WORD CreateObject(WORD obj) {
   WORD i, nr;
   for (i=0; i<=noofobjects && object[i]!=-1; i++)
     ;
   putdword(4L);
   putword(0X012D); /*SelectObject */
   putword(i);
   if (noofobjects<=i) noofobjects=i+1;
   if (noofobjects>OBJECTS) {
      cprintf("Too many objects (%u) defined\r\n",noofobjects+1);
      exit(-1);
   }
   for (nr=0; nr<OBJECTS && object[nr]!=obj; nr++)
        ;
   if (nr<OBJECTS) {
      putdword(4L);
      putword(0X01F0); /*DeleteObject */
      putword(nr);
      object[nr]=-1;
   }
   object[i]=obj;
   return i;
}

WORD pen(WORD style, WORD width, DWORD color) {
   putdword(9L)      ; /*Size */
   putword(0X02FA)      ; /*CreatePenIndirect */
   putword(style);
   putword(width);
   putword(0);
   putdword(color);
   PenColor=color;
   NewPenColor=0;
   putword(34); /*=> extra item */
   return CreateObject(PEN);
}

WORD brush(WORD style, WORD hatch, DWORD color) {
   putdword(7L)      ; /*Size */
   putword(0X02FC)   ; /*CreateBrushIndirect */
   putword(style);
   putdword(color);
   putword(hatch);
   BrushColor=color;
   BrushStyle=style;
   NewBrushColor=0;
   return CreateObject(BRUSH);
}

void wmfmove(WORD x,WORD y) {
   x*=WMFFACTOR;y*=WMFFACTOR;
   currentx=x;
   currenty=y;
}

void wmfdraw(WORD x,WORD y) {
//   if (Pen==-1 || color!=PenColor) Pen=pen(PenStyle,PenWidth,color);
   x*=WMFFACTOR;y*=WMFFACTOR;
   putdword(5L);
   putword(0X0214);
   putword(HEIGHT-currenty);
   putword(currentx);
   putdword(5L);
   putword(0X0213);
   putword(HEIGHT-y);
   putword(x);
   currentx=x;
   currenty=y;
}

void putbyte(BYTE b) {
   fwrite(&b,sizeof(BYTE),1,fpwmf);
}

void putword(WORD w) {
   fwrite(&w,sizeof(WORD),1,fpwmf);
}

void putdword(DWORD dw) {
   fwrite(&dw,sizeof(DWORD),1,fpwmf);
}

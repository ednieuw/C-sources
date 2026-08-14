#include "include\wmf.h"
#include "plab.h"
#include "include\stdio.h"
#include "include\stdlib.h"
#include "include\alloc.h"
#include "include\process.h"
#include "include\math.h"
#include "include\io.h"
#include "include\string.h"
#include "include\ctype.h"
#include "include\conio.h"
#include "include\fcntl.h"
#include "include\dos.h"
#include "include\dir.h"
#include "include\bios.h"
//#define BARWIDTH 185;
//Word CreatePen(Dword color);
//Word CreateBrush(Dword color);
//void wmfdraw(Word x,Word y, Dword color);

int  ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag, int format, int xflag);
Word brush(Word style, Word hatch, Dword color);
void center(int lines, float *cx, float *cy, float *cz, float *min, float *max);
void dotted_h_line(int x0, int x1, int y0);
void dotted_v_line(int x0, int y0, int y1);
void draw_line(int n, float xmin, double scale_xfactor, float ymin, double scale_yfactor, int wmf_flag);
//void draw_regression(int n);
void init_colors(void);
char *colormenu(char *s);
Word CreateFont(Word orientation, Word Font, Word HsizeWMF, Word VsizeWMF);
Word CreateObject(Word obj);
char getsymbol(int rangenumber);
Word pen(Word style, Word width, Dword color);
void pic0draw(int x,int y);
void pic0move(int x,int y);
int  picbottomline(int n);
void piccolor(int color);
int  piccopy(char *source, char *target);
void piccorner();
int  picdot(int x,int y, int type, int size, int dataline);
int  picdotdot(double step1,double step2,double xfactor,double yfactor,double *xpos,double *ypos,double xlim,double ylim);
void picdotted_line(int x0,int y0,int x,int y,int linetype,Dword rgbcolor, int wmf_flag);
void picdraw(int x,int y);
void draw_xy(int x_origin, float xmin, float xmax, double x_factor, int y_origin, float ymin, float ymax, double y_factor);
void picend(void);
void picextra_text(int x_origin, int y_origin, int lx, int ly);
void picfont(char font);
void picheaders(int headerlines);
void piclegend(int legends_per_line);
void picmove(int x,int y);
void picopen(char *s);
void picrectangle(int xpos,int ybase,int xpos2,int y, int fill);
void picsize(int x,int y);
void pictext(int direction,int position, char *s);
int  pictextlines(int xpos, int ypos, char *text, int direction, int position, int picflag);
int  pictopline(int n);
int  picx(float x);
int  picx_cal(int logflag, int picflag);
void picxy(int x,int y);
int  picy(float y,int range);
int  picy_cal(int logflag, int picflag);
int  picy2_cal(int logflag, int picflag);
int  picy_axis_title(char *y_axis_string, int y_pos, int picflag);
int  picy2_axis_title(char *y2_axis_string, int y2_pos, int picflag);
void plot_point(int column, int range_number, int bar);
void polyline(int r);
void putbyte(Byte b);
void putdword(Dword dw);
//void putword(Word w);
void wmftextcolor(Dword color);
void wmfdot(Word x, Word y, Word size, Word style, Dword color);
void wmfdraw(Word x,Word y, Word style, Word width, Dword color);
void wmfend(void);
void wmfline(Word x0,Word y0,Word x1,Word y1, Word style, Word width, Dword color);
void wmfmove(Word x,Word y);
void wmfopen(void);
void wmfpolylinestart(Word x, Word y, Word points, Word style, Word width, Dword color);
void wmfpolylinestep(Word x,Word y);
void wmfrectangle(Word x0, Word y0, Word x1, Word y1, Word brushstyle, Dword pencolor, Dword brushcolor);
void wmfsquare(Word x, Word y, Word size, Word style, Dword color);
void wmfdiamond(Word x, Word y, Word size, Word style, Dword color);
Word wmftext(Word direction, Word alignment, Dword color, char *s);
void wmftriangle(Word x, Word y, Word size, Word style, Dword color);
void write_wmf(void);

long rgb[28];

int Pen=-1,Brush=-1,Font=-1, Filltype=-1, VsizeWMF, HsizeWMF;
//int Wmftextsize=-1;
int FontStyle, BrushStyle, Bold, Italic, Underline, Charset;
Dword BrushColor, PenColor, TextColor;
Byte NewPenColor=1, NewBrushColor=1;
Word PenWidth=PENWIDTH, PenStyle=PENSTYLE, NoAsk;
int object[OBJECTS];
Word noofobjects, Orientation=0;
Word Alignment=WMFBOTTOMLEFT;

/* no vertical central alignment in wmf-definition! Only: Base, Bottom and Top
 0  6  2       1  7  3
 8 14 10       9 15 11
24 30 26      25 31 27
introduced: 125, 131 and 127 for vertical central alignment
*/
Word vertices,alignment,direction,position;
Word currentx, currenty;
Dword maxrecordsize, filelen;

char getsymbol(int rangenumber) {
   rangenumber %= strlen(symbolstring);
   return symbolstring[rangenumber];
}

int picdot(int x,int y, int type, int size, int dataline) {
   int ybase, dx,dy,r=size/2, t, xpos, xpos2, xw,yw,rw,barsize;
   Dword color;
   char buffer[2]=" ";
//   color=rgb[scolorstring[type%strlen(scolorstring)]-'A'];
   color=SYMBOLCOLOR(type);

   t=symbolstring[type%strlen(symbolstring)];
   xw=x*WFACTOR;
   yw=y*WFACTOR;
   rw=r*WFACTOR;
//   wmfdot(     1500,1000,1000,0,RED);
//   wmfsquare(  1500,1000,1000,2,ORANGE);
//   wmftriangle(1500,1000,1000,0,GREEN);
/*
   if (type>=' ' && type<127) {
      *buffer=type;
      picsize(2*size, size*18/8);
      picmove(x,y);
      pictext(0,CENTER,buffer);
      picsize(Hsize,Vsize);
      return -1;
   }
*/
   if (t=='D' || t=='R') return t;
   if (Literal) {
      *buffer=t;
      picsize(2*size, size*18/8);
      picmove(x,y);
      pictext(0,CENTER,buffer);
      picsize(Hsize,Vsize);
      return -1;
   }
   if (t=='L') return -1;
   if (toupper(t)=='B') {
      ybase=y_origin;
      if (ymin < 0 && (!logyflag || (logyflag && Y1line))) ybase-=ymin * scale_yfactor;
      if (dataline==-1) barsize=size;
      else if (Bar_size) barsize=Bar_size;
      else barsize=getbarsize(dataline);
      xpos=x-barsize;
      xpos2=x+barsize;
      picrectangle(xpos,ybase,xpos2,y,(t=='B'));
      return -1;
   }
   switch(t) {
      case '0':                     /* open circle */
   wmfdot(xw,yw,rw,0,WHITE);
         pic0move(x,y+r);
         for (dy=r; dy > -r; dy -= PIXELS_PER_LINE) {
            dx= sqrt((long)r*r-(long)dy*dy);
            pic0draw(x+dx,y+dy);
         }
         for ( ; dy <= r; dy+= PIXELS_PER_LINE) {
            dx= sqrt((long)r*r-(long)dy*dy);
            pic0draw(x-dx,y+dy);
         }
         break;
      case '1':                     /* filled circle */
   wmfdot(xw,yw,rw,0,color);
         for (dy=r; dy > -r; dy -= PIXELS_PER_LINE) {
            dx= sqrt((long)r*r-(long)dy*dy);
            pic0move(x-dx,y+dy);
            pic0draw(x+dx,y+dy);
         }
         break;
      case '2':                     /* open square */
   wmfsquare(xw,yw,rw,0,WHITE);
         pic0move(x-r,y+r);
         pic0draw(x+r,y+r);
         pic0draw(x+r,y-r);
         pic0draw(x-r,y-r);
         pic0draw(x-r,y+r);
         break;
      case '3':                     /* filled square */
   wmfsquare(xw,yw,rw,0,color);
         for (dy=r; dy > -r; dy -= PIXELS_PER_LINE) {
            pic0move(x-r,y+dy);
            pic0draw(x+r,y+dy);
         }
         break;
      case '4':                     /* open triangle */
   wmftriangle(xw,yw,rw,0,WHITE);
         pic0move(x  ,y+r);
         pic0draw(x-r,y-r);
         pic0draw(x+r,y-r);
         pic0draw(x  ,y+r);
         break;
      case '5':                     /* filled triangle */
   wmftriangle(xw,yw,rw,0,color);
         for (dx=0,dy=r; dx <= r ; dx+= PIXELS_PER_LINE/2, dy -= PIXELS_PER_LINE) {
            pic0move(x-dx,y+dy);
            pic0draw(x+dx,y+dy);
         }
         break;
      case '6':                     /* upside_down triangle, open */
   wmftriangle(xw,yw,rw,0,WHITE);
         pic0move(x-r,y+r);
         pic0draw(x+r,y+r);
         pic0draw(x  ,y-r);
         pic0draw(x-r,y+r);
         break;
      case '7':                     /* upside_down triangle, filled */
   wmftriangle(xw,yw,rw,2,color);
         for (dx=0,dy=-r; dx <= r ; dx+= PIXELS_PER_LINE/2, dy+= PIXELS_PER_LINE) {
            pic0move(x-dx,y+dy);
            pic0draw(x+dx,y+dy);
         }
         break;
      case '8':                     /* star */
   wmftriangle(xw,yw,rw,4,color);
         pic0move(x, y-r);
         pic0draw(x, y+r);
         pic0move(x-r, y);
         pic0draw(x+r, y);
         if (columns<100 && Symbol_size >24) {
            pic0move(x-r,y-r);
            pic0draw(x+r,y+r);
            pic0move(x+r,y-r);
            pic0draw(x-r,y+r);
         }
         break;
      case '9':                     /* open diamond */
         wmfdiamond(xw,yw,rw,0,color);
         pic0move(x,  y+r);
         pic0draw(x-r,y);
         pic0draw(x,  y-r);
         pic0draw(x+r,y);
         pic0draw(x,  y+r);
         break;
     default:                       /* ASCII-karakter */
        buffer[1]='\0';
        buffer[0]=t;
        picsize(15*r, 12*r);
        picmove(x,y);
        pictext(0, CENTER, buffer);
        picsize(Hsize, Vsize);
      }
      return -1;
}

void picrectangle(int xpos,int ybase,int xpos2,int y, int fill) {
   wmfrectangle(WFACTOR*xpos, WFACTOR*ybase, WFACTOR*xpos2, WFACTOR*y, 1, 0,0);
   if (fill) {
         for (; xpos<=xpos2; xpos++) {
            pic0move(xpos, ybase);
            pic0draw(xpos, y);
         }
   } else {
         pic0move(xpos,  ybase);
         pic0draw(xpos,  y);
         pic0draw(xpos2, y);
         pic0draw(xpos2, ybase);
         pic0draw(xpos,  ybase);
   }
}

void picdotted_line(int x0, int y0, int x, int y, int linetype, Dword color, int wmf_flag) {
#define SIZE1 8.0
#define SIZE2 32.0
#define SIZE3 40.0
#define SIZE4 64.0
   int t, i, ddx, ddy;
   double dx, dy, size, xpos, ypos, xfactor, yfactor;
   t=linetypestring[linetype%strlen(linetypestring)]-'0';
   if (t>9) t-='A'-'0';
   dx=(double)(x-x0);
   dy=(double)(y-y0);
   size=sqrt(dx*dx+dy*dy);
   if (!size) return;
   xfactor=dx/size;
   yfactor=dy/size;
   xpos=(double)x0;
   ypos=(double)y0;
   if (wmf_flag) wmfline(x0*WFACTOR,y0*WFACTOR,x*WFACTOR,y*WFACTOR, t, 1, color);
   switch (t) {
      case 1: /* ________ */
         while (picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y));
         break;
      case 2: /* ........ */
         while (picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y));
      case 3: /* _._._._. */
         do {
            i=       picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i=picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
         break;
      case 4: /*.. _..._. */
         do {
            i=        picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
         break;
      case 5: /* ======= */
         ddx=(int)(6.0*yfactor);
         ddy=(int)(6.0*xfactor);
         pic0move(x0+ddx ,y0-ddy);
         pic0draw(x +ddx, y -ddy);
         pic0move(x0-ddx, y0+ddy);
         pic0draw(x -ddx, y +ddy);
         break;
      case 6: /* = = = = = = */
         xpos=x0+6.0*yfactor;
         ypos=y0-6.0*xfactor;
         while (picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x+6.0*yfactor, (double) y-6.0*xfactor));
         xpos=x0-6.0*yfactor;
         ypos=y0+6.0*xfactor;
         while (picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x-6.0*yfactor, (double) y+6.0*xfactor));
         break;
      case 7: /* :::::: */
         xpos=x0+6.0*yfactor;
         ypos=y0-6.0*xfactor;
         while (picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x+6.0*yfactor, (double) y-6.0*xfactor));
         xpos=x0-6.0*yfactor;
         ypos=y0+6.0*xfactor;
         while (picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x-6.0*yfactor, (double) y+6.0*xfactor));
         break;
      case 8: /* =:::=::: */
         xpos=x0+6.0*yfactor;
         ypos=y0-6.0*xfactor;
         do {
            i=        picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
         xpos=x0-6.0*yfactor;
         ypos=y0+6.0*xfactor;
         do {
            i=        picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
         break;
      case 9:
         break;
      case 10: /* __..__.. */
         do {
                   i= picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
         break;
      case 0:
      default:
         pic0move(x0,y0);
         pic0draw(x,y);
         break;
   }
}
 
int picdotdot(
      double step1,
      double step2,
      double xfactor,
      double yfactor,
      double *xpos,
      double *ypos,
      double xlim,
      double ylim) {
   int hor, vert;
   double dx, dy;
   hor=(*ypos==ylim);
   vert=(*xpos==xlim);
   if (hor && vert) return 0;
   pic0move((int)*xpos, (int)*ypos);
   dx=xfactor * step1;
   dy=yfactor * step1;
  *xpos=(
      (dx>0 && *xpos+dx>xlim) ||
      (dx<0 && *xpos+dx<xlim)) ?
         xlim:
         *xpos+dx;
   *ypos=(
      (dy>0 && *ypos+dy>ylim) ||
      (dy<0 && *ypos+dy<ylim)) ?
         ylim:
         *ypos+dy;
   pic0draw((int)*xpos, (int)*ypos);
   if ((!vert && *xpos==xlim) || (!hor && *ypos==ylim)) return 0;
   dx=xfactor * step2;
   dy=yfactor * step2;
   *xpos=(
      (dx>0 && *xpos+dx>xlim) ||
      (dx<0 && *xpos+dx<xlim)) ?
         xlim:
         *xpos+dx;
   *ypos=(
      (dy>0 && *ypos+dy>ylim) ||
      (dy<0 && *ypos+dy<ylim)) ?
         ylim:
         *ypos+dy;
   if ((!vert && *xpos==xlim) || (!hor && *ypos==ylim)) return 0;
   return 1;
}

int picbottomline(int n) {
   return(Y_start+n*Vsize);
}

int pictopline(int n) {
    return PICTOP-n*Vsize;
}

void plot_point(int column, int range_number, int bar) {
   int xxx2,yyy1,yyy2,yyy3,d,orientation,yas2;
   float min,yfactor,dy;
   yas2=(range_type[range_number]);
   yfactor=(yas2)?scale_y2factor:scale_yfactor;
   min=(yas2)?y2min:ymin;
   xxx2=(xx2!=MISSING)? x_origin + ( xx2 - xmin ) * scale_xfactor:MISSING;
   yyy1=(yy1!=MISSING)? y_origin + ( yy1 - min ) * yfactor:MISSING;
   yyy2=(yy2!=MISSING)? y_origin + ( yy2 - min ) * yfactor:MISSING;
   yyy3=(yy3!=MISSING)? y_origin + ( yy3 - min ) * yfactor:MISSING;
   if (log0 && !x[column] && !bar) xxx2=x_origin-log0;
   if (xxx2!=MISSING && yyy2!=MISSING) {
      if (!dual_axis) {
         if (xllflag && xxx2<xll) xxx2=xll-Symbol_size-Extra_rim/4;
         if (xulflag && xxx2>xul) xxx2=xul+Symbol_size;
         if (yllflag && yyy2<yll) yyy2=yll-Symbol_size;
         if (yulflag && yyy2>yul) yyy2=yul+Symbol_size;
      }
      orientation=picdot(xxx2 , yyy2, range_number, Symbol_size, column);
      if (orientation>-1) {
         picmove(xxx2,yyy2);
         if (orientation=='D') pictext(0,CENTER,name[column]);
         else pictext(1,CENTER,name[column]);
      }
      dy=(toupper(symbolstring[range_number])=='B')?0:Symbol_size/2;
      if (yyy1!=MISSING && (d=yyy2-yyy1)>dy && d>0 && !yllflag && !yulflag) {
         picmove(xxx2, yyy1);
         picdraw(xxx2, yyy2-dy);
         picmove(xxx2-Cap/2, yyy1);
         picdraw(xxx2+Cap/2, yyy1);
      }
      if (yyy3!=MISSING && (d=yyy3-yyy2)>dy && d>0 && !yllflag && !yulflag) {
         picmove(xxx2, yyy2+dy);
         picdraw(xxx2, yyy3);
         picmove(xxx2-Cap/2, yyy3);
         picdraw(xxx2+Cap/2, yyy3);
      }
   }
}


void draw_line(int n, float xmin, double scale_xfactor, float ymin, double scale_yfactor, int wmf_flag) {
   int column, xx, yy, xxx0, yyy0, xxx, yyy, skip=FALSE, symbol_size;
   Dword color;
   double a, dx, dy, distance;
   float min,yfactor;
   int yas2,logflag;
   column=range_start[n];
   symbol_size=((getsymbol(n)=='9')|| (getsymbol(n)=='L'))?0:Symbol_size;
   yas2=range_type[n];
   logflag=(yas2)?logy2flag:logyflag;
   min=(yas2)?y2min:ymin;
   yfactor=(yas2)?scale_y2factor:scale_yfactor;
   color=LINECOLOR(n);
   xx2=x[column];
   yy2=y[2][column];
   if (xx2!=MISSING && yy2!=MISSING) {
      if (logxflag) xx2=(xx2>0)?log10(xx2):0;
      if (logflag) yy2=(yy2>0)?log10(yy2):MISSING;
   }
   if (xx2!=MISSING && yy2!=MISSING) {
      if (log0 && x[column]==0) {
         xx= x_origin-log0+3*Extra_rim/4;
         yy= y_origin + (yy2-min) * yfactor;
         if (yllflag && yy<yll) yy=yll-symbol_size;
         if (yulflag && yy>yul) yy=yul+symbol_size;
         for (column++; column<range_start[n]+range_size[n] && (x[column]==MISSING || datapoint_code[column]=='*'); column++)
            ;
         if (column<range_start[n]+range_size[n] ) {
            xx2=x[column];
            yy2=y[2][column];
            if (xx2!=MISSING && yy2!=MISSING) {
               if (logxflag) xx2=(xx2>0)?log10(xx2):MISSING;
               if (logflag) yy2=(yy2>0)?log10(yy2):MISSING;
            }
            xxx=x_origin + (xx2-xmin) * scale_xfactor;
            yyy=y_origin + (yy2-min) * yfactor;
            if (!dual_axis) {
               if (xllflag && xxx<xll) xxx=xll-symbol_size;
               if (xulflag && xxx>xul) xxx=xul+symbol_size;
               if (yllflag && yyy<yll) yyy=yll-symbol_size;
               if (yulflag && yyy>yul) yyy=yul+symbol_size;
            }
            a=(xxx-xx>0)?(double)(yyy-yy)/(xxx-xx):0;
            if (a) {
               picdotted_line(xx, yy, x_origin-log0/2, yy+a*log0/2, n, color, wmf_flag);
               xx=x_origin;
               yy=yy+a*log0;
            } else {
               picdotted_line(xx, yy, x_origin-log0/2, yy, n, color, wmf_flag);
               xx=x_origin;
            }
            wmfmove(xx,yy);
            if (xxx-x_origin>SYMBOL_SIZE/2) {
               picdotted_line(xx, yy, xxx, yyy, n, color, wmf_flag);
               xx=xxx;
               yy=yyy;
            }
            column++;
         }
      } else {
         xx2=x[column];
         yy2=y[2][column];
         if (xx2!=MISSING && yy2!=MISSING){
            if (logxflag) xx2=(xx2>0)?log10(xx2):MISSING;
            if (logflag) yy2=(yy2>0)?log10(yy2):MISSING;
         }
         xx= x_origin + (xx2-xmin) * scale_xfactor;
         yy= y_origin + (yy2-min) * yfactor;
         if (xllflag && xx<xll) xx=xll;
         if (xulflag && xx>xul) xx=xul;
         if (yllflag && yy<yll) yy=yll;
         if (yulflag && yy>yul) yy=yul;
         column++;
      }
      for ( ; column<range_start[n]+range_size[n]; column++) {
         xx2=x[column];
         yy2=y[2][column];
         if (xx2!=MISSING && yy2!=MISSING) {
            if (logxflag) xx2=(xx2>0)?log10(xx2):MISSING;
            if (logflag) yy2=(yy2>0)?log10(yy2):MISSING;
         }
         xxx= x_origin + (xx2-xmin) * scale_xfactor;
         yyy= y_origin + (yy2-min) * yfactor;
         if (!dual_axis) {
         if (xllflag && xxx<xll) xxx=xll;
         if (xulflag && xxx>xul) xxx=xul;
         if (yllflag && yyy<yll) yyy=yll;
         if (yulflag && yyy>yul) yyy=yul;
         }
         dy=(double)(yyy-yy);
         dx=(double)(xxx-xx);
         distance=sqrt(dy*dy+dx*dx);
         if (distance>symbol_size) {
            xxx0=xxx;
            yyy0=yyy;
            xx   +=symbol_size*dx/distance/2;
            xxx -=symbol_size*dx/distance/2;
            yy   +=symbol_size*dy/distance/2;
            yyy -=symbol_size*dy/distance/2;
            if (!dual_axis) {
            if (xllflag && xx<xll) xx-=Extra_rim/4;
            if (xllflag && xxx<xll) xx-=Extra_rim/4;
            }
            if (!skip) picdotted_line(xx, yy, xxx, yyy, n, color, wmf_flag);
            if (linetypestring[n%strlen(linetypestring)]>'9') skip=(skip)?0:1;
            xxx=xxx0;
            yyy=yyy0;
         }
         xx=xxx;
         yy=yyy;
      }
   }
}

void picopen(char *s) {
int n;
unsigned char c;
   openout(s);
   for (n=0;n<17;n++){
      c=header_vector[n];
      fputc(c,fpout);
   }
   picsize(Hsize, Vsize);
   picfont(1);
   piccolor(0);
}

void piccorner() {
   picmove(X_ORIGIN,Y_ORIGIN+X_LENGTH);
   picdraw(X_ORIGIN,Y_ORIGIN);
   picdraw(X_ORIGIN+X_LENGTH,Y_ORIGIN);
   picmove(X_LENGTH,PICTOP-Y_LENGTH);
   picdraw(X_LENGTH,PICTOP);
   picdraw(X_LENGTH-Y_LENGTH,PICTOP);
}

int picx(float x) {   /* transforms x-value from datafile into picunits */
   int xx;
   if (logxflag) {
      if (x<=0) xx=(log0)?x_origin:x_origin-Symbol_size;
      else xx=x_origin+scale_xfactor*(log10(x)-xmin);
   }
   else xx=x_origin+scale_xfactor*(x-xmin);
   if (xllflag && xx<xll) xx=xll-Symbol_size;
   if (xulflag && xx>xul) xx=xul+Symbol_size;
   return (int)(WFACTOR*(float)xx);
}

int picy(float y, int range) {   /* transforms y-value from datafile into picunits */
   int yy=0;
   if (range_type[range]==0) {
      if (logyflag) {
         if (y<=0) yy=y_origin-Symbol_size;
         else yy=y_origin+scale_yfactor*(log10(y)-ymin);
      }
      else yy=y_origin+scale_yfactor*(y-ymin);
   } else {
      if (logy2flag) {
         if (y<=0) yy=y_origin-Symbol_size;
         else yy=y_origin+scale_y2factor*(log10(y)-y2min);
      }
      else yy=y_origin+scale_y2factor*(y-y2min);
   }
   if (!dual_axis) {
     if (yllflag && yy<yll) yy=yll-Symbol_size;
     if (yulflag && yy>yul) yy=yul+Symbol_size;
   }
   return (int)(WFACTOR*(float)yy);
}

void picxy(int x,int y) {
   fputc(x/256,fpout);
   fputc(x%256,fpout);
   fputc(y/256,fpout);
   fputc(y%256,fpout);
}

void picmove(int x,int y) {
   fputc(MOVE,fpout);
   picxy(x,y);
   wmfmove(x*WFACTOR,y*WFACTOR);
}

void pic0move(int x,int y) {
   fputc(MOVE,fpout);
   picxy(x,y);
}

void picfont(char font) {
   fputc(PICFONT,fpout);
   fputc(font,fpout);
}

void piccolor(int color) {
   fputc(COLOR0+color%6,fpout);
}

void picsize(int x,int y) {
   fputc(SIZE,fpout);
   picxy(x,y);
}

void picdraw(int x,int y) {
   fputc(DRAW,fpout);
   picxy(x,y);
   wmfdraw(x*WFACTOR,y*WFACTOR,0,1,(Color)?BLACK:0);
}

void pic0draw(int x,int y) {
   fputc(DRAW,fpout);
   picxy(x,y);
}

void pictext(int direction,int position, char *s) {
   char *s0;
   int alignment;
   s0=(s[0]=='!')?s+1:s;
   switch (position) {
      case CENTER:                         /* 0 */
         alignment=WMFCENTER;
         break;
      case CENTERLEFT:
         alignment=WMFCENTERLEFT;
         break;
      case CENTERTOP:
         alignment=WMFCENTERTOP;
         break;
      case CENTERRIGHT :
         alignment=WMFCENTERRIGHT;
         break;
      case CENTERBOTTOM:
         alignment=WMFCENTERBOTTOM;
         break;
      case TOPLEFT:
         alignment=WMFTOPLEFT;
         break;
      case TOPRIGHT:
         alignment=WMFTOPRIGHT;
          break;
      case BOTTOMLEFT:
         alignment=WMFBOTTOMLEFT;
         break;
      case BOTTOMRIGHT:
         alignment=WMFBOTTOMRIGHT;
   }
   wmftext(900*direction, alignment, BLACK, s0);
   fputc(TEXT,fpout);
   fputc((unsigned char)direction*16+position,fpout);
   fputs(s0,fpout);
   fputc('\0',fpout);
}

void picend(void) {
   fputc(END,fpout);
   fclose(fpin);
   if (fclose(fpout)==EOF) exit(-1);
}

void picheaders(int headerlines) {
   int h;
   if (*first_title) {  
      picmove(x_origin + (x_length -log0)/2,pictopline(0));
      pictext(0,CENTERTOP,first_title);
   }
   if (*second_title) {
      picmove(x_origin + (x_length -log0)/2,pictopline(1));
      pictext(0,CENTERTOP,second_title);
   }
   for (h=2;h<headerlines;h++) {
      picmove(x_origin + (x_length -log0)/2,pictopline(h));
      pictext(0,CENTERTOP,headerline[h]);
   }
}

int pictextlines(int xpos, int ypos, char *text, int direction, int position, int picflag) {
   int line, lines, xpos0, ypos0;
   char *textpointer;
   xpos0=xpos;
   ypos0=ypos;
   strncpy(buffer,text,MAX_LEN);
   lines=count_char(buffer,'\\')+1;
   textpointer=strtok(buffer,"\\");
   for (line=0; line<lines; line++) {
        if (picflag) {
           picmove(xpos, ypos);
           if (line==0) pictext(direction,position,buffer);
           else if (textpointer) pictext(direction,position,textpointer);
        }
        textpointer=strtok(NULL,"\\");
/*
        if (direction==0) ypos-= VsizeWMF;
        if (direction==1) xpos+= VsizeWMF;
        if (direction==2) ypos+= VsizeWMF;
        if (direction==3) xpos-= VsizeWMF;
*/
        if (direction==0) ypos-= Vsize;
        if (direction==1) xpos+= Vsize;
        if (direction==2) ypos+= Vsize;
        if (direction==3) xpos-= Vsize;
  }
  return (direction%2)?abs(xpos0-xpos):abs(ypos0-ypos);
}

void piclegend(int legends_per_line) {
   int i, x, xx, xpos, xpos1, xpos2, y, dy, lines, legendlines, size=LEGEND_SYMBOL_SIZE;
   Dword color;
   char text[80], *textfragment;
   if (!legends_per_line) return;
   dy=0.1*Vsize;
   if (Legendpos==0) {
      lines=ceil((double)ranges/legends_per_line);
      for (i=0; i<ranges; i++) {
//       if (Color) piccolor(i);
         color=LINECOLOR(i);
         if (color!=PenColor) Pen=pen(PenStyle,PenWidth,color);
         if ( ranges<2 ) x= x_origin-log0;
         else x= x_origin -log0 + (x_length*((i/lines)%legends_per_line))/legends_per_line;
         y= picbottomline(lines-(i%lines)-1) + Vsize/2;
         if (toupper(symbolstring[i])=='B') {
            xpos1=(Line || Statflag)?x+(XSIZE+size)/2:x;
            xpos2=xpos1+size/2;
            picmove(xpos1-size, y-VSIZE/3);
            picdraw(xpos1+size, y-VSIZE/3);
            if (symbolstring[i]=='B') {
               for (xx=xpos1-size/2; xx<=xpos2; xx++) {
                  picmove(xx, y-VSIZE/3);
                  picdraw(xx, y+VSIZE/4);
               }
            } else {
               xpos1-=size/2;
               picmove(xpos1, y-VSIZE/3);
               picdraw(xpos1, y+VSIZE/4);
               picdraw(xpos2, y+VSIZE/4);
               picdraw(xpos2, y-VSIZE/3);
            }
         } else picdot (x, y - dy, i, size,-1);
         if (toupper(symbolstring[i])!='B') {

            if (Line || Statflag) {
               if (toupper(symbolstring[i])!='L') {
                  picdotted_line(x+size/2, y-dy,x+size/2+XSIZE , y - dy, i , color, 1);
                  picdot (x+size+XSIZE, y - dy , i, size,-1);
               } else {
                  picdotted_line(x, y - dy, x+size+XSIZE, y - dy, i, color, 1);
               }
            }
/*
            else 
            if (toupper(symbolstring[i])=='L') {
               picdotted_line(x-size/2, y-dy, x+size/2, y-dy, i, color, 1);
            }
*/
         }
         if (Line || Statflag) picmove(x+2*size+XSIZE+DX, y);
         else picmove(x+size+DX, y);
         pictext(0, CENTERLEFT, range_name[i]);
      }
   } else {
      legendlines=get_legendlines();
      y= y_origin+y_length - 0.4*Vsize-(Xcal/3)*RIM;
      if (Legendpos==1) {
      x= x_origin + x_length + ((Ycal==0)?Rim:0) + 50;
//        x=X_length-legendblocksize-2*Rim+50;
      }
      if (Legendpos==2) x= x_origin + x_length - 3*Rim - legendblocksize;
      if (Legendpos==3) x= x_origin+(Ycal/3)*Rim;
/*
      if (Legendpos==2) x= x_origin + x_length - ((Ycal==3)?Rim:0) - legendblocksize;
      if (Legendpos==3) x= x_origin+(Ycal/3==0)?Rim:0 ;
      if (Box)
          picmove(x, y);
          picdraw(x,y2);
          picdraw(x+legendblocksize, y2);
          picdraw(x+legendblocksize, y);
          picdraw(x, y);
      }
*/
      if (Box) {
          picmove(x, y + 0.6*Vsize);
          picdraw(x, y - legendlines*Vsize+0.4*Vsize);
          picdraw(x+legendblocksize, y - legendlines*Vsize+0.4*Vsize);
          picdraw(x+legendblocksize, y + 0.6*Vsize);
          picdraw(x, y + 0.6*Vsize);
      }
      xpos=x+DX+size/2;
      for (i=0; i<ranges; i++) {
         x=xpos;
//       if (Color) piccolor(i);
         color=LINECOLOR(i);
         if (color!=PenColor) Pen=pen(PenStyle,PenWidth,color);
         if (toupper(symbolstring[i])=='B') {
            xpos1=(Line || Statflag)?x+(XSIZE+size)/2:x;
            xpos2=xpos1+size/2;
            picmove(xpos1-size, y-VSIZE/3);
            picdraw(xpos1+size, y-VSIZE/3);
            if (symbolstring[i]=='B') {
               for (xx=xpos1-size/2; xx<=xpos2; xx++) {
                  picmove(xx, y-VSIZE/3);
                  picdraw(xx, y+VSIZE/4);
               }
            } else {
               xpos1-=size/2;
               picmove(xpos1, y-VSIZE/3);
               picdraw(xpos1, y+VSIZE/4);
               picdraw(xpos2, y+VSIZE/4);
               picdraw(xpos2, y-VSIZE/3);
            }
         } else picdot (x, y - dy, i, size, -1);
/*
         if (symbolstring[i]=='B') {
            xpos1=(Line || Statflag)?x+(XSIZE+size)/2:x;
            picmove(xpos1-size, y-VSIZE/3);
            picdraw(xpos1+size, y-VSIZE/3);
            for (xx=xpos1-size/2; xx<=xpos1+size/2; xx++) {
               picmove(xx, y-VSIZE/3);
               picdraw(xx, y+VSIZE/4);
            }
         } else picdot (x, y - dy, i, size,-1);
*/
         if (toupper(symbolstring[i])!='B') {
            if (Line || Statflag) {
               if (symbolstring[i]!='L') {
                  picdotted_line(x+size/2,y - dy,x+size/2+XSIZE , y - dy, i, color, 1);
                  picdot (x+size+XSIZE, y - dy , i, size,-1);
               } else {
                  picdotted_line(x,        y - dy, x+size+XSIZE , y - dy, i, color, 1);
               }
            } else if (symbolstring[i]=='L') {
               picdotted_line(x-size/2,y-dy, x+size/2, y-dy, i, color, 1);
            }
         }
         if (Line || Statflag) x+=2*size+XSIZE+DX;
         else x+=size+DX;
         strncpy(text,range_name[i],79);
         textfragment=strtok(text,"\\");
         do {
            picmove(x, y);
            if (textfragment) pictext(0, CENTERLEFT, textfragment);
            textfragment=strtok(NULL,"\\");
            y -= YSIZE;
         }  while (textfragment);
      }
   }
}

void dotted_h_line(int x0, int x1, int y0) {
   wmfline(x0,y0,x1,y0,1,1,0);
   for (;x0+10<=x1;x0+=40) {
      pic0move(x0,y0);
      pic0draw(x0+10,y0);
   }
}
void dotted_v_line(int x0, int y0, int y1) {
   wmfline(x0,y0,x0,y1,1,1,0);
   for (;y0+10<=y1;y0+=40) {
      pic0move(x0,y0);
      pic0draw(x0,y0+10);
   }
}

void init_colors(void) {
   strcpy(lcolorstring,"RB0GMCYPO");
   strcpy(scolorstring,"RB0GMCYPO");
   strcpy(l0colorstring,"RB0GMCYPO");
   strcpy(s0colorstring,"RB0GMCYPO");
   rgb[0]=PICCOLOR;
   rgb[1]=RGB1;
   rgb[2]=RGB2;
   rgb[3]=RGB3;
   rgb[4]=RGB4;
   rgb[5]=RGB5;
   rgb[6]=RGB6;
   rgb[7]=RGB7;
   rgb[8]=RGB8;
   rgb[9]=RGB9;
   rgb[10]=RGB10;
   rgb[11]=RGB11;
   rgb[12]=RGB12;
   rgb[13]=RGB13;
   rgb[14]=RGB14;
   rgb[15]=RGB15;
   rgb[16]=RGB16;
   rgb[17]=RGB17;
   rgb[18]=RGB18;
   rgb[19]=RGB19;
   rgb[20]=RGB20;
   rgb[21]=RGB21;
   rgb[22]=RGB22;
   rgb[23]=RGB23;
   rgb[24]=RGB24;
   rgb[25]=PAPERCOLOR;
   rgb[26]=BLACK;
   rgb[27]=WHITE;
}

char *colormenu(char *s) {
   int n,i;
   Dword color;
   char c;
   if (!*s) do {
      clrscr();
      cprintf("Current palette Blue/Green/Red:\r\n");
      cprintf("Special colors:\r\n",
               "0 or %c = Black\n\r"
               "1 or %c = White\r\n"
               "A = Graph background (invisible)\n\r"
               "Z = Papercolor\r\n\r\n",'A'+26, 'A'+27);
      for (n=0; n<4; n++) {
         for (i=0; i<7; i++) cprintf("%c = %06lX ",'A'+n*7+i, rgb[n*7+i]);
         cprintf("\r\n");
      }
      cprintf("RETURN to accept palette, A-%c to change\r\n",'A'+27);
      c=toupper(getch());
      if (c==RETURN) break;
      if (c=='0') c='A'+26;
      if (c=='1') c='A'+27;
      if (c>='A' && c<='A'+27) {
            cprintf("Enter value for color %c, RETURN to accept current value %06lX \r\n",c, rgb['A'-c]);
            gets(buffer);
            if (strlen(buffer)) n=sscanf(buffer,"%06X",&color);
            rgb[c-'A']=color;
//            if (n<0) c==RETURN;
cprintf("\r\n%s %4d : color =%06lX c-'A'=%d rgb[c-'A']=%06lX n=%d",
__FILE__, __LINE__,color,c-'A', rgb[c-'A'],n);if (getch()==27) exit(-1);putch('\r');putch('\n');
      }
   } while (!(c==RETURN || c==27 || c==3));
   if (!s) return 0;
   clrscr();
     cprintf("\n\r\n\rColor sequence (A-Z,0,1) used: \"%s\" \n\r",s);
               cprintf("0 = Black %06lX\n\r",rgb[26]);
               cprintf("1 = White %06lX\r\n",rgb[27]);
               cprintf("A = Graph background (invisible) %06lX\n\r",rgb[0]);
               cprintf("Z = Papercolor %06lX\r\n",rgb[25]);
               cprintf("R = Red %06lX\n\r",rgb[17]);
               cprintf("G = Green %06lX\n\r",rgb[6]);
               cprintf("B = Blue %06lX\n\r",rgb[1]);
               cprintf("M = Magenta %06lX\n\r",rgb[12]);
               cprintf("C = Cyan %06lX\n\r",rgb[2]);
               cprintf("Y = Yellow %06lX\r\n",rgb[24]);
               cprintf("P = Purple %06lX\r\n",rgb[15]);
               cprintf("O = Orange %06lX\r\n",rgb[14]);
            strcpy(s, keycopy(buffer, s, 39));
            for (n=0; n<strlen(s); n++) {
               s[n]=toupper(s[n]);
               if (s[n]=='0') s[n]='A'+26;
               if (s[n]=='1') s[n]='A'+27;
               if (s[n]<'A' || s[n]>'A'+27) s[n]='A';
            }
            return s;
}

void wmfopen(void) {
   #define PIXELS_PER_INCH 6000
   int i;
   Word checksum;
//   BrushColor=BRUSHCOLOR;
   BrushColor=(Color)?BRUSHCOLOR:0;
//   PenColor=PENCOLOR;
   PenColor=(Color)?PENCOLOR:0;
//   TextColor=TEXTCOLOR;
   TextColor=(Color)?TEXTCOLOR:0;
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
   putword((Word)WIDTH);  /*  bottom */
   checksum^=(Word)WIDTH;
   putword((Word)HEIGHT); /*  right */
   checksum^=(Word)HEIGHT;

   putword(PIXELS_PER_INCH);
   checksum^=PIXELS_PER_INCH;
//   putword(576);    /*  pixels per inch */
//   checksum^=576;

//   putword(2540);    /*  pixels per inch */
//   checksum^=2540;
//   putword(5080);    /*  pixels per inch */
//   checksum^=5080;
//   putword(10160);    /*  pixels per inch */
//   checksum^=10160;
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
   putword((Word)HEIGHT);   /*  right */
   putword((Word)WIDTH);   /*  bottom */
   putdword(4L); /*Size */
   putword(0X0102); /*SetBkMode */
   if (Color) putword(2);      /* 2=opaque; 1=transparant */
   else putword(1);      /* 2=opaque; 1=transparant */
   putdword(5L); /*Size */
   putword(0X0201); /*SetBkColor */
   if (Color) putdword(rgb[25]); /* BkColor w1 0000=zwart 00ff=rood FF00=groen FFFF=geel; BkColor w2 00FF=blauw*/
//   else putdword(rgb[1]);
   else putdword(rgb[26]);
   putdword(4L); /*Size */
   putword(0X0104); /*SetROP2 */
// putword(9);   /* rood+groen => zwart; default: 13 */
//   putword(13);
    putword(13);


// if (Color) wmfrectangle(0,0,WIDTH,HEIGHT,0, rgb[25],rgb[25]);

//   putdword(4L);
//   putword(0X012E); /* SetTextAlign */
//   putword(Alignment>100?Alignment-100:Alignment);
//   if (Pen==-1) Pen=CreatePen(0,0,0);
     wmftextcolor(TextColor);
     Font=CreateFont(0,Font,HsizeWMF,VsizeWMF);
     Brush=brush(0,0,BRUSHCOLOR);
}

Word CreateFont(Word orientation, Word Font, Word HsizeWMF, Word VsizeWMF) {
   Orientation=orientation;
   putdword(16L);          /* Size*/
   putword(0X02FB);        /* CreateFontIndirect */
   putword(-VsizeWMF);     /* Height; negative=automatic horizontal scaling */
   putword(HsizeWMF);      /* Width; automatic with negative Height*/
   putword(Orientation);   /* Escapement =  rotation angle * 10 */
   putword(0);             /* orientation: not used */
   putword(Bold?700:0);    /* Wheight */
   putbyte(Italic?1:0);
   putbyte(Underline?1:0);
// putword(0X0000);        /* Italic,Underline */
   putbyte(0);             /* Strikeout */
   putbyte(Charset);
// putword(0X0000);        /* StrikeOut,CharSet */
   putword(0X0000);        /* OutPrecision,ClipPrecision */
   putword(0X1000);        /* Quality,PitchAndFamily */
   putword(0X7241);        /* Ar */
   putword(0X6169);        /* ia */
   putword(0X006C);        /* l. */
   putword(0X02D1);        /* Ñ."Arial.Ñ." */
   Font=CreateObject(FONT);
   return Font;
}

void wmfend(void) {
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
}

Word CreateObject(Word obj) {
   Word i, nr;
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

Word pen(Word style, Word width, Dword color) {
   putdword(9L)      ; /*Size */
   putword(0X02FA)   ; /*CreatePenIndirect */
   putword(style);
   putword(width);
   putword(0);
   putdword(color);
   PenColor=color;
   NewPenColor=0;
   putword(34); /*=> extra item */
   return CreateObject(PEN);
}

Word brush(Word style, Word hatch, Dword color) {
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

void wmftextcolor(Dword color) {
   putdword(5L);
   putword(0X0209);
   putdword((Color)?color:0);
   TextColor=(Color)?color:0;
}

void wmfmove(Word x,Word y) {
   currentx=x;
   currenty=y;
}

void wmfdraw(Word x,Word y, Word style, Word width, Dword color) {
   if (Pen==-1 || color!=PenColor || style!=PenStyle || width!=PenWidth) Pen=pen(PenStyle,PenWidth,color);
   putdword(5L);
   putword(0X0214);
   putword((Word)(HEIGHT-currenty));
   putword(currentx);
   putdword(5L);
   putword(0X0213);
   putword((Word)(HEIGHT-y));
   putword(x);
   currentx=x;
   currenty=y;
}

void wmfline(Word x0,Word y0,Word x1,Word y1, Word style, Word width, Dword color) {
   if (Pen==-1 || color!=PenColor || style!=PenStyle || width!=PenWidth) Pen=pen(style,width,color);
   putdword(5L);
   putword(0X0214);
   putword(HEIGHT-y0);
   putword(x0);
   putdword(5L);
   putword(0X0213);
   putword(HEIGHT-y1);
   putword(x1);
   currentx=x1;
   currenty=y1;
}

void polyline(int r) {
   int i=0,column;
   Word style;
   Dword color;
   style=LINESTYLE(r)-'0';
   color=LINECOLOR(r);
// int skip=0;
   column=range_start[r];
   if (log0 && x[column]==0) column++;
//   color=rgb[lcolorstring[r%strlen(lcolorstring)]-'A'];
//   for (i=0; i<range_size[r]; i++) if (SYMBOLSTYLE(i)<='9') break;
   if (i<range_size[r]-1 || (log0 && x[column-1]==0)) {
      wmfpolylinestart(picx(x[column]),picy(y[2][column],r),range_size[r]-(log0 && x[column-1]==0),style,1,color);
      for (i=1; i<range_size[r]-(log0 && x[column-1]==0); i++) {
         wmfpolylinestep(picx(x[column+i]),picy(y[2][column+i],r));
/*
         if (!skip) wmfpolylinestep(picx(x[column+i]),picy(y[2][column+i],r));
         if (SYMBOLSTYLE(i)>'9') skip=(skip)?0:1;
*/
      }
   }
}

void wmfpolylinestart(Word x, Word y, Word points, Word style, Word width, Dword color) {
   if (Pen==-1 || color!=PenColor || style!=PenStyle || width!=PenWidth) Pen=pen(style,width,color);
   putdword((long)(4+2*points));
   putword(0X0325);
   putword(points);
   putword(x);
   putword(HEIGHT-y);
   currentx=x;
   currenty=y;
}

void wmfpolylinestep(Word x,Word y) {
   putword(x);
   putword(HEIGHT-y);
   currentx=x;
   currenty=y;
}

Word wmftext(Word direction, Word alignment, Dword color, char *s) {
   int len, extra, pos;
   Dword words, recordlen;
   if (Pen==-1 || PenColor!=PENCOLOR) Pen=pen(PenStyle,PenWidth,PENCOLOR);
   if (color!=TextColor) wmftextcolor(color);
/* no vertical central alignment! Only: Base, Bottom and Top
 0  6  2       1  7  3
 8 14 10       9 15 11
24 30 26      25 31 27
introduced: 125, 131 and 127 for vertical central alignment
ypos+=0.4*VsizeWMF;
*/
   if (alignment>100) {
      wmfmove(currentx,currenty-4*VsizeWMF/10);
//    alignment-=100;
   }
   len=strlen(s);
   extra=len%2;
   words=(Dword)((len+extra)>>1);
   if (direction!=Orientation || Font==-1) {
      Font=CreateFont(direction,Font,HsizeWMF,VsizeWMF);
      Orientation=direction;
   }
//   if (alignment!=(Alignment>100?Alignment-100:Alignment))
   if (alignment!=Alignment) {
      Alignment=alignment;
      putdword(4L);
      putword(0X012E);
      putword(Alignment>100?Alignment-100:Alignment);
   }
   recordlen=7L+words;
   if (maxrecordsize<recordlen) maxrecordsize=recordlen;
   putdword(recordlen);
   putword(0X0A32); /*ExtTextOut*/
   putword(HEIGHT-currenty);
   putword(currentx);
   putword(len);
   putword(0);
   for (pos=0; pos<len; pos++) putbyte(s[pos]);
   if (extra) putbyte(0);
   return Font;
}

void putbyte(Byte b) {
   fwrite(&b,sizeof(Byte),1,fpwmf);
}

void putword(Word w) {
   fwrite(&w,sizeof(Word),1,fpwmf);
}

void putdword(Dword dw) {
   fwrite(&dw,sizeof(Dword),1,fpwmf);
}

void wmfdot(Word x, Word y, Word size, Word style, Dword color) {
//   if (Pen==-1 || color!=PenColor || PenStyle!=0 || PenWidth!=1) Pen=pen(0,1,color);
// if (Brush==-1 || color!=BrushColor || style!=BrushStyle) Brush=brush(style,0,color);
   Pen=pen(0,1,BLACK);
   Brush=brush(style,0,color);
//   cprintf("\r\n%s %4d : brushcolor=%08lX WHITE=%08lX BLACK=%08lX",__FILE__, __LINE__,color,WHITE,BLACK);if (getch()==27) exit(-1);putch('\r');putch('\n');
//   size>>=2; factor 2 later weer nodig voor opschaling
   putdword(7L);      /* Size */
   putword(0X0418);   /* Ellipse */
   putword(HEIGHT-y-size);
   putword(x-size);
   putword(HEIGHT-y+size);
   putword(x+size);
}

void wmftriangle(Word x, Word y, Word size, Word picstyle, Dword color) {
   Word style;
   if (picstyle<2)     style=picstyle;
   else if (picstyle<4) style=picstyle-2;
   else if (picstyle==4)  {
      style=BrushStyle;
   }
//   if (Pen==-1 || color!=PenColor || PenStyle!=0 || PenWidth!=1) Pen=pen(0,1,color);
//   if (Brush==-1 || color!=BrushColor || style!=BrushStyle) Brush=brush(style,0,color);
//   size>>=1; factor 2 later weer nodig voor opschaling
   Pen=pen(0,1,BLACK);
   Brush=brush(style,0,color);
   if (picstyle<4) {
      putdword(10L);      /* Size 4 + 3 x,y-pairs*/
      putword(0X0324);    /* Polygon */
      putword(3);        /* N */
      if (picstyle <2) {
         putword(x);
         putword(HEIGHT-(y+size));
         putword(x-size);
         putword(HEIGHT-(y-size));
         putword(x+size);
         putword(HEIGHT-(y-size));
      } else {
         putword(x);
         putword(HEIGHT-(y-size));
         putword(x-size);
         putword(HEIGHT-(y+size));
         putword(x+size);
         putword(HEIGHT-(y+size));
      }
   } else {
      putdword(28L);      /* Size 4 + 12 x,y-pairs*/
      putword(0X0324);    /* Polygon */
      putword(12);        /* N */

      putword(x);
      putword(HEIGHT-(y+size));
      putword(x);
      putword(HEIGHT-y);

      putword(x+5*size/7);
      putword(HEIGHT-(y+4*size/7));
      putword(x);
      putword(HEIGHT-y);

      putword(x+5*size/7);
      putword(HEIGHT-(y-4*size/7));
      putword(x);
      putword(HEIGHT-y);

      putword(x);
      putword(HEIGHT-(y-size));
      putword(x);
      putword(HEIGHT-y);

      putword(x-5*size/7);
      putword(HEIGHT-(y-4*size/7));
      putword(x);
      putword(HEIGHT-y);

      putword(x-5*size/7);
      putword(HEIGHT-(y+4*size/7));
      putword(x);
      putword(HEIGHT-y);
   }
}

void wmfsquare(Word x, Word y, Word size, Word style, Dword color) {
   if (Pen==-1 || color!=PenColor || PenStyle!=0 || PenWidth!=1) Pen=pen(0,1,color);
//   if (Brush==-1 || color!=BrushColor || style!=BrushStyle) Brush=brush(style,0,color);
   Pen=pen(0,1,BLACK);
   Brush=brush(style,0,color);
//   size>>=1; factor 2 later weer nodig voor opschaling
   putdword(7L);      /* Size */
   putword(0X041B);   /* Rectangle */
   putword(HEIGHT-y-size);
   putword(x-size);
   putword(HEIGHT-y+size);
   putword(x+size);
}
void wmfdiamond(Word x, Word y, Word size, Word style, Dword color) {
//   if (Pen==-1 || color!=PenColor || PenStyle!=0 || PenWidth!=1) Pen=pen(0,1,color);
//   if (Brush==-1 || color!=BrushColor || style!=BrushStyle) Brush=brush(style,0,color);
   Pen=pen(0,1,BLACK);
   Brush=brush(style,0,color);
//   size>>=1; factor 2 later weer nodig voor opschaling
      putdword(14L);      /* Size 4 + 5 x,y-pairs*/
      putword(0X0324);    /* Polygon */
      putword(4);        /* N */
      putword(x);
      putword(HEIGHT-(y+size));
      putword(x+size);
      putword(HEIGHT-y);
      putword(x);
      putword(HEIGHT-(y-size));
      putword(x-size);
      putword(HEIGHT-y);
      putword(x);
      putword(HEIGHT-(y+size));
}

void wmfrectangle(Word x0, Word y0, Word x1, Word y1, Word brushstyle, Dword pencolor, Dword brushcolor) {
   int xsize, ysize;
//   if (Pen==-1 || pencolor!=PenColor || PenStyle!=0 || PenWidth!=1) Pen=pen(0,1,pencolor);
//   if (Brush==-1 || brushcolor!=BrushColor || brushstyle!=BrushStyle) Brush=brush(brushstyle,0,brushcolor);
   Pen=pen(0,1,pencolor);
   Brush=brush(brushstyle,0,brushcolor);
//   size>>=1; factor 2 later weer nodig voor opschaling
   xsize=x1-x0;
   ysize=y1-y0;
   putdword(7L);      /* Size */
   putword(0X041B);   /* Rectangle */
   putword(HEIGHT-(y0+ysize));
   putword(x0);
   putword(HEIGHT-y0);
   putword(x0+xsize);
}

int piccopy(char *source, char *target) {
   int n1, n2;
   while ((fpin=fopen(source,"rb"))== NULL) {
       clrscr();
       cprintf("Cannot open source file \"%s\"\r\nPress any key to continue ",source);
       if (getch()!=CR) exit(-1);
       return 0;
   }
   while ((fpout=fopen(target,"wb"))== NULL) {
       fclose(fpin);
       clrscr();
       cprintf("Cannot open target file \"%s\"\r\nPress any key to continue ",target);
       if (getch()!=CR) exit(-1);
       return 0;
   }
   if (Show) cprintf("\r\nWriting file \"%s\"\r\n",target);
   do {
      n1=fread(buffer,1,128,fpin);
      if (n1>0) n2=fwrite(buffer,1,n1,fpout);
      if (n2<n1) {
         fputc(END,fpout);
         fclose(fpin);
         fclose(fpout);
         clrscr();
         cprintf("Error writing to target file \"%s\"; disc full?\r\nPress any key to continue ",target);
         if (getch()!=CR) exit(-1);
         return 0;
      }
   } while (n1==128);
   fclose(fpin);
/*
   fputc(END,fpout);
*/
   fclose(fpout);
   return 1;
}

int picy_axis_title(char *y_axis_string, int y_pos, int picflag) {
   if (picflag) {
      picsize(Hsize,Vsize);
//      picmove(VsizeWMF/2,y_pos);
      picmove(VsizeWMF/2,y_pos);
   }
//   return pictextlines(VsizeWMF/2, y_pos, y_axis_string, 1, TOPRIGHT, picflag);
   return pictextlines(Vsize/2, y_pos, y_axis_string, 1, TOPRIGHT, picflag);
}

int picy2_axis_title(char *y2_axis_string, int y2_pos, int picflag) {
   int xpos;
   xpos=(Legendpos==1)?X_LENGTH-legendblocksize:X_LENGTH;
   if (picflag) {
      picsize(Hsize,Vsize);
      picmove(xpos,y2_pos);
   }
   return pictextlines(xpos, y2_pos, y2_axis_string, 3, TOPLEFT, picflag);
}

int ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag, int expflag, int xflag) {
    int n, range, exp, exp0, t, t0, found, negative, precision, extra_decimal=0;
    double unit, value, tempmin, tempmax, ratio, base, lowest_calibrator;
    int exponent[MAXNOOFCALIBRATORS];
//  tempmin=(logflag==2 && *min<2*Sd)?2*Sd:*min;
    tempmin=*min;
    if (*max-tempmin<0.0001) {
       cprintf("PROBLEM in axis calibration: range too small: %12.5f %12.5f\r\n",*min,*max);
       exit(-1);
    }
    if (logflag) round(scientific2(tempmin, &base, &exp));
    else round(scientific2(*max, &base, &exp));
    tempmin=tempmin * 10 +  fabs(tempmin)/10000;
    tempmax=*max * 10 -  fabs(*max)/10000;
    lowest_calibrator=val(10,exp);
    for (n=0;n<MAXNOOFCALIBRATORS;n++) {
        *calibratorstring[n]='\0';
        value_of_calibrator[n]=MISSING_CALIBRATOR;
    }

    if (logflag && tempmax<=0) logflag=FALSE;
    if (logflag) {
        if (tempmin<=0) tempmin= lowest_calibrator;
        ratio= (double)(tempmax / tempmin);
        if (ratio < 51 )       range= SMALL;
        else if (ratio < 301 ) range= MEDIUM;
        else                   range= LARGE;
    } else range=LINEAR;
    t=t0=0;
    found=FALSE;
    do{
       exp0=--exp;
       unit=val(calbase[range][t],exp);
    } while (3*unit>=tempmax-tempmin);
    while (!found) {
       if (! logflag) {
          found= ( 6*unit >= tempmax - tempmin);
          if (!found) {
             t++;
             t%=range+1;
             if (t==0) exp++;
             unit=val(calbase[range][t],exp);
          }
          if (found && t==2) extra_decimal=1;
       } else {
           value=val((double)calbase[range][t],exp+1);
           found= (value >= tempmin);
           if (!found) {
             t0=t;
             exp0=exp;
             t++;
             t%=range+1;
             if (t==0) exp++;
           } else {
             t=t0;
             exp=exp0;
             value=val((double)calbase[range][t],exp+1);
          }
       }
    }

PHASE2: /* find corrected min and max and fill array */
    if (!logflag) {
       if ( tempmin/unit-(int)(tempmin/unit) != 0) {
          negative= ( tempmin < 0);
          tempmin=  unit * (int)(tempmin/unit);
          if (negative) tempmin -= unit;
       }
    } else tempmin=val((double)calbase[range][t],exp+1);
    for (n=0 ; n < MAXNOOFCALIBRATORS ; n++) {
        if ( ! logflag ) {
           value_of_calibrator[n]=(tempmin + n*unit)/10;
           exponent[n]=exp;
        } else {
           exponent[n]= exp;
           value_of_calibrator[n]= val((double)calbase[range][t],exp);
           t++;
           t%=range+1;
           if (t==0) exp++;
        }
        if (value_of_calibrator[n] >= tempmax/10) {
            tempmax= value_of_calibrator[n]*10;
            *noofcalibrators=n+1;
            break;
        }
    }
    *min=tempmin/10;
    *max=tempmax/10;
    if (!calcflag) return logflag;

/* phase 3: print results in minimal-length strings */

    for (n=0; n< *noofcalibrators ; n++) {
        if (logflag && expflag) sprintf(calibratorstring[n],"%d",exponent[n]);
        else {
           if (value_of_calibrator[n]>(*max>0?1.0001* *max:0.9999* *max)) {
              break;
           }
           precision= (range==LINEAR) ? (extra_decimal - exponent[n]): -exponent[n];
           if (precision<0) precision=0;
           sprintf(calibratorstring[n],"%.*lf", precision, value_of_calibrator[n]);
       }
    }
/* phase 4: if (logflag) transform data */

   if (logflag) for (n=0; n<*noofcalibrators; n++) {
      if (xflag) value_of_calibrator[n]=log10(value_of_calibrator[n]);
      else {
         if (logflag==2) value_of_calibrator[n]=transform(value_of_calibrator[n],Sd,Vc);
         else value_of_calibrator[n]=log10(value_of_calibrator[n]);
      }
   }

/* return logflag to indicate whether log-transformation was actually performed */

    return logflag;
}

int picy_cal(int logflag, int picflag) {
   int pos, cal, logyflag, mantissa, lowest_exponent, highest_exponent;
   int ox, lx;
   double calibrator_value;
   float min,max;
   logyflag= ascal(&noofcalibrators, &ymin, &ymax, logflag, TRUE, Yformat,0);
   if (logyflag==0) {
      min=ymin;
      max=ymax;
   } else if (logyflag==1) {
      min=log10(ymin);
      max=log10(ymax);
   } else {
      min=transform(ymin,Sd,Vc);
      max=transform(ymax,Sd,Vc);
   }
   if (Xcal/3==0) {
      ox=x_origin - Rim;
      lx=x_length + Rim + ((Ycal==0)?Rim:0);
   }else {
      ox=x_origin;
      lx=x_length;
   }
   ox-=log0;
   scale_yfactor=(double)y_length/(max - min);
   if (!picflag) return logyflag;
   picmove(ox,y_origin   );
   picdraw(ox,y_origin+y_length);
   if (Xcal%3<2 && !dual_axis) {
      picmove(ox+lx+log0,y_origin   );
      picdraw(ox+lx+log0,y_origin+y_length);
   }
   if (Xcal%3==1&& !dual_axis) {
      picmove(ox+lx+log0,y_origin-(Ycal<3?Rim/2:0));
      picdraw(ox+lx+log0,y_origin+y_length+(Ycal==0?Rim/2:0));
   }
   picsize(Hsize,Vsize);
   for (cal=0; cal<noofcalibrators; cal++) {
      if (logyflag==2 && value_of_calibrator[cal]<transform(2*Sd,Sd,Vc)) continue;
      calibrator_value=value_of_calibrator[cal];
      pos= (cal==noofcalibrators-1)?y_origin+y_length: y_origin + scale_yfactor * (calibrator_value-min);
      if (Hgrid) {
         if (!dual_axis) {
            picmove(ox, pos);
            picdraw(ox + lx+log0,pos);
         }
      } else {
            picmove(ox,pos);
            picdraw(ox+Rim/2,pos);
            if (Xcal%3==0 && !dual_axis) {
               picmove(ox + lx+log0,pos);
               picdraw(ox + lx+log0-Rim/2,pos);
            }
      }
         picmove(ox-Rim/2,pos);
         pictext(0,CENTERRIGHT,calibratorstring[cal]);
   }
   if (logyflag) {
      highest_exponent=(int)(max+0.001);
      lowest_exponent =(int)(min-1.1);
      for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
         for (mantissa=1;mantissa<10;mantissa++) {
            if (logyflag==0) calibrator_value = val((double)mantissa,cal);
            else if (logyflag==1) calibrator_value = log10(val((double)mantissa,cal));
            else calibrator_value = transform(val((double)mantissa,cal),Sd,Vc);
            if (calibrator_value > min && calibrator_value < max) {
               pos= y_origin + scale_yfactor * (calibrator_value-min);
               if (pos<y_origin-5 || pos>y_origin + y_length+5) continue;
               if (abs(y_origin+y_length-pos)<=5) pos=y_origin+y_length;
                  if (Hgrid) {
                     picmove(ox,pos);
                     picdraw(ox+lx+log0,pos);
                  } else {
                     picmove(ox,pos);
                     picdraw(ox+Rim/4,pos);
                     if (Xcal%3==0 && !dual_axis) {
                        picmove(ox+lx+log0,pos);
                        picdraw(ox+lx+log0-Rim/4,pos);
                     }
                  }
            }
         }
      }
   }
   return logyflag;
}

int picy2_cal(int logflag, int picflag) {
   int pos, cal, logy2flag, mantissa, lowest_exponent, highest_exponent;
   int xline, xtick, xtick2, xtext, ystart, ystop;
   double calibrator_value;
   float min,max;
   logy2flag= ascal(&noofcalibrators, &y2min, &y2max, logflag, TRUE, Yformat,0);
   if (logy2flag==0) {
      min=y2min;
      max=y2max;
   } else if (logy2flag==1) {
      min=log10(y2min);
      max=log10(y2max);
   } else {
      min=transform(y2min,Sd,Vc);
      max=transform(y2max,Sd,Vc);
   }
   scale_y2factor=(double)y_length/(max - min);
   if (!picflag) return logy2flag;
//   xline=x_origin+x_length+log0;
   xline=x_origin+x_length;
   xtick=xline-Rim/2;
   xtick2=xline-Rim/4;
   xtext=xline+Rim/2;
   ystart=y_origin;
   ystop=y_origin+y_length;
   if (Xcal%3==1) {
      ystart-=(Ycal<3?Rim/2:0);
      ystop+= (Ycal==0?Rim/2:0);
   }
   picmove(xline,ystart);
   picdraw(xline,ystop);
   picsize(Hsize,Vsize);
   for (cal=0; cal<noofcalibrators; cal++) {
      if (logy2flag==2 && value_of_calibrator[cal]<transform(2*Sd,Sd,Vc)) continue;
      calibrator_value=value_of_calibrator[cal];
      pos= (cal==noofcalibrators-1)?y_origin+y_length: y_origin + scale_y2factor * (calibrator_value-min);
      picmove(xline,pos);
      picdraw(xtick,pos);
      picmove(xtext,pos);
      pictext(0,CENTERLEFT,calibratorstring[cal]);
   }
   if (logy2flag) {
      highest_exponent=(int)(max+0.001);
      lowest_exponent =(int)(min-1.1);
      for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
         for (mantissa=1;mantissa<10;mantissa++) {
            if (logy2flag==0) calibrator_value = val((double)mantissa,cal);
            else if (logy2flag==1) calibrator_value = log10(val((double)mantissa,cal));
            else calibrator_value = transform(val((double)mantissa,cal),Sd,Vc);
            if (calibrator_value > min && calibrator_value < max) {
               pos= y_origin + scale_y2factor * (calibrator_value-min);
               if (pos<y_origin-5 || pos>y_origin + y_length+5) continue;
               if (abs(y_origin+y_length-pos)<=5) pos=y_origin+y_length;
               picmove(xline,pos);
               picdraw(xtick2,pos);
            }
         }
      }
   }
   return logy2flag;
/*
   if (Xcal%3==1) {
      picmove(xline,y_origin-(Ycal<3?Rim/2:0));
      picdraw(xline,y_origin+y_length+(Ycal==0?Rim/2:0));
   }

   if (Xcal/3==0) {
      ox=x_origin - Rim;
      lx=x_length + Rim + ((Ycal==0)?Rim:0);
   }else {
      ox=x_origin;
      lx=x_length;
   }
   picmove(ox+lx+log0,y_origin   );
   picdraw(ox+lx+log0,y_origin+y_length);
   if (Xcal%3==1) {
      picmove(ox+lx,y_origin-(Ycal<3?Rim/2:0));
      picdraw(ox+lx,y_origin+y_length+(Ycal==0?Rim/2:0));
   }
*/
}

void draw_xy(int x_origin, float xmin, float xmax, double scale_xfactor, int y_origin, float ymin, float ymax, double scale_yfactor) {
   float x1, x2, y1, y2;
   x1= (xmin<ymin) ? ymin:xmin;
   y1= x1;
   x2= (xmax<ymax) ? xmax:ymax;
   y2= x2;
   picmove(x_origin + (x1-xmin) * scale_xfactor, y_origin + (y1-ymin) * scale_yfactor);
   picdraw(x_origin + (x2-xmin) * scale_xfactor, y_origin + (y2-ymin) * scale_yfactor);
}

int picx_cal(int logflag, int picflag) {
   int month, pos, cal, mantissa, lowest_exponent, highest_exponent;
   int oy, ly;
   char name[10];
   double calibrator_value;
   float min,max;
   if (Xformat!=2) {
      logxflag= ascal(&noofcalibrators, &xmin, &xmax, logflag, 1, Xformat,1);
      min=(logxflag)?log10(xmin):xmin;
      max=(logxflag)?log10(xmax):xmax;
   } else {
      logxflag= 0;
      min= xmin=(int) xmin;
      max= xmax=ceil(xmax);
   }
   if (Ycal/3==0) {
      oy=y_origin-Rim;
      ly=y_length + Rim + ((Xcal==0)?Rim:0);
   } else {
      oy=y_origin;
      ly=y_length;
   }
   if (!picflag) return logxflag;
   scale_xfactor=(double)x_length/(max - min);
   if (Xformat==1) picmove(x_origin-log0,oy - Vsize*1.5-40);
   else picmove(x_origin-log0,oy - Vsize-40);
   pictext(0, TOPLEFT, x_legend);
   if (log0) {
      picmove(x_origin-log0,  oy+Rim/2);
      picdraw(x_origin-log0,  oy);
      picdraw(x_origin-log0/2,oy);
      if (Ycal%3==0) {
         picmove(x_origin-log0/2,oy+ly);
         picdraw(x_origin-log0,  oy+ly);
         picdraw(x_origin-log0,  oy+ly-Rim/2);
      }
      else if (Ycal%3==1) {
         picmove(x_origin,oy+ly);
         picdraw(x_origin-log0-(Xcal==0?Rim/2:0),oy+ly);
      }
      picmove(x_origin-log0,oy-Vsize);
      pictext(0, CENTERBOTTOM, "0");
   }
   picmove(x_origin   , oy);
   picdraw(x_origin+x_length, oy);
   if (Ycal%3<2) {
      picmove(x_origin,    oy+ly);
      picdraw(x_origin+x_length, oy+ly);
   }
   if (Ycal%3==1) {
      picmove(x_origin         - (Xcal<3 ?Rim/2:0), oy+ly);
      picdraw(x_origin+x_length+ (Xcal==0?Rim/2:0), oy+ly);
   }
   if (Xformat<2) for (cal=0; cal<noofcalibrators; cal++) {
      calibrator_value=value_of_calibrator[cal];
      pos= x_origin + scale_xfactor * (calibrator_value-min);
      if (abs(x_origin+x_length-pos)<=5) pos=x_origin+x_length;
      if (Vgrid) {
            picmove(pos, oy);
            picdraw(pos, oy+ly);
      } else {
            picmove(pos,oy+Rim/2);
            picdraw(pos,oy);
            if (Ycal%3==0) {
               picmove(pos,oy+ly);
               picdraw(pos,oy+ly-Rim/2);
            }
      }
      picmove(pos,oy-Vsize);
      if (!logxflag || !Xformat) pictext(0, CENTERBOTTOM, calibratorstring[cal]);
      else {
         pictext(0,BOTTOMRIGHT,"10");
         picmove(pos,oy-0.8*Vsize);
         pictext(0,BOTTOMLEFT,calibratorstring[cal]);
      }
   } else {
      for (month=(int) min; month<=ceil(max); month++) {
         pos= x_origin + scale_xfactor * (month-(int)min);
         if (abs(x_origin+x_length-pos)<=5) pos=x_origin+x_length;
            if (Vgrid) {
               picmove(pos, oy);
               picdraw(pos, oy+ly);
            } else {
               picmove(pos,oy+Rim/2);
               picdraw(pos,oy);
               if (Ycal%3==0) {
                  picmove(pos,oy+ly);
                  picdraw(pos,oy+ly-Rim/2);
               }
            }
            picmove(pos,oy-Vsize);
            if (find_monthname(name, month, scale_xfactor)) pictext(0, CENTERBOTTOM, name);
      }
      for (month=12; month<ceil(max); month+=12) {
         if (month < min) continue;
         pos= x_origin + scale_xfactor * ((float) month - min + 0.5);
         if (abs(x_origin+x_length-pos)<=5) pos=x_origin+x_length;
         picmove(pos,oy - Vsize);
         picdraw(pos,oy+ly);
      }
   }
   if (logxflag) {
      highest_exponent=(int)(max/log10(10)+0.001);
      lowest_exponent=(int)(min/log10(10)-1.1);
      for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
         for (mantissa=1;mantissa<10;mantissa++) {
            calibrator_value= log10(val((double)mantissa,cal));
            if (calibrator_value > min && calibrator_value < max) {
               pos= x_origin + scale_xfactor * (calibrator_value-min);
               if (pos<x_origin-5 || pos>x_origin+ x_length+5) continue;
               if (abs(x_origin+x_length-pos)<=5) pos=x_origin+x_length;
                  if (Vgrid) {
                     picmove(pos, oy);
                     picdraw(pos, oy+ly);
                  } else {
                     picmove(pos, oy);
                     picdraw(pos, oy+Rim/4);
                     if (Ycal%3==0) {
                        picmove(pos, oy+ly);
                        picdraw(pos, oy+ly-Rim/4);
                     }
                  }
            }
         }
      }
   }
   return logxflag;
}

long LINECOLOR(int pos) {
   char c;
   c=lcolorstring[pos%strlen(lcolorstring)];
   if (c>'@') c-='A';
   else if (c>='0') c='Z'+c-'0';
   else c=COLORS-1;
   return rgb[c];
}

long SYMBOLCOLOR(int pos) {
   char c;
   int i;
   if (!Color) i=26;
   else {
   c=scolorstring[pos%strlen(scolorstring)];
   if (c>'@') i=c-'A';
   else if (c>='0') i='Z'+c-'0';
   else i=COLORS-1;
   }
//cprintf("\r\n%s %4d : pos=%d c=%c i=%d rgb[%d]=%08Xl",__FILE__, __LINE__,pos,c,i,rgb[i]);if (getch()==27) exit(-1);putch('\r');putch('\n');
   return rgb[i];
}

/* FFFFFFFFFFFFFFF   PIC-FILE ROUTINES    FFFFFFFFFFFFFFFFFFFFFF */
#include "ttest.h"
/* module $pic.c */

int  ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
int  picbottomline(int n);
//int  fill_array[200];  /* array van x,y coord voor FILL(O)*/

void piccolor(int color);
void piccolumnlegend(int t, int xpos, int range_type, int testnamelines, int evaltextlines);
void piccorner(void);
void picdot(int x,int y, int type, int size);
void picdotted_line(int x0, int y0, int x, int y, int type);
void picfill(int x0, int y0, int x, int y, int type);
void picdraw(int x,int y);
void picend(void);
void picextra_text(int x_origin, int y_origin, int lx, int ly);
void picfillo(int vertices,int *data_array,int fill);
void picfirst_title(char *title, int picmidx);
void picfont(char font);
void piclegend(int legends_per_line);
void picmove(int x,int y);
void picopen(char *s);
int  picranges(int range_type, int testnamelines, int evaltextlines, double inter);
void picsecond_title(char *title, int picmidx);
void picsize(int x,int y);
int  picstring(int datapoint, int column, int x, int y, int n, int intra,int size);
void pictext(int direction,int position, char *s);
int  pictextlines(int xpos, int ypos, char *textline, int direction, int position);
int  pictopline(int n);
void picxy(int x,int y);
int  picy_axis_title(char *y_axis_string, int y_pos);
int  picy_cal(int logflag);

/* local routines: */
int    picdotdot(double step1,double step2,double xfactor,double yfactor,double *xpos,double *ypos,double xlim,double ylim);
double pow10d(double exponent);
double scientific2(double d, double *base, int *exp);
int    round(double d);
double val(double mantissa, int exponent);

int picbottomline(int n) {
   return(Y_ORIGIN + n * Vsize);
}

void piccolumnlegend(int t, int xpos, int range_type, int testnamelines, int evaltextlines) {
   int n, y, line, legendlines, rangelines, hsize;
   char *bufferpointer;
   hsize=(Small)?Hsize/2:Hsize;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d t=%d range_type=%d testnamelines=%d evaltextlines=%d",
            __FILE__, __LINE__, t, range_type, testnamelines, evaltextlines);
   #endif
   for (n=0;n<Plotter;n++) {
      picsize(hsize,Vsize);
      if (!Subselect || range_type!=1) {
         strcpy(buffer, testname[test[t]]);
         columntext[t]=testname[test[t]];
         legendlines=testnamelines;
         rangelines=evaltextlines;
      } else {
         strcpy(buffer, evaltext[t]);
         columntext[t]=evaltext[t];
         legendlines=evaltextlines;
         rangelines=testnamelines;
      }
      bufferpointer=strtok(buffer,"\\");

      if (Swap) y= (!Subselect || Ranges_in_header || No_ranges) ? bottomlines: bottomlines - rangelines - 1;
/*
		if (Swap) y= (!Subselect || Ranges_in_header || No_ranges) ? bottomlines: bottomlines - rangelines - 2;
*/
      else y= bottomlines - 1;
      if (Stagger && (t-empty_tests(0,t))%2) y--;
      for (line=0; line<legendlines; line++, y--) {
        #ifdef TRACE
           if (Trace==2) fprintf(tracefile,"\n%4d y=%d text=%s bufferpointer=%s",
                 __LINE__, y, buffer, bufferpointer);
        #endif
/*
        picmove(xpos, picbottomline(y)+32);
*/
        picmove(xpos, picbottomline(y));
/*
        if (line==0)  pictext(0,CENTERBOTTOM,buffer);
        else if (bufferpointer) pictext(0,CENTERBOTTOM,bufferpointer);
*/
        if (bufferpointer) pictext(0,CENTERBOTTOM,bufferpointer);
        bufferpointer=strtok(NULL,"\\");
      }
   }
   picsize(Hsize,Vsize);
}

void picdotted_line(int x0, int y0, int x, int y, int type) {
#define SIZE1 8.0
#define SIZE2 32.0
#define SIZE3 40.0
#define SIZE4 64.0
   int t, i, ddx, ddy;
   double dx, dy, size, xpos, ypos, xfactor, yfactor;
   t=Symbol_string[type%strlen(Symbol_string)]-'0';
	dx=(double)(x-x0);
   dy=(double)(y-y0);
   size=sqrt(dx*dx+dy*dy);
   if (!size) return;
   xfactor=dx/size;
   yfactor=dy/size;
   xpos=(double)x0;
   ypos=(double)y0;
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
         picmove(x0+ddx ,y0-ddy);
			picdraw(x +ddx, y -ddy);
         picmove(x0-ddx, y0+ddy);
         picdraw(x -ddx, y +ddy);
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
      case 9: /* solid+dot */
         ddx=(int)(6.0*yfactor);
         ddy=(int)(6.0*xfactor);
         picmove(x0+ddx ,y0-ddy);
         picdraw(x +ddx, y -ddy);
         xpos=x0-6.0*yfactor;
         ypos=y0+6.0*xfactor;
         while (picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x-6.0*yfactor, (double) y+6.0*xfactor));
/*
         do {
            i=        picdotdot( SIZE4, SIZE2, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
            if (i) i= picdotdot( SIZE1, SIZE3, xfactor, yfactor, &xpos, &ypos, (double) x, (double) y);
         } while (i);
*/
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
         picmove(x0,y0);
         picdraw(x,y);
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
   picmove((int)*xpos, (int)*ypos);
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
   picdraw((int)*xpos, (int)*ypos);
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

void picfill(int x0, int y0, int x, int y, int type) {
   int d, i, xx, yy;
   if (type<0 || type>5) return;
   if (x0<0 || y0<0) return;
   if (x>3200 || y>3200) return;
   if (type==1) d=40;
   if (type==2) d=32;
   if (type==3) d=24;
   if (type==4) d=16;
   if (y0==y) {
      if (type==5) {
         picmove(x0,y);
         picdraw(x,y);
         return;
      } else {
         for (xx=x0; xx<=x; xx+=2*d) {
            picmove(xx,y);
            picdraw(xx+d, y);
         }
         return;
      }
   }
   if (type==0) {
         picmove(x0,y0);
         picdraw(x,y0);
         picmove(x0,y);
         picdraw(x,y);
         return;
   }
   if (type==5) {
      for (yy=y0; yy<=y; yy+=2) {
         picmove(x0,yy);
         picdraw(x,yy);
      }
      return;
   }
   i=0;
   for (yy=y0; yy<=y; yy+=d) {
      i=(i)?0:d/2;
      for (xx=x0+i; xx<=x; xx+=d){
        picmove(xx, yy-1);
        picdraw(xx, yy+1);
      }
   }
}

void picopen(char *s) {
int n;
unsigned char c;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d picopen \"%s\"",__FILE__, __LINE__,s);
   #endif
   if ((fpout=fopen(s,"wb")) == NULL) {
       E5;
       getch();
       farewell(pic_no);
   }
   clearerr(fpout);
   for (n=0;n<17;n++){
      c=header_vector[n];
      fputc(c,fpout);
   }
   bytecounter=17;
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\n",
         bytecounter);
      if (getch()!='R') farewell(pic_no);
   }
   picsize(Hsize,Vsize);
   picfont(1);
   piccolor(0);
}

void piccorner() {
   picmove(0,10);
   picdraw(0,0);
   picdraw(10,0);
}

void picextra_text() {
   int i, x, y, position;
   for (i=0; i<10; i++) {
      if (extra_text[i][0]) {
         switch (i) {
            case 0:
               position=TOPLEFT;
               x=X_ORIGIN;
               y=PICTOP;
               break;
            case 1:
               position=TOPRIGHT;
               x=X_ORIGIN+X_length;
               y=PICTOP;
               break;
            case 2:
               position=TOPLEFT;
               x=x_origin+12;
               y=y_origin+y_length-12;
               break;
            case 3:
               position=CENTERTOP;
               x=x_origin + x_length/2;
               y=y_origin+y_length-12;
               break;
            case 4:
               position=TOPRIGHT;
               x=x_origin+x_length-12;
               y=y_origin+y_length-12;
               break;
            case 5:
               position=BOTTOMLEFT;
               x=x_origin+12;
               y=y_origin+24;
               break;
            case 6:
               position=CENTERBOTTOM;
               x=x_origin + x_length/2;
               y=y_origin+24;
               break;
            case 7:
               position=BOTTOMRIGHT;
               x=x_origin+x_length-12;
               y=y_origin+24;
               break;
            case 8:
               position=BOTTOMLEFT;
               x=X_ORIGIN;
               y=picbottomline(bottomlines-1);
               break;
            case 9:
               position=BOTTOMRIGHT;
               x=X_length;
               y=Y_ORIGIN;
               break;
         }
         picmove(x,y);
         pictext(0,position, extra_text[i]);
      }
   }
}
         
void picmove(int x,int y) {
   fputc(MOVE,fpout);
   bytecounter+= 1;
   picxy(x,y);
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npicmove: MOVE=%d x=%d y=%d\r\n",
         bytecounter, MOVE, x, y);
      if (getch()!='R') farewell(pic_no);
   }
}

void picfont(char font) {
   fputc(FONT,fpout);
   fputc(font,fpout);
   bytecounter+= 2;
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npicfont: FONT=%d font=%d\r\n",
         bytecounter, FONT, font);
      if (getch()!='R') farewell(pic_no);
   }
}

void piccolor(int color) {
   fputc(COLOR0+color%16,fpout);
   bytecounter+= 1;
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npiccolor: COLOR=%d\r\n",
         bytecounter, COLOR0+color%16);
      if (getch()!='R') exit(-1);
   }
}

void picsize(int x,int y) {
   fputc(SIZE,fpout);
   bytecounter+= 1;
   picxy(x,y);
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npicsize: SIZE=%d x=%d y=%d\r\n",
         bytecounter, SIZE, x, y);
      if (getch()!='R') farewell(pic_no);
   }
}

void picdraw(int x,int y) {
   fputc(DRAW,fpout);
   bytecounter+= 1;
   picxy(x,y);
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npicdraw: DRAW=%d x=%d y=%d\r\n",
         bytecounter, DRAW, x, y);
      if (getch()!='R') farewell(pic_no);
   }
}

void pictext(int direction,int position, char *s) {
   fputc(TEXT,fpout);
   fputc((unsigned char)direction*16+position,fpout);
   fputs(s,fpout);
   fputc('\0',fpout);
   bytecounter+=3+strlen(s);
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npictext: TEXT=%d %d(direction)*16+%d(position)=%d \"%s\"\r\n",
         bytecounter, TEXT, direction, position, (unsigned char)direction*16+position, s);
      if (getch()!='R') farewell(pic_no);
   }
}

int pictextlines(int xpos, int ypos, char *text, int direction, int position) {
   int line, lines, n;
   char *bufferpointer;
   char buffer[MAX_LEN+1];
   if (*text=='\\') strncpy(buffer,text+1,MAX_LEN);
   else strncpy(buffer,text,MAX_LEN);
   lines=count_char(buffer,'\\')+1;
   bufferpointer=strtok(buffer,"\\");
   for (line=0; line<lines; line++) {
      #ifdef TRACE
        if (Trace==2) fprintf(tracefile,"\n%4d xpos=%d ypos=%d text=%s bufferpointer=%s",
              __LINE__, xpos, ypos, buffer, bufferpointer);
      #endif
      picmove(xpos, ypos);
      if (line==0) for (n=0;n<Plotter;n++) pictext(direction,position,buffer);
      else if (bufferpointer) for (n=0;n<Plotter;n++) pictext(direction,position,bufferpointer);
      bufferpointer=strtok(NULL,"\\");
      if (!direction) ypos-=Vsize;
      else            xpos+=1.2*Hsize;
  }
  if (direction) return xpos;
  return ypos;
}

void picend(void) {
   fputc(END,fpout);
   fclose(fpin);
   fclose(fpout);
   bytecounter+= 1;
   if (ferror(fpout)) {
      cprintf("\r\nError writing PIC-file at byte %ld\r\npicend: END=%d\r\n",
         bytecounter, END);
      if (getch()!='R') farewell(pic_no);
   }
}

void picxy(int x,int y) {
int e;
   fputc(x/256,fpout);
   fputc(x%256,fpout);
   fputc(y/256,fpout);
   fputc(y%256,fpout);
   bytecounter+= 4;
   if ((e=ferror(fpout))!=0) {
      cprintf("\r\nError %d writing PIC-file at byte %ld\r\npicxy: x=%d y=%d\r\n",
         e, bytecounter, x, y);
      if (getch()!='R') farewell(pic_no);
   }
}

void picfirst_title(char *title, int picmidx) {
   picsize(Hsize,Vsize);
   picmove(picmidx,PICTOP);
   pictext(0,CENTERTOP,title);
}

void picsecond_title(char *title, int picmidx) {
   picsize(Hsize,Vsize);
   picmove(picmidx,PICTOP-Vsize);
   pictext(0,CENTERTOP,title);
}

int picy_axis_title(char *y_axis_string, int y_pos) {
   picsize(Hsize,Vsize);
   picmove(40,y_pos);
   return pictextlines(0, y_pos, y_axis_string, 1, TOPRIGHT)+1;
}

int pictopline(int n) {
    return PICTOP- n*Vsize;
}

void piclegend(int legends_per_line) {
   int i, t, n, dy, x, y, lx=XSIZE, lines;
   char *bufferpointer;
   #ifdef TRACE
     if (Trace==2) fprintf(tracefile,"\n%s %4d piclegend",__FILE__, __LINE__);
   #endif
   if (!legends_per_line) return;
   for (i=0,n=0; i<SYMBOLS && !n; i++) if (symbol_in_use[i]) n=1;
   if (!n) return;
/*
   dy=(0.5 * Vsize - Legend_symbol_size)/2; /* tekst te laag */
   dy=(Vsize - Legend_symbol_size)/2; /* tekst te hoog */
   dy=0.5 * Vsize - Legend_symbol_size; /* tekst te laag */
   dy=(0.5 * Vsize - Legend_symbol_size)/2+20; /* tekst te hoog */
   dy=(0.5 * Vsize - Legend_symbol_size)/2-20; /* tekst veel te laag */
*/
   dy=(0.5 * Vsize - Legend_symbol_size)/2+10; /* tekst */
   if (Legendpos==0) {
      lines=ceil((double)symbols/legends_per_line);
      for (i=0,t=0; i<SYMBOLS; i++) {
         if (!symbol_in_use[i]) continue;
         if (Color>1) piccolor(Symbol_color[i%strlen(Symbol_color)]);
         x= x_origin + (x_length*((t/lines)%legends_per_line))/legends_per_line;
/*
         y= picbottomline(lines-(t%lines)-1) + Vsize;
*/
         y= picbottomline(lines-(t%lines)-1) + Vsize/2;
         picdot (x, y - dy , i, Legend_symbol_size);
         if (Line) {
            picdot (x+lx , y - dy , i, Legend_symbol_size );
            picmove(x    , y - dy );
            picdraw(x+lx , y - dy );
            picmove(Legend_symbol_size + x + lx, y);
         } else picmove(Legend_symbol_size + x, y);
         pictext(0, CENTERLEFT, symbol_legend[i]);
         #ifdef TRACE
            if (Trace==2) fprintf(tracefile,"\n%s %4d legend %d: \"%s\"",__FILE__, __LINE__,i,symbol_legend[i]);
         #endif
         t++;
      }
   } else {
      if (Legendpos==1) x= x_origin + x_length + RIM + 50;
      if (Legendpos==2) x= x_origin + x_length - maxlegendsize-2*Legend_symbol_size;
      if (Legendpos==3) x= x_origin;
      y= y_origin+y_length - 0.6*Vsize;
/*
      legendlines=get_legendlines();
      picmove(x, y + 0.6*Vsize);
      picdraw(x, y - legendlines*Vsize+0.6*Vsize);
      picdraw(x+maxlegendsize+2*Legend_symbol_size, y - legendlines*Vsize+0.6*Vsize);
      picdraw(x+maxlegendsize+2*Legend_symbol_size, y + 0.6*Vsize);
      picdraw(x, y + 0.6*Vsize);
*/
      x+=Legend_symbol_size;
      for (i=0; i<SYMBOLS; i++) {
         if (!symbol_in_use[i]) continue;
         if (Color>1) piccolor(Symbol_color[i%strlen(Symbol_color)]);
			picdot (x, y - dy, i, Legend_symbol_size);
         strncpy(buffer,symbol_legend[i],79);
         bufferpointer=strtok(buffer,"\\");
         if (Line) { 
            picdot (x+lx , y - dy , i, Legend_symbol_size );
            picmove(x    , y - dy );
            picdraw(x+lx , y - dy );
         }
         do {
            if (Line) picmove(Legend_symbol_size + x + lx, y);
            else picmove(Legend_symbol_size + x, y);
            if (bufferpointer) pictext(0, CENTERLEFT, bufferpointer);
            bufferpointer=strtok(NULL,"\\");
            y -= Vsize;
         }  while (bufferpointer);
         #ifdef TRACE
            if (Trace==2) fprintf(tracefile,"\n%s %4d legend %d: \"%s\"",__FILE__, __LINE__,i,symbol_legend[i]);
         #endif
      }
   }
}

int picstring(int datapoint, int column, int x, int y, int n, int intra,int size) {
	int point, dx, row;
	unsigned char s;
	for (row=0; row<rows; row++) symbol[row]=255;
	for (point=0; point<n; point++) {
		s=get_symbol(datapoint+point,column,&row);
		if (s>0 && s<SYMBOLS) symbol_in_use[s]=TRUE;
		symbol[row]=(Datasymbol==-1 && row<rows && row>=0)?s:Datasymbol;
	}
	for (point=0,row=0; row<rows && point<n; row++) {
		if (symbol[row]==255) continue;
		dx= find_dx( point, n, intra);
		if (Color>1 && symbol[row]>0) piccolor(Symbol_color[symbol[row]%strlen(Symbol_color)]);
		if (Literal && symbol[row]>0 && symbol[row]<SYMBOLS) picdot(x+dx,y,symbol[row]+'0',size);
		else picdot(x+dx,y,symbol[row],size);
		#ifdef TRACE
			if (Trace==2) fprintf(tracefile,"\n%4d %s: datapoint=%2d n=%2d column=%2d row=%3d point=%2d symbol[%d]=%d color=%d",
					__LINE__, __FILE__, datapoint, n, column, row, point, row, symbol[row], Symbol_color[symbol[row]%strlen(Symbol_color)]);
		#endif
		point++;
	}
	return datapoint+point;
}

#define MAXFILLARRAY 1000
void picdot(int x,int y, int type, int size) {
	int dx,dy,r=size/2,t,n;
	int  fill_array[MAXFILLARRAY];  /* array van x,y coord voor FILL(O)*/

	char buffer[2]=" ";
	char fillornofill;
/*
	if (x<0 || y<0 || type<0 || size<0 || x>3200 || y>PICTOP || type>255 || size>255) {
		cprintf("\r\n picdot-error: x=%d (max=%d) y=%d (max=%d) type=%d size=%d",
		x, 3200, y, PICTOP, type, size);
		if (getch()==ESC) farewell(pic_no);
		return;
	}
*/
	if (type>=' ' && type<256 && !(Graph_type ==DOTMATRIX || cormatrix_flag)) {
		*buffer=type;
		picsize(2*size, size*18/8);
		picmove(x,y);
		pictext(0,CENTER,buffer);
		picsize(Hsize,Vsize);
		return;
	}
	if (size<4 || (type==SYMBOLS && strlen(Symbol_string)<SYMBOLS)) {
		picmove(x,y-2);
		picdraw(x,y+2);
		return;
	}
	if (Graph_type==DOTMATRIX || cormatrix_flag) t=type;
	else t=(Datasymbol==-1)?Symbol_string[type%strlen(Symbol_string)]:Datasymbol+'0';
	#ifdef TRACE
		if (Trace==2) fprintf(tracefile,"\n%4d: type=%d Symbol_string=%s t=%d size=%d",
				__LINE__, type, Symbol_string, t, size);
	#endif

	fillornofill=1;

	switch(t) {
		case '0': fillornofill=0;      /* open circle */
		case '1': fillornofill*=1;     /* filled circle */
			picfont(11+fillornofill);   /* identificatie circle font 11 & 12 */
			picmove(x,y);
			picmove(r,r);
//			picmove(x,y);
			n=0;
			for (dy=r; dy > -r; dy -= PIXELS_PER_LINE)
			  {
				dx= (int)(sqrt((double)r*r-(double)dy*dy)+0.5);
				if(n>=MAXFILLARRAY) break;
				fill_array[n++]=x+dx;
				fill_array[n++]=y+dy;
			  }
			for ( ; dy <= r; dy+= PIXELS_PER_LINE)
			 {
				dx= (int)(sqrt((double)r*r-(double)dy*dy)+0.5);
				if(n>=MAXFILLARRAY) break;
				fill_array[n++]=x-dx;
				fill_array[n++]=y+dy;
				if(n>MAXFILLARRAY-5) break;
			  }
			picfillo(n/2-1,fill_array,fillornofill);
			picfont(10);
			break;
		case '2':  fillornofill=0;     /* open square */
		case '3':  fillornofill*=1;    /* filled square */
			n=0;
			picfont(13+fillornofill);   /* identificatie  */
			picmove(x,y);
			picmove(r,r);
//			picmove(x,y);
			fill_array[n++]=x+r;
			fill_array[n++]=y+r;
			fill_array[n++]=x+r;
			fill_array[n++]=y-r;
			fill_array[n++]=x-r;
			fill_array[n++]=y-r;
			fill_array[n++]=x-r;
			fill_array[n]=y+r;
			picfillo(n/2,fill_array,fillornofill);
			picfont(10);
			break;

		case '4':    fillornofill=0;   /* open triangle */
		case '5':    fillornofill*=1;  /* filled triangle */
			n=0;
			picfont(15+fillornofill);   /* identificatie  */
			picmove(x,y);
			picmove(r,r);

//			picmove(x  ,y+r);
			fill_array[n++]=x;
			fill_array[n++]=y+r;
			fill_array[n++]=x-r;
			fill_array[n++]=y-r;
			fill_array[n++]=x+r;
			fill_array[n]=y-r;
			picfillo(n/2,fill_array,fillornofill);
			picfont(10);
			break;
		case '6':   fillornofill=0;    /* upside_down triangle, open */
		case '7':   fillornofill*=1;   /* upside_down triangle, filled */
			n=0;
			picfont(17+fillornofill);   /* identificatie  */
			picmove(x,y);
			picmove(r,r);

//			picmove(x-r,y+r);
			fill_array[n++]=x-r;
			fill_array[n++]=y+r;
			fill_array[n++]=x+r;
			fill_array[n++]=y+r;
			fill_array[n++]=x;
			fill_array[n]=y-r;
			picfillo(n/2,fill_array,fillornofill);
			picfont(10);
			break;
		case '8':                     /* star */
			picmove(x-r/2,y-r);
			picdraw(x+r/2,y+r);
			picmove(x+r/2,y-r);
			picdraw(x-r/2,y+r);
			picmove(x-r,y);
			picdraw(x+r,y);
			break;
		case '9':                     /* vertical line */
			picmove(x,y+r);
			picdraw(x,y-r);
			break;
	  default:                       /* ASCII-karakter */
		  buffer[1]='\0';
		  buffer[0]=t;
		  picsize(15*r, 12*r);
		  picmove(x,y);
		  pictext(0, CENTER, buffer);
		  picsize(Hsize, Vsize);
		}
}

void	picfillo(int vertices,int *data_array,int fill)
{

 int n;
	if(fill)  	fputc(FILL,fpout);
	else	 	 	fputc(FILLO,fpout);
	fputc(vertices,fpout);
	bytecounter+= 2;
	for (n=0;n<(vertices+1)*2;n++)
	 {
		 fputc(data_array[n]/256,fpout);
		 fputc(data_array[n]%256,fpout);
		 bytecounter+= 2;
		 if (ferror(fpout)) {
			cprintf("\r\nError writing PIC-file at byte %ld\r\npicfill(o): FILL(O)=%d\r\n",
				bytecounter, FILL);
		 if (getch()!='R') farewell(pic_no);
		}
	 }
}

int picy_cal(int logflag) {
	int n, ox, lx, pos, cal, logyflag, maxlen=0, mantissa, lowest_exponent, highest_exponent;
	double calibrator_value, max, min;
	#ifdef TRACE
		if (Trace==2) fprintf(tracefile,"\n%4d picy_cal min=%.4f max=%.4f",
				__LINE__, ymin, ymax);
	#endif
	logyflag= ascal(&noofcalibrators, &ymin, &ymax, logflag, TRUE);
	#ifdef TRACE
	if (Trace==2) fprintf(tracefile,"\n%4d Logflag=%d logyflag=%d picy_cal min=%.4f max=%.4f",
			 __LINE__, Logflag, logyflag, ymin, ymax);
	#endif
	min=(logyflag)?log(ymin):ymin;
	max=(logyflag)?log(ymax):ymax;
	scale_yfactor=(double)y_length/(max - min);
	for (n=0, maxlen=0;n<noofcalibrators;n++) if (maxlen<strlen(calibratorstring[n])) maxlen=strlen(calibratorstring[n]);
	x_origin+= 40 + maxlen * Hsize/2;
	x_length= (Datasymbol==-1 && Legendpos==1 && Graph_type==TTEST) ? X_length - x_origin - maxlegendsize-2*Legend_symbol_size - 3*RIM:X_length - x_origin;
	if (x_length<=100) x_length=100;
	ox= x_origin;
	lx= x_length;
   #ifdef TRACE
		if (Trace==2) fprintf( tracefile,"\n%4d min=%.4lf max=%.4lf factor=%8.4lf ox=%d lx=%d ly=%d",
            __LINE__, ymin, ymax, scale_yfactor, x_origin, x_length, y_length);
   #endif
   if (Ycal/4) {                    /* bottom line */
      picmove(ox,    y_origin);
      picdraw(ox+lx, y_origin);
   }
   if (Ycal/4==2) {                 /* top line */
      picmove(ox,    y_origin+y_length);
      picdraw(ox+lx, y_origin+y_length);
   }
   if (Ycal>9) {                    /* right line */
		picmove(ox+lx, y_origin);
      picdraw(ox+lx, y_origin+y_length);
   }
   picsize(Hsize,Vsize);
   for (cal=0; cal<noofcalibrators; cal++) {
      calibrator_value=(float)value_of_calibrator[cal];
      pos= (cal==noofcalibrators-1)?y_origin+y_length: y_origin + scale_yfactor * (calibrator_value-min);
      for (n=0;n<Plotter;n++) {
         picmove(ox,pos);
         picdraw(ox+RIM/2,pos);
         picmove(ox-Hsize/2,pos);
         pictext(0,CENTERRIGHT,calibratorstring[cal]);
         if ((Ycal%4)<2) {
            picmove(ox+lx,pos);
            picdraw(ox+lx-RIM/2,pos);
         }
      }
   }
   if (Ycal%2) {    /* tick-marks outside */
      ox+=RIM/2;
		lx-=RIM;
   }
   for (n=0;n<Plotter;n++) {    /* Y-axis line */
      picmove(ox,y_origin   );
      picdraw(ox,y_origin+y_length);
      if ((Ycal%4)<2) {
         picmove(ox+lx,y_origin   );
         picdraw(ox+lx,y_origin+y_length);
      }
   }
   if (Ycal%2) {    /* tick-marks outside */
      ox-=RIM/4;
		lx+=RIM/4;
   } else lx-=RIM/4;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d Logflag(old)=%d Logflag(new)=%d min:%8.2lf max:%8.2lf factor:%8.2lf",__FILE__, __LINE__,Logflag,logyflag, min, max, scale_yfactor);
   #endif
   if (logyflag) {
      highest_exponent=(int)(max/log(10)+0.001);
      lowest_exponent=(int)(min/log(10)-1.1);
      #ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n%4d: highest exponent:%d lowest exponent:%d",
               __LINE__, highest_exponent, lowest_exponent);
      #endif
      for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
         for (mantissa=1;mantissa<10;mantissa++) {
            calibrator_value= log(val((double)mantissa,cal));
            if (calibrator_value > min && calibrator_value < max) {
               pos= y_origin + scale_yfactor * (calibrator_value-min);
               if (pos<y_origin || pos>y_origin+ y_length) continue;
               for (n=0;n<Plotter;n++) {
                  picmove(ox,pos);
						picdraw(ox+RIM/4,pos);
                  if ((Ycal%4)<2) {
                     picmove(ox+lx,pos);
                     picdraw(ox+lx+RIM/4,pos);
                  }
               }
            }
         }
      }
   }
   x_origin+=RIM;
   x_length-=RIM;
	if (Ycal%4<2) x_length-=RIM;       /* right axis */
   return logyflag;
}

int picranges(int range_type, int testnamelines, int evaltextlines, double inter) {
/* if testnamelines==-1 no picfile, only stat-textfile */
   int t, r, n, y, y2, rangesize, hsize, legendlines, xpos, line, lines;
   char text1[MAX_LEN], text2[MAX_LEN], *bufferpointer;
   hsize=(Small)?Hsize/2:Hsize;
   if (testnamelines>-1) picsize(hsize,Vsize);
   xpos= x_origin;
   for (t=0, r=1, ranges=-1; t<nooftests; t+=r) {
      ranges++;
      r=0;
      do {
         if (t+r+1 < nooftests) {
            if (range_type==1) {
               strcpy(text1,testname[test[t+r]]);
               strcpy(text2,testname[test[t+r+1]]);
               rangetext[ranges]=testname[test[t+r]];
				} else {
               strcpy(text1,evaltext[t+r]);
               strcpy(text2,evaltext[t+r+1]);
               rangetext[ranges]=evaltext[t+r];
            }
         }
         r++;
      } while (!strcmp(text1,text2) && t+r<nooftests);
      if (t+r==nooftests) {
         if (range_type==1) strcpy(buffer, testname[test[nooftests-1]]);
         else strcpy(buffer, evaltext[nooftests-1]);
         bufferpointer=strtok(buffer,"\\");
			if (r>1 && strcmp(text1,text2)) r--;
      } else bufferpointer=strtok(text1,"\\");
      rangesize=r*inter;
      tests_per_range[ranges]=r;
      #ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n%s %4d ranges=%d rangesize=%d tests_per_range=%d text1=%s text2=%s bufferpointer=%s",
               __FILE__, __LINE__, ranges, rangesize,r, text1, text2, bufferpointer);
      #endif
      if (No_ranges || testnamelines==-1) continue;
      lines=(range_type==1)?testnamelines:evaltextlines;
      if (Swap) legendlines=0; else legendlines=(!Subselect || range_type!=1 || No_ranges) ? testnamelines: evaltextlines;
      if (!Ranges_in_header) y= bottomlines - legendlines-1;
      else                   y= headerlines - lines;
      for (n=0;n<Plotter;n++) {
          for (line=0; line<lines; line++, (!Ranges_in_header)? y--: y++) {
            #ifdef TRACE
               if (Trace==2) fprintf(tracefile,"\n%4d \"%s\" \"%s\" bottomlines=%d lines=%d y=%d xpos=%d r=%d rangesize=%d",
                     __LINE__, text1, bufferpointer, bottomlines, lines, y, xpos, r, rangesize);
            #endif
            picmove(xpos+rangesize/2,(!Ranges_in_header)? picbottomline(y):pictopline(y)-24);
				if (line==0)            pictext(0,Ranges_in_header?CENTERTOP:CENTERBOTTOM, bufferpointer);
            else if (bufferpointer) pictext(0,Ranges_in_header?CENTERTOP:CENTERBOTTOM, bufferpointer);
            bufferpointer=strtok(NULL,"\\");
         }
         if (!Ranges_in_header) {
            y2= (Swap) ? picbottomline(bottomlines-lines)-12: picbottomline(lines)-12;
            if (t+r!=nooftests) {
               picmove(xpos-RIM,             y2);
               picdraw(xpos+rangesize,       y2);
               picmove(xpos-RIM,             1);
               picdraw(xpos+rangesize,       1);
               picdraw(xpos+rangesize,       y_origin+y_length);
				} else {
               picmove(xpos-RIM,              y2);
               picdraw(x_origin+x_length+RIM, y2);
               picmove(xpos-RIM,              1);
               picdraw(x_origin+x_length+RIM, 1);
               picdraw(x_origin+x_length+RIM, y_origin);
            }
/*
            picmove(xpos+dx, t==0? y2 + RIM: y_origin+y_length);
            picdraw(xpos+dx, y2);
            picdraw(xpos-dx+rangesize,  y2);
            picdraw(xpos-dx+rangesize, t+r==nooftests?y2 + RIM: y_origin+y_length);
*/
         } else {
            y2=y_origin+y_length;
            if (t+r!=nooftests) {
               picmove(xpos-RIM,             y2);
               picdraw(xpos+rangesize,       y2);
               picmove(xpos-RIM,             12+pictopline(headerlines-lines));
               picdraw(xpos+rangesize,       12+pictopline(headerlines-lines));
					picdraw(xpos+rangesize,       y_origin);
            } else {
               picmove(xpos-RIM,             y2);
               picdraw(x_origin+x_length+RIM,y2);
               picmove(xpos-RIM,             12+pictopline(headerlines-lines));
               picdraw(x_origin+x_length+RIM,12+pictopline(headerlines-lines));
               picdraw(x_origin+x_length+RIM,y2);
            }
         }
       }
       xpos+= rangesize;
   }

/* Draw top- and bottomline */
   if (testnamelines>-1) {
      picmove(x_origin-RIM,          y_origin);
      picdraw(x_origin+x_length+RIM, y_origin);
      picmove(x_origin-RIM,          y_origin+y_length);
      picdraw(x_origin+x_length+RIM, y_origin+y_length);

      if (Ranges_in_header) {
         picmove(x_origin-RIM,          12+pictopline(headerlines-lines));
         picdraw(x_origin-RIM,          y_origin+y_length);
       } else {
         picmove(x_origin-RIM,          1);
         picdraw(x_origin-RIM,          y_origin);
       }

      picsize(Hsize,Vsize);
   }
   return ranges;
}

/* local routines */

int ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag) {
    int n, range, exp, exp0, t, t0, found, negative, precision, extra_decimal=0;
    double unit, value, tempmin, tempmax, ratio, base, lowest_calibrator;
    int calbase[4][4]=   {1 , 0, 0, 0,
                      1 , 3, 0, 0,
                      1 , 2, 5, 0,
                      10,20,25,50
                     };
    int exponent[MAXNOOFCALIBRATORS];
	 #ifdef TRACE
       if (Trace==2) fprintf(tracefile,"\n%4d: *min=%.4f *max=%.4f",
             __LINE__, *min, *max);
    #endif
    if (logflag) round(scientific2(*min, &base, &exp));
    else round(scientific2(*max, &base, &exp));
    tempmin=*min * 10 +  fabs(*min)/10000;
    tempmax=*max * 10 -  fabs(*max)/10000;
    lowest_calibrator=val(10,exp);
    for (n=0;n<MAXNOOFCALIBRATORS;n++) {
        *calibratorstring[n]='\0';
        value_of_calibrator[n]=MISSING_CALIBRATOR;
    }

    if (tempmax<=0 || tempmin<=0) logflag=FALSE;
    if (logflag) {
        if (tempmin<=0) tempmin= lowest_calibrator;
        ratio= (double)(tempmax / tempmin);
        if (ratio < 51 )       range= SMALL;
        else if (ratio < 501 ) range= MEDIUM;
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
    #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d: *min=%.4f *max=%.4f",
				__LINE__, *min, *max);
    #endif
    if (!calcflag) return logflag;

/* phase 3: print results in minimal-length strings */

    for (n=0; n< *noofcalibrators ; n++) {
        if (value_of_calibrator[n]>(*max>0?1.0001* *max:0.9999* *max)) {
           #ifdef TRACE
              if (Trace==2) fprintf(tracefile,"\ncal[%d]=%.4lf *max=%.4lf verschil:%lf",
                    n, value_of_calibrator[n], *max, value_of_calibrator[n] - *max);
           #endif
			  break;
        }
        precision= (range==LINEAR) ? (extra_decimal - exponent[n]): -exponent[n];
        if (precision<0) precision=0;
        sprintf(calibratorstring[n],"%.*lf", precision, value_of_calibrator[n]);
        #ifdef TRACE
           if (Trace==2) fprintf(tracefile, "\n%s %4d : precision=%d exponent[%d]=%d range=%d extra_decimal=%d string %d: %s",__FILE__, __LINE__, precision, n, exponent[n], range, extra_decimal, n, calibratorstring[n]);
        #endif
    }
                                                       
/* phase 4: if (logflag) transform data */

   if (logflag) for (n=0; n<*noofcalibrators; n++) value_of_calibrator[n]=log(value_of_calibrator[n]);

/* return logflag to indicate whether log-transformation was actually performed */

    return logflag;
}

double scientific2(double d, double *base, int *exp) {
	double logd;
   int neg=FALSE;
   if (d==0) {
      *base=0;
      *exp=-1;
      return *base;
   }
   if (d<0) {
      neg=1;
      d=-d;
   }
   logd=log10(d);
	*exp= (int)logd - (d<1)-1;
   *base= pow10d(logd-*exp);
   if (neg) *base=-*base;
   return *base;
}

double val(double mantissa, int exponent) {
   return mantissa*pow10(exponent);
}

int round(double d) {
    int i=(int)d;
    if (d-i<.5 ) return (int)(d+.5);
    return (int)(d+1);
}

double pow10d(double exponent) {
   /* 10^exponent, exponent is double rather than int, as required for pow() */
   return exp(exponent*log(10));
}


#include "ttest.h"
extern int  fgetline(FILE *fpin, char *s);
extern int m2v(int x, int y, int n);

#define HSPACE 16
#define VSPACE 12

/* module $tt.c */
void tt(void);
int write_ttlog(int warningflag);
void dotmatrix(void);

/*local routines: */
void write_tt(int x, int y0, int ttmin, int lower_deviation, int central_value, int upper_deviation, int t, int intra, double class_unit, int bar_size,int **class);
void showclasses(int y_origin, int tt_min);
void tt_parameters(int max_n, double *class_unit, int *intra, double *inter, int *symbol_size, int *column_width);
int frequency_per_class(void);
int classify(float d);
void draw_prob(void);

void dotmatrix(void) {
   int valid_rows, r, rr, row, lines, t, column, cal, size, x, y, y1, xpos, startpos, ypos;
   int ystart=PICTOP-40, dx, dy, oy, i, hsize, testnamelines, ranktest, found, symbolvalue, n;
   char *bufferpointer;
   float value, factor;
   xpos=startpos=X_length-Clxlength;
   if (Datasymbol==-1) Datasymbol=1;
   if (cormatrix_flag) Dotflag=TRUE;
   for (t=0, testnamelines=1; t<nooftests; t++) {
      i=count_char(testname[test[t]],'\\');
      if (i>testnamelines-1) testnamelines=i+1;
   }
   if (!cormatrix_flag) {
      for (row=valid_rows=0; row<rows; row++) {
         if (rowlabel[row]=='_' || rowlabel[row]=='=') valid_rows++;
         else if (!(symbolstart>-1 && row>=symbolstart) && rowlabel[row]) {
            for (t=found=0; t<nooftests && !found; t++) if (getdata(row,t,FALSE)>MISSING) found=TRUE;
            if (found) valid_rows++;
         }
      }
      lines=rows;
   } else {
      if (!Distance) ymax=1.0;
      else for (i=0; i<(nooftests-1)*(nooftests-2); i++) if (ymax<dist[i]) ymax=dist[i];
/*
      if (ymax>10.0) ymax=10.0;
*/
      ymin=0.0;
      Logflag=0;
      valid_rows=lines=nooftests;
      Rank_column=0;
   }
   if (Stagger || cormatrix_flag) testnamelines++;
   if (ymax<=ymin || valid_rows<1 || nooftests<1) {
      clrscr();
      cprintf("Problem encountered: ymin=%.4f ymax=%.4f valid_rows=%d nooftests=%d\r\n", ymin, ymax, valid_rows, nooftests);
      cprintf("\r\n%s %4d : ",__FILE__, __LINE__);
      if (getch()==ESC) farewell(-1);
      putch('\r');
      putch('\n');
      return;
   }
   hsize=(Small)?Hsize/2:Hsize;
   cprintf("\r\nBuilding picture ");
   fclose(fpin);
   picopen(ttpic);
   if (*First_title && *First_title!=' ') {
      picfirst_title(First_title, xpos + Clxlength/2);
      ystart=PICTOP-1.2*Vsize-60;
   }
   if (*Second_title && *Second_title!=' ') {
      picsecond_title(Second_title, xpos + Clxlength/2);
      ystart=PICTOP-2.4*Vsize-60;
   }
   ystart*=(float)(1.0-1.0/(2.0*(float)valid_rows));
   ypos=ystart;
   y_length=ystart-Vsize*(testnamelines+2)-40;
   if (Legendpos<3) y_length-=300;
   if (!cormatrix_flag) find_minmax(0);
   Logflag= ascal(&noofcalibrators, &ymin, &ymax, Logflag, TRUE);
   if (!cormatrix_flag) find_minmax(1);
   picextra_text(x_origin, y_origin, X_length, y_length);
   if (Logflag) {
      ymin=log(ymin);
      ymax=log(ymax);
   }
/*
   dx=(nooftests>1)?Clxlength/(nooftests-1):Clxlength/nooftests;
*/
   dx=(Clxlength-100)/nooftests;
   dy=(valid_rows>1)?y_length/valid_rows:y_length/2;
   size=(dx<dy && Dotflag)?dx:dy;
   if (size>220) size=220;
   if (size<21) size=21;
   if (dy<size) dy=size;
   if (dx<size && Dotflag) dx=size;
   size-=20;
   factor=(Dotflag)?
      size/sqrt(ymax-ymin):
      size/(ymax-ymin);
/*
   ranktest=(Outcome_test==-1)?test[medianrank[0]]:Outcome_test;
*/
   if (Outcome_test==-1) {
      for (i=0; i<nooftests && medianrank[i]==-1; i++)
         ;
      ranktest=test[medianrank[i]];
   } else ranktest=Outcome_test;
   for (r=0; r<lines; r++) {
      putch(r+'1');
      row=(Rank_column)?rank0[r][ranktest]:r;
      if (!cormatrix_flag) {
         if (rowlabel[row]=='=' || rowlabel[row]=='_') {
            picmove(startpos - 200, ypos);
            picdraw(startpos+nooftests*dx, ypos);
            if (Showclasses%2) {
               picmove(startpos-50, ypos+dy/2);
               picdraw(startpos+nooftests*dx, ypos+dy/2);
            }
            ypos-=dy;
            continue;
         }
         if (rowlabel[row]!='_' && rowlabel[row]!='=') {
            for (column=found=0; column<nooftests && !found; column++) if (getdata(row,column,FALSE)!=MISSING) found=TRUE;
            if (!found || row<0 || row>=rows || !rowlabel[row] || (symbolstart>-1 && row>=symbolstart)) continue;
         }
         openin("wksdata.ids");
         *buffer='\0';
         for (rr=0;rr<=row+Cutoff_flag;rr++) fgetline(fpin, buffer);
         fclose(fpin);
      } else strcpy(buffer, testname[test[r]]);
      xpos=startpos;
      if (Showclasses%2) {
         picmove(xpos-50, ypos+dy/2);
         picdraw(xpos+nooftests*dx, ypos+dy/2);
      }
      picmove(xpos-100, ypos);
      pictext(0, CENTERRIGHT, buffer);
      xpos+=dx/2;
      for (column=0; column<nooftests; column++, xpos+=dx) {
         t= (!cormatrix_flag && Rank_column && Outcome_test==-1) ? medianrank[nooftests-column-1]:column;
         if (test[t]==-1) continue;
         picmove(xpos, ypos);
         if (cormatrix_flag) {
            if (Distance) value=(row==t)?0.0:dist[m2v(row,t,nooftests)];
            else value=(row==t)?1.0:cormatrix[m2v(row,t,nooftests)];
            if (value>0) symbolvalue=Symbol_string[1]-'0';
            else if (value<=0 && value>=-1) symbolvalue=Symbol_string[0]-'0';
            else symbolvalue=Symbol_string[4]-'0';
            if (value<0) value=-value;
         } else {
            value= getdata(row,t,FALSE);
            if      (!(Ymin==MISSING || (Ymin<=0 && Logflag)) && value<Ymin) value=Ymin;
            else if (!(Ymax==MISSING || (Ymax<=0 && Logflag)) && value>Ymax) value=Ymax;
            if (Logflag) {
               if (value>0) value=log(value);
               else value=MISSING;
            }
            symbolvalue= (symbolstart>-1) ? (int) getdata(row+symbolstart,t,TRUE): Symbol_string[1]-'0';
         }
         if (value>MISSING && symbolvalue>=0 && value>=ymin && value<=ymax) {
            y1=ypos-size/2+factor*(value-ymin);
            if (Dotflag) {
               y=(Symbol_string[symbolvalue]>'1' && symbolstart==-1)?ypos-size/2+factor*sqrt(value-ymin)/2:ypos;
               picdot(
                  xpos,
                  y,
                  symbolvalue+'0',
                  factor*sqrt(value-ymin));
            } else {
               if (symbolstart==-1) {
                  for (x=xpos-8; x<xpos+8; x+=2) {
                     picmove(x, ypos-size/2);
                     picdraw(x, ypos-size/2+factor*(value-ymin));
                  }
               } else {
                  if (!(rowlabel[row+symbolstart])) break;
                  i=(Symbol_string[symbolvalue%strlen(Symbol_string)]-'0')%5;
                  switch (i) {
                  case 1:
                     picmove(xpos-dx/2+20, y1);
                     picdraw(xpos+dx/2-20, y1);
                     picmove(xpos+dx/2-20, ypos-size/2);
                     picdraw(xpos+dx/2-20, ypos-size/2+factor*(value-ymin));
                     picmove(xpos-dx/2+20, ypos-size/2);
                     picdraw(xpos-dx/2+20, ypos-size/2+factor*(value-ymin));
                     for (y=ypos-size/2; y<y1; y+=20) {
                        picmove(xpos-dx/2+20, y);
                        picdraw(xpos+dx/2-20, y);
                     }
                     break;
                  case 2:
                     picmove(xpos-dx/2+20, y1);
                     picdraw(xpos+dx/2-20, y1);
                     picmove(xpos+dx/2-20, ypos-size/2);
                     picdraw(xpos+dx/2-20, ypos-size/2+factor*(value-ymin));
                     for (x=xpos-dx/2+20; x<xpos+dx/2-20; x+=20) {
                        picmove(x, ypos-size/2);
                        picdraw(x, ypos-size/2+factor*(value-ymin));
                     }
                     break;
                  case 3:
                     picmove(xpos+dx/2-20, ypos-size/2);
                     picdraw(xpos+dx/2-20, ypos-size/2+factor*(value-ymin));
                     picmove(xpos-dx/2+20, y1);
                     picdraw(xpos+dx/2-20, y1);
                     for (y=ypos-size/2; y<y1; y+=20) {
                        picmove(xpos-dx/2+20, y);
                        picdraw(xpos+dx/2-20, y);
                     }
                     for (x=xpos-dx/2+20; x<xpos+dx/2-20; x+=20) {
                        picmove(x, ypos-size/2);
                        picdraw(x, ypos-size/2+factor*(value-ymin));
                     }
                     break;
                  case 4:
                     for (x=xpos-dx/2+20; x<xpos+dx/2-20; x+=2) {
                        picmove(x, ypos-size/2);
                        picdraw(x, ypos-size/2+factor*(value-ymin));
                     }
                     break;
                  case 0:
                  default:
                     picmove(xpos-dx/2+20, ypos-size/2);
                     picdraw(xpos-dx/2+20, ypos-size/2+factor*(value-ymin));
                     picdraw(xpos+dx/2-20, ypos-size/2+factor*(value-ymin));
                     picdraw(xpos+dx/2-20, ypos-size/2);
                     picdraw(xpos-dx/2+20, ypos-size/2);
                     break;
                  }
               }
            }
         } else if (symbolstart==-1) picdot(xpos, ypos-10, Symbol_string[4], 48);
/*
         else {
            cprintf("\r\n%s %4d : symbolvalue=%d row=%d t=%d value=%.3f, Ymax=%.3f ymin=%.3f ymax=%.3f",
               __FILE__, __LINE__, symbolvalue, row, t, (float)value, (float)Ymax, (float)ymin, (float)ymax);
            if (getch()==ESC) farewell(-1);
            putch('\r');
            putch('\n');
         }
*/
      }
      if (!Dotflag) {
         picmove(xpos - dx/2, ypos-size/2);
         picdraw(startpos-100, ypos-size/2);
      }
      ypos-=dy;
   }
   if (Showclasses%2) {
      picmove(startpos-50,            ypos+dy/2);
      picdraw(startpos+nooftests*dx, ypos+dy/2);
   }
   oy=ypos+dy/2;
   if (Dotflag && Showclasses!=1) {
      picmove(startpos, oy);
      picdraw(startpos, ystart+dy/2);
   }
   for (t=0; t<nooftests; t++) {
      if (test[t]==-1) {
         picmove(startpos+t*dx+dx/2, oy);
/*
         picdraw(startpos+t*dx+dx/2, ystart+VSIZE/2);
*/
         picdraw(startpos+t*dx+dx/2, ystart+dy/2);
      }
      if (Showclasses/2) {
         picmove(startpos+t*dx+dx, oy);
         picdraw(startpos+t*dx+dx, ystart+dy/2);
      }
   }
   xpos=startpos+dx/2;
   oy-=Vsize/2;
   picsize(hsize,Vsize);
   for (column=0; column<nooftests; column++, xpos+=dx) {
      t= (Rank_column && Outcome_test==-1)?medianrank[nooftests-column-1]:column;
      if (test[t]==-1) continue;
/*
      if (!valid_data[t]) continue;
*/
      ypos=oy;
      if ((Stagger || (cormatrix_flag && dx<400)) && column%2) ypos-=Vsize;
      picmove(xpos, ypos);
      n=3*dx/Hsize;
      if (n<strlen(testname[test[t]]) && cormatrix_flag) {
         strncpy(buffer,testname[test[t]],n);
         buffer[n]='\0';
         bufferpointer=buffer;
      } else {
         strcpy(buffer,testname[test[t]]);
         bufferpointer=strtok(buffer,"\\");
      }
      pictext(0, CENTERTOP,bufferpointer);
      if (!cormatrix_flag) {
         bufferpointer=strtok(NULL,"\\");
         while (bufferpointer!='\0') {
            ypos-=Vsize;
            picmove(xpos, ypos);
            pictext(0, CENTERTOP,bufferpointer);
            bufferpointer=strtok(NULL,"\\");
         }
      }
   }
   picsize(Hsize,Vsize);
   if (cormatrix_flag) {
      noofcalibrators=5;
      for (cal=0; cal<noofcalibrators; cal ++) sprintf(calibratorstring[cal],"%.1f",((float)cal-2.0)/2.0);
   }
   if (Legendpos<3) {
      ypos=oy-testnamelines*Vsize-size-40;
      for (cal=0, xpos=startpos; cal<noofcalibrators; cal++) {
         value=(cormatrix_flag)?
            ((float)cal-2.0)/2.0:
            (float)value_of_calibrator[cal];
/*
         if (1.1*value<ymin || 0.9*value>ymax) continue;
*/
         if (Dotflag) picdot(
            xpos,
            ypos+size/2,
            cormatrix_flag && value<0?
               Symbol_string[0]:
               Symbol_string[1],
            cormatrix_flag && value<0?
               factor*sqrt(-value-ymin):
               factor*sqrt(value-ymin));
         else {                                                                                                                
            for (i=xpos-8; i<xpos+8; i+=2) {
                picmove(i, ypos);
                picdraw(i, ypos+factor*(value-ymin));
            }
         }
         picmove(xpos, ypos-20);
         pictext(0,CENTERTOP,calibratorstring[cal]);
         xpos+=(Clxlength-size)/noofcalibrators;
      }
      picmove(startpos, ypos-Vsize-40);
      pictext(0, TOPLEFT, y_axis_title);
   } else if (Legendpos==3) {
      xpos=440+ Vsize * count_char(y_axis_title,'\\') * 1.2;
      for (cal=noofcalibrators-1, ypos=ystart; cal>=0; cal--) {
         value=(cormatrix_flag)?
                  ((float)cal-2.0)/2.0:
                  (float)value_of_calibrator[cal];
/*
         if (1.1*value<ymin || 0.9*value>ymax) continue;
*/
         if (Dotflag) picdot(
            xpos,
            ypos+size/2,
            cormatrix_flag && value<0?
               Symbol_string[0]:
               Symbol_string[1],
            cormatrix_flag && value<0?
               factor*sqrt(-value-ymin):
               factor*sqrt(value-ymin));
         else {
            for (i=0; i<16; i+=2) {
                picmove(xpos+Vsize*1.2+i, ypos);
                picdraw(xpos+Vsize*1.2+i, ypos+factor*(value-ymin));
            }
         }
/*
         picmove(xpos+(Dotflag)?size+40:60, ypos-20);
         picmove(xpos+(Dotflag)?size/2+150:150, ypos);
*/
         if (Dotflag) {
            picmove(xpos-size/2-40, ypos+size/2);
            pictext(0,CENTERRIGHT,calibratorstring[cal]);
         } else {
            picmove(xpos-40, ypos+size/2);
            pictext(0,BOTTOMRIGHT,calibratorstring[cal]);
         }
         ypos-=250;
      }
      xpos=1;
      picmove(xpos, ystart);
      strcpy(buffer,y_axis_title);
      bufferpointer=strtok(buffer,"\\");
      pictext(1, TOPRIGHT, bufferpointer);
      bufferpointer=strtok(NULL,"\\");
      while (bufferpointer!='\0') {
         xpos+=Vsize;
         picmove(xpos, ystart);
         pictext(1, TOPRIGHT, bufferpointer);
         bufferpointer=strtok(NULL,"\\");
      }
   }
   picend();
   picture_ready_flag=TRUE;
}

void tt(void) {
int y, y0, intra;
int column_width;
int r, t, n, start, max_n=0, xpos, row, cl, dx, pendown=0, datapoint, extrapoints, noofpoints;
int virgin, color, symbol, range, range_type, testnamelines, evaltextlines;
float d, d2, tempmin, tempmax;
float p10, p25, se_min, p50, se_max, p75, p90;
int *intpointer;
int **class;             /* [MAXNOOFCLASSES][MAX_COL+2] */
int **classcounter;      /* [MAXNOOFCLASSES][MAX_COL+2] */
   if ((class=        (int  **)   calloc(Noofclasses+2,  sizeof(intpointer)))==0) no_room();
   if ((classcounter= (int  **)   calloc(Noofclasses+2,  sizeof(intpointer)))==0) no_room();
   for (row=0; row<Noofclasses+2; row++) if ((class[row]=        (int  *) calloc( (nooftests+2),  sizeof(int)))==0) no_room();
   for (row=0; row<Noofclasses+2; row++) if ((classcounter[row]= (int  *) calloc( (nooftests+2),  sizeof(int)))==0) no_room();
   setdisk(startup_drive);
   if (Trace==1) {
       cprintf("\r\n%s %4d Logflag=%d ymin=%.4f\r\n",__FILE__, __LINE__, Logflag, ymin);
       if (getch()==27) exit(-1);
   }
   cprintf("\r\nWorking on test: ");
   if (calc_flag) {
      if (!Prob_flag) {
        if (Stat!=-1) {
           if (!get_mean(1)) return;
           if (nooftests==2) get_diff(0, 1);
        }
        if (!find_minmax(0)) return;
        if (Stat!=-1) adjust_minmax();
        if (ymax<=ymin) return;
      }
      ascal(&noofcalibrators, &ymin, &ymax,Logflag, 0);
      find_minmax(1);
      tt_minmax();
/*
      count_excess();
*/
      yll=yul=0; /* number of points lower or higher than limit values */
      gotoxy(1,1);
      clreol();
      for (t=0; t<nooftests;t++) {
         putch('.');
         if (Trace==1)       putch('0');
         if (test[t]==-1) continue;
         if (!N[t]) continue;
         class[0][t]=class[Noofclasses+1][t]=0;
         if (!valid_data[t]) continue;
         for (row=0;row<rows;row++) {
            if ((d=getdata( row, t, FALSE))==MISSING) continue;
            if (d<ymin || (Logflag && d<=0)) class[0][t]++;
            else if (d>ymax) class[Noofclasses+1][t]++;
         }
         if (class[0][t]) {
            yll++;
            putch('<');
         }
         if (class[Noofclasses+1][t]) {
            yul++;
            putch('>');
         }
      }
    }
/* range_type: 1 if evaltext[0]==evaltext[1] */
    bottomlines= get_bottomlines(&range_type, &testnamelines, &evaltextlines);
    headerlines= get_headerlines(range_type, testnamelines, evaltextlines);
    y_origin= picbottomline(bottomlines)+8;
    if (yll) y_origin+=24;
    else if (*extra_text[8]) y_origin+=16;
/* calculate approximate class_unit: vertical scale_units per class */
    class_unit=(pictopline(headerlines)-y_origin)/(Noofclasses-1);
    if (yll) y_origin+=class_unit;
    y_length= pictopline(headerlines)-y_origin-20;
    if (yll) y_length-=class_unit;
    if (yul) y_length-=class_unit;
    symbols=find_symbols_in_use(1);
    maxlegend= get_maxlegend();
    maxlegendsize=(Line && Subselect!=NON_PAIRED) ?2*LEGEND_SYMBOL_SIZE+XSIZE+maxlegend*((float)Hsize/1.5):maxlegend*((float)Hsize/1.5);
/*
    maxlegendsize=(Line && Subselect!=NON_PAIRED) ?2*LEGEND_SYMBOL_SIZE+XSIZE+maxlegend*Hsize/2:maxlegend*Hsize/2;
*/
#ifdef TRACE
          if (Trace==2) fprintf(tracefile, "\n%s %4d y_origin=%d y_length=%d Logflag=%d Ymax=%5.1f ymin=%.4f ymax=%.4f",
                __FILE__,__LINE__, y_origin, y_length, Logflag, Ymax, ymin, ymax);
#endif
    write_ttlog(0);
    picopen(ttpic);
    new_tests=FALSE;
    x_origin=picy_axis_title(y_axis_title, y_origin + y_length)+80;
    Logflag= picy_cal(Logflag);
    if (yll) {
       if (ymin>true_min) dotted_h_line(x_origin,x_origin+x_length,y_origin);
       ttmin=ymin;
    }
    if (yul) {
       if (ymax<true_max) dotted_h_line(x_origin,x_origin+x_length,y_origin+y_length);
       ttmax=ymax;
    }
    picextra_text(x_origin, y_origin, x_length, y_length);
    tempmin=ymin;
    tempmax=ymax;
    if (Logflag) {
       ttmin=log(ttmin);
       ttmax=log(ttmax);
       ymin=log(ymin);
       ymax=log(ymax);
    }
    y0=(ymin <= 0) ? y_origin - ymin * scale_yfactor: y_origin;
    interval=(ttmax - ttmin) / (Noofclasses-1);
/*
    max_n=frequency_per_class();
*/
   cprintf("\r\ncalculating distribution: ");
   for (t=0;t<nooftests;t++) {
      for (cl=0;cl<Noofclasses+2;cl++) class[cl][t]=0;
      if (!valid_data[t]) continue;
      cprintf("%s ",numbertolotus(test[t]+1,"  "));
      for (row=0;row<rows;row++) {
         d=getdata(row, t, FALSE);
         if (d>MISSING) class[classify(d)][t]++;
      }
   }
   datapoint=0;
   if (Median_flag==2) for (t=0,max_n=0;t<nooftests;t++) {
      if (!N[t]) continue;
      extrapoints=(N[t]+5)/10;
      for (cl=0;datapoint<extrapoints && cl<Noofclasses+2;cl++) {
         noofpoints=(datapoint+class[cl][t]<extrapoints)?class[cl][t]:extrapoints-datapoint;
         if (noofpoints<max_n) max_n=noofpoints;
         datapoint+=class[cl][t];
      }
      datapoint=0;
      for (cl=Noofclasses+1,y=y_origin+ttmax;datapoint<extrapoints && cl>0;cl--, y-=class_unit) {
         noofpoints=(datapoint+class[cl][t]<extrapoints)?class[cl][t]:extrapoints-datapoint;
         if (noofpoints<max_n) max_n=noofpoints;
         datapoint+=class[cl][t];
      }
   } else for (t=0,max_n=0;t<nooftests;t++) {
      if (!N[t]) continue;
      for (cl=0;cl<Noofclasses+2;cl++) if (max_n<class[cl][t]) max_n=class[cl][t];
   }
   fflush(stdin);
    tt_parameters(max_n, &class_unit, &intra, &inter, &symbol_size, &column_width);
#ifdef TRACE
    if (Trace==2) {
       fprintf(tracefile,"\n%s %4d:\n"
       "interval=%.3lf\n"
       "class_unit=%.3lf\n"
        "intra=%d\n"
        "inter=%.3lf\n"
        "symbol_size=%d\n"
        "column_width=%d\n",
        __FILE__, __LINE__,
        interval,
        class_unit,
        intra,
        inter,
        symbol_size,
        column_width);
     }
#endif
     if (range_type) ranges=picranges(range_type, testnamelines, evaltextlines, inter);
     if (*First_title) for (n=0;n<Plotter;n++) picfirst_title(First_title, x_origin + x_length/2);
     if (*Second_title) for (n=0;n<Plotter;n++) picsecond_title(Second_title, x_origin + x_length/2);
/*
     if (Showclasses) showclasses(y_origin, (int)((ttmin-ymin) * scale_yfactor));
*/
     cprintf("\r\nwriting test ");
     for (t=0;t<nooftests;t++) {
/*
        if (kbhit) {
           fputc(END,fpout);
           fclose(fpout);
           fflush(stdin);
           return;
        }
*/
        xpos= x_origin + inter/2 + t*inter;
        if (!valid_data[t]) continue;
        cprintf("%s ",numbertolotus(test[t]+1,"  "));
        find_values(t, Logflag);
        #ifdef TRACE
           if (Trace==2) fprintf(tracefile,"\n%4d %s: N[%2d]=%2d ttmin=%.3f lower_deviation=%.3f central_value=%.3f upper_deviation=%.3f",
                 __LINE__, __FILE__, t, N[t], ttmin, lower_deviation, central_value, upper_deviation);
        #endif
        write_tt(
           xpos,
           y0,
           ( ttmin           - ymin ) * scale_yfactor,
           ( lower_deviation - ymin ) * scale_yfactor,
           ( central_value   - ymin ) * scale_yfactor,
           ( upper_deviation - ymin ) * scale_yfactor,
           t,
           intra,
           class_unit,
           column_width,
           class);
        if (filltype[t]!=-1) {
           d=(fill_min[t]==MISSING)?ymin:(Logflag?log(fill_min[t]):fill_min[t]);
           d2=(fill_max[t]==MISSING)?ymax:(Logflag?log(fill_max[t]):fill_max[t]);
           picfill(
                    xpos-column_width/2,
                    y_origin+ (d - ymin) * scale_yfactor,
                    xpos+column_width/2,
                    y_origin+ (d2 - ymin ) * scale_yfactor,
                    filltype[t]);
        }
        if (Median_flag==2) {
            start=(Outliers_flag)?0:class[0][t];
            n=(Outliers_flag)?valid_data[t]:N[t];
            p10=get_ile(start, t, 0.10, FALSE);
            p25=get_ile(start, t, 0.25, FALSE);
            se_min=get_ile(start, t, ilevalue( n, 0.25, 0.50), FALSE);
            p50=get_ile(start, t, 0.50, FALSE);
            se_max=get_ile(start, t, ilevalue( n, 0.75, 0.50), FALSE);
            p75=get_ile(start, t, 0.75, FALSE);
            p90=get_ile(start, t, 0.90, FALSE);
            if (n<3) p10=p25=se_min=se_max=p75=p90=p50;
            if (Logflag) {
               p10=    p10>0     ?log(p10)   :p25>0?log(p25):log(p50);
               p25=    p25>0     ?log(p25)   :log(p50);
               se_min= (se_min>0)?log(se_min):log(p50);
               p50=    log(p50);
               se_max= se_max>0  ?log(se_max):log(p50);
               p75=    p75>0     ?log(p75)   :log(p50);
               p90=    p90>0     ?log(p90)   :log(p50);
            }
            write_box(
               xpos,
               y_origin,
               (p10    - ymin) * scale_yfactor,
               (p25    - ymin) * scale_yfactor,
               (se_min - ymin) * scale_yfactor,
               (p50    - ymin) * scale_yfactor,
               (se_max - ymin) * scale_yfactor,
               (p75    - ymin) * scale_yfactor,
               (p90    - ymin) * scale_yfactor,
               column_width,
               0);
        }
        piccolumnlegend(t, xpos, range_type, testnamelines, evaltextlines);
        if (Cutoff_flag==1) {
           d=cutoff_value[test[t]];
           if (Logflag && d>0) d=log(d);
           if (d>MISSING) dotted_h_line(xpos-column_width/2, xpos+column_width/2, y_origin+(d-ymin)*scale_yfactor);
        }
     }
     if (filltype[FILL_COL]!=-1) {
        d=(fill_min[FILL_COL]==MISSING)?ymin:(Logflag?log(fill_min[FILL_COL]):fill_min[FILL_COL]);
        d2=(fill_max[FILL_COL]==MISSING)?ymax:(Logflag?log(fill_max[FILL_COL]):fill_max[FILL_COL]);
        picfill(
                 x_origin-RIM/2,
                 y_origin+ (d - ymin) * scale_yfactor,
                 x_origin + x_length+RIM/2,
                 y_origin+ (d2 - ymin ) * scale_yfactor,
                 filltype[FILL_COL]);
     }
     if (Datasymbol==-1) piclegend(Legend);
     if (Line  && Subselect!=NON_PAIRED && nooftests>1) {
        for (cl=0;cl<Noofclasses+2;cl++) for (t=0;t<nooftests;t++) classcounter[cl][t]=0;
        for (row=0, virgin=TRUE;row<rows;row++) {
           pendown=FALSE;
           for (t=0, range=0, r=0;t<nooftests;t++) {
                 if (range_type==2 && t-r==tests_per_range[range]) {
                 pendown=FALSE;
                 range++;
                 r=t;
              }
              if (!valid_data[t]) {
                 pendown=FALSE;
                 continue;
              }
              d= getdata(row, t,FALSE);
              #ifdef TRACE
                 if (Trace==2) fprintf(tracefile,"\n%4d t=%2d range_type=%d r=%2d range=%2d tests_per_range=%d d=%.3f",
                       __LINE__, t, range_type, r, range, tests_per_range[range], d);
              #endif
              if (d>MISSING) {
                 cl=classify(d);
                 dx=find_dx(classcounter[cl][t],class[cl][t],intra);
                 #ifdef TRACE
                    if (Trace==2) fprintf(tracefile,"\n%4d:d=%.3f cl=%d dx=%d class=%d classcounter=%d",
                       __LINE__, d, cl, dx, class[cl][t], classcounter[cl][t]);
                 #endif
                 if (virgin || !pendown) {
                    picmove( x_origin + inter/2 + t*inter + dx, y_origin + (ttmin-ymin)*scale_yfactor + (cl-1) * class_unit);
                    if (Color>1 && Datasymbol==-1) {
                       symbol= rowlabel[casnum[row][t]]-'0';
                       if (symbol >=0 ) color=Symbol_color[symbol%strlen(Symbol_color)];
                       else color=0;
                       #ifdef TRACE
                       if (Trace==2) fprintf(tracefile,"\n%4d row=%d t=%d symbol=%d color=%d",
                             __LINE__, row, t, symbol, color);
                       #endif
                       piccolor(color);
                    }
                    pendown=TRUE;
                    virgin=FALSE;
                 }
                 else picdraw(x_origin + inter/2 + t*inter + dx, y_origin + (ttmin-ymin)*scale_yfactor + (cl-1) * class_unit);
                 (classcounter[cl][t])++;
              }
           }
        }
     }
     if (Prob_flag) draw_prob();
     picend();
/*
     if (fclose(fpout)==EOF) {
       cprintf("\r\nError writing file \"%s\" to disc: disc full?",ttpic)
       if (getch()==ESC) farewell(pic_no);
     }
*/
     fflush(stdin);
     picture_ready_flag=TRUE;
     ttmin=ymin=tempmin;
     ttmax=ymax=tempmax;
     for (row=0; row<Noofclasses+2; row++) free(classcounter[Noofclasses+1-row]);
     for (row=0; row<Noofclasses+2; row++) free(class[Noofclasses+1-row]);
     free(classcounter);
     free(class);
}

void draw_prob(void) {
   int i, xpos, textlen, xpos1, xpos2, ypos;
clrscr();
   for (i=0; i<PSTRING; i++) {
      if (pstart[i]==-1 || pend[i]==-1 || pstring[i][0]=='\0' || pvalue[i]==MISSING) continue;
      textlen=Hsize*strlen(pstring[i])/2.5;
      if (textlen<inter) {
        xpos1= x_origin + inter/2 + pstart[i]*inter+20;
        xpos2= x_origin + inter/2 + pend[i]  *inter-20;
        xpos=(xpos1+xpos2)/2;
        ypos=(Logflag && pvalue>0)?
           y_origin+(log(pvalue[i])-ymin)*scale_yfactor:
           y_origin+(pvalue[i]-ymin)*scale_yfactor;
        picmove(xpos1,ypos-48);
        picdraw(xpos1,ypos);
        picdraw(xpos-textlen/2-24,ypos);
        picmove(xpos+textlen/2+24,ypos);
        picdraw(xpos2,ypos);
        picdraw(xpos2,ypos-48);
      }
/*
      picmove((xpos1+xpos2)/2,ypos+32);
*/
      picmove(xpos,ypos+12);
      pictext(0, CENTER, pstring[i]);
   }
}

void showclasses(int y_origin, int ttmin) {
   int cl,y;
#ifdef TRACE
   if (Trace==2) fprintf(tracefile, "\n%s %4d showclasses; ttmin=%d; class_unit=%.2f",__FILE__, __LINE__,ttmin, class_unit);
#endif
   for (cl=0; cl<Noofclasses+2;cl++) {
      y=y_origin + ttmin - class_unit/2 + cl * class_unit;
      picmove(x_origin          ,y);
      picdraw(x_origin+x_length ,y);
   }
}

void tt_parameters(int max_n, double *class_unit, int *intra, double *inter, int *symbol_size, int *column_width) {
   int size1, size2;
   *class_unit=   (ttmax-ttmin)*scale_yfactor/(Noofclasses-1);
   *inter=        (Inter_t<0)? INTER_T : Inter_t;
   *column_width= x_length/((1.0 + *inter) * nooftests);
   *inter=        (1.0 + *inter)* *column_width;
   *intra=        (max_n>0 && Intra==-1)?*column_width/max_n:Intra;
   if (*intra<0)  *intra=0;
   if (max_n<1)   *intra=(Intra==-1)?*class_unit-VSPACE:Intra;
/*
   *intra= (Intra==-1)?INTRA:Intra;
   *intra= (Intra==-1)?(*class_unit)/2:Intra;
   *intra= (max_n>1 && Intra==-1)?*column_width/(max_n-1):Intra;
   x=(max_n>0)?*column_width/max_n:*column_width;
*/
   if (Symbol_size<0) {
      size1=(*intra>4+HSPACE)?*intra-4:4;
      if (size1>*class_unit-VSPACE) size1=*class_unit-VSPACE;
      size2=*class_unit-VSPACE;
      if (size2>*intra-HSPACE) size2=*intra-HSPACE;
      *symbol_size=(size1>size2)?size1:size2;
   } else *symbol_size=Symbol_size;
   if (Intra==-1) *intra=*symbol_size+HSPACE;
}

int classify(float d) {
    int classification;
#ifdef TRACE
    if (Trace==2) fprintf(tracefile,"\n%s %4d:%12.3f class ",__FILE__, __LINE__,d);
#endif
    if(Logflag) {
       if (d<=0) return 0;
       else d=log(d);
    }
    if (Ymax!=MISSING && d>ymax) classification= Noofclasses+1;
    else if (d<ttmin) classification= 0;
    else classification= (int)(0.5001+(d-ttmin)/interval+1);
    if (classification<0 || classification>MAXNOOFCLASSES+1) {
       clrscr();
       cprintf("classification error: class %d found\r\n",classification);
       farewell(-1);
    }
#ifdef TRACE
    if (Trace==2) fprintf(tracefile,"%2d",classification);
#endif
    return classification;
}

void write_tt(
   int x,               /* horizontal position (screen-units) */
   int y0,
   int ttmin,           /* lowest class in screen_units */
   int lower_deviation,
   int central_value,
   int upper_deviation,
   int t,               /* test-number in worksheet */
   int intra,           /* screen_units between points in string */
   double class_unit,   /* screen-units between units */
   int column_width,
   int **class)
{
   int n, cl, y, datapoint, extrapoints, noofpoints;
/* draw individual data_points in graph: */
   datapoint=0;
   n=(Outliers_flag)?valid_data[t]:N[t];
   if (Median_flag==2) {
      extrapoints=(n+4.99)/10;
      for (cl=0,y=y_origin+ttmin;datapoint<extrapoints && cl<Noofclasses+2;cl++, y+=class_unit) {
         noofpoints=(datapoint+class[cl][t]<extrapoints)?class[cl][t]:extrapoints-datapoint;
         picstring( datapoint, t, x, y_origin +ttmin+(cl-1)*class_unit, noofpoints, intra, symbol_size);
         datapoint+=class[cl][t];
      }
      datapoint=0;
      for (cl=Noofclasses+1,y=y_origin+ttmax;datapoint<extrapoints && cl>0;cl--, y-=class_unit) {
         noofpoints=(datapoint+class[cl][t]<extrapoints)?class[cl][t]:extrapoints-datapoint;
         picstring( n-datapoint-noofpoints, t, x, y_origin +ttmin+(cl-1)*class_unit, noofpoints, intra, symbol_size);
         datapoint+=class[cl][t];
      }
   } else for (cl=0,y=y_origin+ttmin;cl<Noofclasses+2;cl++, y+=class_unit) {
      datapoint=picstring( datapoint, t, x, y_origin +ttmin+(cl-1)*class_unit, class[cl][t], intra, symbol_size);
   }
   if (Color>1) piccolor(0);
/* draw mean/median line in graph: */
   if (Stat>0 && Median_flag!=2) {
         write_bar(
            x,
            y_origin,
            y0,
            lower_deviation,
            central_value  ,
            upper_deviation,
            column_width,
            Stat,
            Tip * symbol_size,  /* tip-size */
            (Line && Graph_type != TTEST)?0:1,
            FALSE,
            TRUE, 
            0,
            0
         );
   }
   fflush(stdin);
}

int write_ttlog(int warningflag) {
int i, t, r, r0, df, start, n, pos;
float p25, p75, se_low, se_high;
char temptext[81];
double tval, p_value, diff, sem_diff, pooled_variance, pooled_sem, total_range, upperrange, lowerrange;
   /**** write results   to PRN-file ****/
         open_logfile(ttlog);
         if (!get_mean(warningflag)) {
cprintf("\r\n%s %4d : logfile to be closed prematurely",__FILE__, __LINE__);
if (getch()==27) farewell(-1);
putch('\r');
putch('\n');
     if (ferror(logfile)) {
        clearerr(logfile);
        clrscr();
        cprintf("Error writing %s\nDisc full?\n\nPress Esc to quit\r\nAny other key to continue ", logfile);
        if (getch()==ESC) farewell(pic_no);
     }
            fclose(logfile);
            return 0;
         }
         if (nooftests==1 && valid_data[test[0]]>1) {
            df=valid_data[0]-1;
            if (df>0) {
               total_range= t95(df) * sqrt(var[0]/valid_data[0]);
               if (Logflag) {
                  total_range= total_range>0?exp(total_range):1;
                  upperrange=mean[0] * total_range;
                  lowerrange=mean[0] / total_range;
               } else {
                  upperrange=mean[0] + total_range;
                  lowerrange=mean[0] - total_range;
               }
                fprintf(logfile,"\\\"95%% confidence interval of mean value:\"\n\\%.2lf  %.2lf\n",lowerrange,upperrange);
            }
         }
         if (nooftests==2) {
             get_diff(0, 1);
             diff=(Logflag)? mean[1]/mean[0]: mean[1]-mean[0];
             df=valid_data[0]+valid_data[1]-2;
             if (df>0) {
                pooled_variance=(var[0]*(valid_data[0]-1)+var[1]*(valid_data[1]-1))/df;
                pooled_sem=sqrt(pooled_variance*(1.0/valid_data[0]+1.0/valid_data[1]));
                if (diff==0) tval=0;
                else tval=(pooled_sem>0)?(Logflag?log(diff):diff)/pooled_sem:MISSING;
                p_value=find_prob(tval,df);
                total_range=pooled_sem*t95(df);
                if (Logflag) total_range=(total_range>0)?exp(total_range):1;
                upperrange=(Logflag)? diff * total_range: diff + total_range;
                lowerrange=(Logflag)? diff / total_range: diff - total_range;
                fprintf(logfile,"\\\"Non-Paired t-test\"\n");
                fprintf(logfile, "\\               \"t=\" %.4lf\n",tval);
                fprintf(logfile, "\\              \"df=\" %d\n",df);
                fprintf(logfile, "\\    \"one-tailed p=\" ");
                if (p_value<0.001)      fprintf(logfile,"\"<0.001\"\n");
                else if (p_value>0.2)   fprintf(logfile,"\">0.2\"\n");
                else                    fprintf(logfile,"%.4lf\n", p_value);
                fprintf(logfile, "\\    \"two-tailed p=\" ");
/*
                if (p_value<0.0005)     strcpy(pstring[0],"p<0.001");
                else if (p_value>0.1)   strcpy(pstring[0],"p>0.2");
                else                    sprintf(pstring[0],"p=%.4lf", 2*p_value);
                fprintf(logfile,"\"%s\"",pstring[0]+2);
*/
                if (p_value<0.0005)     fprintf(logfile,"\"<0.001\"\n");
                else if (p_value>0.1)   fprintf(logfile,"\">0.2\"\n");
                else                    fprintf(logfile,"%.4lf\n", 2*p_value);
/*
                fprintf(logfile,"\\\"Non-paired t-test\"\"t=\"%.4lf \"df=\"%d\n"
                                "\\\"one-tailed\"\"two-tailed\"\n",tval, df);
                if (p_value<0.0005)     fprintf(logfile,"\\\"<0.001\"    \"<0.001\"\n");
                else if (p_value<0.001) fprintf(logfile,"\\\"<0.001\"   %.4lf\n",p_value);
                else if (p_value>0.2)   fprintf(logfile,"\\\">0.1\"      \">0.2\n");
                else                    fprintf(logfile,"\\\  %.4lf        %.4lf\n", p_value, 2*p_value);
*/
                if (Logflag) {
                   fprintf(logfile,"\\\"Ratio of the two geometric means:\"%.4lf\n",diff);
                   fprintf(logfile,"\\\"95%% confidence interval of this ratio:\"%.4lf %.4lf\n",lowerrange, upperrange);
                } else {
                   fprintf(logfile,"\\\"Difference between the two means:\"%.4lf\n",diff);
                   fprintf(logfile,"\\\"95%% confidence interval of this difference:\"%.4lf %.4lf\n",lowerrange, upperrange);
                }
             }
             df=n_diff-1;
             if (df>0 && n_diff>1) {
                if (mean_diff==0) tval=0;
                else {
                   sem_diff=sqrt(var_diff/n_diff);
                   tval=(sem_diff>0)?(Logflag?log(mean_diff):mean_diff)/sem_diff:MISSING;
                }
                p_value=find_prob(tval,df);
                total_range= t95(df) * sqrt(var_diff/n_diff);
                if (Logflag) {
                   total_range= (total_range>0) ?exp(total_range):1;
                   upperrange=mean_diff * total_range;
                   lowerrange=mean_diff / total_range;
                } else {
                   upperrange=mean_diff + total_range;
                   lowerrange=mean_diff - total_range;
                }
                fprintf(logfile,"\\=======================================================================\n\r");
                fprintf(logfile,"\\\"Paired t-test    \"\n");
                fprintf(logfile,"\\               \"t=\" %.4lf\n",tval);
                fprintf(logfile,"\\              \"df=\" %d\n",df);
                fprintf(logfile,"\\    \"one-tailed p=\" ");
                if (p_value<0.001)      fprintf(logfile,"\"<0.001\"\n");
                else if (p_value>0.2)   fprintf(logfile,"\">0.2\"\n");
                else                    fprintf(logfile,"%.4lf\n", p_value);
                fprintf(logfile,"\\    \"two-tailed p=\" ");
                if (p_value<0.0005) fprintf(    logfile,"\"<0.001\"\n");
                else if (p_value<0.001) fprintf(logfile,"\"%.4lf\n",2*p_value);
                else if (p_value>0.1)   fprintf(logfile,"\">0.2\"\n");
                else                    fprintf(logfile,"%.4lf\n", 2*p_value);
                if (Logflag) {
                fprintf(logfile,"\\      \"Mean ratio=\" %.4lf\n"
                             "\\\"95%% confidence interval of the mean value of the %d ratios:\"%.4lf  %.4lf\n",
                             mean_diff, n_diff, lowerrange, upperrange);
                } else {
                   fprintf(logfile,"\\ \"Mean difference=\" %.4lf\n"
                             "\\\"95%% confidence interval of mean value of the %d differences:\"%.4lf  %.4lf\n",
                             mean_diff, n_diff, lowerrange, upperrange);
                }
             }
         }
         fprintf(logfile,"\\=======================================================================\n\r");
         fprintf(logfile, "\\\"statistic: %s;  log-transformation: %s\"\n",
            range_string, Logflag?"YES":"NO");
         if (Mean_flag && Prob_flag) fprintf(logfile, "\\\"%s\"\n",statmode_string[Mean_flag-1]);
         if (*First_title) fprintf(logfile, "%s\n", quoted(buffer,First_title));
         if (*Second_title) fprintf(logfile, "%s\n", quoted(buffer,Second_title));
         if (*y_axis_title) fprintf(logfile, "%s\n", quoted(buffer,y_axis_title));
         else               fprintf(logfile, "\"\"");
         if (Median_flag==2) fprintf(logfile,"\"test\"\"N\"\"10-ile\"\"25-ile\"\"50-ile - SE\"\"50-ile\"\"50-ile + SE\"\"75-ile\"\"90-ile\"\n");
         if (Subselect && rangetext[0] && *rangetext[0]) fprintf(logfile, "%s\n", quoted(buffer,rangetext[0]));
         for (t=0,r=0,r0=0;t<nooftests;t++,r0++) {
            if (!valid_data[t]) {
               r++;
               r0=0;
               continue;
            }
            if (Subselect && r0==tests_per_range[r]) {
               r++;
               if (rangetext[r] && *rangetext[r]) fprintf(logfile, "%s\n", quoted(buffer,rangetext[r]));
               r0=0;
            }
            for (i=0;i<30;i++) buffer[i]=' ';
            buffer[31]='\0';
            buffer[30]='\"';
            if (Subselect) strcpy(temptext,columntext[t]);
            else strcpy(temptext, testname[test[t]]);
            i=strlen(temptext)>29?28:strlen(temptext)-1;
            for (pos=29;i>-1; i--, pos--) buffer[pos]=temptext[i];
            buffer[pos]='\"';
            if (Median_flag==2) {
/*
                 start=(Outliers_flag)?0:class[0][t];
*/
                 start=0;
                 n=(Outliers_flag)?valid_data[t]:N[t];
                 p25=get_ile(start, t, 0.25, FALSE);
                 p75=get_ile(start, t, 0.75, FALSE);
                 se_low=get_ile(start, t, ilevalue( n, 0.25, 0.50), FALSE);
                 se_high=get_ile(start, t, ilevalue( n, 0.75, 0.50), FALSE);
                 fprintf(logfile,"%s %3d %12.4lf %12.4lf %12.4lf %12.4lf %12.4lf %12.4lf %12.4lf\n",
                    buffer,
                    n,
                    get_ile(start, t, 0.10, FALSE),
                    p25,
                    se_low,
                    get_ile(start, t, 0.50, FALSE),
                    se_high,
                    p75,
                    get_ile(start, t, 0.90, FALSE));
            } else {
               find_values(t, 0);
               fprintf(logfile,"%s %3d %12.4lf %12.4lf %12.4lf\n",buffer, valid_data[t], lower_deviation, central_value, upper_deviation);
            }
    }
    if (ferror(logfile)) {
        clearerr(logfile);
        clrscr();
        cprintf("Error writing %s\nDisc full?\n\nPress Esc to quit\r\nAny other key to continue ", logfile);
        if (getch()==ESC) farewell(pic_no);
    }
    fclose(logfile);
    return 1;
}

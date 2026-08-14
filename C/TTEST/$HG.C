#include "ttest.h"
/* module $hg.c */
void hg(int graph_type);
void ogive(void);
void write_bar(int x, int y, int y0, int lower_deviation, int central_value, int upper_deviation, int x_unit, float cap, int tip, int bar, int combi, int tt, int valid_data, int zeroflag);
void write_box(int xpos,int y_origin,int p10,int p25,int se_min,int median,int se_max,int p75, int p90,int x_unit, int valid_data);


/* local routines: */
float  cut_off(char *cut_off_string, int t, int flag);
int    warning(int column1, int column2, int pospos, int posneg, int negpos,int negneg);

void hg(int graph_type) {
 int i, r, r0, t, y, y0, xpos, key, logflag1;
 int row, pen_up=0, repeat=TRUE;
 int column_width, pos;
 int code, n, start;
 int range, range_type, testnamelines, evaltextlines;
 char temptext[MAX_LEN+1];
 float d1, d2, p25, p75, se_low, se_high, tempmin, tempmax;
 double ci, m, cut_off1, cut_off2, root1, root2;
 double positive, pospos, negpos, posneg, negneg, se;
     logflag1=Logflag;
     write_stat_strings();
/*
     if (Graph_type==BAR_GRAPH && Prob_flag) write_prob_strings(FALSE);
*/
     switch (graph_type) {
        case BAR_GRAPH:
        case PERCENTAGE_POSITIVE:
/*
           logflag=Logflag;
*/
           break;
        case PEARSON:
           Logflag=0;
           break;
        case RELATIVE_RISK:
        case ODDS_RATIO:
           Logflag=1;
           break;
    }
    if (calc_flag && (graph_type == BAR_GRAPH || graph_type == PEARSON)) {
         if (!Prob_flag) {
            if (!get_mean(0)) return;
            if (nooftests==2) get_diff(0,1);
            if (!find_minmax(0)) return;
            if (Stat!=-1) adjust_minmax();
            if (ymax<=ymin) return;
         }
         ascal(&noofcalibrators, &ymin, &ymax,Logflag, 0);
         find_minmax(1);
     }
     bottomlines= get_bottomlines(&range_type, &testnamelines, &evaltextlines);
     headerlines= get_headerlines(range_type, testnamelines, evaltextlines);
     y_origin= picbottomline(bottomlines)+8;
     if (*extra_text[8]) y_origin+=16;
     y_length= pictopline(headerlines)-y_origin-20;
     if (Nn && !Subselect) y_length-=VSIZE;
     maxlegend = get_maxlegend();
     maxlegendsize=Line?2*Legend_symbol_size+XSIZE+maxlegend*((float)HSIZE/1.5):maxlegend*((float)HSIZE/1.5);
/*
     maxlegendsize=Line?2*Legend_symbol_size+XSIZE+maxlegend*HSIZE/2:maxlegend*HSIZE/2;
*/
     while (repeat) {
        switch (graph_type) {
        case BAR_GRAPH:
           if (Stat==-1) {
              cprintf("\r\nNo statistics defined!");
              cprintf("\r\n%4d %s:\r\nPress any key",__LINE__, __FILE__);
              if (getch()==27) farewell(-1);
              return;
           }
           repeat=FALSE;
           if (!Prob_flag) {
              find_minmax(0);
              if (Stat != -1) {
                 cprintf("\r\nCalculating statistics: ");
                 if (!get_mean(1)) return;
              }
              find_minmax(1);
           }
           break;
        case PERCENTAGE_POSITIVE:
           repeat=(Cutoff_flag==1)?FALSE:TRUE;
           Sd_flag=TRUE;
           ci=(Sem_flag)?1.0:1.96;
           ymin=ymax=0;
           if (Cutoff_flag!=1) {
              cprintf("\r\nNB: values <= cut-off value will be counted as negative\r\n\r\n");
              cprintf("Enter cut_off value (%g): ", Cut_off);
              gets(buffer);
              if (strlen(buffer)) Cut_off=atof(buffer);
           }
           write_stat_strings();
           open_logfile(ttlog);   /*** open 1 log ***/
           fprintf(logfile,"\\\"filename:\"\"%s\"\n",filename);
           if (Cutoff_flag!=1) fprintf(logfile,"\\\"cutoff:\"\" %g\"\n",Cut_off);
           else                fprintf(logfile,"\\\"cutoff: AUTO\n");
           if (Subselect) {
              for (t=0;t<nooftests;t++) {
                 fprintf(logfile,"\\%-20s ", quoted(buffer,testname[test[t]]));
                 fprintf(logfile,"%-20s\n", quoted(buffer,evaltext[t]));
              }
            }
            if (*First_title) fprintf(logfile, "%s\n", quoted(buffer,First_title));
            if (*Second_title) fprintf(logfile, "%s\n", quoted(buffer,Second_title));
            fprintf(logfile, "%s\n", quoted(buffer,y_axis_title));
            fprintf(logfile,"\\\"testname\"\"N\"\"negative\"\"positive\"\"lower limit\"\"percentage\"\"upper limit\"\n");
            cprintf("\r\nCalculating frequencies: ");
            for (t=0;t<nooftests;t++) {
              if (Cutoff_flag==1) Cut_off=cutoff_value[test[t]];
              if ((N[t]=valid_data[t])==0) continue;
              cprintf("%s ", numbertolotus(test[t]+1,"  "));
              for (row=positive=0;row<rows;row++) if (getdata( row, t, FALSE) > Cut_off) positive++;
              m=positive/N[t];
              mean[t]= m*100;
              root1=ci*ci-(2.0+1.0/N[t])+4.0*m*(N[t]*(1.0-m)+1.0);
              root2=ci*ci+(2.0-1.0/N[t])+4.0*m*(N[t]*(1.0-m)-1.0);
              lower_limit[t]=(mean[t]>2 && root1>0)? ((2.0*N[t]*m+ci*ci-1.0)-ci*sqrt(root1))/(0.02*(N[t]+ci*ci)):0;
              upper_limit[t]=(mean[t]<98 && root2>0)?((2.0*N[t]*m+ci*ci+1.0)+ci*sqrt(root2))/(0.02*(N[t]+ci*ci)):100;
              fprintf(logfile,"%-20s %3d %3.0lf %3.0lf %12.3lf %12.3lf %12.3lf\n",
                 quoted(buffer,testname[test[t]]), N[t],N[t]-positive,positive,lower_limit[t],mean[t], upper_limit[t]);
              if (ymax<upper_limit[t]) ymax=upper_limit[t];
           }
           fclose(logfile);   /*** close 1 log ***/
           break;
        case PEARSON:
           repeat=FALSE;
           Sd_flag=TRUE;
           ci=(Sem_flag)?1.0:1.96;
           pearson(logflag1,Logflag2);
           break;
        case SPEARMAN:
           repeat=FALSE;
           Sd_flag=TRUE;
           ci=(Sem_flag)?1.0:1.96;
           spearman();
          break;
        case RELATIVE_RISK:
        case ODDS_RATIO:
           repeat=TRUE;
           Sd_flag=TRUE;
           ci=(Sem_flag)?1.0:1.96;
           ymin=1;
           ymax=MISSING;
           clrscr();
           cprintf("NB: values <= cut-off value will be counted as negative\r\n\r\n");
           cprintf("Enter cut_off value for antecedent factor%c:\r\n", ((nooftests>1)?'s':' '));
           if (Cutoff_flag) cprintf("A = AUTO (based on cut-off value in worksheet)\r\n");
           cprintf("M = median\r\n"
           "T = upper tertile\r\n"
           "Q = upper quartile\r\n"
           "P = upper pentile (=quintile)\r\n"
           "D = upper decile\r\n"
           "default value: %s: ",Cut_off1);
           keycopy(buffer,Cut_off1,14);
           if (strlen(buffer)) strncpy(Cut_off1,buffer,9);
           Cut_off1[0]=toupper(Cut_off1[0]);
           if (Outcome_test!=-1) {
              cprintf("\r\n\r\nEnter cut_off value for outcome event %s:\r\n", testname[Outcome_test]);
              if (Cutoff_flag) printf("A = AUTO (based on cut-off value in worksheet)\r\n");
              printf("M = median\r\n"
                     "T = upper tertile\r\n"
                     "Q = upper quartile\r\n"
                     "P = upper pentile (=quintile)\r\n"
                     "D = upper decile\r\n"
                     "default value: %s: ", Cut_off2);
              keycopy(buffer, Cut_off2, 14);
              if (strlen(buffer)) strncpy(Cut_off2,buffer,9);
              else return;
              Cut_off2[0]=toupper(Cut_off2[0]);
              cut_off2=cut_off(Cut_off2,Outcome_test,TRUE);
              if (cut_off2==MISSING) {
                 if (Cutoff_flag) cut_off2=cutoff_value[Outcome_test];
                 else cut_off2=0;
              }
           } else {
              cprintf("Outcome test based on codes in column A in worksheet:\r\n"
                  "Code 0 will be scored as negative"
                  "Code 1 will be scored as positive");
              cut_off2=0;
           }
           write_stat_strings();
           open_logfile(ttlog);   /*** open 2 log ***/
           fprintf(logfile,"\\\"filename:\"\"%s\"\n",filename);
           fprintf(logfile,"\\\"outcome-test:\"\"%s\"\"cutoff:\"\" %.3lf\"\n",testname[Outcome_test], cut_off2);
           if (Subselect) {
              for (t=0;t<nooftests;t++) {
                 fprintf(logfile,"\\%-20s ", quoted(buffer,testname[test[t]]));
                 fprintf(logfile,"%-20s\n", quoted(buffer,evaltext[t]));
              }
            }
            if (*First_title) fprintf(logfile, "%s\n", quoted(buffer,First_title));
            if (*Second_title) fprintf(logfile, "%s\n", quoted(buffer,Second_title));
            fprintf(logfile, "%s\n", quoted(buffer,y_axis_title));
            fprintf(logfile,"\\\"testname\"\"cut_off\"\"N\"\"negneg\"\"negpos\"\"posneg\"\"pospos\"\"result\"\n");
            cprintf("\r\nCalculating crosstab frequencies: ");
            for (t=0;t<nooftests;t++) {
              if (valid_data[t]==0) continue;
              cprintf("%s ", numbertolotus(test[t]+1,"  "));
              cut_off1=cut_off(Cut_off1,t,FALSE);
              if (cut_off1==MISSING) {
                 if (Cutoff_flag) cut_off1=cutoff_value[test[t]];
                 else cut_off1=0;
              }
              for (row=0,N[t]=negneg=negpos=posneg=pospos=0;row<rows;row++) {
/*
                 d1= (row <valid_data[t])? getdata(row, t,FALSE): MISSING;
*/
                 d1= getdata(row, t,FALSE);
                 if (Outcome_test!=-1) {
                    d2=data0[casnum[row][t]][Outcome_test];
                    if (d1<=MISSING || d2<=MISSING) continue;
                    if (d1 > cut_off1 && d2 > cut_off2) pospos++;
                    else if (d1 > cut_off1) posneg++;
                    else if (d1<=cut_off1 && d2 > cut_off2) negpos++;
                    else negneg++;
                 } else {
                    code= rowlabel[casnum[row][t]]-'0';
                    if (d1 > cut_off1                && code==1) pospos++;
                    if (d1 > cut_off1                && code==0) posneg++;
                    if (d1 > MISSING && d1<=cut_off1 && code==1) negpos++;
                    if (d1 > MISSING && d1<=cut_off1 && code==0) negneg++;
                 }
#ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n %4d %s %12.3f(%12.3lf) %12.3f(%12.3lf) %3d++ %3d+- %3d-+ %3d--",
               __LINE__, __FILE__, d1, cut_off1, d2, cut_off2, pospos, posneg, negpos, negneg);
#endif
              }
              N[t]=negneg+negpos+posneg+pospos;
              if (pospos*posneg*negpos*negneg==0) {
                 if (warning(test[t], Outcome_test, (int)pospos, (int)posneg, (int)negpos, (int)negneg)==ESC){
                    return;
                 }
                 zeroflag[t]=1;
                    pospos+=0.5;
                    posneg+=0.5;
                    negpos+=0.5;
                    negneg+=0.5;
              } else zeroflag[t]=0;
              if (graph_type==ODDS_RATIO) {
                 m=mean[t] = pospos*negneg/(posneg*negpos);
                 se=sqrt(1/pospos + 1/posneg + 1/negpos + 1/negneg);
                 lower_limit[t]=exp(log(m)-ci*se);
                 upper_limit[t]=exp(log(m)+ci*se);
              } else {
                 m=mean[t] =(pospos/(pospos+negpos))/(posneg/(posneg+negneg));
                 se=sqrt(1/pospos+1/(pospos+negpos)+1/posneg+1/(posneg+negneg));
                 lower_limit[t]=exp(log(m)-ci*se);
                 upper_limit[t]=exp(log(m)+ci*se);
              }
               fprintf(logfile,"%-20s %12.3f %3d %5.1lf %5.1lf %5.1lf %5.1lf %12.3lf %12.3lf %12.3lf\n",
               quoted(buffer,testname[test[t]]),cut_off1, N[t],negneg,negpos,posneg,pospos,lower_limit[t],mean[t], upper_limit[t]);
              if (ymax<upper_limit[t]) ymax=upper_limit[t];
              if (ymin>lower_limit[t]) ymin=lower_limit[t];
           }
           fclose(logfile);   /*** close 2 log ***/
        }
        new_tests=FALSE;
        if (option==VIEW_GRAPH || option==SAVE_GRAPH) {
        if (!(
              (Outcome_test==-1 &&
                 (Graph_type==PEARSON ||
                  Graph_type==SPEARMAN)) ||
               Graph_type==DENDROGRAM    ||
               Graph_type==XYCLUSTER)) {
   /**** write histogram to HG-PIC-file ****/
           picopen(ttpic);
           x_origin=picy_axis_title(y_axis_title, y_origin + y_length)+80;
           if (ymin>=ymax) {
              cprintf("\r\n%s %4d ERROR: ymin=%.4f ymax=%.4f", __FILE__,__LINE__, ymin, ymax);
              if (getch()!=RETURN) farewell(pic_no);
              putch('\r');putch('\n');
           }
           Logflag = picy_cal(Logflag);
           tempmin=ymin;
           tempmax=ymax;
           if (Logflag) {
              ymin=log(ymin);
              ymax=log(ymax);
           }
           picextra_text(x_origin, y_origin, x_length, y_length);
           inter = (Inter_bar==-1)? INTER_BAR : Inter_bar;
           column_width=x_length/((1.0+inter)*nooftests);
           inter=(1.0+inter)*column_width;
           y0=(ymin <= 0) ? y_origin - ymin * scale_yfactor: y_origin;
           if (range_type) ranges=picranges(range_type, testnamelines, evaltextlines, inter);
           if (*First_title) picfirst_title( First_title,  x_origin + x_length/2);
           if (*Second_title) picsecond_title(Second_title, x_origin + x_length/2);
           if (Line) {
              for (t=0, range=0, r=0;t<nooftests;t++) {
                 if (range_type==2 && t-r==tests_per_range[range]) {
                    pen_up=1;
                    range++;
                    r=t;
                 }
                 xpos = x_origin + inter/2 + t*inter;
                 if (!valid_data[t]) {
                    pen_up=1;
                    continue;
                 }
                 find_values(t, Logflag);
                 y = y_origin + scale_yfactor*(central_value - ymin);
                 if (pen_up || t==0) picmove( xpos , y);
                 else picdraw(xpos , y);
                 pen_up=0;
              }
           }
           for (t=0,r=0,r0=0;t<nooftests;t++,r0++) {
               if (Trace==1) cprintf("\r\n%s %4d test %d",__FILE__, __LINE__, t);
               if (!valid_data[t]) {
                  r++;
                  r0=0;
                  continue;
               }
               if (Subselect && r0==tests_per_range[r]) {
                  r++;
                  if (Graph_type==BAR_GRAPH && Median_flag==2 && rangetext[r] && *rangetext[r]) fprintf(logfile, "%s\n", quoted(buffer,rangetext[r]));
                  r0=0;
               }
              xpos = x_origin + inter/2 + t*inter;
              if (!valid_data[t]) continue;
              find_values(t, Logflag);
   #ifdef TRACE
              if (Trace==2) fprintf(tracefile,"\n%s %4d %12.4lf %12.4lf %12.4lf",
                     __FILE__,__LINE__, lower_deviation, central_value, upper_deviation);
   #endif
              piccolumnlegend(t, xpos, range_type, testnamelines, evaltextlines);
              if (Graph_type==BAR_GRAPH && Median_flag==2) {
/*
                 start=(Outliers_flag)?0:class[0][t];
*/
                 start=0;
                 n=(Outliers_flag)?valid_data[t]:N[t];
                 p25=get_ile(start, t, 0.25, FALSE);
                 p75=get_ile(start, t, 0.75, FALSE);
                 se_low=get_ile(start, t, ilevalue( n, 0.25, 0.50), FALSE);
                 se_high=get_ile(start, t, ilevalue( n, 0.75, 0.50), FALSE);
                 if (n<3) p25=se_low=se_high=p75=get_ile(start, t, 0.50, FALSE);
                 for (i=0;i<30;i++) buffer[i]=' ';
                 buffer[31]='\0';
                 buffer[30]='\"';
                 if (Subselect) strcpy(temptext,columntext[t]);
                 else strcpy(temptext, testname[test[t]]);
                 i=strlen(temptext)>29?28:strlen(temptext)-1;
                 for (pos=29;i>-1; i--, pos--) buffer[pos]=temptext[i];
                 buffer[pos]='\"';
/*
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
*/
                 if (Logflag) {
                    p25=p25>0?log(p25):central_value;
                    p75=p75>0?log(p75):central_value;
                    se_low =(se_low>0) ?log(se_low) :central_value;
                    se_high=(se_high>0)?log(se_high):central_value;
                 }
                 if (lower_deviation==MISSING) lower_deviation=central_value;
                 write_box(
                    xpos,
                    y_origin,
                    (lower_deviation - ymin ) * scale_yfactor,
                    (p25-ymin)*scale_yfactor,
                    (se_low-ymin)*scale_yfactor,
                    (central_value   - ymin ) * scale_yfactor,
                    (se_high-ymin)*scale_yfactor,
                    (p75-ymin)*scale_yfactor,
                    (upper_deviation - ymin ) * scale_yfactor,
                    column_width,
                    n);
                 if (Cutoff_flag==1) {
                    d1=cutoff_value[test[t]];
                    if (Logflag && d1>0) d1=log(d1);
                    if (d1>MISSING) dotted_h_line(xpos-column_width/2, xpos+column_width/2, y_origin+(d1-ymin)*scale_yfactor);
                 }
              } else write_bar(
                 xpos,
                 y_origin,
                 y0,
                 ( lower_deviation - ymin ) * scale_yfactor,
                 ( central_value   - ymin ) * scale_yfactor,
                 ( upper_deviation - ymin ) * scale_yfactor,
                 column_width,
                 Cap,
                 0,
                 (Line)?0:1,
                 TRUE,
                 FALSE,
                 N[t],
                 zeroflag[t]
              );
              if (t< FILL_COL && filltype[t]!=-1) {
                 d1=(fill_min[t]==MISSING)?ymin:(Logflag?log(fill_min[t]):fill_min[t]);
                 d2=(fill_max[t]==MISSING)?ymax:(Logflag?log(fill_max[t]):fill_max[t]);
                 picfill(
                    xpos - column_width/2,
                    y_origin+ (d1 - ymin) * scale_yfactor,
                    xpos + column_width/2,
                    y_origin+ (d2 - ymin ) * scale_yfactor,
                    filltype[t]);
              }
           }
           if (filltype[FILL_COL]!=-1 && Graph_type==BAR_GRAPH) {
              d1=(fill_min[FILL_COL]==MISSING)?ymin:(Logflag?log(fill_min[FILL_COL]):fill_min[FILL_COL]);
              d2=(fill_max[FILL_COL]==MISSING)?ymax:(Logflag?log(fill_max[FILL_COL]):fill_max[FILL_COL]);
              picfill(
                 x_origin-RIM/2,
                 y_origin+ (d1 - ymin) * scale_yfactor,
                 x_origin + x_length+RIM/2,
                 y_origin+ (d2 - ymin ) * scale_yfactor,
                 filltype[FILL_COL]);
           }
     /* draw bottomline */
           if (!(Line || (Graph_type==BAR_GRAPH && Median_flag==2))) {
              for (i=0;i<Plotter;i++) {
                 picmove(x_origin         +(inter-column_width)/2, y0);
                 picdraw(x_origin+x_length-(inter-column_width)/2, y0);
              }
           }
           if (range_type) ranges=picranges(range_type, testnamelines, evaltextlines, inter);
           if (graph_type==BAR_GRAPH && Prob_flag) draw_prob();
           picend();
/*
           if (fclose(fpout)==EOF) {
             cprintf("\r\nError writing file \"%s\" to disc: disc full?",ttpic)
             getch();
             farewell(pic_no);
           }
*/
           setdisk(startup_drive);
        }
        if (graph_type==BAR_GRAPH && Median_flag!=2) write_ttlog(0);
        fflush(stdin);
        if (repeat) {
           clrscr();
           if (graph_type==PERCENTAGE_POSITIVE) cprintf("Cut-off value: %.3f\r\n\r\n",Cut_off);
           cprintf("\r\nSPACE  : Modify cut-off value%c\r\n"
                       "RETURN : Proceed\r\n: ", (graph_type==PERCENTAGE_POSITIVE)?' ':'s');
           key=getkey(0);
           if (!key) return;
           if (key==RETURN) repeat=FALSE;
           if (key==ESC) return;
        }
     } else repeat=FALSE;
     picture_ready_flag=TRUE;
  }
  Logflag=logflag1;
  ymin=tempmin;
  ymax=tempmax;
}

void ogive(void) {
   int i, t, n, extra_rim=RIM/2, dd, xpos0, ypos0;
   int ypos_legend, y;
   int column, testname_column, row, pendown;
   float d, d2, tempmin, tempmax;
   double dx, xpos, ypos;
/*
   double percent_per_case;
*/
   fclose(logfile); /* should not be necessary */
   if (Median_flag==2) Median_flag=1; /* should not be necessary */
   if (!find_minmax(0)) return;
   if (ymin>=ymax) return;
   ascal(&noofcalibrators, &ymin, &ymax,Logflag, 0);
   find_minmax(1);
/*
   tt_minmax();
   count_excess();
*/
   bottomlines=2;
   headerlines= get_headerlines(0, 0, 0);
   y_origin= picbottomline(bottomlines)+8;
   if (*extra_text[8]) y_origin+=16;
   if (Ycal>3) y_origin-=RIM;
   dd=0;
   if (yll) {
      y_origin+=extra_rim;
      dd=-extra_rim;
   }
   y_length= pictopline(headerlines)-y_origin-20;
   if (yll) y_length-=extra_rim;
   if (yul) {
      y_length-=extra_rim;
      dd+=extra_rim;
   }
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d:min=%.3f max=%3f",__LINE__, ymin, ymax);
#endif
   if (Logflag && ymin <=0) cprintf("No log-transformation: lowest value: %8.3f\r\n",ymin);
   /**** write ogives to PIC-file ****/
   picopen(ttpic);

   if (Color>1) piccolor(0);

   new_tests=FALSE;
   x_origin=picy_axis_title(y_axis_title, y_origin + y_length)+80;
   Logflag= picy_cal(Logflag);
   if (yll) dotted_h_line(x_origin,x_origin+x_length,y_origin);
   if (yul) dotted_h_line(x_origin,x_origin+x_length,y_origin+y_length);
   tempmin=ymin;
   tempmax=ymax;
   if (Logflag) {
      ymin=log(ymin);
      ymax=log(ymax);
   }
   for (n=0;n<Plotter;n++) {
      picmove(x_origin, 10);
      pictext(0, BOTTOMLEFT, "Percentile");
   }
/*
/* LOGFILE */   open_logfile(ttlog);  /*** open 4 log ***/
/* LOGFILE */                      fprintf(logfile,"\\\"filename:\"\"%s\"\n",filename);
/* LOGFILE */   if (*First_title)  fprintf(logfile, "\"%s\"\n", First_title);
/* LOGFILE */   if (*Second_title) fprintf(logfile, "\"%s\"\n", Second_title);
/* LOGFILE */                      fprintf(logfile, "\"%s\"\n\"%s\"\n", "percent", y_axis_title);
*/
   picextra_text(x_origin, y_origin, x_length, y_length);
   if (*First_title)  for (n=0;n<Plotter;n++) picfirst_title( First_title,  x_origin + x_length/2);
   if (*Second_title) for (n=0;n<Plotter;n++) picsecond_title(Second_title, x_origin + x_length/2);
   ypos_legend=y_origin + y_length;
   cprintf("\r\nProcessing test ");
   for (t=0; t<nooftests; t++) {
      if (Color>1) piccolor(Symbol_color[(nooftests-t)%strlen(Symbol_color)]);  /*%(Color+1));*/
      column= medianrank[t];
      if (column==-1 || N[column]<2) continue;
      testname_column= test[column];
      xpos=x_origin+x_length/4;
      if (Subselect) strcpy(buffer,evaltext[column]);
      else strcpy(buffer,testname[testname_column]);
      ypos_legend=pictextlines(xpos,ypos_legend,buffer,0,TOPRIGHT);
      d=i50[column];
      dd=0;
      if (Logflag) {
         if (d>0) d=log(d);
         else {
            d=ymin;
            dd=-extra_rim;
         }
      }
      if (d<ymin) {
         d=ymin;
         dd=-extra_rim;
      }
      if (Ymax!=MISSING && d>ymax) {
         d=ymax;
         dd=extra_rim;
      }
/*
      picmove(xpos,ypos_legend+Vsize/2);
      picdraw(x_origin+(Centile?Centile:.5)*x_length, y_origin+scale_yfactor*(d-ymin)+dd);
*/
      picdotted_line(xpos,ypos_legend+Vsize/2,x_origin+(Centile?Centile:.5)*x_length, y_origin+scale_yfactor*(d-ymin)+dd,2);
   }
/* actual curves draw low-to-high for optimal log-file output: */
   for (t=0; t<nooftests; t++) {
      if (Color>1) piccolor(Symbol_color[(1+t)%strlen(Symbol_color)]);  /*%(Color+1));*/
/*
      if (Color>1) piccolor(1+t%(Color+1));
*/
      column= medianrank[nooftests-t-1];
      if (column==-1 || N[column]<2) continue;
      testname_column= test[column];
      cprintf("%s/", numbertolotus(test[column]+1,"  "));
      dx=(double)x_length/(valid_data[column]+1);
/*
      percent_per_case=100.0/(valid_data[column]+1);
*/
      if (Subselect) strcpy(buffer,evaltext[column]);
      else strcpy(buffer,testname[testname_column]);
/*
/* LOGFILE */      fprintf(logfile,"\"%s\"\n",buffer);
*/
      xpos=x_origin+dx;
      for (row=0, pendown=FALSE;row<rows-Cutoff_flag;row++) {
            dd=0;
            d=getdata(rank[row][column], column, FALSE);
            if (d<=MISSING) continue;
/*
/* LOGFILE */      fprintf(logfile,"%12.3f %12.3lf\n", d, (double)(row+1)*percent_per_case);
*/
            #ifdef TRACE
               if (Trace==2) fprintf(tracefile,"\n%4d:t=%2d column=%2d row=%3d rank=%3d d=%12.3f",
                     __LINE__, t, column, row,rank[row][column],d);
            #endif
            if (Logflag) {
               if (d>0) d=log(d);
               else {
                  d=ymin;
                  dd=-extra_rim;
               }
            }
            if (d<ymin) {
               d=ymin;
               dd=-extra_rim;
            }
            if (Ymax!=MISSING && d>ymax) {
               d=ymax;
               dd=extra_rim;
            }
            ypos=y_origin+scale_yfactor*(d-ymin)+dd;
	    if (!pendown){
               xpos0=(int)xpos;
               ypos0=(int)ypos;
               pendown=TRUE;
            } else {
               picmove(xpos0, ypos0);
               picdraw((int)xpos,(int)ypos);
/*
               if (Datasymbol==-1) {
               } else picdotted_line(xpos0, ypos0, (int)xpos,(int)ypos, Datasymbol);
*/
               xpos0=(int)xpos;
               ypos0=(int)ypos;
            }
            xpos+=dx;
      }
   }
/*
/* LOGFILE */   fclose(logfile);   /*** close 4 log ***/
*/
   cprintf("|");
   if (Color>1) piccolor(0);
   y=(Ycal<4)?y_origin:y_origin+RIM;
   for (n=0;n<Plotter;n++) {
      if (Ycal<4) {
         picmove(x_origin,          y-RIM);
         picdraw(x_origin+x_length, y-RIM);
      }
      for (i=0;i<5;i++) {
          picmove(x_origin+i*x_length/4,y-RIM);
          picdraw(x_origin+i*x_length/4,y-RIM/2);
          picmove(x_origin+i*x_length/4,y-RIM-0.2*VSIZE);
          sprintf(buffer,"%d",i*25);
          pictext(0, CENTERTOP,buffer);
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
   picend();
   setdisk(startup_drive);
   fflush(stdin);
   picture_ready_flag=TRUE;
   ymin=tempmin;
   ymax=tempmax;
}

/* local routines */

float cut_off(char *cut_off_string, int t, int flag) {
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d: cut_off: %s t=%d (%s) flag=%d",
         __LINE__, cut_off_string, t, (flag)?testname[t]:testname[test[t]],flag);
#endif
   if (*cut_off_string=='A') return MISSING;
   if (*cut_off_string=='M') return get_ile( 0, t, 0.5, flag);
   if (*cut_off_string=='T') return get_ile( 0, t, 0.666, flag);
   if (*cut_off_string=='Q') return get_ile( 0, t, 0.75, flag);
   if (*cut_off_string=='P') return get_ile( 0, t, 0.8, flag);
   if (*cut_off_string=='D') return get_ile( 0, t, 0.9, flag);
   return atof(cut_off_string);
}

int warning(int column1, int column2, int pospos, int posneg, int negpos,int negneg) {
            clrscr();
            cprintf(
            "%50s\r\n"
            "%30s         -      +\r\n\r\n"
            "%30s  -     %3d    %3d\r\n"
            "%30s  +     %3d    %3d\r\n\r\n"
            "WARNING: Empty cells found!\r\n"
            "For the calculation all cells will be increased by 0.5\r\n"
            "Tests with empty cells will be indicated in the graph by bracketed N-values\r\n"
            "\r\n\r\nPress RETURN to continue\nPress Esc to go back to test selection\n",
            testname[column1],"", testname[column2], negneg, posneg, "", negpos, pospos);
            return (getkey(0));
}

void write_bar(
   int xpos,            /* lower left of graph */
   int y_origin,        /* lower left of graph */
   int y0,              /* 0-value on axis */
   int lower_deviation, /* value lower error-bar */
   int central_value,   /* value actual bar-size */
   int upper_deviation, /* value upper error-bar */
   int x_unit,      /* column width */
   float cap,       /* bar width as fraction of column width */
   int tip,         /* size of tip on cap */
   int bar,         /* flag, ON=draw complete bar + error-lines, OFF=draw error-lines only */
   int combi,       /* flag, ON=draw complete bar, OFF=draw horizontal line */
   int tt,          /* flag, ON=no vertical error_line */
   int valid_data,
   int zeroflag
){
    int x, y, base, stat;
    float tipfactor;
/*
    tipfactor=(tip) ? 0.8 : 1;
*/
    tipfactor= 0.8;
    stat=Sd_flag;
    if (stat<Sem_flag) stat=Sem_flag;
    if (stat<Iqr_flag) stat=Iqr_flag;
    base= (y0>y_origin) ? y0 : y_origin;
    if (Trace==1) cprintf("\r\n%s %4d stat=%d Sd_flag=%d Sem_flag=%d Iqr_flag=%d swapstat=%d\r\n",__FILE__, __LINE__, stat, Sd_flag, Sem_flag, Iqr_flag, swapstat);
    if (Graph_type==TTEST && stat>1) {                                                                                    
       if (stat>3 && swapstat) stat++;
       y=central_value+y_origin;
/*
       x=(Sd_flag==2 || Sem_flag==2 || Iqr_flag==2)?xpos-cap*x_unit/2:xpos+cap*x_unit/2;
*/
   if (Trace==1) {
        cprintf("\r\n%s %4d stat=%d Sd-flag=%d\r\n",__FILE__, __LINE__, stat, Sd_flag);
        if (getch()==27) farewell(-1);
   }
       x=(stat%2)?xpos-cap*x_unit/2:xpos+cap*x_unit/2;
       if (Combi) {
          picmove(x, upper_deviation+y_origin);
          picdraw(x, lower_deviation+y_origin);
          picmove(x - Combi/2 , base);
          picdraw(x - Combi/2 , y);
          picdraw(x + Combi/2 , y);
          picdraw(x + Combi/2 , base);
          picmove(x_origin,base);
          picdraw(x_origin+x_length, base);
       } else {
          picmove(x, upper_deviation+y_origin);
          picdraw(x, y+Legend_symbol_size/2);
          picdraw(x+Legend_symbol_size/2, y);
          picdraw(x, y-Legend_symbol_size/2);
          picdraw(x-Legend_symbol_size/2, y);
          picdraw(x, y+Legend_symbol_size/2);
          picmove(x, y-Legend_symbol_size/2);
          picdraw(x, lower_deviation+y_origin);
       }
       if (tip) {    
         tip=(Combi)?Combi*Cap/2:Legend_symbol_size*Tip/2;
         picmove(x - tip, upper_deviation+y_origin);
         picdraw(x + tip, upper_deviation+y_origin);
         picmove(x - tip, lower_deviation+y_origin);
         picdraw(x + tip, lower_deviation+y_origin);
       }
       swapstat=(swapstat)?0:1;
       return;
    }

   /* draw mean/median bar in graph: */

   base= (y0>y_origin) ? y0 : y_origin;
   if (central_value>=0 && central_value<=y_length) {
      y= central_value + y_origin;
      if (bar) {
         if (combi) {
            picmove(xpos - x_unit/2 , base);
            picdraw(xpos - x_unit/2 , y);
            picdraw(xpos + x_unit/2 , y);
            picdraw(xpos + x_unit/2 , base);
         } else {
            picmove(xpos - x_unit*cap/2 , y + tip/2);
            picdraw(xpos - x_unit*cap/2 , y - tip/2);
            picmove(xpos - x_unit*cap/2 , y);
            picdraw(xpos + x_unit*cap/2 , y);
            picmove(xpos + x_unit*cap/2 , y + tip/2);
            picdraw(xpos + x_unit*cap/2 , y - tip/2);
         }
      }
   }
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d x=%d central value: base=%d top=%d",
         __LINE__, xpos - x_unit/2, base, y);
#endif
   if (Sd_flag || Sem_flag || Iqr_flag) {
      if (upper_deviation>=0 && upper_deviation<=y_length) {
      /* draw upper deviation line (MEAN+SD, MEAN+SEM or 75-percentile) in graph: */
      y= upper_deviation + y_origin;
      if (tip) {
         picmove(xpos - x_unit * cap/2*tipfactor , y + tip/2);
         picdraw(xpos - x_unit * cap/2*tipfactor , y - tip/2);
         picmove(xpos + x_unit * cap/2*tipfactor , y + tip/2);
         picdraw(xpos + x_unit * cap/2*tipfactor , y - tip/2);
      }
      picmove(xpos - x_unit * cap/2*tipfactor , y);
      picdraw(xpos + x_unit * cap/2*tipfactor , y);
      if (Nn && !tt) {
         if (zeroflag) sprintf(buffer,"(n=%d)",valid_data);
         else          sprintf(buffer,"n=%d",valid_data);
         picmove(xpos, y + 32);
         pictext(0, CENTERBOTTOM,buffer);
      }
    }

         /* draw lower deviation line (MEAN-SD, MEAN-SEM or 25-percentile) in graph: */
    if (lower_deviation>=0 && lower_deviation<=y_length) {
       y= lower_deviation + y_origin;
       if (tip) {
            picmove(xpos - x_unit * cap/2 *tipfactor , y + tip/2);
            picdraw(xpos - x_unit * cap/2 *tipfactor , y - tip/2);
            picmove(xpos + x_unit * cap/2 *tipfactor , y + tip/2);
            picdraw(xpos + x_unit * cap/2 *tipfactor , y - tip/2);
       }
       picmove(xpos - x_unit * cap/2 *tipfactor , y);
       picdraw(xpos + x_unit * cap/2 *tipfactor , y);
   #ifdef TRACE
            if (Trace==2) fprintf(tracefile,"\n%s %4d lower_deviation= %d",__FILE__, __LINE__, y);
   #endif
      }
      if (!tt) {
         /* draw vertical line from upper- to lower_deviation: */
         picmove(xpos , upper_deviation + y_origin);
         picdraw(xpos , lower_deviation + y_origin);
   /*
         y=(upper_deviation<=ymin)?0:upper_deviation;
         y=(upper_deviation>=ymax)?y_length:upper_deviation;
         y=(lower_deviation<0)?0:lower_deviation;
         y=(lower_deviation>y_length)?y_length:lower_deviation;
   */
      }
   }
}

void write_box(
   int xpos,            /* lower left of graph */
   int y_origin,        /* lower left of graph */
   int p10,             /* 10-centile */
   int p25,             /* 25-centile */
   int se_min,          /* median - SE */
   int median,          /* median */
   int se_max,          /* median + SE */
   int p75,             /* 75 centile */
   int p90,             /* 90 centile */
   int x_unit,          /* column width */
   int valid_data
){
    picmove(xpos-x_unit/2, p75   + y_origin);
    picdraw(xpos+x_unit/2, p75   + y_origin);
    picdraw(xpos+x_unit/2, se_max + y_origin);
    picdraw(xpos,          median + y_origin);
    picdraw(xpos-x_unit/2, se_max + y_origin);
    picdraw(xpos-x_unit/2, p75   + y_origin);

    picmove(xpos-x_unit/2, p25    + y_origin);
    picdraw(xpos+x_unit/2, p25    + y_origin);
    picdraw(xpos+x_unit/2, se_min + y_origin);
    picdraw(xpos,          median + y_origin);
    picdraw(xpos-x_unit/2, se_min + y_origin);
    picdraw(xpos-x_unit/2, p25    + y_origin);

    picmove(xpos,p10+y_origin);
    picdraw(xpos,p25+y_origin);

    picmove(xpos,p90+y_origin);
    picdraw(xpos,p75+y_origin);
    if (Nn && valid_data>0) {
       sprintf(buffer,"n=%d",valid_data);
       picmove(xpos, y_length + y_origin-20);
       pictext(0, CENTERTOP,buffer);
    }
}


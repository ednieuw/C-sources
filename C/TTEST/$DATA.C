#include "ttest.h"
/* module $data.c */
int    accept_data(float d, int warningflag);
void   adjust_minmax(void); /* corrects ymin/ymax for deviations extending beyond ttmin/ttmax */
/*
void   count_excess(void);
*/
double find_prob(double tval, int df);
int    find_minmax(int cycle);
int    find_N(void); /* find the number of valid valid-data */
int    find_row(int row, int t, int flag);
void   find_values(int t, int logflag);
float  getdata(int row, int t, int flag);
void   get_diff(int t0, int t1);
float  get_ile(int start, int column,double ile, int flag);
int    get_mean(int warningflag);
double ilevalue(int n, double level, double centile);
void   make_centiles(int nooftests, int outliers_flag);
int    process_data(void);
void   qs(int t, int rows);
void   qs0(int left, int right);
void   rank_data(int nooftests);
void   rank_medians(void);
void   pearson(int logflag1, int logflag2);
void   spearman(void);
double t95(int df);
void   tt_minmax(void);
double tt_calc(int t0, int t1, int statmode);

/* local routines: */

int m2v(int x, int y, int n);
void v2m(int v, int *x, int *y, int n);
int vec0(int x, int n);
void calc_dist_array(float *dist, float *matrix, int nooftests);
float cluster_get_dist(int case0, int case1, signed char cluster[MAX_COL][MAX_COL+1], float *dist, int nooftests);
void cluster_merge(int case0, int case1, signed char cluster[MAX_COL][MAX_COL+1]);
void cluster_show(signed char cluster[MAX_COL][MAX_COL+1], int nooftests);
void dendrogram(float* matrix, int nooftests);
void xycluster(float* dist_matrix, int nooftests);

#define XORIGIN 20
#define YORIGIN 200
#define YLENGTH 2100

int accept_data(float d, int warningflag) {
   if (d==MISSING) return 0;
   if (Logflag && d<=0) {
      if (warningflag && Mean_flag) {
         clrscr();
         cprintf("Warning: value %.3f cannot be log-transformed.\r\nData skipped, thus mean-value will be OVER-estimated!\r\n\r\nPress any key to continue\r\n\r\n",d);
         if (getch()==ESC) return -1;
      }
      return 0;
   }
   if (Outliers_flag) return 1;
   if (d<ymin) return 0;
   if (Ymax!=MISSING && d>ymax) return 0;
   return 1;
}

void adjust_minmax(void) { /* corrects ymin/ymax for deviations extending beyond ttmin/ttmax */
   int t;
   for (t=0;t<nooftests;t++) {
      if (Trace==1) putch('2');
      if (test[t]==-1) continue;
      if (!N[t]) continue;
      find_values(t, 0);
      if (lower_deviation < ymin
            &&
            (Ymin==MISSING || lower_deviation>=Ymin)
            &&
            !(Logflag && lower_deviation<=0)
         ) ymin= lower_deviation;
      if (upper_deviation > ymax
            &&
            (Ymax==MISSING || upper_deviation<=Ymax)
            &&
            !(Logflag && upper_deviation<=0)
         ) ymax= upper_deviation;
      if (ymax<=ymin) {
         cprintf("\r\nERROR: ymin (%.3f) >= ymax (%.3f)",ymin, ymax);
         cprintf("\r\n%4d %s:\r\nPress any key",__LINE__, __FILE__);
         if (getch()==27) farewell(-1);
         return;
      }
   }
   /* approximated scaling factor for use in vertical positioning probability strings */
   if (Logflag && ymin<0 || (ymax<=ymin)) {
      cprintf("\r\n%4d %s Logflag=%d ymax=%.3f ymin=%.3f: ",__LINE__, __FILE__, Logflag, ymax, ymin);
      if (getch()==27) farewell(-1);
      putch('\r');
      putch('\n');
   }
   scale_yfactor=(Logflag)?(double)Y_LENGTH/(log(ymax) - log(ymin)):(double)Y_LENGTH/(ymax - ymin);
}
            
void dendrogram(float* matrix, int nooftests) {
   int i, j, left, right, n, Xmin=100, headerlines, y_length;
   int case0, case1, clusters;
   float d, maxcoeff, mincoeff;
   double xfactor, yfactor;
   int xpos1, ypos0, ypos1;
   int xpos[MAX_COL], ypos[MAX_COL];
   int casenumber[MAX_COL*2];
   int casepointer[MAX_COL];
   int alias[MAX_COL];
   int stage[MAX_COL-1][3];
   signed char cluster[MAX_COL][MAX_COL+1];
   float coeff[MAX_COL-1];
   clusters=nooftests;
   if (nooftests<3) {
      cprintf("Too few cases: N=%d\r\n\r\nPress any key to return to the main menu\r\n", nooftests);
      getch();
      return;
   }
   clrscr();
   n=nooftests-2;left=nooftests;right=nooftests+1;
   for (i=0;i<2*MAX_COL;i++) casenumber[i]=-1;
   for (i=0;i<MAX_COL;i++) {
      casepointer[i]=-1;
      alias[i]=i;
      cluster[i][0]=i;
      cluster[i][1]=-1;
   }
/*   calc_dist_array(dist, matrix, nooftests); */
   do {
      mincoeff=-MISSING;
      for (i=0; i<nooftests-1; i++) {
         for (j=i+1; j<nooftests; j++) {
            d=cluster_get_dist(i, j, cluster, dist, nooftests);
            if (d>MISSING && d<mincoeff) {
               mincoeff=d;
               case0=i;
               case1=j;
            }
         }
      }
      coeff[nooftests-clusters]=mincoeff;
      stage[nooftests-clusters][0]=nooftests-clusters+1;
      stage[nooftests-clusters][1]=case0+1;
      stage[nooftests-clusters][2]=case1+1;
      cluster_merge(case0, case1, cluster);
      cluster_show(cluster, nooftests);
      clusters--;
   } while (clusters>1);
   cprintf("\r\n");
   casenumber[left]=stage[n][1]-1; casenumber[right]=stage[n][2]-1;
   n--;
   while (n>-1) {
     for (i=left;i<=right;i++) casepointer[casenumber[i]]=i;
     if   (casepointer[stage[n][1]-1]==-1) {
        case1=stage[n][1]-1;
        case0 =stage[n][2]-1;
     } else if (casepointer[stage[n][2]-1]==-1) {
        case1=stage[n][2]-1;
        case0 =stage[n][1]-1;
     } else cprintf("n=%d: no case1 found\r\n");
     if (casepointer[case0]<=nooftests) for (i=left--; i<=casepointer[case0] && i>0 && i<MAX_COL*2; i++) casenumber[i-1]=casenumber[i];
     else for (i=right++; i>=casepointer[case0] && i>=0 && i+1<MAX_COL*2; i--) casenumber[i+1]=casenumber[i];
     casenumber[casepointer[case0]]=case1;
     n--;
  }
  for (i=left;i<=right;i++) casepointer[casenumber[i]]=i;
  if      (*Second_title) headerlines= 2;
  else if (*First_title)  headerlines= 1;
  else                    headerlines= 0;
  y_length= pictopline(headerlines)-YORIGIN-RIM;
  yfactor=y_length/(nooftests-1);
  maxcoeff=0.0; mincoeff=-MISSING;
  for (n=0; n<nooftests-1; n++) {
     if (coeff[n]>maxcoeff) maxcoeff=coeff[n];
     if (coeff[n]<mincoeff) mincoeff=coeff[n];
  }
  for (n=0; n<nooftests; n++) ypos[n]=YORIGIN+(int)((casepointer[n]-left)*yfactor);
  for (n=0; n<nooftests; n++) ypos[n]=YORIGIN+(int)((casepointer[n]-left)*yfactor);
  for (n=0; n<nooftests; n++) xpos[n]=XORIGIN+Clxlength;
  for (i=0;i<MAX_COL;i++) alias[i]=i;
  if (mincoeff>=maxcoeff) {
     cprintf("\n%s %4d : mincoeff=%.4f maxcoeff=%.4f",
     __FILE__, __LINE__, mincoeff, maxcoeff);
     if (getch()==27) farewell(pic_no);
     putch('\r');
     putch('\n');
  }
  xfactor=(Clxlength-(Zero?0:Xmin))/(maxcoeff-(Zero?0:mincoeff));
  picopen(ttpic);
  if (*First_title) picfirst_title( First_title,  XORIGIN + Clxlength/2);
  if (*Second_title) picsecond_title(Second_title,XORIGIN + Clxlength/2);
  xpos1=XORIGIN+Clxlength+40;
  for (n=0; n<nooftests; n++) {
     picmove(xpos1,ypos[n]);
     if (!Subselect) pictext(0, CENTERLEFT, testname[test[n]]);
     else pictext(0, CENTERLEFT, evaltext[n]);
  }
  for (n=0; n<nooftests-1; n++) {
     case0=alias[stage[n][1]-1];
     case1=alias[stage[n][2]-1];
     xpos1=XORIGIN+Clxlength-(Zero?0:Xmin)-(int)((coeff[n]-(Zero?0:mincoeff))*xfactor);
     ypos0=ypos[case0];
     ypos1=ypos[case1];
     ypos[case1]=(ypos0+ypos1)/2;
     if (Ortho) {
        picmove(xpos[case0],ypos0);
        picdraw(xpos1      ,ypos0);
        picmove(xpos[case1],ypos1);
        picdraw(xpos1      ,ypos1);
        picdraw(xpos1      ,ypos0);
     } else {
        picmove(xpos[case0],ypos0);
        picdraw(xpos1      ,ypos[case1]);
        picmove(xpos[case1],ypos1);
        picdraw(xpos1      ,ypos[case1]);
     }
     for (i=0; i<nooftests; i++) if (alias[i]==case0) alias[i]=case1;
     xpos[case1]=xpos1;
   }
   picend();
}

void xycluster(float* dist, int nooftests) {
   int i, j, xorigin, yorigin, xlength, ylength, headerlines;
   int case0, case1;
   float maxcoeff, mincoeff, x, y;
   double xfactor, yfactor;
   if (nooftests<3) {
      cprintf("Too few cases: N=%d\r\n\r\nPress any key to return to the main menu\r\n", nooftests);
      if (getch()==ESC) farewell(pic_no);
      return;
   }
   clrscr();
   if      (*Second_title) headerlines= 2;
   else if (*First_title)  headerlines= 1;
   else                    headerlines= 0;
   y_length= pictopline(headerlines)-YORIGIN;
   open_logfile(ttlog);
   maxcoeff=MISSING;
   mincoeff=-MISSING;
   for (i=0; i<(nooftests-2)*(nooftests-1); i++) {
      if (maxcoeff<dist[i]) {
         maxcoeff=dist[i];
         v2m(i,&case0,&case1,nooftests);
      }
      if (mincoeff>dist[i]) mincoeff=dist[i];
   }
   if (!Distance) {
      maxcoeff=1;
      mincoeff=-MISSING;
      for (i=0; i<(nooftests-2)*(nooftests-1); i++) if (mincoeff>cormatrix[i]) mincoeff=cormatrix[i];
   }
   if (maxcoeff<=mincoeff) {
      maxcoeff=Distance?4:1;
      mincoeff=Distance?0:-1;
   }
   fprintf(logfile,"\nXY-clustering: distance tabel\n");
   for (i=0;i<nooftests;i++) {
      for (j=0;j<nooftests;j++) {
         fprintf(logfile, "%8.4f",i==j?0.0:dist[m2v(i,j,nooftests)]);
      }
      fprintf(logfile,"\n");
   }
   fprintf(logfile,"%s\n",testname[test[case0]]);
   fprintf(logfile,"%s\n",testname[test[case1]]);
   for (i=0; i<nooftests; i++) {
      fprintf(logfile,"%8.4f  %8.4f\n", i==case0?0.0:dist[m2v(i,case0,nooftests)], i==case1?0.0:dist[m2v(i,case1,nooftests)]);
   }
   fprintf(logfile,"\n");
   fclose(logfile);
   xorigin=3*RIM;
   yorigin=3*RIM;
   xlength=Clxlength-4*RIM;
   xfactor=xlength/(maxcoeff-mincoeff);
   if      (*Second_title) headerlines= 2;
   else if (*First_title)  headerlines= 1;
   else                    headerlines= 0;
   ylength= pictopline(headerlines)-yorigin-RIM;
   yfactor= ylength/(maxcoeff-mincoeff);
   picopen(ttpic);
   if (*First_title) picfirst_title( First_title,  xorigin + xlength/2);
   if (*Second_title) picsecond_title(Second_title,xorigin + xlength/2);
   picmove(xorigin-RIM,        yorigin-RIM);
   picdraw(xorigin+xlength+RIM,yorigin-RIM);
   picdraw(xorigin+xlength+RIM,yorigin+ylength+RIM);
   picdraw(xorigin-RIM,        yorigin+ylength+RIM);
   picdraw(xorigin-RIM,        yorigin-RIM);
   if (!Distance && mincoeff<0) {
      picdotted_line(xorigin-mincoeff*xfactor, yorigin-RIM,xorigin-mincoeff*xfactor, yorigin+ylength+RIM,2);
      picdotted_line(xorigin-RIM, yorigin-mincoeff*yfactor,xorigin+xlength+RIM, yorigin-mincoeff*yfactor,2);
   }
   for (i=0; i<nooftests; i++) {
      if (Fullname) {
         if (!Subselect) strcpy(buffer,testname[test[i]]);
         else strcpy(buffer,evaltext[i]);
      } else {
        buffer[0]='A'+i;
        buffer[1]='\0';
      }
      if (Distance) {
         x=(i==case0)?0:dist[m2v(i,case0,nooftests)];
         y=(i==case1)?0:dist[m2v(i,case1,nooftests)];
      } else {
         x=(i==case1)?1.0:cormatrix[m2v(i,case1,nooftests)];
         y=(i==case0)?1.0:cormatrix[m2v(i,case0,nooftests)];
      }
      picmove(xorigin+(x-mincoeff)*xfactor, yorigin+(y-mincoeff)*yfactor);
      pictext(0, CENTER, buffer);
   }
   if (!Fullname) {
      x=xorigin+xlength+RIM+28;
      y=yorigin+ylength+RIM;
      for (i=0; i<nooftests && y>Vsize; i++) {
         if (!Subselect) sprintf(buffer,"%c=%s",'A'+i,testname[test[i]]);
         else sprintf(buffer,"%c=%s",'A'+i,evaltext[i]);
         picmove(x,y);
         pictext(0, TOPLEFT, buffer);
         y-=0.8*Vsize;
      }
   }
   picend();
}

void calc_dist_array(float *dist, float *matrix, int nooftests) {
   int i,ii,j=1;
   double d;
   gotoxy(1,1);
   clrscr();
   for (i=0; i<nooftests-1;i++) putch('\\');
   for (i=0;i<nooftests*(nooftests-1);i++) dist[i]=0.0;
   for (j=0;j<nooftests-1;j++) {
      putch('\b');
      for (i=j+1;i<nooftests;i++) {
         dist[m2v(i,j,nooftests)]=0;
         for (ii=0;ii<nooftests;ii++) {
            d=((i==ii)?1.0:matrix[m2v(i,ii,nooftests)])-((j==ii)?1.0:matrix[m2v(j,ii,nooftests)]);
            dist[m2v(i,j,nooftests)]+= d*d;
         }
      }
   }
}

float cluster_get_dist(int case0, int case1, signed char cluster[MAX_COL][MAX_COL+1], float *dist, int nooftests) {
   int i,j,n=0;
   float result=0.0;
   if (cluster[case0][0]==-1 || cluster[case1][0]==-1) return MISSING;
   for (i=0;cluster[case0][i]!=-1;i++) {
      for (j=0;cluster[case1][j]!=-1;j++) {
         if (cluster[case0][i]!=cluster[case1][j]) result+=dist[m2v(cluster[case0][i],cluster[case1][j],nooftests)];
         n++;
      }
   }
   return result/n;
}

void cluster_merge(int case0, int case1, signed char cluster[MAX_COL][MAX_COL+1]) {
   int pos0=0, pos1=0;
   while (cluster[case0][pos0]!=-1) pos0++;
   while (cluster[case1][pos1]!=-1) cluster[case0][pos0++]=cluster[case1][pos1++];
   cluster[case0][pos0]=-1;
   cluster[case1][0]=-1;
}

void cluster_show(signed char cluster[MAX_COL][MAX_COL+1], int nooftests) {
   int i,j;
   clrscr();
   for (i=0;i<nooftests && i<25;i++) {
      cprintf("%2d: ",i);
      j=0;
      while (cluster[i][j++]!=-1) putch(219);
      cprintf("]\r\n");
   }
}
/*
void count_excess(void) {
   int t, row;
   float d;
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
*/
int find_minmax(int cycle) {
   int t, row;
   float d;
   if (nooftests<1) {
      for (t=0; t<columns; t++) test[t]=test0[t]=t;
      test[t]=test0[t]=-99;
      nooftests=columns;
   }
/*
   if (!cycle && Ymax !=MISSING && !(Logflag && Ymax==0)) ymax= Ymax;
   if (!cycle && Ymin !=MISSING && !(Logflag && Ymin==0)) ymin= Ymin;
*/
   if (cycle==0) {
      if (Ymax !=MISSING) {
         if (!(Logflag && Ymax<=0)) ymax= Ymax;
      } else ymax=MISSING;
      if (Ymin !=MISSING) {
         if (!(Logflag && Ymin<=0)) ymin= Ymin;
      } else ymin=-MISSING;
   }
   if (Ymax==MISSING || Ymin==MISSING || cycle) {
      gotoxy(1,1);
      clreol();
      for (t=0; t<nooftests;t++) putch(cycle?'.':'*');
      for (t=0; t<nooftests;t++) {
         if (Trace!=1) putch('\b');
         if (Trace==1) putch('3');
         if (test[t]==-1) continue;
         if (Trace==1) putch('4');
         if (cycle && !valid_data[t]) continue;
         if (Trace==1) putch('5');
         if (Graph_type==TTEST || Graph_type==OGIVE || Graph_type==DOTMATRIX) {
            for (row=0;row<rows;row++) {
               d=getdata( row, t, FALSE);
               if (
                     d>MISSING &&
                     !(Logflag && d<=0) &&
                     d>=Ymin &&
                     (Ymax==MISSING || d<=Ymax)) {
                  if (ymin > d) ymin= d;
                  if (ymax < d) ymax= d;
               }
            }
            if (Cutoff_flag) {
               d=cutoff_value[test[t]];
               if (
                     d>MISSING &&
                     !(Logflag && d<=0) &&
                     d>=Ymin &&
                     (Ymax==MISSING || d<=Ymax)) {
                  if (ymin > d) ymin= d;
                  if (ymax < d) ymax= d;
               }
            }
            if (Logflag && ymin<=0) {
               cprintf("\r\n%4d %s Logflag=%d ymax=%.3f ymin=%.3f: ",__LINE__, __FILE__, Logflag, ymax, ymin);
               if (getch()==27) farewell(-1);
               putch('\r');
               putch('\n');
            }
         } else {
            find_values(t, 0);
            if (lower_deviation < ymin
                  &&
                  (Ymin==MISSING || lower_deviation>=Ymin)
                  &&
                  !(Logflag && lower_deviation<=0)
               ) ymin= lower_deviation;
            if (upper_deviation > ymax
                  &&
                  (Ymax==MISSING || upper_deviation<=Ymax)
                  &&
                  !(Logflag && upper_deviation<=0)
               ) ymax= upper_deviation;
            if (Cutoff_flag && Graph_type==BAR_GRAPH && Median_flag==2) {
               d=cutoff_value[test[t]];
               if (
                     d>MISSING &&
                     !(Logflag && d<=0) &&
                     d>=Ymin &&
                     (Ymax==MISSING || d<=Ymax)) {
                  if (ymin > d) ymin= d;
                  if (ymax < d) ymax= d;
               }
            }
         }
      }
/*
      if ((!cycle || ymax>Ymax) && Ymax !=MISSING && !(Logflag && Ymax==0)) ymax= Ymax;
      if ((!cycle || ymin<Ymin) && Ymin !=MISSING && !(Logflag && Ymin==0)) ymin= Ymin;
*/
      if (Ymin==MISSING && !Logflag && ymin>0 && Zero) ymin=0;
   }
   if (ymax==MISSING || ymin==ymax) {
      cprintf("\r\nNo valid data found\r\npress RETURN to re-try\r\npress Esc to quit\r\n");
      if (getkey(0)==RETURN) return 0;
      if (!result) farewell(pic_no);
      clrscr();
   }
   if (Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO || (ymin>0 && Logflag)) {
      if (ymax*ymin<=0) {
         cprintf("\r\n%4d %s Logflag=%d ymax=%.3f ymin=%.3f: ",__LINE__, __FILE__, Logflag, ymax, ymin);
         if (getch()==27) farewell(-1);
         putch('\r');
         putch('\n');
      }
      if (ymax / (ymin+0.0000001) <6 && ymax*ymin>0) {
         ymin=sqrt(ymax * ymin)/2.5;
         ymax=ymin*6;
      }
   }
   if (Logflag && ymin<=0) {
      cprintf("\r\n%4d %s Logflag=%d ymax=%.3f ymin=%.3f: ",__LINE__, __FILE__, Logflag, ymax, ymin);
      if (getch()==27) farewell(-1);
      putch('\r');
      putch('\n');
   }
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d %s: cycle=%d ymin=%.4f ymax=%.4f",
            __LINE__, __FILE__, cycle, ymin, ymax);
   #endif
   fflush(stdin);
   return 1;
}

int find_N(void) {
   int n, t, row;
   float d, true_min, true_max;
   true_max=MISSING;
   true_min=-MISSING;
   for (t=n=0; t<nooftests;t++) {
      if (Trace==1)       putch('N');
      N[t]=0;
      if (test[t]==-1) continue;
      for (row=0;row<rows;row++) {
         if ((d=getdata( row, t, FALSE))==MISSING) continue;
         if (accept_data(d,0)) N[t]++;
         if (true_min > d) true_min= d;
         if (true_max < d) true_max= d;
      }
      if (n<N[t]) n=N[t];
   }
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d:true_min=%.4f true_max=%.4f",
            __LINE__, true_min, true_max);
   #endif
   return n;
}

int find_row(int row, int t, int flag) {  /* returns case_number with rank_number t */
   int i, case_number=-1;
   if (flag) for (i=0; i<rows; i++) {
      if (rank0[i][t]==row) {
         case_number=i;
         break;
      }
   }
   else for (i=0; i<rows; i++) {
      if (rank[i][t]==row) {
         case_number=i;
         break;
      }
   }
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d %s:find_row: rank=%d i=%d rows=%d case_number=%d",
            __LINE__, __FILE__, row, i, rows, case_number);
   #endif
   return case_number;
}

void find_values(int t, int logflag) {
   if (N[t]==0 ||
	 (Stat==-1 && !(
	    Graph_type==PERCENTAGE_POSITIVE ||
	    Graph_type==PEARSON ||
	    Graph_type==SPEARMAN ||
	    Graph_type==RELATIVE_RISK||
	    Graph_type==ODDS_RATIO)
	 )
      ) {
      upper_deviation= 0;
      central_value= 0;
      lower_deviation= 0;
      return;
   } else if (Mean_flag ||
         Graph_type==PERCENTAGE_POSITIVE ||
         Graph_type==PEARSON ||
         Graph_type==SPEARMAN ||
         Graph_type==RELATIVE_RISK||
         Graph_type==ODDS_RATIO) {
      lower_deviation= lower_limit[t];
      central_value= mean[t];
      upper_deviation= upper_limit[t];
   } else if (Median_flag) {
      lower_deviation= i25[t];
      central_value= i50[t];
      upper_deviation= i75[t];
   }
   else upper_deviation=lower_deviation=central_value;
   if (logflag) lower_deviation=(lower_deviation>0)?log(lower_deviation):MISSING;
   if (logflag) central_value=(central_value>0)?log(central_value):MISSING;
   if (logflag) upper_deviation=(upper_deviation>0)?log(upper_deviation):MISSING;
}

float getdata(int row, int t, int flag) {
   if (flag) return data0[row][t];
/*
   return (casnum[row][t]>-1)?data0[casnum[row][t]][test[t]]:MISSING;
   return (casnum[row][t]<rows)?data0[casnum[row][t]][test[t]]:MISSING;
*/
   return data0[casnum[row][t]][test[t]];
}

void get_diff(int t0, int t1) {
   int row;
   double sx, sxx;
   float d, d0, d1;
      for (row=0,sx=0,sxx=0,n_diff=0; row<rows; row++) {
         d0=getdata( row, t0, FALSE);
         d1=getdata( row, t1, FALSE);
         if (accept_data(d0,0) && accept_data(d1,0)) {
            n_diff++;
            if(Logflag) {
               d0=log(d0);
               d1=log(d1);
            }
            d=d1-d0;
            sx+=d;
            sxx+=d*d;
         }
      }
      mean_diff= (n_diff>0)      ? sx/n_diff              :0;
      var_diff= (n_diff>1)      ?(sxx-sx*sx/n_diff)/(n_diff-1):0;
      sd_diff= (var_diff>0) ? sqrt(var_diff)  :0;
      if (Sd_flag) {
         lower_limit_diff= (n_diff>0) ? mean_diff-sd_diff:0;
         upper_limit_diff= (n_diff>0) ? mean_diff+sd_diff:0;
      } else if (Sem_flag) {
         lower_limit_diff= (n_diff>0) ? mean_diff-sd_diff / sqrt(n_diff):0;
         upper_limit_diff= (n_diff>0) ? mean_diff+sd_diff / sqrt(n_diff):0;
      } else lower_limit_diff=upper_limit_diff=mean_diff;
      if (Logflag) {
         lower_limit_diff= exp(lower_limit_diff);
         mean_diff= exp(mean_diff);
         upper_limit_diff= exp(upper_limit_diff);
      }
}

float get_ile(int start, int column,double ile, int flag) {
   float value, low_value, high_value;
   double factor;
   int n,score;
   n=(flag)
      ? valid_data0[column]
      : ((Outliers_flag)
         ? valid_data[column]
         : N[column]);
   if (n==0) return MISSING;
   if (n<3 || ile<0.01 || ile>0.99) return MISSING;

   if (n<4 || ile<0.01 || ile>0.99) ile=(Centile)?Centile:0.5;

   score=(int)((n+1)*ile);
   factor=(n+1)*ile - (double)score;
   if (!score) value=(flag)
      ? getdata(rank0[start][column], column, TRUE)
      : getdata(rank[start][column], column, FALSE);
   else {
      low_value=(score-1<n)
         ? ((flag)
            ? getdata(rank0[start+score-1][column], column, TRUE)
            : getdata(rank[start+score-1][column], column, FALSE))
         : getdata(rank[start+n-1][column], column, FALSE);
      high_value= (score<n)
         ? ((flag)
            ? getdata(rank0[start+score][column], column, TRUE)
            : getdata(rank[start+score][column], column, FALSE))
         : getdata(rank[start+n-1][column], column, FALSE);
      value= low_value + (factor * (high_value - low_value));
   }
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,
            "\n%4d %s %3u ile=%.2lf n=%3d score=%d factor=%.3lf %5.2f =%5.2f -> %5.2f",
            __LINE__, __FILE__,column, ile, n, score, factor, low_value, high_value, value);
   #endif
   return value;
}

int get_mean(int warningflag) {
   int n, t, i, row;
   double sx, sxx;
   float d;
   for (t=0;t<nooftests;t++) {
      if (Trace==1)          putch('1');
      if (!N[t]) {
         mean[t]=0;
         sd[t]=0;
         continue;
      }
      cprintf("-%s",numbertolotus(test[t]+1,"  "));
/*
cprintf("\r\n%s %4d : nooftests=%d test[%d]=%d ",__FILE__, __LINE__, nooftests, t, test[t]);
if (getch()==ESC) farewell(-1);
putch('\r');
putch('\n');
*/
      for (row=0,n=0,sx=0,sxx=0;row<rows;row++) {
         d=getdata( row, t, FALSE);
         if ((i=accept_data(d,warningflag))==-1) return 0;
         if (i) {
            if(Logflag) d=log(d);
            sx+=d;
            sxx+=d*d;
            n++;
         }
      }
      mean[t]= (n>0)    ? sx/n              :0;
      var[t]= (n>1)     ?(sxx-sx*sx/n)/(n-1):0;
      sd[t]= (var[t]>0) ? sqrt(var[t])      :0;
      if (Sd_flag) {
         lower_limit[t]= (n>0) ? mean[t]-sd[t]:0;
         upper_limit[t]= (n>0) ? mean[t]+sd[t]:0;
      } else if (Sem_flag) {
         lower_limit[t]= (n>0) ? mean[t]-sd[t] / sqrt(n):0;
         upper_limit[t]= (n>0) ? mean[t]+sd[t] / sqrt(n):0;
      } else lower_limit[t]=upper_limit[t]=mean[t];
      #ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n%4d %s t=%d test=%d sx=%.3lf sxx=%.3lf n=%d valid_data[%d]=%d mean[t]=%8.2lf sd[t]=%8.2lf ll=%8.2lf ul=%8.2lf",
               __LINE__, __FILE__,t, test[t], sx, sxx, n, t, valid_data[t], mean[t], sd[t], lower_limit[t], upper_limit[t]);
      #endif
      if (Logflag) {
         lower_limit[t]= exp(lower_limit[t]);
         mean[t]= exp(mean[t]);
         upper_limit[t]= exp(upper_limit[t]);
      }
      #ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n%4d %s t=%d test=%d sx=%.3lf sxx=%.3lf n=%d valid_data[%d]=%d mean[t]=%8.2lf sd[t]=%8.2lf ll=%8.2lf ul=%8.2lf",
               __LINE__, __FILE__,t, test[t], sx, sxx, n, t, valid_data[t], mean[t], sd[t], lower_limit[t], upper_limit[t]);
      #endif
   }
   return 1;
}

double ilevalue(int n, double level, double centile) {
   double result;
   n+=1;
   if (centile>0) {
      if (level==0.25) result= (n * centile - ( sqrt(n*centile*(1-centile))))/n;
      if (level==0.50) result= centile; 
      if (level==0.75) result= (1 + n * centile + ( sqrt(n*centile*(1-centile))))/n;
      }
   else result = level;
   return result;
}

void make_centiles(int nooftests, int outliers_flag){
   int t, start, n;
   for (t=0; t < nooftests ; t++) {
      if (Trace==1)       putch('6');
/*
      start=(outliers_flag)?0:class[0][t];
*/
      start=0;
      n=(outliers_flag)?valid_data[t]:N[t];
      if (n) {
         min[t]= getdata(rank[start][t],t,FALSE);
         if ((Graph_type==TTEST || Graph_type==BAR_GRAPH) && Median_flag==2) {
            i25[t]= get_ile(start, t, 0.10, FALSE);
            i50[t]= get_ile(start, t, 0.50, FALSE);
            i75[t]= get_ile(start, t, 0.90, FALSE);
         } else {
            i25[t]= get_ile(start, t, ilevalue( n, 0.25, Centile), FALSE);
            i50[t]= get_ile(start, t, ilevalue( n, 0.50, Centile), FALSE);
            i75[t]= get_ile(start, t, ilevalue( n, 0.75, Centile), FALSE);
         }
         max[t]= getdata(rank[start+n-1][t], t, FALSE);
         #ifdef TRACE
            if (Trace==2) fprintf(tracefile,"\n%4d %s: min=%.4f %.4f %.4f %.4f max=%.4f",
                  __LINE__, __FILE__,min[t], i25[t], i50[t], i75[t], max[t]);
         #endif
      }
   }
   fflush(stdin);
}

int process_data(void) {
   if (calc_flag) return 1;
   if (Stat!=-1 && !get_mean(1)) return -1;
   if (nooftests==2 && !Prob_flag) get_diff(0, 1);
   if (!find_minmax(0)) return -1;
   ranges=0;
   if (!find_N()) {;
      cprintf("\r\nNo valid data found ymin=%.3f true_min=%.3f ymax=%.3f true_max=%.3f\r\nPress any key to continue",
         ymin, true_min, ymax, true_max);
      if (getch()==ESC) farewell(-1);
      return -1;
   }
   if (!sort_flag && sort_required) rank_data(nooftests);
/*
   logflag=Logflag;
*/
   calc_flag=TRUE;
   if ((Graph_type==TTEST || Graph_type==BAR_GRAPH) && Prob_flag) write_prob_strings(FALSE);
   fflush(stdin);
   return 1;
}

void qs(int t, int rows) {
   int row, valid_rows, rankno;
   float d;
   valid_rows=(symbolstart>-1)?symbolstart:rows;
   for (row=0;row<valid_rows; row++) {
      temprank[row]=row;
      d=getdata(row,t, FALSE);
      tempdata[row]=(d>MISSING)?d:-MISSING;
   }
   qs0(0,valid_rows-1);
   for (rankno=0;rankno<valid_rows; rankno++) rank[rankno][t]=temprank[rankno];
}

void qs0(int left, int right) {
   register int i, j;
   int r;
   float x,y;
   i=left;   j=right;
   x=tempdata[(left+right)/2];
   do {
      while (tempdata[i] <  x  && i < right) i++;
      while (x < tempdata[j]   && j > left ) j--;
      if (i<=j) {
         y= tempdata[i];               r=temprank[i];
         tempdata[i] = tempdata[j];    temprank[i]=temprank[j];
         tempdata[j] = y;              temprank[j]=r;
         i++;j--;
      }
   } while (i<=j);
   if (left < j) qs0(left,j);
   if (i < right) qs0(i,right);
}

void rank_data(int nooftests) {
   int row, t;
   for (t=0;t<nooftests;t++) {
      if (Trace==1)       putch('9');
      if (test[t]==-1) continue;
      if (!valid_data[t]) continue;
      if (Subselect) {
         cprintf("%s",numbertolotus(test[t]+1,"  "));
         for (row=0;row<rows;row++) rank[row][t]=-1;
         qs(t, rows);
      } else for (row=0;row<rows;row++) rank[row][t]=rank0[row][test[t]];
   }
   #ifdef TRACE
      if (Trace==2) {
         fprintf(tracefile,"\n%4d %s:rank[][]-array",__LINE__, __FILE__);
         for (row=0;row<rows;row++) {
           fprintf(tracefile,"\nrow %4d ",row);
           for (t=0;t<nooftests;t++) if (test[t]!=-1) fprintf(tracefile,"%3d", rank[row][t]);
         }
         for (row=0;row<=rows;row++) {
           fprintf(tracefile,"\nscore %4d ",row);
           for (t=0;t<nooftests;t++) {
              if (test[t]==-1) continue;
              if (getdata(rank[row][t],t, FALSE) >MISSING) fprintf(tracefile,"%8.2f",
                 getdata(rank[row][t],t,FALSE));
              else fprintf(tracefile,"%8s","...");
           }
         }
         fprintf(tracefile,"\n\ndata0\n        ");
         for (t=0;t<columns;t++) fprintf(tracefile,"%8d",t);
         for (row=0;row<=rows;row++) {
           fprintf(tracefile,"\nrow %4d ",row);
           for (t=0;t<columns;t++) {
              if (data0[row][t]>MISSING) fprintf(tracefile,"%8.3f",data0[row][t]);
              else fprintf(tracefile,"%8s","...");
           }
         }
      }
   #endif
   make_centiles(nooftests,Outliers_flag);
/*
   if (Show) if (!show_centiles()) return 0;
*/
   rank_medians();
   sort_flag=TRUE;
   fflush(stdin);
}

void rank_medians(void) {
int t;
   for (t=0;t<nooftests; t++) {
      if (!valid_data[t] || test[t]==-1) {
         temprank[t]=-1;
         tempdata[t]=-MISSING;
         continue;
      }
      temprank[t]=t;
      tempdata[t]=i50[t];
   }
   qs0(0,nooftests-1);
   for (t=0;t<nooftests; t++) medianrank[nooftests-t-1]=temprank[t];
   #ifdef TRACE
      if (Trace==2) {
         fprintf(tracefile,"\n%4d %s rank_medians",__LINE__, __FILE__);
         for (t=0;t<nooftests; t++) fprintf(tracefile,"\nrank=%2d t=%2d test=%2d median=%12.3lf",
            t, medianrank[t], test[medianrank[t]], i50[medianrank[t]]);
      }
   #endif
}

void pearson(int logflag1, int logflag2) {
int t, row;
int outcome, outcome_test;
float d1, d2;
double ci, sx, sy,sxx, syy, sxy, root1, root2, m, xm, ym, z, z1, z2;
   ci=(Sem_flag)?1.0:1.96;
   open_logfile(ttlog);
   if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) {
      fprintf(logfile,"\\\"filename:\"\"%s\"\n",filename);
      fprintf(logfile,"\\\"logtransformation first test(s):\"\"%s\"\n",(logflag1)?"YES":"NO");
      fprintf(logfile,"\\\"Pearson correlation\"\"correlating test:\"\"%s\"\"logtransformed:\"\"%s\"\n",testname[Outcome_test],(logflag2)?"YES":"NO");
   } else for (t=0;t<nooftests;t++) {
      if (test[t]==-1) continue;
      fprintf(logfile,"\"%s\"\n", testname[test[t]]);
   }
   if (Subselect && Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) {
      for (t=0;t<nooftests;t++) {
         if (test[t]==-1) continue;
         fprintf(logfile,"\\%-20s ", quoted(buffer,testname[test[t]]));
         fprintf(logfile,"%-20s\n", quoted(buffer,evaltext[t]));
      }
    }
    if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) {
       if (*First_title && Outcome_test>-1) fprintf(logfile, "%s\n", quoted(buffer,First_title));
       if (*Second_title && Outcome_test>-1) fprintf(logfile, "%s\n", quoted(buffer,Second_title));
       fprintf(logfile, "%s\n", quoted(buffer,y_axis_title));
       fprintf(logfile,"\\\"testname\"\"N\"\"LL\"\"Pearson r\"\"UL\"\n");
    }
    outcome=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)?0:nooftests;
    if (calc_flag!=2 && ((Outcome_test>-1  && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) || nooftests>1)) do {
       ymin=1;
       ymax=-1;
       outcome_test=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)?test[outcome]:Outcome_test;
       if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) cprintf("\r\nCalculating Pearson correlation coefficients\r\nprocessing test : ");
       else cprintf("\r\ntest %-12s versus test: ",testname[outcome_test]);
       for (t=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)?outcome+1:0;t<nooftests;t++) {
         if (valid_data[t]==0 || test[t]==-1) {
            zeroflag[t]=1;
            continue;
         }
         zeroflag[t]=0;
         mean[t]=lower_limit[t]=upper_limit[t]=0;
         cprintf("%s ", numbertolotus(test[t]+1,"  "));
          for (row=sx=sy=sxx=syy=sxy=N[t]=0;row<rows;row++) {
            d1=getdata(row, t,FALSE);
/*
            d2=(casnum[row][t]>-1)?data0[casnum[row][t]][outcome_test]:MISSING;
            d2=getdata(row,outcome_test,TRUE);
*/
            d2=data0[casnum[row][t]][outcome_test];
            if (d1<=MISSING || d2<=MISSING) continue;
            if ((logflag1 && d1<=0) || (logflag2 && d2<=0)) continue;
            if (logflag1) d1=log(d1);
            if (logflag2) d2=log(d2);
            N[t]++;
            sx+=d1;
            sxx+=d1*d1;
            sy+=d2;
            syy+=d2*d2;
            sxy+=d1*d2;
         }
         if (N[t]<3) continue;
         xm=sx/N[t];
         ym=sy/N[t];
         root1=sxx-N[t]*xm*xm;
         root2=syy-N[t]*ym*ym;
         if (root1<=0 || root2<=0) continue;
         m=mean[t] = (sxy-N[t]*xm*ym)/sqrt(root1*root2);
         if (m>-1 && m<1 && N[t]>3) {
            z=log((1+m)/(1-m))/2;
            z1=z-ci/sqrt(N[t]-3);
            z2=z+ci/sqrt(N[t]-3);
            lower_limit[t]=(exp(2*z1)-1)/(exp(2*z1)+1);
            upper_limit[t]=(exp(2*z2)-1)/(exp(2*z2)+1);
         } else lower_limit[t]=upper_limit[t]=m;
         if (Outcome_test>-1) fprintf(logfile,"%-20s %3d %14.4lf %14.4lf %14.4lf\n",
                                 quoted(buffer,testname[test[t]]), N[t],lower_limit[t],mean[t], upper_limit[t]);
         else cormatrix[m2v(outcome,t,nooftests)]=mean[t];
         if (ymin>lower_limit[t]) ymin=lower_limit[t];
         if (ymax<upper_limit[t]) ymax=upper_limit[t];
      }
      outcome++;
   } while (outcome<nooftests-1);
   if (Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER) {
      calc_dist_array(dist, cormatrix, nooftests);
      for (t=0; t<nooftests; t++) {
         for (outcome=0; outcome<nooftests; outcome++) fprintf(logfile,"%8.4f",t==outcome?1.0:cormatrix[m2v(outcome,t,nooftests)]);
         fprintf(logfile,"\n");
      }
   }
   calc_flag=2;
   if (ymin>0) ymin=0;
   if (ymax<0) ymax=0;
   if (ymin==ymax) {
      ymin=-1;
      ymax=1;
   }
   if (nooftests>2) {
      if      (cormatrix_flag)         dotmatrix();
      else if (Graph_type==DENDROGRAM) dendrogram(cormatrix, nooftests);
      else if (Graph_type==XYCLUSTER)  xycluster(dist, nooftests);
   }
   fclose(logfile);
   fflush(stdin);
   clrscr();
}

void spearman(void) {
int t, column, casnum0, casnum1, r, rr, rank1, rank2, rankcolumn, col0, col1, tiecount, tieflag, rankvalue, valid_ranks, tied_rank,n;
int target, found;
int outcome, outcome_test;
double d1, d2, ci, sxx, syy, sdd, tie, m, z, z1, z2;
   ci=(Sem_flag)?1.0:1.96;
   open_logfile(ttlog);
   if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) fprintf(logfile,"\\\"filename:\"\"%s\"\n\\\"Spearman correlation\"\"correlating test:\"\"%s\"\"interval\"\"%s\"\n", filename, testname[Outcome_test],Sem_flag?"SE":"95% CI");
   else for (t=0;t<nooftests;t++) {
      if (test[t]==-1) continue;
      fprintf(logfile,"\"%s\"\n", testname[test[t]]);
   }
   if (Subselect && Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) {
      for (t=0;t<nooftests;t++) {
         if (test[t]==-1) continue;
         fprintf(logfile,"\\%-20s ", quoted(buffer,testname[test[t]]));
         fprintf(logfile,"%-20s\n", quoted(buffer,evaltext[t]));
      }
    }
    if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) {
       if (*First_title) fprintf(logfile, "%s\n", quoted(buffer,First_title));
       if (*Second_title) fprintf(logfile, "%s\n", quoted(buffer,Second_title));
       fprintf(logfile, "%s\n", quoted(buffer,y_axis_title));
       fprintf(logfile,"\\\"testname\"\"N\"\"LL\"\"Spearman r\"\"UL\"\n");
    }
    outcome=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)?0:nooftests;
    if (
          calc_flag!=3 &&
          (
             (
                Outcome_test>-1  && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER
             )
             || nooftests>1
          )
       ) do {
       ymin=1;
       ymax=-1;
       outcome_test=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)?test[outcome]:Outcome_test;
       if (Outcome_test>-1 && Graph_type!=DENDROGRAM && Graph_type!=XYCLUSTER) cprintf("\r\nCalculating Spearman correlation coefficients\r\nprocessing test : ");
       else cprintf("\r\ntest %-12s versus test: ",testname[outcome_test]);
       for (t=(Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER)
            ?outcome+1
            :0;
          t<nooftests;t++) {
/*
         if (kbhit) {
            fclose(logfile);
            fflush(stdin);
            return;
         }
*/
         if (valid_data[t]==0 || test[t]==-1) {
            zeroflag[t]=1;
            continue;
         }
         zeroflag[t]=0;
         N[t]=rows;
         sxx=syy=mean[t]=lower_limit[t]=upper_limit[t]=0;
         cprintf("%s ", numbertolotus(test[t]+1,"  "));
         for (casnum1=0;casnum1<rows;casnum1++) extra_rank[casnum1][0]=extra_rank[casnum1][1]=extra_rank[casnum1][2]=-1;

   /* FILL extra_rank-ARRAY MET RANKS */

         for (casnum1=0;casnum1<rows;casnum1++) {
            casnum0=casnum[casnum1][t];
            d1=data0[casnum0][test[t]];
            d2=data0[casnum0][outcome_test];
            rank1=find_row(casnum0, test[t], TRUE);
            rank2=find_row(casnum0, outcome_test, TRUE);
            extra_rank[casnum1][0]=(d1>MISSING)?rank1:-1;
            extra_rank[casnum1][1]=(d2>MISSING)?rank2:-1;
         }

   /* CHECK FOR MISSING VALUES */

         for (rankcolumn=0; rankcolumn<2; rankcolumn++) {
            col0=rankcolumn?0:1;
            col1=rankcolumn?1:0;
            for (casnum1=0;casnum1<rows;casnum1++) {
               if (extra_rank[casnum1][col0]>-1 && extra_rank[casnum1][col1] ==-1) {
                  rank1=extra_rank[casnum1][col0];
                  extra_rank[casnum1][col0]=-1;
                  for (r=0; r<rows; r++) {
                     if (extra_rank[r][col0] == -1) continue;
                     if (extra_rank[r][col0] == rank1) extra_rank[r][col0] = -1;
                     else if (extra_rank[r][col0] > rank1) extra_rank[r][col0] --;
                  }
               }
            }
         }
         for (casnum1=0;casnum1<rows;casnum1++) if (extra_rank[casnum1][0]==-1) N[t]--;

   /* ADJUST RANKS FOR MISSING VALUES */

         for (rankcolumn=0; rankcolumn<2; rankcolumn++) {
            for (casnum1=0;casnum1<rows;casnum1++) if (extra_rank[casnum1][0]!=-1) extra_rank[casnum1][2]=extra_rank[casnum1][rankcolumn];
            for (target=0; target<rows; target++) {
               for (casnum1=found=0;casnum1<rows;casnum1++) {
                     if (extra_rank[casnum1][0]==-1) continue;
                     if (extra_rank[casnum1][rankcolumn] == target) {
                     found=TRUE;
                     break;
                  }
               }
               if (!found) for (casnum1=0;casnum1<rows;casnum1++) if (extra_rank[casnum1][rankcolumn] > target) extra_rank[casnum1][2]--;
            }
            for (casnum1=0;casnum1<rows;casnum1++) if (extra_rank[casnum1][0]!=-1) extra_rank[casnum1][rankcolumn]=extra_rank[casnum1][2];
         }

   /* TIES: CORRECT RANKS (TIMES 10 TO AVOID FLOATS) AND CALCULATE TIE-CORRECTION: */

         for (rankcolumn=1; rankcolumn>-1; rankcolumn--) {
            column=(rankcolumn==0)?test[t]:outcome_test;
            for (casnum1=0;casnum1<rows;casnum1++) if (extra_rank[casnum1][0]>-1) extra_rank[casnum1][rankcolumn+1]=10*extra_rank[casnum1][rankcolumn];
            tie=0;
            valid_ranks=-1;
            for (rank1=r=tiecount=0; rank1<rows; rank1++) {
               casnum1=rankcolumn?rank0[rank1][outcome_test]:rank[rank1][t];
               if (extra_rank[casnum1][0]==-1) continue;
               valid_ranks++;
               rankvalue=10*valid_ranks;
               tied_rank=valid_ranks;
               d1=data0[casnum1][column];
               for(tiecount=1, r=1, tieflag=TRUE;rank1 + r <rows && tieflag;r++) {
                  casnum1=rankcolumn?rank0[rank1+r][outcome_test]:rank[rank1+r][t];
                  if (extra_rank[casnum1][0]==-1) continue;
                  d2=data0[casnum1][column];
                  if (d1!=d2) tieflag=FALSE;
                  else {
                     rankvalue+= 5;
                     tiecount++;
                  }
               }
               r--;
               if (tiecount>1) {
                  tie+=((double)tiecount*tiecount*tiecount-tiecount)/12.0;
                  for (rr=0;rr<tiecount;rr++) {
                     for (n=0; n<rows; n++) if (extra_rank[n][rankcolumn]==tied_rank) {
                        extra_rank[n][rankcolumn+1]=rankvalue;
                        tied_rank++;
                        break;
                     }
                  }
                  rank1+=r-1;
                  valid_ranks+=tiecount-1;
               }
            }
            if (rankcolumn==1) sxx=((double)N[t]*N[t]*N[t]-(double)N[t])/12.0-(double)tie;
            else syy=((double)N[t]*N[t]*N[t]-(double)N[t])/12.0-(double)tie;
         }

   /* CALCULATE DIFFERENCES IN RANKS AND CALCULATE RHO: */

         for (casnum1=sdd=0; casnum1<rows; casnum1++) {
            if (extra_rank[casnum1][0]==-1) extra_rank[casnum1][0]=-999;
            else {
               extra_rank[casnum1][0]=extra_rank[casnum1][1]-extra_rank[casnum1][2];
               sdd+=(double)((double) extra_rank[casnum1][0] * (double)extra_rank[casnum1][0]/100.0);
            }
         }
         m=mean[t]=sxx*syy>0?(sxx+syy-sdd)/(2*sqrt(sxx*syy)):0;
         if (m<-1 || m>1) {
           cprintf("\r\n%s %4d \r\n error test %d: Rho=%12.4lf",__FILE__, __LINE__, t, mean[t]);
           if (getch()==27) farewell(pic_no);
         }
	 #ifdef TRACE
     if (Trace) fprintf(tracefile,"\n\"%4d %s:n=\"%3d \"%s\" \"%s\" \"sxx=\"%12.4lf \"syy=\"%12.4lf \"sdd=\"%12.4lf \"Rs=\"%12.4lf",
	       __LINE__, __FILE__,N[t], testname[outcome_test], testname[test[t]], sxx, syy, sdd, m);
     #endif
   /* CALCULATE CONFIDENCE INTERVAL: */

         if (m>-1 && m<1 && N[t]>3) {
            z=log((1+m)/(1-m))/2;
            z1=z-ci/sqrt(N[t]-3);
            z2=z+ci/sqrt(N[t]-3);
            lower_limit[t]=(exp(2*z1)-1)/(exp(2*z1)+1);
            upper_limit[t]=(exp(2*z2)-1)/(exp(2*z2)+1);
         } else lower_limit[t]=upper_limit[t]=m;
         if (Outcome_test>-1) fprintf(logfile,"%-20s %3d %14.4lf %14.4lf %14.4lf\n",
            quoted(buffer, testname[test[t]]), N[t],lower_limit[t],mean[t], upper_limit[t]);
         else cormatrix[m2v(outcome,t,nooftests)]=mean[t];

   /* ADJUST MIN AND MAX-VALUES: */

         if (ymin>lower_limit[t]) ymin=lower_limit[t];
         if (ymax<upper_limit[t]) ymax=upper_limit[t];
      }
      if (ymin>0) ymin=0;
      if (ymax<0) ymax=0;
      if (ymin==ymax) {
         ymin=-1;
         ymax=1;
      }
      outcome++;
   } while (outcome<nooftests-1);
   if (Outcome_test==-1 || Graph_type==DENDROGRAM || Graph_type==XYCLUSTER) {
      calc_dist_array(dist, cormatrix, nooftests);
      for (t=0; t<nooftests; t++) {
         for (outcome=0; outcome<nooftests; outcome++)
            fprintf(logfile,"%8.4f",t==outcome?1.0:cormatrix[m2v(outcome,t,nooftests)]);
         fprintf(logfile,"\n");
      }
   }
   calc_flag=3;
   if (nooftests>2) {
      if      (cormatrix_flag)         dotmatrix();
      else if (Graph_type==DENDROGRAM) dendrogram(cormatrix, nooftests);
      else if (Graph_type==XYCLUSTER)  xycluster(dist, nooftests);

   }
   fclose(logfile);
   fflush(stdin);
   clrscr();
}

/* drie routines om diagonaal-symmetrische matrix m (size n x n) als vector uit te lezen */
/* bv voor 6x6-matrix de volgende 15-elementen-vector:
(0,1)(0,2)(0,3)(0,4)(0,5)(1,2)(1,3)(1,4)(1,5)(2,3)(2,4)(2,5)(3,4)(3,5)(4,5)
    -  0   1  2  3  4
    0  -   5  6  7  8
    1  5   -  9 10 11
    2  6   9  - 12 13
    3  7  10 12  - 14
    4  8  11 13 14  -
*/

int m2v(int x, int y, int n) {
/* bv matrix-element (4,2) of het gelijkwaardige element (2,4) staat in vec[10] */
   return (x>y) ? vec0(y,n)+x-y-1
                : vec0(x,n)+y-x-1;
}

void v2m(int v, int *x, int *y, int n) {
/* berekent x en y positie in matrix uit vector-positie v */
/* bv vec[10] bevat de waarde van het matrix-element waarvan x=4 en y=2 */
   int size=v;
   *x=0;
   *y=0;
   n--;
   while (n<=v && n>=0) {
      *x+=n;
      (*y)++;
      v-=n;
      n--;
   }
   *x=*y+1+size-*x;
}

int vec0(int y, int n) {
/* berekent vectorpositie voor de eerste waarde van de y-row in matrix */
/* de waardes 0,5,9,12,en 14 voor n=6 en y is 0,1,2,3 en 4*/
   return (-y*y+2*n*y-y)/2;
}

double t95(int df) {
    double a=3.45/1000,b=2.334,c=.505,n;
	n=(double)(1000.0/df);
	return (1.96+(a*n*n+b*n+c)/1000);
}

void tt_minmax(void) {
   ttmax=ymax;
   ttmin=ymin;
}
/*
void tt_minmax(void) {
   int t, row;
   float d;
   ttmax=MISSING;
   ttmin=-MISSING;
   gotoxy(1,1);
   clreol();
   for (t=0; t<nooftests;t++) putch('-')
   for (t=0; t<nooftests;t++) {
      putch('\b');
      if (Trace==1)       putch('5');
      if (test[t]==-1) continue;
      if (!valid_data[t]) continue;
      for (row=0;row<rows;row++) {
         d=getdata( row, t, FALSE);
         if (
               d>MISSING &&
               !(Logflag && d<=0) &&
               d>=ymin &&
               d<=ymax) {
            if (ttmin > d) ttmin= d;
            if (ttmax < d) ttmax= d;
         }
      }
   }
   if (ttmin<ymin) ttmin=ymin;
   if (ttmax>ymax) ttmax=ymax;
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d: ttmin=%.4f ymin=%.4f ttmax=%.4f ymax=%.4f",
         __LINE__, ttmin, ymin, ttmax, ymax);
#endif
}
*/

double tt_calc(int t0, int t1, int statmode) {
   int df;
   double diff, tval, pooled_variance, pooled_sem, sem_diff;
   if (statmode>2) {       /* NON-PAIRED TEST. 3=one-tailed, 4=two-tailed */
      diff=(Logflag)? log(mean[t1]/mean[t0]): mean[t1]-mean[t0];
      df=valid_data[t0]+valid_data[t1]-2;
      if (df>0) {
         pooled_variance=(var[t0]*(valid_data[t0]-1)+var[t1]*(valid_data[t1]-1))/df;
         pooled_sem=sqrt(pooled_variance*(1.0/valid_data[t0]+1.0/valid_data[t1]));
         if (diff==0) return 1;
         else tval=(pooled_sem>0)?diff/pooled_sem:MISSING;
         return find_prob(tval,df)*(statmode-2);
      } else return MISSING;
   }
   if (statmode<3) {       /* PAIRED TEST 1=one-tailed, 2=two-tailed */
      get_diff(t0, t1);
      df=n_diff-1;
      if (df>0 && n_diff>1) {
         if ((!Logflag && mean_diff==0) || (Logflag && mean_diff==1)) return 1;
         else {
            sem_diff=sqrt(var_diff/n_diff);
            tval=(sem_diff>0)?(Logflag?log(mean_diff):mean_diff)/sem_diff:MISSING;
         }
         return find_prob(tval,df)*statmode;
      } else return MISSING;
   }
   return MISSING;
}

double find_prob(double tval, int df) {
   double p0, p1, result=0.0;
   int i;
   unsigned int tt, t0, t1;
   if (df<=0) return MISSING;
   if (df>30) df=30;
   tt=(df==1)?abs(tval*100):abs(tval*1000);
   if (tt<t_value[0][df]) {
/*
printf("\n%4d %s: tt=%u < t_value[0][df]=%u ??",__LINE__,__FILE__, tt, t_value[0][df]);
if (getch()==27) farewell(-1);
putch('\n');
*/
      result=1;
   }
   else for (i=0;i<7;i++) {
      if (tt>=t_value[i][df] && tt<t_value[i+1][df]) {
         p0=log(t_value[i][0]);
         p1=log(t_value[i+1][0]);
         t0=t_value[i][df];
         t1=t_value[i+1][df];
         result=exp(p0+(tt-t0)*(p1-p0)/(t1-t0))/10000;
/*
printf("\n%4d %s: tt=%u >= t_value[i][df]=%u <t_value[i+1][df]=%u ??",__LINE__,__FILE__, tt, t_value[i][df], t_value[i+1][df]);
printf("\np0=%.4lf p1=%.4lf t0=%u t1=%u result=%.4lf", exp(p0),exp(p1),t0,t1,result);

if (getch()==27) farewell(-1);
putch('\n');
*/
         break;
      }
   }
   if (!result) result=0.00009;
   return result;
}

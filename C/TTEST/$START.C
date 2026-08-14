/*AAAAAAAAAAAAAAAAAAAAAAAAA start-up AAAAAAAAAAAAAAAAAAAAAAAAAAAAA*/
#include "ttest.h"

/* module $start.c */
void allocate(void);
void clear_array1(void);
void farewell(int);
void initialise_arrays(void);
void no_room(void);

void allocate(void) {
   int row, t, textlines;
   long mem[20];
   float *floatpointer;
   int *intpointer;
   char *charpointer;
   double *doublepointer;
   mem[0]=coreleft();
   textlines=20+4*(MAX_COL+1)+SYMBOLS+PSTRING;
   if ((buffer=        (char *)   calloc( BUFFERSIZE,            1))==0) no_room();
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"%5ld buffer\n",getaddress(buffer));
   #endif
   if ((data0=         (float  **) calloc(  rows+1,  sizeof(floatpointer)))==0) no_room();
   mem[1]=coreleft();
/*
   if ((rank=          (int  **)   calloc(  (symbolstart>-1)?symbolstart+1:rows+1, sizeof(intpointer)))==0) no_room();
*/
   if ((rank=          (int  **)   calloc(  rows+1, sizeof(intpointer)))==0) no_room();
   mem[2]=coreleft();
   if ((rank0=         (int  **)   calloc(  rows+1,            sizeof(intpointer)))==0) no_room();
   mem[3]=coreleft();
   if ((extra_rank=    (int  **)   calloc(  rows+1,            sizeof(intpointer)))==0) no_room();
   mem[4]=coreleft();
/*
   if ((casnum=        (int  **)   calloc(  (symbolstart>-1)?symbolstart+1:rows+1,            sizeof(intpointer)))==0) no_room();
*/
   if ((casnum=        (int  **)   calloc(  rows+1,            sizeof(intpointer)))==0) no_room();
   mem[5]=coreleft();
   mem[7]=coreleft();
/*
   for (row=0;row<rows;row++) if ((data0[row]=      (float  *) calloc( MAX_COL+1,  sizeof(float)))==0) no_room();
*/
   for (row=0;row<rows+1;row++) if ((data0[row]=      (float  *) calloc( MAX_COL+1,  sizeof(float)))==0) no_room();
   mem[8]=coreleft();
/*
   for (row=0;row<(symbolstart>-1)?symbolstart+1:rows+1;row++) if ((rank[row]=       (int  *)   calloc( MAX_COL+2,  sizeof(int)))==0) no_room();
*/
   for (row=0;row<rows+1;row++) if ((rank[row]=       (int  *)   calloc( MAX_COL+2,  sizeof(int)))==0) no_room();
   mem[9]=coreleft();
   for (row=0;row<rows+1;row++) if ((rank0[row]=      (int  *)   calloc( MAX_COL+2,  sizeof(int)))==0) no_room();
   mem[10]=coreleft();
   for (row=0;row<rows+1;row++) if ((extra_rank[row]= (int  *)   calloc( 3,        sizeof(int)))==0) no_room();
   mem[11]=coreleft();
/*
   for (row=0;row<(symbolstart>-1)?symbolstart+1:rows+1;row++) if ((casnum[row]=     (int  *)   calloc( MAX_COL+1,  sizeof(int)))==0) no_room();
*/
   for (row=0;row<rows+1;row++) if ((casnum[row]=     (int  *)   calloc( MAX_COL+1,  sizeof(int)))==0) no_room();
   mem[12]=coreleft();
   if ((data0[rows]= (float  *) calloc( MAX_COL,sizeof(float)))==0) no_room();
   mem[13]=coreleft();
/*
   for (row=0; row<MAXNOOFCLASSES+2; row++) if ((class[row]=        (int  *) calloc( (MAX_COL+2),  sizeof(int)))==0) no_room();
   mem[14]=coreleft();
   for (row=0; row<MAXNOOFCLASSES+2; row++) if ((classcounter[row]= (int  *) calloc( (MAX_COL+2),  sizeof(int)))==0) no_room();
   mem[15]=coreleft();
*/
   if ((symbol=      (unsigned char  *) calloc(  rows+1 ,  sizeof(char) ))==0) no_room();
   if ((valid_data=  (int  *) calloc(  MAX_COL+1 ,  sizeof(int) ))==0) no_room();
   if ((valid_data0= (int  *) calloc(  MAX_COL+1 ,  sizeof(int) ))==0) no_room();
   if ((filltype=    (int  *) calloc(  FILL_COL+1,  sizeof(int) ))==0) no_room();
   if ((N=           (int  *) calloc(  MAX_COL+1 ,  sizeof(int) ))==0) no_room();
   if ((medianrank=  (int  *) calloc(  MAX_COL+1,   sizeof(int) ))==0) no_room();
   if ((temprank=    (int  *) calloc(  rows+1,      sizeof(int) ))==0) no_room();
   if ((pstart=      (int  *) calloc(  PSTRING,     sizeof(int) ))==0) no_room();
   if ((pend=        (int  *) calloc(  PSTRING,     sizeof(int) ))==0) no_room();
   if ((cormatrix=   (float *) calloc( (MAX_COL*(MAX_COL-1))/2,sizeof(float) ))==0) no_room();
   if ((dist     =   (float *) calloc( (MAX_COL*(MAX_COL-1))/2,sizeof(float) ))==0) no_room();
   if ((cutoff_value=(float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((min=         (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((max=         (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((i25=         (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((i50=         (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((i75=         (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((lower_limit= (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
   if ((upper_limit= (float *) calloc( MAX_COL+1 ,  sizeof(float) ))==0) no_room();
/*
   if ((tempdata=    (float *) calloc( (symbolstart>-1)?symbolstart+1:rows+1    ,   sizeof(float) ))==0) no_room();
*/
   if ((tempdata=    (float *) calloc( rows+1     ,   sizeof(float) ))==0) no_room();
   if ((fill_min=    (float *) calloc( FILL_COL+1 ,   sizeof(float) ))==0) no_room();
   if ((fill_max=    (float *) calloc( FILL_COL+1 ,   sizeof(float) ))==0) no_room();
   if ((pvalue=      (float *) calloc( PSTRING ,      sizeof(float) ))==0) no_room();
   if ((mean=        (double  *) calloc( MAX_COL+1 ,  sizeof(double) ))==0) no_room();
   if ((var=         (double  *) calloc( MAX_COL+1 ,  sizeof(double) ))==0) no_room();
   if ((sd=          (double  *) calloc( MAX_COL+1 ,  sizeof(double) ))==0) no_room();
   mem[16]=coreleft();
   if ((textpointer= (char *)  calloc( textlines,  (MAX_LEN+1)))==0) no_room();
   if ((textstring=  (char **) calloc( textlines ,  sizeof(charpointer)))==0) no_room();
   if ((testname=       (char **) calloc( (MAX_COL) ,  sizeof(charpointer)))==0) no_room();
   if ((testunit=       (char **) calloc( (MAX_COL) ,  sizeof(charpointer)))==0) no_room();
   if ((evalstring=     (char **) calloc( (MAX_COL) ,  sizeof(charpointer)))==0) no_room();
   if ((evaltext=       (char **) calloc( (MAX_COL) ,  sizeof(charpointer)))==0) no_room();
   if ((symbol_legend=  (char **) calloc( (SYMBOLS) ,  sizeof(charpointer)))==0) no_room();
   if ((extra_text=     (char **) calloc( (10) ,       sizeof(charpointer)))==0) no_room();
   if ((defaultstring=  (char **) calloc( (8) ,        sizeof(charpointer)))==0) no_room();
   if ((columntext=     (char **) calloc( (MAX_COL+1) ,  sizeof(charpointer)))==0) no_room();
   if ((rangetext=      (char **) calloc( (MAX_COL+1) ,  sizeof(charpointer)))==0) no_room();
   if ((pstring=        (char **) calloc( (PSTRING) ,  sizeof(charpointer)))==0) no_room();
   for (t=0;t<textlines;t++) textstring[t]=textpointer+t*(MAX_LEN+1);
   samplenumber_string= textstring[0];
   samplecode_string=   textstring[1];
   for (t=0;t<MAX_COL;t++) {
      testname[t]=  textstring[t+2];
      testunit[t]=  textstring[t+2+  MAX_COL];
      evalstring[t]=textstring[t+2+2*MAX_COL];
      evaltext[t]=  textstring[t+2+3*MAX_COL];
   }
   for (t=0;t<SYMBOLS;t++)  symbol_legend[t]= textstring[t+ 2+4*MAX_COL];
   for (t=0;t<10;t++)       extra_text[t]=    textstring[t+ 2+4*MAX_COL+SYMBOLS];
   for (t=0;t< 8;t++)       defaultstring[t]= textstring[t+12+4*MAX_COL+SYMBOLS];
   for (t=0;t< PSTRING;t++) pstring[t]=       textstring[t+20+4*MAX_COL+SYMBOLS];
   mem[17]=coreleft();
/*
   testname=            textstring+2;
   testunit=            textstring+2 + MAX_COL;
   evalstring=          textstring+2+2*MAX_COL;
   evaltext=            textstring+2+3*MAX_COL;
   symbol_legend=       textstring+2+4*MAX_COL;
   extra_text=          textstring+2+4*MAX_COL+SYMBOLS;
   defaultstring=       textstring+12+4*MAX_COL+SYMBOLS;
*/
   #ifdef TRACE
      if (Trace==2) {
         fprintf(tracefile,"\n%5ld memstart      :%5ld\n", mem[0],0);
         fprintf(tracefile,"%5ld data0           :%5ld\n", mem[1],mem[0]  -mem[1]);
         fprintf(tracefile,"%5ld rank            :%5ld\n", mem[2],mem[1]  -mem[2]);
         fprintf(tracefile,"%5ld rank0           :%5ld\n", mem[3],mem[2]  -mem[3]);
         fprintf(tracefile,"%5ld extrarank       :%5ld\n", mem[4],mem[3]  -mem[4]);
         fprintf(tracefile,"%5ld casnum          :%5ld\n", mem[5],mem[4]  -mem[5]);
/*
         fprintf(tracefile,"%5ld class           :%5ld\n", mem[6],mem[5]  -mem[6]);
         fprintf(tracefile,"%5ld classcounter    :%5ld\n", mem[7],mem[6]  -mem[7]);
*/
         fprintf(tracefile,"%5ld data0[0]        :%5ld\n", mem[8],mem[7]  -mem[8]);
         fprintf(tracefile,"%5ld rank[0]         :%5ld\n", mem[9],mem[8]  -mem[9]);
         fprintf(tracefile,"%5ld rank0[0]        :%5ld\n", mem[10],mem[9] -mem[10]);
         fprintf(tracefile,"%5ld extra_rank0[0]  :%5ld\n", mem[11],mem[10]-mem[11]);
         fprintf(tracefile,"%5ld casnum[0]       :%5ld\n", mem[12],mem[11]-mem[12]);
         fprintf(tracefile,"%5ld data0[rows]     :%5ld\n", mem[13],mem[12]-mem[13]);
/*
         fprintf(tracefile,"%5ld class[0]        :%5ld\n", mem[14],mem[13]-mem[14]);
         fprintf(tracefile,"%5ld classcounter[0] :%5ld\n", mem[15],mem[14]-mem[15]);
*/

         fprintf(tracefile,"%5ld *symbol etc     :%5ld\n", mem[16], mem[15]-mem[16]);
         fprintf(tracefile,"%5ld text            :%5ld\n", mem[17], mem[16]-mem[17]);
         fprintf(tracefile,"bytes requested for %d textlines %d each: %d\n",textlines, MAX_LEN+1, textlines*(MAX_LEN+1));
      }
   #endif
}

void initialise_arrays(void) {
   int i;
   Color=COLOR;
   Combi=0;
   Datasymbol=1;
   Distance=FALSE;
   Dotflag=TRUE;
   Graph_type=GRAPH_TYPE;
   Intra=-1;
   Iqr_flag=1;
   Legend=LEGEND;
   Legendpos=LEGENDPOS;
   Legend_symbol_size=LEGEND_SYMBOL_SIZE;
   Line=0;
   Literal=0;
   Logflag2=0;
   Logflag=0;
   Mean_flag=1;
   Median_flag=0;
   Nn=TRUE;
   Noofclasses=DEFAULTNOOFCLASSES;
   No_ranges=FALSE;
   Outcome_test=0;
   Outliers_flag=TRUE;
   Plotter=PLOTTER;
   Prob_flag=FALSE;
   Ranges_in_header=RANGES_IN_HEADER;
   Rank_column=FALSE;
   Sd_flag=1;
   Sem_flag=0;
   Se_flag=-1;
   Showclasses=3;
   Skipnegatives=0;
   Small=0;
   Stagger=STAGGER;
   Stattext_flag=TRUE;
   Swap=SWAP;
   Subselect=0;
   Symbol_size=-1;
   Textsize=TEXTSIZE;
   User_legend_flag=FALSE;
   X_length=X_LENGTH;
   Clxlength=2000;
   Ycal=YCAL;
   Zero=TRUE;
   Ortho=TRUE;
   Fullname=FALSE;
   Cap=CAP;
   Centile=CENTILE;       /* Centile    0: 25,50,75 centiles
                             Centile 0.75: 75 centile (SE)
                             Centile 0.50: 50 centile (SE)*/
   Cut_off=CUT_OFF;
   Inter_bar=-1;
   Inter_t=-1;
   Stat=STAT;
   Tip=TIP;
   Ymax=MISSING;
   Ymin=MISSING;
   for (i=0; i<10; i++) extra_text[i][0]='\0';
   for (i=0; i<8;  i++) strcpy(defaultstring[i],"@>0");
   for (i=0; i<MAX_COL; i++) {
      evalstring[i][0]='\0';
      sprintf(evaltext[i],"#%i",i);
   }
   strcpy(samplenumber_string,"nr");
   strcpy(samplecode_string,"code");
   for (i=0; i<FILL_COL+1; i++) {
      filltype[i]=-1;
      fill_min[i]=MISSING;
      fill_max[i]=MISSING;
   }
   reset_prob();
}

void clear_array1(void) {
   int row,column;
   for (row=0;row<rows;row++) for (column=0;column<MAX_COL;column++) casnum[row][column]=rows;
}

void no_room(){
   E3;
   getch();
   farewell(-1);
}

void farewell(int pic_no) {
   fclose(0);
   textcolor(LIGHTGRAY);
   textbackground(BLACK);
   clrscr();
   setdisk(startup_drive);
   exit(pic_no);
}


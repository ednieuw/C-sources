 /*     corrcalc  */

#include "corr.h"
#include "alloc.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "conio.h"

#define MISSING -999999.0
#define LAMBDA 1
#define AS_AANTAL 23   /*aantal aslabelingen */

void   calc_corr(void);
void   fill_temp(int i, int j, int z);
void   reken(int i,int j);
int    comp(float* , float* );
double predinterval (double x);
void   teken_graph();
void   spear(int, float* , float*, float* ,float * );
void   free_vector(float* v,int nl,int nh);
float  *vector(int nl, int nh);
float  gammln(float);
float  betacf(float,float,float);
float  betai(float,float,float);
void   print_labels(void);


/*extern void menuver(char** menu,int num, int topy,int topx,int breed);*/
extern void teken3dgraph(void);

extern unsigned long MAX_ROW;
extern unsigned long MAX_COL;
extern int   lijn,lijnxy,conf,THREE_D;
extern int   print, show, num,spearm,lost;
extern float upx,upy,upz;
extern float maxttest;
extern char  outfile[40];
extern char  file[20];
extern float **data;
extern char  **label;
extern int   *SCHERMPTR;
extern float **PP;

/*extern int **rank;*/
extern float *sortx;
extern float *sorty;
extern float **temp;

int    x_as,y_as,z_as;
int    tiex=0,tiey=0;
int    rows, columns;
int    labels,xx,yy,zz,aa,rows_temp;
float  aantal;
float  slope,intercept,ttest,corr,rspearman,probrs,ppst;
float  SEslope,SEintercept;
float  *P;
float  somx,somy;
float  slopehigh,slopelow,intercepthigh,interceptlow,corrlow,corrhigh;
double varx,vary,Sres;
 double facx,facy;



/*************** Bereken corr,a,b,ttest ***************/

void calc_corr(void )
 {
 int i,j;
if(print) fputc(15,stdprn);
/* display_labels();*/
 printf("\nupper x limiet >%0.3f   upper y limiet >%0.3f\n",upx,upy);
 printf("\n columns %d rows %d\n\n",columns,rows);
 if (spearm)
   printf("                                     R         p       Rspearm   PSpearman\n");
 else
   printf("                                     R         p   \n");

if(print)
  {
fprintf(stdprn,"\nupper x limiet >%0.3f   upper y limiet >%0.3f   print p < %0.3f\n",upx,upy,maxttest);
if (spearm)
  fprintf(stdprn,"            R          p      n      slope    intercept     y/x    ttest      df   Rspearm Pspear\n");
 else
  fprintf(stdprn,"            R          p      n      slope    intercept     y/x    ttest      df\n");
  }

for(i=0;i<columns;i++)
 {
 for(j=i+1;j<columns;j++)
  {
   reken(i,j);
	if(bioskey(1)) {getch(); return;}
   printf("\r");

    if (aantal>3 && *P < maxttest)
     {
      if (spearm)
      {
   printf("(%2.0d,%2.0d) %-12.12s <-> %-12.12s % 6.4f %10.2G % 6.4G % 10.2G\n",i+1,j+1,&label[i][1],&label[j][1],corr,*P,rspearman,probrs);
    if (print){
   if(rspearman>1)
   fprintf(stdprn,"(%2.0d,%2.0d) % 6.4f % 10.2G % 5.0f % 10.3G % 10.3G % 10.3G % 8.3f % 5.0f     ---    ---  %-15.15s %-12.12s\n",i+1,j+1,corr,*P,aantal,slope,intercept,somy/somx,ttest,aantal-2,&label[i][1],&label[j][1]);
   else
   fprintf(stdprn,"(%2.0d,%2.0d) % 6.4f % 10.2G % 5.0f % 10.3G % 10.3G % 10.3G % 8.3f % 5.0f % 6.4f %6.2G  %-15.15s %-12.12s\n",i+1,j+1,corr,*P,aantal,slope,intercept,somy/somx,ttest,aantal-2,rspearman,probrs,&label[i][1],&label[j][1]);
       }}
       else
      {
     printf("(%2.0d,%2.0d) %-17.17s <-> %-17.17s  %# 6.4G %# 10.2G\n",i+1,j+1,&label[i][1],&label[j][1],corr,*P);

      if (print)
     fprintf(stdprn,"(%2.0d,%2.0d) % 6.4f % 10.2G % 5.0f % 10.3G % 10.3G % 10.3G % 8.3G % 5.0f  %-13.13s %-13.13s\n",i+1,j+1,corr,*P,aantal,slope,intercept,somy/somx,ttest,aantal-2,&label[i][1],&label[j][1]);
     }

    }
  }
 }
  if (print) tabel();
}


/****************  fill temp  ****************/

 void fill_temp(int i, int j, int z)

{
 int k;
 float vx=0,vy=0,vz=0;

   rows_temp=0;


    for(k=0;k<rows;k++)
      {
	 vx = data[k][i];
	 vy = data[k][j];
	 vz = data[k][z];
     if (vx >= 0 && vy >= 0 && vz >= 0)
      {
	  if (vx<=upx && vy<=upy && vz<=upz)
       {
	 temp[0][rows_temp] = vx;
	 temp[1][rows_temp] = vy;
	 temp[2][rows_temp] = vz;
	 sortx[rows_temp]   = vx;
	 sorty[rows_temp]   = vy;
 if (show) printf("x=%0.3f  y=%0.3f\n",vx,vy);
	++rows_temp;
	}
       }
      }

}

/***************  rekenen ***************/

  void reken(int i,int j)
 {
 register int k;
 double somxx,somxy,somyy,sx,sy,sxy,nop,zz,z1,z2;
 register float vx,vy;
 int a,b,c,d,l,s,t,m,n;
 double sc,st,sr2,rc,rs,r2,p,z,rx,ry,r,v1;
 float tdd,tzd;
 float betai(float,float,float);


 float stucon [34][2] =  {
	{ 1 , 12.706 } ,
	{ 2 ,  4.303 } ,
	{ 3 ,  3.182 } ,
	{ 4 ,  2.776 } ,
	{ 5 ,  2.571 } ,
	{ 6 ,  2.447 } ,
	{ 7 ,  2.365 } ,
	{ 8 ,  2.306 } ,
	{ 9 ,  2.262 } ,
	{10 ,  2.228 } ,
	{11 ,  2.201 } ,
	{12 ,  2.179 } ,
	{13 ,  2.160 } ,
	{14 ,  2.145 } ,
	{15 ,  2.131 } ,
	{16 ,  2.120 } ,
	{17 ,  2.110 } ,
	{18 ,  2.101 } ,
	{19 ,  2.093 } ,
	{20 ,  2.086 } ,
	{21 ,  2.080 } ,
	{22 ,  2.074 } ,
	{23 ,  2.069 } ,
	{24 ,  2.064 } ,
	{25 ,  2.060 } ,
	{26 ,  2.056 } ,
	{27 ,  2.052 } ,
	{28 ,  2.048 } ,
	{29 ,  2.045 } ,
	{30 ,  2.042 } ,
	{40 ,  2.021 } ,
	{60 ,  2.000 } ,
   {120 ,  1.980 } ,
   {1000,  1.960 }  };

 tdd=0.0;
 tzd=0.0;
 probrs=0.0;

   printf("Processing (%d,%d)  ",i+1,j+1);

    fill_temp(i,j,j);

    somx=0;
    somy=0;
    somxx=0;
    somyy=0;
    somxy=0;
    aantal=0;

    for(k=0;k<rows_temp;k++)
    {
    vx=temp[0][k];
    vy=temp[1][k];
      somx+= vx;
      somy+= vy;
      somxx+=vx*vx;
      somyy+=vy*vy;
      somxy+=vx*vy;
      aantal++;
    }
   if(aantal<4) return;

    sxy=(somxy-((somx*somy)/ aantal));
    sx= (somxx-((somx*somx)/ aantal));
    sy= (somyy-((somy*somy)/ aantal));
	varx = sx/(aantal - 1);
	vary = sy/(aantal - 1);

    nop=(sx-LAMBDA*sy)*(sx-LAMBDA*sy)+4*LAMBDA*sxy*sxy;

	 corr = (sxy / (sqrt(sx * sy))) - 0.000001;
	 slope = (LAMBDA * sy - sx + sqrt(nop)) / (2 * LAMBDA * sxy);
	 intercept = (somy / aantal) - (somx / aantal) * slope;
	 ttest = corr * sqrt((aantal-2)/((1 - corr+10e-20)*(1+corr+10e-20)));

  /********* Berekening p ************/

	*P = betai(0.5*(aantal-2),0.5,(aantal-2)/((aantal-2)+ttest*ttest));
	PP[i][j] = *P;
	PP[j][i] = *P;

  /***********************************/

  /***** bepalen bepalen t0.975 uit tabel *****/

   for (n=0;n<34;n++)
	{
	  if ( (aantal - 2) <= stucon[n][0] )
			   { ppst = stucon[n][1];	   break; }
		   ppst = 1.960;
	}

   /******   Britisch medical jounal vol 296 30-04-88 pag 1238 *******/

   Sres = sqrt(fabs( (aantal - 1) * (vary - slope*slope * varx) / (aantal-2) ));
   SEslope = Sres / sqrt(varx * (aantal - 1));
   SEintercept = Sres * sqrt ( (1/aantal) + (somx/aantal)*(somx/aantal) / sx);

   slopehigh = slope + ppst * SEslope;
   slopelow  = slope - ppst * SEslope;
   intercepthigh = intercept + ppst * SEintercept;
   interceptlow  =  intercept - ppst * SEintercept;

   zz = 0.5 * log ( (1 + corr) / ( 1- corr ));
   z1 = zz - (1.96/ sqrt(aantal-3) );
   z2 = zz + (1.96/ sqrt(aantal-3) );
   corrlow = (exp(2 * z1) - 1) / (exp(2 * z1) + 1);
   corrhigh = (exp(2 * z2) - 1) / (exp(2 * z2) + 1);

   /**************************************************************/

   if (spearm)    spear(rows_temp,&tdd,&tzd,&rspearman,&probrs);
		 else rspearman=9.9999;

 }

/******************* compare *********************/

int comp( float *na,  float *nb)
{

if (*na != *nb ) return ((*na > *nb) ? 1: -1);
return (0);
}


/**** Berekening 95 % prediction confidence interval  ******/
/** Britsch medical journal vol 296 pag 1240 **/

double predinterval (double x)
{
double prediction=0;    /*Als prediction = 1 dan prediction interval*/
return(ppst*Sres*sqrt(prediction+(1/aantal)+((x-somx/aantal)*(x-somx/aantal)/((aantal-1)*varx))));
}


/*************** teken plaatje ***************/

  void teken_graph()
 {
// int x_as,y_as,z_as;
 int toggle;
 char nop[12];
 char text[MAX_LEN],text1[MAX_LEN];
 int i,j;
 float maxx,maxy,as_unit;
 float minx,miny;
 float vx,vy,asx,asy;
 double offpixx,offpixy;
 double lx,ly,mx,my,a,b;

float as[AS_AANTAL];
as[0]=0.01;
as[1]=0.02;
as[2]=0.05;
as[3]=0.1;
as[4]=0.2;
as[5]=0.5;
as[6]=1;
as[7]=2;
as[8]=5;
as[9]=10;
as[10]=20;
as[11]=50;
as[12]=100;
as[13]=250;
as[14]=500;
as[15]=1000;
as[16]=2500;
as[17]=5000;
as[18]=10000;
as[19]=20000;
as[20]=100000L;
as[21]=500000L;
as[22]=1000000L;

  clrscr();
  while(kbhit()) getch();
  xx=yy=0;
  strcpy(outfile,"0");
  print_labels();
  printf("Welke rij op de X-as? ");
  gets(nop);
  xx=atoi(nop);
  if (xx >columns || xx<1)  return;
  printf("Welke rij op de Y-as? ");
  gets(nop);
  yy=atoi(nop);
  if (yy>columns || yy<1) return;
  if(THREE_D)
  {
   printf("Welke rij op de Z-as? ");
   gets(nop);
	zz=atoi(nop);
   if (zz >columns || zz<1)  return;
  }
  printf("filename PIC file (RETURN=Q) ");
  gets(nop);
  nop[9]='\0';
  if(nop[0]==0) strcpy(nop,"Q");
  strcat(nop,".PIC");
  strcpy(outfile,nop);
  x_as=xx;
  y_as=yy;
  z_as=zz;
  x_as--;
  y_as--;
  z_as--;
 if(THREE_D)
  {
      fill_temp(xx-1,yy-1,zz-1);
      teken3dgraph();
      return;
  }

  maxx=maxy=0.01;
  reken((int)x_as,(int)y_as);
  if(aantal<3) {printf("Aantal is < 3, Graph gestopt\n");
		 delay(2000); return; }
  picopen();
  for (i=0;i<rows_temp;i++)            /*vind as vermenigvuldigingsfactor*/
  {
   vx=temp[0][i];
   vy=temp[1][i];

     if (vx>maxx)
       maxx=vx;

     if (vy>maxy)
       maxy=vy;
   }
     maxx=maxx*1.05;
     maxy=maxy*1.05;

     for (i=0;i<=AS_AANTAL;i++)
     {
      asx=as[i];
      if (9*asx > maxx)
	 break;
     }

     for (i=0;i<=AS_AANTAL;i++)
     {
      asy=as[i];
      if (9*asy> maxy)
	  break;
      }

     offpixx=H_OFFSET;
     offpixy=V_OFFSET;

     lx=(H_WIDTH-H_OFFSET)*H_SCALE;               /*lengte assen*/
     ly=(V_WIDTH-V_OFFSET)*V_SCALE;

     facx=lx/maxx; /*factor data*/
     facy=ly/maxy;

     moef(offpixx,offpixy);                      /*draw box*/
     teken(lx+offpixx,offpixy);
     teken(lx+offpixx,ly+offpixy);
     teken(offpixx,ly+offpixy);
     teken(offpixx,offpixy);


   for (i=0;i<rows;i++)
   {
	vx= data[i][x_as];
	vy= data[i][y_as];

     if (vx>=0 && vy>=0)
      {
      if (vx<=upx && vy<=upy)
       {
    if (num)     numm((double)vx*facx+offpixx,(double)vy*facy+offpixy,i);
    else         point((double)vx*facx+offpixx,(double)vy*facy+offpixy);
       }
      }
    }

    for (i=0;i<=(int)(maxx/asx);i++)
       {
	mx=(double)((float)i*facx*asx+offpixx);
	my=(double)(offpixy);
	moef(mx,my);
	teken(mx,my+100.0);
	gcvt((float)(i*asx),5,text);
	prnt(mx,my-250.0,1,0,text);
       }


   for (i=0;i<=(int)(maxy/asy);i++)
       {
	mx=offpixx;
	my=(double)((float)i*facy*asy+offpixy);
	moef(mx,my);
	teken(mx+100.0,my);
	gcvt((float)i*asy,5,text);
	prnt(mx-400.0,my-100,1,0,text);
       }

	prntc(offpixx+200,20.0,1,0 ,&label[(int)x_as][1]);
		prnt(100.0,offpixy,1,90,&label[(int)y_as][1]);  /* print assen */


	sprintf(text,"%5.4f",corr);                       /* print regressie data */
	prnt(510.0,2700.0,1,0,"R=");
	prnt(810.0,2700.0,1,0,text);
	sprintf(text,"%5.4f",corrlow);

	prnt(10.0,2550.0,1,0,".95conf");
	sprintf(text,"%5.4f",corrlow);
	sprintf(text1,"%5.4f",corrhigh);
	strcat(text," | ");
	strcat(text,text1);
	prnt(510.0,2550.0,1,0,text);

	sprintf(text,"%5.3f",slope);
	prnt(1800.0,2700.0,1,0,"a=");
	prnt(2100.0,2700.0,1,0,text);
	sprintf(text,"%5.3f",slopelow);
	sprintf(text1,"%5.3f",slopehigh);
	strcat(text," | ");
	strcat(text,text1);
	prnt(1700.0,2550.0,1,0,text);

	sprintf(text,"%5.3f",intercept);
	prnt(3000.0,2700.0,1,0,"b=");
	prnt(3200.0,2700.0,1,0,text);
	sprintf(text,"%5.3f",interceptlow);
	sprintf(text1,"%5.3f",intercepthigh);
	strcat(text," | ");
	strcat(text,text1);
	prnt(2950.0,2550.0,1,0,text);

	sprintf(text,"%5.3f",somy/somx);
	prnt(1210.0,2950.0,1,0,"y/x=");
	prnt(1610.0,2950.0,1,0,text);
	gcvt(aantal,4,text);
	prnt(3110.0,2950.0,1,0,"n=");
	prnt(3310.0,2950.0,1,0,text);

  if (spearm )
   {
	sprintf(text,"%5.4f",rspearman);
	if(rspearman>1) strcpy(text,"---");
	prnt(110.0,2950.0,1,0,"Rs=");
	prnt(510.0,2950.0,1,0,text);
	sprintf(text,"%5.4f",probrs);
	if(rspearman>1) strcpy(text,"---");
	prnt(110.0,2850.0,1,0,"Ps=");
	prnt(510.0,2850.0,1,0,text);
   }
	sprintf(text,"%5.3G",*P);
	prnt(2110.0,2950.0,1,0,"p=");
	prnt(2310.0,2950.0,1,0,text);


   if (lijn)
   {
   a=(double)slope;                               /* teken regressie lijn */
   b=(double)intercept;
   my=b;
   mx=0;
   toggle = 1;

   if(fabs(a) <=1 )
       {
   if (my < 0.0)        {  my = 0.0;     mx=(-b/a);    }
   if (my > maxy)        {  my=maxy;    mx=(my-b)/a;  }
   moef(mx*facx+offpixx,my*facy+offpixy);
	   for(mx = 0; mx < maxx; mx += maxx/50)
	   {
		 my=a*mx+b;
		 if(my>maxy) continue;
		 if (my>0)
		  {
		   if (toggle)   teken(mx*facx+offpixx,my*facy+offpixy);
			 else    moef(mx*facx+offpixx,my*facy+offpixy);
		   toggle=1-toggle;
		  }
	   }
	}
    else
       {
   my=0;
   mx=-b/a;

   if (mx < 0.0)        {  mx = 0.0;    my=b;    }
   if (mx > maxx)        {  mx=maxx;    my=a*mx+b;  }
   moef(mx*facx+offpixx,my*facy+offpixy);
	   for(my = 0; my < maxy; my += maxy/50)
	   {
		 mx=(my-b)/a;
		 if(mx>maxx) continue;
		 if (mx>0)
		  {
		   if (toggle)   teken(mx*facx+offpixx,my*facy+offpixy);
			 else    moef(mx*facx+offpixx,my*facy+offpixy);
		   toggle=1-toggle;
		  }
	   }
	}

  if(lijnxy)
  {                                              /* teken y=x lijn */
   moef(0.0+offpixx,0.0+offpixy);
   if (maxx>maxy)
     {
      mx=maxy*facx+offpixx;
      my=maxy*facy+offpixy;
     }
  else
     {
      mx=maxx*facx+offpixx;
      my=maxx*facy+offpixy;
     }
   teken(mx,my);
   prnt(mx,my-75,1,0,"X=Y");
  }
}  					/* einde teken lijn  */


/********* teken 95% prediction interval lijnen ********/
 if(conf)
 {

	vy=predinterval(0);  /* confidence interval wordt nu berekend*/
	my=vy+intercept;
   if (my < 0.0)   my = 0.0;
   if (my > maxy)  my=maxy;

   moef(0*facx+offpixx,my*facy+offpixy);

   for(vx=0;vx<maxx;vx+=maxx/40)
   {
   vy=predinterval(vx);
   my=vy+slope*vx+intercept;
   if (my < 0.0)      my = 0.0;
   if (my > maxy)     my=maxy;
   teken(vx*facx+offpixx,my*facy+offpixy);
	}

	vy=predinterval(0);
	my=-vy+intercept;
   if (my < 0.0)   my = 0.0;
   if (my > maxy)  my=maxy;

   moef(0*facx+offpixx,my*facy+offpixy);

   for(vx=0;vx<maxx;vx+=maxx/80)
	{
	vy=predinterval(vx);
	my=-vy+slope*vx+intercept;
	if (my < 0.0)      my = 0.0;
	if (my > maxy)     my=maxy;
	teken(vx*facx+offpixx,my*facy+offpixy);
	}
 }
 picend();
}


/*********************  SPEARMAN RANK  ***************/
/*  PAG 507 FF NUMERICAL RECIPES IN C  *******/

static float sqrarg;
#define SQR(a) (sqrarg=(a),sqrarg*sqrarg)


 void spear(n,dd,zd,rsp,probrs)

float *dd,*zd,*rsp,*probrs;
int n;
{

 int j;
 float vard,t,sg,sf,fac,en3n,en,df,aved,*wksp1,*wksp2;
 void sort2(),crank(),free_vector();
 float betai(float,float,float);
float *vector();

 wksp1=vector(1,n+1);
 wksp2=vector(1,n+1);
 for (j=1;j<=n;j++)
  {
   wksp1[j]=sortx[j-1];
   wksp2[j]=sorty[j-1];
  }

 sort2(n,wksp1,wksp2);
 crank(n,wksp1,&sf);

 sort2(n,wksp2,wksp1);
 crank(n,wksp2,&sg);

 *dd=0.0;

 for (j=1;j<=n;j++)
  *dd += SQR(wksp1[j]-wksp2[j]);

 en=n;
 en3n=en*en*en-en;
 aved=en3n/6.0-(sf+sg)/12.0;
 fac=(1.0-sf/en3n)*(1.0-sg/en3n);
 vard=((en-1.0)*en*en*SQR(en+1.0)/36.0)*fac;
 *zd=(*dd-aved)/sqrt(vard);
 /* ----      *probd=erfcc(fabs(*zd)/1.4142136);*/
 *rsp=(0.999999-(6.0/en3n)*(*dd+0.5*(sf+sg)))/fac;
 if((*rsp+1.0)*(1.0-(*rsp))<0) {rspearman= 9.9999; return;}
 t=(*rsp)*sqrt((en-2.0)/((*rsp+1.0)*(1.0-(*rsp))));
 df=en-2.0;
 *probrs=betai(0.5*df,0.5,df/(df+t*t));
 free_vector(wksp2,1,n);
 free_vector(wksp1,1,n);
 }

 void crank (n,w,s)
 float w[],*s;
 int n;
 {
  int j=1,ji,jt;
  float t,rank;

  *s=0.0;
  while( j < n)
    {
     if (w[j+1] != w[j])
       {
	w[j]=j;
	++j;
        }
     else
     {
       for (jt=j+1;jt<=n;jt++)
         if (w[jt] != w[j]) break;
       rank=0.5*(j+jt-1);
       for (ji=j;ji <= (jt-1);ji++)  w[ji]=rank;
       t=jt-j;
       *s += t*t*t-t;
       j=jt;
      }

     }
     if (j == n) w[n] = n;
 }

void sort2(n,ra,rb)
int n;
float ra[],rb[];
{
  int l,j,ir,i;
  float rrb,rra;

  l=(n >> 1)+1;
  ir=n;
  for (;;)
    {
     if (l > 1)
      {
       rra=ra[--l];
       rrb=rb[l];
      }
     else
      {
        rra=ra[ir];
        rrb=rb[ir];
        ra[ir]=ra[1];
        rb[ir]=rb[1];
        if (--ir == 1)
         {
          ra[1]=rra;
          rb[1]=rrb;
          return;
         }
       }

   i=l;
   j=l << 1;
   while (j <= ir)
     {
      if (j < ir && ra[j] < ra[j+1]) ++j;
      if (rra < ra[j])
	{
	 ra[i]=ra[j];
	 rb[i]=rb[j];
	 j += (i=j);
	}
      else j=ir+1;
   }
   ra[i]=rra;
   rb[i]=rrb;
  }
 }

void free_vector(float* v,int nl,int nh)
{
nh=nh;
free((char *) (v+nl));
}

float *vector(int nl, int nh)
{
 float *v;
 v=(float *) malloc((unsigned) (nh-nl-1)*sizeof(float));
 if (!v) { printf("Allocation error in vector\n"); exit(-1); }
 return( v-nl);
 }

 float betai(float a,float b,float x)
 {
  float bt;
  float gammln(float);
  float betacf(float,float,float);

   if(x == 0.0 || x == 1.0)  bt = 0.0;
   else
     bt=exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));
     if(x <(a+1.0)/(a+b+2.0))
       return bt*betacf(a,b,x)/a;
     else
       return 1.0 -bt*betacf(b,a,1.0-x)/b;
   }

   #define ITMAX 100
   #define EPS 3.0e-7

   float betacf(float a,float b,float x)

   {
     float qap,qam,qab,em,tem,d;
     float bz,bm=1.0,bp,bpp;
     float az=1.0,am=1.0,ap,app,aold;
     int m;

     qab=a+b;
     qap=a+1.0;
     qam=a-1.0;
     bz=1.0-qab*x/qap;
     for (m=1;m<=ITMAX;m++)
     {
       em=(float)m;
       tem=em+em;
       d=em*(b-em)*x/((qam+tem)*(a+tem));
       ap=az+d*am;
       bp=bz+d*bm;
       d = -(a+em)*(qab+em)*x/((qap+tem)*(a+tem));
       app=ap+d*az;
       bpp=bp+d*bz;
       aold=az;
       am=ap/bpp;
       bm=bp/bpp;
       az=app/bpp;
       bz=1.0;
       if(fabs(az-aold) < (EPS*fabs(az))) return (az);
     }
     printf("Error in betacf");
     return(-99.0);
  }


  float gammln(float xxx)

  {
   double x,tmp,ser;
  double cof[6]={76.18009173,-86.50532033,24.01409822,
		 -1.231739516,0.120858003e-2,-0.536382e-5};
   int j;

   x=xxx-1.0;
   tmp=x+5.5;
   tmp -= (x+0.5)*log(tmp);
   ser=1.0;
   for (j=0;j<=5;j++)
	{
	x += 1.0;
	ser += cof[j]/x;
	}
  return (-tmp+log(2.50662827465*ser));
  }

void print_labels(void)
{
 int n,colx=0,rowy=5;
 char text[MAX_LEN];
 for(n=0;n<labels;n++)
   {
	itoa(n+1,text,10);
	strcat(text,"    ");
	text[3]=0;
	strcat(text,&label[n][1]);
	text[15]=0;
	swrite(colx,rowy,text,0x1700);
	if(++rowy > 24)  { colx +=16; rowy=5;}
	if(colx>66) break;
   }
}
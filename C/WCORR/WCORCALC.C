 /*     corrcalc  */

#include "wcorr.h"
#include <alloc.h>
HWND hwndlocal;

void   	calc_corr(void);
void   	reken(HWND hwnd,int i,int j);
int 		comp( double *na,  double *nb);
double 	predinterval (double x);
void 		spear(int n,double *dd,double *zd,double *rsp,double *probrs);
double  	gammln(double);
double  	betacf(double,double,double);
double  	betai(double,double,double);
double 	erfcc(double x);
void   	print_labels(void);
void 		sort2(int n,double *ra,double *rb);
void		crank (int n,double *w,double *s);
void 		avevar(double *data, int n, double *ave, double *svar);
void 		tptest(double *data1, double *data2, int n, double *t, double *prob);
void 		tutest(double *data1, double *data2, int nn, double *t, double *prob);
void 		signed_rank_test(double *data1, double *data2, int n, double *zsrt, double *probsrt);
void 		MannWhitney(double *data1, double *data2, int n, double *z, double *probsmw);

int		x_as,y_as,z_as;
double  	rspearman,probrs,ppst,ttest;
double  	SEslope,SEintercept;
double 	tpt,tpprob; //paired ttest t en paired ttest p
double 	tut,tuprob; //unpaired ttest t en paired ttest p
int		nx,ny; 		//teller unpaired samples
double 	probd;
double	zsrt,probsrt,zmw,probsmw;  // wilcoxon signed rank test, Mann-Whitney srt
double  	P;
double  	slopehigh,slopelow,intercepthigh,interceptlow,corrlow,corrhigh,corrslow,corrshigh;
double 	varx,vary,Sres;
double 	facx,facy;

/*************** Bereken corr,a,b,ttest ***************/
void calc_corr(void )
 {
/*
 int i,j;
// display_labels();
fsrintf(text,"\nupper x limiet >%0.3f   upper y limiet >%0.3f   print p < %0.3f\n",upx,upy,maxttest);
if (spearm) {
  sprintf(text,"            R          p      n      slope    intercept     y/x    ttest      df   Rspearm Pspear\n");
  Edprint(text);
  }
 else {
  sprintf(text,"            R          p      n      slope    intercept     y/x    ttest      df\n");
  Edprint(text);
 }
for(i=0;i<columns;i++)
 {
 for(j=i+1;j<columns;j++)
  {
   reken(i,j);
	sprintf(text,"\r\n");
   Edprint(text);

	 if (aantal>3 && *P < maxttest)
	  {
      if (spearm){
			sprintf(text,"(%2.0d,%2.0d) %-12.12s <-> %-12.12s % 6.4f %10.2G % 6.4G % 10.2G\n",
								i+1,j+1,&label[i][1],&label[j][1],corr,*P,rspearman,probrs);
			Edprint(text);
      }
		else [
			sprintf(text,"(%2.0d,%2.0d) %-17.17s <-> %-17.17s  %# 6.4G %# 10.2G\n",
								i+1,j+1,&label[i][1],&label[j][1],corr,*P);
			Edprint(text);
      }
	  }

  }
 }
}
tabel();
*/
}
//***************  rekenen ***************
void  reken(HWND hwnd,int i,int j)
{
 register int k;
 double somxx,somxy,somyy,sx,sy,sxy,nop,zz,z1,z2;
 register double vx,vy;
 int n;
 double tdd,tzd;
 double betai(double,double,double);

 double stucon [34][2] =  {
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

 hwndlocal=hwnd;

 tdd=0.0;
 tzd=0.0;
 probrs=0.0;

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

	 sxy=(somxy-((somx*somy)/ aantal)+10e-20);
	 sx= (somxx-((somx*somx)/ aantal)+10e-20);
	 sy= (somyy-((somy*somy)/ aantal)+10e-20);
	 varx = sx/(aantal - 1);
	 vary = sy/(aantal - 1);

	 nop=(sx-LAMBDA*sy)*(sx-LAMBDA*sy)+4*LAMBDA*sxy*sxy;

	 corr = (sxy / (sqrt(sx * sy))) - 1E-10;
	 slope = (LAMBDA * sy - sx + sqrt(nop)) / (2 * LAMBDA * sxy);
	 intercept = (somy / aantal) - (somx / aantal) * slope;
	 ttest = corr * sqrt((aantal-2)/((1 - corr+10e-20)*(1+corr+10e-20)));
	// Statistics W.L. Hays by Holt,Rinehart and Winston SBN 03 910025 1 1970 pag529
	// and numerical recipes  in C Pag 504
  //********* Berekening p ************

	P = betai(0.5*(aantal-2),0.5,(aantal-2)/((aantal-2)+ttest*ttest));
//	PP[i][j] = P;
//	PP[j][i] = P;

  //***********************************

  //***** bepalen bepalen t0.975 uit tabel *****

	for (n=0;n<34;n++)
	{
	  if ( (aantal - 2) <= stucon[n][0] )
				{ ppst = stucon[n][1];	   break; }
			ppst = 1.960;
	}

	//******   Britisch medical jounal vol 296 30-04-88 pag 1238 *******

	Sres = sqrt(fabs( (aantal - 1) * (vary - slope * slope * varx) / (aantal-2) ));
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
//	if (TRUE)//Spearm)
		{
		spear(rows_temp,&tdd,&tzd,&rspearman,&probrs);
		//ttest voor paired samples
		tptest((double*)temp[0],(double*)temp[1], rows_temp, &tpt, &tpprob);
		signed_rank_test((double*)temp[0],(double*)temp[1], rows_temp, &zsrt, &probsrt);
		MannWhitney((double*)data[i],(double*)data[j], MAX_ROW, &zmw, &probsmw);
		//ttest voor unpaired samples
		tutest((double*)data[i],(double*)data[j], MAX_ROW, &tut, &tuprob);
		if(rspearman<9)
			{
			if(fabs(rspearman)==1) zz=20;
			else 						  zz = 0.5 * log ( (1 + rspearman) / ( 1- rspearman ));
			z1 = zz - (1.96/ sqrt(aantal-3) );
			z2 = zz + (1.96/ sqrt(aantal-3) );
			corrslow = (exp(2 * z1) - 1) / (exp(2 * z1) + 1);
			corrshigh = (exp(2 * z2) - 1) / (exp(2 * z2) + 1);
			}
		}
  //		 else rspearman=9.9999;
 }

//******************* compare *********************

int comp( double *na,  double *nb)
{
if (*na != *nb ) return ((*na > *nb) ? 1: -1);
return (0);
}


//**** Berekening 95 % prediction confidence interval  ******
//** Britsch medical journal vol 296 pag 1240 **
double predinterval (double x)
{
double prediction=0;    /*Als prediction = 1 dan prediction interval*/
return(ppst*Sres*sqrt(prediction+(1/aantal)+((x-somx/aantal)*(x-somx/aantal)/((aantal-1)*varx))));
}


//*************** teken plaatje ***************
/*
void teken_graph(HWND hwnd,HDC hdc,struct ASCAL *ascal,int rowx,int rowy)
 {
 int toggle;
 char nop[12];
 char text[80],text1[80];
 int i,j;
 double maxx,maxy;
 double minx,miny;
 double vx,vy,asx,asy;
 double offpixx,offpixy;
 double lx,ly,mx,my,ml,mh,a,b;
 float xpos,ypos;

 int row1=9800,row2=9500,row3=9200,row4=8900,
	  col1=ascal->X_Offset,col2=3300,col3=5000,col4=8500;
 col3=col3;
 hwnd=hwnd;
 minx=ascal->X_Min;
 maxx=ascal->X_Max;
 miny=ascal->Y_Min;
 maxy=ascal->Y_Max;
 Piccolor(hdc,BLUE,1);
 if(rspearman==MISSING) Print(hdc,5000,5000,1,0,"ALLOCATION ERROR");
*/
/*
// Print(hdc,ascal->X_Offset,ascal->Y_Offset/3,2,0,(char far*) label[rowx]+1);
// Print(hdc,600,ascal->Y_Offset,2,90,(char far*)label[rowy]+1);  // print assen
 PrintBottom(hdc,ascal->X_Offset,0,2,0,(char far*) label[rowx]+1);
 PrintTop(hdc,0,ascal->Y_Offset,2,90,(char far*)label[rowy]+1);  //print assen

 sprintf(text,"% s %0.3G","Ps =",probrs);
 if(rspearman>1) strcpy(text,"-");
 Printis(hdc,col1,row1,1,0,text);

 sprintf(text,"% s %0.3G","Pp =",P);
 Printis(hdc,col1,row2,1,0,text);

 gcvt(aantal,4,text1);
 sprintf(text,"% s %s","n =",text1);
 Printis(hdc,col1,row3,1,0,text);

 sprintf(text,"% s %+0.3f","y/x =",somy/somx);
 Printis(hdc,col1,row4,1,0,text);

 sprintf(text,"% s % 0.4f (%+5.4f <-> %+5.4f)","Rs =",rspearman,corrslow,corrshigh);
 if(rspearman>1) strcpy(text,"    ?    ");
 Printis(hdc,col2,row1,1,0,text);

 sprintf(text,"% s % 0.4f (%+5.4f <-> %+5.4f)","Rp =",corr,corrlow,corrhigh);
 Printis(hdc,col2,row2,1,0,text);

 sprintf(text,"% s % 0.4f (%+5.3f <-> %+5.3f)","a =",slope,slopelow,slopehigh);
 Printis(hdc,col2,row3,1,0,text);

 sprintf(text,"% s % 0.4f (%+5.3f <-> %+5.3f)","b =",intercept,interceptlow,intercepthigh);
 Printis(hdc,col2,row4,1,0,text);

 sprintf(text,"% s % 0.3G","p Ttest =",tpprob);
 Printis(hdc,col4,row1,1,0,text);

 sprintf(text,"% s % 0.3G","p Wilcoxon =",probsrt);
 Printis(hdc,col4,row2,1,0,text);

// sprintf(text,"% s % 0.3G","p MannWhitney =",probsmw);
// Printis(hdc,col4,row3,1,0,text);

 sprintf(text,"% s % 0.3G","Vary/Varx =",LAMBDA);
 Printis(hdc,col4,row4,1,0,text);

 Piccolor(hdc,BLACK,1);

	lp.lopnStyle   = PS_SOLID;
	lp.lopnWidth.x = 2;
	lp.lopnWidth.y = 0;              // y-dimension not used
	if(ascal->Color!=255)	lp.lopnColor = dwColor[GREEN];//ascal->Color];
	else							lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
	hpen = SelectObject(hdc, CreatePenIndirect(&lp));

	if (Line_regr)
	{
	a=(double)slope;                               // teken regressie lijn
	b=(double)intercept;
	my=b;
	mx=minx;
	toggle = 0;

	if(fabs(a) <=1 )
		 {
	if (my < miny)        {  my=miny;    mx=(-b/a);    }
	if (my > maxy)        {  my=maxy;    mx=(my-b)/a;  }
	for(mx = minx; mx <= maxx; mx += (maxx-minx)/200)
		{
		 my=a*mx+b;
		 if(mx>maxx) continue;
		 if(my>maxy) continue;
		 if(mx<minx) continue;
		 if(my<miny) continue;
		  {
			xpos=Data_X_2screen(mx,ascal);
			ypos=Data_Y_2screen(my,ascal);
			if (toggle) Draw(hdc,xpos,ypos);
			 else       Move(hdc,xpos,ypos);
			 toggle=1;
		  }
		}
	}
 else
  {
   my=miny;
   mx=-b/a;

	if (mx < minx)        {  mx=minx;    my=b;    }
   if (mx > maxx)        {  mx=maxx;    my=a*mx+b;  }
	for(my = miny; my <= maxy; my += (maxy-miny)/200)
		{
		 mx=(my-b)/a;
		 if(mx>maxx) continue;
		 if(my>maxy) continue;
		 if(mx<minx) continue;
		 if(my<miny) continue;
		 if (mx>minx)
		  {
			xpos=Data_X_2screen(mx,ascal);
			ypos=Data_Y_2screen(my,ascal);
			if (toggle) Draw(hdc,xpos,ypos);
			 else       Move(hdc,xpos,ypos);
         toggle=1;
		  }
	   }
	}
  xpos=ypos=-999;
  DeleteObject(SelectObject(hdc, hpen));
  lp.lopnWidth.x = 1;
  if(ascal->Color!=255)	lp.lopnColor = dwColor[BLACK];//ascal->Color];
  else							lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
  hpen = SelectObject(hdc, CreatePenIndirect(&lp));
  toggle=0;
  if(Line_xy)
  {                                              //teken y=x lijn
	if (minx<miny)   ml=miny;
	else  			  ml=minx;
	if (maxx>maxy)   mh=maxy;
	else  			  mh=maxx;
	for(mx = ml; mx < mh; mx += (mh-ml)/200)
		{
		 xpos=Data_X_2screen(mx,ascal);
		 ypos=Data_Y_2screen(mx,ascal);
		 if (toggle) Draw(hdc,xpos,ypos);
		 else       Move(hdc,xpos,ypos);
		 toggle=1;
		}
	if(xpos>0 && ypos>0) Print(hdc,xpos,ypos,1,0,"X=Y");
  }
}  					// einde teken lijn
	DeleteObject(SelectObject(hdc, hpen));
	lp.lopnWidth.x = 1;
	if(ascal->Color!=255)	lp.lopnColor = dwColor[CYAN];//ascal->Color];
	else							lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
	hpen = SelectObject(hdc,CreatePenIndirect(&lp));
*/


// ********* teken 95% prediction interval lijnen ********
/*
 if(	Line_prob)
  {
	toggle=0;
	mx=minx;
	vy=predinterval(minx);  // confidence interval wordt nu berekend
	my=vy+slope*mx+intercept;
	if (my < miny)  my=miny;
	if (my > maxy)  my=maxy;
	for(vx=minx;vx<=maxx;vx+=(maxx-minx)/200)
		{
		vy=predinterval(vx);
		my=vy+slope*vx+intercept;
		if (my < miny)  		continue;
		if (my > maxy)  		continue;
		xpos=Data_X_2screen(vx,ascal);
		ypos=Data_Y_2screen(my,ascal);
		if (toggle) Draw(hdc,xpos,ypos);
		else        Move(hdc,xpos,ypos);
		toggle=1;
		}
	toggle=0;
	mx=minx;
	vy=predinterval(mx);
	my=-vy+slope*mx+intercept;//vy+intercept;
	if (my < miny)  my=miny;
	if (my > maxy)  my=maxy;
	for(vx=minx;vx<=maxx;vx+=(maxx-minx)/200)
		{
		vy=predinterval(vx);
		my=-vy+slope*vx+intercept;
		if (my < miny)    continue;
		if (my > maxy)    continue;
		xpos=Data_X_2screen(vx,ascal);
		ypos=Data_Y_2screen(my,ascal);
		if (toggle) Draw(hdc,xpos,ypos);
		else        Move(hdc,xpos,ypos);
		toggle=1;
		}
 }
DeleteObject(SelectObject(hdc, hpen));
}

*/

/*********************  SPEARMAN RANK  ***************/
/*  PAG 507 FF NUMERICAL RECIPES IN C  *******/

static double sqrarg;
#define SQR(a) (sqrarg=(a),sqrarg*sqrarg)


void spear(int n,double *dd,double *zd,double *rsp,double *probrs)
{
 int j;
 double t,sg,sf,en3n,en,df,*wksp1,*wksp2,sx,sy;
 zd=zd;
 wksp1=(double *) farmalloc((n+5)*sizeof(double));
 if (!wksp1) { MessageBox(hwndlocal,"Error","ALLOCATION ERROR (wksp1)",MB_OK);rspearman= 9.9999; return; }
 wksp2=(double *) farmalloc((n+5)*sizeof(double));
 if (!wksp2) { MessageBox(hwndlocal,"Error","ALLOCATION ERROR (wksp2)",MB_OK);rspearman= 9.9999; return; }

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
 for (j=1;j<=n;j++)     *dd += SQR(wksp1[j]-wksp2[j]);
/* en=n;
 en3n=en*en*en-en;
 aved=en3n/6.0-(sf+sg)/12.0; 			//expectation value of D = sum squared difference of ranks
 fac=(1.0-sf/en3n)*(1.0-sg/en3n);
 vard=((en-1.0)*en*en*SQR(en+1.0)/36.0)*fac;   //variance of D
 *zd=(*dd-aved)/sqrt(vard);                    //number of sd's
 probd=erfcc(fabs(*zd)/1.4142136);            // and significance
*/
if(n<4) {rspearman= 9.9999;farfree(wksp2);farfree(wksp1);return;	}
 en=n;
 en3n=(en*en*en-en)/12;
 sx=en3n-sf/12;
 sy=en3n-sg/12;
 *rsp=(sx+sy-*dd)/(2*sqrt(sx*sy+1E-10));
// *rsp = ((aved-*dd)/(2*sqrt((en3n/en-sf/en)*(en3n/en-sg/en))));
 // Deze formule uit Siegel nonparametric statistics pag 206 ev



 // *rsp=(0.99999999999999-((6.0/en3n)) * (*dd+0.5*(sf+sg)))/fac; //tie correction
// *rsp=(0.99999999999999-((*dd)*6.0/en3n)); //no tie correction
// if((*rsp+1.0)*(1.0-(*rsp))<=0)
// {rspearman= 9.9999;farfree(wksp2);farfree(wksp1);return;	}

// *rsp=(1.0-(6.0/en3n) * (*dd+0.5*(sf+sg)))/fac;
// if(fabs(*rsp)>=1.0)	*rsp=1.0-1E-10*(int)(*rsp);
 df=en-2.0;
 t=(*rsp)*sqrt(df/((*rsp+1.0)*(1.0+1E-10 - (*rsp))));
 *probrs=betai(0.5*df,0.5,df/(df+t*t));
 farfree(wksp2);
 farfree(wksp1);
 }
//-------------------------
 void crank (int n,double *w,double *s)
 {
  int j=1,ji,jt;
  double t,rank;

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
		 for (jt=j+1;jt<=n;jt++)	if (w[jt] != w[j]) break;
		 rank=0.5*(j+jt-1);
		 for (ji=j;ji <= (jt-1);ji++)  w[ji]=rank;
		 t=(double)(jt-j);
		 *s += t*t*t-t;
		 j=jt;
		}
	 }
  if (j == n) w[n] = n;
 }
//-------------------------------------------------------
void sort2(int n,double *ra,double *rb)
{
  int l,j,ir,i;
  double rrb,rra;

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

double betai(double a,double b,double x)
 {
  double bt;
//  double gammln(double);
//  double betacf(double,double,double);

	if(x == 0.0 || x == 1.0)  bt = 0.0;
	else
	  bt=exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));
	  if(x <(a+1.0)/(a+b+2.0))
		 return (bt*betacf(a,b,x)/a);
	  else
		 return (1.0 -bt*betacf(b,a,1.0-x)/b);
	}

	#define ITMAX 100
	#define EPS 3.0e-7

double betacf(double a,double b,double x)
	{
	  double qap,qam,qab,em,tem,d;
	  double bz,bm=1.0,bp,bpp;
	  double az=1.0,am=1.0,ap,app,aold;
     int m;

     qab=a+b;
     qap=a+1.0;
     qam=a-1.0;
	  bz=1.0-qab*x/qap;
	  for (m=1;m<=ITMAX;m++)
     {
		 em=(double)m;
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
//     printf("Error in betacf");
     return(-99.0);
  }

double erfcc(double x)
  {
	double t,z,ans;
	z=fabs(x);
	t = 1.0 / (1.0 + 0.5 * z);
	ans=t*exp(-z * z -1.26551223 +	t * ( 1.00002368 +
												t * ( 0.37409196 +
												t * ( 0.09678418 +
												t * (-0.18628806 +
												t * ( 0.27886807 +
												t * (-1.13520398 +
												t * ( 1.48851587 +
												t * (-0.82215223 +
												t *  0.17087277)))))))));
	return(x >= 0.0 ? ans : 2.0 - ans);
  }

double gammln(double xxx)
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
 char text[80];
 for(n=0;n<labels;n++)
   {
	itoa(n+1,text,10);
	strcat(text,"    ");
	text[3]=0;
	strcat(text,(char*)label[n]+1);
	text[15]=0;
//	swrite(colx,rowy,text,0x1700);
	if(++rowy > 24)  { colx +=16; rowy=5;}
	if(colx>66) break;
	}
}
/******************** p tabel **********************/
  void tabel()

 {
/*
 int a,pag,i,j,k,l;
 float p;
 int col_pag=15;

  printf("\n\n\n\n       Printing P tabel\n\n");
  fprintf(stdprn,"         * = p < E-3       ! = p < E-6\n");

  pag=labels / col_pag;
  for(k=0;k<=pag;k++)
	{
		fprintf(stdprn,"           ");
		for(i=k*col_pag;i< (k+1)*col_pag;i++)
	{
	if(i >= labels) break;
	fprintf(stdprn,"%7.0d",i+1);
	}
      fprintf(stdprn,"\n");

		fprintf(stdprn,"            ");
		for(i=k*col_pag;i< (k+1)*col_pag;i++)
	{
	if( i >= labels) break;
	fprintf(stdprn,"%7.6s",&label[i][1]);
	}
      fprintf(stdprn,"\n");

	  for(j=0;j<labels;j++)
     {
      fprintf(stdprn,"%7.7s(%2.0d) ",&label[j][1],j+1);
      for(i=k*col_pag;i< (k+1)*col_pag;i++)
	{
	   if ( i >= labels )  break;
	   if(i==j) fprintf(stdprn,"    -  ");
	else
	{
	 p = PP[j][i] + 1e-38;

	if(p<1e-6) fprintf(stdprn," !%5.0E",p);
	 else
	  if(p<1e-3) fprintf(stdprn," *%5.0E",p);
	 else
		fprintf(stdprn,"%7.0E",p);

	}

      }
		if (i<=labels) fprintf(stdprn," %9.9s(%2.0d) ",&label[j][1],j+1);

      fprintf(stdprn,"\n");
		if(bioskey(1)) {getch(); return;}
	  }
		fprintf(stdprn,"           ");
		for(i=k*col_pag;i< (k+1)*col_pag;i++)
	{
	if(i >= labels) break;
	fprintf(stdprn,"%7.0d",i+1);
	}
		fprintf(stdprn,"\n");

      fprintf(stdprn,"            ");
      for(i=k*col_pag;i< (k+1)*col_pag;i++)
	{
	if( i >= labels) break;
	fprintf(stdprn,"%7.6s",&label[i][1]);
	}
		fprintf(stdprn,"\n\n");
	}
  fprintf(stdprn,"\r");
  fputc(27,stdprn); fputc(64,stdprn);
  */
}


void avevar(double *data, int n, double *ave, double *svar)
{
 int j;
 double s;
 *ave=(*svar)=0.0;
 for (j=1;j<=n;j++) *ave += data[j];
 *ave /= n;
 for(j=1;j<=n;j++) {
	s=data[j]-(*ave);
	*svar += s*s;
 }
 *svar /= (n-1);
}

void tptest(double *data1, double *data2, int n, double *t, double *prob)
{
//Numerical recipes in C pag485
 int j;
 double var1,var2,ave1,ave2,sd,df,cov=0.0;

 memmove(data1+1,data1,n*sizeof(double));
 memmove(data2+1,data2,n*sizeof(double));

 avevar(data1,n,&ave1,&var1);
 avevar(data2,n,&ave2,&var2);
 for(j=1;j<=n;j++) 	cov += (data1[j]-ave1) * (data2[j]-ave2);
 cov /= df = n-1;
 if( ((var1 + var2 - 2.0 * cov)/n) <=0) sd =1E-10;
 else  sd = sqrt((var1 + var2 - 2.0 * cov)/n);
 *t = (ave1 - ave2) / sd;
 *prob = betai(0.5 * df, 0.5, df/(df+(*t)*(*t)));

 memmove(data1,data1+1,n*sizeof(double));
 memmove(data2,data2+1,n*sizeof(double));
}

void tutest(double *data1, double *data2, int nn, double *t, double *prob)
{
//Numerical recipes in C pag485
// extern int		nx,ny; 		//teller unpaired samples
 int n;
 double var1,var2,ave1,ave2,df;

 nx=0;
 for(n=0;n<=nn;n++)	if(data1[n]!=MISSING)	 sortx[++nx]=data1[n];
 ny=0;
 for(n=0;n<=nn;n++)	if(data2[n]!=MISSING)	 sorty[++ny]=data2[n];

 avevar((double*)sortx,nx,&ave1,&var1);
 avevar((double*)sorty,ny,&ave2,&var2);
 *t = (ave1 - ave2) / sqrt(var1/nx+var2/ny);
 df=sqrt(var1/nx+var2/ny) / (sqrt(var1/nx)/(nx-1)+sqrt(var2/ny)/(ny-1));
// *prob = betai(0.5 * df, 0.5, df/(df+sqrt(fabs(*t)))); fout in boek??
 *prob = betai(0.5 * df, 0.5, df/(df+(*t)*(*t)));
}

void signed_rank_test(double *data1, double *data2, int n, double *z, double *probsrt)
{
 int j,aantal;
 double daantal;
 double sf,Som,somplus=0,sommin=0;

 aantal=0;
 for(j=0;j<n;j++)
  {
	if(data1[j]-data2[j])
	{
	sortx[++aantal]=data1[j]-data2[j];
	sorty[aantal]=fabs(sortx[aantal]);
	}
  }
 if(aantal>1)
  {
	sort2(aantal,(double*)sorty,(double*)sortx);
	crank(aantal,(double*)sorty,&sf);

	for(j=1;j<=aantal;j++)
		{
		if(sortx[j]<0) 	sommin += sorty[j];
		else             somplus += sorty[j];
		}
	if(fabs(sommin)<fabs(somplus)) Som=sommin; else Som=somplus;
	daantal=aantal;
	*z= (Som - daantal*(daantal+1.0)/4.0) / (sqrt(daantal*(daantal+1.0)*(2.0*daantal+1.0)/24.0));
  }
  else *z=0;
  *probsrt=erfcc(fabs(*z)/1.41421356);
}


void MannWhitney(double *data1, double *data2, int n, double *z, double *probsmw)
{
 int j,aantal;
 double sf;
 double somx=0,somy=0,m=0,nn=0;

 aantal=0;
 for(j=0;j<n;j++)
  {
	if(data1[j]!=MISSING){	sortx[++aantal]=data1[j];	sorty[aantal]=0; m++; }
	if(data2[j]!=MISSING){	sortx[++aantal]=data2[j];	sorty[aantal]=1; nn++;}
	}
 if(aantal>1)
  {
	sort2(aantal,(double*)sortx,(double*)sorty);
	crank(aantal,(double*)sortx,&sf);

	for(j=1;j<=aantal;j++)
		{
		if(!sorty[j]) 	  somx += sortx[j];
		else             somy += sortx[j];
		}
  *z=(somx+0.5-m*(m+nn+1)/2) / sqrt(m*nn*(m+nn+1)/12 );
  *probsmw=erfcc(fabs(*z)/1.41421356);

  }
  else {
			*z=0;
			*probsmw=erfcc(fabs(*z)/1.41421356);
		 }
}

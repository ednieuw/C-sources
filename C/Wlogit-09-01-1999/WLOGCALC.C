 /*     wlogcalc  */

#include "wlogit.h"
#include <alloc.h>

void 	teken_graph(HWND hwnd,HDC hdc,struct ASCAL *ascal,int rowx,int rowy);
double 	logit(double meet);
double 	rechte(double meet);
int     Check_Log(double check,int err);
void 	regressie(HWND hwnd,int i, int j);
void 	LogitRegressie(HWND hwnd,int i, int j);
int 	calc(HANDLE hgraph, HWND hwndgraph, int rowx, int rowy);
void   	fill_temp(HWND,int ,int );
int 	linaire_regressie(HWND hwnd, int i, int j);
void 	Bereken_spreiding(double sx,double sy);

double 	Sres,SEslope,SEintercept,slopehigh,slopelow;
double  intercepthigh,interceptlow,corrlow,corrhigh;
HWND 	hwndlocal;
struct 	ASCAL  axcal;
/*************** teken plaatje ***************/

void teken_graph(HWND hwnd,HDC hdc,struct ASCAL *ascal,int rowx,int rowy)
 {
 short  toggle,flag,moved=FALSE;
 char   text[200],text1[80];
 int 	i,n;
 double maxx,maxy;
 double minx,miny;
 double lowy,highy;
 double inc;
 double vy;
 double mx,my,a,b,tus;
 float  xpos,ypos;
 int row1=9500,row2=9000;//,col1=200,col2=3700,col3=5000,col4=8500;

 hwnd=hwnd;
 minx=ascal->X_Min;
 maxx=ascal->X_Max;
 lowy=miny=ascal->Y_Min;
 highy=maxy=ascal->Y_Max;
 PrintBottom(hdc,ascal->X_Offset,0,2,0,(char far*) label[rowx]+1);
 PrintTop(hdc,0,ascal->Y_Offset,2,90,(char far*)label[rowy]+1);  /* print assen */
 i=0;
	switch (Regressie)
		{
		 case IDC_POLYNOOM :
//			 lowy=Poly(minx);
//			 highy=Poly(maxx);
			 sprintf(text,"R=%6.3f              %d degree polynoom",corr,polynoomgraad);
			 Print(hdc,ascal->X_Offset,row1,1,0,text);
			 strcpy(text,"x=");
			 for(n=polynoomgraad;n>1;n--)
			 {
			   sprintf(text1,"%6.6g y^%d + ", polycoeif[n],n);
			   strcat(text,text1);
			   if(strlen(text)>180) break;
			 }
			 sprintf(text1,"%6.6g y + %6.6g", polycoeif[1],polycoeif[0]);
			 strcat(text,text1);
			 Print(hdc,ascal->X_Offset/10,row2,0.75,0,text);
			 break ;
		 case IDC_LINEAIR :
			 lowy=rechte(minx);
			 highy=rechte(maxx);
			 sprintf(text,"R=%6.4f ",corr);
			 Print(hdc,ascal->X_Offset,row1,1,0,text);
			 sprintf(text,"Slope=%6.4f    intercept=%6.4f ",slope,intercept);
			 Print(hdc,ascal->X_Offset,row2,1,0,text);
			 if(lowy>highy) { b=lowy; lowy=highy; highy=b;}
			 break ;
		 case IDC_RID :
//			 lowy=rechte(minx);
//			 highy=rechte(maxx);
			 sprintf(text,"R=%6.4f ",corr);
			 Print(hdc,ascal->X_Offset,row1,1,0,text);
			 sprintf(text,"Slope=%6.4f    intercept=%6.4f ",slope,intercept);
			 Print(hdc,ascal->X_Offset,row2,1,0,text);
			 if(lowy>highy) { b=lowy; lowy=highy; highy=b;}
			 break ;
		 case IDC_LOGIT :
			 if(C<D) { maxy=D; miny=C;} else {maxy=C; miny=D;}
			 if(miny==0.0) miny=ascal->Y_Min/10;
			 sprintf(text,"R=%6.4f   slope=%6.4f  intercept=%6.4f",corr,slope,intercept);
			 Print(hdc,ascal->X_Offset,row1,1,0,text);
			 sprintf(text,"Blank=%6.4f    Bmax=%6.4f    X(1/2 Bmax)=%6.4f",C,D,exp(-intercept/slope));
			 Print(hdc,ascal->X_Offset,row2,1,0,text);
			 a=exp(slope*log(minx+1E-10)+intercept);
			 lowy=(a*D+C)/(1+a);
			 a=exp(slope*log(maxx*.999)+intercept);
			 highy=(a*D+C)/(1+a);
			 if(C>D) {b=lowy; lowy=highy; highy=b;}
			 break ;
		  }

	lp.lopnStyle   = PS_SOLID;
	lp.lopnWidth.x = 2;
	lp.lopnWidth.y = 0;              // y-dimension not used
	if(lowy>highy) 		{tus=lowy; lowy=highy; lowy=tus;}
	if(lowy<=0) 		lowy=miny;
	if(highy<=0) 		highy=maxy*2;
	if(highy>maxy*2) 	highy=2*maxy;
	if(ascal->Color!=255)	lp.lopnColor = dwColor[GREEN];//ascal->Color];
	else					lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
	hpen = SelectObject(hdc, CreatePenIndirect(&lp));
	if (Line_regr)
	{
	flag=toggle=FALSE;
	if(ascal->Y_Astrans==1) 	{ lowy=log(lowy); highy=log(highy);}
	inc=(highy-lowy)/100;
	if(Regressie==IDC_POLYNOOM) inc=(highy-lowy)/200;
	for(vy = lowy*1.00; vy <= highy; vy += inc)
		{
		 if(vy>0.95*highy&&i)	 {inc/=5; i=0;}
		 if(ascal->Y_Astrans==1) my=exp(vy); else my=vy;
		 switch (Regressie)
			{
			 case IDC_POLYNOOM :	mx=Poly(my);			  				break;
			 case IDC_LINEAIR :     mx=((my-intercept)/slope);				break;
			 case IDC_RID :      	mx=((my-intercept)/slope);				break;
			 case IDC_LOGIT :    	mx=exp((logit(my)-intercept)/slope);	break;
			}
		 if(mx<=0) continue;
		 xpos=Data_X_2screen(mx,ascal);
		 ypos=Data_Y_2screen(my,ascal);
		 if(mx<ascal->X_Min) {xpos=Data_X_2screen(ascal->X_Min,ascal);flag=TRUE;}
		 if(mx>ascal->X_Max) {xpos=Data_X_2screen(ascal->X_Max,ascal);flag=TRUE;}
		 if(my<ascal->Y_Min) {ypos=Data_Y_2screen(ascal->Y_Min,ascal);flag=TRUE;}
		 if(my>ascal->Y_Max) {ypos=Data_Y_2screen(ascal->Y_Max,ascal);flag=TRUE;}
		 if(!toggle) flag=FALSE;
		 if(flag&&moved)  {Move(hdc,xpos,ypos); 			flag=FALSE; continue;}
		 if(flag)		  {Draw(hdc,xpos,ypos); moved=TRUE; flag=FALSE; continue;}
//		 if (mx>=minx && mx<=maxx)
		  {
			if (toggle) Draw(hdc,xpos,ypos);
			 else       Move(hdc,xpos,ypos);
			moved=FALSE;
			toggle=1;
		  }
		}
	DeleteObject(SelectObject(hdc, hpen));
	lp.lopnWidth.x = 1;
	if(ascal->Color!=255)	lp.lopnColor = dwColor[BLACK];//ascal->Color];
	else					lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
	hpen = SelectObject(hdc, CreatePenIndirect(&lp));

	}  					/* einde teken lijn  */
 DeleteObject(SelectObject(hdc, hpen));
 lp.lopnWidth.x = 1;
 if(ascal->Color!=255)	lp.lopnColor = dwColor[CYAN];//ascal->Color];
 else					lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
 hpen = SelectObject(hdc,CreatePenIndirect(&lp));
 DeleteObject(SelectObject(hdc, hpen));
}

//*************************  LOGIT ********************************

double logit(double meet)
{
 if((meet-C)/(D-meet)<=0) return(MISSING);
 return(log((double)((meet-C)/(D-meet))));
}

//*************************  RECHTE ********************************

double rechte(double meet)
{
 if(Regressie==IDC_RID) meet*=meet;
 return(slope * meet + intercept);
}
//*************************** REGRESSIE ***************************
void regressie(HWND hwnd,int i, int j)
{
 switch (Regressie)
	{
	 case IDC_POLYNOOM : 	Polynoom(hwnd, i, j);	  		break;
	 case IDC_LINEAIR :
	 case IDC_RID :         linaire_regressie(hwnd, i, j); 	break;
	 case IDC_LOGIT : 		LogitRegressie(hwnd, i, j);		break;
	}
}

/*************************** Logit Regressie ***************************/
void LogitRegressie(HWND hwnd,int i, int j)
{
register int k;
register double Vx,vy;
double   somxx,somxy,somyy,sx,sy,sxy;
double   vx[MAX_YK_PNT+1];
double   xmax,ymax,ymin,yc;
double   factor=1.1,maalfactor=1.01;
double   corroud = 0;
int      loop=1,iter;

if(!berekend)
{
 maxregel=0;
 lastregel=1; //nodig bij uitrekenen om tellers te resetten
 xmax=ymax=D=yc=0.0;
 ymin=1E10;
 corr=1.0;
 if(rows_temp<3 || aantal<3) fill_temp(hwnd,i,j);
 somx=0;    somy=0;    somxx=0;    somyy=0;    somxy=0;    aantal=0;
 for(k=0;k<rows_temp;k++)
  {
  if(ykpunten[k]==TRUE)
	 {
		 vy=temp[1][k];
		 Vx=temp[0][k];
		 aantal++;
		 somx+=Vx; somy+=vy; somxy+=Vx*vy; somxx+=Vx*Vx;
		}
  }
 sxy=(somxy-(somx*somy / aantal));
 sx= (somxx-(somx*somx / aantal));
 slope = sxy / sx;
 if(slope<0) yc=-1; else yc=1;
 somx=0;    somy=0;    somxx=0;    somyy=0;    somxy=0;    aantal=0;
//--
 for (k=0;k<rows_temp;k++)
	{
	 if(ykpunten[k]==TRUE)
		{
		 if (temp[0][k] > xmax)  xmax=temp[0][k];
		 if (temp[1][k] > ymax)  ymax=temp[1][k];
		 if (temp[1][k] < ymin)  ymin=temp[1][k];
		 if (yc>0 && temp[1][k] <= C )		 ykpunten[k]=FALSE; //gooi punt eruit
		 if (yc<0 && temp[1][k] >= C )		 ykpunten[k]=FALSE; //gooi punt eruit
		}
	}
 if(yc>0 && C>=ymax)
	{
	MessageBox(hwnd,"Blank too high\n Correct Blank","Error",MB_OK|MB_ICONSTOP);
	berekend=FALSE;
	return ;
	}
 if(yc<0 && C<=ymin)
	{
	MessageBox(hwnd,"Blank too low\n Correct Blank","Error",MB_OK|MB_ICONSTOP);
	berekend=FALSE;
	return ;
	}
//--
 for(k=0;k<rows_temp;k++)
	{
	 if(ykpunten[k]==TRUE)
		{
		if(Check_Log(temp[0][k],1)) return;
		vx[k]=log(temp[0][k]);
		vy=temp[1][k];
		somx+= vx[k];	 somxx+=vx[k]*vx[k];	 aantal++;
		}
	 }
  if ( yc < 0.0 )   D=ymin / factor;
  else              D=ymax * factor;
  iter=0;
  while (loop>0)
	{
	 iter++;
	 somy=0;    somyy=0;    somxy=0;
	 for(k=0;k<rows_temp;k++)
	  {
		if(ykpunten[k]==TRUE)
		{
		 vy=logit(temp[1][k]);
		 somy+= vy; somyy+=vy*vy; somxy+=vx[k]*vy;
		}
	  }
	sxy= (somxy-(somx*somy / aantal));
	sx = (somxx-(somx*somx / aantal));
	sy = (somyy-(somy*somy / aantal));
	corr = (sxy / (sqrt( sx * sy)));
	slope = sxy / sx;
	intercept = ( somy / aantal ) - ( somx / aantal ) * slope;
	if(corr<0) corr=0-corr;           	/* maak R positief */
	factor*=maalfactor;
	if (corr<=corroud) //ic<=ico )
	 {
		maalfactor=1;
		loop++;          				/* verhoog de loop teler*/
		if (loop==2)
		{
			factor=(((factor-1)/10)+1); /* Ga nog max 10 keer terug */
			yc=-yc;		     			/* In veranderde richting */
		}
		if (loop==3)
		{
			factor=(((factor-1)/5)+1);  /* Ga nog max 5 keer terug */
			yc=-yc;		     			/* In veranderde richting */
		}
		if (loop==4)
		{			     				/* Ga nog 1 keer terug */
			yc=-yc;		     			/* In veranderde richting */
		}
	 }
  if(loop <= 4 )  corroud=corr;
		 else {berekend = TRUE; break;} /* Als loop>3 verlaat iteratie*/
  if (yc > 0 )   D *= factor;        	/* D verlagen of verhogen ? */
		 else D /= factor;
  if(D>C)
	  { if(D<=ymax)	 D=ymax*1.01; } 	/* D niet < ymax */
  if(iter > 1000) break;
  }										/* einde if berekend */

 corr = sxy / sqrt(sx*sy);
 slope = sxy / sx;
 intercept= ( somy / aantal ) - ( somx / aantal ) * slope;
 Bereken_spreiding(sx,sy);
 berekend=TRUE;
}
return;
}
//************************  check log <=0 ***************************/
int Check_Log(double check,int err)
{
 err=err;
 if(check<=0)
  {
/*   printf("Log Error %d: %f :Druk een toets",err,check);
	getch();*/
	return(TRUE);
  }
	else return(FALSE);
}

//**************************  fill temp  *******************************
void fill_temp(HWND hwnd,int i,int j)
{
 int    k,n;
 double vx,vy;
 double cc[50];
 int    cteller=0;

 rows_temp=0;
 for(k=0;k<=rows;k++)
	{
	temp[0][k]=MISSING;
	temp[1][k]=MISSING;
	vx=data[i][k];
	vy=data[j][k];
   if(vx==MISSING || vy==MISSING) continue;
	if(vx==0.0 && cteller<50 && Regressie==IDC_LOGIT)	 { cc[cteller++]=vy;	  continue;}
//	if(ascal->X_Astrans==1 && vx<=0) { ascal->X_Astrans=0; LogXas = 0;}
//	if(ascal->Y_Astrans==1 && vy<=0) { ascal->Y_Astrans=0; LogYas = 0;}
	 if (( vx>=0 && vx<upx)&& (vy>=0 && vy<upy))
	  {
		temp[0][rows_temp]=vx;
		temp[1][rows_temp]=vy;
		ykpunten[rows_temp]=TRUE;
		if(++rows_temp >=MAX_YK_PNT)
		{
		  MessageBox(hwnd,"Too many calibrationpoints","Warning",MB_OK|MB_ICONINFORMATION);
		  break;
		}
	  }
	 }
 if(Regressie==IDC_LOGIT)
  {
	C=0;
	if(cteller>0) {for (n=0;n<cteller;C+=cc[n++]);	C/=(cteller);}
  }
}
//------------------------------ CALC -----------------------------------------------
int calc(HANDLE hgraph, HWND hwndgraph, int rowx, int rowy)
{
 int 	i;
 double maxx,maxy,minx,miny;

maxx=maxy=-10E20;
minx=miny=+10e20;
axcal.X_As=rowx;
axcal.Y_As=rowy;
fill_temp(hwndgraph,rowx,rowy);
if(Regressie==IDC_POLYNOOM && rows_temp<polynoomgraad)
{
	MessageBox(hwndgraph,"Too less datapoints","No Go",MB_OK|MB_ICONSTOP);
	return FALSE;
}
if(rows_temp<3)
{
	MessageBox(hwndgraph,"Too less datapoints","No Go",MB_OK|MB_ICONSTOP);
	return FALSE;
}
regressie(hwndgraph,axcal.X_As,axcal.Y_As);
if(!berekend) return 0;
if(NoGraph) return 0;
for (i=0;i<rows_temp;i++)
	{
		 if (temp[0][i]>maxx)      maxx=temp[0][i];
		 if (temp[0][i]<minx)      minx=temp[0][i];
		 if (temp[1][i]>maxy) 	   maxy=temp[1][i];
		 if (temp[1][i]<miny)	   miny=temp[1][i];
	}
axcal.X_Zeroforced=0;
axcal.Y_Zeroforced=0;
if(Xzero && !axcal.X_Astrans) {minx=0; axcal.X_Zeroforced=Xzero;}
if(Yzero && !axcal.Y_Astrans) {miny=0; axcal.Y_Zeroforced=Yzero;}

if(Regressie==IDC_RID) {miny*=miny; maxy*=maxy;}
axcal.X_Min		= minx;
axcal.X_Max		= maxx * 1.05;
axcal.Y_Min		= miny;
axcal.Y_Max		= maxy * 1.05;
axcal.Red		= 55;
axcal.Green		= 33;
axcal.Blue		= 122;   			// RGB colors line
axcal.Color		= BLUE;				// Als Color ==255 dan RGB values
axcal.PointColor= RED;				// Als Color ==255 dan RGB values
axcal.Linethickness	= 1;
axcal.Pointthickness= 2;
axcal.X_Offset	= 0.15 * ScreenX;
axcal.Y_Offset	= 0.15 * ScreenY;
axcal.X_Length	= 0.80 * ScreenX;	// length axis in units (10000)
axcal.X_Ticks = Ticks;				// 0=inside 1=through 2=outside
axcal.X_NoofTicks	= 5;
axcal.Y_Length	= 0.70 * ScreenY;	// length axis in units (10000)
axcal.Y_Ticks = Ticks;				// 0=inside 1=through 2=outside
axcal.Y_NoofTicks	= 7;
axcal.Noof_DataPoints = rows_temp;
DoGraphDrawOpenDlg (hgraph, hwndgraph);

return(TRUE);
}
//****************************** lineaire regressie **********************
int linaire_regressie(HWND hwnd, int i, int j)
{
 register int k;
 double somxx,somxy,somyy,sx,sy,sxy;
 register float vx,vy;
	somx=0;	 somy=0;	 somxx=0;	 somyy=0;	 somxy=0;	 aantal=0;

	for(k=0;k<rows_temp;k++)
	 {
	 if(ykpunten[k]==TRUE)
		{
		vx=temp[i][k];
		vy=temp[j][k];
		if(Regressie==IDC_RID) vy*=vy;
		somx+= vx; 		somy+= vy;		somxx+=vx*vx;
		somyy+=vy*vy;  somxy+=vx*vy;	aantal++;
		}
	 }
	if(aantal<3)
		{	MessageBox(hwnd,"Too less datapoints","No Go",MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	sxy=(somxy-((somx*somy)/ aantal));
	sx= (somxx-((somx*somx)/ aantal));
	sy= (somyy-((somy*somy)/ aantal));
	corr = (sxy / (sqrt(fabs( sx * sy))));
	slope = sxy / sx;
	intercept = ( somy / aantal ) - ( somx / aantal ) * slope;
	Bereken_spreiding(sx,sy);
	berekend=TRUE;
	return(TRUE);
}
void Bereken_spreiding(double sx,double sy)
{
 int n;
 double ppst,Sres,varx,vary;
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
	{120 , 1.980 } ,
	{1000, 1.960 }  };


	varx = sx/(aantal - 1);
	vary = sy/(aantal - 1);
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

 }




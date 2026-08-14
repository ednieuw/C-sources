// wcortek.c  Ed Nieuwenhuys 1992 onderdeel van wcorr.c

#include "wcorr.h"
struct ASCAL  axcal;

int 		calc(HANDLE hgraph, HWND hwndgraph, int rowx, int rowy);
void   	fill_temp(int ,int );
void 		teken_graph(HWND hwnd,HDC hdc,int rowx,int rowy);
void     teken_shoestrap_graph(HWND hwnd,HDC hdc);
extern 	BOOL PrintMyPage (HWND hwnd);
extern   double huge *BSarray;  // uit wcorr.c
double 	maxx,maxy;
double 	minx,miny;
double	VC,VCn;

float gasdev(void)
{
 static int iset=0;
 static float gset;
 float fac,r,v1,v2;

 if (iset==0)
	{
	do
		{
		v1=2.0 *(float) rand()/(RAND_MAX+1.0)-1.0;
		v2=2.0 *(float) rand()/(RAND_MAX+1.0)-1.0;
		r= v1 * v1 + v2 * v2;
		}
	while (r>=1.0);
	fac = sqrt(-2.0 * log(r) / r);
	gset = v1 * fac;
	iset=1;
	return (v2*fac);
	}
	else
	{
	 iset = 0;
	 return gset;
	}
}

/**************************  fill temp  *******************************/
void fill_temp(int i,int j)

{
 int pos,k;
 double vx,vy;
 int SSperc;
 float dummy;

 SSperc=Shoestrap_percentage;
 rows_temp=0;
 for(k=0;k<=rows;k++)
	{
	temp[0][k]=MISSING;
	temp[1][k]=MISSING;
	vx=data[i][k];
	vy=data[j][k];
	if(axcal.X_Astrans==1 && vx<=0) 	continue;//vx=MISSING ;
	if(axcal.Y_Astrans==1 && vy<=0) 	continue;//vy=MISSING ;
	if(vx>upx || vy>upy)					continue;
	if (vx!=MISSING && vy!=MISSING)
	  {
		if(LogXData) vx= log10(vx);
		if(LogYData) vy= log10(vy);
		if(SHOESTRAP)
		 {
	  dummy=gasdev();
	  pos=(int)(10*((dummy+5.0)*SSperc)/SSperc);
	  if(pos>=0 && pos<MAX_HIS_SIZE)	 Histogram_dev[pos]+=1.0;
	  vx*=dummy*SSperc/100+1.0;
	  if(fabs(dummy*SSperc)<SSperc) VC++; else VCn++;
	  dummy=gasdev();
	  pos=(int)(10*((dummy+5.0)*SSperc)/SSperc);
	  if(pos>=0 && pos<MAX_HIS_SIZE)	 Histogram_dev[pos]+=1.0;
	  vy*=dummy*SSperc/100+1.0;
	  if(fabs(dummy*SSperc)<SSperc) VC++; else VCn++;
//	  VC+=dummy*SSperc*dummy*SSperc;
//	  VCn++;
//		  vx*=gasdev()*SSperc/1000+1.0;
//		  vy*=gasdev()*SSperc/1000+1.0;
//	  vx*=((float)(random(SSperc*2)-SSperc)/1000)+1.0;
//	  vy*=((float)(random(SSperc*2)-SSperc)/1000)+1.0;
											  // verander met +/- bootstrap percentage
		 }
		sortx[rows_temp]    = vx;
		sorty[rows_temp]    = vy;
		temp[0][rows_temp]  = vx;
		temp[1][rows_temp++]= vy;
		}

  }
}

int calc(HANDLE hgraph, HWND hwndgraph, int rowx, int rowy)
{
int i;

maxx=maxy=-10E20;
minx=miny=+10e20;
axcal.X_Astrans=LogXas; 					//Lineair=0 Log=1
axcal.Y_Astrans=LogYas;
axcal.X_As=rowx;
axcal.Y_As=rowy;

fill_temp(rowx,rowy);
reken(hwndgraph,rowx,rowy);

if(aantal<4) 					return 0;
if(NoGraph && !Autosave) 	return 0;
for (i=0;i<rows_temp;i++)
	{
		 if (temp[0][i]>maxx)      maxx=temp[0][i];
		 if (temp[0][i]<minx)      minx=temp[0][i];
		 if (temp[1][i]>maxy) 		maxy=temp[1][i];
		 if (temp[1][i]<miny)		miny=temp[1][i];
	}
axcal.X_Zeroforced=0;
axcal.Y_Zeroforced=0;
if(Xzero && !LogXas) {minx=0; axcal.X_Zeroforced=Xzero;}
if(Yzero && !LogYas) {miny=0; axcal.Y_Zeroforced=Yzero;}

axcal.X_Min=minx;
axcal.X_Max=maxx*1.05;
axcal.Y_Min=miny;
axcal.Y_Max=maxy*1.05;
axcal.Red=55;
axcal.Green=33;
axcal.Blue=122;   				// RGB colors line
axcal.Color=BLUE;					// Als Color ==255 dan RGB values
axcal.PointColor=RED;			// Als Color ==255 dan RGB values
axcal.Linethickness=1;
axcal.Pointthickness=2;
axcal.X_Offset=0.15*ScreenX;
axcal.Y_Offset=0.15*ScreenY;
axcal.X_Length=0.80*ScreenX; 	// length axis in units (10000)
axcal.X_Ticks=Ticks;				// 0=inside 1=through 2=outside
axcal.X_NoofTicks=5;
axcal.Y_Length=0.70*ScreenY; 	// length axis in units (10000)
axcal.Y_Ticks=Ticks;				// 0=inside 1=through 2=outside
axcal.Y_NoofTicks=7;
axcal.Noof_DataPoints=rows_temp;

DoGraphDrawOpenDlg (hgraph, hwndgraph);

return(0);
}

// *************** teken plaatje ***************

void teken_graph(HWND hwnd,HDC hdc,int rowx,int rowy)
 {
 int toggle;
 char text[80],text1[80];
 double maxx,maxy;
 double minx,miny;
 double vx,vy;
 double mx,my,ml,mh,a,b;
 float xpos,ypos;

 int row1=9800,row2=9500,row3=9200,row4=8900,
	 col1=axcal.X_Offset,col2=3300,col3=5000,col4=8500;
 col3=col3;
 hwnd=hwnd;
 minx=axcal.X_Min;
 maxx=axcal.X_Max;
 miny=axcal.Y_Min;
 maxy=axcal.Y_Max;
 Piccolor(hdc,BLUE,1);
 if(rspearman==MISSING) Print(hdc,5000,5000,1,0,"ALLOCATION ERROR");

// Print(hdc,axcal.X_Offset,axcal.Y_Offset/3,2,0,(char far*) label[rowx]+1);
// Print(hdc,600,axcal.Y_Offset,2,90,(char far*)label[rowy]+1);  // print assen
 PrintBottom(hdc,axcal.X_Offset,0,2,0,(char far*) label[rowx]+1);
 PrintTop(hdc,0,axcal.Y_Offset,2,90,(char far*)label[rowy]+1);  //print assen

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
	if(axcal.Color!=255)	lp.lopnColor = dwColor[GREEN];//axcal.Color];
	else							lp.lopnColor = RGB(axcal.Red,axcal.Green,axcal.Blue);
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
			xpos=Data_X_2screen(mx);
			ypos=Data_Y_2screen(my);
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
			xpos=Data_X_2screen(mx);
			ypos=Data_Y_2screen(my);
			if (toggle) Draw(hdc,xpos,ypos);
			 else       Move(hdc,xpos,ypos);
			toggle=1;
		  }
		}
	}
  xpos=ypos=-999;
  DeleteObject(SelectObject(hdc, hpen));
  lp.lopnWidth.x = 1;
  if(axcal.Color!=255)	lp.lopnColor = dwColor[BLACK];//axcal.Color];
  else							lp.lopnColor = RGB(axcal.Red,axcal.Green,axcal.Blue);
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
		 xpos=Data_X_2screen(mx);
		 ypos=Data_Y_2screen(mx);
		 if (toggle) Draw(hdc,xpos,ypos);
		 else       Move(hdc,xpos,ypos);
		 toggle=1;
		}
	if(xpos>0 && ypos>0) Print(hdc,xpos,ypos,1,0,"X=Y");
  }
}  					// einde teken lijn
	DeleteObject(SelectObject(hdc, hpen));
	lp.lopnWidth.x = 1;
	if(axcal.Color!=255)	lp.lopnColor = dwColor[CYAN];//axcal.Color];
	else							lp.lopnColor = RGB(axcal.Red,axcal.Green,axcal.Blue);
	hpen = SelectObject(hdc,CreatePenIndirect(&lp));

// ********* teken 95% prediction interval lijnen ********
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
		xpos=Data_X_2screen(vx);
		ypos=Data_Y_2screen(my);
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
		xpos=Data_X_2screen(vx);
		ypos=Data_Y_2screen(my);
		if (toggle) Draw(hdc,xpos,ypos);
		else        Move(hdc,xpos,ypos);
		toggle=1;
		}
 }
DeleteObject(SelectObject(hdc, hpen));
}

// *************** teken shoestrap plaatjes ***************

void teken_shoestrap_graph(HWND hwnd,HDC hdc)
 {
 char text[80],text1[20];
 int n;
 double mx,my;
 float xpos,ypos;
 int row1,row2,row3,row4,col1,col2,col3,col4;

 hwnd=hwnd;

axcal.X_Min=0.01;
axcal.X_Max=1;
axcal.Y_Min=BSarray[0];
axcal.Y_Max=BSarray[axcal.Noof_DataPoints];
 if(axcal.Graphtype==SS_HISTOGRAM)
	{
	axcal.X_Min=-5*Shoestrap_percentage;
	axcal.X_Max=5*Shoestrap_percentage;
	axcal.Y_Min=0;
	axcal.Y_Max=0;
	for(n=0;n<MAX_HIS_SIZE;n++)	 if(BSarray[n]>axcal.Y_Max) axcal.Y_Max=BSarray[n];
	}
axcal.Red=55;
axcal.Green=33;
axcal.Blue=122;   				// RGB colors line
axcal.Color=BLUE;					// Als Color ==255 dan RGB values
axcal.PointColor=RED;			// Als Color ==255 dan RGB values
axcal.Linethickness=1;
axcal.Pointthickness=2;
axcal.X_Offset=0.15*ScreenX;
axcal.Y_Offset=0.15*ScreenY;
axcal.X_Length=0.80*ScreenX; 	// length axis in units (10000)
axcal.X_Ticks=Ticks;				// 0=inside 1=through 2=outside
axcal.X_NoofTicks=5;
axcal.Y_Length=0.70*ScreenY; 	// length axis in units (10000)
axcal.Y_Ticks=Ticks;				// 0=inside 1=through 2=outside
axcal.Y_NoofTicks=7;

 minx=axcal.X_Min;
 maxx=axcal.X_Max;
 miny=axcal.Y_Min;
 maxy=axcal.Y_Max;
 row1=9800,row2=9500,row3=9200,row4=8900,
 col1=axcal.X_Offset,col2=ScreenX/2,col3=2000,col4=7500;

Teken_assen(hwnd,hdc);

 Piccolor(hdc,BLUE,1);

 if(axcal.Graphtype!=SS_HISTOGRAM)PrintBottom(hdc,axcal.X_Offset,0,2,0,"Percentiel");
 if(axcal.Graphtype==SS_HISTOGRAM)PrintBottom(hdc,axcal.X_Offset,0,2,0,"Coeifficient of Variation");
 if(axcal.Graphtype==SS_SLOPE) PrintTop(hdc,0,axcal.Y_Offset,2,90,"Slope");  //print assen
 if(axcal.Graphtype==SS_INTERCEPT) PrintTop(hdc,0,axcal.Y_Offset,2,90,"Intercept");  //print assen
 if(axcal.Graphtype==SS_CORR) PrintTop(hdc,0,axcal.Y_Offset,2,90,"Pearson correlation");  //print assen
 if(axcal.Graphtype==SS_SCORR) PrintTop(hdc,0,axcal.Y_Offset,2,90,"Spearman correlation");  //print assen
 if(axcal.Graphtype==SS_HISTOGRAM)  PrintTop(hdc,0,axcal.Y_Offset,2,90,"Number");  //print deviation

 Piccolor(hdc,BLACK,1);
 lp.lopnStyle   = PS_SOLID;
 lp.lopnWidth.x = 2;
 lp.lopnWidth.y = 0;              // y-dimension not used
 if(axcal.Color!=255)	lp.lopnColor = dwColor[LIGHTRED];//axcal.Color];
 else							lp.lopnColor = RGB(axcal.Red,axcal.Green,axcal.Blue);
 hpen = SelectObject(hdc, CreatePenIndirect(&lp));

 xpos=Data_X_2screen(0);
 if(axcal.Graphtype==SS_HISTOGRAM)
//	  xpos=Data_X_2screen(((axcal.X_Max-axcal.X_Min)/20)*Shoestrap_percentage*SS_CORRECTIE);
	  xpos=Data_X_2screen(-5*Shoestrap_percentage);
 ypos=Data_Y_2screen(BSarray[0]);
 Move(hdc,xpos,ypos);
 for(n=1;n<=axcal.Noof_DataPoints;n++)
	{
	 mx=n*(axcal.X_Max/axcal.Noof_DataPoints);
	 if(axcal.Graphtype==SS_HISTOGRAM)
		 mx=((float)n*Shoestrap_percentage/10)-5*Shoestrap_percentage;
	 xpos=Data_X_2screen(mx);
	 ypos=Data_Y_2screen(BSarray[n]);
	 Draw(hdc,xpos,ypos);
	}
 if(axcal.Color!=255)	lp.lopnColor = dwColor[GREEN];//axcal.Color];
 else							lp.lopnColor = RGB(axcal.Red,axcal.Green,axcal.Blue);
 hpen = SelectObject(hdc, CreatePenIndirect(&lp));

 if(axcal.Graphtype!=SS_HISTOGRAM)
  {
  sprintf(text,"X: %s <> Y: %s",
	(char far*) label[axcal.X_As]+1,(char far*) label[axcal.Y_As]+1);
  Printc(hdc,col2,row4,1.5,0,text);
  mx=0;
  xpos=Data_X_2screen(mx);
  my=BSarray[((axcal.Noof_DataPoints+1)*0.95)-1];
  ypos=Data_Y_2screen(my); //95% perc
  Move(hdc,xpos,ypos);
  mx=(axcal.X_Max);
  xpos=Data_X_2screen(mx);
  Draw(hdc,xpos,ypos);
  sprintf(text,"% s %0.4G","95% perc =",my);
  Print(hdc,col1,row1,1,0,text);

  mx=0;
  xpos=Data_X_2screen(mx);
  my=BSarray[((axcal.Noof_DataPoints+1)/2)-1];
  ypos=Data_Y_2screen(my); //50% perc
  Move(hdc,xpos,ypos);
  mx=(axcal.X_Max);
  xpos=Data_X_2screen(mx);
  Draw(hdc,xpos,ypos);
  sprintf(text,"% s %0.4G","50% perc =",my);
  Print(hdc,col1,row2,1,0,text);

  mx=0;
  xpos=Data_X_2screen(mx);
  my=BSarray[((axcal.Noof_DataPoints+1)/20)-1];
  ypos=Data_Y_2screen(my); //5% perc
  Move(hdc,xpos,ypos);
  mx=(axcal.X_Max);
  xpos=Data_X_2screen(mx);
  Draw(hdc,xpos,ypos);
  sprintf(text,"% s %0.4G"," 5% perc =",my);
  Print(hdc,col1,row3,1,0,text);
  }
  else
  {
  sprintf(text,"within 1*CV: %6.6G  out: %6.6G = %6.4G%% within",VC,VCn,100*VC/(VC+VCn));
  Print(hdc,col3,row3,1,0,text);
  }
  sprintf(text,"% s %d","No of shoestraps = ",Shoestrap_iterations);
  Printis(hdc,col4,row1,1,0,text);

  gcvt(aantal,4,text1);
  sprintf(text,"% s %s","No of datapoints = ",text1);
  Printis(hdc,col4,row2,1,0,text);

  DeleteObject(SelectObject(hdc, hpen));
}


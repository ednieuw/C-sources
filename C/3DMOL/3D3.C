/* 3d3 */
#include "corr.h"

#define AS_AANTAL 23   /*aantal aslabelingen */
#define MAX_POLY_LENGTH 4
#define TEST_LINE_LENGTH (sizeof(Lines)/sizeof(struct Point3))
#define TEST_POINT_LENGTH rows_temp
#define TRUE	1			/* Define some handy constants	*/
#define FALSE	0			/* Define some handy constants	*/
#define ON	1			/* Define some handy constants	*/
#define OFF	0			/* Define some handy constants	*/
#define PICXF 3000/4000       /* correctie factor naar LOTUS.PIC file */
#define PICYF 2200/3000
#define BOTTOMLEFT 7

#define X_length 300
#define Y_length 220
#define Z_length 200

struct Point {
 float X;
 float Y;
 };

struct Point3 {
 float X;
 float Y;
 float Z;
 };
extern unsigned long MAX_ROW;
extern unsigned long MAX_COL;
extern float **temp;
extern int   rows_temp;
extern int   DrawPicFile;    //Schrijf PIC file naar disk
extern int   DrawGraphScreen;//Teken op het geinitialiseerde graphscherm, alleen bij 3D
extern int   x_as,y_as,z_as;

struct Point3 Lines[] = {
{00		,00		,00		},
{00		,Y_length,00		},
{00		,Y_length,Z_length},
{00		,00		,Z_length},
{X_length,00		,Z_length},
{X_length,Y_length,Z_length},
{00		,Y_length,Z_length},
{00		,00		,Z_length},
{00		,00		,00		},
{X_length,00		,00		},
{X_length,00		,Z_length} };


/*
{00		,00		,00		},
{X_length	,00		,00		},
{X_length	,Y_length	,00		},
{00		,Y_length	,00		},
{00		,00		,00		},
{00		,00		,Z_length	},
{X_length	,00		,Z_length	},
{X_length	,00		,00		},
{X_length	,00		,Z_length	},
{X_length	,Y_length	,Z_length	},
{X_length	,Y_length	,00		},
{00		,Y_length	,00		},
{00		,Y_length	,Z_length	},
{00		,00		,Z_length	},
{00		,Y_length	,Z_length	},
{X_length	,Y_length	,Z_length	} }; complete cubus */


int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
float HSF;		/*  Scale Factors */
float VSF;
/*extern struct palettetype palette;     /* for palette info		*/*/

double Sin,Cos; 		//sin en cos van Rotation;
extern double facx,facy; 	//Correctie naar lotus PIC file
float  Xfac,Yfac,Zfac; 		//Correctie graphscreen -> maxlen assen
float  Xm,Ym,Zm,Xmin,Ymin,Zmin;
int    X_org = 100;
int    Y_org = 70;
int    Xi,Yi,Zi,bolgrootte=50;
int    Make_picfile;
extern int SBOX;
extern char  **label;
extern int x_as,y_as,z_as;
extern void picbox(int ox,int oy,int lx,int ly);

void teken3dgraph(void);
int  Initialize(void);
void einde(void);
void cline(double xs,double ys,double zs,double xe, double ye, double ze,int Color);
void cpoint(double xs,double ys,double zs,int Color,short verton);
void picbol(int ox,int oy,int lx,int ly,int Color);
int  getkey(int flag);
void ddbox(int Xl,int Yl, int Zl);
void plotpunten(int Xl,int Yl, int Zl);
void scalen(int Xl,int Yl, int Zl);
void teken_3dassen(void);

/*************************************************************/

void teken3dgraph(void)
{
double Rotation = 60.0/180.0 * M_PI;
int    c,loop;

Initialize();

DrawPicFile=FALSE;    //Schrijf PIC file naar disk
DrawGraphScreen=TRUE;//Teken op het geinitialiseerde graphscherm, alleen bij 3D

Xi=Yi=Zi=0;
Make_picfile=FALSE;
loop=TRUE;
 while(loop) {
  Sin = sin(Rotation);
  Cos = cos(Rotation);
  scalen(X_length+Xi,Y_length+Yi,Z_length+Zi);
  ddbox(X_length+Xi,Y_length+Yi,Z_length+Zi);
  plotpunten(X_length+Xi,Y_length+Yi,Z_length+Zi);
  teken_3dassen();

  c=getkey(1);
  cleardevice();
  switch(c) {
  case '-'  : Rotation += 5.0 /180.0 * M_PI; break;
  case '+'  : Rotation -= 5.0 /180.0 * M_PI; break;
  case UP   : Y_org +=15; break;
  case DOWN : Y_org -=15; break;
  case RIGHT: X_org +=15; break;
  case LEFT : X_org -=15; break;
  case HOME : X_org = 50; Y_org=50; Xi=0;Yi=0;Zi=0;
					Rotation = 60.0 /180.0 * M_PI;
					break;
  case 'x'  : Xi-=10; break;
  case 'X'  : Xi+=10; break;
  case 'y'  : Yi-=10; break;
  case 'Y'  : Yi+=10; break;
  case 'z'  : Zi-=10; break;
  case 'Z'  : Zi+=10; break;
  case 'b'  : bolgrootte-=5; break;
  case 'B'  : bolgrootte+=5; break;
  default   : loop=FALSE;
  }
  if(!kbhit()) continue;
 }
 DrawPicFile=FALSE;    //Schrijf PIC file naar disk
 DrawGraphScreen=TRUE;//Teken op het geinitialiseerde graphscherm, alleen bij 3D
 Make_picfile=FALSE;
 picopen();
 scalen(X_length+Xi,Y_length+Yi,Z_length+Zi);
 plotpunten(X_length+Xi,Y_length+Yi,Z_length+Zi);
 ddbox(X_length+Xi,Y_length+Yi,Z_length+Zi);
 teken_3dassen();
 picend();
 Make_picfile=FALSE;
 closegraph();
 einde();
}

//----------------------------------------------------------------------------------
int Initialize(void)
{
/* select driver and mode that supports use of setrgbpalette */
//  int gdriver = VGA, gmode = VGAHI, errorcode;

  if (registerfarbgidriver(CGA_driver_far)    < 0) return(-1);
  if (registerfarbgidriver(Herc_driver_far)   < 0) return(-1);
  if (registerfarbgidriver(EGAVGA_driver_far) < 0) return(-1);
//  if (registerbgifont(small_font)      < 0) return(-1);

	/* initialize graphics and local variables */
//	initgraph(&gdriver, &gmode, "");
  GraphDriver = DETECT;			/* Request auto-detection	*/

  initgraph( &GraphDriver, &GraphMode,"");
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
	 cprintf(" Graphics System Error: %s\r\n", grapherrormsg( ErrorCode ) );
	 return( 1 );
  }
/*getpalette(&pal);
	// create gray scale
	for (i=0; i<pal.size; i++)
		setrgbpalette(pal.colors[i], i*4, i*4, i*4);
*/
/*  getpalette( &palette );		/* Read the palette from board	*/*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy(); /* Read size of screen		*/

  facx=H_WIDTH/MaxX * H_SCALE; //factor data naar lotus PIC file
  facy=V_WIDTH/MaxY * V_SCALE;

  HSF = 3400.0 / (float)MaxX;   /* Horizontal conversionfactor */
  VSF = 2400.0 / (float)MaxY;   /* Verical conversionfactor */
return(1);
}

/*******************************************/
void einde(void)
{
 DrawPicFile=FALSE;    //Schrijf PIC file naar disk
 DrawGraphScreen=FALSE;//Teken op het geinitialiseerde graphscherm, alleen bij 3D

}
/*******************************************/


void cline(double xs,double ys,double zs,double xe, double ye, double ze,int Color)
{
double Zxs,Zxe,Zys,Zye;
int x1,x2,y1,y2;

  Zxs = Cos * zs;
  Zys = Sin * zs;
  Zxe = Cos * ze;
  Zye = Sin * ze;

  x1=(int) xs + Zxs + X_org;
  x2=(int) xe + Zxe + X_org;
  y1=(int) ys + Zys + Y_org;
  y2=(int) ye + Zye + Y_org;

  piccolor(Color);
  picmove((int)((double)x1 * facx),(int)((double)y1 * facy));
  picdraw((int)((double)x2 * facx),(int)((double)y2 * facy));
}

/*********************************************************/

void cpoint(double xs,double ys,double zs,int Color,short verton)
{
double Zxs,Zys;
int x1,y0,y1;
verton=verton;
  Zxs = Cos * zs;
  Zys = Sin * zs;

  x1=(int) xs+Zxs+X_org;
  y1=(int) ys+Zys+Y_org;
  y0=(int)    Zys+Y_org;

  piccolor(LIGHTRED);
  picmove((int)((double)x1 * facx),(int)((double)y0 * facy));
//  if(verton) picdraw((int)((double)x1 * facx),(int)((double)y1 * facy)); //verticale lijn
  piccolor(Color);
  picbol((int)((double)x1 *facx),(int)((double)y1 *facy),bolgrootte,bolgrootte,Color);
}

//----------------------------------------------------------------------
void picbol(int ox,int oy,int lx,int ly,int Color)
 {
  int sx,sy,px,py;
  int poly[16];
  sx=ox-lx/4;
  sy=oy-ly/2;
  px=lx/4;
  py=ly/4;
/*  DrawGraphScreen=FALSE;
	picmove(sx			,sy );
	picdraw(sx+px*2	,sy);
	picdraw(sx+px*3	,sy+py);
	picdraw(sx+px*3	,sy+py*3);
	picdraw(sx+px*2	,sy+py*4);
	picdraw(sx+px*0	,sy+py*4);
	picdraw(sx+px*-1	,sy+py*3);
	picdraw(sx+px*-1	,sy+py*1);
	picdraw(sx			,sy);
	DrawGraphScreen=TRUE;
*/
  lx/=facx;
  ly/=facy;
  sx=ox/facx-lx/4;
  sy=MaxY-oy/facy-ly/2;
  px=lx/4;
  py=ly/4;
  if(DrawGraphScreen)
				{
				setfillstyle(SOLID_FILL,Color);
				poly[0]=sx;
				poly[1]=sy;
				poly[2]=sx+px*2;
				poly[3]=sy;
				poly[4]=sx+px*3;
				poly[5]=sy+py;
				poly[6]=sx+px*3;
				poly[7]=sy+py*3;
				poly[8]=sx+px*2;
				poly[9]=sy+py*4;
				poly[10]=sx+px;
				poly[11]=sy+py*4;
				poly[12]=sx+px*-1;
				poly[13]=sy+py*3;
				poly[14]=sx+px*-1;
				poly[15]=sy+py*1;
				fillpoly(8,poly);
//				setcolor(WHITE);
//				setlinestyle(SOLID_LINE,0XFFFF,THICK_WIDTH);
//				line(sx,   sy+py,  sx+2*px,sy+py);
//				line(sx-px+2,sy+py-1,sx+4*px-2,sy+py-1);
//				setlinestyle(SOLID_LINE,0XFFFF,NORM_WIDTH);
				}

 }

/************************************************************/

int getkey(int flag) {
int c;
	c=getch();
	if (c==SPECIAL_KEY)
		c=256+getch();
	else {
		if (c==CTRL_C) einde();
		if (!flag) c=toupper(c);
	}
	return c;
}


/****************************************************/
void ddbox(int Xl,int Yl, int Zl)
{
 int i,j;
 double x1,x2,y1,y2,z1,z2;
 j=TEST_LINE_LENGTH;

 for(i=Xl/5;i<Xl;i+=Xl/5)
	cline((double)i,0.0,0.0,
	 (double)i,0.0,(double)Zl,DARKGRAY);

 for(i=Xl/5;i<Xl;i+=Xl/5)
	cline((double)i,0.0,(double)Zl,
	 (double)i,(double)Yl,(double)Zl,DARKGRAY);

 for(i=Yl/5;i<Yl;i+=Yl/5)
	cline(0.0,(double)i,0.0,
	 0.0,(double)i,(double)Zl,DARKGRAY);

 for(i=Yl/5;i<Yl;i+=Yl/5)
	cline(0.0,(double)i,(double)Zl,
	 (double)Xl,(double)i,(double)Zl,DARKGRAY);

 for(i=Zl/5;i<Zl;i+=Zl/5)
	cline(0.0,0.0,(double)i,
	 0.0,(double)Yl,(double)i,DARKGRAY);

 for(i=Zl/5;i<Zl;i+=Zl/5)
	cline(0.0,0.0,(double)i,
	 (double)Xl,0.0,(double)i,DARKGRAY);

 for(i=0;i<j-1;i++)
  {
	x1=Lines[i].X;
	y1=Lines[i].Y;
	z1=Lines[i].Z;
	x2=Lines[i+1].X;
   y2=Lines[i+1].Y;
   z2=Lines[i+1].Z;
   if(x1>0) x1+=Xi;
	if(x2>0) x2+=Xi;
	if(y1>0) y1+=Yi;
	if(y2>0) y2+=Yi;
	if(z1>0) z1+=Zi;
	if(z2>0) z2+=Zi;

	cline((double)(x1),
	 (double)(y1),
	 (double)(z1),
	 (double)(x2),
	 (double)(y2),
	 (double)(z2),
	 GREEN);
  }
}
/**************************************************/

void plotpunten(int Xl,int Yl, int Zl)
{
 int   i,j;
 short vertical_on;
 Xm=Ym=Zm=0;

 j=TEST_POINT_LENGTH;

 for(i=0;i<j;i++) // vind hoogste waarde voor scalen
 {
 if(temp[0][i] > Xm) Xm=temp[0][i];
 if(temp[1][i] > Ym) Ym=temp[1][i];
 if(temp[2][i] > Zm) Zm=temp[2][i];
 }

 Xfac=((float)Xl/(float)Xm)*0.95;
 Yfac=((float)Yl/(float)Ym)*0.95;
 Zfac=((float)Zl/(float)Zm)*0.95;

 for(i=0;i<j;i++)
 { if(kbhit()) break;
	vertical_on=((i/10)*10==i);
	cpoint((double)temp[0][i] * Xfac,
			 (double)temp[1][i] * Yfac,
			 (double)temp[2][i] * Zfac,
			 (int)temp[3][i],vertical_on);
	if(i>=1)
		{
		cline((double)temp[0][i] * Xfac,(double)temp[1][i] * Yfac,(double)temp[2][i] * Zfac,
				(double)temp[0][i-1] * Xfac,(double)temp[1][i-1] * Yfac,(double)temp[2][i-1] * Zfac,
				 DARKGRAY);

/*		cline((double)temp[0][i] * Xfac,(double)temp[1][i] * Yfac,(double)temp[2][i] * Zfac,
				(double)temp[0][i-2] * Xfac,(double)temp[1][i-2] * Yfac,(double)temp[2][i-2] * Zfac,
				 YELLOW);
*/		 }
 }
}
/****************************************************/

void scalen(int Xl,int Yl, int Zl)
 {
 int i,j;

 j=TEST_POINT_LENGTH;

 for(i=0;i<j-1;i++) /* vind hoogste waarde voor scalen */
 {
 if(temp[0][i] > Xm) Xm=temp[0][i];
 if(temp[1][i] > Ym) Ym=temp[1][i];
 if(temp[2][i] > Zm) Zm=temp[2][i];
 }
 Xmin=Xm;
 Ymin=Ym;
 Zmin=Zm;

 for(i=0;i<j-1;i++) /* vind laagste waarde voor scalen */
 {
 if(temp[0][i] < Xmin) Xmin=temp[0][i];
 if(temp[1][i] < Ymin) Ymin=temp[1][i];
 if(temp[2][i] < Zmin) Zmin=temp[2][i];
 }


 Xfac=((float)Xl/(float)Xm)*0.95;
 Yfac=((float)Yl/(float)Ym)*0.95;
 Zfac=((float)Zl/(float)Zm)*0.95;
 }


/*************** teken plaatje ***************/

void teken_3dassen(void)
{
char text[MAX_LEN];
int i;
float asx,asy,asz;
double mx,my;

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

for (i=0;i<=AS_AANTAL;i++)
  {
	asx=as[i];
	if (9*asx > Xm)  break;
  }

for (i=0;i<=AS_AANTAL;i++)
  {
	asy=as[i];
	if (9*asy> Ym)	  break;
  }
for (i=0;i<=AS_AANTAL;i++)
  {
	asz=as[i];
	if (9*asz> Zm)	  break;
  }

for (i=0;i<=(int)(Xm/asx);i++)
	 {
		mx=facx * (i * Xfac * asx + X_org);
		my=facy * Y_org;
		gcvt((float)(i*asx),3,text);
		piccolor(LIGHTGREEN);
		picmove((int)mx,(int)my);
		picdraw((int)mx,(int)my-30);
		picmove((int)mx,(int)my-100);
		pictext(0,BOTTOMLEFT,text);
		if(i==0)
		{
		picmove((int)mx+500,(int)my-175);
		pictext(0,BOTTOMLEFT,&label[(int)x_as][1]);
		}
	}

for (i=0;i<=(int)(Ym/asy);i++)
	  {
		mx=facx * X_org ;
		my=facy * ((float)i * Yfac * asy + Y_org);
		gcvt((float)i*asy,3,text);
		piccolor(LIGHTGREEN);
		picmove((int)mx    ,(int)my);
		picdraw((int)mx-30 ,(int)my);
		picmove((int)mx-280,(int)my-20);
		pictext(0,BOTTOMLEFT,text);
		if(i==0)
		 {
			picmove((int)mx-410,(int)my+20);
			pictext(1,BOTTOMLEFT,&label[(int)y_as][1]);
		 }
	  }

for (i=0;i<=(int)(Zm/asz);i++)
	 {
		mx=(double)(X_org + X_length + Xi + (Cos * (float)i * asz * Zfac));
		my=(double)(Y_org +(                 Sin * (float)i * asz * Zfac));
		gcvt((float)i*asz,3,text);
		mx *= facx;
		my *= facy;
		piccolor(LIGHTGREEN);
		picmove((int)mx    ,(int)my);
		picdraw((int)mx-30 ,(int)my);
		picmove((int)mx+150,(int)my-50);
		pictext(0,BOTTOMLEFT,text);
		if(i==0)
		{
			picmove((int)mx+200,(int)my);
			  pictext(0,BOTTOMLEFT,&label[(int)z_as][1]);
		}
	 }

}

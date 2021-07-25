//PIC files en grafisch routines Ed Nieuwenhuys 1993

#include "wlogit.h"
extern lf;
extern cf;
//extern struct axcal;
/*
Een grafisch scherm is 10000 units breed en 10000 units hoog.
Het nulpunt ligt linksonderin het beeldscherm
De volgende routines kunnen voor het tekenen in het scherm gebruikt worden

extern int   	DrawPicFile;			//Schrijf PIC file naar disk
extern int   	DrawGraphScreen;	//Teken op het geinitialiseerde graphscherm, alleen bij 3D
extern int		PICOPEN;
Bovenstaande variabelen zijn gebruikt als actie variabele
extern TEXTMETRIC    tm;
extern short cxClient,cyClient;
extern short cxChar,cxCaps,cyChar;
extern char *Pic_output_file;
extern char Pic_output_file[256]
*/
  #define MAXNOOFCALIBRATORS 8
  #define MISSING_CALIBRATOR -999
  enum range {LARGE=0, MEDIUM, SMALL, LINEAR};
  double value_of_calibrator[MAXNOOFCALIBRATORS];
  char calibratorstring[MAXNOOFCALIBRATORS][40];

DWORD dwColor[16]={RGB(0,0,0),		RGB(0,0,128),	RGB(0,128,0),		RGB(0,255,255),
						 RGB(128,0,0),		RGB(255,0,255),RGB(128,128,0),	RGB(192,192,192),
						 RGB(128,128,128),RGB(0,0,255),	RGB(0,255,0),		RGB(0,128,128),
						 RGB(255,0,0),		RGB(128,0,128),RGB(255,255,0),	RGB(255,255,255)};

//							BLACK,    		BLUE,     		GREEN,    			CYAN,
//							RED,    			MAGENTA,    	BROWN,     			LIGHTGRAY,
//							DARKGRAY,  		LIGHTBLUE,    	LIGHTGREEN,    	LIGHTCYAN,
//							LIGHTRED,  		LIGHTMAGENTA,  YELLOW,    			WHITE

const char 			header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
const double 		LotusPICX =  3000;
const double 		LotusPICY =  2200;
double 				ScreenX   = 10000;
double 				ScreenY   = 10000;
const float  		Pic_default_size = 80; //standaard grootte letters in PIC file


// Funtion prototypes

void 		ConvertPicXY(double x,double y);
void 		Draw(HDC hdc,double xp,double yp);
void 		Move(HDC hdc,double xp,double yp);
void 		Point(HDC hdc,double xp,double yp, double box);
void 		Numm(HDC hdc,double xp,double yp,int number);
void 		Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
void 		PrintTop(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
void 		PrintBottom(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
void 		Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
void	 	Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst);
void 		Printis(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
int  		Pic_Open(HWND hwnd);
void	 	Pic_End(HWND hwnd);
void 		Picfont(char font);
void	 	Piccolor(HDC hdc,int color, int thickness);
void 		Picsize(int x, int y);
void 		Pictext(HDC hdc, int direction,int position,char* s,float size);
void	 	Picxy(int x,int y);
void 		Picbox(HDC hdc,int ox,int oy,int lx,int ly);
char 		Teken_assen(HWND hwndas,HDC hdc,struct ASCAL *ascal);
char 		Teken_data(HWND hwndas,HDC hdc,struct ASCAL *ascal);
//void 		KeyDown(void);
int 		Ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);
double 	scientific2(double d, double *base, int *exp);
double 	val(double mantissa, int exponent);
int 		round(double d);
double 	pow10d(double exponent);

double  			PicX,PicY,GraphX,GraphY;
int   			MaxX,MaxY; 					//screen coordinates
short  			nDrawingMode = R2_COPYPEN ;
int				Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
RECT 				rect;
PAINTSTRUCT		ps;
HPEN         	hpen,hpen1, hpenOld;
LOGPEN 			lp;

//-----------------------------------------------------------------------

/*************** Subroutines tekenen en schrijven PIC file ***************/
void ConvertPicXY(double x,double y)
{
 PicX  = (LotusPICX / ScreenX)    * x;
 PicY  = (LotusPICY / ScreenY)    * y;
 GraphX= ((double)MaxX / ScreenX) * x;
 GraphY= ((double)MaxY / ScreenY) * y;
}

void Draw(HDC hdc,double xp,double yp)
  {
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(DRAW,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen) LineTo(hdc,(int)GraphX,MaxY-(int)GraphY);
  }

void Move(HDC hdc,double xp,double yp)
{
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(MOVE,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
	 {
	  MoveToEx(hdc,(int)GraphX,MaxY-(int)GraphY,NULL);
	  Last_move_X=(int)GraphX;
	  Last_move_Y=MaxY-(int)GraphY;
    }
}

void Point(HDC hdc,double xp,double yp, double box)
	{
	double boxx,boxy;
	boxx=box;
	//tm.tmAveCharWidth*(ScreenX/cxClient)/2;
	boxy=box*cxClient/cyClient;
	//tm.tmAveCharWidth*(ScreenY/cyClient)/2;
	Move(hdc,xp+boxx,yp+boxy);	Draw(hdc,xp-boxx,yp+boxy);
	Draw(hdc,xp-boxx,yp-boxy);	Draw(hdc,xp+boxx,yp-boxy);
	Draw(hdc,xp+boxx,yp+boxy);
	}

 void Numm(HDC hdc,double xp,double yp,int number)
  {
	char tekst[10];
	itoa(number,tekst,10);
	Printc(hdc,xp,yp,1,0,tekst);
  }

 void Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
   SetTextAlign(hdc,TA_LEFT|TA_BASELINE);
	Pictext(hdc,((int)(richting)),BOTTOMLEFT,tekst,size);
  }
 void PrintTop(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
	SetTextAlign(hdc,TA_LEFT|TA_TOP);
	Pictext(hdc,((int)(richting)),TOPLEFT,tekst,size);
  }
 void PrintBottom(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
	SetTextAlign(hdc,TA_LEFT|TA_BOTTOM);
	Pictext(hdc,((int)(richting)),BOTTOMLEFT,tekst,size);
  }

 void Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
   SetTextAlign(hdc,TA_RIGHT|TA_BASELINE);
  Last_move_Y+=tm.tmHeight/2;
	Pictext(hdc,((int)(richting)),CENTERRIGHT,tekst,size);
  }

 void Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst)
 {
//  ypos-=tm.tmHeight/2;
  Move(hdc,xpos,ypos);
  SetTextAlign(hdc,TA_CENTER|TA_BASELINE);
  Last_move_Y+=tm.tmHeight/2;
  Pictext(hdc,((int)(richting)),CENTER,tekst,size);
 }

 void Printis(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	char textleft[80],textright[80];
	char *ptr;

	ptr=strchr(tekst,'=');
	strcpy(textright,ptr);
	ptr[0]=0;
	strcpy(textleft,tekst);
	Move(hdc,xpos,ypos);
	Last_move_Y+=tm.tmHeight*size/2;
	SetTextAlign(hdc,TA_RIGHT|TA_BASELINE);
	Pictext(hdc,((int)(richting)),CENTERRIGHT,textleft,size);

	Move(hdc,xpos,ypos);
	Last_move_Y+=tm.tmHeight*size/2;
	SetTextAlign(hdc,TA_LEFT|TA_BASELINE);
	Pictext(hdc,((int)(richting)),CENTERLEFT,textright,size);
  }


/*************** openen en verwerken PIC file ***************/
int Pic_Open(HWND hwnd)
{
 int n;
 HDC hdc;

/* hdc=GetDC(hwnd);
 cxClient= GetDeviceCaps(hdc,HORZRES);
 cyClient= GetDeviceCaps(hdc,VERTRES);
 ReleaseDC(hwnd,hdc);
*/  if(DrawPicFile)
     {
		if ((fpout=fopen(Pic_output_file,"wb")) == NULL)
		 {
		MessageBox (hwnd, "Disk Full", "Output error",MB_OK | MB_ICONSTOP);
		return FALSE;
		 }
		for (n=0;n<17;n++)	fputc(header_vector[n],fpout);
		Picsize(Pic_default_size,Pic_default_size);
		Picfont(1);
	 }
 return TRUE;
}

//-----------------------------------------------------------------------------------------------
void Pic_End(HWND hwnd)
{
  PICOPEN=FALSE;
  if(DrawPicFile)
  {
	fputc(96,fpout);					// END
	fclose(fpout);
  }
hwnd=hwnd;
}
//-----------------------------------------------------------------------------------------------
void Picfont(char font)  {
 if(DrawPicFile) {
	fputc(_FONT,fpout);
	fputc(font,fpout);
	}
}

void Piccolor(HDC hdc,int color,int thickness)
 {
 	if(DrawPicFile)		fputc(COLOR0+color,fpout);
 	if(DrawGraphScreen)
	{
	hpen = SelectObject(hdc, hpenOld);
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = thickness;
	lp.lopnWidth.y = 0;              /* y-dimension not used */
	lp.lopnColor = dwColor[color];
	DeleteObject(SelectObject(hdc, hpen));
	}
 }

void Picsize(int x, int y)
  {
  if(DrawPicFile) { fputc(_SIZE,fpout); Picxy(x,y); }
  }

void Pictext(HDC hdc, int direction,int position,char* s,float size)
 {
 HFONT 	hfont, hfontOld;
 //int 		MapModePrevious;
 PSTR 	pszFace ="Modern";//"Helv";
 int		breed,hoog;

 GetTextMetrics(hdc,&tm);
 breed= cxClient/100;
 hoog = cyClient/40;

 if(DrawPicFile)
		{
		 Picsize(size*Pic_default_size,size*Pic_default_size);
		 fputc(_TEXT,fpout);
		 fputc((unsigned char)(direction/90)*16+position,fpout);
		 fputs(s,fpout);
		 fputc('\0',fpout);
		}

 if(DrawGraphScreen)
	  {
//			hfont = CreateFont(-tm.tmHeight*(size), -tm.tmAveCharWidth*(size),  direction * 10,0, FW_NORMAL,
			hfont = CreateFont(-hoog*(size), -breed*(size),  direction * 10,0, FW_NORMAL,
			0, 0, 0, 0, OUT_TT_PRECIS, CLIP_LH_ANGLES, PROOF_QUALITY, 0x2000, pszFace);
			hfontOld = SelectObject(hdc, hfont);
			TextOut(hdc,Last_move_X,Last_move_Y,s,strlen(s));
			DeleteObject(SelectObject(hdc, hfontOld));
	 }
 }

void Picxy(int x,int y)
 {
  fputc(x/256,fpout); fputc(x%256,fpout);
  fputc(y/256,fpout); fputc(y%256,fpout);
 }

void Picbox(HDC hdc,int ox,int oy,int lx,int ly)
 {
	Move(hdc,ox   ,oy   );
	Draw(hdc,ox   ,oy+ly);
	Draw(hdc,ox+lx,oy+ly);
	Draw(hdc,ox+lx,oy   );
	Draw(hdc,ox   ,oy   );
 }

//------------------------------------------------------------------------------------------------
float Data_X_2screen(float number,struct ASCAL *ascal)
{
 if(ascal->X_Astrans==1)
  return (ascal->X_Offset+((log(number) - log(ascal->X_Min))/(log(ascal->X_Max) - log(ascal->X_Min)))  * ascal->X_Length);
 else
  return (ascal->X_Offset+((number - ascal->X_Min)/(ascal->X_Max - ascal->X_Min))  * ascal->X_Length);
}

float Data_Y_2screen(float number,struct ASCAL *ascal)
{
 if(ascal->Y_Astrans==1)
  return (ascal->Y_Offset+((log(number) - log(ascal->Y_Min))/(log(ascal->Y_Max) - log(ascal->Y_Min)))  * ascal->Y_Length);
 else
  return (ascal->Y_Offset+((number - ascal->Y_Min)/(ascal->Y_Max - ascal->Y_Min))  * ascal->Y_Length);
}
//------------------------------------------------------------------------------------------------
char Teken_assen(HWND hwndas,HDC hdc,struct ASCAL *ascal)
{
//  double value_of_calibrator[MAXNOOFCALIBRATORS]; resultaten van de ascalibratie in doubles
//char calibratorstring[MAXNOOFCALIBRATORS][40];  resultaten van de ascalibratie in tekst
  LOGPEN lp;
  int 	n,Xoff,Yoff,XTick_length,YTick_length;
  float xpos,ypos;

  XTick_length=0.02*(ascal->X_Length+ascal->Y_Length)/2;
  YTick_length=0.02*((ascal->X_Length+ascal->Y_Length)/2)*cyClient/cxClient;

  lp.lopnStyle   = PS_SOLID;
  lp.lopnWidth.x = ascal->Linethickness;
  lp.lopnWidth.y = 0;              // y-dimension not used
  if(ascal->Color!=255)	lp.lopnColor = dwColor[ascal->Color];
  else						lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
  hpen = SelectObject(hdc,CreatePenIndirect(&lp) );
  ascal->Y_Astrans=(BYTE)Ascal(&ascal->Y_NoofTicks,&ascal->Y_Min,&ascal->Y_Max,(int)(ascal->Y_Astrans),1);
  ascal->X_Astrans=(BYTE)Ascal(&ascal->X_NoofTicks,&ascal->X_Min,&ascal->X_Max,(int)(ascal->X_Astrans),1);
  if(ascal->Y_Astrans) 	Yoff=ascal->Y_Offset-tm.tmHeight*(ScreenY/cyClient);  // als log schaal as iets lager tekenen
  else						Yoff=ascal->Y_Offset;
  Move(hdc,ascal->X_Offset,Yoff);
  Draw(hdc,ascal->X_Offset+ascal->X_Length,Yoff);

  for(n=0;n<ascal->X_NoofTicks;n++)  // Xas labeling
	{
	 ypos=Yoff-(1.5*cyClient/40)*(ScreenY/cyClient);
	 if(ascal->X_Astrans)	 xpos=Data_X_2screen(exp(value_of_calibrator[n]),ascal);
	 else 						 xpos=Data_X_2screen(value_of_calibrator[n],ascal);
	 Printc(hdc,xpos,ypos,1,0, calibratorstring[n]);
	 if(ascal->X_Ticks==0)
	 {
	  Move(hdc,xpos,Yoff);
	  Draw(hdc,xpos,Yoff+XTick_length);
	 }
	 if(ascal->X_Ticks==1)
	 {
	  Move(hdc,xpos,Yoff-XTick_length);
	  Draw(hdc,xpos,Yoff+XTick_length);
	 }
	 if(ascal->X_Ticks==2)
	 {
	  Move(hdc,xpos,Yoff);
	  Draw(hdc,xpos,Yoff-XTick_length);
	 }
	}
  ascal->Y_Astrans=(BYTE)Ascal(&ascal->Y_NoofTicks,&ascal->Y_Min,&ascal->Y_Max,(int)(ascal->Y_Astrans),1);
  if(ascal->X_Astrans) 	Xoff=ascal->X_Offset-tm.tmHeight*(ScreenY/cyClient);  // als log schaal as iets lager tekenen
  else						Xoff=ascal->X_Offset;
  Move(hdc,Xoff,ascal->Y_Offset);
  Draw(hdc,Xoff,ascal->Y_Offset+ascal->Y_Length);
  Move(hdc,ascal->X_Offset,ascal->Y_Offset+ascal->Y_Length);
  Draw(hdc,ascal->X_Offset+ascal->X_Length,ascal->Y_Offset+ascal->Y_Length);
  Draw(hdc,ascal->X_Offset+ascal->X_Length,ascal->Y_Offset);
  for(n=0;n<ascal->Y_NoofTicks;n++)  //Yas labeling
	{
	 xpos=Xoff- (3*cxClient/100)*(ScreenX/cxClient);
	 if(ascal->Y_Astrans)	 ypos=Data_Y_2screen(exp(value_of_calibrator[n]),ascal);
	 else 						 ypos=Data_Y_2screen(value_of_calibrator[n],ascal);
	 Printr(hdc,xpos,ypos,1,0, calibratorstring[n]);
	 if(ascal->Y_Ticks==0)
	 {
	  Move(hdc,Xoff,ypos);
	  Draw(hdc,Xoff+ YTick_length,ypos);
	 }
	 if(ascal->Y_Ticks==1)
	 {
	  Move(hdc,Xoff- YTick_length,ypos);
	  Draw(hdc,Xoff+ YTick_length,ypos);
    }
	 if(ascal->Y_Ticks==2)
	 {
	  Move(hdc,Xoff,ypos);
	  Draw(hdc,Xoff- YTick_length,ypos);
	 }
	}
  DeleteObject(SelectObject(hdc, hpen));
 hwndas=hwndas;
 return 0;
}
//---------------------------------------------------------------------------------------------
char Teken_data(HWND hwndas,HDC hdc,struct ASCAL *ascal)
{

  LOGPEN lp;
  int 	n;
  float xpos,ypos;
  char text[80];

  lp.lopnStyle   = PS_SOLID;
  lp.lopnWidth.x = ascal->Pointthickness;
  lp.lopnWidth.y = 0;              // y-dimension not used
  if(ascal->Color!=255)	lp.lopnColor = dwColor[ascal->PointColor];
  else						lp.lopnColor = RGB(ascal->Red,ascal->Green,ascal->Blue);
  hpen = SelectObject(hdc,CreatePenIndirect(&lp) );
  ascal->X_Astrans=(BYTE)Ascal(&ascal->X_NoofTicks,&ascal->X_Min,&ascal->X_Max,(int)(ascal->X_Astrans),1);
  for(n=0;n<ascal->Noof_DataPoints;n++)
	{
	 ypos=Data_Y_2screen(temp[1][n],ascal);
	 xpos=Data_X_2screen(temp[0][n],ascal);
	 if(Numgraph) Printc(hdc,xpos,ypos,1,0,itoa(n,text,10));
	 else		Point(hdc,xpos,ypos,ascal->X_Length/200);
	 //	 	Point(hdc,xpos,ypos,tm.tmAveCharWidth*(ScreenX/cxClient)/2);
//	 ypos=Data_Y_2screen(temp[1][n],ascal)+0.5*tm.tmHeight*(ScreenY/cyClient);
	}
 DeleteObject(SelectObject(hdc, hpen));
 hwndas=hwndas;
 return 0;
}
/*
//-------------------------------------------------------------------------------------------------
 void KeyDown(void)
 {
 MSG msg;
 while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))    if (msg.message == WM_KEYDOWN)   break;
 }
*/
//-------------------------------------------------------------------------------------------------
int Ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag)
 {
	 int n, range, exp, exp0, t, t0, found, negative, precision, extra_decimal=0;
	 double unit, value, tempmin, tempmax, ratio, base, lowest_calibrator;
	 int calbase[4][4]={{1,0,0,0},{1,3,0,0},{1,2,5,0},{10,20,25,50}};
	 int exponent[MAXNOOFCALIBRATORS];
	 if (logflag) round(scientific2(*min, &base, &exp));
    else round(scientific2(*max, &base, &exp));
    tempmin=*min * 10 +  fabs(*min)/ScreenX;
    tempmax=*max * 10 -  fabs(*max)/ScreenY;
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
    if (!calcflag) return logflag;

/* phase 3: print results in minimal-length strings */

    for (n=0; n< *noofcalibrators ; n++) {
        if (value_of_calibrator[n]>(*max>0?1.0001* *max:0.9999* *max)) {
           break;
		  }
        precision= (range==LINEAR) ? (extra_decimal - exponent[n]): -exponent[n];
        if (precision<0) precision=0;
        sprintf(calibratorstring[n],"%.*lf", precision, value_of_calibrator[n]);
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




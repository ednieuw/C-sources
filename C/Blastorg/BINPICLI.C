//PIC files en grafisch routines Ed Nieuwenhuys 1993

#include "blast.h"
#include <commdlg.h>

//extern lf;
extern CHOOSEFONT cf;
struct ASCAL axcal;
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
  #define MAXNOOFCALIBRATORS 10
  #define MISSING_CALIBRATOR -999
  enum range {LARGE=0, MEDIUM, SMALL, LINEAR};
  double value_of_calibrator[MAXNOOFCALIBRATORS];
  char calibratorstring[MAXNOOFCALIBRATORS][40];

DWORD dwColor[16]={RGB(0,0,0),		RGB(0,0,128),  RGB(0,128,0),	RGB(0,255,255),
				   RGB(128,0,0),	RGB(255,0,255),RGB(128,128,0),	RGB(192,192,192),
				   RGB(128,128,128),RGB(0,0,255),  RGB(0,255,0),	RGB(0,128,128),
				   RGB(255,0,0),	RGB(128,0,128),RGB(255,255,0),	RGB(255,255,255)};

//					BLACK,    		BLUE,     		GREEN,    		CYAN,
//					RED,    		MAGENTA,    	BROWN,     		LIGHTGRAY,
//					DARKGRAY,  		LIGHTBLUE,    	LIGHTGREEN,    	LIGHTCYAN,
//					LIGHTRED,  		LIGHTMAGENTA,  	YELLOW,    		WHITE

const char 	 	header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
const double 	LotusPICX =  3200;
const double 	LotusPICY =  2311;
double 			ScreenX   = 2400;//10000;
double 			ScreenY   = 3400;//10000;
const float 	Pic_default_size = 80; //standaard grootte letters in PIC file


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
int  		Pic_Open(HWND hwnd);
void	 	Pic_End(HWND hwnd);
void 		Picfont(char font);
void	 	Piccolor(HDC hdc,int color, int thickness);
void 		Picsize(int x, int y);
void 		Pictext(HDC hdc, int direction,int position,char* s,float size);
void	 	Picxy(int x,int y);
void 		Picbox(HDC hdc,int ox,int oy,int lx,int ly);
void		DrawPolygon(HDC hdc,	POINT *drawpos, int vertices, int fill);

double  	PicX,PicY,GraphX,GraphY;
int   		MaxX,MaxY; 					//screen coordinates
short  		nDrawingMode = R2_COPYPEN ;
int			Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
RECT 		rect;
PAINTSTRUCT	ps;
HPEN        hpen,hpen1, hpenOld;
LOGPEN 		lp;
POINT		box[5];//--------------------
HBRUSH		brush;
LOGBRUSH 	lb;

//-----------------------------------------------------------------------

/*************** Subroutines tekenen en schrijven PIC file ***************/
void ConvertPicXY(double x,double y)
{
 PicX  = (LotusPICX / ScreenX)    * x;
 PicY  = (LotusPICY / ScreenY)    * y;
 GraphX= ((double)MaxX / ScreenX) * (x + axcal.X_Offset);
 GraphY= ((double)MaxY / ScreenY) * (y + axcal.Y_Offset);
}

void Draw(HDC hdc,double xp,double yp)
  {
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(DRAW,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen) LineTo(hdc,(int)GraphX,(int)GraphY);//MaxY-
  }

void Move(HDC hdc,double xp,double yp)
{
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(MOVE,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
	 {
	  MoveTo(hdc,(int)GraphX,(int)GraphY);//MaxY-
	  Last_move_X=(int)GraphX;
	  Last_move_Y=(int)GraphY;//MaxY-
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

 void Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
	SetTextAlign(hdc,TA_LEFT|TA_BASELINE);
	Last_move_Y+=tm.tmHeight*size/2;
	Pictext(hdc,((int)(richting)),CENTERLEFT,tekst,size);
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
	Last_move_Y+=tm.tmHeight*size/2;
	Pictext(hdc,((int)(richting)),CENTERRIGHT,tekst,size);
  }

 void Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst)
 {
  Move(hdc,xpos,ypos);
  SetTextAlign(hdc,TA_CENTER|TA_BASELINE);
	Last_move_Y+=tm.tmHeight*size/2;
  Pictext(hdc,((int)(richting)),CENTER,tekst,size);
 }


/*************** openen en verwerken PIC file ***************/
int Pic_Open(HWND hwnd)
{
 int n;

  if(DrawPicFile)
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
	lb.lbStyle = BS_SOLID;
	lb.lbColor = dwColor[color];
	lb.lbHatch = NULL;
	SetTextColor(hdc,dwColor[color]);
//	SetBkColor(hdc,dwColor[DARKGRAY]);
	DeleteObject(SelectObject(hdc, hpen));
	}
 }

void Picsize(int x, int y)
  {
  if(DrawPicFile) { fputc(_SIZE,fpout); Picxy(x,y); }
  }

void Pictext(HDC hdc, int direction,int position,char* s,float size)
 {
 TEXTMETRIC ed;
 int nop,nop1;

 HFONT 	hfont, hfontOld;

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
	 nop=lf.lfHeight;
	 nop1=lf.lfWidth;
	 lf.lfHeight=(short)(size*(float)lf.lfHeight*(float)cxClient/(float)cxClientOrg);
	 lf.lfWidth=(short)(lf.lfWidth*size*(float)cyClient/(float)cyClientOrg);
	 lf.lfEscapement=direction * 10;
	 hfont = CreateFontIndirect(cf.lpLogFont);
	 switch (position)
		{
		case 0:
		case 1:
		case 3:
			GetTextMetrics(hdc,&ed);
			//Last_move_Y+=lf.lfHeight/2;
		}
	 lf.lfHeight=nop;
	 lf.lfWidth=nop1;
	  }

	hfontOld = SelectObject(hdc, hfont);
	TextOut(hdc,Last_move_X,Last_move_Y,s,strlen(s));
	DeleteObject(SelectObject(hdc, hfontOld));
/*




	  //			hfont = CreateFont(-tm.tmHeight*(size), -tm.tmAveCharWidth*(size),  direction * 10,0, FW_NORMAL,
			hfont = CreateFont(-hoog*(size*0.9), -breed*(size*0.9),  direction * 10,0, FW_NORMAL,
			0, 0, 0, 0, OUT_TT_PRECIS, CLIP_LH_ANGLES, PROOF_QUALITY, 0x2000, pszFace);
			hfontOld = SelectObject(hdc, hfont);

 //			if((GetDeviceCaps(hdc,TEXTCAPS) == TC_CR_90 ) && direction==90)
				TextOut(hdc,Last_move_X,Last_move_Y,s,strlen(s));
//			else
//				TextOut(hdc,Last_move_X,0,s,strlen(s));
			DeleteObject(SelectObject(hdc, hfontOld));
	 }
*/


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
float Data_X_2screen(float number)
{
 if(axcal.X_Astrans==1)
  return (axcal.X_Offset   +  ((log(number)
		- log(axcal.X_Min))  /  (log(axcal.X_Max)
		- log(axcal.X_Min)))  * axcal.X_Length);
 else
  return (axcal.X_Offset+((number - axcal.X_Min)/(axcal.X_Max - axcal.X_Min))  * axcal.X_Length);
}

float Data_Y_2screen(float number)
{
 if(axcal.Y_Astrans==1)
  return (axcal.Y_Offset +
			((log(number)	   - log(axcal.Y_Min)) /
			 (log(axcal.Y_Max)- log(axcal.Y_Min)))
			* axcal.Y_Length);
 else
  return (axcal.Y_Offset+((number - axcal.Y_Min)/(axcal.Y_Max - axcal.Y_Min))  * axcal.Y_Length);
}

void	DrawPolygon(HDC hdc,	POINT *drawpos, int vertices, int fill)
  {
	int n;
	 for(n=0;n<vertices;n++)
	  {
		ConvertPicXY((double)drawpos[n].x,(double)drawpos[n].y);
		drawpos[n].x=(int)GraphX;
		drawpos[n].y=MaxY-(int)GraphY;
	  }
	 if(fill) brush=SelectObject(hdc,CreateBrushIndirect(&lb));//GetStockObject(BLACK_BRUSH));
	 hpenOld = SelectObject(hdc, CreatePenIndirect(&lp));
	 Polygon(hdc,drawpos,vertices);
	 DeleteObject(SelectObject(hdc, hpenOld));
	 if(fill) DeleteObject(SelectObject(hdc, brush));
  }


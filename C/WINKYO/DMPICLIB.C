//PIC files en grafisch routines Ed Nieuwenhuys 1993

#include "picdump.h"
#include "commdlg.h"

extern CHOOSEFONT cf;

/*Een grafisch scherm is 10000 units breed en 10000 units hoog.
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

DWORD dwColor[16]={RGB(0,0,0),		RGB(0,0,128),	RGB(0,128,0),		RGB(0,255,255),
						 RGB(128,0,0),		RGB(255,0,255),RGB(128,128,0),	RGB(192,192,192),
						 RGB(128,128,128),RGB(0,0,255),	RGB(0,255,0),		RGB(0,128,128),
						 RGB(255,0,0),		RGB(128,0,128),RGB(255,255,0),	RGB(220,220,220)};

//							BLACK,    		BLUE,     		GREEN,    			CYAN,
//							RED,    			MAGENTA,    	BROWN,     			LIGHTGRAY,
//							DARKGRAY,  		LIGHTBLUE,    	LIGHTGREEN,    	LIGHTCYAN,
//							LIGHTRED,  		LIGHTMAGENTA,  YELLOW,    			WHITE

const char 			header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
const double 		LotusPICX =  3200;
const double 		LotusPICY =  2300;
double 				ScreenX   = 10000;
double 				ScreenY   = 10000;
const float  		Pic_default_size = 80; //standaard grootte letters in PIC file


// Funtion prototypes

void 		ConvertPicXY(double x,double y);
void 		Draw(HDC hdc,double xp,double yp);
void		DrawPolygon(HDC hdc,POINT *drawpos, int vertices, int fill);
void 		DrawEllipse(HDC hdc,double left,double top,double right,double bottom);
void 		Move(HDC hdc,double xp,double yp);
void 		Point(HDC hdc,double xp,double yp, double box);
void 		Numm(HDC hdc,double xp,double yp,int number);
void 		Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst);
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
char 		Teken_assen(HWND hwndas,HDC hdc,struct ASCAL *ascal);
char 		Teken_data(HWND hwndas,HDC hdc,struct ASCAL *ascal);

double  			PicX,PicY,GraphX,GraphY;
int   			MaxX,MaxY; 					//screen coordinates
short  			nDrawingMode = R2_COPYPEN ;
int				Last_move_X,Last_move_Y; //coord van laatste move voor Pictext
HFONT 			hfont, hfontOld;
RECT 				rect;
PAINTSTRUCT		ps;
HPEN         	hpen,hpen1, hpenOld;
HBRUSH			brush;
LOGPEN 			lp;
LOGBRUSH 		lb;

//-----------------------------------------------------------------------

/*************** Subroutines tekenen en schrijven PIC file ***************/
void ConvertPicXY(double x,double y)
{
 PicX  = (LotusPICX / ScreenX)    * x;
 PicY  = (LotusPICY / ScreenY)    * y;
// GraphX= ((double)MaxX / ScreenX) * x;
// GraphY= ((double)MaxY / ScreenY) * y;
 GraphX= ((double) MaxX / LotusPICX) * x;// * 0.9;
 GraphY= ((double) MaxY / LotusPICY) * y;// * 0.9;
}

void Draw(HDC hdc,double xp,double yp)
  {
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(DRAW,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
		{
		hpenOld = SelectObject(hdc, CreatePenIndirect(&lp));
		LineTo(hdc,(int)GraphX,MaxY-(int)GraphY);
		DeleteObject(SelectObject(hdc, hpenOld));
		}
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

  void DrawEllipse(HDC hdc,double left,double top,double right,double bottom)
  {
	ConvertPicXY(left,top);
	left=GraphX;
	top =GraphY;
	ConvertPicXY(right,bottom);
//	if(DrawPicFile)   { fputc(DRAW,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
		{
		hpenOld = SelectObject(hdc, CreatePenIndirect(&lp));
		Ellipse(hdc,(int)left,MaxY-(int)top,(int)GraphX,MaxY-(int)GraphY);
		DeleteObject(SelectObject(hdc, hpenOld));
		}
  }

  void DrawFillEllipse(HDC hdc,double left,double top,double right,double bottom)
  {
	ConvertPicXY(left,top);
	left=GraphX;
	top =GraphY;
	ConvertPicXY(right,bottom);
//	if(DrawPicFile)   { fputc(DRAW,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
		{
	 	brush=SelectObject(hdc,CreateBrushIndirect(&lb));//GetStockObject(BLACK_BRUSH));
		hpenOld = SelectObject(hdc, CreatePenIndirect(&lp));
		Ellipse(hdc,(int)left,MaxY-(int)top,(int)GraphX,MaxY-(int)GraphY);
		DeleteObject(SelectObject(hdc, hpenOld));
		DeleteObject(SelectObject(hdc, brush));
		}
  }

void Move(HDC hdc,double xp,double yp)
{
	ConvertPicXY(xp,yp);
	if(DrawPicFile)   { fputc(MOVE,fpout);  Picxy(PicX,PicY); }
	if(DrawGraphScreen)
	 {
	  MoveTo(hdc,(int)GraphX,MaxY-(int)GraphY);
	  Last_move_X=(int)GraphX;
	  Last_move_Y=MaxY-(int)GraphY;
	 }
}

void Point(HDC hdc,double xp,double yp, double box)
	{
	double boxx,boxy;
	boxx=box;
	boxy=box*cxClient/cyClient;
	Move(hdc,xp+boxx,yp+boxy);	Draw(hdc,xp-boxx,yp+boxy);
	Draw(hdc,xp-boxx,yp-boxy);	Draw(hdc,xp+boxx,yp-boxy);
	Draw(hdc,xp+boxx,yp+boxy);
	}

 void Numm(HDC hdc,double xp,double yp,int number)
  {
	unsigned char tekst[10];
	itoa(number,tekst,10);
	Printc(hdc,xp,yp,1,0,tekst);
  }

 void Print(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
   SetTextAlign(hdc,TA_LEFT|TA_BASELINE);
	Pictext(hdc,richting,BOTTOMLEFT,tekst,size);
  }
 void Printr(HDC hdc,double xpos,double ypos,float size,int richting,char *tekst)
  {
	Move(hdc,xpos,ypos);
	SetTextAlign(hdc,TA_RIGHT|TA_BASELINE);
	Pictext(hdc,richting,BOTTOMRIGHT,tekst,size);
  }

 void Printc(HDC hdc, double xpos,double ypos,float size,int richting, char *tekst)
 {
  Move(hdc,xpos,ypos);
  SetTextAlign(hdc,TA_CENTER|TA_BASELINE);
  Pictext(hdc,richting,CENTER,tekst,size);
 }


/*************** openen en verwerken PIC file ***************/
int Pic_Open(HWND hwnd)
{
 int n;
 HDC hdc;

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

//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = thickness;
	lp.lopnWidth.y = 0;              /* y-dimension not used */
	lp.lopnColor = dwColor[color];
	lb.lbStyle = BS_SOLID;
	lb.lbColor = dwColor[color];
	lb.lbHatch = NULL;
	SetTextColor(hdc,dwColor[color]);
	}
 }
void PicRGBcolor(HDC hdc,int Rcolor,int Gcolor,int Bcolor,int thickness)
 {
	if(DrawPicFile)		fputc(COLOR0,fpout);
	if(DrawGraphScreen)
	{
	lp.lopnStyle = PS_SOLID;
	lp.lopnWidth.x = thickness;
	lp.lopnWidth.y = 0;              /* y-dimension not used */
	lp.lopnColor = RGB(Rcolor,Gcolor,Bcolor);
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(Rcolor,Gcolor,Bcolor);
	lb.lbHatch = NULL;
	SetTextColor(hdc,RGB(Rcolor,Gcolor,Bcolor));
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
			Last_move_Y-=lf.lfHeight/2;
	 }
	 lf.lfHeight=nop;
	 lf.lfWidth=nop1;
	 }

	hfontOld = SelectObject(hdc, hfont);
	TextOut(hdc,Last_move_X,Last_move_Y,s,strlen(s));
	DeleteObject(SelectObject(hdc, hfontOld));
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



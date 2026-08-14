
// window varia   Ed Nieuwenhuys 1993

#include "WCORR.H"

#define TRUE      1
#define FALSE     0
#define LEEG     -1

extern HPEN   hPenBlack, hPenWhite ,hPenBlue,hPenRed,hPenGreen,hPenYellow;

HBRUSH hbrush;
char   Specialkey,Lastkey,Rounded,YN;
int    Userabort=FALSE;
int    actcpos,maxcel,Init;
int    Calcmode,Inputmode;

//        wvaria           function prototypes
void   Gotoxy(int x,int y,int *xn,int *yn);
void   WisRect(HDC hdc,int x,int y);
void   Cputs(HDC hdc,int x,int y,char *str);
void   CputsRC(HDC hdc,int x,int y,char *str);
void   CputsR(HDC hdc,int x,int y,char *str);
void   CputsL(HDC hdc,int x,int y,char *str);
void   VerwerkSkey(char toets);
char   getkey(HWND);
char   *Cgets(HDC,int,int,char*,HWND);
RECT   recttext;		/*Rectangle for drawtext*/


// ------------------------------------------------------------


void Gotoxy(int x,int y,int *xn,int *yn)
{
 *xn=x * cxChar;
 *yn=y * cyChar+1;
}

void WisRect(HDC hdc,int x,int y)
{
 SetRect(&recttext,x,y,x+cxChar*10,y+cyChar);
 FillRect(hdc,&recttext,GetStockObject(WHITE_BRUSH));
}

void Cputs(HDC hdc,int x,int y,char *str)
{
TextOut(hdc,x,y,str,strlen(str));
}

void CputsRC(HDC hdc,int x,int y,char *str) // zet txt in rectangle
{
WORD nop;
 nop=SetTextAlign(hdc, TA_RIGHT);
 TextOut(hdc,x+cxChar*9,y,str,strlen(str));
 hbrush=SelectObject(hdc,GetStockObject(NULL_BRUSH));
 if(Rounded)	RoundRect(hdc,x,y,x+cxChar*10,y+cyChar,20,20);
 else 			Rectangle(hdc,x,y,x+cxChar*10,y+cyChar);
 DeleteObject(hbrush);
 SetTextAlign(hdc, HIWORD(nop)|LOWORD(nop));
}


void CputsR(HDC hdc,int x,int y,char *str)
{
 SetRect(&recttext,0,0,x,y);
 DrawText(hdc,str,-1,&recttext,DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
}
void CputsL(HDC hdc,int x,int y,char *str)
{
 SetRect(&recttext,x,0,cxClient,y);
 DrawText(hdc,str,-1,&recttext,DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
}

char *Cgets(HDC hdc,int x,int y,char* tekst,HWND hwndGet)
{
char *c;

int  i=0;
	c=tekst;
	while( TRUE)
     {
		*c=getkey(hwndGet);
		if( *c >31 &&!Specialkey )
	 	{
	 		TextOut(hdc,x+cxChar,y,tekst,++i);
	 		++c;
	 	}
		else if(*c==VK_BACK&&i>=0)
	 	{
	 		tekst[i-1]=' ';
	 		tekst[i]=' ';
	 		TextOut(hdc,x+cxChar,y,tekst,i+1);
	 		if(i>0) --i;
	 		--c;
	 	}
		else break;
      }
	Lastkey = *c;
	*c=0;
return(tekst);
}

void VerwerkSkey(char toets)
{
       switch(toets)
	{
	   case     VK_UP:    break;
	   case   VK_LEFT:       break;
	   case   VK_DOWN:       break;
	   case  VK_RIGHT:            break;
	   case   VK_HOME:        break;
	   case    VK_END:   break;
	   case VK_DELETE: 
			   break;
	   case VK_ESCAPE: break;
	   case VK_RETURN:
			   break;
	  }
 }

/*............................  Getkey .......................*/
char getkey(HWND hwndkey)
{
 MSG   msg;
 char i;

i  = FALSE;
Specialkey = FALSE;

while(!i )
	{
	   if(!GetMessage(&msg,hwndkey,0,0) ){ Userabort=TRUE ; }
	   TranslateMessage (&msg) ;
	   DispatchMessage (&msg) ;
	   if(msg.message == WM_KEYDOWN)
	      {
		  		i=(char)msg.wParam;
		  		Specialkey = 1;
	      }
		if(PeekMessage(&msg,hwndkey,0,0,PM_REMOVE))
      	{
	   	if(msg.message == WM_CHAR)
			 {
			 	i=(char)msg.wParam;
		 		Specialkey = FALSE;
	       }
      	if(msg.message == WM_PAINT)
			 {
			 	if(Inputmode)
	         	{
                  Init = TRUE;
		  				i=VK_HOME;
	          	}
          }
			if(msg.message == WM_QUIT)
			 {
	     		EndPaint (hwndkey, &ps) ;
	     		PostMessage(HWND_BROADCAST,WM_QUIT,NULL,NULL);
          }
	      }
	 }
Lastkey = i;
return(i);
}


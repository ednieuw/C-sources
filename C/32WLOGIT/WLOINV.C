
// window logit invoer wloinv.c  Ed Nieuwenhuys dec 1993

#include "wlogit.h"

FARPROC lpfnIKnop;
HANDLE hInstIKnop;
HBRUSH hbrush;
char   Specialkey,Lastkey;
char	 Rounded,YN;
int    Userabort=FALSE;
int    actcpos,maxcel,Init;
int    Calcmode,Manualinput=FALSE,Invoerscherm=FALSE;//,Manualberekenen=FALSE;
int    XX[MAX_YK_PNT*2+10],YY[MAX_YK_PNT*2+10];
double Conc=1000,Dilfactor=2;
int    Replicas=1;//,maxcel = 0;

//                   function prototypes
BOOL 	 FAR PASCAL IKnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam);
int 	 DoInputOpenDlg (HANDLE Inst, HWND hwndparent);
BOOL 	 FAR PASCAL InputDlgProc (HWND, WORD, WORD, LONG);
void   Gotoxy(int x,int y,int *xn,int *yn);
void   WisRect(HDC hdc,int x,int y);
void   Cputs(HDC hdc,int x,int y,char *str);
void   CputsRC(HDC hdc,int x,int y,char *str);
void   CputsR(HDC hdc,int x,int y,char *str);
void   CputsL(HDC hdc,int x,int y,char *str);
void   Manual_input(HWND);
void 	 InvoerScherm(HWND hwndEdit);
void   VerwerkSkey(char toets);
char   getkey(HWND);
char   *Cgets(HDC,int,int,char*,HWND);

extern PAINTSTRUCT   ps ;
RECT   recttext;		/*Rectangle for drawtext*/


BOOL FAR PASCAL IKnoppenProc (HWND hDlg, WORD message, WORD wParam, LONG lParam)
 {
  char text[80];
  switch (message)
	  {
		case WM_INITDIALOG:
			sprintf(text,"%6.3f",Conc);
			SetDlgItemText(hDlg,IDM_HIGHCONC,text);
			sprintf(text,"%6.1f",Dilfactor);
			SetDlgItemText(hDlg,IDM_DILFACTOR,text);
			sprintf(text,"%2d",Replicas);
			SetDlgItemText(hDlg,IDM_NOOFREPL,text);
					 return TRUE ;
		case WM_KEYDOWN:
		   if(wParam==VK_RETURN) return 0;

		case WM_COMMAND :
			 switch (LOWORD(wParam))
            {
			 case  IDM_HIGHCONC:
								GetDlgItemText(hDlg,IDM_HIGHCONC,text,20);
								Conc=atof(text);
                        if(Conc==0) Conc=1000;
								return 0;
			 case  IDM_DILFACTOR:
								GetDlgItemText(hDlg,IDM_DILFACTOR,text,20);
								Dilfactor=atof(text);
                        if(Dilfactor<1) Dilfactor=2;
                        return 0;
			 case  IDM_NOOFREPL:
								GetDlgItemText(hDlg,IDM_NOOFREPL,text,20);
								Replicas=atof(text);
                        if(Replicas<1||Replicas>5) Replicas=1;
                        return 0;
//			 case IDM_OK:
  //								EndDialog (hDlg, wParam) ;
	 //							return TRUE ;
			 default:
								EndDialog (hDlg, wParam) ;
								return TRUE ;
				 }
	  }
	  return FALSE ;
	  }
//----------------------------------------------------------------------------------------------
int DoInputOpenDlg (HANDLE Inst, HWND hwndparent)
	 {
	  MSG      msg;
	  HWND     hwnd ;
	  WNDCLASS wndclass ;
	  static char szAppName[] = "Input";//"Wcorr" ;

	  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;//| CS_SAVEBITS;
	  wndclass.lpfnWndProc	 = (BOOL (FAR PASCAL*)())InputDlgProc;
	  wndclass.cbClsExtra    = 0 ;
	  wndclass.cbWndExtra    = 0 ;
	  wndclass.hInstance     = Inst;
	  wndclass.hIcon         = NULL;
	  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
	  wndclass.lpszMenuName  = NULL;//szAppName ;
	  wndclass.lpszClassName = szAppName ;//"Graph";

	  RegisterClass (&wndclass) ;
	  hwnd = CreateWindow (szAppName ,NULL,
			  //WS_POPUP,
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  CW_USEDEFAULT, CW_USEDEFAULT,
			  hwndparent, NULL, Inst, NULL);
	  ShowWindow (hwnd,  SW_SHOWMAXIMIZED);
	  UpdateWindow (hwnd);
	  while (GetMessage (&msg, NULL, 0, 0))
			 {
			 TranslateMessage (&msg) ;
			 DispatchMessage (&msg) ;
			 }
	  return msg.wParam ;
	  }


BOOL FAR PASCAL InputDlgProc (HWND hwnd, WORD message, WORD wParam, LONG lParam)
	  {
	  HDC hdc;
	  switch (message)
        {
	  case WM_CREATE:
          hdc=GetDC(hwnd);
			 GetTextMetrics (hdc, &tm) ;
			 cxChar = tm.tmAveCharWidth ;
			 cyChar = tm.tmHeight + tm.tmExternalLeading ;
			 hInstIKnop = ((LPCREATESTRUCT) lParam)->hInstance ;
			 lpfnIKnop = MakeProcInstance (IKnoppenProc, hInstIKnop) ;
			 ReleaseDC(hwnd,hdc);
			 return 0 ;
	  case WM_SIZE:
			 cxClient= LOWORD(lParam);
			 cyClient= HIWORD(lParam);
			 return 0 ;
	  case WM_KEYDOWN:
			 Lastkey=(char)wParam;
			 Specialkey = TRUE;
			 return 0;
	  case WM_CHAR:
			 Lastkey=(char)wParam;
			 Specialkey = FALSE;
          return 0;
	  case WM_PAINT :
			 hdc = BeginPaint (hwnd, &ps) ;
			 if(Manualinput)			Manual_input(hwnd);
			 if(Invoerscherm)		 	InvoerScherm(hwnd);
			 EndPaint(hwnd,&ps);
			Manualinput=FALSE;
			Invoerscherm=FALSE;
 //			 return 0;

/*	  case WM_RBUTTONDOWN:
	  case WM_KEYDOWN:
	  case WM_LBUTTONDOWN:
								DestroyWindow (hwnd) ;
								return 0;
	  case WM_MBUTTONDOWN:
								return 0;
*/	  case WM_CLOSE:
								DestroyWindow (hwnd) ;
								return 0 ;
	  case WM_DESTROY:
	       					PostQuitMessage (0) ;
			 					return 0 ;
	  case WM_QUIT:
	       					PostQuitMessage (0) ;
			 					return 0 ;
	  }
	  return DefWindowProc (hwnd, message, wParam, lParam) ;
  }

// -------------------------------------------------------------------------------------

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

void CputsRC(HDC hdc,int x,int y,char *str)
{
WORD nop;
nop=SetTextAlign(hdc, TA_RIGHT);
TextOut(hdc,x+cxChar*9,y,str,strlen(str));
hbrush=SelectObject(hdc,GetStockObject(NULL_BRUSH));
if(Rounded) RoundRect(hdc,x,y,x+cxChar*10,y+cyChar,20,20);
else Rectangle(hdc,x,y,x+cxChar*10,y+cyChar);
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


void   Manual_input(HWND hwndEdit)
{
HDC    hdcEdit;
DLGPROC dlgprc;
int    i,j,m,n;
char   text[40];
WORD 	 wParam;

Init=TRUE;
hdcEdit = GetDC (hwndEdit) ;
//Manualinput = TRUE;
 if((rows_temp<3 || aantal<3) && geladen) fill_temp(hwndEdit,0,1);
 rows=rows_temp;
 HAND=FALSE;
 YN='Y';
 if(ykpunten[2]==TRUE)
	{
	 YN=MessageBox(hwndEdit,"Correction ?","Cancel = New dataset",MB_OKCANCEL);
	 if(YN==IDOK)
	 {
		 if(temp[0][rows+1]!=MISSING) YN='Y';
		 maxcel=rows_temp*2;
		 goto Entry;
	 }

else {
		rows_temp=0;
		YN='Y';
		Conc=1000;
		Dilfactor=2;
		Replicas=1;
		maxcel = 0;
	  }
    }
 wParam=DialogBox(hInstIKnop,"AUTOCONC",hwndEdit,lpfnIKnop);
 if(Dilfactor>1)
	{
	geladen=FALSE;
	  for(j=0;j<MAX_YK_PNT;j++)
		{
		 for(i=0;i<2;i++)  temp[i][j]=MISSING;
		 ykpunten[j]=FALSE;
		}
	  C=MISSING;
	  Conc *= Dilfactor;
	  m=0;
	  while(m<MAX_YK_PNT)
			{
			 Conc /= Dilfactor;
			for(n=0;n<Replicas;n++)    temp[0][m++] = Conc;
			}
	  }
 Entry:
 ReleaseDC(hwndEdit,hdcEdit);

 }
//--------------------------------invoerscherm--------------------------------------------------
 void InvoerScherm(HWND hwndEdit)
 {
 HDC 	  hdcEdit;
 int 	  i,j,n;
 int    Oldcpos=0,cpos=0; /* cursor position */
 double Vx;
 char   Invoer[20],toets=0;
 char   text[40];
 hdcEdit=GetDC(hwndEdit);
 xx=0;
 yy=0;
 SetTextColor(hdcEdit,0X00FF33FFL);
 Gotoxy(10,0,&xx,&yy);
 Cputs(hdcEdit,xx,yy,"Press ESCAPE to stop");
 SetTextColor(hdcEdit,0X003344FFL);
 Gotoxy(8,1,&xx,&yy);
 Cputs(hdcEdit,xx,yy,"Conc");
 Gotoxy(19,1,&xx,&yy);
 Cputs(hdcEdit,xx,yy,"Meas");
 Gotoxy(37,1,&xx,&yy);
 Cputs(hdcEdit,xx,yy,"Conc");
 Gotoxy(48,1,&xx,&yy);
 Cputs(hdcEdit,xx,yy,"Meas");

 j=7; i=3;
 XX[0]=18;  YY[0]=2;
 for(n=1;n<MAX_YK_PNT*2-1;n+=2)
  {
  if(i<27) {	XX[n]   = j;  	YY[n]   = i;
		XX[n+1] = j+11;	YY[n+1] = i++;	   }
      else {	j=37;
		XX[n]   = j;	YY[n]   = i-25;
		XX[n+1] = j+11; YY[n+1] = i-25;
		i++;				   }
  }
  cpos=0;
  actcpos=0;
 for(n=0;n<MAX_YK_PNT*2-1;n++)
  {
	Gotoxy(XX[cpos],YY[cpos],&xx,&yy);
   Rectangle(hdcEdit,xx,yy,xx+cxChar*10,yy+cyChar);
   cpos++;
  }
  cpos=0;

 while(TRUE)
 {
  SetTextColor(hdcEdit,GetSysColor(COLOR_WINDOWTEXT));
  cpos=0;
  Gotoxy(7,2,&xx,&yy);                             
  CputsRC(hdcEdit,xx,yy,"Blank = 0");
  if(cpos==actcpos)
    {  Rounded =TRUE;	  SetTextColor(hdcEdit,GetSysColor(COLOR_MENU));    }
  else Rounded = FALSE;
  Gotoxy(XX[cpos],YY[cpos],&xx,&yy);
  WisRect(hdcEdit,xx,yy);
  if(C==MISSING){ sprintf(text,"%8s","   ?  ");   CputsRC(hdcEdit,xx,yy,text); }
  else {  			sprintf(text,"% 8.6g",C); 	     CputsRC(hdcEdit,xx,yy,text); }
  SetTextColor(hdcEdit,GetSysColor(COLOR_WINDOWTEXT));
  for(i=0,j=2;i<rows+2;i++)
	 {
	if(ykpunten[i])
	  {
	  cpos++;
//   if(Init || cpos==Oldcpos || cpos==actcpos || YN=='Y')
      {
		 if(cpos==actcpos) Rounded =TRUE; else Rounded = FALSE;
       if(cpos==actcpos)  SetTextColor(hdcEdit,GetSysColor(COLOR_MENU));
       Gotoxy(XX[cpos],YY[cpos],&xx,&yy);
       WisRect(hdcEdit,xx,yy);
       if(temp[0][i]==MISSING) sprintf(text,"%8s","       ");
//		 else	 if(temp[1][i]==MISSING) sprintf(text,"%8s","       ");
		 else  sprintf(text,"% 6.5g",temp[0][i]);
		 CputsRC(hdcEdit,xx,yy,text);
      }
       SetTextColor(hdcEdit,GetSysColor(COLOR_WINDOWTEXT));
		 cpos++;
//   if(Init || cpos==Oldcpos || cpos==actcpos)
      {
		 SetTextColor(hdcEdit,GetSysColor(COLOR_WINDOWTEXT));
		 if(cpos==actcpos) Rounded =TRUE; else Rounded = FALSE;
       if(cpos==actcpos)  SetTextColor(hdcEdit,GetSysColor(COLOR_MENU));
       Gotoxy(XX[cpos],YY[cpos],&xx,&yy);
       WisRect(hdcEdit,xx,yy);
		 if(temp[1][i]==MISSING) 		 sprintf(text,"%8s","       ");
		 else	 if(temp[0][i]==MISSING) sprintf(text,"%8s","       ");
		 else		       					 sprintf(text,"% 6.5g",temp[1][i]);
       CputsRC(hdcEdit,xx,yy,text);
       SetTextColor(hdcEdit,GetSysColor(COLOR_WINDOWTEXT));
      }
	  }
	 else {
	  Rounded = FALSE;
/*		 Gotoxy(XX[i*2+2],YY[i*2+2],&xx,&yy);
		 WisRect(hdcEdit,xx,yy);
		 sprintf(text,"%8s","       ");     CputsRC(hdcEdit,xx,yy,text);
		 Gotoxy(XX[i*2+3],YY[i*2+3],&xx,&yy);
		 WisRect(hdcEdit,xx,yy);
		 CputsRC(hdcEdit,xx,yy,text);
*/     }
	 }
    Rounded=FALSE;
    Init=FALSE;
    toets = getkey(hwndEdit);
    if(toets==VK_ESCAPE) break;
    Oldcpos = actcpos;
    if(Specialkey||toets==13)VerwerkSkey(toets);
	  else
	   {
       switch(toets)
			{
	   case 46:
	   case 48:
		case 49:
		case 50:
	   case 51:
	   case 52:
	   case 53:
	   case 54:
	   case 55:
	   case 56:
	   case 57:
       n=0;
       SetTextColor(hdcEdit,0x00FF00FFL);
       Gotoxy(XX[actcpos],YY[actcpos],&xx,&yy);
		 PostMessage(hwndEdit,WM_KEYDOWN,0,0);
		 PostMessage(hwndEdit,WM_CHAR,toets,0);
		 PostMessage(hwndEdit,WM_KEYUP,0,0);
		 Vx=atof(Cgets(hdcEdit,xx,yy,Invoer,hwndEdit));
       if(Lastkey==VK_ESCAPE) continue;
       if(actcpos==0)   C = Vx; 
		 else
			{
			 i= (actcpos-1)/2;
			 if((actcpos/2)*2==actcpos)
				{
				temp[1][i] = Vx;
				ykpunten[i]=TRUE;
				}
			 else  temp[0][i] = Vx;
	  		}
		 VerwerkSkey(Lastkey);
       break;
        }	//einde switch
		} 		//einde else

       if(toets == VK_ESCAPE) break;
		 rows=1;
		 for(n=0;n<MAX_YK_PNT;n++)    if(ykpunten[n]) rows=n+1;
       if(actcpos > rows*2)   	   rows++; ykpunten[rows-1]=TRUE;
       if(actcpos > MAX_YK_PNT*2-1) actcpos=0;
       if(actcpos<0)                actcpos=0;
       if(maxcel >= MAX_YK_PNT*2-1)  maxcel = MAX_YK_PNT*2-1;
       if(maxcel<0)                 maxcel = 0;

 }
 rows=0;
 for(n=0;n<MAX_YK_PNT;n++)
	 {
	  if(temp[0][n]!=MISSING && temp[1][n]!=MISSING){ ykpunten[n]=TRUE; rows++;}
	 }
 geladen = FALSE;
 if(rows<3) return; // Te weinig datapunten
 geladen=TRUE;
 rows_temp=rows;
 HAND=TRUE;
 for(i=0;i<=rows;i++)
	{
	 data[0][i]=temp[0][i];
	 data[1][i]=temp[1][i];
	}
ReleaseDC(hwndEdit,hdcEdit);

}

//----------------------------- verwerkSkey -------------------------------------------------
void VerwerkSkey(char toets)
{
 int n,i;
 switch(toets)
	{
		case     VK_UP:   actcpos -= 2;      break;
		case   VK_LEFT: --actcpos;           break;
	   case   VK_DOWN:   actcpos += 2;      break;
	   case  VK_RIGHT: ++actcpos;           break;
	   case   VK_HOME:   actcpos = 0;       break;
	   case    VK_END:   actcpos = maxcel;  break;
	   case VK_DELETE: i=(actcpos+1)/2-1;   //wissen ijklijnpaar
			   for(n=i;n<MAX_YK_PNT-1;n++)
			    {
			     temp[0][n]=temp[0][n+1];      temp[1][n]=temp[1][n+1];
			     ykpunten[n]=ykpunten[n+1];
			    }
			    maxcel-=2;
			    actcpos-=2;
			   break;
	   case VK_ESCAPE: break;
	   case VK_RETURN:
			   if(actcpos==0 && C == MISSING)  C=0;
				if(actcpos >= maxcel )
				{
				  maxcel++;
				  if(YN=='Y') maxcel++;
				  actcpos = maxcel;
				}
				break;
	  }
 if (actcpos>=maxcel) maxcel=actcpos;
 }

//------------------------------------  Getkey ---------------------------------------
char getkey(HWND hwndkey)
{
 MSG   msg;
 char i;

 i = FALSE;
 Specialkey = FALSE;

 while(!i ) {
		if(!GetMessage(&msg,hwndkey,0,0) )		 { Userabort=TRUE ; }
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
		if(msg.message == WM_KEYDOWN)    { i=(char)msg.wParam; Specialkey = TRUE;    }
		if(msg.message == WM_QUIT)  PostMessage(HWND_BROADCAST,WM_QUIT,0,0);
		if(PeekMessage(&msg,hwndkey,0,0,PM_REMOVE))
				{
//				if(msg.message == WM_PAINT){ if(Inputmode) { Init = TRUE;i=VK_HOME; 	}  }
				if(msg.message == WM_CHAR)	{ i=(char)msg.wParam; Specialkey = FALSE;   	}
				}
	 }

 Lastkey = i;
return(i);

}


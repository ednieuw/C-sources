// Invoer routines voor LOGIT Ed Nieuwenhuys juli 1993
#include "alloc.h"
#include "graphics.h"
#include "conio.h"
#include "io.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "process.h"
#include "dir.h"
#include "time.h"
#include "dos.h"
#include "math.h"
#include "fcntl.h"
#include "ctype.h"

#define breedte_menu     35     /* Breedte van menu's */
#define MAXDISK   5
#define TRUE      1
#define FALSE     0
#define LEEG     -1
#define SPECIAL_KEY  0
#define UP     72
#define DOWN   80
#define HOME   71
#define ENDT   79
#define LEFT   75
#define UP     72
#define EXEC   13
#define ESC    27
#define RIGHT  77
#define PgUp   73
#define PgDn   81
#define DEL    83
#define SPACE  32
#define RETURN 13
#define TAB     9
#define CHBIN 0xF000    /* Blink Invers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */
#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define BELL 7
#define MAXROWS 1500          /* maximale dimensies worksheet */
#define MAX_COL 3
#define MAX_YK_PNT 150        /* Maximaal 150 ijklijnpunten */
#define MISSING -999999.0

extern double 	**data;
extern char   	**label;
extern double 	**temp;
extern char   	**naam;

extern char 	ykpunten[MAX_YK_PNT];   /*string voor actieve ijklijnpunten */
extern int 		rows, columns,print,Manualrows;
extern int 		labels,xx,yy,aa,rows_temp,kolom;
extern int 		HAND;
extern int 		geladen;
extern double 	aantal,C,D;

extern double 	logit(double );
extern void 	fill_temp(int, int);
void   			Manual_input(void);
void   			Manual_berekenen(void);

extern double 	slope,intercept,C,D,reslogit;
extern double 	corr;
extern double 	somx,somy,upx,upy;
extern FILE 	*fpout;
extern FILE 	*printer;

int 				Correction=FALSE;
int 				Rows;
//--------------------------------------------------------------------------
void   Manual_input(void)
{
int 	 i,j,m,n;
double Conc,Dilfactor,Vx,Vy;
int    Replicas=0;
char   YN=0,Invoer[20],toets=0,ch;
int    XX[80],YY[80],cpos,actcpos; /* cursor position */

 textcolor(BLACK);
 textbackground(WHITE);
 clrscr();
 if((rows_temp<3 || aantal<3) && geladen) fill_temp(0,1);
 HAND=FALSE;
 Correction=FALSE;
 if(ykpunten[0]==TRUE)
	{
	 cputs("\r\n New dataset or Correction (N/C) ? ");
	 YN = toupper(getche());
	 if(YN=='C')
	 {
		 Correction=TRUE;
		 Rows=rows;
//		 if(temp[0][rows+1]!=MISSING) YN='Y';
		 goto Entry;
	 }
	}
 cputs("\r\n\r\r Constant dilution factor (Y/N) ? ");
 YN = toupper(getche());
 if(YN == ESC) return;
 Invoer[0]=19;
 for(j=0;j<MAX_YK_PNT;j++)
	{
	for(i=0;i<2;i++)  temp[i][j]=MISSING;
	data[2][j]=1.0;
	ykpunten[j]=FALSE;
	}

 if(YN=='J' || YN=='Y')
	{ geladen=TRUE;
	  for(j=0;j<50;j++)
		 {
			for(i=0;i<2;i++)  temp[i][j]=MISSING;
			ykpunten[j]=FALSE;
		 }
	  C=MISSING;

	  YN='Y';
	  cputs("\r\n");

	  CO:
	  cputs("\r\n        Highest concentration : ");
	  Conc=atof(cgets(Invoer));
	  if(Conc==0) goto CO;

	  DI:
	  cputs("\r\n              Dilution factor : ");
	  Dilfactor=atof(cgets(Invoer));
	  if(Dilfactor==0) goto DI;

	  cputs("\r\n               No of replicas : ");
	  Replicas=atof(cgets(Invoer));
	  if(Replicas==0) Replicas=1;
	}

 Conc *= Dilfactor;

 if(YN=='Y')
	 {
		m=0;
		while(m<50)
		{
		 Conc /= Dilfactor;
		 for(n=0;n<Replicas;n++)    temp[0][m++] = Conc;
		}
	  }
 HAND=TRUE;

 Entry:
 clrscr();
// textcolor(WHITE);

 i=4; j=18;
 for(n=0;n<80;n++)
  {
	YY[n]=i++; XX[n++]=j;
  if(i>23) {i=4; j=48;}
	j-=13;
	YY[n]=i; XX[n]=j;
	j+=13;
  }
  cpos=0;
  actcpos=0;
  geladen=TRUE;
 while(TRUE)
 {
 _setcursortype(_NOCURSOR);
  textcolor(BLACK);   textbackground(WHITE);
  gotoxy(1,1);
 textcolor(YELLOW);
 cputs("          Press ESCAPE or RETURN in last entry to stop\r\n");
 textcolor(LIGHTGREEN);
 cputs("     Conc       Meas                  Conc      Meas\r\n");
  textcolor(BLACK);   textbackground(WHITE);
  cpos=0;
  gotoxy(3,4);
  cputs("Blank");
  if(cpos==actcpos){ textcolor(YELLOW);  textbackground(BLACK);	}
  gotoxy(XX[cpos],YY[cpos]);
  cpos++;
  if(C==MISSING) cprintf("          ",C);
  else           cprintf("%8.4g  ",C);
  textcolor(BLACK);   textbackground(WHITE);
  for(i=0,j=2;i<40;i++)
	 {
	 if(ykpunten[i])
		{
		 if(cpos==actcpos){ textcolor(YELLOW);  textbackground(BLACK);	}
		 gotoxy(XX[cpos],YY[cpos]); cprintf("%8.4g\r\n",temp[0][i]);
		 cpos++;
		 textcolor(BLACK);       textbackground(WHITE);
		 if(cpos==actcpos){ textcolor(YELLOW);  textbackground(BLACK);	}
		 gotoxy(XX[cpos],YY[cpos]); cprintf("%8.4g\r\n",temp[1][i]);
		 cpos++;
		 j++; j++;
		 textcolor(BLACK);       textbackground(WHITE);
		}
	 }
	 gotoxy(XX[cpos+1],YY[cpos+1]); cprintf("             ");
	 gotoxy(XX[cpos+2],YY[cpos+2]); cprintf("             ");
//	 gotoxy(XX[cpos+0],YY[cpos+0]); cprintf("             ");
	 gotoxy(XX[actcpos],YY[actcpos]);
	 if(actcpos==0)              Vy=C;
	 else
	 if((actcpos/2)*2==actcpos) Vy=temp[1][(actcpos-2)/2]; /*houdige inhoud cel*/
		  else         Vy=temp[0][(actcpos-0)/2];

	 if(Vy!=MISSING)
		{
		 if(((actcpos+1)/2)*2==actcpos+1 && YN=='Y' && actcpos!=0)//als const dilution
			{
			cprintf("%8.4g\r\n",Vy);
			actcpos++;
			continue;
			}
		gotoxy(XX[actcpos],YY[actcpos]);
		textcolor(YELLOW);  textbackground(BLACK);
		gotoxy(XX[actcpos],YY[actcpos]);
		cprintf("%8.4g\r\n",Vy);
		gotoxy(XX[actcpos],YY[actcpos]);
		}
 else {
		gotoxy(XX[actcpos],YY[actcpos]);
		textcolor(YELLOW);  textbackground(BLACK);
		gotoxy(XX[actcpos],YY[actcpos]);
		cprintf("        ");
		gotoxy(XX[actcpos],YY[actcpos]);
		}
	 _setcursortype(_NORMALCURSOR);

	 if(toets==ESC) break;
	 toets = getch();
	 if(toets == SPECIAL_KEY)
	  {
		toets = getch();
		switch (toets)
		{
		case UP: 	gotoxy(XX[actcpos],YY[actcpos]);
						textcolor(BLACK);   textbackground(WHITE);
						cprintf("        ");
						actcpos -= 2;      break;
		case LEFT:  --actcpos;           break;
		case DOWN:    actcpos += 2;      break;
		case RIGHT: ++actcpos;           break;
		case HOME:    actcpos = 0;       break;
		case ENDT:    actcpos = j - 1;   break;
		case DEL:   if(geladen >1)
							 {
							 sound(2200);
							 delay(13);
							 nosound();
							 break;
							 }
						i=actcpos/2-1;   /*wissen ijklijnpaar*/
						if(i<0) break;
						for(n=i;n<50;n++)
							{
							temp[0][n]=temp[0][n+1];
							temp[1][n]=temp[1][n+1];
							ykpunten[n]=ykpunten[n+1];
							}
						textcolor(BLACK);   textbackground(WHITE);
						clrscr();
						j-=2;
						break;
						}
	 if(toets==DEL)         continue;
	 if(actcpos < 0)        actcpos = 0;
	 if(actcpos > j - 2)    actcpos--;
	 if(actcpos > j - 1)
		{
		j++;
		gotoxy(XX[actcpos],YY[actcpos]);
		textcolor(YELLOW);  textbackground(BLACK);
		cprintf("        ");
		gotoxy(XX[actcpos],YY[actcpos]);
		ch=getch();
		if(ch==ESC) {toets=ESC; continue;}
		if(ch>45 && ch <58)
			{
			ungetch(ch);
			Vx=atof(cgets(Invoer));
			textcolor(BLACK);   textbackground(WHITE);
			i= (actcpos-1)/2;
			if((actcpos/2)*2==actcpos)	{ 	temp[1][i] = Vx; ykpunten[i]=TRUE;}
			else									temp[0][i] = Vx;
			actcpos++;
			}
		}
		else if(toets==LEFT)         continue;
		else if(toets==RIGHT)        continue;
	  }
	 else
		{
		 ch = toets;
		 if(ch==ESC) continue;
		 if(ch==RETURN)
			{
			if(actcpos > j - 1)    j++;
			if(actcpos==0)
				{
				if(C == MISSING)     {C=0; actcpos++; continue; }
				}
			if(actcpos == j-1) {toets=ESC; break;}
			continue;
			}
		if(ch>45 && ch <58) ungetch(ch);
		gotoxy(XX[actcpos],YY[actcpos]);
		textcolor(YELLOW);  textbackground(BLACK);
		cprintf("        ");
		gotoxy(XX[actcpos],YY[actcpos]);
		ch=getch();
		if(ch==ESC) {toets=ESC; continue;}
		if(ch>45 && ch <58)
			{
			ungetch(ch);
			Vx=atof(cgets(Invoer));
			if(Invoer[3]==13) {toets=ESC; continue;}
			textcolor(BLACK);   textbackground(WHITE);

			if(actcpos==0) {  C = Vx; actcpos++;}
			else
				{ 	i= (actcpos-1)/2;
					if((actcpos/2)*2==actcpos)
						{ temp[1][i] = Vx; ykpunten[i]=TRUE;}
					else temp[0][i] = Vx;

					actcpos++;
					if(actcpos < 0)        actcpos = 0;
					if(actcpos > j - 1)    j++;

				}
			}
		}
 }
 _setcursortype(_NORMALCURSOR);
  rows=0;
  for(n=0;n<50;n++)
  {
  if(temp[0][n]<0 || temp[1][n]<0) ykpunten[n]=0;
  if(ykpunten[n]) rows=n+1;
  }
 if(rows<3) return; /* Te weinig datapunten */
 for(i=0;i<=rows;i++)
	{
	 data[0][i]=temp[0][i];
	 data[1][i]=temp[1][i];
	}
 if(Correction) rows=Rows;
}
//-------------------------------------------------------------------------
void   Manual_berekenen(void)
{
int    k;
double invoer,nop,resultt,dil,Dill,endresult;
char   ch,YesNo,Invoer[80];

k=1;
textcolor(WHITE);   textbackground(BLACK);
clrscr();
while(TRUE)
 {
 Dill=MISSING;
 printf("\r\n\r\r Constant dilution (Y/N), InputNo?(B) or ESC to quit? ");
 YesNo = toupper(getche());
 Invoer[0]=19;
 dil=0;

 if(YesNo=='J' || YesNo=='Y')
	{
	  YesNo='Y';
	  Dil:
	  printf("\r\n Which dilution ? : ");
	  Dill=atof(cgets(Invoer));
	  puts("\r\n");
	  if(Invoer[2]==27) return;
	  if(Dill==0) goto Dil;
	}
	else if(YesNo==27) return;
	else if(YesNo=='B')
	 {
	  printf("\r\n Which input number ? : ");
	  k=atoi(cgets(Invoer));
	  puts("\r\n");
	 }
	else Dill=MISSING;
		 printf("\r\n                     Meas     result      dil     endresult\n");
 if(print)
  fprintf(printer,"\r\n	            Meas     result      dil     endresult\n");

	printf("\r\n\r\n");
	while(Invoer[2]!=27)
	{
	dil=Dill;
	if(dil==MISSING)
		{
		 printf(" %d   Dilution ? : ",k);
		 ch = getch();
		 if(ch==ESC) break;
		 if(ch>43 && ch <58 || ch==13)
	{
	 ungetch(ch);
	 dil=atof(cgets(Invoer));
	}
		}

	if(Invoer[2]==27) break;
	printf("  Meas.? : ");
	ch = getch();
	if(ch==ESC) break;
	if(ch>43 && ch <58 || ch==13)
	 {
	  ungetch(ch);
	  invoer=atof(cgets(Invoer));
	  nop = logit(invoer);
	  resultt=exp((nop-intercept)/slope);
	  if (dil > 0) {         endresult = resultt * dil; }
	 else   { dil = 1;  endresult = resultt; }

	  printf("\r%4d",k);
	  if(nop==MISSING)
		cprintf("     .    %12.3f    << >>  %9.1f       << >>\r\n",invoer,dil);
	  else
		cprintf("     .    %12.3f %9.3f %9.1f %12.3f \r\n",invoer,resultt,dil,endresult);
	  if(print)
	  {
	  fprintf(printer,"%4d",k);
	  if(nop==MISSING)
		 fprintf(printer,"     .    %12.3f    << >>  %9.1f       << >>\r\n",invoer,dil);
	  else
		 fprintf(printer,"     .    %12.3f %9.3f %9.1f %12.3f\r\n",invoer,resultt,dil,endresult);
	  }
	  fprintf(fpout,"%4d",k);
	  if(nop==MISSING)
		 fprintf(fpout,"   -1       %12.3f   -9999   %9.1f      -9999\r\n",invoer,dil);
	  else
		 fprintf(fpout,"   -1       %12.3f %9.3f %9.1f %12.3f\r\n",invoer,resultt,dil,endresult);
	  k++;
	 }
	 else break;
	}
 Manualrows=k;
 }
}



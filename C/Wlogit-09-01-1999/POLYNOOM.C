#include "wlogit.h"

int Polynoom(HWND hwnd,int i, int j);
void sub1(void);
void PolyCorrelatie(void);
double Poly(double y);

int polynoomgraad=3;
double coeifpolynoom[MAX_POLYNOOM+2];

double polycoeif[500];
int A,B,Cc,F;
double Dd,G,H,I;

int Comp(double getal)
{
 int iremainder,iint;
 iint=(int)getal;
 iremainder=(int)((getal-(double)iint)*1001);
 if(iint<iremainder) return -1;
 if(iint==iremainder) return 0;
 return 1;
}


int Polynoom(HWND hwnd,int i, int j)
{
register int k,m,n;
register double Vx,vy;
double   somxx,somxy;

if(polynoomgraad >9)
  {
	  MessageBox (hwnd, "Too high degree", "Error",MB_OK | MB_ICONSTOP);
	  return FALSE;
  }
if(!berekend)
{
 Entry:
 berekend=FALSE;
 maxregel=0;
 lastregel=1; //nodig bij uitrekenen om tellers te resetten
 aantal=somx=somy=somxy=somxx=0;
  for(k=0;k<=rows_temp;k++)
  {
  if(ykpunten[k]==TRUE)
	 {
		 vy=temp[1][k];
		 Vx=temp[0][k];
		 somx=Vx; somy+=vy; somxy+=Vx*vy; somxx+=Vx*Vx;
		 aantal++;
		}
  }
 if(aantal<=polynoomgraad)
  {
  MessageBox (hwnd, "Too less datapoints for polynoom", "Error",MB_OK | MB_ICONSTOP);
  for(k=0;k<rows_temp;k++)ykpunten[k]=TRUE;
  fill_temp(hwnd,i,j);
  goto Entry;
  }
 for(n=0;n<= polynoomgraad * polynoomgraad + 3 * polynoomgraad + 10;n++)	polycoeif[n]=0;
 polycoeif[4]=polynoomgraad;
 polycoeif[5]=polynoomgraad+1;
 for(i=0;i<=rows_temp;i++)
  {
  if(ykpunten[i]==TRUE)
	 {
		polycoeif[1]=temp[0][i];
		polycoeif[0]=temp[1][i];
	 }
  else continue;
	polycoeif[2]=polycoeif[4]/1000.0;
	polycoeif[8]=aantal;
 Entry1:
	B=(int)polycoeif[2];
	Cc=(int)polycoeif[5];
	sub1();
	if((int)polycoeif[2]==0) polycoeif[3]=1;
	else
	polycoeif[3]=  pow(polycoeif[0],(int)polycoeif[2]);
	polycoeif[(int)polycoeif[7]] += polycoeif[1] * polycoeif[3];
	polycoeif[7] -= polycoeif[5];
	polycoeif[(int)polycoeif[7]] += polycoeif[3];
	polycoeif[6] = polycoeif[4];
 Entry2:
	polycoeif[7] ++;
	polycoeif[3] *= polycoeif[0];
	polycoeif[(int)polycoeif[7]] += polycoeif[3];
	polycoeif[6] --;
	if(Comp(polycoeif[6])!=0) goto Entry2;
//	if((int)polycoeif[6] != (int)(1010.0 * (polycoeif[6] - (int)polycoeif[6] ))) goto Entry2;
	polycoeif[2] += 1;
	if(Comp(polycoeif[2])!=1) goto Entry1;
//	if((int)polycoeif[2] <= (int)(1010.0 * (polycoeif[2] - (int)polycoeif[2] ))) goto Entry1;
  }

  polycoeif[8] = aantal;
  polycoeif[1] = polycoeif[4] / 1000.0;
  polycoeif[6] = polycoeif[1];
 Entry7:
  B = Cc = polycoeif[1];
  polycoeif[2] = polycoeif[5] - (double)B;
  sub1();
  polycoeif[0] = polycoeif[(int)polycoeif[7]];
  polycoeif[(int)polycoeif[7]] = 1.0;

 Entry3:
  polycoeif[7]++;
  polycoeif[(int)polycoeif[7]] /= polycoeif[0];
  polycoeif[2] --;
	if(Comp(polycoeif[2])!=0) goto Entry3;
//  if((int)polycoeif[2] != (int)(1010.0 * (polycoeif[2] - (int)polycoeif[2] ))) goto Entry3;
  polycoeif[2] = polycoeif[6];
 Entry4:
  if( polycoeif[1] == polycoeif[2]) goto Entry6;
  B = (int)polycoeif[2];
  Cc = (int)polycoeif[1];
  sub1();
  polycoeif[0] = polycoeif[(int)polycoeif[7]];
  polycoeif[3] = polycoeif[5] / 1000.0 + (double) Cc;
 Entry5:
  B = (int)polycoeif[1];
  Cc = (int)polycoeif[3];
  sub1();
  Dd = polycoeif[(int)polycoeif[7]];
  B = (int)polycoeif[2];
  Cc = (int)polycoeif[3];
  sub1();
  polycoeif[(int)polycoeif[7]] -= polycoeif[0] * Dd;
  polycoeif[3] ++;
	if(Comp(polycoeif[3])!=1) goto Entry5;
//  if((int)polycoeif[3] <= (int)(1010.0 * (polycoeif[3] - (int)polycoeif[3] ))) goto Entry5;
 Entry6:
  polycoeif[2] ++;
  if(Comp(polycoeif[2])!=1) goto Entry4;
//  if((int)polycoeif[2] <= (int)(1010.0 * (polycoeif[2] - (int)polycoeif[2] ))) goto Entry4;
  polycoeif[1] ++;
if(Comp(polycoeif[1])!=1) goto Entry7;
//  if((int)polycoeif[1] <= (int)(1010.0 * (polycoeif[1] - (int)polycoeif[1] ))) goto Entry7;
  F = (int)polycoeif[4];
  G = (int)((polycoeif[4] + 3.0) * polycoeif[4] + 9.0);
  H = (int)(polycoeif[4] + 9.0);
  I = (int)(polycoeif[4] + 2.0);
  m = F;
  for(n = G;n>=H;n-=I)  polycoeif[m--] = polycoeif[n];
 }
else return FALSE;
 berekend=TRUE;
 PolyCorrelatie();

 return TRUE;
}

void sub1(void)
{
 polycoeif[7]= polycoeif[5] * B + B + Cc + 8;
 return;
}

void PolyCorrelatie(void)
{
 double s,var,x1,x2,X,Y;
 int k;

 s=x1=x2=0;
 var=1E-10;
 for(k=0;k<=rows_temp;k++)
  {
  if(ykpunten[k]==TRUE)
	 {
		 X=temp[0][k];
		 Y=temp[1][k];
		 x1+=X;
		 x2=Poly(Y);
		 s+=(X-x2) * (X-x2);
		 var+= (X-x1/aantal) * (X-x1/aantal);
		}
  }
  corr=sqrt((var-s) / var);
 return;
}

double Poly(double y)
{
  return poly(y,polynoomgraad,polycoeif);
}


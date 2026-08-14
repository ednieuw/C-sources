 /*     corrcalc  */

#include "corr.h"

#define MISSING -999999.0
#define LAMBDA 1
#define AS_AANTAL 23   /*aantal aslabelingen */

void fill_temp(int i, int j, int z);
void teken_graph(void);
void print_labels(void);

extern void teken3dgraph(void);

extern unsigned long MAX_ROW;
extern unsigned long MAX_COL;
//extern int   lijn,lijnxy,conf,THREE_D;
//extern int   print, show, num,spearm,lost;
extern float upx,upy,upz;
extern float maxttest;
extern char  outfile[40];
extern char  file[20];
extern float **data;
extern char  **label;
extern int   *SCHERMPTR;
extern float **PP;

extern float **temp;
int 	 xx,yy,zz;
int    x_as,y_as,z_as;
int    rows, columns,rows_temp,labels;
double facx,facy;

//---------------------------------------------------------------
 void fill_temp(int i, int j, int z)
{
 int k;
 float vx=0,vy=0,vz=0,vc=0;

 rows_temp=0;
 for(k=0;k<rows;k++)
	{
	 vx = data[k][i];
	 vy = data[k][j];
	 vz = data[k][z];
	 vc = data[k][0];
	  if (vx >= 0 && vy >= 0 && vz >= 0)
		{
	  if (vx<=upx && vy<=upy && vz<=upz)
		 {
	 temp[0][rows_temp] = vx;
	 temp[1][rows_temp] = vy;
	 temp[2][rows_temp] = vz;
	 temp[3][rows_temp] = vc;
	 ++rows_temp;
	}
		 }
		}

}

//---------------------------------------------------------------
/*************** teken plaatje ***************/

void teken_graph(void)
 {
// int x_as,y_as,z_as;
 char nop[12];

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

  clrscr();
  while(kbhit()) getch();
  xx=yy=0;
  strcpy(outfile,"0");
  print_labels();
  printf("Welke rij op de X-as? ");
  gets(nop);
  xx=atoi(nop);
  if (xx >columns || xx<1)  return;
  printf("Welke rij op de Y-as? ");
  gets(nop);
  yy=atoi(nop);
  if (yy>columns || yy<1) return;
  printf("Welke rij op de Z-as? ");
  gets(nop);
  zz=atoi(nop);
  if (zz >columns || zz<1)  return;
  x_as=xx;
  y_as=yy;
  z_as=zz;
  x_as--;
  y_as--;
  z_as--;
  fill_temp(xx-1,yy-1,zz-1);
  teken3dgraph();
  return;
}


void print_labels(void)
{
 int n,colx=0,rowy=5;
 char text[MAX_LEN];
 for(n=0;n<labels;n++)
   {
	itoa(n+1,text,10);
	strcat(text,"    ");
	text[3]=0;
	strcat(text,&label[n][1]);
	text[15]=0;
	swrite(colx,rowy,text,0x1700);
	if(++rowy > 24)  { colx +=16; rowy=5;}
	if(colx>66) break;
   }
}

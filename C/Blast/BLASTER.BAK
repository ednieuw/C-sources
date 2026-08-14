// BLAST: print blokkerende antistoffen-uitslag+bar-graph vanuit LOTUS naar printer
// ed nieuwenhuys feb 1996
// blasto.c
#include "blast.h"
#include "blasto.h"
HDC hdc;
extern FILE *fpin,*fpout;


void 	clear_arrays(void);
void 	print_letter(HDC hdc);
void 	skip(FILE *fp,long noofbytes);
double 	getd(FILE *fp);
int  	read_wks(HWND hwnd, char *filename);
void 	assign_number(int row, int column, float value, int formattype);
void 	kyo62tabel(HDC hdc,int tabelcolumn, int tabelrow, int xtab1, int xtab2,
			int xtab3, int xtab4, int xtab5, int xtab6, int ytab1, int ytab2);
void 	error(int n, long count);
void 	farewell(void);
char 	*strleft(char *buffer, char *s, int n);
int 	strpos(char *s,char c,int pos);
void 	makedate(int *m,int *d,int *y,unsigned total);
void 	maketime(int *h,int *m,int *s,unsigned total);
int 	isvoid(char *s);
void 	kyoy_axis_title(HDC hdc,char *y_axis_string, int y_pos);
char 	*clear_string(char *s);
void 	kyomove(HDC hdc,int x, int y);
void 	kyodraw(HDC hdc,int x, int y);
void 	kyobar(HDC hdc,int filltype, int x0, int y0, int x1, int y1);
void 	kyofill(HDC hdc,int t, int x0, int y0, int lx, int ly);
char 	*datestring(char *buffer, float date);
int 	yearnumber(float datenumber);
float 	datenumber(int jaar, int maand, int dag);
int 	isdate(int formatbyte);
int 	kyoy_cal(HDC hdc,int logflag);
int 	ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag);


int 	Ycal=0;
extern 	char calibratorstring[MAXNOOFCALIBRATORS][16];
extern 	double value_of_calibrator[MAXNOOFCALIBRATORS];
int 	noofcalibrators=MAXNOOFCALIBRATORS;
int 	Logflag, Kader, Barsize, Fixed_distance, Fflag;
char 	y_legend[MAXSTRLEN+1]="Antistof-eenheden (U/ml)";//in header blast.h
int 	tabelfont=1;
int 	tabelcolumn=200;
int 	tabelrow=2700;
int 	totalfiles, nooffiles, nop;
int 	startup_drive, fileno, Startrow, Startcolumn;//, rows, columns;
int 	x, y, result, Header, Show, Trace, Kyo=1, Group, Lotus;
float 	X_factor=X_FACTOR, Y_factor=Y_FACTOR;
int 	Paperlength=PAPERLENGTH, Pen=PEN, Font=FONT1, Fillpen=FILLPEN;
int 	Lettershift=LETTERSHIFT;
int 	n, count, Hsize=24, Vsize=32;
FILE  	*tracefile;
char 	volumelabel[20]="no disc name";
char 	shortfilename[13];
char 	buffer[81],printbuffer[81];
long 	currentfilesize;
int 	x_origin, x_length, y_origin, y_length;
int 	linefont[MAXTEXTROWS], linerow[MAXTEXTROWS], linecolumn[MAXTEXTROWS];
float 	xdata[MAXDATAROWS];
float 	ydata[MAXDATAROWS][MAXDATACOLUMNS];
float 	xmin, xmax, ymin=0.01, ymax=10000;
char 	textline[MAXTEXTROWS][MAXLEN];
char 	default_text[MAXTEXTROWS][MAXLEN];
char 	volgnr[MAXDATAROWS][10];
double 	xfactor, yfactor;

void clear_arrays(void) {
	int i, j;
	x_origin=y_origin=100;
	x_length=y_length=2000;
	for (i=0; i<MAXTEXTROWS; i++) {
	 linefont[i]=10;
	 linerow[i]=100*i;
	 linecolumn[i]=50;
	 textline[i][0]='\0';
	}
	for (i=0; i<MAXDATAROWS; i++) {
	 xdata[i]=MISSING;
	 for (j=0; j<MAXDATACOLUMNS; j++) ydata[i][j]=MISSING;
	}
	xmin=32000.0;
	xmax=33000.0;
	ymin=0.1;
	ymax=100;
}

void print_letter(HDC hdc) {
	int i, x, y, rim=RIM, datarows=0, year, yrs, textpos;
	double d, value, inter, min;
	double IgG, IgG4, ratio, ff;    /* ff=foezelfactor: (IgG in tabel)/(aanvankelijk bepaalde IgG) */
	float lettersize;
	int xtab1=220; // kolom breedte uitslagen
	int xtab2=300;
	int xtab3=320;
	int xtab4=300;
	int xtab5=400;
	int xtab6=500;
	int ytab1=140;
	int ytab2;
	x_length-=x_origin;
	if (!Fixed_distance) {	 y_origin-=1.5*Vsize;   }
	y_length=y_origin-y_length;
	DrawGraphScreen=TRUE;

	/*
	if (Logflag && ymin>0 && ymax>0) {
	  ymin=log(ymin);
	  ymax=log(ymax);
	}
	yfactor=(double)y_length/(ymax-ymin);
*/
	for (i=datarows=0; i<MAXDATAROWS; i++) if (xdata[i]!=MISSING) datarows++;
	for (i=0; i<MAXTEXTROWS; i++) {
	 if(*default_text[i]) {
//	 ChangeFont(hdc,linefont[i]);
	 switch(linefont[i])
	 {
	  case 1:	lettersize = 1.0; break;
	  case 3:	lettersize = 0.75; break;
	  case 10:	lettersize = 1.5; break;
	  default:	lettersize = 1.0; break;
	 }

	 Print(hdc,(double)(linecolumn[i]),(double)(linerow[i]),lettersize,0,default_text[i]);
	 //	fprintf(fpout, "FONT %d;\r\nMAP %d,%d;\r\nTEXT'%s';\r\n",
	 //	   linefont[i], linecolumn[i], linerow[i], default_text[i]);
		if (*textline[i])
	 Print(hdc,(double)(linecolumn[i]+450),(double)(linerow[i]),1,0,textline[i]);
//		 fprintf(fpout, "MAP %d,%d;\r\nTEXT': %s';\r\n",
//			 linecolumn[i]+450, linerow[i], textline[i]);
	 }
	}
	Piccolor(hdc,BLACK,Pen);
	  Piccolor(hdc,BLACK,1);
//   fprintf(fpout, "SPD %d;\r\n", Pen);
	min=ymin;
	Logflag = kyoy_cal(hdc,Logflag);
	if (Logflag && ymin>0 && ymax>0) {
	  ymin=log(ymin);
	  ymax=log(ymax);
	  Piccolor(hdc,BLACK,1);
//	  fprintf(fpout, "SPD 1;\r\n");
	  for (value=0.01; value<=10000; value*=10) {
		d=log(value);
		if (d>=ymin && d<=ymax) {
		  kyomove(hdc,x_origin-RIM,y_origin-(d-ymin)*yfactor);
		  kyodraw(hdc,x_origin+x_length+RIM, y_origin-(d-ymin)*yfactor);
		}
	  }
	  Piccolor(hdc,BLACK,Pen);
	  Piccolor(hdc,BLACK,1);
//	  fprintf(fpout, "SPD %d;\r\n", Pen);
	}
	if (Fixed_distance) {
	  x_length-=2*rim;
	  x_origin+=rim;
   }
   xfactor=(xmax>xmin)?(double)x_length/(xmax-xmin):1;
   inter=(datarows)?(double)x_length/(datarows-1):0;
   ff=1.0;
   for (i=MAXDATAROWS-1; i>=0; i--) {
	 if (xdata[i]==MISSING) continue;
	 IgG= (ydata[i][3]==MISSING)? ydata[i][1]:ydata[i][3]*ff;
	 IgG4=(ydata[i][2]==MISSING)? ydata[i][0]:ydata[i][2];
	 ratio=(ydata[i][3]==MISSING)? IgG4/ydata[i][1]:IgG4/ydata[i][3];
	 IgG4=ratio*IgG;
/*
	 if (ydata[i][1]!=MISSING && ydata[i][3]!=MISSING && Fflag) IgG4*=IgG/ydata[i][3];
*/
	 ff=(ydata[i][1]==MISSING || !Fflag)?1.0:IgG/ydata[i][1];

	 //	 sprintf(printbuffer,"     %2d       %9s     %9s", i+1,datestring(buffer,xdata[i]), volgnr[i]);
	//     ChangeFont(hdc,1);
//	 Print(hdc,(double)tabelcolumn,(double)(tabelrow+i*70+ytab1+60),1,0,printbuffer);
	 sprintf(printbuffer,"%2d", i+1);
	//     ChangeFont(hdc,1);
	 Print(hdc,(double)tabelcolumn+50,(double)(tabelrow+i*70+ytab1+60),1,0,printbuffer);
	 sprintf(printbuffer,"%9s",datestring(buffer,xdata[i]));
	 Print(hdc,(double)tabelcolumn+xtab1+20,(double)(tabelrow+i*70+ytab1+60),1,0,printbuffer);
	 sprintf(printbuffer,"%9s",volgnr[i]);
	 Print(hdc,(double)tabelcolumn+xtab1+xtab2+20,(double)(tabelrow+i*70+ytab1+60),1,0,printbuffer);

//	 fprintf(fpout,"FONT 1;\r\nMAP %d,%d;\r\nTEXT'   %2d   %9s   %9s';\r\n",
//		tabelcolumn, tabelrow+i*70+ytab1+60, i+1, datestring(buffer,xdata[i]), volgnr[i]);
	 if (IgG4>=.99*min) sprintf(printbuffer, "%.0f",IgG4);
	 else if (IgG4==MISSING) sprintf(printbuffer, "  ---");
	 else sprintf(printbuffer, "< %.0f",min);
	//     ChangeFont(hdc,1);
	 Printr(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+150),
				(double)(tabelrow+i*70+ytab1+60),  //60>70
				1,0,printbuffer);
//	 fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%6s';\r\n",
//		tabelcolumn+xtab1+xtab2+xtab3, tabelrow+i*70+ytab1+60, buffer);
	 if (IgG>=.99*min) sprintf(printbuffer, "%.0f",IgG);
	 else if (IgG==MISSING) sprintf(printbuffer, "  ---");
	 else sprintf(printbuffer, "< %.0f",min);
//     ChangeFont(hdc,1);
	 Printr(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+150),
					(double)(tabelrow+i*70+ytab1+60),     //60>70
					1,0,printbuffer);

	 //fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%7s';\r\n",
	 //	tabelcolumn+xtab1+xtab2+xtab3+xtab4, tabelrow+i*70+ytab1+60, buffer);
	 if (IgG>RATIO_LIMIET && IgG4!=MISSING) {
		if (IgG4>=.99*min) sprintf(printbuffer, "%6.0f",100*IgG4/IgG);
		else sprintf(printbuffer,"< %4.0f",100*min/IgG);
	 } else sprintf(printbuffer, "  ---");
//     ChangeFont(hdc,1);
	 Printr(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+150),
					(double)(tabelrow+i*70+ytab1+60),
					1,0,printbuffer);
//	 fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'%7s';\r\n",
//		tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+60, tabelrow+i*70+ytab1+60, buffer);
//	 cprintf("\r\n%d %10s %12.3f %12.3f %12.3f", i+1, datestring(buffer,xdata[i]), IgG4, IgG, ff);
	 x=(Fixed_distance)?
		  (x_origin+(
			 (inter)?
				i*inter:
				x_length/2)):
		  (x_origin+(int)((xdata[i]-xmin)*xfactor));
	 if (IgG4>MISSING) {
		if (IgG4<0.99*min) y=y_origin+20;
		else if (Logflag) {
		   if (IgG4 >0) y=y_origin-(log(IgG4)-ymin)*yfactor;
		   else y=y_origin+20;
		} else y=y_origin-(IgG4-ymin)*yfactor;
	 } else continue;
	 kyobar(hdc,6, x-((Fixed_distance)?Barsize:4*Barsize/10), y_origin, x, y);
	 y=y_origin-(IgG-ymin)*yfactor;
	 if (IgG>MISSING) {
		if (IgG<0.99*min) y=y_origin+20;
		else if (Logflag) {
			if (IgG >0) y=y_origin-(log(IgG)-ymin)*yfactor;
			else y=y_origin+20;
		} else y=y_origin-(IgG-ymin)*yfactor;
	 } else continue;
	 kyobar(hdc,8, x, y_origin, x+((Fixed_distance)?Barsize:4*Barsize/10), y);
	 sprintf(printbuffer,"%d",i+1);
//     ChangeFont(hdc,1);
	 Print(hdc,	(double)(x-14),	(double)(y_origin+70), 1.5,0,printbuffer); // bar nummer
//	 fprintf(fpout, "FONT %d;\r\nMAP %d,%d;\r\nTEXT'%d';\r\n",10, x-14, y_origin+80, i+1);
   }
   if (!Fixed_distance) {
	  yrs=yearnumber(xmax)-yearnumber(xmin);
	   Piccolor(hdc,BLACK,2);
//	  Changefont(hdc,yrs>6?15:10);
	  Move(hdc,x_origin-rim/2, y_origin+200);
	  Draw(hdc,x_origin+x_length+3*rim/4, y_origin+200);
//	  fprintf(fpout, "SPD 2;\r\nFONT %d;\r\nMAP %d,%d;\r\nDAP %d,%d;\r\n",
//		 yrs>6?15:10, x_origin-rim/2, y_origin+200, x_origin+x_length+3*rim/4, y_origin+200);
	  year=yearnumber(xmin);
	  do {
		 x=(x_origin+(int)((datenumber(year,1,1)-xmin)*xfactor));
		 textpos=x+(datenumber(year+1,1,1)-datenumber(year,1,1))*xfactor/2;
		 if (yrs>6) textpos+=20;
		 if (x>x_origin-rim) {
			if (textpos<=x_origin+x_length+rim) {
				kyomove(hdc,x, y_origin + 160);
				kyodraw(hdc,x, y_origin + 200);
				sprintf(printbuffer,"%d",1900+year);   //print jaren bij x-as

				Printc(hdc,(double)(textpos),(double)(y_origin+140),yrs>6?1.0:1.5,0, printbuffer);
//				Print(0,CENTERTOP, textpos, y_origin+120, buffer);
			}
		 }
		 year++;
	  } while (x<x_origin+x_length);
	  Piccolor(hdc,BLACK,Pen);
//	  fprintf(fpout, "SPD %d;\r\n", Pen);
   }
   ytab2=datarows*70+40;
   if (Kader)
	kyo62tabel(hdc,tabelcolumn, tabelrow, xtab1, xtab2, xtab3, xtab4, xtab5, xtab6, ytab1, ytab2);
   y=tabelrow+ytab1-50;
   sprintf(printbuffer, "< %.0f",min);
//     ChangeFont(hdc,1);
	 Print(hdc,	(double)(tabelcolumn+20),
				(double)(y),
				1,0,"volg-nr");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'volg-nr';\r\n",
//	  tabelcolumn+20, y);

	 Print(hdc,	(double)(tabelcolumn+xtab1+20),
				(double)(y),
				1,0,"datum");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'  datum';\r\n",
//	  tabelcolumn+xtab1+20, y);
	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+20),
				(double)(y),
				1,0,"CLB-nr");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'  CLB-nr';\r\n",
//	  tabelcolumn+xtab1+xtab2+20, y);

	kyobar(hdc,6, tabelcolumn+xtab1+xtab2+xtab3+20,y-60,tabelcolumn+xtab1+xtab2+xtab3+130,y+20);

	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+150),
				(double)(y),
				1,0,"IgG4");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+150, y);

//	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+150+85),
//				(double)(y+20),
//				0.7,0,"4");
//   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'4';\r\n",
//	 tabelcolumn+xtab1+xtab2+xtab3+150+90, y+20);
	kyobar(hdc,8, tabelcolumn+xtab1+xtab2+xtab3+xtab4+20,y-60,tabelcolumn+xtab1+xtab2+xtab3+xtab4+130,y+20);

	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+140),
				(double)(y),
				1,0," IgG totaal");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+xtab4+140, y);
//	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+140+100),
//				(double)(y+20),
//				0.7,0,"totaal");
//   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'totaal';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+xtab4+140+100, y+20);

	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20),
				(double)(y),
				1,0,"IgG4 / IgG totaal (%)");
//   fprintf(fpout, "FONT 1;\r\nMAP %d,%d;\r\nTEXT'IgG /IgG    (%)';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20, y);
//	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+85),
//				(double)(y+20),
//				0.7,0,"4");
//   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'4';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+90, y+20);

//	 Print(hdc,	(double)(tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+240),
//				(double)(y+20),
//				0.7,0,"totaal");
//   fprintf(fpout, "FONT 13;\r\nMAP %d,%d;\r\nTEXT'totaal';\r\n",
//	  tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+20+250, y+20);
}

void skip(FILE *fp,long noofbytes)
{char c;
while (noofbytes>0)
   {
	 c=getc(fp);
	 noofbytes--;
	}
c=c;
}
double getd(FILE *fp)
{
 double a;
 char *b;
 int i,j;
  b=(char *) &a;
  for (i=0;i<sizeof(double);++i)
	{
	 if((j = getc(fp)) == EOF) return(-1.0);
	 b[i]= j & 0xFF;
	}
return(a);
}

int read_wks(HWND hwnd,char *filename) {
   int integer, op, formula_size, file_type, formatbyte,
	   column, row, end_col, end_row, max_len=MAX_LEN;
   long counter, body_length;
   long mark;
   double lotusnumber;
   unsigned char c, pos;
	char stringbuffer[MAX_LEN+1];
	char text[80];

	clear_arrays();
	if((fpin=fopen(filename,"rb"))==NULL)
	{
 	  sprintf(text,"Not a valid lotus filename: %s",filename);
	  MessageBeep(MB_ICONHAND);
	  MessageBox (hwnd, "Not a proper Lotus worksheet; filename",filename,MB_OK | MB_ICONQUESTION);
	  return(FALSE);
	}
	//   filesize=filelength(fileno(fpin));
	counter=0;
	mark=0;
	op=getw(fpin);
	while (op!=Eof)
	{
	mark++;
	*stringbuffer='\0';
	body_length=(long)getw(fpin);
	if (body_length < 1) {
		MessageBeep(MB_ICONHAND);
//		MessageBox (hwnd, "Not a proper Lotus worksheet bodylength<1",filename,MB_OK | MB_ICONQUESTION);
		MessageBox (hwnd, text,filename,MB_OK | MB_ICONQUESTION);
		return(FALSE);
		}
	  counter += body_length+4;
	  switch(op){
			case BOF:
			if (!Kyo) fprintf(fpout,"bytecount %5ld: BOF\n",counter-body_length-4);
			file_type=getw(fpin);
				if ( file_type!=LOTUS2) //file_type!=LOTUS1 ||
				{
					sprintf(text,"file-type: %4X ipv %4X of %4X: dus geen LOTUS-worksheet",file_type,LOTUS1,LOTUS2);
					MessageBeep(MB_ICONHAND);
//	  //				MessageBox (hwnd, "Not a proper Lotus worksheet3 Lotus1 or 2",filename,MB_OK | MB_ICONQUESTION);
					MessageBox (hwnd, text,filename,MB_OK | MB_ICONQUESTION);
					return(FALSE);

				}
				break;
			case DIMENSIONS:
				getw(fpin);          /* start_col */
			   getw(fpin);          /* start_row */
			   end_col=getw(fpin);
			   end_row=getw(fpin);
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: DIMENSIONS; end_row=%d end_col=%d\n",counter-body_length-4, end_row, end_col);
			   break;
			case INTEGER:
			   formatbyte=isdate(getc(fpin));      /* format */
			   column=getw(fpin);
			   row=getw(fpin);
			   integer=getw(fpin);
			   assign_number(row, column, (float)integer, formatbyte);
			   if (!Kyo) fprintf(fpout,
				  "bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%d\n",
				  counter-body_length-4, "INTEGER", formatbyte, row, column, integer);
			   break;
		   case NUMBER:
			   formatbyte=isdate(getc(fpin));      /* format */
			   column=getw(fpin);
               row=getw(fpin);
			   fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               assign_number(row, column, lotusnumber, formatbyte);
               if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%9.3lf\n",counter-body_length-4, "NUMBER", formatbyte, row, column, lotusnumber);
               break;
		   case FORMULA:
			   formatbyte=isdate(getc(fpin));      /* format */
               column=getw(fpin);
			   row=getw(fpin);
               fread(&lotusnumber,sizeof(lotusnumber),1,fpin);
               if (row==FIRSTTEXTROW && column==4) strcpy(textline[0],datestring(buffer,lotusnumber));
			   else assign_number(row, column, lotusnumber, formatbyte);
			   formula_size=getw(fpin);
			   skip(fpin,formula_size);
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d value=%9.3lf\n",counter-body_length-4, "FORMULA", formatbyte, row, column, lotusnumber);
			   break;
		   case BLANK:
			   formatbyte=isdate(getc(fpin));      /* format */
				column=getw(fpin);
			   row=getw(fpin);
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; row=%3d column=%3d\n",counter-body_length-4, "BLANK", row, column);
			   break;
		   case LABELCODE:
			   pos=0;
			   clear_string(stringbuffer);
			   formatbyte=isdate(getc(fpin));      /* format */
			   column=getw(fpin);
			   row=getw(fpin);
			   while ((c=getc(fpin))!=0 && pos<MAX_LEN) {
				 if (pos<max_len-1) {
					if (pos>0) stringbuffer[pos-1] = c;
					pos++;
				 }
			   }
			   stringbuffer[pos-1]='\0';
			   if (row==0 && column==0 && strcmp(stringbuffer,"BLAST 1")!=0) {
//				cprintf("Not a BLAST-1 worksheet\r\n"  "Press any key ");
				   farewell();
			   } else if (column==4 && row>=FIRSTTEXTROW && row<=LASTTEXTROW) strcpy(textline[row-FIRSTTEXTROW],stringbuffer);
			   else if (column==3 && row>=FIRSTTEXTROW && row<=LASTTEXTROW) strcpy(default_text[row-FIRSTTEXTROW],stringbuffer);
			   else if (row>=FIRSTDATAROW && row<=LASTDATAROW) {
				 if (column==1) {
					strncpy(volgnr[row-FIRSTDATAROW],stringbuffer,9);
					volgnr[row-FIRSTDATAROW][9]='\0';
				 }
				 else assign_number(row, column, ymin/2, formatbyte);
			   }
			   if (!Kyo) fprintf(fpout,"bytecount %5ld: %12s; format=%3d row=%3d column=%3d \"%s\"\n",counter-body_length-4, "LABEL", formatbyte, row, column, stringbuffer);
			   break;
		   default:
			   skip(fpin,body_length);
		 }
	  op=getw(fpin);
   }
   fclose(fpin);
   return(TRUE);
}

void assign_number(int row, int column, float value, int formattype){
   if (row==1) {
	  if (column==1) x_origin=(int)value;
	  else if (column==2) x_length=(int)(value+0.49);
	  else if (column==3) y_origin=(int)(value+0.49);
	  else if (column==4) y_length=(int)(value+0.49);
   } else if (row==2) {
	  if (column==1) xmin=value;
	  else if (column==2) xmax=value;
	  else if (column==3) ymin=value;
	  else if (column==4) ymax=value;
   } else if (row==FIRSTTEXTROW-1) {
	  if (column==0) tabelfont=(int)(value+0.49);
	  else if (column==1) tabelrow=(int)(value+0.49);
	  else if (column==2) tabelcolumn=(int)(value+0.49);
   } else if (row>=FIRSTTEXTROW && row<=LASTTEXTROW) {
	  if (column==0) linefont[row-FIRSTTEXTROW]=(int)(value+0.49);
	  else if (column==1) linerow[row-FIRSTTEXTROW]=(int)(value+0.49);
	  else if (column==2) linecolumn[row-FIRSTTEXTROW]=(int)(value+0.49);
	  else if (column==3) {
		 if (formattype==-1)     datestring(buffer,value);
		 else if (formattype>-1) sprintf(buffer,"%.*f",formattype, value);
		 else                    sprintf(buffer,"%.0f",value);
		 strcpy(default_text[row-FIRSTTEXTROW],buffer);
	  }
	  else if (column==4) {
		 if (formattype==-1)     datestring(buffer,value);
		 else if (formattype>-1) sprintf(buffer,"%.*f", formattype, value);
		 else                    sprintf(buffer,"%.0f",value);
		 strcpy(textline[row-FIRSTTEXTROW],buffer);
	  }
	} else if (row>=FIRSTDATAROW && row<=LASTDATAROW) {
	  if (column==0) xdata[row-FIRSTDATAROW]=value;
	  else if (column>=FIRSTDATACOLUMN && column<=LASTDATACOLUMN)
		 ydata[row-FIRSTDATAROW][column-FIRSTDATACOLUMN]=(value>=ymin)?value:ymin/2;
   }
}

void kyo62tabel(HDC hdc,
   int tabelcolumn,
   int tabelrow,
   int xtab1,
   int xtab2,
   int xtab3,
   int xtab4,
   int xtab5,
   int xtab6,
   int ytab1,
   int ytab2) {
   int x_length, y_length;
   x_length= xtab1+xtab2+xtab3+xtab4+xtab5+xtab6;
   y_length= ytab1+ytab2;
   kyomove(hdc,tabelcolumn,          tabelrow);
   kyodraw(hdc,tabelcolumn+x_length, tabelrow);
   kyomove(hdc,tabelcolumn,          tabelrow+ytab1);
   kyodraw(hdc,tabelcolumn+x_length, tabelrow+ytab1);
   kyomove(hdc,tabelcolumn,          tabelrow+ytab1+ytab2);
   kyodraw(hdc,tabelcolumn+x_length, tabelrow+ytab1+ytab2);
   kyomove(hdc,tabelcolumn,                                     tabelrow);
   kyodraw(hdc,tabelcolumn,                                     tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1,                               tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1,                               tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1+xtab2,                         tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1+xtab2,                         tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1+xtab2+xtab3,                   tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1+xtab2+xtab3,                   tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4,             tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4,             tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5,       tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5,       tabelrow+y_length);
   kyomove(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+xtab6, tabelrow);
   kyodraw(hdc,tabelcolumn+xtab1+xtab2+xtab3+xtab4+xtab5+xtab6, tabelrow+y_length);
}

// void error(int n, long count) {
//   clrscr();
//   if (!Kyo) printf("Write error %d (ferror()=%d) at bytecount %ld\ndisc full?", n, ferror(fpout), count);
//   else {
//	  printf("Write error %d (ferror()=%d) at bytecount %ld", n, ferror(fpout), count);
//	  printf("\nPress ESC to quit\nPress any other key to continue ");
//	  if (getch()==27) farewell();
//   }
//}

/*int get_options(void) {
   int n, option;
   for (;;) {
	  do {
		 clrscr();
		 printf("B - Bar dikte               : ");
			if (Fixed_distance) HLON;
			printf("%d",Barsize);
			if (Fixed_distance) HLOFF;
			putch(' ');
			if (!Fixed_distance) HLON;
			printf("(%d)\r\n", 4*Barsize/10);
			if (!Fixed_distance) HLOFF;
		 cprintf("C - Continuiteits-correctie : ");
		 yesno(Fflag,"YES","NO");
		 cprintf("F - Font                    : %d\r\n", Font);
		 cprintf("I - print file-Information  : ");
		 yesno(Header,"YES","NO");
		 cprintf("K - Kader                   : ");
		 yesno(Kader,"YES","NO");
		 cprintf("L - Log-transformatie       : ");
		 yesno(Logflag,"YES","NO");
		 cprintf("P - Pen dikte               : %d (fill)/%d (contour)\r\n", Fillpen, Pen);
		 cprintf("T - Tijds-afhankelijke x-as : ");
		 yesno(!Fixed_distance,"YES","NO");
		 cprintf("Y - Y-legende               : %s\r\n", y_legend);
		 cprintf("\r\nD - naar Disc? %s\r\n",
		  (Kyo)?"Nu gaan gegevens naar de KYOCERA i.p.v. naar een floppie":
				"Nu gaan gegevens naar een floppie i.p.v. naar de Kyocera");
		 cprintf("\r\nRETURN = accept options: ");
		 if ((option=getkey(0))==RETURN || option==EXEC) return 0;
		 if (option==ESC || option==CANCEL)             return -1;
	  } while (strpos("BCDFIKLPTY",option,0)==-1);
	  switch (option) {
		 case 'B':
			clrscr();
			cprintf("Bar size (1-1000) default:%d\r\n: ",Barsize);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=1000) Barsize=n;
			break;
		 case 'C':
			Fflag=(Fflag)?0:1;
			break;
		 case 'I':
			Header=(Header)?0:1;
			break;
		 case 'K':
			Kader=(Kader)?0:1;
			break;
		 case 'L':
			Logflag=(Logflag)?0:1;
			break;
		 case 'D':
			Kyo=(Kyo)?0:1;
			break;
		 case 'T':
			Fixed_distance=(Fixed_distance)?0:1;
			break;
		 case 'P':
			clrscr();
			cprintf("Fill Pen size (1-40) default:%d\r\n: ",Fillpen);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=200) Fillpen=n;
			cprintf("\r\n\r\nContour Pen size (1-40) default:%d\r\n: ",Pen);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=200) Pen=n;
			break;
		 case 'F':
			clrscr();
			cprintf("Fonts:\r\n\n\n"
				   " 7, 16  = very small  ( 7-point)\r\n"
				   "15      = small       ( 9-point)\r\n"
				   " 6      = Elite       (10-point)\r\n"
				   " 8      = Gothic      (12-point)\r\n"
				   " 9      = Gothic bold (12-point)\r\n"
				   " 1      = Courier     (12-point)\r\n"
				   "10      = Helve Bold  (14.4-point)\r\n\r\n"
				   " default:%d\r\n: ",Font);
			gets(buffer);
			n=atoi(buffer);
			if (n>0 && n<=16) Font=n;
			break;
		 case 'Y':
			clrscr();
			printf("\nLegend y-axis \"%s\": ",y_legend);
			strcpy(y_legend, keycopy(buffer, y_legend, MAX_LEN));
			break;
	  }
   }
}
*/

int dots(char c) {
 c=c;
//   if (Font!=10 || (c<32 || c>127)) return font[Font][2];
//   return font10[c-32];
return 1;
}


void farewell(void) {
   fclose(fpout);
   fclose(fpin);
   setdisk(startup_drive);
   exit(0);
}


char *strleft(char *buffer, char *s, int n) {
   int i;
   for (i=0; s[i] && i<n; i++) buffer[i]=s[i];
   buffer[i]='\0';
   return buffer;
}

int strpos(char *s,char c,int pos) {
/* finds next occurrence of c in s starting at pos */
int p;
	 for (p=pos+1;s[p];p++) if (s[p]==c) return p;
	 for (p=0;p<=pos;p++) if (s[p]==c) return p;
	 return -1;
}

void makedate(int *m,int *d,int *y,unsigned total) {
	*m=(total>>5)&0XF;
	*d=total & 0X1F;
	*y=(total>>9)&0X3F;
	*y+=80;
}

void maketime(int *h,int *m,int *s,unsigned total) {
	*h=(total>>11) & 0X1F;
	*m=(total>>5) & 0X3F;
	*s=total & 0X1F;
}

int isvoid(char *s) {
   int i;
   for (i=0;s[i];i++) if (!isspace(s[i])) return 0;
   return 1;
}

void kyoy_axis_title(HDC hdc,char *y_axis_string, int y_pos) {
   kyomove(hdc,40,y_pos);
   Printr(hdc,(double)(x_origin),(double)(y_pos),1.5,90, y_axis_string);
//   Print( 1, TOPRIGHT, x_origin, y_pos, y_axis_string);
}

char *clear_string(char *s) {
   *s=' ';
   *(s+1)='\0';
   return s;
}

int dots_per_text(char *text) {
/*   int pos, dotszin;
   for (pos=dotszin=0; pos<strlen(text); pos++)
	  dotszin+=
		 (Font!=10 || text[pos]<32 || text[pos]>127)?
			font[Font][2]:
			font10[text[pos]-32];
   return dotszin;
*/
text=text;
return 0;
}

void kyomove(HDC hdc,int x, int y) {
//   int w;
   Move(hdc,(double)x, (double)y);
//   w=fprintf(fpout,"MAP %d, %d;\r\n", x, y);
//   count+=4;
//   if (w==EOF || ferror(fpout)) error(w,count);
}

void kyodraw(HDC hdc,int x, int y) {
 //  int w;
   Draw(hdc,(double)x, (double)y);
//   w=fprintf(fpout,"DAP %d, %d;\r\n", x, y);
//   count+=4;
//   if (w==EOF || ferror(fpout)) error(w,count);
}

void kyobar(HDC hdc,int filltype, int x0, int y0, int x1, int y1) {
   int lx, ly;
   lx=x1-x0;
   ly=y1-y0;
   kyomove(hdc,x0, y0);
   kyodraw(hdc,x1, y0);
   kyodraw(hdc,x1, y1);
   kyodraw(hdc,x0, y1);
   kyodraw(hdc,x0, y0);
   Piccolor(hdc,BLACK,Fillpen);
//   fprintf(fpout,"SPD %d;\r\n",Fillpen);
   kyofill(hdc,filltype, x0, y0, lx, ly);
   Piccolor(hdc,BLACK,Pen);
//   fprintf(fpout,"SPD %d;\r\n",Pen);
}

void kyofill(HDC hdc,int t, int x0, int y0, int lx, int ly) {
   int d, dd, direction, dx1, dy1, dx2, dy2;
   if (lx<0) {
	  lx=-lx;
	  x0-=lx;
   }
   if (ly<0) {
	  ly=-ly;
	  y0-=ly;
   }
   if (t<=0 || t>8) return;
   if (t==2) {
	  kyofill(hdc,3, x0, y0, lx, ly);
	  kyofill(hdc,1, x0, y0, lx, ly);
      return;
   }
   if (t==4) {
	  for (d=VFILL1; d<lx; d+=VFILL1) {
         kyomove(hdc,x0+d,y0);
		 kyodraw(hdc,x0+d,y0+ly);
      }
      return;
   }
   if (t==8) {
      for (d=VFILL2; d<lx; d+=VFILL2) {
         kyomove(hdc,x0+d,y0);
         kyodraw(hdc,x0+d,y0+ly);
      }
      return;
   }
   if (t==7) {
	  kyofill(hdc,2, x0, y0, lx, ly);
	  kyofill(hdc,5, x0, y0, lx, ly);
      return;
   }
   if (t==3) {
	  dd=XFILL1;
      direction=1;
   }
   if (t==1) {
	  dd=XFILL1;
	  direction=-1;
      x0 += lx;
   }
   if (t==5) {
      dd=XFILL2;
      direction=1;
   }
   if (t==6) {
      dd=XFILL2;
      direction=-1;
	  x0 += lx;
   }
   for (d=dd; d<lx + ly; d+=dd) {
      dx1 = (d<lx) ?  direction*d : direction*lx;
      dx2 = (d<ly) ?  0           : direction*(d-ly);
      dy1 = (d<lx) ?  ly          : ly-(d-lx);
      dy2 = (d<ly) ?  ly-d        : 0; 
      kyomove(hdc,x0+dx1,y0+dy1);
      kyodraw(hdc,x0+dx2,y0+dy2);
   }
}

char *datestring(char *buffer, float date) {
   int jaar, maand, dag, found=FALSE;
   jaar= (int)(date/365.25);
   dag=date-jaar*365-(jaar-1)/4-1;
   while (!found) {
	  if (dag<32) {
		 maand=1;
		 found=TRUE;
		 break;
      }
      dag-=31;
	  if (dag<29+((jaar%4)==0)) {
         maand=2;
         found=TRUE;
		 break;
      }
      dag-=28+((jaar%4)==0);
      if (dag<32) {
		 maand=3;
		 found=TRUE;
		 break;
      }
	  dag-=31;
	  if (dag<31) {
		 maand=4;
         found=TRUE;
         break;
	  }
      dag-=30;
	  if (dag<32) {
         maand=5;
         found=TRUE;
         break;
	  }
      dag-=31;
	  if (dag<31) {
		 maand=6;
		 found=TRUE;
		 break;
      }
      dag-=30;
	  if (dag<32) {
         maand=7;
         found=TRUE;
		 break;
      }
      dag-=31;
      if (dag<32) {
		 maand=8;
		 found=TRUE;
		 break;
      }
      dag-=31;
	  if (dag<31) {
         maand=9;
		 found=TRUE;
         break;
	  }
	  dag-=30;
	  if (dag<32) {
         maand=10;
         found=TRUE;
         break;
      }
      dag-=31;
	  if (dag<31) {
		 maand=11;
		 found=TRUE;
		 break;
      }
      dag-=30;
      if (dag<32) {
         maand=12;
         found=TRUE;
		 break;
	  }
      maand=13;
	  found=TRUE;
   }
   sprintf(buffer,"%2d-%02d-%2d",dag, maand, jaar);
   return buffer;
}

int yearnumber(float datenumber) {
   return (int)(datenumber/365.25);
}

float datenumber(int jaar, int maand, int dag) {
   float date;
   date= dag + (float)jaar*365.0 + 1;
   if (jaar>3)  date+=(jaar-1)/4;
   if (maand>1) date+=31;                   /* feb */
   if (maand>2) {                           /* mrt */
	  if (jaar>0 && jaar%4==0) date+=29;
	  else date+=28;
   }
   if (maand>3) date+=31;                   /* apr */
   if (maand>4) date+=30;                   /* mei */
   if (maand>5) date+=31;                   /* jun */
   if (maand>6) date+=30;                   /* jul */
   if (maand>7) date+=31;                   /* aug */
   if (maand>8) date+=31;                   /* sep */
   if (maand>9) date+=30;                   /* okt */
   if (maand>10) date+=31;                  /* nov */
   if (maand>11) date+=30;                  /* dec */
   return date;
}



int isdate(int formatbyte) {
int hi, lo;
/* returns -1 if date, 0-15 for number of decimal places, -2 for other */
   hi=(formatbyte>>4)&7;
   lo=formatbyte&15;
/*
cprintf("\r\n%s %4d : shift4=%d formatbyte=%d hi=%d lo=%d",
__FILE__, __LINE__, formatbyte>>4, formatbyte, hi, lo);
if (getch()==27) farewell();
putch('\r');
putch('\n');
*/
   if (hi==7) {
	  if ( lo==2 ||
		   lo==3 ||
		   lo==4 ||
		  (lo >6 && lo<10)
	  ) return -1;
	  if (lo==1 || lo==15) return 0;
	  return -2;
   }
   return lo;
}

int kyoy_cal(HDC hdc,int logflag) {
   int n, ox, lx, pos, cal, logyflag, maxlen=0, mantissa, lowest_exponent, highest_exponent;
   double calibrator_value, max, min;
   kyoy_axis_title(hdc,y_legend, y_origin - y_length);
   x_origin+=Vsize+80;
   if (!Logflag) ymin=0;
   logyflag= ascal(&noofcalibrators, &ymin, &ymax, logflag, TRUE);
   min=(logyflag)?log(ymin):ymin;
   max=(logyflag)?log(ymax):ymax;
   yfactor=(double)y_length/(max - min);
   for (n=0, maxlen=0;n<noofcalibrators;n++)
	 if (maxlen<strlen(calibratorstring[n])) maxlen=strlen(calibratorstring[n]);
   x_origin+= 40 + maxlen * Hsize/2;
   x_length-= x_origin;
   if (x_length<=100) x_length=100;
   ox= x_origin;
   lx= x_length;
   if (Ycal/4 || logyflag) {                    /* bottom line */
	  Piccolor(hdc,BLACK,1);
//	  fprintf(fpout,"SPD 1;\r\n");
	  kyomove(hdc,ox,    y_origin);
	  kyodraw(hdc,ox+lx, y_origin);
	  Piccolor(hdc,BLACK,Pen);
//	  fprintf(fpout,"SPD %d;\r\n",Pen);
   }
   if (Ycal/4==2) {                 /* top line */
	  kyomove(hdc,ox,    y_origin+y_length);
	  kyodraw(hdc,ox+lx, y_origin+y_length);
   }
   if (Ycal>9) {                    /* right line */
	  kyomove(hdc,ox+lx, y_origin);
	  kyodraw(hdc,ox+lx, y_origin+y_length);
   }
   for (cal=0; cal<noofcalibrators; cal++) {
	  if (value_of_calibrator[cal]<0.99*ymin || value_of_calibrator[cal]>1.01*ymax ) continue;
	  calibrator_value=(Logflag && value_of_calibrator[cal]>0)?log(value_of_calibrator[cal]):
					   value_of_calibrator[cal];
	  pos= (cal==noofcalibrators-1)?y_origin-y_length: y_origin - yfactor * (calibrator_value-min);
	  kyomove(hdc,ox,pos);
	  kyodraw(hdc,ox+RIM/2,pos);
	  Printr(hdc,(double)(ox-Hsize/2),(double)(pos),1.5,0, calibratorstring[cal]);
		 // Print(0,CENTERRIGHT,ox-Hsize/2,pos,calibratorstring[cal]);
	  if ((Ycal%4)<2) {                                        // print y-as
	  kyomove(hdc,ox+lx,pos);
	  kyodraw(hdc,ox+lx-RIM/2,pos);
	  }
   }
   if (Ycal%2) {    /* tick-marks outside */
	  ox+=RIM/2;
	  lx-=RIM;
   }
	  kyomove(hdc,ox,y_origin   );
	  kyodraw(hdc,ox,y_origin-y_length);
	  if ((Ycal%4)<2) {
		 kyomove(hdc,ox+lx,y_origin   );
		 kyodraw(hdc,ox+lx,y_origin-y_length);
	  }
   if (Ycal%2) {    /* tick-marks outside */
	  ox-=RIM/4;
	  lx+=RIM/4;
   } else lx-=RIM/4;
   if (logyflag) {
	  highest_exponent=(int)(max/log(10)+0.001);
	  lowest_exponent=(int)(min/log(10)-1.1);
	  for (cal=lowest_exponent; cal<=highest_exponent; cal++) {
		 for (mantissa=1;mantissa<10;mantissa++) {
			calibrator_value= log(val((double)mantissa,cal));
			if (calibrator_value > min && calibrator_value < max) {
			   pos= y_origin - yfactor * (calibrator_value-min);
			   if (pos>y_origin || pos<y_origin - y_length) continue;
				  kyomove(hdc,ox,pos);
				  kyodraw(hdc,ox+RIM/4,pos);
				  if ((Ycal%4)<2) {
					 kyomove(hdc,ox+lx,pos);
					 kyodraw(hdc,ox+lx+RIM/4,pos);
				  }
			   }
		 }
	  }
   }
   x_origin=ox+RIM;
   x_length=lx-RIM;
   if (Ycal%4<2) x_length-=RIM;       /* right axis */
   return logyflag;
}


int ascal(int *noofcalibrators, float *min, float *max, int logflag, int calcflag) {
	int n, range, exp, exp0, t, t0, found, negative, precision, extra_decimal=0;
	double unit, value, tempmin, tempmax, ratio, base, lowest_calibrator;
	int calbase[4][4]={{1,0,0,0},{1,3,0,0},{1,2,5,0},{10,20,25,50}};

	int exponent[MAXNOOFCALIBRATORS];
    #ifdef TRACE
       if (Trace==2) {
          tracefile=fopen("TRACE","at");
		  fprintf(tracefile,"\n%4d: *min=%.4f *max=%.4f",
			 __LINE__, *min, *max);
          fclose(tracefile);
       }
    #endif
	if (logflag) round(scientific2(*min, &base, &exp));
    else round(scientific2(*max, &base, &exp));
    tempmin=*min * 10 +  fabs(*min)/10000;
    tempmax=*max * 10 -  fabs(*max)/10000;
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
/*
    tempmin/=10;
    tempmax/=10;

	#ifdef TRACE
	  if (Trace==2) {
         tracefile=fopen("TRACE","at");
         fprintf(tracefile,"\n%4d: tempmin=%.4f tempmax=%.4f",
			__LINE__, tempmin, tempmax);
         fclose(tracefile);
      }
    #endif
*/
    if (!calcflag) return logflag;

/* phase 3: print results in minimal-length strings */

    for (n=0; n< *noofcalibrators ; n++) {
        if (value_of_calibrator[n]>(tempmax>0?1.0001* tempmax:0.9999* tempmax)) {
		   #ifdef TRACE
              if (Trace==2) {
				 tracefile=fopen("TRACE","at");
				 fprintf(tracefile,"\ncal[%d]=%.4lf tempmax=%.4lf verschil:%lf",
                    n, value_of_calibrator[n], tempmax, value_of_calibrator[n] - tempmax);
                 fclose(tracefile);
              }
           #endif
           break;
        }
        precision= (range==LINEAR) ? (extra_decimal - exponent[n]): -exponent[n];
		if (precision<0) precision=0;
        sprintf(calibratorstring[n],"%.*lf", precision, value_of_calibrator[n]);
    }
/*
   if (logflag) for (n=0; n<*noofcalibrators; n++) value_of_calibrator[n]=log(value_of_calibrator[n]);
*/
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
   //	10^exponent, exponent is double rather than int, as required for pow()
   return exp(exponent*log(10));
}



 /*     corrcomm  */

#include "corr.h"

#define PICXF 3000/4000       /* correctie factor naar LOTUS.PIC file */
#define PICYF 2200/3000
/*#define SBOX 25               /* grootte viekant meetpunt in plaatje */*/
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8


double getd(FILE *fp);
void display_labels();
void teken(double xp, double yp);
void moef(double xp, double yp);
void point(double xp,double yp);
void numm(double xp, double yp,int number);
void prnt(double xpos,double ypos,int size,int richting,char *tekst);
void prntc(double xpos,double ypos,int size,int richting, char *tekst);
void picopen(void);
void picmove(int x,int y);
void picfont(char font);
void piccolor(int color);
void picsize(int x,int y);
void picdraw(int x,int y);
void pictext(int direction,int position,char *s);
void picend(void);
void picxy(int x,int y);
void picbox(int ox,int oy,int lx,int ly);
void display_data();
void read_wks(char *infile);
void skip(FILE *fp, long noofbytes);

extern int   lijn,lijnxy;
extern int   MaxX,MaxY; //screen coordinates
extern int   print, show, num,spearm,lost;
extern int   rows, columns;
extern int   labels,xx,yy,aa,rows_temp;
extern int   DrawPicFile;    //Schrijf PIC file naar disk
extern int   DrawGraphScreen;//Teken op het geinitialiseerde graphscherm, alleen bij 3D
extern int   *SCHERMPTR;
extern double facx,facy;
extern float upx,upy;
extern float maxttest;
extern float **data;
extern float **PP;
extern char  **label;
extern char  outfile[40];
extern char  file[20];
extern unsigned long MAX_ROW;
extern unsigned long MAX_COL;

FILE *fpout;
FILE *fp;

char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
int     SBOX=25;           /* grootte viekant meetpunt in plaatje */


/********************  GET DOUBLE ***********************/
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


/*************** Print labels ***************/

 void display_labels()
 {
   int label_no;
	puts("\n\nEDOMATRIX\n\n");
   for (label_no=0;label_no<labels;label_no++)
	{
	   printf("#%d %s\n",label_no+1,&label[label_no][1]);
	   if(print)
	       fprintf(stdprn,"#%d %s\n",label_no+1,&label[label_no][1]);
	 }

 }

/*************** Subroutines tekenen en schrijven PIC file ***************/

 void teken(double xp,double yp)
  {  picdraw((int)(xp*PICXF),(int)(yp*PICYF)); }

 void moef(double xp,double yp)
  {  picmove((int)(xp*PICXF),(int)(yp*PICYF));  }

 void point(double xp,double yp)
   {
   moef  (xp+SBOX,yp+SBOX);
   teken (xp-SBOX,yp+SBOX);
   teken (xp-SBOX,yp-SBOX);
   teken (xp+SBOX,yp-SBOX);
   teken (xp+SBOX,yp+SBOX);
   }

 void numm(double xp,double yp,int number)
  {
   char tekst[5];
   itoa(number,tekst,10);
	prnt(xp,yp,1,0,tekst);
  }

 void prnt(double xpos,double ypos,int size,int richting,char *tekst)
  {
   moef(xpos,ypos);
   pictext(((int)(richting/90)),BOTTOMLEFT,tekst);
   size = size;
  }

 void prntc(double xpos,double ypos,int size,int richting, char *tekst)
 {
  moef(xpos,ypos);
  pictext(((int)(richting/90)),BOTTOMLEFT,tekst);
  size = size;
 }


/*************** openen en verwerken PIC file ***************/

 void picopen(void)
 {
 int n;
 unsigned char c;

	if ((fpout=fopen(outfile,"wb")) == NULL)
		 {
		 printf("cannot open output-file %s\n",outfile);
		 exit(-1);
		 }

	for (n=0;n<17;n++)
      {
      c=header_vector[n];
      fputc(c,fpout);
      }
   picsize(80,80);
   picfont(1);
   piccolor(0);
}


 void picfont(char font)  {   fputc(FONT,fpout);   fputc(font,fpout);  }

 void piccolor(int color)
 {
 if(DrawPicFile)
   {
   if (color==0) fputc(COLOR0,fpout);
   if (color==1) fputc(COLOR1,fpout);
   if (color==2) fputc(COLOR2,fpout);
	if (color==3) fputc(COLOR3,fpout);
   if (color==4) fputc(COLOR4,fpout);
   if (color==5) fputc(COLOR5,fpout);
   if (color==6) fputc(COLOR6,fpout);
   if (color==7) fputc(COLOR7,fpout);
   if (color==8) fputc(COLOR8,fpout);
   if (color==9) fputc(COLOR9,fpout);
   if (color==10) fputc(COLOR10,fpout);
   if (color==11) fputc(COLOR11,fpout);
   if (color==12) fputc(COLOR12,fpout);
   if (color==13) fputc(COLOR13,fpout);
   if (color==14) fputc(COLOR14,fpout);
   if (color==15) fputc(COLOR15,fpout);
   }
   if(DrawGraphScreen) setcolor(color);
 }

 void picsize(int x, int y)
  {
  if(DrawPicFile) { fputc(SIZE,fpout); picxy(x,y); }
  }

 void picdraw(int x, int y)
  {
   x=x;
   xx=((double)x/facx);
   yy=((double)y/facy);
  if(DrawPicFile)   { fputc(DRAW,fpout);  picxy(x,y); }
  if(DrawGraphScreen) lineto((int)xx,MaxY-(int)yy);
  }

  void picmove(int x,int y)
  {
  double xx,yy;
   xx=((double)x/facx);
   yy=((double)y/facy);

  if(DrawPicFile)   { fputc(MOVE,fpout);   picxy(x,y); }
  if(DrawGraphScreen) moveto((int)(xx),MaxY-(int)yy);
  }

 void pictext(int direction,int position,char* s)
 {
   if(DrawPicFile)
      {
       fputc(TEXT,fpout);
       fputc((unsigned char)direction*16+position,fpout);
       fputs(s,fpout);
       fputc('\0',fpout);
      }

   if(DrawGraphScreen)
     {
      setusercharsize(10,10,10,10);
      if(direction)
		   {
		    settextjustify(LEFT_TEXT,BOTTOM_TEXT);
		    settextstyle(SMALL_FONT,VERT_DIR ,0);
		   }
      else 	   {
		    settextjustify(LEFT_TEXT,TOP_TEXT);
		    settextstyle(SMALL_FONT,HORIZ_DIR,0);
			}
		outtext(s);
	 }
 }

 void picend(void) {   fputc(96,fpout);/*END*/   fclose(fpout); }

 void picxy(int x,int y) { fputc(x/256,fpout); fputc(x%256,fpout);
			   fputc(y/256,fpout); fputc(y%256,fpout); }

 void picbox(int ox,int oy,int lx,int ly)
 {
   picmove(ox   ,oy   );
   picdraw(ox   ,oy+ly);
   picdraw(ox+lx,oy+ly);
   picdraw(ox+lx,oy   );
   picdraw(ox   ,oy   );
 }

/************************ print data ************************/

 void display_data()
 {
 int n,m,i,j,pages,cols;
 float value=0;

 if (print)
  {
   fputc(15,stdprn);
			 /* set condensed  */
  }
 pages=(int)(columns/14)+1;
 for (i=1;i<=pages;i++)
 {
  j=(i-1)*14;

  if((i*14) < columns) cols=i*14;
  else cols=columns;

 for (n=0;n<rows;n++)
  {
   printf("%4d",n);
   if(print) fprintf(stdprn,"%4d",n);

   for (m=j;m<cols;m++)
    {
    if(bioskey(1)) {getch(); return;}
  if (n==0)
    {
     printf("    #%d   ",m+1);
     if(print) fprintf(stdprn,"    #%d   ",m+1);
    }
 else {
	  value= data[n][m];
	    if (value==MISSING)
       {
	printf("     --- ");
	if (print)
	  fputs("     --- ",stdprn);
       }
      else if (value >=0)
	 {
	   printf("%9.3f ",value);
		if (print)
	       fprintf(stdprn,"%9.3f",value);

	   }
      }
     }
	puts(" ");
	if (print)  fputs("\n",stdprn);
   }
   if (print)  fputc(12,stdprn);

  }
  if (print)
     {
     fputc(27,stdprn);
     fputc(64,stdprn);
     fputs("\n\n",stdprn);
     }

}

/*************** Lees lotus *.wk1 of *.wks in ***************/

 void read_wks(char *infile)
 {
 enum opcode {
	BOF=0,Eof,CALCMODE,CALCORDER,SPLIT,
	SYNC=5,DIMENSIONS,WINDOW1,COLW1,WINDOW2,
	COLW2=10,NRANGE,BLANK,INTEGER,NUMBER,
	LABEL=15,FORMULA,
	TABLE=24,QRANGE,PRANGE,SRANGE,FRANGE,KRANGE1,
	DRANGE=32,KRANGE2=35,PROTECT,FOOTER,HEADER,SETUP,MARGINS,
	LABELFMT=41,TITLES,GRAPH=45,NGRAPH,CALCOUNT,FORMAT,CURSORW12,
	STRING=51,SNRANGE=71,WKSPASS=75,
	HIDCOL1=100,HIDCOL2,PARSE,RRANGES,MRANGES=105,CPI=150};

	int  op, file_type;
	int  column, row,tel;
	long start_col, start_row, end_col, end_row;
	char format, c, order=0, pos;
	float floating,integer;
	long count, body_length, formula_size;
   unsigned char buffer[439];
   char nop[9];

   clrscr();
   lost=FALSE;
   count=tel=labels=0;
   rows=columns=0;

   if ((fp=fopen(infile,"rb")) == NULL)
    {
      printf("Can't find file %s",infile);
       exit(-1);
    }

   printf("\n\n\n\n\n\n\n\n                   LOADING %s\n\n",infile);
   swrite (10,10,"Bytes read : \0",0x0600);
   op=getw(fp);
   while (op!=Eof)
   {
       body_length=(long)getw(fp);
		 if (body_length < 0)
	   {
	   printf("\nDeze worksheet kan ik niet verwerken\n");
	   exit(-1);
	   }


   count += body_length+4;
	tel += (int)body_length+4;

   if (!show)
       {
        if (tel>100) { tel-=100;  swrite(23,10,ltoa(count,nop,10),0x0600); }
       }

  if (show) printf("count: %d ",count);
  switch(op)
    {
           case INTEGER:
               format=getc(fp);
               column=getw(fp);
               row=getw(fp);
               integer=(float)getw(fp);
	       if (integer < 0 ) break;
               if (row >= MAX_ROW || column >= MAX_COL) break;
               if (show)
                  printf("integer: col %d, row %d, read:%8.3f\n",column,row,integer);
			   if (row > rows) rows=row;
				  data[row][column]=integer;
               break;
           case NUMBER:
					format=getc(fp);
	       column=getw(fp);
               row=getw(fp);
               floating=getd(fp);
			   if (floating <0 || floating > 1e10) break;
			   if (row >= MAX_ROW || column >= MAX_COL) break;
	       if (show)
				  printf("number: col %d, row %d, read:%8.3f\n",column,row,floating);
				if (row > rows) rows=row;
				  data[row][column]= floating;
               break;
	   case FORMULA:
               format=getc(fp);
               column=getw(fp);
               row=getw(fp);
               floating=(float)getd(fp);
               formula_size=getw(fp);
               skip(fp,formula_size);
			   if (floating <0 || floating > 1e10) break;
               if (row >= MAX_ROW || column >= MAX_COL) break;
               if (show)
                   printf("FORMULA: col %d, row %d, read:%8.3f\n",column,row,floating);
			   if (row > rows) rows=row;
				   data[row][column]= floating;
	       break;
	   case BLANK:
	       format=getc(fp);
	       column=getw(fp);
	       row=getw(fp);
	       if (row >= MAX_ROW || column >= MAX_COL) break;
	       if (show) printf("BLANK: col %d, row %d\n",column,row);
				  data[row][column]=(float)MISSING;
	       break;
	   case LABEL:
	       pos=0;
	       format=getc(fp);
	       column=getw(fp);
	       row=getw(fp);
	       if (show) printf("LABEL: col %4d , row %4d ",column,row);
	       if (row < MAX_ROW && column < MAX_COL)
				{
				  if (column > columns) columns=column;
				  if(row==0) data[row][column]=(float)MISSING+1+labels;
					else     data[row][column]=(float)MISSING;
				 }

				c=getc(fp);
				while (c)
				 {
		  if (show) putchar(c);
		  if ((column < MAX_COL) && (pos<22) && (row==0))
			     label[column][pos++]=c;
		  c=getc(fp);
		  }
				 if ((column < MAX_COL) )
					  label[column][pos++]=0;
				 if (show) putchar('\n');
				break;
	   case BOF:
	       file_type=getw(fp);
			 if (show)
			printf("file-type %X\n",file_type);
			 break;
		case DIMENSIONS:
			 start_col=getw(fp);
			 start_row=getw(fp);
			 end_col=getw(fp);
			 end_row=getw(fp);
			sprintf(buffer,"dimensions col;row: %ld;%ld en %ld;%ld\r\n",
					 start_col,start_row,end_col,end_row);
			gotoxy(1,1);
			cputs(buffer);
			if (!show) swrite(23,10,ltoa(count,nop,10),0x0600);
			  rows   =(int) ((end_row >= MAX_ROW ) ? MAX_ROW : end_row) ;
			  columns=(int) ((end_col >= MAX_COL)  ? MAX_COL : end_col) ;
		swrite(23,12,"Initializing",0x0600);
/*                for (row=0;row<MAX_ROW;row++)
		    for (column=0;column<MAX_COL;column++)
			data[row][column]=MISSING;*/
		for (row=0;row<=rows+3;row++)
		    for (column=0;column<=columns;column++)
			data[row][column]=MISSING;
		swrite(23,12,"            ",0x0600);
		rows=columns=0;
		break;
	   case CPI:
			 if (show)
			printf("CPI=%d, dus %d actieve kolommen\n",body_length,body_length/6);
			 skip(fp,body_length);
			 break;
		default:
			 if (show) printf("opcode: %d lengte: %d bytes \n",op,body_length);
	       skip(fp,body_length);
      }
     op=getw(fp);
	}

fclose(fp);
if (row >= MAX_ROW || column>= MAX_COL)    lost=TRUE;
columns++;
rows++;
labels=columns;
nop[1]=format;  /* voorkom foutmelding never used */
nop[2]=order;
}

/********************** skip n bytes ******************/
void skip(FILE *fp, long noofbytes)
  { while(noofbytes>0L) {fgetc(fp); noofbytes--;}   }



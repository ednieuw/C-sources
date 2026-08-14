// kyo6: translate lotus.pic file to kyocera F1000 laserprinter
// 14/2/1988 EJN
// 6/16/89 RCA
// EJN aug 1993
// EJN dec 1993 drive a: & b: auto selectie
// FILL(O) , circle support Okt 1994
/* compile: Large
if exist error del error
if exist kyo6.exe del kyo6.exe
rem tcc -mh -v -C kyo6
tcc -mh -C kyo6 >error
if exist kyo6.exe del kyo6.obj
type error
*/




#define TRUE   1
#define FALSE 	0

#define HEADER TRUE
#define LOTUS FALSE
#define FIG 2
#define MAXFIG 6
#define PAPERLENGTH     3300
#define BOTTOM_OF_PAGE    5
#define BOTTOM_OF_PAGE6 100
#define KYO_X           24 /* KYO CORRECTIE */
#define PIXELS_PER_LINE 2
/*
#define KYO_Y           -96 /* KYO CORRECTIE */
*/
#define KYO_Y           -106 /* KYO CORRECTIE */
#define LETTERSHIFT     -10
#define X_FACTOR        0.65
#define X_FACTOR6       0.27
#define Y_FACTOR        0.65
#define Y_FACTOR6        0.3
#define X_ORIGIN         300
#define X_ORIGIN1       2000
#define X_ORIGIN2       3200
#define X_LENGTH        3200
#define Y_ORIGIN           0
#define Y_ORIGIN1       2000
#define Y_ORIGIN2       3200
#define Y_LENGTH        2800
#define POS0 		 		 100
#define POS1 				  20
#define POS2 				-140
#define POS3 				 -30


#define SHOW 1
#define KYO	 1
#define OFF ' '
#define ON  '#'
#define GROUP 8
#define LENFILENAME 80
#define MAXNOOFITEMS 320
#define NOOFLINES 20
#define MAXFILES MAXNOOFITEMS
#define MAX_LEN 80
#define MISSING -999999.0
#define HLON   textcolor(BLACK); textbackground(WHITE)
#define HLOFF  textcolor(LIGHTGRAY); textbackground(BLACK)
#define CLS    clrscr()
#define CURSOR(a,b) gotoxy(a,b)
#define MORE   gotoxy(70,25);HLON;cprintf(" MORE --> ");HLOFF;getch();CLS
/*
#define HLON  cprintf("\033[7m")
#define HLOFF cprintf("\033[0m")
#define CURSOR(a,b) cprintf("\033[%d;%dH",a,b)
#define HLON  highvideo()
#define HLOFF normvideo()
*/
/* pic.h: header for pic-files 280288 */
#define PAPERWIDTH      2240
#define LINE_HEIGHT 20
#define KYOBOTTOM	1000
#define PEN             3   /* pen diameter in dots */
#define FONT1 10 /* 14-point bold proportional */
#define FONT6 15 /* 9-point */
#define MOVE 160
#define DRAW 162
#define FILL 48
#define FILLO 208
#define COLOR 1		/* number of colors */
#define COLOR0 176
#define COLOR15 191
#define TEXT 168
#define PICFONT 167
#define SIZE 172
#define END 0X60
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8

#define QUOTE 34
#define SPECIAL_KEY 0
#define UP 328
#define DOWN 336
#define HOME 327
#define SHIFT_HOME 382
#define LEFT 331
#define RIGHT 333
#define PgUp 329
#define PgDn 337
#define EXEC 335
#define ESC 27
#define CANCEL 366
#define INSERT 338
#define DELETE 339
#define F1 315
#define F2 316
#define F3 317
#define F4 318
#define F5 319
#define F6 320
#define F7 321
#define F8 322
#define F9 323
#define F10 324
#define HELP 367 /* F1 toets */
#define SPACE 32
#define RETURN 13
#define BACKSPACE 8
#define CR 13
#define TAB 9
#define BACKTAB 271
#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_Z 26

#include <stdio.h>
#include <bios.h>
#include <process.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <mem.h>

#define W_SPECIAL_KEY 31
#define W_UP 448
#define W_DOWN 450
#define W_HOME 452
#define W_ENDKEY 455
#define W_LEFT 451
#define W_RIGHT 449
#define W_PgUp 456
#define W_PgDn 457
#define W_EXEC 453
#define W_CANCEL 480	/* = annuleer = laat vervallen = cancel */
#define W_BACKTAB 461
#define W_INSERT 454
#define W_DELETE 455
#define W_F1 384
#define W_F2 385
#define W_F3 386
#define W_F4 387
#define W_F5 388
#define W_F6 389
#define W_F7 390
#define W_F8 391
#define W_F9 392
#define W_F10 393
#define W_HELP 481
#define MAXLEN 80

#define S1 cprintf("Programma \"KYO8.EXE\" dd %s",__DATE__)
#define S2 cprintf("Inlichtingen en fouten-melding: Ed Nieuwenhuys / Rob Aalberse")
#define S3 cprintf("Reading *.PIC directory ...");
#define M0 cprintf("\r\nNo *.PIC-file found\r\n\r\nInsert another disc and press RETURN to re-try\r\n")
#define M14  cprintf("%c%12s",flag[n],filename[n])
#define M14a cprintf("%c",flag[n])
#define M0_STRMENU cprintf("%-13s",array[n])
#define E0 cprintf("\r\ncannot open input file %s; press RETURN to re-try\r\n",filename[0])
#define E1 cprintf("\r\nNo PIC-files found; press RETURN to re-try\r\n");
#define E2 cprintf("\r\nNon-printable character \" %c \" (ASCII %d): wrong input-file \"%s\"?\r\nprogramm aborted\r\n",c,c,filename[0])
#define E3 cprintf("\r\nError writing file to disc: Disc full?")
#define M0_CHECK_OUTPUT_FILE cprintf("\r\n\r\noutput-file: \"%s\" \r\n",s)
#define E1_CHECK_OUTPUT_FILE cprintf("\r\n\r\noutput file %s allready exists\r\nenter new name or press RETURN to overwrite\r\noutput-PIC-file :",s)
#define E2_CHECK_OUTPUT_FILE cprintf("\r\nerror %d: cannot open \"%s\" for output\r\nenter new name or press ESC to quit\r\noutput-PIC-file :",ferror(fpout),s)

void 	startup_message(void);
int 	initialise_printer(void);
void 	set_param(void);
void 	get_charsize(void);
void 	kyostart(void);
void 	kyoend(void);
void 	extra_text_menu(void);
void 	picextra_text(void);
void 	getxy(int fig);
void 	farewell(void);
void 	read_environment(void);
float set_env_variable(char env_string[], float default_value);
void 	printstring(char c,int n);
void 	make_ruler(int x, int y, int units, int unitsize);
void 	frame(int hor,int vert);
void 	down(char c,int n);
int 	get_inputfiles(int argc, char *argv1);
void 	makedate(int *m,int *d,int *y,unsigned total);
void 	maketime(int *h,int *m,int *s,unsigned total);
void 	get_outputfile(char *filename, int argc, char * argv2);
int 	select_files(void);
int 	check_output_file(char *filename);
int 	strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines);
void 	write_screen(int startrow, int rows, int nooflines, int new) ;
char 	*strleft(char *buffer, char *s, int n);
int 	strpos(char *s,char c,int pos);
char 	*keycopy(char *target,char *source,int maxlen);
int 	getkey(int flag);
char 	*make_kyofile_name(char *kyofile, char *picfile);
int 	getfile(char *path, char filename[MAXFILES][LENFILENAME+1]);
void 	error(int n, long count);
void 	translate_picfile(char *picfile);
int 	dots(char c);
int 	isvoid(char *s);
int 	check_drive(char *s);

char extra_text[9][MAX_LEN+1];
char header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char kyofile[80];
char kyostring[2][80];
int  font[][4]={
		{0,17,30,54},
		{1,17,30,54},
		{2,19,25,45},
		{3,20,25,45},
		{4,21,25,45},
		{5,22,18,36},
		{6,23,25,45},
		{7,25,18,32},
		{8,26,25,54},
		{9,28,25,54},
	  {10,29,40,64},
	  {11,30,35,54},
	  {12,31,30,45},
	  {13,32,18,36},
	  {14,33,16,27},
	  {15,34,18,40},
	  {16,36,14,31},
		{1,18,30,54},
		{6,24,25,45},
		{8,27,25,45},
	  {15,35,18,40} };
char font10[] = {
	  17,20,20,46,34,52,42,16,22,22,30,69,17,20,17,19,34,34,34,34,34,34,34,34,34,34,18,18,69,69,69,34,
	  65,41,42,45,44,39,36,46,45,18,33,42,35,53,44,46,40,46,43,40,36,43,38,57,38,37,36,23,30,23,64,30,
	  69,34,37,33,37,34,19,37,37,16,16,33,17,54,43,43,37,37,23,33,19,43,32,46,32,32,30,30,30,30,69 };
char font11[] = {
	  14,17,16,38,28,43,35,14,19,19,25,50,14,17,14,16,28,28,28,28,28,28,28,28,28,28,15,15,50,50,50,28,
	  50,34,35,37,36,33,30,38,37,15,28,35,29,45,37,39,33,39,36,33,31,36,32,47,33,31,30,19,25,19,50,25,
	  50,28,31,28,31,29,16,31,30,13,13,28,13,45,30,31,31,31,19,27,16,30,26,38,26,27,25,25,25,25,50 };
/*
	 ,  , !, ", #, $, %, &, ', (, ), *, +, ,, -, ., /, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?,
		@, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _,
		`, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, {, |, }, ~
*/
int fig,Dwars=FALSE;
int totalfiles, nooffiles, noofgraphs;
int startup_drive, fileno;
int x, y, result, Header, Fig, Show, Trace, Kyo, Group, Lotus;
float X_factor=X_FACTOR, Y_factor=Y_FACTOR;
int Paperlength=PAPERLENGTH, Pen=PEN, Font=FONT1, default_font=TRUE;
int Y_origin[MAXFIG], Y_length;
int X_origin[MAXFIG], X_length;
int Lettershift=LETTERSHIFT;
int n, count, size;
FILE *fpin, *fpout, *tracefile;
char filename[MAXFILES][LENFILENAME+1];
char fileinfo[MAXFILES][80];
long filesize[MAXFILES];
int file[MAXFILES];        /* files in order of disc directory */
int piccode[MAXFILES];     /* files in plotting order dictated by picstring */
char picstring[80]="ABCDEFGHIJKLMNOPQRSTUVWX";
char flag[MAXFILES];
char startchars[MAXFILES];
char volumelabel[20]="no disc name";
int charsize[256];
char buffer[81];
long currentfilesize;
char Picfont;

int main(int argc, char* argv[]) {
	int i;
	read_environment();
		if (Trace) {
		if ((tracefile=fopen("C:trace","wt"))==0) {
			printf("cannot open \"C:trace\"");
			farewell();
		} else cprintf("C:trace open\r\n");
	}
	if (argc>1) strcpy(filename[0],argv[1]);
	if (argc>2) strcpy(kyofile, argv[2]);
	get_charsize();
	if (argc>1) Show=0;
	if (argc>2) Kyo=0;
	if (Show) startup_message();

	if(check_drive("A:"))	setdisk(0);
	else
	if(check_drive("B:"))	setdisk(1);
	else{

			printf("No floppy in Drive A: or B:\nPress any key");
			getch();
			farewell();
	    }
	while ( (nooffiles=get_inputfiles(argc,filename[0]))<=0) {
		E1;
		if (getch()!=RETURN) farewell();
		else {
		  clrscr();
		  if (Show) startup_message();
		}


	}
	noofgraphs=nooffiles;
	set_param();
	if (Kyo) if (!initialise_printer()) Kyo=0;
	clrscr();
	make_ruler(24,1,10,5);
	if (!Kyo && argc<3) {
		if (piccode[0]==-1) get_outputfile("kyo.kyo", argc, kyofile);
		else get_outputfile(filename[piccode[0]], argc, kyofile);
	}
	kyostart();
	for (i=0;i<noofgraphs;i+=Fig) {
		picextra_text();
		if (Header && !Lotus) {
			fprintf(fpout, "FONT 16;\r\nMAP 0,%d;\r\nTEXT'Disc: %s';\r\n",
				PAPERLENGTH-((Fig==6)?BOTTOM_OF_PAGE6:BOTTOM_OF_PAGE),
				volumelabel);
			for (fig=0;fig<Fig && i+fig<noofgraphs;fig+=2) {
				if (i+fig+1<noofgraphs && fig+1<Fig) fprintf(fpout,"MAP 500,%d;\r\nTEXT'%12s %18s %12s %18s';\r\n",
					PAPERLENGTH - ((Fig==6)?BOTTOM_OF_PAGE6:BOTTOM_OF_PAGE) + fig*20,
					(piccode[i+fig]==-1)?"":filename[piccode[i+fig]],
					(piccode[i+fig]==-1)?"":fileinfo[piccode[i+fig]],
					(piccode[i+fig+1]==-1)?"":filename[piccode[i+fig+1]],
					(piccode[i+fig+1]==-1)?"":fileinfo[piccode[i+fig+1]]);
				else fprintf(fpout,"MAP 500,%d;\r\nTEXT'%12s %18s';\r\n",
					PAPERLENGTH - ((Fig==6)?BOTTOM_OF_PAGE6:BOTTOM_OF_PAGE) + fig*20,
					(piccode[i+fig]==-1)?"":filename[piccode[i+fig]],
					(piccode[i+fig]==-1)?"":fileinfo[piccode[i+fig]]);
			}
		}
		for (fig=0;fig<Fig && i+fig<noofgraphs;fig++) {
			if (noofgraphs>24) {
				fprintf(fpout,"cmnt \"----\";\r\ncmnt FILE: \"%s\";\r\n",filename[i+fig]);
				translate_picfile(filename[piccode[i+fig]]);
			}
			else {
				fileno=piccode[i+fig];
				if (fileno>=0 && fileno<totalfiles) {
					fprintf(fpout,"cmnt \"----\";\r\ncmnt FILE: \"%s\";\r\n",filename[fileno]);
					translate_picfile(filename[fileno]);
				}
			}
		}
		if (Lotus) fprintf(fpout,"\"PAGE;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n");
		else fprintf(fpout,"PAGE;\r\n");
	}
	kyoend();
	if (!Kyo) if (Show) cprintf("\r\noutput-file: %s\r\n",kyofile);
	fclose(fpout);
	farewell();
	return 1;
}


void set_param(void) {
int i;
 if(Dwars) {
	if (Fig==1) {
		if (default_font) Font=FONT1;
		X_origin[0]= Y_ORIGIN;
		Y_origin[0]= X_ORIGIN1;
		X_factor=Y_FACTOR;
		Y_factor=X_FACTOR;

	}
	if (Fig==2) {
		if (default_font) Font=FONT1;
		Y_length=X_LENGTH/2;
		X_origin[0]=Y_origin[1]=Y_ORIGIN;
		Y_origin[0]=X_LENGTH/2+100;
		Y_origin[1]=X_ORIGIN2-100;
		X_factor=Y_FACTOR;
		Y_factor=X_FACTOR;
	}
	if (Fig==4) {
		 if (default_font) Font=FONT6;
		 Y_length=X_LENGTH/2;
		 for (i=0;i<Fig;i+=2) {
			 Y_origin[i]=X_origin[i+1]=(i+2)*X_LENGTH/4;
			 X_origin[i]=Y_ORIGIN;
			 X_origin[i+1]=Y_LENGTH/2-Y_ORIGIN;
			 X_factor= X_FACTOR6; //++++++++++++++Nog aanpassen
			 Y_factor= Y_FACTOR6;
		}
	}
 }
 else {
	if (Fig==1) {
		if (default_font) Font=FONT1;
		X_origin[0]= X_ORIGIN;
		Y_origin[0]= Y_ORIGIN1;
		X_factor=X_FACTOR;
		Y_factor=Y_FACTOR;

	}
	if (Fig==2) {
		if (default_font) Font=FONT1;
		Y_length=Y_LENGTH/2;
		X_origin[0]=X_origin[1]=X_ORIGIN;
		Y_origin[0]=Y_LENGTH/2+100;
		Y_origin[1]=Y_ORIGIN2-100;
		X_factor=X_FACTOR;
		Y_factor=Y_FACTOR;
	}
	if (Fig==6) {
		 if (default_font) Font=FONT6;
		 Y_length=Y_LENGTH/3;
		 for (i=0;i<Fig;i+=2) {
			 Y_origin[i]=Y_origin[i+1]=(i+2)*Y_LENGTH/6;
			 X_origin[i]=X_ORIGIN;
			 X_origin[i+1]=X_LENGTH/2-X_ORIGIN;
			 X_factor= X_FACTOR6;
			 Y_factor= Y_FACTOR6;
		}
	}
 }
}

int initialise_printer(void) {
	int i;
	char c=0;
/*
	char errorcode[5];
*/
	do {
	  fpout=stdprn;	 /**** output directly to Kyocera ****/
/*
	  cprintf("Printing .... ");
*/
	  i=biosprint(1,0,0);	/* initialises LPT1 */
	  if (Trace) fprintf(tracefile,"\r\n%4d biosprint: %d",__LINE__, i);
	  if (i!=16 && i!=144) {
		  if (i==1) {
			  clrscr();
			  cprintf("Kyocera niet goed geinitialiseerd\r\nZet Kyocera aan en re-start computer");
			  farewell();
		  }
		  cprintf("\r\nPrinter error %d\r\nFix and press RETURN\r\n: ",i);
/*
		  if (i&8) cprintf("Switch printer ON and press RETURN\r\n: ");
		  if (i&32) cprintf("Printer out of paper? Fix and press RETURN\r\n: ");
		  *buffer='\0';
		  itoa(i, errorcode, 10);
		  keycopy(buffer, errorcode, 80);
		  if (itoa(buffer,"              ",10)==i && !result) Kyo=1;
		  if (!*buffer) farewell();
*/
		  c=getkey(0);
	  }
	} while (i!=16 && i!=144 || !Kyo || c==ESC);
	if (c==ESC) return 0;
	return 1;
}

/**************** TRANSLATE PIC FILE *********************/

void translate_picfile(char *picfile) {
	int mark=0, c, i, v, w, x1, y1, pos, dotszin, lettershift, direction;
	float step;
	long size, count;
	unsigned char chop, position, vertices;
	char text[MAXLEN+1];
	char posstring[MAXLEN+1];
	int xsize, ysize; /* sizes from PIC-file */
	int fouten = 0;
	int dx,dy,xr,yr,xcentrum,ycentrum;
	Picfont=0;
	if ((fpin=fopen(picfile,"rb")) == NULL) {
		 cprintf("cannot open input file \"%s\"\r\n",picfile);
		 cprintf("\r\nPress ESC to quit\r\nAny other key to continue");
		 if (getch()==27) farewell();
	}
	size=filelength(fileno(fpin));
	clearerr(fpin);
	clearerr(fpout);
	cprintf("\r\n%13s %6ld : ", picfile, size);
	for (i=0; i<17; i++) {
		c=getc(fpin);
		if (c!=header_vector[i]) {
			cprintf("%s is geen PIC-file\r\n");
			farewell();
		}
	}
	chop=getc(fpin);
	step= (float)size/50.0;
	count = 18;
	if (Trace) fprintf(tracefile,"\r\n%4d count=%5ld opcode=%i %2X", __LINE__, count, chop, chop);
	while ((chop>>4) != (END>>4) && fouten <20){
		while ( mark*step < count && mark<50) {
			if (mark<50) {
				putch(220);
				mark++;
			}
		}
		if (chop>=COLOR0 && chop <=COLOR15) {
				  if (!Lotus) w=fprintf(fpout,"cmnt COLOR%d;\r\n",chop-COLOR0);
				  if (w==EOF || ferror(fpout)) error(w,count);
		} else switch(chop) {
			 case SIZE:
				  xsize=256*getc(fpin)+getc(fpin);
				  ysize=256*getc(fpin)+getc(fpin);
				  count+=4;
				  if (!Lotus) w=fprintf(fpout,"cmnt SIZE %d %d;\r\n",xsize, ysize);
				  if (w==EOF || ferror(fpout)) error(w,count);
				  break;
			 case MOVE:
				  getxy(fig);
				  count+=4;
				  if(Picfont <11)
				  {
					if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", x, y);
					else w=fprintf(fpout,"MAP %d, %d;\r\n", x, y);
					if (w==EOF || ferror(fpout)) error(w,count);
				  }
				  break;
			 case DRAW:
				  getxy(fig);
				  count+=4;
				  if(Picfont <11)
				  {
					if (Lotus) 	w=fprintf(fpout,"\"DAP\"%4d\",\"%4d\";\"\r\n", x, y);
					else 			w=fprintf(fpout,"DAP %d, %d;\r\n", x, y);
					if (w==EOF || ferror(fpout)) error(w,count);
				  }
				  break;
			 case FILL:
			 case FILLO:
				  vertices=getc(fpin);
				  getxy(fig);
				  count+=5;
				  x1=x; y1=y;
				  if(Picfont<11)
				  {
					if (Lotus)  w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", x, y);
					else     	w=fprintf(fpout,"MAP %d, %d;\r\n", x, y);
					if (w==EOF || ferror(fpout)) error(w,count);
				  }
				  for (v=1;v<vertices+1;v++) {
					  getxy(fig);
					  count+=4;
					  if(Picfont<11)
					  {
						if (Lotus)  w=fprintf(fpout,"\"DAP\"%4d\",\"%4d\";\"\r\n", x, y);
						else     	w=fprintf(fpout,"DAP %d, %d;\r\n", x, y);
						if (w==EOF || ferror(fpout)) error(w,count);
					  }
				  }
				  if(Picfont<11)
				  {
					if (Lotus)  w=fprintf(fpout,"\"DAP\"%4d\",\"%4d\";\"\r\n", x1, y1);
					else     	w=fprintf(fpout,"DAP %d, %d;\r\n", x1, y1);
					if (w==EOF || ferror(fpout)) error(w,count);
				  }
				  break;
			 case PICFONT:
				  c=getc(fpin);
				  Picfont=c;
				  count++;
				  if(Picfont<10)
					{
					if (Lotus) w=fprintf(fpout,"\"FONT \"%d\";\"\r\n",font[Font][0]);
					else w=fprintf(fpout,"FONT %d;\r\n",font[Font][0]);
					if (w==EOF || ferror(fpout)) error(w,count);
					}
			 else if(Picfont>10)
			 {
			  getc(fpin);
			  getxy(fig);
			  xcentrum=x;ycentrum=y; // centrum figuur
			  count+=5;
			  getc(fpin);
			  x=256*getc(fpin)+getc(fpin);
			  y=256*getc(fpin)+getc(fpin);
			  xr=(int)((float) x * X_factor);
			  yr=(int)((float) y * Y_factor);
			  count+=5;

			 switch(Picfont)
				{
				case 11: // open circle
					if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum, ycentrum);
					else w=fprintf(fpout,"MAP %d, %d;\r\n",xcentrum, ycentrum);
					if (w==EOF || ferror(fpout)) error(w,count);

					if (Lotus) w=fprintf(fpout,
							"\"CIR\"%4d \";\"\r\n",xr-Pen,xr);
					else w=fprintf(fpout,"CIR %4d;\r\n",xr);
					if (w==EOF || ferror(fpout)) error(w,count);
					break;
				case 12: //filled circle
					if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum, ycentrum);
					else w=fprintf(fpout,"MAP %d, %d;\r\n",xcentrum, ycentrum);
					if (w==EOF || ferror(fpout)) error(w,count);

					if (Lotus) w=fprintf(fpout,
							"\"ARC\" 0 \",\" %4d \",\" 0 \",\" 360 \";\"\r\n",xr);
					else w=fprintf(fpout,"ARC 0, %4d, 0, 360;\r\n",xr);
					if (w==EOF || ferror(fpout)) error(w,count);
					break;
				case 13: // open box
//					getc(fpin);
//					getxy(fig);
//					count+=4;  // get MOVE linker bovenhoek
					if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum-xr, ycentrum-yr);
					else w=fprintf(fpout,"MAP %d, %d;\r\n", xcentrum-xr, ycentrum-yr);
					if (w==EOF || ferror(fpout)) error(w,count);

					if (Lotus) w=fprintf(fpout,
							"\"BOX\"%4d\"%4d\";\"\r\n",2*xr,2*yr);
					else w=fprintf(fpout,"BOX %4d, %4d;\r\n",2*xr,2*yr);
					if (w==EOF || ferror(fpout)) error(w,count);
					break;

				case 14: 		// filled box
//					getc(fpin);
//					getxy(fig);
//					count+=4;  // get MOVE linker bovenhoek
					if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum-xr, ycentrum-yr);
					else w=fprintf(fpout,"MAP %d, %d;\r\n", xcentrum-xr, ycentrum-yr);
					if (w==EOF || ferror(fpout)) error(w,count);

					if (Lotus) w=fprintf(fpout,
							"\"BLK\"%4d\",\"%4d\";\"cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n",2*xr,2*yr);
					else w=fprintf(fpout,"BLK %4d, %4d;\r\n",2*xr,2*yr);
					if (w==EOF || ferror(fpout)) error(w,count);
					break;
				case 15:    // triangle
						Picfont=10; break;
				case 16:    // filled triangle
					for (dx=0,dy=-xr; dx <= xr ;
						dx+= PIXELS_PER_LINE/2, dy+= PIXELS_PER_LINE)
						 {
						if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum-dx, ycentrum+dy);
						else w=fprintf(fpout,"MAP %d, %d;\r\n", xcentrum-dx, ycentrum+dy);
						if (w==EOF || ferror(fpout)) error(w,count);
						if (Lotus) w=fprintf(fpout,"\"DAP\"%4d\",\"%4d\";\"\r\n", xcentrum+dx, ycentrum+dy);
						else w=fprintf(fpout,"DAP %d, %d;\r\n", xcentrum+dx, ycentrum+dy);
						if (w==EOF || ferror(fpout)) error(w,count);
						 }
					break;
				case 17:     // upside_down triangle
						Picfont=10; break;
				case 18:     // upside_down triangle, filled
					for (dx=0,dy=xr; dx <= xr ; dx+= PIXELS_PER_LINE/2, dy -= PIXELS_PER_LINE)
						{
						if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n", xcentrum-dx, ycentrum+dy);
						else w=fprintf(fpout,"MAP %d, %d;\r\n", xcentrum-dx, ycentrum+dy);
						if (w==EOF || ferror(fpout)) error(w,count);
						if (Lotus) w=fprintf(fpout,"\"DAP\"%4d\",\"%4d\";\"\r\n", xcentrum+dx, ycentrum+dy);
						else w=fprintf(fpout,"DAP %d, %d;\r\n", xcentrum+dx, ycentrum+dy);
						if (w==EOF || ferror(fpout)) error(w,count);
						}
					break;
				 }
				}
				break;
			 case TEXT:
				  pos=0;
				  text[MAXLEN-1]='\0';
				  c=getc(fpin);
				  count++;
				  direction=90*(c>>4);
				  position=c%16;
				  dotszin=0;
				  do {
					  c=getc(fpin);
					  count++;
					  text[pos]=c;
                 if (c) dotszin+=dots(c);
					  pos++;
              } while (c!=0 && pos<MAXLEN);
				  if(direction==90 || direction==270) {
					  lettershift=Lettershift;
                 x1 = PAPERLENGTH - y;
                 y  = x +KYO_Y;
					  x  = x1;
              } else lettershift=Lettershift;
              if (direction==0 || direction==90) {
                 switch (position) {
                    case CENTER:
                       strcpy(posstring,"CENTER");
                       x-=dotszin/2;
                       y+=font[Font][3]/2 + lettershift;
						break;
                    case CENTERLEFT:
                       strcpy(posstring,"CENTERLEFT");
                       y+=font[Font][3]/2 + lettershift;
							  break;
						  case CENTERTOP:
                       strcpy(posstring,"CENTERTOP");
                       x-=dotszin/2;
                       y+=font[Font][3] + lettershift;
							  break;
                    case CENTERRIGHT:
                       strcpy(posstring,"CENTERRIGHT");
                       x-=dotszin;
							  y+=font[Font][3]/2 + lettershift;
                       break;
                    case CENTERBOTTOM:
							  strcpy(posstring,"CENTERBOTTOM");
							  x-=dotszin/2;
							  break;
                    case TOPLEFT:
							  strcpy(posstring,"TOPLEFT");
                       y+=font[Font][3] + lettershift;
                       break;
                    case TOPRIGHT:
							  strcpy(posstring,"TOPRIGHT");
                       x-=dotszin;
                       y+=font[Font][3] + lettershift;
							  break;
                    case BOTTOMLEFT:
                       strcpy(posstring,"BOTTOMLEFT");
                       break;
                    case BOTTOMRIGHT:
							  strcpy(posstring,"BOTTOMRIGHT");
                       x-=dotszin;
                       break;
                 }
              } else if (direction==180 || direction==270) {
					  switch (position) {
                    case 0:
                       strcpy(posstring,"CENTER");
                       x+=dotszin/2;
							  y-=font[Font][3]/2 + lettershift;
                       break;
                    case 1:
                       strcpy(posstring,"CENTERLEFT");
							  y-=font[Font][3]/2 + lettershift;
                       break;
                    case 2:
                       strcpy(posstring,"CENTERTOP");
                       x+=dotszin/2;
							  break;
						  case 3:
							  strcpy(posstring,"CENTERRIGHT");
                       x+=dotszin;
                       y-=font[Font][3]/2 + lettershift;
                       break;
						  case 4:
                       strcpy(posstring,"CENTERBOTTOM");
							  x+=dotszin/2;
							  y-=font[Font][3] + lettershift;
							  break;
						  case 5:
							  strcpy(posstring,"TOPLEFT");
							  x+=dotszin;
							  break;
						  case 6:
							  strcpy(posstring,"TOPRIGHT");
							  break;
						  case 7:
							  strcpy(posstring,"BOTTOMLEFT");
							  x+=dotszin;
							  y-=font[Font][3] + lettershift;
							  break;
						  case 8:
							  strcpy(posstring,"BOTTOMRIGHT");
							  y-=font[Font][3] + lettershift;
							  break;
					  }
				  }
/*
				  if (x<0) x=0;
				  if (y<0) y=0;
*/
				  if (!Lotus) w=fprintf(fpout,"cmnt %s;\r\n",  posstring);
				  if (Lotus) w=fprintf(fpout,"\"\"\"cmnt TEXT direction=%d position=%d x=%d y=%d\"\"\"\"\"\"\"\"\"\";\"\r\n",direction, position, x, y);
				  else w=fprintf(fpout,"cmnt TEXT direction=%d position=%d x=%d y=%d;\r\n",direction, position, x, y);
				  if (w==EOF || ferror(fpout)) error(w,count);
				  if (direction==90 || direction==270) {
					  if (Lotus) w=fprintf(fpout,"\"FONT %d;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n", font[Font][1] );
					  else w=fprintf(fpout,"cmnt direction=1; FONT %d;\r\n", font[Font][1] );
				  }
				  if (Lotus) w=fprintf(fpout,"\"MAP\"%4d\",\"%4d\";\"\r\n",x, y);
				  else w=fprintf(fpout,"MAP %d, %d;\r\n",x, y);
				  if (w==EOF || ferror(fpout)) error(w,count);
				  if (Lotus) w=fprintf(fpout,"\"TEXT '%s';cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n", text);
				  else w=fprintf(fpout,"TEXT '%s';\r\n", text);
				  if (w==EOF || ferror(fpout)) error(w,count);
				  /* zet zo nodig font weer recht */
				  if(direction==90 || direction == 270) {
					  if (Lotus) w=fprintf(fpout,"\"FONT %d;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n", font[Font][0]);
					  else w=fprintf(fpout,"FONT %d;\r\n", font[Font][0]);
					  if (w==EOF || ferror(fpout)) error(w,count);
				  }
				  break;
			 default:
				 cprintf("NOP     onbekende opcode %d\r\n",chop);
				 fouten++;
		}
		chop=getc(fpin);
		if (Trace) fprintf(tracefile,"%4d count=%5ld opcode=%i %2X", __LINE__, count, chop, chop);
		count++;
	}
	while (mark++<50) putch(220);
	fclose(fpin);
/*
	if (Show) {
		gotoxy(1,22); cprintf("Lengte PIC file %5ld bytes\r\n", count);
		if (*kyofile) cprintf("Resultaat in A:%s", kyofile);
	}
*/
}

void error(int n, long count) {
	clrscr();
	if (!Kyo) cprintf("Write error %d (ferror()=%d) at bytecount %ld\r\ndisc full?", n, ferror(fpout), count);
	else {
		cprintf("Write error %d (ferror()=%d) at bytecount %ld", n, ferror(fpout), count);
		printf("\r\nPress ESC to quit\r\nPress any other key to continue ");
		if (getch()==27) farewell();
	}
putch('\r\n');
}

int get_options(void) {
int i ,ii, n, option;
	strcpy(kyostring[0],"Current output to FILE rather than to Kyocera");
	strcpy(kyostring[1],"Current output to KYOCERA rather than to file");
	for (;;) {
		do {
			clrscr();
			cprintf("D - to Disc? %s\r\n",kyostring[Kyo]);
//			cprintf("O - Portrait or Landscape? %s\r\n",(!Dwars)?" .. Portrait":" .. Landscape");
			cprintf("E - Extra text: Header/Footer/Y-legend\r\n");
			cprintf("F - Font: %d\r\n",Font);
			if (!Lotus) cprintf("H - print file-header: %s\r\n",(Header)?"YES":"NO");
			if (!Kyo) cprintf("L - Lotus Number-format for Kyo-file: %s\r\n",(Lotus)?"YES":"NO");
			cprintf("P - Pen size: %d\r\n",Pen);
			if(!Dwars){
			cprintf("N - number of figures per page: %d\r\n",Fig);
			cprintf("    number of figures selected: %d\r\n\r\n",noofgraphs);
						 }
			else {
			printf("N - number of figures per page: %d\r\n",Fig);
			printf("    number of figures selected: %d\r\n\r\n",noofgraphs);
				  }
			if (noofgraphs<25) {
				for (i=0;i<noofgraphs;i+=Fig) {
//				if (Fig>2) cprintf("       ");
					for (n=0;n<Fig;n+=2) {
					if(n) cprintf("       : ");
					 else cprintf("Page %2d: ",i/Fig+1);
						if (i+n+1<noofgraphs) cprintf("%-12s   %-12s\r\n",
						piccode[i+n]==-1?".":filename[piccode[i+n]],
						piccode[i+n+1]==-1?".":filename[piccode[i+n+1]]);
						else if (i+n<noofgraphs) cprintf("%-12s\r\n",
						piccode[i+n]==-1?",":filename[piccode[i+n]]);
					}
				}
			}
			cprintf("\r\nRETURN = accept options: ");
			if ((option=getkey(0))==RETURN || option==EXEC) return 0;
			if (option==ESC || option==CANCEL)             return -1;
		} while (strpos("DEFHLNOP",option,0)==-1);
		switch (option) {
			case 'E':
				extra_text_menu();
				break;
			case 'H':
				Header=(Header)?0:1;
				break;
			case 'L':
				Lotus=(Lotus)?0:1;
				break;
			case 'N':
				clrscr();
		if(Dwars) {
				cprintf("Number of figures per page (1, 2 or 4) default:%d\r\n: ", Fig);
				gets(buffer);
				n=atoi(buffer);
				if (n==1 || n==2 || n==4) Fig=n;
				set_param();
				if (Fig>1 && noofgraphs<25) {
					for (i=0;i<noofgraphs;i+=2)
						cprintf("%c = %12s        %c %c %12s\r\n",i+'A', filename[file[i]],(i+1<noofgraphs)?i+1+'A':'\0',(i+1<noofgraphs)?'=':'\0', (i+1<noofgraphs)?filename[file[i+1]]:"");
					cprintf("\r\nPIC-file sequence: \"%s\"\r\n",picstring);
					keycopy(buffer,picstring,25);
					strcpy(picstring,buffer);
					for (i=0;i<strlen(picstring);i++) {
						picstring[i]=toupper(picstring[i]);
						ii=picstring[i]-'A';
						if (ii>=0 && ii<noofgraphs) piccode[i]=file[ii];
						else piccode[i]=-1;
					}
					noofgraphs=strlen(picstring);
				}
			  }

		 else {
				cprintf("Number of figures per page (1, 2 or 6) default:%d\r\n: ", Fig);
				gets(buffer);
				n=atoi(buffer);
				if (n==1 || n==2 || n==6) Fig=n;
				set_param();
				if (Fig>1 && noofgraphs<25) {
					for (i=0;i<noofgraphs;i+=2)
						cprintf("%c = %12s        %c %c %12s\r\n",i+'A', filename[file[i]],(i+1<noofgraphs)?i+1+'A':'\0',(i+1<noofgraphs)?'=':'\0', (i+1<noofgraphs)?filename[file[i+1]]:"");
					cprintf("\r\nPIC-file sequence: \"%s\"\r\n",picstring);
					keycopy(buffer,picstring,25);
					strcpy(picstring,buffer);
					for (i=0;i<strlen(picstring);i++) {
						picstring[i]=toupper(picstring[i]);
						ii=picstring[i]-'A';
						if (ii>=0 && ii<noofgraphs) piccode[i]=file[ii];
						else piccode[i]=-1;
					}
					noofgraphs=strlen(picstring);
				 }
				}
				break;
			case 'D':
				Kyo=(Kyo)?0:1;
				break;
			case 'P':
				clrscr();
				cprintf("Pen size (1-200) default:%d\r\n: ",Pen);
				gets(buffer);
				n=atoi(buffer);
				if (n>0 && n<=200) Pen=n;
				break;
			case 'F':
				clrscr();
				cprintf("Fonts:\r\n\r\n"
						 " 7, 16  = very small  ( 7-point)\r\n"
						 "15      = small       ( 9-point)\r\n"
						 " 6      = Elite       (10-point)\r\n"
						 " 8      = Gothic      (12-point)\r\n"
						 " 9      = Gothic bold (12-point)\r\n"
						 " 1      = Courier     (12-point)\r\n"
						 "11      = Helve Bold  (12-point)\r\n"
						 "10      = Helve Bold  (14.4-point)\r\n\r\n"
						 " default:%d\r\n: ",Font);
				gets(buffer);
				n=atoi(buffer);
				if (n>0 && n<=16) Font=n;
				default_font=FALSE;
				break;
		}
	}
}

void get_charsize(void) {
	int i;
	for (i=0;i<256;i++) charsize[i]=1;
}

int dots(char c) {
	if (!(Font==10 || Font==11) || c<32 || c>127) return font[Font][2];
	if (Font==10) return font10[c-32];
	else return font11[c-32];
}

void kyostart(void){
	int w;
	if (Lotus) w=fprintf(fpout,"\"!R! RES;\r\nUNIT D; SPD %d;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n",Pen);
	else w=fprintf(fpout,"!R! RES;\r\nUNIT D; SPD %d;\r\n",Pen);
	if (w==EOF || ferror(fpout)) error(w,count);
}

void kyoend(void) {
	int w;
	if (Lotus) w=fprintf(fpout,"\"RES; EXIT;cmnt\"\"\"\"\"\"\"\"\"\";\"\r\n");
	else w=fprintf(fpout,"RES; EXIT;\r\n");
	if (w==EOF || ferror(fpout)) error(w,count);
}

void make_ruler(int x, int y, int units, int unitsize) {
	int i, ii;
	gotoxy(x, y); putch(213);
	for (i=0;i<units-1;i++) {
		for (ii=0; ii<unitsize-1;ii++) putch(205);
		putch(209);
	}
	for (ii=0; ii<unitsize-1;ii++) putch(205);
	putch(184);
}

void getxy(int fig) {
	x=256*getc(fpin)+getc(fpin);
	y=256*getc(fpin)+getc(fpin);
	x=X_origin[fig] + (int)((float) x * X_factor);
	y=Y_origin[fig] - (int)((float) y * Y_factor);
}

void farewell(void) {
	fclose(fpout);
	fclose(fpin);
	setdisk(startup_drive);
	exit(0);
}

int get_inputfiles(int argc, char *argv1) {
	int i,c;
	if (argc>1) {
		strncpy(filename[0],argv1,sizeof(filename[0]));
		while ((fpin=fopen(filename[0],"rb")) == NULL) {
			 fclose(fpin);
			 clrscr();
			 E0;
			 if (getkey(0)!=CR) farewell();
		}
		fclose(fpin);
		nooffiles=getfile(filename[0],filename);
	} else {
		do {
			totalfiles=getfile("*.pic", filename);
			while (!totalfiles) {
			  clrscr();
			  M0;
			  if (getkey(0)!=CR) farewell();
			  clrscr();
			  totalfiles=getfile("*.pic",filename);
			}
			if (Trace) fprintf(tracefile,"\r\n%4d total_files=%d",__LINE__, totalfiles);
			nooffiles=select_files();
			if (nooffiles<1) {
				clrscr();
				cprintf("no files selected\r\npress RETURN to re-try\r\npress ESC to quit\r\n: ");
				c=getkey(0);
				if (c==CTRL_C ||c==ESC || c==CANCEL) farewell();
				c=ESC;
			} else {
				noofgraphs=nooffiles;
				clrscr();
				cprintf("selected PIC-files:\r\n\r\n");
				for (i=c=0;i<noofgraphs;i++,c++) {
					gotoxy(1+c/15*20, 4+c%15);
					if (noofgraphs<25) picstring[i]='A'+i;
					piccode[i]=file[i];
					cprintf("%-12s %6ld\r\n", filename[file[i]], filesize[file[i]]);
					if (c>=59) {
						MORE;
						clrscr();
						c=-1;
					}
				}
				if (noofgraphs<25) picstring[noofgraphs]='\0';
				gotoxy(1,21);
				printf("RETURN to accept\r\nESC for selection of other files\r\n");
				if (Kyo) cprintf("M for option-menu\r\n: ");
				else     cprintf("Current output: disc-file; press M for option-menu\r\n");
				c=getkey(0);
				if (c==CTRL_C) farewell();
			}
			clrscr();
		} while (c==ESC || c==CANCEL);
		if (c=='M') if (get_options()==-1) return -1;
	}
	return nooffiles;
}

void get_outputfile(char *s, int argc, char *argv2) {
  /**** output to *.kyo file (ASCII) ****/
	  setmem(kyofile,MAXLEN,'\0');
	  if (argc>2) strncpy(kyofile,argv2, sizeof(kyofile));
	  else make_kyofile_name(kyofile,s);
	  if (!check_output_file(kyofile) || (fpout=fopen(kyofile,"wb")) == NULL) {
		  cprintf("cannot open output file %s\r\n",kyofile);
        farewell();
	  }
}

int check_output_file(char *s) {
   char buffer[80];
   result=0;
   while ((fpin=fopen(s,"rb"))!=NULL && result!=RETURN) {
	  strcpy(buffer,s);
     fclose(fpin);
	  E1_CHECK_OUTPUT_FILE;
     if (keycopy(s,buffer,80)==NULL) return 0;
	  if (s==NULL || strlen(s)<1) return 0;
	  if (Show) M0_CHECK_OUTPUT_FILE;
   }
   return 1;
}

int strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines) {
	  int pos, key;
	  int maxlen, n, result, row, col, maxnoofitems;
	  char shorttestname[81];
	  maxlen=80*nooflines/rows-3;
	  if (maxlen<1) maxlen=1;
	  if (maxlen>12) maxlen=12;
	  maxnoofitems=nooflines*20;
	  if (rows>maxnoofitems) rows=maxnoofitems;
	  if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
	  *selected_test=-2;
	  startchars[rows]='\0';
	  n = current_pos;
	  row=startrow + n % nooflines;
	  col=(maxlen+2)*(n/nooflines)+1;
	  gotoxy(col,row);
	  HLON;
	  strleft(shorttestname,filename[n],maxlen);
	  cprintf("%c%-*s",flag[n],maxlen,shorttestname);
	  gotoxy(25,25);
	  cprintf("%s %s (%ld bytes)",filename[n],fileinfo[n],filesize[n]);
	  HLOFF;
	  key=getkey(0);
	  gotoxy(25,25);
	  clreol();
	  gotoxy(col,row);
	  cprintf("%c%-*s", flag[n], maxlen, strleft(shorttestname, filename[n], maxlen));
	  if (key<=SPACE || key>256) {
        switch (key) {
            case F10:
               if (spawnlp(P_WAIT,"drawpic.exe","drawpic.exe",filename[n],NULL)==-1) {
						cprintf("\r\nspawn-error\r\n");
                  farewell();
					}
					startup_message();
					write_screen(startrow,rows,nooflines,TRUE);
					result=n;
               break;
           case UP:
           case W_UP:
                n--;
					 if (n<0) n=rows-1;
                result = n;
              break;
           case DOWN:
           case W_DOWN:
                n++;
                if (n > rows-1) n=0;
                result = n;
				  break;
			  case TAB:
			  case RIGHT:
           case W_RIGHT:
                if (n<rows-nooflines) n+=nooflines;
                result = n;
              break;
           case BACKTAB:
           case LEFT:
           case W_LEFT:
					 if (n>nooflines-1) n-=nooflines;
                result = n;
				  break;
			  case PgUp:
			  case W_PgUp:
					 n=(n/nooflines)*nooflines;
                result = n;
              break;
           case HOME:
           case W_HOME:
                n=0;
                result = n;
				  break;
           case PgDn:
           case W_PgDn:
					 n=(n/nooflines)*nooflines + nooflines-1;
					 if (n>rows-1) n=rows-1;
					 result = n;
				  break;
			  case SHIFT_HOME:
					 n=rows-1;
					 result = n;
				  break;
				 case CTRL_A:
					 *selected_test= -4;
					 result = n;
				  break;
				 case CTRL_B:
				 case CTRL_Z:
					 *selected_test= -3;
					 result = n;
				  break;
				 case RETURN:
				 case SPACE:
					 *selected_test=n;
					 result = n;
              break;
				 case W_ENDKEY:
             case W_EXEC:
             case EXEC:
             case ESC:
             case W_CANCEL:
				 case CANCEL:
                *selected_test= -1;
					 result = n;
              break;
           case CTRL_C:
					exit(0);
           default:
               result = n;
					break;
		  }
	 } else {
            pos=strpos(startchars,toupper(key),n);
            n=(pos<0) ? n : pos;
            result =  n;
    }
    return result;
}

int getkey(int flag) {
int c;
	result=1;
   c=getch();
	if (c==SPECIAL_KEY || c==W_SPECIAL_KEY) {
		c=256+getch();
		if (c>F5 && c<=F10) return c;
	} else {
		if (c==ESC) result=0;
		if (c==CTRL_C) farewell();
		if (!flag) c=toupper(c);
	}
	return c;
}

char *strleft(char *buffer, char *s, int n) {
	int i;
	for (i=0; s[i] && i<n; i++) buffer[i]=s[i];
	buffer[i]='\0';
	return buffer;
}
int getfile(char *path, char filename[MAXFILES][LENFILENAME+1]) {
//int getfile(char *path, char filename[][LENFILENAME+1]) {
	 int n=0, i=0, done, result;
	int month, day, year, hour, min, sec;
	struct ffblk ffblk;
	done=findfirst("*.*",&ffblk,FA_LABEL);
	if (!done) strncpy(volumelabel,ffblk.ff_name,12);
	 else {
		 i=1;
		 cprintf("No disc name found.\r\nEnter disc name (RETURN for none): ...........\b\b\b\b\b\b\b\b\b\b\b");
		 gets(buffer);
		 if (*buffer) {
			 strncpy(volumelabel,buffer,11);
			 volumelabel[11]='\0';
			 sprintf(buffer,"LABEL %s",volumelabel);
			 i=system(buffer);
			 if (i==-1) {
				 cprintf("Unable to perform command \"%s\"\r\nPress any key to continue",buffer);
				 if (getch()==27) farewell();
				 strcpy(volumelabel,"no disc name");
			 }
		 }
		 else strcpy(volumelabel,"no disc name");
		 clrscr();
		 if (!i) cprintf("Disc name: \"%s\"",volumelabel);
		 else puts(volumelabel);
	 }
	 gotoxy(28,14);
	 S3;
	 done=findfirst(path,&ffblk,0);
		  if (done) result= 0;
	else for(n=0,done=0;!done && n<MAXFILES;n++) {
		strcpy(filename[n],ffblk.ff_name);
		filesize[n]=ffblk.ff_fsize;
		maketime(&hour, &min, &sec, ffblk.ff_ftime);
		makedate(&month, &day, &year, ffblk.ff_fdate);
		sprintf(fileinfo[n],"%2.2d-%2.2d-19%2.2d, %2.2d:%2.2d",
			  day, month, year,hour, min);
					 done=findnext(&ffblk);
	}
    result = n;
	for (i=10;i<20;i++) {
	   gotoxy(1,i);
		delline();
	}
	return result;
}

int select_files(void) {
	int t, column, nooffiles, group, fileno, startrow, nooflines, selected_file;
	nooffiles=0;
	selected_file=-1;
	startrow=5;
	nooflines=17;
	startchars[totalfiles]='\0';
	for (t=0;t<totalfiles;t++) {
		startchars[t]=toupper(filename[t][0]);
		flag[t]=OFF;
		file[t]=t;
	}
	write_screen(startrow, totalfiles, nooflines, TRUE);
	fileno=0;
	do {
		group=1;
		gotoxy(1, 23);
		cprintf("Move cursor and then select PIC-files with RETURN-key\r\n"
						  "Press UITVOEREN to end selection of PIC-files\r\n"
						  "Press F10 to view file: ");
		fileno = strmenu(
			fileno,
			&selected_file,
			totalfiles,
			startrow,
			nooflines);
		if ( selected_file == -1) break;
		if ( selected_file == -2) continue;
		if ( selected_file == -3) {
			group=Group;
			if (fileno+group>=totalfiles) {
				group=totalfiles-fileno;
				putch(7);
			}
		}
		if ( selected_file == -4) {
			group=totalfiles-fileno;
		}
		for (t=fileno;t<fileno+group;t++) {
			if (flag[t]==OFF && nooffiles<MAXFILES && nooffiles<totalfiles) {
				flag[t]=ON;
				nooffiles++;
			}
			else if (flag[t]==ON) {
				flag[t]=OFF;
				nooffiles--;
			}
			else {
				fileno--;
				putch(7);
			}
		}
		write_screen( startrow, totalfiles, nooflines, FALSE);
		fileno+=group-1;
		fileno = (++fileno) % totalfiles;
	} while (selected_file != -1);
	for (t=0,column=0;t<totalfiles;t++) if (flag[t]==ON) file[column++]=t;
	if (nooffiles>2 && !Dwars) Fig=6;
	if (nooffiles>2 &&  Dwars) Fig=4;
	return nooffiles;
}

void write_screen(int startrow, int rows, int nooflines, int new) {
int maxlen, n, row, col, maxnoofitems;
  char shorttestname[81];
  maxlen=80*nooflines/rows-3;
  if (maxlen<1) maxlen=1;
  if (maxlen>12) maxlen=12;
  maxnoofitems=nooflines*20;
  if (Trace) fprintf(tracefile,"\r\n%4d: rows=%d nooflines=%d maxnoofitems=%d maxlen=%d",
	  __LINE__, rows, nooflines, maxnoofitems, maxlen);
  if (rows>maxnoofitems) rows=maxnoofitems;
  if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
  for (n=0;n<rows;n++) {
	  row=startrow + n % nooflines;
	  col=(maxlen+2)*(n/nooflines)+1;
	  gotoxy(col,row);
	  if (new) {
			 strleft(shorttestname,filename[n],maxlen);
			 cprintf("%c%-*s",flag[n],maxlen,shorttestname);
		}
	  else putch(flag[n]);
  }
}


char *make_kyofile_name(char *kyofile, char *picfile) {
	int pos=0;
	char c;
	*kyofile='\0';
	do {
		c=picfile[pos];
		kyofile[pos]=c;
		pos++;
	} while(c!='.' && c!='\0');
	if (c!='.') {
		kyofile[pos]='.';
		pos++;
	}
	strcat(kyofile,"kyo");
	if (Trace) fprintf(tracefile,"\r\n%4d kyofile=%s picfile=%s",__LINE__,kyofile, picfile);
	return kyofile;
}

void startup_message(void) {
	clrscr();
	frame(65,4);
	gotoxy(3,2);
	S1;
	gotoxy(3,3);
   S2;
   gotoxy(1,5);
}

void frame(int hor,int vert) {
   int x,y;
   x=wherex();
	y=wherey();
   putch(201);
	printstring(205,hor-2);
   putch(187);
   down(186,vert-2);
	putch(188);
   gotoxy(x,y);
   down(186,vert-2);
   putch(200);
   printstring(205,hor-2);
   gotoxy(x,y+vert+1);
}

void printstring(char c,int n) {
   int i;
   for (i=0;i<n;i++) putch(c);
}

void down(char c,int n) {
   int i;
   for (i=0;i<n;i++) {
		putch('\n');
		putch('\b');
		putch(c);
	}
	putch('\n');
	putch('\b');
}

void read_environment(void) {
	startup_drive=getdisk();
	Trace= (int)set_env_variable("TRACE", 0);
	Fig= (int)set_env_variable("FIG", FIG);
	Header= (int)set_env_variable("HEADER", HEADER);
	Show= (int)set_env_variable("SHOW", SHOW);
	Group= set_env_variable("GROUP", GROUP);
	Kyo= (int)set_env_variable("KYO", KYO);
	Lotus= (int)set_env_variable("LOTUS", LOTUS);
	Lettershift= (int)set_env_variable("SHIFT",LETTERSHIFT);
	Paperlength= (int)set_env_variable("PAPERLENGTH",PAPERLENGTH);
	Pen=(int)set_env_variable("PEN",PEN);
	Font=(int)set_env_variable("FONT",FONT1);
}

float set_env_variable(char *env_string, float default_value) {
   char *s;
	float result;
   s=getenv(env_string);
	if (!s)				result= default_value;
   else {
		if (!stricmp(s,"AUTO"))		result= MISSING;
      if (!stricmp(s,"OFF"))		result= 0;
		if (!stricmp(s,"FALSE"))	result= 0;
		if (!stricmp(s,"ON"))		result= 1;
		if (!stricmp(s,"TRUE"))		result= 1;
		else result= atof(s);
   }
	if (Trace) fprintf(tracefile,"%4d set_env_variable %12s \"%s\" default=%7.2f result=%7.2f\r\n",
      __LINE__, env_string, s, default_value, result);
   return result;
}

char *keycopy(char *target,char *source,int maxlen) {
int t,tt,s,insert=0;
int c;
result=0;
for (t=0;t<maxlen;t++) target[t]='\0';
maxlen--;
c=0;
     for (t=0,s=0;t<maxlen && c!=RETURN;) {
			 c=getkey(1);
          if (c>31 && c<127) {
					putch(c);
               target[t++]=c;
               if (insert==0 && (source[s+1])!='\0') s++;
          } else {
               switch (c) {
               case EXEC:
               case RETURN:
               case W_EXEC:
                    if (!target[0]) {
							  strcpy(target,source);
                       result=RETURN;
						  }
                    else target[t]='\0';
                    break;
               case ESC:
               case CANCEL:
               case BACKTAB:
               case W_BACKTAB:
                    target[0]=0;
                    c=RETURN;
                    break;
               case BACKSPACE:
                    insert=0;
						  if (t>0) {
								 cprintf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
						  break;
               case RIGHT:
               case W_RIGHT:
                    insert=0;
                    if (t<maxlen && source[s]){
								 putch(source[s]);
                         target[t++]=source[s++];
						  }
                    break;
               case DELETE:
               case W_DELETE:
                    source++;
                    insert=0;
                    break;
               case INSERT:
               case W_INSERT:
						  insert=1;
						  break;
					case LEFT:
               case W_LEFT:
                    insert=0;
                    if (t>0) {
								 cprintf("\b \b");
                         t--;
                    }
                    if (s>0) s--;
                    break;
               case F3:
               case W_F3:
                    insert=0;
                    while (t<maxlen && source[s]) {
                         putch(source[s]);
                         target[t++]=source[s++];
                    }    
                    break;
					case F5:
               case W_F5:
                    insert=0;
                    for (tt=0;tt<t;tt++) source[tt]=target[tt];
                    for (t=0;t<maxlen;t++) target[t]='\0';
						  putch('@');
						  putch('\r\n');
						  s=t=0;
                    break;
               case CTRL_C:
						  farewell();
                    break;
               }
          }
     }
     return target;
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

void extra_text_menu(void) {
	int t, response;
   char buffer[MAX_LEN+1];
	for (;;) {
      clrscr();
		cprintf("\Extra text at the indicated positions:\r\n\r\n"
					"      0\r\n"
					"   1     2\r\n\r\n"
					"3\r\n"
					"4\r\n"
					"5\r\n\r\n"
					"   6     7\r\n"
					"      8\r\n\r\n");
		for (t=0;t<9;t++) if (extra_text[t][0]) cprintf("\r\nextra text %d: %s",t,extra_text[t]);
		cprintf("\r\n\r\nRETURN = accept\r\nModify/add which text (0 - 8) : ");
      do {
			response=getkey(0);
         t=response-'0';
      } while (response!=RETURN && (t<0 || t>8));
      if (response==RETURN) break;
		cprintf("%c\r\n%s: ",response,extra_text[t]);
      keycopy(buffer,extra_text[t],MAX_LEN);
      if (isvoid(buffer)) extra_text[t][0]='\0';
      else strcpy(extra_text[t],buffer);
   }
}

void picextra_text(void) {
   int i, x, y, pos, c, vertical=0, dotszin;
   for (i=0; i<9; i++) {
      if (extra_text[i][0]) {
			pos=0;
         dotszin=0;
         do {
              c=extra_text[i][pos];
              if (c>31 && c<127) {
					  if (Font==11) dotszin+=font11[c-32];
                 else dotszin+=font10[c-32];
				  }
              pos++;
         } while (c!=0);
         switch (i) {
            case 0:
					vertical=FALSE;
               x=(X_LENGTH-X_ORIGIN)/2-dotszin/2-X_ORIGIN+POS0;
               y=0;
               break;
				case 1:
					vertical=FALSE;
					x=(X_LENGTH-X_ORIGIN)/4-dotszin/2+POS1;
               y=0;
               break;
            case 2:
					vertical=FALSE;
               x=3*(X_LENGTH-X_ORIGIN)/4-dotszin/2-X_ORIGIN+POS2;
               y=0;
					break;
            case 3:
               vertical=TRUE;
               x=3300-Y_LENGTH/6-dotszin/2+POS3;
               y=80;
					break;
            case 4:
               vertical=TRUE;
               x=3300-Y_LENGTH/2-dotszin/2+POS3;
               y=80;
					break;
            case 5:
               vertical=TRUE;
					x=3300-5*Y_LENGTH/6-dotszin/2+POS3;
               y=80;
					break;
            case 6:
					vertical=FALSE;
               x=(X_LENGTH-X_ORIGIN)/4-dotszin/2+POS1;
               y=Y_LENGTH+180;
               break;
            case 7:
					vertical=FALSE;
               x=3*(X_LENGTH-X_ORIGIN)/4-dotszin/2-X_ORIGIN+POS2;
               y=Y_LENGTH+180;
               break;
				case 8:
					vertical=FALSE;
					x=(X_LENGTH-X_ORIGIN)/2-dotszin/2-X_ORIGIN+POS0;
               y=Y_LENGTH+180;
               break;
			}
			fprintf(fpout,"FONT %d;\r\n",vertical?29:10);
			fprintf(fpout,"MAP %d,%d;\r\nTEXT '%s';\r\n",x,y,extra_text[i]);
		}
	}
}

int isvoid(char *s) {
	int i;
	for (i=0;s[i];i++) if (!isspace(s[i])) return 0;
	return 1;
}


int check_drive(char *s) {
int  result, drive,namestart=0;
char c,tuffer[512];
	if (s[1]==':' && (c=toupper(*s))>'@' && c<'C') namestart=2;
	drive=c-'A';
	if(namestart==2 && drive<2)
	{
	result = biosdisk(4,drive,0,0,0,1,tuffer);
	if(result==128)    {
		  gotoxy(1,13);
	cprintf("Drive %c Not Ready\r\n",drive +'A');
		  return(0);
		}
	}
	return 1;
}

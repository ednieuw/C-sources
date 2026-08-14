/*
	FRACT386 - 386-specific Mandelbrot/Julia Fractal Generator
	C-code				By Bert Tyler/Ed Nieuwenhuys
				
*/

   extern int cputype(void);
   extern int getakey(void);
   extern int setvideomode(int,int,int,int);
   extern int calcdots();
   extern int drawbox(void);

struct videoinfo {		/* All we need to know about a Video Adapter */
	char	name[40];	/* Adapter name (IBM EGA, etc)		*/
	int	videomodeax;	/* begin with INT 10H, AX=(this)	*/
	int	videomodebx;	/*		...and BX=(this)	*/
	int	videomodecx;	/*		...and CX=(this)	*/
	int	videomodedx;	/*		...and DX=(this)	*/
	int	dotmode;	/* video access method used by asm code	*/
				/*	1 == BIOS 10H, AH=12,13 (SLOW)	*/
				/*	2 == access like EGA/VGA	*/
				/*	3 == access like MCGA		*/
	int	xdots;		/* number of dots across the screen	*/
	int	ydots;		/* number of dots down the screen	*/
	int	colors;		/* number of colors available		*/
	char	comment[40];	/* Comments (UNTESTED, etc)		*/
	};

struct videoinfo videomode[] = {
/*
	Feel free to add your favorite video adapter to the following table.
	Just remember that only the first 50 entries get displayed and
	assigned Function keys.

--Adapter/Mode------|---INT 10H---|Dot-|-Resolution-|------Comments-----------
-----Name-----------|-AX--BX-CX-DX|Mode|X-|-Y-|Color|-------------------------
*/
"IBM Low-Rez EGA",   0x0d, 0, 0, 0, 2, 320, 200, 16, "Quick",
"IBM 16-Color EGA",  0x10, 0, 0, 0, 2, 640, 350, 16, "Slower",
"IBM 256-Color MCGA",0x13, 0, 0, 0, 3, 320, 200,256, "Quick, LOTS of colors",
"IBM 16-Color VGA",  0x12, 0, 0, 0, 2, 640, 480, 16, "Nice, high resolution",
"IBM 4-Color CGA",   0x05, 0, 0, 0, 1, 320, 200,  4, "UNTESTED: may not work",
"IBM Hi-Rez B&W CGA",0x06, 0, 0, 0, 1, 640, 200,  2, "UNTESTED: may not work",
"IBM B&W EGA",       0x0f, 0, 0, 0, 2, 640, 350,  2, "(Monochrome EGA)",
"IBM B&W VGA",       0x11, 0, 0, 0, 2, 640, 480,  2, "(Monochrome VGA)",
"IBM Med-Rez EGA",   0x0e, 0, 0, 0, 2, 640, 200, 16, "UNTESTED: may not work",
"ATI EGA Wonder",    0x51, 0, 0, 0, 1, 640, 480, 16, "UNTESTED: may not work",
"ATI EGA Wonder",    0x52, 0, 0, 0, 1, 800, 560, 16, "UNTESTED: may not work",
"Everex EVGA",       0x70, 2, 0, 0, 2, 800, 600, 16, "UNTESTED: may not work",
"Everex EVGA",       0x70,19, 0, 0, 1, 512, 480,256, "UNTESTED: may not work",
"Everex EVGA",       0x70,21, 0, 0, 1, 640, 350,256, "UNTESTED: may not work",
"Paradise VGA",      0x5e, 0, 0, 0, 1, 640, 400,256, "UNTESTED: may not work",
"Paradise VGA",      0x5f, 0, 0, 0, 1, 640, 480,256, "UNTESTED: may not work",
"Paradise VGA",      0x58, 0, 0, 0, 2, 800, 600, 16, "UNTESTED: may not work",
"Paradise EGA-480",  0x50, 0, 0, 0, 1, 640, 480, 16, "UNTESTED: may not work",
"VEGA VGA",          0x2d, 0, 0, 0, 1, 640, 350,256, "UNTESTED: may not work",
"VEGA VGA",          0x5e, 0, 0, 0, 1, 640, 400,256, "UNTESTED: may not work",
"VEGA VGA",          0x2e, 0, 0, 0, 1, 640, 480,256, "UNTESTED: may not work",
"VEGA VGA",          0x27, 0, 0, 0, 1, 720, 512, 16, "UNTESTED: may not work",
"VEGA VGA",          0x2f, 0, 0, 0, 1, 720, 512,256, "UNTESTED: may not work",
"VEGA VGA",          0x29, 0, 0, 0, 1, 800, 600, 16, "UNTESTED: may not work",
"VEGA VGA",          0x30, 0, 0, 0, 1, 800, 600,256, "UNTESTED: may not work",
"VEGA VGA",          0x36, 0, 0, 0, 1, 960, 720, 16, "UNTESTED: may not work",
"VEGA VGA",          0x37, 0, 0, 0, 1,1024, 768, 16, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x60,0,0,1, 752, 410, 16, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x61,0,0,1, 720, 540, 16, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x62,0,0,1, 800, 600, 16, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x63,0,0,1,1024, 768,  2, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x64,0,0,1,1024, 768,  4, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x65,0,0,1,1024, 768, 16, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x66,0,0,1, 640, 400,256, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x67,0,0,1, 640, 480,256, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x68,0,0,1, 720, 540,256, "UNTESTED: may not work",
"Video-7 Vram VGA", 0x6f05,0x69,0,0,1, 800, 600,256, "UNTESTED: may not work",
"AT&T 6300",         0x41, 0, 0, 0, 1, 640, 200, 16, "UNTESTED: may not work",
"AT&T 6300",         0x48, 0, 0, 0, 1, 640, 400,  2, "UNTESTED: may not work",
"AT&T 6300",         0x42, 0, 0, 0, 1, 640, 400, 16, "UNTESTED: may not work",
"END",                  3, 0, 0, 0, 0,   0,   0,  0, "Marks END of the List"
	};

int	maxvideomode;		/* size of the above list */
int	adapter;		/* Video Adapter chosen from above list	*/

char *fkeys[] = {		/* Function Key names for display table */
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
	"SF1","SF2","SF3","SF4","SF5","SF6","SF7","SF8","SF9","SF10",
	"CF1","CF2","CF3","CF4","CF5","CF6","CF7","CF8","CF9","CF10",
	"AF1","AF2","AF3","AF4","AF5","AF6","AF7","AF8","AF9","AF10",
	"Alt-1","Alt-2","Alt-3","Alt-4","Alt-5",
	"Alt-6","Alt-7","Alt-8","Alt-9","Alt-0",
	"END"};
char *accessmethod[] = {" ","B"," "," "}; /* BIOS access indicator */

#define	FUDGEFACTOR	29		/* fudge all values up by 2**this */
#define MAXHISTORY	100		/* save this many historical rcds */

long	history[MAXHISTORY][6];	/* for historical (HOME Key) purposes */

/*
   the following variables are out here only so
   that the assembler routine can get at them easily
*/
	int	dotmode;			/* video access method      */
						/* 1 == use the BIOS (ugh)  */
						/* 2 == access like EGA/VGA */
						/* 3 == access like MCGA    */
	int	xdots, ydots;			/* # of dots on the screen  */
	int	colors;				/* maximum colors available */
	int	maxit;				/* try this many iterations */
	int	ixmin, ixmax, iymin, iymax;	/* corners of the zoom box  */

	int	julia;				/* if == 0, use Mandelbrot  */
	long	creal, cimag;			/* real, imag'ry parts of C */
	long	lx0[2048], ly0[2048];		/* x, y grid (2K x 2K max)  */



int far *SCHERMPTR;
void swrite(int posx,int posy,char * tekst,int kleur);
void twrite(int posx,int posy,char * tekst,int forground,int background);
void veeg(int topy,int topx,int totl,int totb);       /* veeg scherm schoon */
void lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur);
int lijn=0;					/*wel of geen vert lijn*/




main(argc,argv)
int argc;
char *argv[];
{
	long	jxmin, jxmax, jymin, jymax;	/* "Julia mode" entry point */
	long	xmin, xmax, ymin, ymax;		/* screen corner values */
	long	delx, dely;			/* screen pixel increments */
	long	fudge;				/* 2**fudgefactor	*/
	double	atof(),xxmin,xxmax,yymin,yymax;	/* (printf) screen corners */
	double	ccreal, ccimag;			/* Julia Set Constant */
	int	xstep, ystep;			/* zoom-box increment values */
	int	axmode, bxmode, cxmode, dxmode;	/* video mode (BIOS ##) */
	int	historyptr;			/* pointer into history tbl */
	int	zoomoff;			/* = 0 when zoom is disabled */
	int	kbdchar;			/* keyboard key-hit value */
	int	more, kbdmore;			/* continuation variables */
	int	i,j,k;				/* temporary loop counters */
	char aa[100];
	SCHERMPTR=(int far *) 0XB8000000L;	/*pointer voor s/twrite*/


if ((i=abs(cputype())) != 386) {		/* oops. not a 386 */
	printf("\n\n\007");
	printf(" I'm sorry.   This program requires an 80386-based PC, and my\n");
	printf(" poking around has determined that this is more like an 80%d.", i);
	printf("\n\n");
	exit(1);
	}

for (maxvideomode = 0;				/* get size of adapter list */
	strcmp(videomode[maxvideomode].name,"END") != 0
		&& maxvideomode < 50;	/* that's all the Fn keys we got! */
	maxvideomode++);


fudge = 1; fudge = fudge << FUDGEFACTOR;	/* fudged value for printfs */

restart:				/* insert key re-starts here */

julia = 0;					/* use the Mandelbrot set */
creal = 0; cimag = 0;				/* dummy C-values         */
						/* grid for full Mandel set */
xmax =  4; xmax = xmax << (FUDGEFACTOR-2);	/* xmax == 1.0		*/
xmin = -8; xmin = xmin << (FUDGEFACTOR-2);	/* xmin == -2.0		*/
ymax =  6; ymax = ymax << (FUDGEFACTOR-2);	/* ymax == 1.5		*/
ymin = -6; ymin = ymin << (FUDGEFACTOR-2);	/* ymin == -1.5		*/

if (argc == 3 || argc == 7) {			/* Julia Set Override */
	julia = 1;
	ccreal = atof(argv[1]);
	ccimag = atof(argv[2]);
	if (ccreal < -1.99 || ccreal > 1.99) ccreal = .0001;
	if (ccimag < -1.99 || ccimag > 1.99) ccimag = .0001;
	ccreal = ccreal * fudge; creal = ccreal;
	ccimag = ccimag * fudge; cimag = ccimag;
	jxmin = xmin;  jxmax = xmax;  jymin = ymin;  jymax = ymax;
	xmax =  7; xmax = xmax << (FUDGEFACTOR-2);	/* xmax =  1.75 */
	xmin = -7; xmin = xmin << (FUDGEFACTOR-2);	/* xmin = -1.75 */
	ymax =  7; ymax = ymax << (FUDGEFACTOR-2);	/* ymax =  1.75 */
	ymin = -7; ymin = ymin << (FUDGEFACTOR-2);	/* ymin = -1.75 */
	}

if (argc == 5 || argc == 7) {			/* starting grid override */
	xxmin = atof(argv[argc-4]);
	xxmax = atof(argv[argc-3]);
	yymin = atof(argv[argc-2]);
	yymax = atof(argv[argc-1]);
	/* keep it reasonable, guys  -- max-min MUST be < 4 */
	if (xxmin < -2.0  || xxmin > 1.99) xxmin = -2.0;
	if (xxmax < -2.0  || xxmax > 1.99) xxmax =  1.99;
	if (yymin < -1.99 || yymin > 1.99) yymin = -1.99;
	if (yymax < -1.99 || yymax > 1.99) yymax =  1.99;
	xxmin = xxmin * fudge; xmin = xxmin;
	xxmax = xxmax * fudge; xmax = xxmax + 100;
	yymin = yymin * fudge; ymin = yymin - 100;
	yymax = yymax * fudge; ymax = yymax;
	}

setvideomode(3,0,0,0);			/* switch to text mode */

clrscr();
swrite(0,1,"         3DFRACT386    by Ed Nieuwenhuys   idea:Bert Tyler                      ",0x4E00);
swrite(0,3,"Give the maximum number of iterations to be allowed before the",0x0700);
swrite(0,4,"program quits and plots the maximum color",0X700);

swrite(0,6,"Maximum No of iterations : ",0x7400);
gotoxy(28,7);
scanf("%s",&aa);
maxit=atoi(aa);
swrite(0,8,"The program draw a verical line in the color indicated or",0X0700);
swrite(0,9,"only plots the color, which gives another nice effect",0x0700);
swrite(0,10,"0 gives no line",0x0700);
swrite(0,12,"255 gives a incremented color line",0x0700);
swrite(0,13,"Any other color gives that color",0x0700);

swrite(0,15,"Color of vertical line or no line (0) : ",0x7400);
gotoxy(41,16);
scanf("%s",&aa);
lijn=atoi(aa);
clrscr();
swrite(0,1,"                     3DFRACT386    by Ed Nieuwenhuys                           ",0x4E00);
swrite(0,3,"The useful keys you can hit while this program is running are:                 ",0x0400);
swrite(0,4,"  PageUp            Shrink the Zoom Box (Zoom in)                              ",0X7A00);
swrite(0,5,"  PageDown          Expand the Zoom Box (Zoom out)                             ",0X7A00);
swrite(0,6,"  Cursor Keys       Move the Zoom Box Left, Right, Up, or Down (Panning)       ",0X7A00);
swrite(0,7,"  Ctrl-Cursor-Keys  Pan as above, but quickly (may require an Enhanced KBD)    ",0X7A00);
swrite(0,8,"  End or Enter      Redraw (full screen) the area inside the Zoom Box          ",0X7A00);
swrite(0,9,"  F1,F2,F3,F4...    Select a new Video Mode and THEN Redraw                    ",0X7A00);
swrite(0,10,"                    (See Instructions Below for a complete Video Mode list)    ",0X7A00);
swrite(0,11,"  Home              Redraw Previous screen (you can 'back out' recursively)    ",0X7A00);
swrite(0,12,"  Tab               Display the Screen or Zoom-Box X and Y Coordinates         ",0X7A00);
swrite(0,13,"  Spacebar          Mandelbrot/Julia Set toggle (read the README file first)   ",0X7A00);
swrite(0,14,"  Insert            Restart the program (at this screen)                       ",0X7A00);
swrite(0,15,"  Delete or Esc     Stop the program and return to MSDOS                       ",0X7A00);

swrite(0,18,"Hitting any key while the program is drawing will stop the drawing immediately ",0X7C00);
swrite(0,19,"to perform the function.   This means you can begin Zooming and Panning (or    ",0X7C00);
swrite(0,20,"anything else) as soon as you see something interesting.  If you haven't yet   ",0X7C00);
swrite(0,21,"hit a key when the screen is finished, the program will beep and wait for you. ",0X7C00);
swrite(0,22,"Please Select your Video Mode by hitting a Function Key (F1, F2, F3, F4,...)   ",0X7C00);
swrite(0,23,"    (Or hit the <ENTER> key to see a complete list of supported Video modes)   ",0X7C00);
swrite(0,24,"The program will begin with the full Mandelbrot Set as a starting point        ",0X7C00);

j = maxvideomode;				/* init for video modes scan */
for (;;) {					/* wait for valid Fn Key */
	kbdchar = getakey();
	if (kbdchar >= 1059 && kbdchar < 1069	/* F1 - F10 */
	 && kbdchar < 1059 + maxvideomode) {
		adapter = kbdchar - 1059;	/* use this adapter initially */
		break;
		}
	if (kbdchar >= 1084 && kbdchar < 1114	/* SF1 - AF10 */
	 && kbdchar < 1074 + maxvideomode) {
		adapter = kbdchar - 1074;	/* use this adapter initially */
		break;
		}
	if (kbdchar >= 1120 && kbdchar < 1130	/* Alt-1 thru Alt-0 */
	 && kbdchar < 1080 + maxvideomode) {
		adapter = kbdchar - 1080;	/* use this adapter initially */
		break;
		}
	if (kbdchar == 1082) goto restart;	/* restart pgm on Insert Key */
	if (kbdchar == 1000) exit(0);		/* Control-C */
	if (kbdchar == 27) exit(0);		/* exit to DOS on Escape Key */
	if (kbdchar == 1083) exit(0);		/* exit to DOS on Delete Key */
	if (kbdchar == 13) {			/* <ENTER> key: display adapter list */
		clrscr();
		k=6;
		j += 14;			/* display next page */
		if (j >= maxvideomode) j = 0;	/* (or 1st page)     */
		swrite(0,3,"The current list of supported Video Adapters and Modes includes:",0x0700);
		sprintf(aa,"      %-20s   Resolution Colors   %-30s",
			"Video Adapter & Mode", "     Comments");
		swrite(0,4,aa,0x0700);
		for (i = j; i < maxvideomode && i < j + 14; i++) {
			sprintf(aa,"%-5s %-20s%6d x%4d%5d %1s   %-30s",fkeys[i],
				videomode[i].name,
				videomode[i].xdots,
				videomode[i].ydots,
				videomode[i].colors,
				accessmethod[videomode[i].dotmode],
				videomode[i].comment
				);
			swrite(0,k++,aa,0x0200);
			}

swrite(0,20,"    (Hit the <ENTER> key to see another page of supported Video modes)       ",0x0700);
swrite(0,21,"Please Select your Video Mode by hitting a Function Key (F1, F2, F3, F4,...) ",0x0700);
swrite(0,22,"    (Notation:  F1, SF1, CF1, AF1 == Normal-, Shift-, Control- or Alt-F1)    ",0x0700);
swrite(0,23,"    'B' after #-of-colors means video access is via the BIOS (s-l-o-w-l-y)   ",0x0700);
swrite(0,24,"The program will begin with the full Mandelbrot Set as a starting point.     ",0x0700);
		}
	}

historyptr = 0;					/* initialize history ptr */
history[0][0] = -1;
zoomoff = 1;					/* zooming is enabled */

more = 1;
while (more) {					/* eternal loop */

						/* collect adapter info */
	axmode  = videomode[adapter].videomodeax; /* video mode (BIOS call) */
	bxmode  = videomode[adapter].videomodebx; /* video mode (BIOS call) */
	cxmode  = videomode[adapter].videomodecx; /* video mode (BIOS call) */
	dxmode  = videomode[adapter].videomodecx; /* video mode (BIOS call) */
	dotmode = videomode[adapter].dotmode;	/* assembler dot read/write */
	xdots   = videomode[adapter].xdots;	/* # dots across the screen */
	ydots   = (videomode[adapter].ydots);	/* # dots down the screen   */
	colors  = videomode[adapter].colors;	/* # colors available */
	ydots -= maxit;
	xstep   = xdots / 40;			/* zoom-box increment: across */
	ystep   = ydots / 40;			/* zoom-box increment: down */
	if (xdots == 640 && ydots == 350)	/* zoom-box adjust:  640x350 */
		{ xstep = 16; ystep =  9; }
	if (xdots == 720 && ydots == 512)	/* zoom-box adjust:  720x512 */
		{ xstep = 20; ystep = 15; }
	if (xdots == 1024 && ydots == 768)	/* zoom-box adjust: 1024x768 */
		{ xstep = 32; ystep = 24; }

	delx = ((xmax - xmin) / (xdots - 1));	/* calculate the stepsizes */
	dely = ((ymax - ymin) / (ydots - 1));

	if (delx <= 0 || dely <= 0) {		/* oops.  zoomed too far */
		zoomoff = 0;
		dely = 1;
		delx = 1;
		}

	lx0[0] = xmin;				/* fill up the x, y grids */
	ly0[0] = ymax;
	for (i = 1; i < xdots; i++ )
		lx0[i] = lx0[i-1] + delx;
	for (i = 1; i < ydots; i++ )
		ly0[i] = ly0[i-1] - dely;

	if (zoomoff == 0) {
		xmax = lx0[xdots-1];		/* re-set xmax and ymax */
		ymin = ly0[ydots-1];
		}

	if (history[0][0] == -1)			/* initialize the history file */
		for (i = 0; i < MAXHISTORY; i++) {
			history[i][0] = xmax;
			history[i][1] = xmin;
			history[i][2] = ymax;
			history[i][3] = ymin;
			history[i][4] = creal;
			history[i][5] = cimag;
		}

	if (history[historyptr][0] != xmax  ||	/* save any (new) zoom data */
	    history[historyptr][1] != xmin  ||
	    history[historyptr][2] != ymax  ||
	    history[historyptr][3] != ymin  ||
	    history[historyptr][4] != creal ||
	    history[historyptr][5] != cimag ) {
		if (++historyptr == MAXHISTORY) historyptr = 0;
		history[historyptr][0] = xmax;
		history[historyptr][1] = xmin;
		history[historyptr][2] = ymax;
		history[historyptr][3] = ymin;
		history[historyptr][4] = creal;
		history[historyptr][5] = cimag;
		}

	setvideomode(axmode,bxmode,cxmode,dxmode); /* switch video modes */

	if (calcdots() == 0)			/* draw the fractal */
		printf("\007");			/* finished!  wake up! */

	ixmin = 0;  ixmax = xdots-1;		/* initial zoom box */
	iymin = 0;  iymax = ydots-1;

	kbdmore = 1;
	while (kbdmore == 1) {			/* loop through cursor keys */
		kbdchar = getakey();
		switch (kbdchar) {
			case 32:			/* spacebar */
				if (julia == 0) {	/* switch to Julia */
					julia = 1;
					creal = (xmax + xmin) / 2;
					cimag = (ymax + ymin) / 2;
					jxmin = xmin;  jxmax = xmax;
					jymin = ymin;  jymax = ymax;
					xmax =  7; 	/* xmax ==  1.75 */
 					xmin = -7; 	/* xmin == -1.75 */
					ymax =  7; 	/* ymax ==  1.75 */
					ymin = -7; 	/* ymin == -1.75 */
					xmax = xmax << (FUDGEFACTOR-2);
					xmin = xmin << (FUDGEFACTOR-2);
					ymax = ymax << (FUDGEFACTOR-2);
					ymin = ymin << (FUDGEFACTOR-2);
					}
				else	{		/* switch to Mandel */
					julia = 0;
					creal = 0;
					cimag = 0;
					xmin = jxmin;  xmax = jxmax;
					ymin = jymin;  ymax = jymax;
					}
				kbdmore = 0;
				break;
			case 1071:			/* home */
				if (--historyptr < 0)
					historyptr = MAXHISTORY-1;
				xmax  = history[historyptr][0];
				xmin  = history[historyptr][1];
				ymax  = history[historyptr][2];
				ymin  = history[historyptr][3];
				creal = history[historyptr][4];
				cimag = history[historyptr][5];
				julia = 0;
				if (creal != 0 || cimag != 0) julia = 1;
				zoomoff = 1;
				kbdmore = 0;
				break;
			case 9:				/* tab */
				ccreal = creal; ccreal = ccreal / fudge;
				ccimag = cimag; ccimag = ccimag / fudge;
				xxmax = xmax; xxmax = xxmax / fudge;
				xxmin = xmin; xxmin = xxmin / fudge;
				yymax = ymax; yymax = yymax / fudge;
				yymin = ymin; yymin = yymin / fudge;
				if (julia == 0 )
					printf("The Current Mandelbrot Boundaries are: \n");
				else	{
					printf("The Current Julia Set Boundaries are: \n");
					printf("    Creal = %12.9f \n",ccreal);
					printf("    Cimag = %12.9f \n",ccimag);
					}
				printf("     Xmin = %12.9f \n",xxmin);
				printf("     Xmax = %12.9f \n",xxmax);
				printf("     Ymin = %12.9f \n",yymin);
				printf("     Ymax = %12.9f \n",yymax);
				continue;
			case 13:			/* Enter */
			case 1079:			/* end */
				kbdmore = 0;
				break;
			case 1082:			/* insert */
				goto restart;
			case 1000:			/* Control-C */
			case 27:			/* Escape */
			case 1083:			/* delete */
				more = 0; kbdmore = 0;
				break;
			case 1075:			/* cursor left */
				if (zoomoff == 1 && ixmin >= 1)
					{ ixmin -= 1; ixmax -= 1; }
					break;
			case 1077:			/* cursor right */
				if (zoomoff == 1 && ixmax < xdots - 1)
					{ ixmin += 1; ixmax += 1; }
					break;
			case 1072:			/* cursor up */
				if (zoomoff == 1 && iymin >= 1)
					{ iymin -= 1; iymax -= 1; }
					break;
			case 1080:			/* cursor down */
				if (zoomoff == 1 && iymax < ydots - 1)
					{ iymin += 1; iymax += 1; }
					break;
			case 1115:			/* Ctrl-cursor left */
				if (zoomoff == 1 && ixmin >= 5)
					{ ixmin -= 5; ixmax -= 5; }
					break;
			case 1116:			/* Ctrl-cursor right */
				if (zoomoff == 1 && ixmax < xdots - 5)
					{ ixmin += 5; ixmax += 5; }
					break;
			case 1141:			/* Ctrl-cursor up */
				if (zoomoff == 1 && iymin >= 5)
					{ iymin -= 5; iymax -= 5; }
					break;
			case 1145:			/* Ctrl-cursor down */
				if (zoomoff == 1 && iymax < ydots - 5)
					{ iymin += 5; iymax += 5; }
					break;
			case 1073:			/* page up */
				if (zoomoff == 1
				  && ixmax - ixmin > 3 * xstep
				  && iymax - iymin > 3 * ystep) {
					/* 640x350 Zoom-In Klooge:  Adjust the
					   Zoom-Box on the initial Zoom-In */
					if (xdots == 640 && ydots == 350
					  && iymin == 0  && iymax == ydots-1) {
						iymin -= 5;
						iymax += 5;
						}
					/* 720x512 Zoom-In Klooge:  Adjust the
					   Zoom-Box on the initial Zoom-In */
					if (xdots == 720 && ydots == 512
					  && iymin == 0  && iymax == ydots-1) {
						iymin -= 14;
						iymax += 14;
						}
					ixmin += xstep;	ixmax -= xstep;
					iymin += ystep;	iymax -= ystep;
					}
					break;
			case 1081:			/* page down */
				if (zoomoff == 1
				 && ixmin >= xstep && ixmax < xdots - xstep
				 && iymin >= ystep && iymax < ydots - ystep) {
					ixmin -= xstep;	ixmax += xstep;
					iymin -= ystep;	iymax += ystep;
					}
					break;
			default:		/* other (maybe a valid Fn key) */
				if (kbdchar >= 1059 && kbdchar < 1069
				 && kbdchar < 1059 + maxvideomode) {
					adapter = kbdchar - 1059;
					kbdmore = 0;
					}
				else if (kbdchar >= 1084 && kbdchar < 1114
				 && kbdchar < 1074 + maxvideomode) {
					adapter = kbdchar - 1074;
					kbdmore = 0;
					}
				else if (kbdchar >= 1120 && kbdchar < 1130
				 && kbdchar < 1080 + maxvideomode) {
					adapter = kbdchar - 1080;
					kbdmore = 0;
					}
				else
					continue;
				break;
			}

		if (zoomoff == 1 && kbdmore == 1) {	/* draw a zoom box? */
			xmin = lx0[ixmin];
			xmax = lx0[ixmax];
			ymin = ly0[iymax];
			ymax = ly0[iymin];
			drawbox();			/* (yup) */
			}
		}

	}

setvideomode(3,0,0,0);			/* all done.  return to text mode. */

}

/***********************  write text  ****************/

void swrite(int posx,int posy,char * tekst,int kleur)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;
   for(;tekst[x];*(nop+x) = tekst[x++] | kleur);
}

void twrite(int posx,int posy,char * tekst,int forground,int background)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;
   unsigned int kleur = (background<<11) + (forground<<8);
   for(;tekst[x]; *(nop+x) = tekst[x++] | kleur);
}



void veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
         *(SCHERMPTR + y*80 + x) = 0x20 | 0x0700;
}

void lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur)
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
		 *(SCHERMPTR + y*80 + x) = kar | kleur;
}


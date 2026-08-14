/* ANSI.H - header for library functions in ANSLIB.C 			*/
/*									*/
/* Contains macro's for common console functions, using the extended 	*/
/*	video driver. This should be loaded at system startup.		*/
 
#define SPACE   printf("\n")		/* blank line */
#define BACK    printf("\b")		/* backspace */
#define DEL     printf("\b \b")		/* destructive backspace */
#define BELL    printf("\007")		/* terminal beep */
#define HOME    printf("\033[H")	/* cursor home */
#define END     printf("\033[24;79H")	/* cursor home down */
#define CUU     printf("\033[A")	/* cursor up */
#define CUD     printf("\033[B")	/* cursor down */
#define CUR     printf("\033[C")        /* cursor right */
#define CUL     printf("\033[D")        /* cursor left */
#define SCP	printf("\033[s")	/* save cursor position */
#define RCP	printf("\033[u")	/* restore cursor position */
#define CLEAR   printf("\033[2J")	/* clear screen */
#define CLINE   printf("\033[K")        /* clear line */
#define STATUS  printf("\033[25;0H")	/* cursor to status line */
#define V_OFF   printf("\033[0m")	/* video attributes off */
#define V_BOLD  printf("\033[1m")  	/* video high intensity */
#define V_BLINK printf("\033[5m")       /* video blink */
#define V_INV   printf("\033[7m")	/* inverse video */

/* Miscellaneous useful macro's */

#define TRUE  1
#define FALSE 0
#define EQ    ==
#define NE    !=
#define GT    >
#define LT    <
#define GE    >=
#define LE    <=

#define abs(x) ((x)<0?-(x):(x))
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<=(b)?(a):(b))


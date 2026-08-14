/* corr.H   18/09/88 */

#define TRUE 1
#define FALSE 0

#define LOAD     0
#define CHADIR   1
#define VIEW     2
#define PRINT    3
#define CALC     4
#define PRNONOF  5
#define LIMIT    6
#define SPEARMAN 7
#define PLAATJE  8
#define NPLAATJE 9
#define MAXCORR 10
#define LIJN    11
#define LIJNXY  12
#define CONF    13
#define THREED  14
#define QUIT    15

#define MOVE    160
#define DRAW    162
#define FILL     48
#define FILLO   208
#define COLOR0  176
#define COLOR1  177
#define COLOR2  178
#define COLOR3  179
#define COLOR4  180
#define COLOR5  181
#define COLOR6  182
#define COLOR7  183
#define COLOR8  184
#define COLOR9  185
#define COLOR10 186
#define COLOR11 187
#define COLOR12 188
#define COLOR13 189
#define COLOR14 190
#define COLOR15 191
#define TEXT    168
#define FONT    167
#define SIZE    172
#define END       6
#define LX     3300              /* Lengte papier 11' in dots */
/*
#define SPECIAL_KEY 0
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
#define SPACE  32
#define RETURN 13
#define TAB     9
*/

#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define H_SCREEN 4000/320     /* screen unit per pixel */
#define V_SCREEN 3000/200
#define H_OFFSET 700          /* offset linker onderhoek box plaatje */
#define V_OFFSET 450
#define H_FONT 8
#define V_FONT 8
#define H_SCALE 0.9           /* factor lengte en breedte box t.o.v. */
#define V_SCALE 0.8           /* overgebleven ruimte na offset */
#define H_WIDTH 4000          /* lengte en breedte lotus graphics screen */
#define V_WIDTH 3000
#define MAX_LEN 40
#define MISSING -999999.0


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
#define F1  315
#define F2  316
#define F3  317
#define F4  318
#define F5  319
#define F6  320
#define F7  321
#define F8  322
#define F9  323
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
// uit corrcomm
//double getd(void *fp);
extern void display_labels();
extern void teken(double xp, double yp);
extern void moef(double xp, double yp);
extern void point(double xp,double yp);
extern void numm(double xp, double yp,int number);
extern void prnt(double xpos,double ypos,int size,int richting,char *tekst);
extern void prntc(double xpos,double ypos,int size,int richting, char *tekst);
extern void picopen();
extern void picmove(int x,int y);
extern void picfont(char font);
extern void piccolor(int color);
extern void picsize(int x,int y);
extern void picdraw(int x,int y);
extern void pictext(int direction,int position,char *s);
extern void picend();
extern void picxy(int x,int y);
extern void picbox(int ox,int oy,int lx,int ly);
extern void display_data();
extern void read_wks(char *infile);
//void skip(void *fp, long noofbytes);


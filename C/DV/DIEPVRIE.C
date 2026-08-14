/* lotus 2 logit   EJN*/

/* l.prj file
l
cga.obj
herc.obj
egavga.obj
litt.obj
Compile in LARGE*/

#include "alloc.h"
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
#include "bios.h"

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
#define SPACE  32
#define RETURN 13
#define TAB     9
#define CHBIN 0xF000    /* Blink Invers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */

#define EDIT    0
#define LOAD    1       /* hoofdmenu items */
#define OVERZICHT 2
#define NEW     3
#define VOEGTOE 4
#define PRNONOF 5
#define CHADIR  6
#define EDITOR  7
#define QUIT    8

#define CHECKCODE 22345

int AUTOMENU = FALSE;
static char titel[QUIT+1][breedte_menu];

#define BARCOLOR 0X2400

#define LOTUS1 0X0404
#define LOTUS2 0X0405
#define BELL 7
#define MAXROWS 200             /* maximale dimensies worksheet */
#define MAX_LEN 40
#define MAX_LEN_NAAM     40      /* Lengte van de naam column */
#define MAX_LEN_MEMO     40      /* Lengte van de naam column */
#define MAX_LEN_PLAATS   30      /* Lengte van de plaats column */
#define MAX_LEN_STATUSNR 13      /* Lengte van de status column */
#define MAX_LEN_DATUM     7      /* Lengte van de telnr column */

#define MISSING -999999.0

#define DATUM __DATE__
int MAX_ROW;

struct ffblk ffblk;
unsigned disk;
char *directory;
char curdir[132];
int size;
int show;


struct vak { 
	     char naam[MAX_LEN_NAAM];
	     char gebdatum[MAX_LEN_DATUM];
	     char statusnr[MAX_LEN_STATUSNR];
	     char datum_1e_afn[MAX_LEN_DATUM];
	     long afnames;
	     char zkhs[MAX_LEN_NAAM];
	     long doosnr;
	     char memo[MAX_LEN_MEMO];
	     int check;
	   };

struct vak **wie;

int lost,print,LXG,LYG,berekend;
int rows, columns;
int corrupted=FALSE;

FILE *fp;
FILE *fpin;
FILE *fpout;
FILE *printer;
char filename[12];     /* naam voor de punt van de ingelezen file */
char file[30];
char filein[40];
char fileout[40];
char fileload[40];
char outfile[20];
int maindisk;
char maindir[40];
char drawdir[40];
char dir[40];
int geladen=0;
int wis=1;
/*
int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;     /* for palette info			*/
*/

int  far *SCHERMPTR;
unsigned char far *video;
unsigned char far *vid;
int far *PTR  = (int far *) 0XB800L;
int SIZE,FONT; 		                  /* wordt gebruikt in graphbody() */
int MaxX, MaxY = 0;
float xnop,ynop;

/****************************  FUNCTION PROTOTYPES ***************/
void   set_prn_filename(char *);
int    display_menu(int ,int );
int    menuver(int ,int ,int ,int, int );
int    load_menu();
void   makedate(int *, int *, int *,unsigned );
char   *filedir(char *);
void   changedir(char *);
int    parse(char *);
void   box(int ,int ,int ,int ,int ,int );
void   swrite(int ,int ,char * , int );
void   twrite(int ,int ,char *,int ,int );
void   initdisplay();
void   veeg(int ,int ,int ,int );
void   lveeg(int ,int ,int ,int ,char,unsigned );
void   prntest();
char   *remove_lt_space(char *);
double getd(FILE *);
void   read_wks(char *);
void   skip(FILE *,long );
void   prnonoff(void);
void   Initialize(void);
void   mallocdata(void);
void   freemallocdata(void);
void   readprn(char *);
void   edit_fields(void);
int    kiesnaam(char *text);
int    save_data(void);
int    load_data(void);
int    display_persoon(int pers);
int    persoon_editen(void);
void   prnt_pers_id(int pers);
void   wissen(int pers);
void   delete_entry(int pers,int dagnr);
void   overzicht(void);
void   print_overzicht(int pers);
void   csort(int n);
/************************************************************/


/************************** MAIN **************************/

main(int argc,char **argv)
{
char *password;
char fnop[15];
int k,actie;
char ext[12],filenop[20];
printer = fopen("LPT1","w");
lost     = FALSE;
print    = TRUE;
show     = FALSE;
LXG      = TRUE;
LYG      = FALSE;
berekend = FALSE;
strcpy(outfile,"");
fileload[0] = 0;
filenop[0] = 0;
actie=EDIT;
MAX_ROW=MAXROWS;

clrscr();
initdisplay();
normvideo();
prntest();

if (argv[1][0]=='?')
 {
  swrite(1,0,"Start het programma op met DIEPVRIE [1] [2],waarin, [1] een optionele ",0X0700);
  swrite(1,1,"optionele filenaam is met de extensie .DVR of .$$$         ",0X0700);
  swrite(1,4,"[1] of [2] kan ook het maximaal aantal regels zijn dan kan   ",0X0700);
  swrite(1,5,"worden ingelezen. Dit kan handig zijn bij te weinig geheugen.",0X0700);
  swrite(1,6,"Coreleft geeft aan hoeveel ruimte er nog vrij is aan geheugen.",0X0700);
  swrite(1,7,"Elke toegevoegde regel neemt 25 bytes in.                    ",0X0700);
  swrite(1,8,"Het maximaal aantal in te lezen personen staat onderin beeld ",0X0700);
 swrite(1,16,"Een PRC file is de ASCII file van dit bestand                ",0X0700);
 swrite(1,22,"Met Editor wordt de PRC of overzichts file geladen.          ",0X0700);
 swrite(1,23,"In de PRC file kunnen dan bijv. regels gewist worden.        ",0X0700);
 swrite(1,24,"Druk F7 om hier uit te komen.                                ",0X0700);
 getch();
 exit(1);
 }

if(argc==2) /* als een file meegegeven in commandline */
   {
   if(argv[1][0]<58) { MAX_ROW=atoi(argv[1]); ext[0]=0;}
    else
     {
     strcpy(filenop,argv[1]);
     strcpy(filenop,strtok(filenop,"."));
     strcpy(ext,strupr(strtok(NULL,".")));
     if(stricmp(ext,"DVR")==0 || stricmp(ext,"$$$")==0)
	 {actie=LOAD; AUTOMENU = TRUE; strcpy(fileload,argv[1]);}
     else if(stricmp(ext,"PRT")==0)
	 {actie=EDIT; AUTOMENU = TRUE; strcpy(fileload,argv[1]);}
    }
   }
if(argc==3) /* als een MAXROW is meegegeven in commandline */
    {
     if(argv[2][0]<58) MAX_ROW=atoi(argv[2]);
    }

mallocdata();
strcpy(curdir,"?:\\");
curdir[0] = 'A'+getdisk();
maindisk = getdisk();
getcurdir(0,curdir+3);
if(curdir[(int)strlen(curdir)-1] != 92) strcat(curdir,"\\");
strcpy(maindir,(char *)curdir);

while(1)
   {
   wis = 1;
   if(!AUTOMENU)
	 actie=display_menu(wis,actie); /* Als geen filenaam in commandline */
   if (actie == -1) actie=QUIT;
   switch (actie)
      {
	   case EDIT:
		  if(!geladen)  break;
		  veeg(0,0,25,80);
		  lost =FALSE;
		  veeg(0,0,25,80);
		  edit_fields();
		  AUTOMENU = FALSE;
		  actie = EDIT; 
		  break;

	   case LOAD:
		  veeg(0,0,25,80);
		  lost = FALSE;
		  if(!AUTOMENU) strcpy(fileload,(char *)filedir("*.DVR"));
		  geladen=load_data();
		  if (geladen)
		    {
		     strcpy(fnop,fileload);
		     strcpy(filename,strtok(fnop,"."));
		    }
		  veeg(0,0,25,80);
		  AUTOMENU = FALSE;
		  actie = EDIT; 
		  break;

	   case OVERZICHT:
		  if(!geladen) break;
		  veeg(0,0,25,80);
		  strcpy(file,fileload);
		  overzicht();
		  AUTOMENU = FALSE;
		  actie=EDIT;
		  break;

	   case NEW:
		  veeg(0,0,25,80);
		  lost =FALSE;
		  password = getpass("Input a password:");
		  if(stricmp(password,"eddie")!=0) break;
		  freemallocdata();
		  mallocdata();
		  if(!AUTOMENU) strcpy(fileload,(char *)filedir("*.PRC"));
		  if (strlen(fileload) < 2) break;
		  strcpy(file,fileload);
		  berekend  = 0;
		  geladen = 1;
		  veeg(0,0,25,80);
		  strcpy(fnop,fileload);
		  strcpy(filename,strtok(fnop,"."));
		  
		  readprn(fileload);
		  strcpy(outfile,fileload);
		  AUTOMENU = FALSE;
		  actie = EDIT; 
		  break;

	   case VOEGTOE:
		  if(!geladen)  break;
		  persoon_editen();
		  AUTOMENU = FALSE;
		  actie=VOEGTOE;
		  break;

	   case CHADIR:
		  veeg(24,0,1,80);
		  swrite(13,22,"RETURN or New directory: \0",0x1300);
		  gotoxy(39,23);
		  gets(dir);
		  if (strlen(dir) >1) changedir(dir);
		  strcpy(curdir,"?:\\");
		  curdir[0]='A'+getdisk();
		  getcurdir(0,curdir+3);
		  if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
		  veeg(23,0,2,80);
		  AUTOMENU = FALSE;
		  actie=EDIT;
		  if(geladen) actie=OVERZICHT;
		  break;

	   case PRNONOF:
		  if(print)           print = FALSE;
		    else {prntest();  print = TRUE;}
		  AUTOMENU = FALSE;
		  actie=EDIT;
		  if(geladen) actie=OVERZICHT;
		  break;

		case EDITOR:
/*		  strcpy(file,fileload);*/
		  strcpy(drawdir,searchpath("pe.exe"));
		  if(spawnlp(P_WAIT,drawdir,"pe.exe",outfile,NULL)==-1)
		   {
		     veeg(0,0,25,80);
		     puts("\n\n\n\n\n      NOT ENOUGH MEMORY FOR EDITOR OR EDITOR NOT FOUND\0");
		     delay(3000);
		    }
		  AUTOMENU = FALSE;
		  actie=EDIT;
		  if(geladen) actie=OVERZICHT;
		  break;

	   case QUIT:
		 default:
		  if(geladen) save_data();
		  veeg(0,0,25,80);
		  setdisk(maindisk);
		  chdir(maindir);
		  printf("%s\n",maindir);
		  freemallocdata();
		  exit(1);
		  break;
      }
  }
}
/**********************  EINDE MAIN ***********************/

int display_menu(int wis,int barkeuze)
{
   int num,nr,breed,keuze;
   char datum[12],nop[50],nopp[30];
   num=load_menu();                   /* laad tekst menu */
   if(wis) lveeg(0,0,25,80,'°',0x2500);
   strcpy(datum,DATUM);
   sprintf(nop,"Farcoreleft %lu bytes",farcoreleft());
   sprintf(nopp,"  Max. personen %d \0",MAX_ROW);
   strcat(nop,nopp);
   swrite(2,24,nop,0x2100);

   twrite(2,20,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\0",YELLOW,RED);
   twrite(2,21,"ºProblemen? Ed Nieuwenhuys   º\0",YELLOW,RED);
   twrite(2,22,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\0",YELLOW,RED);


   swrite(17,0,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\0",0x0600);
   swrite(17,1,"º Cryodata                   º\0",0x0600);
   swrite(17,2,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\0",0x0600);
   swrite(34,1,datum,0x0600);
   swrite(17,3,curdir,0x0700);
   swrite(50,6,fileload,0x0700);
   if(lost) swrite(0,22,"Database te klein om iedereen in te lezen",0X0700);
   gotoxy(1,1);
   breed=breedte_menu;
   keuze = menuver(num,5,17,breed-5, barkeuze);
   if (keuze==QUIT || keuze==-1) veeg(0,0,20,80);
   return(keuze);

}


int menuver(int num,int ltopy,int ltopx,int breed, int barkeuze)
{
   long secs_now;
   char *str_now;
   char s[33];
   int x, y, code, code1, oke, yy, xx, keer,leave,keus;
   int keuze;
   char ch;

   yy = ltopy;
   xx = ltopx;
   oke = 0;
   keuze = barkeuze;
   box(yy,xx,num,breed,1,0);

   while (1)
	 {
	 yy=ltopy;
	 for (y=0; y<num; y++)
	  {
	   yy++;
	   xx=ltopx+1;
	   for (x=0; x<strlen(titel[y]); x++)
		{
		xx++;
		ch = titel[y][x];
		if (y==keuze)   *(SCHERMPTR + yy*80 + xx) = ch | 0x7000;
		else
		  if (x==0)     *(SCHERMPTR + yy*80 + xx) = ch | 0x1300;
			else    *(SCHERMPTR + yy*80 + xx) = ch | 0x4700;
		 }
		}
	  if(oke == 1)		return(keuze);
	  while(bioskey(1)==0)
		{
		 time(&secs_now);
		 str_now=ctime(&secs_now);
		 xx=55;
		 str_now[24]='\0';
		 swrite(xx,0,str_now,0x1300);
		 }
	   code = getch();
	   code1=SPECIAL_KEY;
	   if(code==SPACE) {code1=SPACE; code=SPECIAL_KEY;}
	   if(code == SPECIAL_KEY)
            {
	     if(code1 !=SPACE)  code1 = getch();
	     switch (code1)
	       {
			   case UP:
			   case LEFT:    --keuze;         break;
			   case SPACE:
			   case DOWN:
			   case RIGHT:   ++keuze;         break;
			   case HOME:
			   case PgUp:      keuze = 0;     break;
			   case ENDT:
			   case PgDn:      keuze = num-1; break;
               }
			   if(keuze < 0)   keuze = num-1;
			   if(keuze > num-1)   keuze = 0;
	     }
	   else
		{
		  ch = code;  keus=keuze;  leave=0;   keer=-1;
		  while(keer<2)
		   {
		    keer++;
		    if(keer==1) { keus=-1;}
		    for(y=keus+1; y<num; y++)
		     {
		      if(strnicmp(&titel[y][0],&ch,1) == 0)
			{ keuze = y; oke = 0; leave=1;  keer=2;  }
		      if(leave) break;
		     }
		    }
		  oke = 1;
		  if(ch == ESC) {    keuze = -1;   oke = 1;   }
		}
	}
}

/********************** LOAD MENU ********************************/
int load_menu()
{
   int nr=QUIT+1;
	  strcpy(titel[0],"Schrijf uren af           \0");
	  strcpy(titel[1],"Laad een afdeling         \0");
	  strcpy(titel[2],"Maak overzichtslijst      \0");
	  strcpy(titel[3],"@Laad PRC file            \0");
	  strcpy(titel[4],"Wissen/toevoegen/editen   \0");
	if (print)
	  strcpy(titel[5],"Print ON/off              \0");
	else
	  strcpy(titel[5],"Print on/OFF              \0");
	  strcpy(titel[6],"Andere drive / directory  \0");
	  strcpy(titel[7],"Tekst editor              \0");
	  strcpy(titel[8],"Quit                      \0");
  return(nr);
 }



/***************** haal datum uit ffblk.ff_fdate ************************/
void makedate(int *d, int *m, int *y,unsigned dosdate)
{
 *m = (dosdate >>5)&0XF;
 *d = dosdate & 0x1F;
 *y = (dosdate >>9) & 0X3F;
 *y += 80;
 }


/* Directory listen op scherm en keuze maken ****************************** */
/*                                                                          */
/* char dir 			zoek sleutel directory                      */
/*                                                                          */
/* ************************************************************************ */

char *filedir(char *filein)
{
   char nop[30];
   int y,leave;
   static char filenaam[250][30];   /* nr,breedte filenaam + datum*/
   char ch;			    /* te printen char */
   int begin = 0;                   /* 1e file uit blok van 25 dat geprint word */
   int keuze = 0;		    /* nummer gekozen file */
   int num = 0;                     /* aantal files */
   int oke;                         /* 0 = geen keuze ESC 1 = keuze RETURN */
   int toets;			    /* toetsaanslag via getch() */
   int pos = 0;			    /* positie binnen filenaam */
   int veld;			    /* nummer van filenaam */
   int x = 0;			    /* teller */
   int xx =0;                       /* row waar geprint moet worden */
   int yy = 0;                      /* colom waar geprint moet worden */
/*   int maxaantal = 110; */        /* aantal files per beeldscherm */
   int keus=0;
   int keer=0;
   int day,month,year;

   oke = findfirst((char *)filein,&ffblk,0);
   while (!oke)
      {
       sscanf(ffblk.ff_name,"%s", &nop);
       makedate(&day,&month,&year,ffblk.ff_fdate);
       sprintf(filenaam[x],"%12.12s %2.2d-%2.2d-%2.2d",nop,day,month,year);
       if(++x == 250)
	{
	 sound(100);
	 nosound();
	 sound(1000);
	 swrite(10,10,"Niet alle files ingelezen\0",0x0200);
	 swrite(10,11,"Max. is 250 files\0",0x0200);
	 delay(50);
	 nosound();
	 sleep(3);
	 break;
	}
      oke = findnext(&ffblk);
      }
   num = x;
   oke = 0;

   box(0,6,(num/3)+1,69,2,1);

   while (1)
    {
    for(veld = begin; veld < begin + num; veld++)
     {
      yy = 1 + (veld - begin) / 3;
      xx = 8 + (((veld - begin) % 3) * 22);
      if(veld < num)
       {
       for(pos = 0; pos < 21; pos++, xx++)
	{
	 if(pos < strlen((char *)filenaam[veld]) )
	       ch = (filenaam[veld][pos]);
	 else  ch = SPACE;
	    if (veld == keuze) *(SCHERMPTR + yy * 80 + xx) = ch | CHHIG;
	    else               *(SCHERMPTR + yy * 80 + xx) = ch | CHINV;
	 }
	}
	 else
	   for(pos = 0; pos < 21; pos++, xx++)
			       *(SCHERMPTR + yy * 80 + xx) = SPACE | CHINV;
     }
    if(oke > 0)
      {
	veeg(0,6,25,69);
	if(oke==1) return((char*)"");
	 else
	  {
	   filenaam[keuze][12]=0;
	   return((char*)remove_lt_space(filenaam[keuze]));
	  }
      }
    toets = getch();
    if(toets == SPECIAL_KEY)
     {
      toets = getch();
      switch (toets)
	{
	   case UP:      keuze -= 3;      break;
	   case LEFT:  --keuze;           break;
	   case DOWN:    keuze += 3;      break;
	   case RIGHT: ++keuze;           break;
	   case HOME:    keuze = 0;       break;
	   case ENDT:    keuze = num - 1; break;
	}
      if(keuze < 0)          keuze = num - 1;
      if(keuze > num - 1)    keuze = 0;
/*    if(keuze > num - 1)    keuze = num - 1;
*/    if(keuze < 25)         begin = 0;
      else                   begin = ((keuze - 20) / 3 ) * 3;
     }
     else
      {
       ch = toets;
       keus=keuze;
       leave=0;
       keer=-1;
       while(keer<2)
	 {
	   keer++;
	   if(keer==1) { keus=-1;}
	     for(y=keus+1; y<num; y++)
	      {
	       if(strnicmp(&filenaam[y][0],&ch,1) == 0)
		  {
		    keuze = y;  oke = 0; leave=1;  keer=2;
		    if(keuze < 25)  begin = 0;
		    else  begin = ((keuze - 20) / 3 ) * 3;
		  }
	       if(leave) break;
	       }
	  }

    if(ch == RETURN)   oke = 2;
    if(ch == ESC) { keuze=-1;   oke = 1;}
   }
  }
}

/************************** CHANGE DIR *****************************/
void changedir(char *filein)
{
if ( parse((char *)filein) )
     {
      swrite(5,23,"Illegal drive input ",0x1300);
      sleep(1);
     }
else setdisk(disk);
if(chdir(directory))
     {
       swrite(5,23,"Illegal dir input ",0x1300);
       sleep(1);
     }
}

int parse(char *ptr)
{
 if ( ptr[1] == ':')
   {
	disk = (unsigned) toupper( ptr[0]) - 'A';
	if (disk > MAXDISK)         return(-1);
    directory = ptr+ 2;
   }
 else    {   disk=getdisk();     directory=ptr;     }
if (*directory=='\0') directory=".";
return(0);
}

/* printen box op het scherm ********************************************** */
/*                                                                          */
/* int topy			row waar box geprint moet worden            */
/* int topx			col waar box geprint moet worden            */
/* int lang			lengte box                                  */
/* int breed		        breedte box                                 */
/* int type			1 = enkel  2 = dubbel                       */
/* int kleur		        0 = norm  1 = invers  2 = high              */
/*                                                                          */
/* ************************************************************************ */

void box(int topy,int topx,int lang,int breed,int type,int kleur)
{
   int x;			/* teller */
   int y;                       /* teller */
   int z;                       /* teller */
   unsigned int ch;		/* horizontale streep */
   unsigned int ck;		/* kleur die geprint moet worden */
   unsigned int clb;		/* hoek links boven */
   unsigned int crb;            /* hoek rechts boven */
   unsigned int clo;            /* hoek links onder */
   unsigned int cro;            /* hoek rechts onder */
   unsigned int cv;             /* verticale streep */

   switch (type)
      {
      case 1:
         clb = 'Ú'; crb = '¿';
         clo = 'À'; cro = 'Ù';
         ch  = 'Ä';  cv  = '³';
         break;
      case 2:
         clb = 'É'; crb = '»';
         clo = 'È'; cro = '¼';
         ch  = 'Í';  cv  = 'º';
         break;
      }
   switch(kleur)
      {
      case 0: ck = CHNOR; break;
      case 1: ck = CHINV; break;
      case 2: ck = CHHIG; break;
      }

   z = 0;
   PTR = SCHERMPTR + (topy + z) * 80 + topx ;
				       *(PTR)               = clb | ck;
   for(x = 1; x < breed - 1; x++)      *(PTR + x)           = ch  | ck;
				       *(PTR + (breed - 1)) = crb | ck;
   for(x = 0; x < lang; x++)
      {
      z = x + 1;
	  PTR = SCHERMPTR + (topy + z) * 80 + topx ;
					   *(PTR)               = cv | ck;
	  for(y = 1; y < breed - 1; y++)   *(PTR + y)           = SPACE | ck;
					   *(PTR + (breed - 1)) = cv | ck;
      }
	z = lang + 1 ;
	PTR = SCHERMPTR + (topy + z) * 80 + topx ;
				       *(PTR)               = clo | ck;
   for(x = 1; x < breed - 1; x++)      *(PTR + x)           = ch  | ck;
				       *(PTR + (breed - 1)) = cro | ck;
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
   unsigned int kleur = (background<<12) + (forground<<8);
   for(;tekst[x]; *(nop+x) = tekst[x++] | kleur);
}

/********************** init display *********************/
void initdisplay(void)
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int  *far)( (reg.h.al == 7) ? 0XB0000000L : 0XB8000000L);
}

void veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
   int y, x;
   for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
		 *(SCHERMPTR + y*80 + x) = SPACE | 0x0700;
}

void lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur)
{
/*   int y, x;
   unsigned k1 = 0x0000 +kleur;
   unsigned k2 = 0x0100;
   for(y=topy; y<topy+totl; y++)
	{
	  k1+=0x0100;
	  kleur = k1 + k2;
	  for(x=topx; x<topx+totb; x++)
	   {
	   kleur += k2;
		  if (((kleur& 0x8000)-0x8000)==0)  kleur = 0x0000;
 		 *(SCHERMPTR + y*80 + x) = kar | kleur;
	   }
	 }
*/
   int y, x;
   for(y=topy; y<topy+totl; y++)
	  for(x=topx; x<topx+totb; x++)
		 *(SCHERMPTR + y*80 + x) = kar | kleur;

}


/**************** PRINTER TEST   EJN 040388   ********************
Dit programma test of de printer op printen staat en blijft dit
testen totdat negen pogingen zijn ondernomen
*/
void prntest(void)
{
int i,j,n,count;

count=0;
while(count<9)
 {
  count++;
  n= biosprint(2,0,0);
  if( n == 0x90) break;
  nosound();
  sound(9000);
  veeg(0,0,25,80);
  if (count>3)
     {
       printf("\n**********************************\n"
	      "*       It does not work        *\n"
	      "*        CALL FOR HELP          *\n"
	      "*********************************\n");
	  nosound();
     }
  printf("\n\n\n\nERROR %x ",n);
  if ((n & 0x01) ==0x01)
	     puts("\n         Printer Device time out error");
  if ((n & 0x08) ==0x08)
	     puts("\n         Printer I/O error");
  if ((n & 0x10) == 0x10)
	     puts("\n         Printer not ONLINE");
  if ((n & 0x20) == 0x20)
	     puts("\n         Printer Out of paper");
  if ((n & 0x40) == 0x40)
	     puts("\n         Printer is OFF");
  if ((n & 0x80) == 0x80)
	     puts("\n         Printer NOT BUSY");
  puts("Press a key after solving the error");
  while(bioskey(1)==0);
  getch();
  nosound();
  }
 }

/************  remove kleiner dan spatie ***************/
char *remove_lt_space(char *inputline)
{
int n;
char nop[1024];
strcpy(nop,strrev(inputline));
for(n=strlen(nop);n>=0;n--)	{ if(nop[n]<=' ') nop[n]=0; else break;}
strcpy(inputline,strrev(nop));
for(n=strlen(inputline);n>=0;n--)
	 { if(inputline[n] <= ' ') inputline[n]=0; else break;}
return(inputline);
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



/******************** TEST OF PRINTER AAN OF UIT ************************/
void prnonoff(void)
{
 if(biosprint(2,0,0)  != 0x90)
 {
  sound(2200);
  delay(13);
  nosound();
  sound(200);
  delay(300);
  nosound();
  closegraph();
  exit(-1);
 }
/* Verlaat het programma als de printer uitstaat*/
 }

/*************************  INITIALIZE *****************************/

void Initialize(void)
{
}

/************************ malloc data ************************/

void mallocdata(void)
 {
   int n;


   /* struct vak */
   wie = (struct vak **)calloc((MAX_ROW+1) , sizeof(struct vak *) );
   for (n=0;n < MAX_ROW+1;n++)
   {
    wie[n] = (struct vak *) calloc(1,sizeof(struct vak));
     if (wie[n]==0)
	   {
	    printf("%u coreleft\n\n",coreleft());
	    puts("TOO LESS MEMORY\n");
	    exit(-1);
	   }
   }
}
/***************** DEALLOCEER DATA *************************************/
void freemallocdata(void)
{
  int n;
  for (n=MAX_ROW; n >= 0;n--)	free(wie[n]);	 free(wie);
}
/*************************READ PRT FILE *******************************/
void readprn(char *filein)
{
   int n,fout,row,ndag;
   char  inputline[1024];
   long count;
   char *token,nop[80],nopp[25];
   char getal[8];
   float bars;


   printf("\n\nInlezen %s ",filein);

   if ((fpin=fopen(filein,"r")) == NULL)
	{
	   printf("cannot open input-file %s\n",filein);
	   exit(1);
	}

  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2700);
  strcpy(nopp,"                     ");
   count=0;
   row=0;
   bars=(float)( filelength(fileno(fpin)) ) / 21;

   while(TRUE)
	{
	  if(fgets(inputline,1024,fpin)==NULL)    break;
	  count+=strlen(inputline)+2;
	  strnset(nopp,176,(int)((float)count/bars));
	  swrite(31,11,nopp,BARCOLOR);
/*	  strcpy(nop,"  ");
	  strcat(nop,inputline);
*/	  strcpy(inputline,remove_lt_space(inputline));

	  fout=FALSE;
	  if(strlen(inputline)<3)  fout=TRUE;
	  if(inputline[0]==';')    fout=TRUE;
	  if(row>=MAX_ROW-3) {lost=TRUE; fout=TRUE;}
	  if(!fout)
	  {

	  strcpy(nop,strtok(inputline,","));
	  nop[MAX_LEN_NAAM-1]=0;
	  strcpy(wie[row]->naam,nop);

	  strcpy(nop,strtok(NULL,","));
	  nop[MAX_LEN_DATUM-1]=0;
	  strcpy(wie[row]->gebdatum,nop);

	  strcpy(nop,strtok(NULL,","));
	  nop[MAX_LEN_STATUSNR-1]=0;
	  strcpy(wie[row]->statusnr,nop);

	  strcpy(nop,strtok(NULL,","));
	  nop[MAX_LEN_DATUM-1]=0;
	  strcpy(wie[row]->datum_1e_afn,nop);

	  strcpy(nop,strtok(NULL,","));
	  wie[row]->afnames=atol(nop);

	  strcpy(nop,strtok(NULL,","));
	  nop[MAX_LEN_NAAM-1]=0;
	  strcpy(wie[row]->zkhs,nop);

	  strcpy(nop,strtok(NULL,","));
	  wie[row]->doosnr=atoi(nop);

	  strcpy(nop,strtok(NULL,","));
	  nop[MAX_LEN_MEMO-1]=0;
	  strcpy(wie[row]->memo,nop);

	  }
	}
rows=row;
fclose(fpin);
}

/********************* edit fields ********************/

void edit_fields(void)
{
}
/*
int persoon;
clrscr();
persoon=0;

while (persoon!=999)
{
persoon=kiesnaam("**AFSCHRIJVEN** Kies een naam door de eerste letter te drukken");
if(persoon != 999 ) display_persoon(persoon); 
}
window(1,1,80,25);

}
*/

/********************* kies naam **************************/
int kiesnaam(char *text)
{
   char nop[80];
   int m,n,y,leave;
   char ch;			/* te printen char */
   int begin = 0;               /* 1e file uit blok van 25 dat geprint word */
   int keuze = 0;		/* nummer gekozen file */
   int num = 0;                 /* aantal files */
   int oke;                     /* 0 = geen keuze ESC 1 = keuze RETURN */
   int toets;			/* toetsaanslag via getch() */
   int pos = 0;			/* positie binnen filenaam */
   int veld;			/* nummer van filenaam */
   int xx =0;                   /* row waar geprint moet worden */
   int yy = 0;                  /* colom waar geprint moet worden */
   int keus=0;
   int keer=0;
  char volnaam[82];

  window(1,1,80,25);
  clrscr();
  gotoxy(1,1);
  textcolor(YELLOW);
  highvideo();
  cprintf("%s",text);
  normvideo();
   for(n=0;n<MAX_ROW;n++)
      {
      if(wie[n]->naam[0]==0) break;
      num++;
      }
   while (1)
    {
    oke=0;
    m=begin+24;
    if(m>num) m=num;
    for(veld = begin; veld < begin+24; veld++)
     {
      yy = 2 + (veld - begin);
      xx = 1;
      if(veld < num)
       {
       sprintf(volnaam," %-30s  gebdat %-6s 1e afn %-6s",wie[veld]->naam,wie[veld]->gebdatum,wie[veld]->datum_1e_afn);
       for(pos = 0; pos < strlen(volnaam); pos++, xx++)
	{
	  ch = (volnaam[pos]);
	  if (veld == keuze) *(SCHERMPTR + yy * 80 + xx) = ch | CHINV;
	  else               *(SCHERMPTR + yy * 80 + xx) = ch | CHNOR;
	 }
	}
	 else
	   for(pos = 0; pos < 80; pos++, xx++)
			     *(SCHERMPTR + yy * 80 + xx) = SPACE | CHNOR;

     }
    if(oke > 0)
      {
	veeg(0,0,25,80);
	if(oke==1) return(999);
	 else	   return(keuze);
      }
    toets = getch();
    if(toets == SPECIAL_KEY)
     {
      toets = getch();
      switch (toets)
	{
	   case UP:      keuze -= 1;      break;
	   case LEFT:  --keuze;           break;
	   case DOWN:    keuze += 1;      break;
	   case RIGHT: ++keuze;           break;
	   case HOME:    keuze = 0;       break;
	   case ENDT:    keuze = num - 1; break;
	}
      if(keuze < 0)          keuze = num - 1;
      if(keuze > num - 1)    keuze = 0;
      if(keuze>20)           begin = keuze - 20;
      else                   begin = 0;
     }
     else
      {
       ch = toets;
       keus=keuze;
       leave=0;
       keer=-1;
       while(keer<2 && ch>31)
	 {
	   keer++;
	   if(keer==1) { keus=-1;}
	     for(y=keus+1; y<num; y++)
	      {
	       if(strnicmp(&wie[y]->naam[0],&ch,1) == 0)
		  {
		    keuze = y;  oke = 0; leave=1;  keer=2;
		    if(keuze < 20)  begin = 0;
		    else  begin = keuze - 20;
		  }
	       if(leave) break;
	       }
	  }

    if(ch == RETURN)  { oke = 2; return(keuze); }
    if(ch == ESC) { keuze=-1;   oke = 1; return(999); }
   }
  }
}
/****************** save data ***************************/
int save_data(void)
{
   FILE *stream;
   int n,i;
   long count;
   float bars;
   char nopp[30];
   int persoon,ndag;

   clrscr();

   strcpy(fileout,filename);
   strcat(fileout,".DVR");
   if ((stream = fopen(fileout, "wb")) == NULL) /* open file TEST.$$$ */
   {
      fprintf(stderr, "Cannot open output file.\n");
      getch();
      return 0;
   }
  printf("\n\nSchrijven %s ",fileout);
  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2000);
  strcpy(nopp,"                     ");

rows=0;
  for(rows=0;rows<MAX_ROW;rows++)
      if(wie[rows]->naam[0]==0 ) break;

  count=0;
  bars=(float)( rows*sizeof(struct vak )) / 21;

 fwrite(&rows, sizeof(int), 1, stream); /* write struct vak to file */

  for(n=0;n<rows;n++)
    {
    wie[n]->check=CHECKCODE;
    fwrite(wie[n], sizeof(struct vak), 1, stream); /* write struct vak to file */
    count+=sizeof(struct vak);
    strnset(nopp,176,(int)((float)count/bars));
    swrite(31,11,nopp,BARCOLOR);
    }
   
   fclose(stream); /* close file */

   strcpy(fileout,filename);
   strcat(fileout,".PRC");

     if ((stream = fopen(fileout, "w")) == NULL)
       {
	 fprintf(stderr, "Cannot open output file %s.\n",fileout);
	 getch();
	 return 0;
	}


rows=0;
  for(rows=0;rows<MAX_ROW;rows++)
      if(wie[rows]->naam[0]==0 ) break;



      for(n=0;n<rows;n++)
	 {
	  fprintf(stream,"%s,%s,%s,%s,%ld,%s,%ld,%s",
	     wie[n]->naam,
	     wie[n]->gebdatum,
	     wie[n]->statusnr,
	     wie[n]->datum_1e_afn,
	     wie[n]->afnames,
	     wie[n]->zkhs,
	     wie[n]->doosnr,
	     wie[n]->memo
	   );
	   fprintf(stream,",|\n");
	  }
  fclose(stream); /* close file */

   return 1;
}

/*****************   load_data(void)*****************/
int load_data(void)
{
   FILE *stream;
   int n;
   long count;
   float bars;
   char nopp[30];

   clrscr();

   if ((stream = fopen(fileload, "rb"))
       == NULL)
   {
      fprintf(stderr,
	      "Cannot open input file.\n");
      getch();
      return 0;
   }
   printf("\n\nInlezen %s ",fileload);
  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2400);
  strcpy(nopp,"                     ");

  count=0;
  bars=(float)( filelength(fileno(stream)) ) / 21;

 fread(&rows, sizeof(int), 1, stream); /* read aantal entries */

  for(n=0;n<rows;n++)
    {
     if(n>MAX_ROW-3) {lost=TRUE; break;}
     fread(wie[n], sizeof(struct vak), 1, stream); /* read struct vak to file */
     count+=sizeof(struct vak);
     if(wie[n]->check != CHECKCODE)
       {
	 cputs("\r\nEr klopt iets niet met deze file\r\n");
	 cputs("Lees de backup file in\r\n");
	 getch();
	 exit(1);
       }
     strnset(nopp,176,(int)((float)count/bars));
     swrite(31,11,nopp,BARCOLOR);
    }
  fclose(stream);
  return 1;
}

/*

/********************* display en edit persoon *****************/
int display_persoon(int pers)
{
int i,j,m,n;
char text[80];
char Text[2][10];
char cc;

while(TRUE)
{

prnt_pers_id(pers);

window(1,7,80,21);
n=0;
while(wie[pers]->dag[n++].RV!=0)
if(n>=MAX_DAGEN-2)   delete_entry(pers,0);

textbackground(LIGHTGRAY);
textcolor(BLACK);
clrscr();
n=0;
gotoxy(1,1);
textcolor(RED);
cputs(" Entry        1e dag       Laatste dag     RVS   uren opgenomen\r\n");

window(1,8,80,21);
textbackground(LIGHTGRAY);
textcolor(BLACK);
clrscr();
gotoxy(1,1);
while(wie[pers]->dag[n].RV!=0)
  {
  if(n) puts("\r");
  cprintf(" %3d          %-6s          %-6s        %c          %d",++n,wie[pers]->dag[n].datumvrij,wie[pers]->dag[n].datumeindvrij,wie[pers]->dag[n].RV,wie[pers]->dag[n].urenopgenomen);
  }
textbackground(BLACK);
textcolor(WHITE);
window(1,22,80,25);
clrscr();

normvideo();
gotoxy(1,1);
while(bioskey(1))
	 getch();
cputs("    @, R, V of S : .               Voer @ in om entry te wissen");
gotoxy(20,1);
cc=getche();
if(cc=='@'){  delete_entry(pers,999); continue; }
if(cc<64) { window(1,1,80,25); clrscr(); return 1;}
if(cc>96) cc-=32;

while (TRUE)
{
 gotoxy(1,2);
 cputs(" Eerste dag vrij : ......          Voer spatie in om te stoppen");
 gotoxy(20,2);
 gets(text);
 if(text[0]==32)  return 0;
 text[MAX_LEN_DATUM-1]=0;
 if(! strlen(text)) continue;
 strcpy(Text[0],text);
 break;
}

while (TRUE)
{
 gotoxy(1,3);
 cputs("Laatste dag vrij : ......");
 gotoxy(20,3);
 gets(text);
 text[MAX_LEN_DATUM-1]=0;
 if(text[0]==32) return 0;
 if(! strlen(text)) continue;
 strcpy(Text[1],text);
 break;
}


while (TRUE)
{
 gotoxy(1,4);
 cputs("  Uren opgenomen : ....");
 gotoxy(20,4);
 gets(text);
 text[4]=0;
 if(text[0]==32) return 0;
 if(! strlen(text)) continue;
 break;
}

wie[pers]->dag[n].RV=cc;
strcpy(wie[pers]->dag[n].datumvrij,Text[0]);
strcpy(wie[pers]->dag[n].datumeindvrij,Text[1]);
wie[pers]->dag[n].urenopgenomen=atoi(text);

wie[pers]->urenover-=wie[pers]->dag[n].urenopgenomen;

}
}
*/

/******************** persoon_editen ***********************/
int    persoon_editen(void)
{
char text[90];
int persoon,pers;

clrscr();
persoon=0;
  for(rows=0;rows<MAX_ROW;rows++)
      if(wie[rows]->naam[0]==0 ) break;
 

persoon=kiesnaam("** EDIT **      Druk escape om toe te voegen");


if(persoon==999)
 pers=rows;
  else pers=persoon;
  if(rows>=MAX_ROW-3) {lost=TRUE; return 0;}

prnt_pers_id(pers);
gotoxy(5,10);
cputs("Voer een SPATIE in een veld in om invoeren te verlaten");
while(TRUE)
{
 window(1,12,80,25);
 clrscr();
 gotoxy(1,1);
 cputs("      Naam :");
 gotoxy(1,2);       
 cputs(" Geb datum :");
 gotoxy(1,3);
 cputs("  Statusnr :");
 gotoxy(1,4);
 cputs("    1e afn :");
 gotoxy(1,5);
 cputs("Aantal afn :");
 gotoxy(1,6);
 cputs("      Zkhs :");
 gotoxy(1,7);
 cputs("    Doosnr :");
 gotoxy(1,8);
 cputs("      Memo :");
  

 gotoxy(1,1);
 cputs("      Naam : .........         Type in: @, om persoon te wissen");
 gotoxy(14,1);
 gets(text);
 text[MAX_LEN_NAAM-1]=0;
 if(text[0]==32) return 0;
 if(strcmp(text,"@")==0) { wissen(pers); return 0; }
 if(strlen(text))
   strcpy(wie[pers]->naam,text);
 if(wie[pers]->naam[0]==0) strcpy(wie[pers]->naam,"?"); 

 
 gotoxy(1,2);
 cputs(" Geb datum : ......");
 gotoxy(14,9);
 gets(text);
 if(text[0]==32) return 0;
 if(strlen(text))
  strncpy(wie[pers]->gebdatum,text,MAX_LEN_DATUM-1);
 if(wie[pers]->gebdatum[0]==0) strcpy(wie[pers]->gebdatum,"?"); 

 gotoxy(1,3);
 cputs(" Status nr : ..................");
 gotoxy(14,3);
 gets(text);
 if(text[0]==32) return 0;
 text[MAX_LEN_STATUSNR-1]=0;
 if(strlen(text))
   strcpy(wie[pers]->statusnr,text);
 if(wie[pers]->statusnr[0]==0) strcpy(wie[pers]->statusnr,"?"); 

 gotoxy(1,4);
 cputs("    1e afn : ...............................");
 gotoxy(14,4);
 gets(text);
 if(text[0]==32) return 0;
 text[MAX_LEN_DATUM-1]=0;
 if(strlen(text))
   strcpy(wie[pers]->datum_1e_afn,text);
 if(wie[pers]->datum_1e_afn[0]==0) strcpy(wie[pers]->datum_1e_afn,"?"); 

 gotoxy(1,5);
 cputs("Aantal afn : .....");
 gotoxy(14,6);
 gets(text);
 if(text[0]==32) return 0;
 if(strlen(text))
  wie[pers]->afnames=atol(text);

 gotoxy(1,6);
 cputs("      Zkhs : ...............................");
 gotoxy(14,4);
 gets(text);
 if(text[0]==32) return 0;
 text[MAX_LEN_NAAM-1]=0;
 if(strlen(text))
   strcpy(wie[pers]->zkhs,text);
 if(wie[pers]->zkhs[0]==0) strcpy(wie[pers]->zkhs,"?"); 

 gotoxy(1,7);
 cputs("    Doosnr : ....");
 gotoxy(14,7);
 gets(text);
 if(text[0]==32) return 0;
 if(strlen(text))
   wie[pers]->doosnr=atol(text);
  
 gotoxy(1,8);
 cputs("      Memo : ..........................................");
 gotoxy(14,9);
 gets(text);
 if(text[0]==32) return 0;
 if(strlen(text))
  strncpy(wie[pers]->memo,text,MAX_LEN_DATUM-1);
 if(wie[pers]->memo[0]==0) strcpy(wie[pers]->memo,"?"); 

 prnt_pers_id(pers);

 gotoxy(5,12);
 cputs(" OK? Return = OK ");
 if(getch()==13) break;

}
   gotoxy(5,15);
   cputs("Ben aan het sorteren");
  for(rows=0;rows<MAX_ROW;rows++)
      if(wie[rows]->naam[0]==0 ) break;
   csort(rows);
window(1,1,80,25);
clrscr();
return 0;
 }


/************* print persoons personalia ********************/
void prnt_pers_id(int pers)
{
window(1,1,80,25);
clrscr();
highvideo();
textcolor(GREEN);
gotoxy(1,1);
cprintf("      Naam : %s",wie[pers]->naam);
gotoxy(25,1);
cprintf("Geb. datum : %-s",wie[pers]->gebdatum);
gotoxy(1,2);
cprintf("  Statusnr : %-s",wie[pers]->statusnr);
gotoxy(1,3);
cprintf("    1e afn : %-s",wie[pers]->datum_1e_afn);
gotoxy(25,3);
cprintf("Aantal afn : %ld",wie[pers]->afnames);
gotoxy(1,4);
cprintf("      Zkhs : %-s",wie[pers]->zkhs);
gotoxy(1,5);
cprintf("    Doosnr : %-ld",wie[pers]->doosnr);
gotoxy(1,6);
cprintf("      Memo : %-5d",wie[pers]->memo);
normvideo();

/*cputs("\r\n--------------------------------------------------------------------------------");*/
}

/**************  persoon wissen ******************************/
void wissen(int pers)
{
int n;
for (n=pers;n<MAX_ROW;n++)
	 memcpy(wie[n],wie[n+1],sizeof(struct vak));
}


/*

/************** delete dag ************************/
void delete_entry(int pers,int dagnr)
{
 int n,entry,noofentries=0;
 char text[80];

 if(dagnr==999)
   {
    gotoxy(1,3);
    cputs("Welke entry wissen : ..");
    gotoxy(22,3);
    gets(text);
    if(text[2]==0);
    if(strlen(text)==0) return;
    entry=atoi(text)-1;

    while(wie[pers]->dag[noofentries].RV!=0)
		  noofentries++;

    if(entry > noofentries) 
      { gotoxy(25,3);
	puts("Bestaat niet,  Druk een toets");
	   sound(50);
	   nosound();
	   sound(50);
	   delay(200);
	   nosound();
	getch();
	return;
       }
    wie[pers]->urenover+=wie[pers]->dag[entry].urenopgenomen;

    for (n=entry;n<MAX_DAGEN;n++)
	 memcpy(&wie[pers]->dag[n],&wie[pers]->dag[n+1],sizeof(struct fvak));
     
    gotoxy(1,4);
    highvideo();
    cputs("De afgeschreven uren van de gewiste dag zijn weer bij het totaal bijgeschreven");
    gotoxy(5,5);
    cputs("Druk een toets");
    normvideo();
    getch();
    return;
   }

for(n=dagnr;n<MAX_DAGEN-1;n++)
memcpy(&wie[pers]->dag[n],&wie[pers]->dag[n+1],sizeof(struct fvak));


}
*/
/************************** OVERZICHT ************************/
void   overzicht(void)
{
int n;
int pers,persoon;
char text[80];

persoon=kiesnaam("**OVERZICHT**    Druk escape om selectie te maken");
pers=persoon;

  for(rows=0;rows<MAX_ROW;rows++)
      if(wie[rows]->naam[0]==0 && wie[rows]->naam[0]==0) break;

if(persoon!=999)
 {
  prnt_pers_id(pers);
  gotoxy(5,15);
  cputs("Hier een overzicht van ?? RETURN=OK");
  if(getch()==13) print_overzicht(persoon);
  return;
 }
 clrscr();

 for(persoon=0;persoon<rows;persoon++)
    {
     gotoxy(2,2);
     highvideo();
     textcolor(GREEN);
     cprintf("      Naam : %s                        ",wie[persoon]->naam);
     normvideo();
     gotoxy(10,10);
     cputs("Druk een toets om het printen te stoppen");
     if(kbhit()) return;
     print_overzicht(persoon);
    }
       
}

/******************* overzicht printen ***********************/
void print_overzicht(int pers)
{
int n;

struct date d;

getdate(&d);
fprintf(printer,"Verwerkingsdatum : %d-%d-%d",d.da_day,d.da_mon,d.da_year);
fprintf(printer,"\n\n      Naam : %s\n",wie[pers]->naam);
fprintf(printer,"Geb. datum : %-s\n",wie[pers]->gebdatum);
fprintf(printer,"  Statusnr : %-s\n",wie[pers]->statusnr);
fprintf(printer,"    1e afn : %-10s ",wie[pers]->datum_1e_afn);
fprintf(printer,"Aantal afn : %-ld\n",wie[pers]->afnames);
fprintf(printer,"      Zkhs : %-d\n",wie[pers]->zkhs);
fprintf(printer,"    Doosnr : %-5d\n",wie[pers]->doosnr);
fprintf(printer,"      memo : %d %\n",wie[pers]->memo);
fprintf(printer,"-------------------------------------------------------------------\n");
fprintf(printer,"\f");
fflush(printer);
}


/************************** sorteren op naam ***********************/
void csort(int n)
{
   int l,j,ir,i,s;

   
   l=(n >> 1)+1;
   s=ir=n;
   memmove(wie[1],wie[0],sizeof(struct vak)*(s+1));
   for(;;)
      {
      if(l > 1)
	   memmove(wie[MAX_ROW-1],wie[--l],sizeof(struct vak));
      else {
	   memmove(wie[MAX_ROW-1],wie[ir],sizeof(struct vak));
	   memmove(wie[ir],wie[1],sizeof(struct vak));
	   if(--ir == 1)
	     {
	      memmove(wie[1],wie[MAX_ROW-1],sizeof(struct vak));
	      memmove(wie[0],wie[1],sizeof(struct vak)*(s+1));
	      return;
	     }
	   }
      i=l;
      j=(l << 1) ;
      while(j <= ir ) 
	{
	 if(j < ir && (wie[j]->afnames>wie[j+1]->afnames)) ++j;
	 if(wie[MAX_ROW-1]->afnames>wie[j]->afnames)
	 {
	 memmove(wie[i],wie[j],sizeof(struct vak));
	 j += (i=j);
	 }
	 else j=ir+1;
	}
      memmove(wie[i],wie[MAX_ROW-1],sizeof(struct vak));
     }
}

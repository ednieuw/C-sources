#include "stdio.h"
#include "io.h"
#include "dos.h"
#include "graphics.h"
#include "dir.h"
#include "string.h"
#include "math.h"
#include "ctype.h"
#include "process.h"
#include "time.h"
#include "alloc.h"

#define TRUE  1
#define FALSE 0
#define BARCOLOR 0X2400

#define breedte_menu     35     /* Breedte van menu's */
#define MAX_MENU_ITEMS 20      /* Maximaal aantal menu keuzes */
#define MAXTL    40            /* Maximale breedte v/e menu keuze */
#define MAXDISK   5
#define TRUE      1
#define FALSE     0
#define LEEG     -1

#define NEW     0
#define LOAD    1       /* hoofdmenu items */
#define SAVE    2
#define CHADIR  3
#define EDITOR  4
#define QUIT    5
static char titel[QUIT+1][breedte_menu];

#define DATUM __DATE__


#define SPECIAL_KEY 0
#define UP 72
#define DOWN 80
#define HOME 71
#define ENDT 79
#define LEFT 75
#define UP 72
#define EXEC 13
#define ESC 27
#define RIGHT 77
#define PgUp 73
#define PgDn 81
#define SPACE 32
#define RETURN 13
#define TAB 9
#define CHBIN 0xF000    /* Blink Invers */
#define CHBHI 0x8F00	/* Blink Highligted */
#define CHHIG 0x0F00    /* Highligted */
#define CHINV 0x7000    /* Invers */
#define CHNOR 0x0700    /* Normaal */



void   set_prn_filename(char *);
int    display_menu(int ,int );
int    menuver(int ,int ,int ,int, int );
int    load_menu();
void   makedate(int *d, int *m, int *y,unsigned dosdate);
char   *filedir(char *dir);
void   changedir(char *dir);
int    parse(char *ptr);
void   box(int topy,int topx,int lang,int breed,int type,int kleur);
void   swrite(int posx,int posy,char * tekst,int kleur);
void   twrite(int posx,int posy,char * tekst,unsigned forground,unsigned background);
void   initdisplay();
void   veeg(int topy,int topx,int totl,int totb);
void   lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur);
char   *remove_lt_space(char *inputline);
void   init_plaat(void);
void   stel_in(void);
void   load_plaat(char *filein);
void   save_plaat(char *fileout);

int print, show,lost,actie;

#define MAXMLEN 6
#define MAXBREED 13
#define MAXLANG 161
struct elisa
{
 char naam[MAXMLEN];
 float concverd;
} pos[MAXLANG][MAXBREED];        /*ruimte voor 20 platen*/

struct ffblk ffblk;
unsigned disk;
char *directory;
char curdir[132];
int size;
int show;

FILE *fp;
FILE *fpin;
FILE *fpout;
FILE *printer;
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

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    MaxX, MaxY;		/* The maximum resolution of the screen */
int    MaxColors;		/* The maximum # of colors available	*/
int    ErrorCode;		/* Reports any graphics errors		*/
struct palettetype palette;     /* for palette info			*/


int far *SCHERMPTR,far *PTR;

char  inputline[255],nop[255],nop1[25];

/********************************** MAIN *******************************/

main(int argc, char **argv)
{
printer = fopen("LPT1","w");
print    = TRUE;
show     = FALSE;
strcpy(outfile,"");
fileload[0] = 0;
actie=NEW;

initdisplay();
clrscr();

if(argc==2 && argv[1][0]=='?')
 {
   clrscr();
   swrite(1,1,"Instelfile maker voor ELISA reken programma              ",0x0700);
   swrite(1,3,"Dit programma maakt maakt een ASCII (tekst) file aan     ",0x0700);
   swrite(1,4,"die aangeeft hoe een elisaplaat is ingezet.              ",0x0700);
   swrite(1,5,"Er kan voor 20 platen aangegeven worden waar wat staat.  ",0x0700);
   swrite(1,6,"Het reken programma middeld alle ijklijnpunten. Het reken",0x0700);
   swrite(1,7,"programma L is beperkt in het aantal ijklijnpunten (zie  ",0x0700);
   swrite(1,8,"HELP programma L), dusreken bij voorkeur per plaat uit.  ",0x0700);
   swrite(1,9,"Ga met de cursor naar een positie en druk M als het om een",0x0700);
   swrite(1,10,"monster gaat, geef het een volgnummer (max 3 cijfers) en  ",0x0700);
   swrite(1,11,"Return en een verdunning van dat monster.                ",0x0700);
   swrite(1,12,"Druk een Y als het om een ijklijnpunt gaat en voer de     ",0x0700);
   swrite(1,13,"concentratie van dat punt in.                             ",0x0700);
   swrite(1,14,"Return copieert de laatste invoer.                       ",0x0700);
   swrite(1,15,"Met PgDn en PgUp verander je van plaat.                  ",0x0700);
   swrite(1,16,"C copieert de vorige plaat en W wist de plaat.            ",0x0700);
   getch();
   clrscr();
   exit(1);
 }

if(argc==2)    strcpy(fileload,argv[1]);

strcpy(curdir,"?:\\");
curdir[0] = 'A'+getdisk();
maindisk = getdisk();
getcurdir(0,curdir+3);
if(curdir[(int)strlen(curdir)-1] != 92) strcat(curdir,"\\");
strcpy(maindir,(char *)curdir);


while(1)
   {
   wis = 1;
   actie=display_menu(wis,actie); /* Als geen filenaam in commandline */
   if (actie == -1) actie=QUIT;
   switch (actie)
      {
	   case NEW:
		  geladen=1;
		  init_plaat();
		  stel_in();
		  strcpy(fileload,"NONAME");
		  actie=SAVE;
		  break;

	   case LOAD:
		  veeg(0,0,25,80);
		  strcpy(fileload,(char *)filedir("*.INE"));
		  if (strlen(fileload) < 2) break;
		  strcpy(file,fileload);
		  init_plaat();
		  load_plaat(file);
		  geladen = 1;
		  veeg(0,0,25,80);
		  stel_in();
		  actie = SAVE; 
		  break;

	   case SAVE:
		  if(!geladen) break;
		  veeg(0,0,25,80);
		  strcpy(file,fileload);
		  save_plaat(file);
		  actie=NEW;
		  break;


	   case CHADIR:
		  swrite(13,16,"RETURN or New directory: \0",0x1300);
		  gotoxy(39,17);
		  gets(dir);
		  if (strlen(dir) >1) changedir(dir);
		  strcpy(curdir,"?:\\");
		  curdir[0]='A'+getdisk();
		  getcurdir(0,curdir+3);
		  if(curdir[strlen(curdir)-1] != 92) strcat(curdir,"\\");
		  actie=NEW;
		  if(geladen) actie=NEW;
		  break;


	   case EDITOR:
		  strcpy(drawdir,searchpath("pe.exe"));
		  if(spawnlp(P_WAIT,drawdir,"pe.exe",fileload,NULL)==-1)
		   {
		     veeg(0,0,25,80);
		     puts("\n\n\n\n\n      NOT ENOUGH MEMORY FOR EDITOR OR EDITOR NOT FOUND\0");
		     delay(3000);
		    }
		  actie=NEW;
		  if(geladen) actie=SAVE;
		  break;

	   case QUIT:
		 default:
		  veeg(0,0,25,80);
		  setdisk(maindisk);
		  chdir(maindir);
		  printf("%s\n",maindir);
		  exit(1);
		  break;
      }
  }

}
/******************** set prn filenaam *************************/
void set_prn_filename(char *filenaam)
{
 if(filenaam[1]!='*') strcpy(outfile,"C:"); else strcpy(outfile,"");/*onzin*/
 strcpy(outfile,filenaam); /*onzin*/
 strcpy(outfile,"C:RESULT.PRN");
 if ((fpout=fopen(outfile,"w")) == NULL)
	 {
	  printf("%s","Can not open outputfile");
	  exit(1);
	 }
}
  
/******************** display menu ***************************/
int display_menu(int wis,int barkeuze)
{
   int num,nr,breed,keuze;
   char datum[12],nop[50],nopp[30];
   num=load_menu();                   /* laad tekst menu */
   if(wis)  lveeg(0,0,25,80,' ',0x2500);
   strcpy(datum,DATUM);
   sprintf(nop,"Coreleft %lu bytes",(unsigned long)farcoreleft());
   swrite(2,24,nop,0x2100);

   twrite(2,20,"ษออออออออออออออออออออออออออออป\0",YELLOW,BLUE);
   twrite(2,21,"บProblemen? Ed Nieuwenhuys   บ\0",YELLOW,BLUE);
   twrite(2,22,"ศออออออออออออออออออออออออออออผ\0",YELLOW,BLUE);


   swrite(17,0,"ษออออออออออออออออออออออออออออป\0",0x7100);
   swrite(17,1,"บ    INE maker               บ\0",0x7100);
   swrite(17,2,"ศออออออออออออออออออออออออออออผ\0",0x7100);
   swrite(34,1,datum,0x7100);
   swrite(17,3,curdir,0x0700);
   swrite(50,6,fileload,0x0700);
   gotoxy(1,1);
   breed=breedte_menu;
   keuze = menuver(num,5,17,breed-5, barkeuze);
   if (keuze==QUIT || keuze==-1) veeg(0,0,20,80);
   return(keuze);

}

/******************  menu verticaal **************************/
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
		if (y==keuze)   *(SCHERMPTR + yy*80 + xx) = ch | 0x1C00;
		else
		  if (x==0)     *(SCHERMPTR + yy*80 + xx) = ch | 0x1A00;
			else    *(SCHERMPTR + yy*80 + xx) = ch | 0x7A00;
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
	  strcpy(titel[0],"New file                  \0");
	  strcpy(titel[1],"Laad INE file             \0");
	  strcpy(titel[2],"Save INE file             \0");
	  strcpy(titel[3],"Andere drive / directory  \0");
	  strcpy(titel[4],"Editor                    \0");
	  strcpy(titel[5],"Quit                      \0");
  return(nr);
 }



/********************** init display  *****************/
void initdisplay(void)
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);

if (reg.h.al == 7) SCHERMPTR=MK_FP(0XB000,0000);
else               SCHERMPTR=MK_FP(0XB800,0000);

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
   int y, x;
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
}


/************  remove kleiner dan spatie ***************/
char *remove_lt_space(char *inputline)
{
	int n;
	  char nop[255];
	  strcpy(nop,strrev(inputline));
	  for(n=strlen(nop);n>0;n--)
			{ if(nop[n]<=' ') nop[n]=0; else break;}
	  strcpy(inputline,strrev(nop));
	  for(n=strlen(inputline);n>0;n--)
			{ if(inputline[n] <= ' ') inputline[n]=0; else break;}
 return(inputline);
}


/************** haal datum uit ffblk.ff_fdate *******************/
void makedate(int *d, int *m, int *y,unsigned dosdate)
{
 *m = (dosdate >>5)&0XF;
 *d = dosdate & 0x1F;
 *y = (dosdate >>9) & 0X3F;
 *y += 80;
}           


/**************** Directory listen op scherm en keuze maken ************/

char *filedir(char *filein)
{
   char nop[30];
   int y,leave;
   static char filenaam[250][30];      /* nr,breedte filenaam + datum*/
   char ch;			/* te printen char */
   int begin = 0;               /* 1e file uit blok van 25 dat geprint word */
   int keuze = 0;		/* nummer gekozen file */
   int num = 0;                 /* aantal files */
   int oke;                     /* 0 = geen keuze ESC 1 = keuze RETURN */
   int toets;			/* toetsaanslag via getch() */
   int pos = 0;			/* positie binnen filenaam */
   int veld;			/* nummer van filenaam */
   int x = 0;			/* teller */
   int xx =0;                   /* row waar geprint moet worden */
   int yy = 0;                  /* colom waar geprint moet worden */
/*   int maxaantal = 110; */    /* aantal files per beeldscherm */
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
		else   ch = SPACE;
		if (veld == keuze) *(SCHERMPTR + yy * 80 + xx) = ch | CHHIG;
		else               *(SCHERMPTR + yy * 80 + xx) = ch | CHINV;
	       }
	    }
	 else  for(pos = 0; pos < 21; pos++, xx++)
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
/*			   if(keuze > num - 1)    keuze = num - 1;
*/			   if(keuze < 25)         begin = 0;
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

/*********************** CHDIR **********************/
void changedir(char *dir)
{
if ( parse((char *)dir) )
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
    disk = (unsigned) toupper( ptr[0]) - 65;
	if (disk > MAXDISK)         return(-1);
    directory = ptr+ 2;
   }
 else
     {
      disk=getdisk();
      directory=ptr;
     }

if (*directory=='\0') directory=".";

  return(0);
}

/* VIDEO.C   20-06-88 */


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
   int x,y,z;				/* teller */
   unsigned int ch;			/* horizontale streep */
   unsigned int ck;			/* kleur die geprint moet worden */
   unsigned int clb;			/* hoek links boven */
   unsigned int crb;            	/* hoek rechts boven */
   unsigned int clo;            	/* hoek links onder */
   unsigned int cro;            	/* hoek rechts onder */
   unsigned int cv;             	/* verticale streep */

   switch (type)
      {
      case 1:
         clb = 'ฺ'; crb = 'ฟ';
         clo = 'ภ'; cro = 'ู';
         ch  = 'ฤ';  cv  = 'ณ';
         break;
      case 2:
         clb = 'ษ'; crb = 'ป';
         clo = 'ศ'; cro = 'ผ';
         ch  = 'อ';  cv  = 'บ';
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


/***********************  write text  ***********************/

void swrite(int posx,int posy,char * tekst,int kleur)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;
   for(;tekst[x];*(nop+x) = tekst[x++] | kleur);
}

void twrite(int posx,int posy,char * tekst,unsigned forground,unsigned background)
{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;
   unsigned int kleur = (background<<11) + (forground<<8);
   for(;tekst[x]; *(nop+x) = tekst[x++] | kleur);
}


/********************* init plaat *****************************/
void init_plaat(void)
{
int m,n;

/* initialyse */
for(n=1;n<MAXBREED;n++)
 for (m=1;m<MAXLANG;m++)
  {
    strcpy(pos[m][n].naam,"  ?  ");
    pos[m][n].concverd=1;
  }
}

/*********************** stel in ******************************/
void stel_in(void)
{

int m,n,i,j,plaat,loop,loopplaat;
int posx,posy;
char code,code1;
char monsternummer[5],monsterverd[7],concentratie[8],monsterconc[20];
char last1[10],last2[10];
unsigned char lastc1,lastc2;
float last3;
veeg(0,0,25,80);
plaat=0;
loopplaat=TRUE;


while(loopplaat)
{
 veeg(0,0,25,80);
 i=plaat*8;                     /* bereken regel */
 sprintf(nop,"PLAAT %d           PLAAT %d          PLAAT %d",plaat+1,plaat+1,plaat+1);
 twrite(22,18,nop,BLUE,LIGHTGRAY);

 for(n=0;n<MAXBREED;n++)
  for (m=0;m<9;m++)
   {
   posx=n;
   posy=m;
   if(n==0)
    {
    nop[0]=m+64;
    nop[1]=0;
    twrite(n,m*2,nop,WHITE,BLACK);
    }
   else if(m==0) twrite(n*6+2,m*2,(char*)itoa(n,nop,10),WHITE,BLACK);
   else if(n>0)
     { if (pos[m+i][n].naam[2]=='?')
	     twrite(posx*6,posy*2,"  ?  ",YELLOW,RED);
       if (pos[m+i][n].naam[1]=='Y')
	   {
	     twrite(posx*6,posy*2-1," Yk  ",WHITE,GREEN);
	     sprintf(nop,"%-5.5g",pos[m+i][n].concverd);
	     twrite(posx*6,posy*2,nop,WHITE,GREEN);
	   }
       if (pos[m+i][n].naam[0]=='M')
	   {
	     twrite(posx*6,posy*2-1,pos[m+i][n].naam,BLACK,BROWN);
	     sprintf(nop,"%-5.5g",pos[m+i][n].concverd);
	     twrite(posx*6,posy*2,nop,BLACK,BROWN);
	   }
     }
   }
 twrite(0,0," ",WHITE,BLACK);

 posx=posy=1;
 loop=TRUE;
 twrite(0,22,"ESCAPE om te eindigen",YELLOW,BLACK);
 twrite(0,23,"C = Copieer vorige plaat, W = Wis plaat\0",WHITE,BLACK);
 twrite(0,24,"Y = ijklijnpunt, M = monster, return = laatste\0",WHITE,BLACK);
 strcpy(nop,"     ");
 strcpy(last2,nop);
 strcpy(last1,"  ?  ");
 lastc1=YELLOW;
 lastc2=RED;
 gotoxy(posx*6+1,posy*2+1);
 while(loop)
  {
   code = getch();
   if(code==ESC) {loop=FALSE; loopplaat=FALSE; break;}
   code1=SPECIAL_KEY;
   if(code==SPACE) {code1=SPACE; code=SPECIAL_KEY;}
   if(code == SPECIAL_KEY)
     {
     if(code1 !=SPACE)  code1 = getch();
     switch (code1)
       {
	   case UP:      --posy;    break;
	   case LEFT:    --posx;    break;
	   case SPACE:
	   case DOWN:    ++posy;    break;
	   case RIGHT:   ++posx;    break;
	   case HOME:      posx=1;  posy=1;
	   case PgUp:    --plaat;   posx=1; posy=1; loop=FALSE; break;
	   case ENDT:      posx=12; posy=8; break;
	   case PgDn:    ++plaat;   posx=1; posy=1; loop=FALSE; break;
       }
     if(posx < 1 || posx>12)   posx=1;
     if(posy < 1 || posy> 8)   posy=1;
     if(plaat< 0 || plaat>19) {plaat=0; posx=1; posy=1;}
     gotoxy(posx*6+1,posy*2+1);
     twrite(0,23,"C = copieer vorige plaat, W = Wis plaat\0",WHITE,BLACK);
     twrite(0,24,"Y = ijklijnpunt, M = monster, return = laatste\0",WHITE,BLACK);
     }

    else
    {
    if(code>95) code-=32;   /* naar hoofdletters */
    switch(code)
    {
     case 'M':
     case 'S':
	     twrite(posx*6,posy*2-1,"Sample\0",WHITE,BLACK);
	     lastc1=BLACK;
	     lastc2=BROWN;
	     twrite(0,24,"Geef monsternummer + RETURN                      \0",WHITE,BLACK);
	     gets(monsternummer);
	     gotoxy(posx*6+1,posy*2+1);
	     strcpy(nop,"M");
	     strcat(nop," ");
	     nop[1]=(unsigned char) (plaat+65);
	     strcat(nop,monsternummer);
	     if(strlen(nop)>=MAXMLEN) 
	      {
	       twrite(0,24,"TE LANG VOLGNUMMER                             \0",WHITE,BLACK);
	       twrite(posx*6,posy*2-1,"      \0",WHITE,BLACK);
	       twrite(posx*6,posy*2  ,"      \0",BLACK,BROWN);
	       break;
	      }
	     strcat(nop,"      ");
	     nop[5]=0;
	     strcpy(pos[i+posy][posx].naam,nop);
	     twrite(posx*6,posy*2-1,"      \0",WHITE,BLACK);
	     twrite(posx*6,posy*2-1,nop,BLACK,BROWN);
	     strcpy(last1,nop);
	     twrite(0,24,"Geef monsterverdunning + RETURN                  \0",WHITE,BLACK);
	     twrite(posx*6,posy*2  ,"     \0",BLACK,BROWN);
	     gets(monsterverd);
	     pos[i+posy][posx].concverd=atof(monsterverd);
	     last3=pos[i+posy][posx].concverd;
	     strcpy(nop,monsterverd);
	     strcat(nop,"      ");
	     nop[5]=0;
	     twrite(posx*6,posy*2,nop,BLACK,BROWN);
	     strcpy(last2,nop);
	     gotoxy(posx*6+1,posy*2+1);
	     break;
     case 'Y':
	     twrite(posx*6,posy*2-1," Yk  \0",WHITE,GREEN);
	     lastc1=WHITE;
	     lastc2=GREEN;
	     strcpy(last1," Yk  ");
	     twrite(0,24,"Geef concentratie + RETURN                        \0",WHITE,BLACK);
	     gets(monsterconc);
	     gotoxy(posx*6+1,posy*2+1);
	     strcpy(pos[posy][posx].naam," Yk  ");
	     pos[i+posy][posx].concverd=atof(monsterconc);
	     last3=pos[i+posy][posx].concverd;
	     strcpy(nop,monsterconc);
	     strcat(nop,"      ");
	     nop[5]=0;
	     twrite(posx*6,posy*2,nop,WHITE,GREEN);
	     strcpy(last2,nop);
	     gotoxy(posx*6+1,posy*2+1);
	     break;

     case RETURN:
	     if(last1[2]=='?')
	     {
	      twrite(posx*6,posy*2  ,"  ?  ",YELLOW,RED);
	      twrite(posx*6,posy*2-1,"     ",WHITE,BLACK);
	      break;
	     }
	     twrite(posx*6,posy*2-1,last1,lastc1,lastc2);
	     twrite(posx*6,posy*2  ,last2,lastc1,lastc2);
	     pos[i+posy][posx].concverd=last3;
	     strcpy(pos[i+posy][posx].naam,last1);
	     break;

     case '?':
	     strcpy(nop,"  ?  ");
	     twrite(posx*6,posy*2,nop,YELLOW,RED);
	     strcpy(last2,nop);
	     strcpy(last1,"     ");
	     lastc1=YELLOW;
	     lastc2=RED;
	     pos[i+posy][posx].concverd=1;
	     strcpy(pos[i+posy][posx].naam,"  ?  ");
	     gotoxy(posx*6+1,posy*2+1);
	     break;

     case 'C':
	     if(plaat>0)
	      {
	       for(n=1;n<MAXBREED;n++)
		for (m=1;m<9;m++)
		 {
		  strcpy(nop,pos[m+i-8][n].naam);
		  if(nop[0]=='M')
		   {
		    nop[1]=(unsigned char) (plaat+65);
		    strcat(nop,"     ");
		    nop[5]=0;
		    strcpy(pos[m+i][n].naam,nop);
		   }
		  else strcpy(pos[m+i][n].naam,pos[m+i-8][n].naam);
		  pos[m+i][n].concverd=pos[m+i-8][n].concverd;
		 }
		}
		loop=FALSE;
		break;

      case 'W':
	       for(n=1;n<MAXBREED;n++)
		 for (m=1;m<9;m++)
		   {
		    strcpy(pos[m+i][n].naam,"  ?  ");
		    pos[m+i][n].concverd=1;
		   }
		loop=FALSE;
		break;

	     
      }/* einde case      */
    }  /* einde else      */
  }    /* einde loop      */
 }     /* einde loopplaat */
}



/**************** instellingen inlezen *************************/
void load_plaat(char *filein)
{
int x,y,n,fout;
char item[MAXBREED][20],soldaat[7];
float fl;
   swrite(6,23,"Kies een instellingen file",0x0700);

/*   strcpy(nop,drivedir);
   strcat(nop,"*.ine");
   strcpy(filein,(char *)filedir(nop));

*/
   if ((fpin=fopen(filein,"rb")) == NULL)
	{
	   printf("cannot open instellingen-file %s\n",filein);
	   sleep(2);
	   exit(-1);
	}
   x=y=0;
   while(TRUE)
    {
     if(fgets(inputline,255,fpin)==NULL)    break;
     strcpy(inputline,remove_lt_space(inputline));
     fout=FALSE;
     if(inputline[0]=='%')   fout=TRUE;
     if(strlen(inputline)<50)fout=TRUE;
     if(!fout)
       {
	x=1;
	y++;
	strcpy(inputline,remove_lt_space(inputline)); /* verwijder spaties */
	strcpy(item[1],strtok(inputline," "));        /* neem lijn uit file */

	while(++x<MAXBREED)
	   {
	    strcpy(item[x],strtok(NULL," "));    /* neem item uit lijn */
	    strcpy(item[x],remove_lt_space(item[x]));
	   }
	 x=0;
	 while(++x<MAXBREED)
	 {
	  if(strlen(item[x])<1 &&item[x][0]==13 &&item[x]!="") break;
	  if(item[x][0]>63)     /* Als begint met letter dan: monster,verd*/
	   {
	    strcpy(nop1,strtok(item[x],","));
	    strncpy(nop1,remove_lt_space(nop1),MAXMLEN);
	    strupr(nop1);
	    if(nop1[1]<64)       /* als geen ine maker file*/
	     {
	      soldaat[0]=nop1[0];
	      soldaat[1]=(unsigned char)((y-1)/8+65);
	      soldaat[2]=nop1[1];
	      soldaat[3]=nop1[2];
	      soldaat[4]=nop1[3];
	      soldaat[5]=0;
	      strcpy(nop1,soldaat);
	     }
	    sprintf(pos[y][x].naam,"%-5.5s",nop1);
	    strcpy(nop1	,strtok(NULL," "));
	    pos[y][x].concverd=atof(nop1);
	   }
	   else 
		   /* Anders is het een concentratie v/d ijklijn */
	   {
	     strcpy(nop1,strtok(item[x]," "));
	     pos[y][x].concverd=atof(nop1);
	     if(item[x][0] != '?')
		   sprintf(pos[y][x].naam," Yk  ");
	     else 
		   sprintf(pos[y][x].naam,"  ?  ");
	   }

	 }   /* WHILE */
       }     /* if fout loop */
      }      /* while loop */
fclose(fpin);


}


void save_plaat(char *fileout)
{

int m,n,i;
int platen;
int regel=0;

veeg(0,0,25,80);

 for(n=0;n<12;n++)  if(fileout[n]=='.') fileout[n]=0;

while(TRUE)
{
 twrite(7,1,"        Saven INE file        \0",BLACK,LIGHTGRAY);
 twrite(3,5,"Geef een filenaam: \0",YELLOW,BLUE);
 twrite(3,7,"Druk RETURN voor OK\0",WHITE,BLACK);
 twrite(23,5,"                     \0",WHITE,BLACK);
 twrite(23,5,fileout,YELLOW,BLACK);
 gotoxy(24,6);
 strcpy(nop,"");
 gets(nop);
 if(nop[0]==0) break;
 strncpy(fileout,nop,8);
 strcat(fileout,"");
}
 strcat(fileout,".INE");

			/* Zoek laatste monster cq plaat */
for(n=1;n<MAXBREED;n++)
 for (m=1;m<MAXLANG;m++)
  {
    if(pos[m][n].naam[0]=='M') regel=m;
  }

platen=(((regel-1)/8)+1);

if ((fpout=fopen(fileout,"wb")) == NULL)
  {
    printf("cannot open ouput instellingen-file %s\n",fileout);
    sleep(2);
    exit(-1);
  }
strcpy(nop,DATUM);
fprintf(fpout,"\%  Instellingen file  EJN INSTEL.EXE dd ");
fprintf(fpout,"%s\n",nop);

 for (m=1;m<platen*8+1;m++)      
 {
   for(n=1;n<MAXBREED;n++)
    {                                            
    if(pos[m][n].naam[0]=='M') 
     {
      for(i=0;i<7;i++) if(pos[m][n].naam[i]==32) pos[m][n].naam[i]=0;
      fprintf(fpout,"%5s,%-5.5g ",pos[m][n].naam,pos[m][n].concverd); 
     }

    else if(pos[m][n].naam[1]=='Y') 
      fprintf(fpout,"%-10.6g ",pos[m][n].concverd); 

    else if(pos[m][n].naam[2]=='?') 
      fprintf(fpout,"     ?     "); 

    else 
      fprintf(fpout,"?          "); 
    }
  fprintf(fpout,"\n");
 }
fclose(fpout);

}
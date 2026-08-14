/* elisa inlees programma om prt files aan te maken EJN */
/*compile small
 use elisa.prj*/

#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
//#include <graphics.h>
#include <dir.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define TRUE  1
#define FALSE 0
#define BARCOLOR 0X2400

#define MAX_MENU_ITEMS 20      /* Maximaal aantal menu keuzes */
#define MAXTL    40            /* Maximale breedte v/e menu keuze */
#define MAXDISK   5
#define TRUE      1
#define FALSE     0
#define LEEG     -1

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

int    GraphDriver;		/* The Graphics device driver		*/
int    GraphMode;		/* The Graphics mode value		*/
int    ErrorCode;		/* Reports any graphics errors		*/

int    read_elisa_file(char *drivedir);
void   pas_index_aan(int, int , int, int );
void   makedate(int *d, int *m, int *y,unsigned dosdate);
char   *filedir(char *dir);
void   changedir(char *dir);
int    parse(char *ptr);
void   box(int topy,int topx,int lang,int breed,int type,int kleur);
void   swrite(int posx,int posy,char * tekst,int kleur);
void   twrite(int posx,int posy,char * tekst,int forground,int background);
void   initdisplay();
void   veeg(int topy,int topx,int totl,int totb);
void   lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur);
char   *remove_lt_space(char *inputline);
void   instellingen(char *drivedir);
void   prnt_prtfile(int,char *drivedir);

int far *SCHERMPTR,far *PTR;
FILE *fpin,*fpout;
char tfileload[40];
char filein[15],fileout[15];
unsigned disk;
char *directory;
char defaultdir[132];
char curdir[132];
struct ffblk ffblk;

#define MAXBREED 13
#define MAXLANG 161
#define LEN_NAAM 6

struct elisa {
float verdunning;			/* array voor 10 plaatjes, 12 breed */
char monsternaam[LEN_NAAM];		/* monsternamen 5 characters */
float concyk;
float extinctie;
} pos[MAXLANG][MAXBREED];

int index[MAXLANG*MAXBREED+5][2];	/* array voor sort index
						index[?][0] = x
						index[?][1] = y  */
char  inputline[255],nop[255],nop1[25];



/***************************  main ***************************/
void main(int argc,char **argv)
{
char insteldrive[MAXDRIVE+MAXDIR];
char extinctiesdrive[MAXDRIVE+MAXDIR];
char outputdrive[MAXDRIVE+MAXDIR];
int aantalindex;
char c;
char drive[MAXDRIVE];
char dir[MAXDIR];
char file[MAXFILE];
char ext[MAXEXT];
int flags;
//char file[15];
//char ffile[18];

initdisplay();
clrscr();

if(argc==2 && argv[1][0]=='?')
 {
	clrscr();
	swrite(1,1,"Elisa vertaalprogramma voor LOGIT.EXE PRT files          ",0x0700);
	swrite(1,2,"Maak een instellingenfile in een ASCII editor en geef die",0x0700);
	swrite(1,3,"een naam met de extensie .INE (bijv. VAKANTIE.INE        ",0x0700);
	swrite(1,4,"Het programma leest 8 * 12 en 12 * 8 extinctiefiles in,  ",0x0700);
	swrite(1,5,"maakt van ,'s .'n en verwijdert alle tekst. De extincties",0x0700);
	swrite(1,6,"moeten gesheiden worden door spaties en langer zijn dan  ",0x0700);
	swrite(1,7,"5 karakters.                                             ",0x0700);
	swrite(1,8,"Start het programma met elisa [1] [2] [3] waarin:        ",0x0700);
	swrite(1,9,"[1] drive + directory van de instellingenfiles           ",0x0700);
	swrite(1,10,"[2] drive + directory van de extinctiefiles              ",0x0700);
	swrite(1,11,"[3] drive + directory van de outputfile ELISA.PRT        ",0x0700);
	swrite(1,12,"[1] [2] en [3] zijn optioneel en zijn default C:         ",0x0700);
	getch();
	clrscr();
	exit(1);
 }
strcpy(insteldrive,"c:");
strcpy(extinctiesdrive,"c:");
strcpy(outputdrive,"c:");


if(argc>1)
 {
 flags=fnsplit(argv[1],drive,dir,file,ext);
  strcpy(insteldrive,drive);
  strcat(insteldrive,dir);
 }
if(argc>2)
 {
 flags=fnsplit(argv[2],drive,dir,file,ext);
  strcpy(extinctiesdrive,drive);
  strcat(extinctiesdrive,dir);
 }
if(argc>3)
 {
 flags=fnsplit(argv[3],drive,dir,file,ext);
  strcpy(outputdrive,drive);
  strcat(outputdrive,dir);
 }

instellingen(insteldrive);
aantalindex=read_elisa_file(extinctiesdrive);
prnt_prtfile(aantalindex,outputdrive);
flags=flags;

}

/********** extincties inlezen en samenvoegen met instellingen ********/
int read_elisa_file(char *drivedir)
{
	int n,i,j,x,y,fout,offset,indx,xx,yy,cmp,dummy;
	char monsternieuw[9],monsterindex[9];
	float verdnieuw,verdindex,cmpf;
	char mnieuw[9],mindex[9];
	int regel,cols;
	long int count;
	char *token,nopp[75];
	float bars;
	char *p;
	char ext[15][15];

	swrite(6,23,"Kies een extinctie file",0x0700);


	strcpy(nopp,drivedir);
	strcat(nopp,"*.prn");
	strcpy(filein,(char *)filedir(nopp));

	if ((fpin=fopen(filein,"rb")) == NULL)
	{
		printf("cannot open input-file %s\n",filein);
		exit(1);
	}
	strcpy(fileout,filein);
	fileout[strlen(fileout)-1]='T';

	printf("\n\nInlezen %s ",filein);
	regel=0;
  swrite(20,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1400);
  swrite(20,11,"³                                           ³",0x1400);
  swrite(20,12,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x1400);
  swrite(20,13,"0%                   50%                   100%",0x2000);
  strcpy(nopp,"                                           ");

	count=30;
	bars=(float)( filelength(fileno(fpin)) ) / 41;

	while(TRUE)
	 {
		if(fgets(inputline,255,fpin)==NULL)    break;
		count+=strlen(inputline)+2;
		strnset(nopp,176,(int)(count/bars));
		swrite(21,11,nopp,BARCOLOR);
		strcpy(nop,"  ");
		strcat(nop,inputline);
		for(n=0;n<strlen(inputline);n++)
		{                       /*verwijder tekst en verander , of * in . */
	if(inputline[n]==',') inputline[n]='.';
	if(inputline[n]=='*') inputline[n]='.';
	if(inputline[n]=='-') inputline[n]=' ';
	if(inputline[n]>57) inputline[n]=' ';
	if(inputline[n]<32) inputline[n]=' ';

		}

		strcpy(inputline,remove_lt_space(inputline));
		fout=FALSE;
		if(strlen(inputline)<25)  fout=TRUE;
	if(!fout)
	{
	x=1;
	strcpy(inputline,remove_lt_space(inputline)); /* verwijder spaties */
	strcat(inputline,"");
	strcpy(nop,"$ ");
	strcat(nop,inputline);
	strcpy(inputline,nop);
	strcpy(ext[x],strtok(inputline," "));    /* neem ext uit lijn */
	while(1)
	{
	p=strtok(NULL," ");                      /* neem item uit lijn */
	if (p==NULL) break;
	strcpy(ext[x++],p);
	if(strlen(p)<5) x--;                     /* als geen extinctie */
	}
	if(x>7)                            /* Als geen regel is weggegooid */
	{
	 for(n=1;n<x;n++)
		if(ext[n][1]!='.')
			 fout=TRUE;                        /* is extinctie ?*/
	}
	else fout=TRUE;
		 if(!fout)
		 {
	cols=x;
	regel++;                                 /* ingelezen goede regel */
	if(x>10)
	 {
	  for(x=1;x<13;x++)
		pos[regel][x].extinctie=atof(ext[x]);
	 }
	else
	 {
	  for(x=0;x<8;x++)
		{
		 yy=9-((((regel-1)/12)+1)*8)+x;
		 xx=((regel-1)%12)+1;
		 pos[yy][xx].extinctie=atof(ext[x+1]);
		}
	 }

	}
	} /* einde !fout */
	}  /*einde regels inlezen */
  fclose(fpin);
	/* sorteren op monsternaam , verdunning */
  index[1][0] = 1;
  index[1][1] = 1;
  indx=1;
  dummy=2;
  clrscr();
  swrite(30,20,"Sorteren\0",0x2400);
  regel=(regel*(cols-1))/12;
  for (y=1;y<=regel;y++)
	{
	 putch('.');
	 for (x=dummy;x<MAXBREED;x++)
	  {
		dummy=1;
		xx= index[indx][0];
		yy= index[indx][1];
		strcpy(monsternieuw,pos[y][x].monsternaam);
		if(monsternieuw[0]=='?')  continue;
		indx++;
		for(j=1;j<indx;j++)
		 {
	xx= index[j][0];
	yy= index[j][1];
	strcpy(monsterindex,pos[yy][xx].monsternaam);
	if(isdigit(monsternieuw[0]) & isdigit(monsterindex[0]))
	  {
		cmpf=atof(monsternieuw)-atof(monsterindex);
		if(cmpf==0) cmp=0;
		if(cmpf>0) cmp=1;
		if(cmpf<0) cmp=-1;
	  }
	else
		 cmp=stricmp(monsternieuw,monsterindex);

	if(cmp<0) break;

	if(cmp==0)
	 {
	  verdnieuw=pos[y][x].verdunning;
	  verdindex=pos[yy][xx].verdunning;
	  cmp=verdindex-verdnieuw;
	  if(cmp>0) break;
				  /* als de verd <= aan index dan invoegen */
	 }
	}
		pas_index_aan(j,indx+1,x,y);
		putch('.');
	 }
	}
/*   for(i=1;i<indx;i++)
	printf( "%s    %f   \n",pos[index[i][1]][index[i][0]].monsternaam,pos[index[i][1]][index[i][0]].verdunning);
	printf("\n\n");
*/
return(indx);
}

void pas_index_aan(int start, int einde, int posx, int posy)
{
 int i;
  for(i=einde;i>start;i--)
	{
	 index[i][0]=index[i-1][0];
	 index[i][1]=index[i-1][1];
	}
	index[start][0]=posx;
	index[start][1]=posy;

/*   for(i=1;i<einde;i++)
	printf( "%s    %f   \n",pos[index[i][1]][index[i][0]].monsternaam,pos[index[i][1]][index[i][0]].verdunning);
	printf("\n\n");
 */
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


/******** haal datum uit ffblk.ff_fdate *************/
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
   static char filenaam[250][30];      /* nr,breedte filenaam + datum*/
   char ch;						/* te printen char */
   int begin = 0;               /* 1e file uit blok van 25 dat geprint word */
   int keuze = 0;				/* nummer gekozen file */
   int num = 0;                 /* aantal files */
   int oke;                     /* 0 = geen keuze ESC 1 = keuze RETURN */
   int toets;					/* toetsaanslag via getch() */
	int pos = 0;					/* positie binnen filenaam */
	int veld;					/* nummer van filenaam */
	int x = 0;					/* teller */
	int xx =0;                   /* row waar geprint moet worden */
	int yy = 0;                  /* colom waar geprint moet worden */
/*   int maxaantal = 110; */         /* aantal files per beeldscherm */
	int keus=0;
	int keer=0;
	int day,month,year;
	 char drive[MAXDRIVE];
    char dir[MAXDIR];
    char file[MAXFILE];
    char ext[MAXEXT];
    int flags;



   oke = findfirst((char *)filein,&ffblk,0);
   while (!oke)
      {
	  sscanf(ffblk.ff_name,"%s", &nop);
	  makedate(&day,&month,&year,ffblk.ff_fdate);
	  sprintf(filenaam[x],"%12.12s %2.2d-%2.2d-%2.2d",nop,day,month,year);
	  if(++x == 250)
		{ sound(100);
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

	 flags=fnsplit(filein,drive,dir,file,ext);
    strcpy(nop,"");
    if(flags & DRIVE)     strcat(nop,drive);
    if(flags & DIRECTORY) strcat(nop,dir);
    strcpy(filenaam[keuze],remove_lt_space(filenaam[keuze]));
    strcat(nop,filenaam[keuze]);
			   return((char*)nop);
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
   unsigned int kleur = (background<<11) + (forground<<8);
   for(;tekst[x]; *(nop+x) = tekst[x++] | kleur);
}

/**************** instellingen inlezen *************************/
void instellingen(char *drivedir)
{
int x,y,n,fout;
char item[MAXBREED][20];

   swrite(6,23,"Kies een instellingen file",0x0700);

   strcpy(nop,drivedir);
   strcat(nop,"*.ine");
   strcpy(filein,(char *)filedir(nop));


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
     if(strlen(inputline)<1) fout=TRUE;
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
	  if(item[x][0]>63)     /* Als begint met letter dan: monster,verd*/
	   {
	    strcpy(nop1,strtok(item[x],","));
	    strncpy(nop1,remove_lt_space(nop1),LEN_NAAM-1);
	    sprintf(pos[y][x].monsternaam,"%-5.5s",nop1);
		 strcpy(nop1	,strtok(NULL," "));
	    pos[y][x].verdunning=atof(nop1);
	   }
	   else 
			/* Anders is het een concentratie v/d ijklijn */
		{
		  strcpy(nop1,strtok(item[x]," "));
		  pos[y][x].concyk=atof(nop1);
		  pos[y][x].verdunning=1;
		  if(item[x][0] != '?')
			sprintf(pos[y][x].monsternaam,"0%-5.5s",nop1);
		  else
			sprintf(pos[y][x].monsternaam,"~%-5.5s",nop1);
		}

	 }   /* WHILE */
		 }     /* if fout loop */
		}      /* while loop */
fclose(fpin);

}

void prnt_prtfile(int regels,char *drivedir)
{
 int i,bars,cmp;
 char barline[80];
 char naam[9],naamvorig[9];
 float extinctie,verdunning;
 char drive[MAXDRIVE];
 char dir[MAXDIR];
 char file[MAXFILE];
 char ext[MAXEXT];
 char qdrive[MAXDRIVE];
 char qdir[MAXDIR];
 char qfile[MAXFILE];
 char qext[MAXEXT];
 int flags;

 clrscr();

 flags=fnsplit(fileout,drive,dir,file,ext);
 flags=fnsplit(drivedir,qdrive,qdir,qfile,qext);
 fnmerge(fileout,qdrive,qdir,file,ext);

 if ((fpout=fopen(fileout,"w")) == NULL)
	{
		printf("cannot open outputfile %s\n",fileout);
		exit(1);
	}
  printf("\n\nschrijven %s ",fileout);
  swrite(20,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x6400);
  swrite(20,11,"³                                           ³",0x6400);
  swrite(20,12,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x6400);
  swrite(20,13,"0%                   50%                 100%",0x1000);
  strcpy(barline,"                                           ");

	bars=(float)( (regels-1) / 41);
	strcpy(naamvorig,"#geen");
	for(i=1;i<=regels;i++)
	  {
		strcpy(naam,pos[index[i][1]][index[i][0]].monsternaam);
		verdunning=pos[index[i][1]][index[i][0]].verdunning;
		extinctie=pos[index[i][1]][index[i][0]].extinctie;
		cmp=strcmp(naamvorig,naam);
		if(naamvorig[0] != '#' && cmp != 0 && naam[0]!='0')
		fputs("   -1      -1      1\n",fpout);
		strcpy(naamvorig,naam);
/*      if(naam[0] > '@') naam[0]=32;*/
		sprintf(nop,"%-6.6s %-6.6f %-6.6f\n",naam,extinctie,verdunning);
		if(fputs(nop,fpout)==NULL)
		 {printf("Disk error : Disk full???"); exit(1);}
		strnset(barline,176,(int)(i/bars));
		swrite(21,11,barline,BARCOLOR);
		}
		fclose(fpout);

 if ((fpout=fopen("@!.bat","w")) == NULL)
	{
		printf("cannot open outputfile @!bat\nDisk Full?");
		exit(1);
	}
	strcpy(drive,"X:\\");      /* fill string with form of response: X:\ */
	drive[0] = 'A' + getdisk();    /* replace X with current drive letter */
	getcurdir(0, dir+1);  /* fill rest of string with current directory */
	fnmerge(fileout,drive,dir,file,ext);
	fprintf(fpout,"logit %s",fileout);
	fclose(fpout);
	flags=flags;

}
/* elisa inlees programma voor dick van der Schaaf, EJN maart 1993 */
/*compile small
 use lees.prj*/

#include "stdio.h"
#include "io.h"
#include "conio.h"
#include "stdlib.h"
#include "dos.h"
//#include "graphics.h"
#include "dir.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define TRUE  1
#define FALSE 0
#define BARCOLOR 0X0700

//#define MAX_MENU_ITEMS 20      /* Maximaal aantal menu keuzes */
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
void   initdisplay(void);
void   veeg(int topy,int topx,int totl,int totb);
void   lveeg(int topy,int topx,int totl,int totb,char kar,unsigned kleur);
char   *remove_lt_space(char *inputline);
void   instellingen(char *drivedir);
void   prnt_prtfile(int,char *drivedir);

int far *SCHERMPTR,far *PTR;
FILE *fpin,*fpout;
char tfileload[40];
char filein[32],fileout[32];
unsigned disk;
char *directory;
char defaultdir[132];
char curdir[132];
struct ffblk ffblk;
   char bufr[24576];
   char obufr[24576];


char  inputline[255],nop[255],nop1[25];
char insteldrive[32];
char extinctiesdrive[32];
char outputdrive[32];

/***************************  main ***************************/
void main(int argc,char **argv)
{
int aantalindex;
char file[15];
char ffile[18];

initdisplay();
clrscr();
strcpy(extinctiesdrive,"C:");
strcpy(outputdrive,"C:");

if(argc>1)
 {
  strcpy(extinctiesdrive,argv[1]);
//  c=extinctiesdrive[strlen(insteldrive)-1];
//  if(c != ':')
//    if(c != '\\') strcat(extinctiesdrive,"\\");
 }
if(argc>2)
{
  strcpy(outputdrive,argv[2]);
//  c=outputdrive[strlen(insteldrive)-1];
//  if(c != ':')
//    if(c != '\\') strcat(outputdrive,"\\");
 }

/*instellingen(insteldrive);*/
 read_elisa_file(extinctiesdrive);
/*prnt_prtfile(aantalindex,outputdrive);*/


}

/********** extincties inlezen en samenvoegen met instellingen ********/
int read_elisa_file(char *drivedir)
{
	int b1,b2,m,n,i,j,x,y,fout,offset,indx,xx,yy,cmp,dummy,lastchar;
	char monsternieuw[9],monsterindex[9];
	float verdnieuw,verdindex,cmpf;
	char mnieuw[9],mindex[9];
	int regel,cols,xpos=0;
	long int count;
	char *token,nopp[75];
	float bars;
	char *p;
//   char ext[15][15];
	char drive[MAXDRIVE];
	char dir[MAXDIR];
	char file[MAXFILE];
	char ext[MAXEXT];
	int flags;
	char firstitem[20];
	char keten[5];
	char ketenoud[5];
	char aantalaa[10];
	char aminoacid[5];
	char line[80];
	int linepos=0;
	keten[0]=ketenoud[0]=0;



//   swrite(6,23,"Kies een  file",0x0700);


	strcpy(nopp,drivedir);
	if (drivedir[strlen(drivedir)-1] =='\\' ||
		 drivedir[strlen(drivedir)-1] ==':' )
		strcat(nopp,"unknown.ent");

//   strcpy(filein,(char *)filedir(nopp));
	strcpy(filein,nopp);

	if ((fpin=fopen(filein,"rb")) == NULL)
	{
		printf("cannot open input-file %s\n",filein);
		exit(1);
	}

	/* set up input stream for minimal disk access,
		using our own character buffer */
	if (setvbuf(fpin, bufr, _IOFBF, 24576) != 0)
      printf("failed to set up buffer for input file\n");
   else
		printf("buffer set up for input file\n");
	strcpy(fileout,filein);

flags=fnsplit(filein,drive,dir,file,ext);

printf("\\\\\\\info:\n");
if(flags & DRIVE)
   printf("\tdrive: %s\n",drive);
if(flags & DIRECTORY)
   printf("\tdirectory: %s\n",dir);
if(flags & FILENAME)
   printf("\tfile: %s\n",file);
if(flags & EXTENSION)
   printf("\textension: %s\n",ext);

 strcpy(nop,outputdrive);
 strcat(nop,dir);
 strcat(nop,file);
 strcat(nop,".aa");
 strcpy(fileout,nop);


//	fileout[strlen(fileout)-1]='@';
	if ((fpout=fopen(fileout,"wb")) == NULL)
	{
		printf("cannot open output-file %s\n",fileout);
		exit(1);
	}
	/* set up output stream for line buffering using space that
		will be obtained through an indirect call to malloc */
	if (setvbuf(fpout, obufr, _IOFBF, 24576) != 0)
		printf("failed to set up buffer for output file\n");
	else
		printf("buffer set up for output file\n");

	/* perform file I/O here */


	printf("\n\nInlezen %s ",filein);
  swrite(30,0,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x0700);
  swrite(30,1,"³                                           ³",0x0700);
  swrite(30,2,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x0700);
  swrite(30,3,"0%                   50%                   100%",0x0700);
  strcpy(nopp,"                                           ");

	count=30;
	bars=(float)( filelength(fileno(fpin)) ) / 41;
	regel=10;
	b1=b2=0;

	while(TRUE)
	 {
		if(fgets(inputline,255,fpin)==NULL)    break;
		count+=strlen(inputline)+2;
		strnset(nopp,176,(int)(count/bars));
		b1=count/bars;
		if(b1>b2){ 		swrite(31,1,nopp,BARCOLOR);	b2=b1;		}
		m=0;
		fout=FALSE;
		if(strlen(inputline)<10)  fout=TRUE;
	if(!fout)
	{
	x=1;
	strcpy(inputline,remove_lt_space(inputline)); /* verwijder spaties */
//	strcat(inputline,"");
//	strcpy(nop,"$ ");
//	strcat(nop,inputline);
//	strcpy(inputline,nop);
	strcpy(firstitem,strtok(inputline," "));    /* neem lijnbeschrijving uit lijn */
	if(strcmp(firstitem,"SEQRES")==0)
	{
	linepos=0;
	while(1)
		{
		p=strtok(NULL," ");                      /* neem regelnr uit lijn */
		if (p==NULL) {fout=TRUE; break;}
		p=strtok(NULL," ");                      /* neem keten  uit lijn */
		if (p==NULL) {fout=TRUE; break;}
		strcpy(keten,p);
		p=strtok(NULL," ");                      /* neem aantalaa uit lijn */
		strcpy(aantalaa,p);
		if (p==NULL) {fout=TRUE; break;}
		while(1)
		  {
			p=strtok(NULL," ");                      /* neem keten  uit lijn */
			if (p==NULL) {fout=TRUE; break;}
			strcpy(aminoacid,p);
			if(strcmp(aminoacid,"ALA")==0) line[linepos++]='A';
			if(strcmp(aminoacid,"ARG")==0) line[linepos++]='R';
			if(strcmp(aminoacid,"ASN")==0) line[linepos++]='N';
			if(strcmp(aminoacid,"ASP")==0) line[linepos++]='D';
			if(strcmp(aminoacid,"CYS")==0) line[linepos++]='C';
			if(strcmp(aminoacid,"GLU")==0) line[linepos++]='E';
			if(strcmp(aminoacid,"GLN")==0) line[linepos++]='Q';
			if(strcmp(aminoacid,"GLY")==0) line[linepos++]='G';
			if(strcmp(aminoacid,"HIS")==0) line[linepos++]='H';
			if(strcmp(aminoacid,"ILE")==0) line[linepos++]='I';
			if(strcmp(aminoacid,"LEU")==0) line[linepos++]='L';
			if(strcmp(aminoacid,"LYS")==0) line[linepos++]='K';
			if(strcmp(aminoacid,"MET")==0) line[linepos++]='M';
			if(strcmp(aminoacid,"PHE")==0) line[linepos++]='F';
			if(strcmp(aminoacid,"PRO")==0) line[linepos++]='P';
			if(strcmp(aminoacid,"SER")==0) line[linepos++]='S';
			if(strcmp(aminoacid,"THR")==0) line[linepos++]='T';
			if(strcmp(aminoacid,"TRP")==0) line[linepos++]='W';
			if(strcmp(aminoacid,"TYR")==0) line[linepos++]='Y';
			if(strcmp(aminoacid,"VAL")==0) line[linepos++]='X';
		  }
		}
	}
	if(strcmp(keten,ketenoud)!=0) fprintf(fpout,"\r\n%s: %s %s ",filein,keten,aantalaa);
	line[linepos++]=0;
	linepos=0;
	fputs(line,fpout);
	strcpy(ketenoud,keten);
	swrite(xpos,regel,line,0x1700);
	xpos+=strlen(line);
	if(xpos>66) { xpos=0;  regel++;}
	if(regel>23) regel=10;

  }

 }




 fcloseall();
clrscr();
return(0);
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
		   if(keuze > num - 1)    keuze = num - 1;
		   if(keuze < 69)         begin = 0;
		   else                   begin = ((keuze-69) / 3 ) * 3;
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
			strcpy(nop,filenaam[y]);
			remove_lt_space(nop);
		 if(strnicmp(&nop[0],&ch,1) == 0)
		  {
		keuze = y;  oke = 0; leave=1;  keer=2;
		if(keuze < 69)  begin = 0;
		   else  begin = ((keuze-69) / 3 ) * 3;
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








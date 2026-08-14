/* Menu.c Ed Nieuwenhuys*/

#include "alloc.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "process.h"
#include "dir.h"
#include "time.h"
#include "dos.h"
#include "errno.h"
#include "conio.h"

#define MENU_breedte_item 19
#define MENU_lang 17
#define MENU_topy 5

#define COMAX 80
#define MAXFILES 90

int far *SCHERMPTR;
char file[MAXFILES][25];
char titel[MAXFILES][40]; /*MENU_breedte_item];*/
char path[MAXFILES][25];
char arg[MAXFILES][25];
char arg2[MAXFILES][25];
char fileload[14];
unsigned char curstart,curend;
char curdir[40];
int HERC=0;
int num,keuze=0;
enum opcode { Dir=101,Input,Output,File,Menu };

void einde();
void swrite(char posx,char posy,char *tekst,int kleur);
void boxx(int topy,int topx,int totl,int totb,int soort,int kleur);
void veeg(int topy,int topx,int totl,int totb);
void initdisplay();
void cursoroff();
void cursoron();
void mode(int mode_code);
void palette(int pnum);
void error(int errornum,int errr);
void getstartdir(char * cdir);
unsigned char fcls[20];

main()
{
   ltoa((long)farcoreleft()+89728L,fcls,10);
   strcat(fcls," memory\0");
	 initdisplay();
	 num=load_dir();                   /* laad menu.dir file */
    while(1)
    {
      initdisplay();
      mode(3);
      palette(0);
      veeg(0,0,25,80);
      menu00();
    }
}

void swrite(char posx,char posy,char *tekst,int kleur)
  {
   int far *LOC;
   int x = 0;
   if (HERC)
     {
      if (kleur<0x0F01) kleur=0x0700;
      else kleur=0x7000;
      }
   LOC = SCHERMPTR + posy*80 + posx;
   while(tekst[x] )   *(LOC++) = tekst[x++] | kleur;
  }


int menu00()                        /* gegevens */
{
   int nr,breed,nop,errorr;
   int wait_overlay;
   char pathfile[60],nopc[5];
   char *ptr;
   cursoroff();
   veeg(0,0,25,80);
   getstartdir(curdir);
   keuze = menuver(titel,num);
   veeg(0,0,25,80);
   if (keuze==998)  return(998);
   cursoron();
   if (keuze==999)  einde();
   strcpy(pathfile,strupr(path[keuze]));
   if (path[keuze][strlen(path[keuze])-1] != '\\')
			 strcat(pathfile,"\\");
   strcat(pathfile,file[keuze]);

/* swrite(30,10," Loading:    \0",0x6100);
   swrite(39,10,strupr(file[keuze]),0x6100);  */

   if (path[keuze][1]==':')     setdisk((path[keuze][0])-'A');
   if(chdir(path[keuze]) )      error(errno,Dir);

	if (stricmp(file[keuze],"STOP")==0) einde();
   if(strstr(strupr(file[keuze]),".BAT")!=0)   /****Als BAT file****/
	{
	if (stricmp(arg[keuze],"STOP")==0)
        	{system(file[keuze]);  einde(); }
	else     system(strcat(strcat(file[keuze]," "),arg[keuze]));
                 chdir(curdir);
                 cursoroff();
                 return(keuze);
	 }

  if (stricmp(arg[keuze],"STOP")==0)
	   errorr=execl(pathfile,NULL,NULL);
  else if (stricmp(arg2[keuze],"STOP")==0)
	   errorr=execlp(pathfile,arg[keuze],NULL,NULL);
  else if (arg2[keuze][0]==0)
	   errorr=spawnlp(P_WAIT,pathfile,file[keuze],arg[keuze],NULL,NULL);
  else 	   errorr=spawnlp(P_WAIT,pathfile,file[keuze],arg[keuze],arg2[keuze],NULL,NULL);

/*  if(errorr && errorr!=9) error(errorr,File);*/

  setdisk((curdir[0]-'A'));
  if(chdir(curdir)) error(0,Dir);
  cursoroff();
  errorr=errorr;
  return(keuze);
}

void einde()
{
setdisk((curdir[0]-'A'));
if(chdir(curdir)) error(0,Dir);
clrscr();
cursoron();
exit(-1);
}


										  /* verticaal menu */
int menuver(menu,num)
char menu[MAXFILES][40];
int  num;
{
   long secs_now;
   long secs_start;
   char *str_now;
   char s[90],t[25];
   char uur[5],min[5],sec[5];
   int iuur,imin,isec,isecoud=99;
   int x, y, code, code1, oke, yy, xx, keer,leave,keus,n;
   int keuze,breed,topx,keuzeoud,menu_lang;
   unsigned int kleur,kleur1,kleur2;
   char ch;
   int no_disks;
   int save, disk;
   unsigned char c;
   long bytesfree=0;
   struct dfree dfres;
   kleur=0x0A00;
   kleur1=0x2F00;
   kleur2=0x2800;

   if (HERC) { kleur = 0x0700; kleur1 = kleur2 = 0x7000; }

   breed = ( 1+ (num-1) / MENU_lang) * MENU_breedte_item;
   menu_lang = (num+1) / (breed / MENU_breedte_item);

   yy = MENU_topy;
   topx = 40 - breed / 2;
	xx=topx;
   oke = 0;
   s[80]=0;
   c=196;
   for (ch=0;ch<80;s[ch++]=c);
   swrite(0,0,s,0x1700);
   swrite(0,2,s,0x1700);

   c=32;
   for (ch=0;ch<80;s[ch++]=c);

   swrite(0,1,s,0x1700);
   swrite(0,24,s,0x0700);

   boxx(yy-1,xx-2,menu_lang,breed+1,2,1);

   swrite(1,1,fcls,0x1700);
   keuzeoud = keuze = 0;
   swrite(19,24,"SPACE , CURSORS OR FIRST LETTER TO SELECT\0",0X0700);
   textcolor(BLACK);
   textbackground(WHITE);
   veeg(0,72,4,8);
   for(n=0;n<25;n++)
     {
      gotoxy(72,25-n);
      itoa(n,s,10);
      cprintf("%02s",s);
     }
   for(n=0;n<61;n=n+3)
     {
      gotoxy(77,25-n/3);
		itoa(n,s,10);
      cprintf("%02s",s);
     }


	 for (y=0; y<num; y++)
	    {
	       yy = MENU_topy + y % menu_lang;
	       xx = (topx - 1)+(y / menu_lang) * MENU_breedte_item;

	       for (x=0; x<strlen(menu[y]); x++)
		  {
		   xx++;
		   ch = menu[y][x];
		   if (y==keuze )
			   *(SCHERMPTR + yy*COMAX + xx) = ch | kleur ;
		     else
			if (x==0)
			   *(SCHERMPTR + yy*COMAX + xx) = ch | kleur1;
			else
			   *(SCHERMPTR + yy*COMAX + xx) = ch | kleur2;
		  }
            }

   /* save original drive */
   save = getdisk();

   for (disk = 0;disk < 26;++disk)
   {
      setdisk(disk);
      if (disk == getdisk())
			no_disks=disk+1;
   }
   setdisk(save);



  if (bytesfree==0)
     {
   while(1){
	  if(kbhit()) break;
	  if(no_disks >=3)
	  {
		getdfree(3,&dfres);
		bytesfree=(long)dfres.df_avail*(long)dfres.df_sclus*(long)dfres.df_bsec;
		ltoa(bytesfree/1000,t,10);
		strcpy(s,"C:");
		if (bytesfree<1) strcpy(t,"0");
		strcat(s,t);
		if(no_disks==3) strcat(s," Kb");
		swrite(16,1,s,0x1E00);
	  }
	  if(kbhit()) break;
	  if(no_disks>=4)
	   {
		getdfree(4,&dfres);
		bytesfree=(long)dfres.df_avail*(long)dfres.df_sclus*(long)dfres.df_bsec;
		ltoa(bytesfree/1000,t,10);
		strcpy(s,"D:");
		if (bytesfree<1) strcpy(t,"0");
		strcat(s,t);
		if(no_disks==4) strcat(s," Kb");
		swrite(24,1,s,0x1E00);
		}
/*	  if(kbhit()) break;
	  if(no_disks>=5)
		{
		getdfree(5,&dfres);
		bytesfree=(long)dfres.df_avail*(long)dfres.df_sclus*(long)dfres.df_bsec;
		ltoa(bytesfree/1000,t,10);
		strcpy(s,"E:");
		if (bytesfree<1) strcpy(t,"0");
		strcat(s,t);
		if(no_disks>=5) strcat(s," Kb");
		swrite(32,1,s,0x1E00);
		}
*/	break;
       }
     }

   while (1)
      {
	  if(oke == 1) break;
	  time(&secs_start);
	  while(bioskey(1)==0)
	     {
           time(&secs_now);
           str_now=ctime(&secs_now);
           xx=44;
           str_now[24]='\0';
	   swrite(xx,1,str_now,0x1300);
           strncpy(uur,str_now+11,2);
           strncpy(min,str_now+14,2);
			  strncpy(sec,str_now+17,2);
           uur[3]=0;
           min[3]=0;
           sec[3]=0;
           iuur=atoi(uur);
           imin=atoi(min);
	   isec=atoi(sec);
           if(isec==0 && isecoud==59){
            veeg(0,73,24,2);
            veeg(4,78,21,1);
            }
           for(n=0;n<=iuur;n++)     swrite(73,24-n,"U\0",0x1E00);
/*           if(iminoud!=imin)
           {
*/           for(n=0;n<=imin;n++)    swrite(75,24-n/3,"M\0",0x2E00);
           if( n%3 == 1 )          swrite(75,24-n/3,"0\0",0x2E00);
           if( n%3 == 2 )          swrite(75,24-n/3,"1\0",0x2E00);
/*           }*/
           if(isecoud!=isec)
           {
           for(n=0;n<=isec;n++)     swrite(78,24-n/3,"S\0",0x3E00);
           if( n%3 == 1 )          swrite(78,24-n/3,"0\0",0x3E00);
           if( n%3 == 2 )          swrite(78,24-n/3,"1\0",0x3E00);
           }
//           iminoud=imin;
	   isecoud=isec;
//           iuuroud=iuur;


		if(secs_now - secs_start >60)
		 {
			spawnlp(P_WAIT,"STANDBY.exe","STANDBY.exe",NULL);
		   return(998);
		 }
	    }
         textbackground(BLACK);

         code = getch();
         code1=0;
	 keuzeoud = keuze;
         if(code==32) {code1=32; code=0;}
	 if(code == 0)
	    {
	    if(code1 !=32)  code1 = getch();
	    switch (code1)
	       {
		   case 32: keuze++; 		break;
		   case 71: keuze = 0; 		break;
		   case 72: keuze--; 		break;
		   case 73: keuze -= 5;		break;
		   case 75: keuze -= menu_lang;	break;
		   case 77: keuze += menu_lang;	break;
		   case 79: keuze = num-1;	break;
		   case 80: ++keuze; 		break;
		   case 81: keuze += 5; 	break;
	       }
	if(keuze < 0)        keuze = num-1;
	if(keuze > num-1)    keuze = 0;
	    }
	 else
	    {
	    ch = code;
	    keus=keuze;
	    leave=0;
	    keer=-1;
	    while(keer<2)
	      {
	       keer++;
	       if(keer==1) { keus=-1;}
	       for(y=keus+1; y<num; y++)
		{
		 if(strnicmp(&menu[y][0],&ch,1) == 0)
		  {
		  keuze = y;
		  oke = 0;
		  leave=1;
		  keer=2;
		  }
     if(leave) break;
	       }
	     }

	    if(ch == 13)   oke = 1;
	    if(ch == 27)   einde();
	    }

	       yy = MENU_topy + keuzeoud % menu_lang;
	       xx = (topx - 1)+(keuzeoud / menu_lang) * MENU_breedte_item;

	    for (x=0; x<strlen(menu[keuzeoud]); x++)
		{
		if (x==0)
		  *(SCHERMPTR + yy*COMAX + ++xx) = menu[keuzeoud][x] | kleur1;
		else
		  *(SCHERMPTR + yy*COMAX + ++xx) = menu[keuzeoud][x] | kleur2;
		  }

	       yy = MENU_topy + keuze % menu_lang;
	       xx = (topx - 1)+(keuze / menu_lang) * MENU_breedte_item;

	       for (x=0; x<strlen(menu[keuze]); x++)
		  *(SCHERMPTR + yy*COMAX + ++xx) = menu[keuze][x] | kleur ;
      }
 return(keuze);
}

                                /* print boxx */
void boxx(int topy,int topx,int totl,int totb,int soort,int kleur)
{
   int x, y, ck;
   unsigned char clb, crb, clo, cro, ch, cv;
   int far *LOC;

   if(soort == 1)
      {
      clb = 218;      crb = 191;      clo = 192;
      cro = 217;      ch  = 196;      cv  = 179;
      }
   else
      {
      clb = 201;      crb = 187;      clo = 200;
      cro = 188;      ch  = 205;      cv  = 186;
      }

   if(kleur == 0)      ck = 0x2700;
   else                ck = 0x4000;
   if(HERC)            ck = 0x7000;
   LOC = SCHERMPTR + topy*COMAX + topx;

   *(LOC++) = clb | ck;        			 /* boven kant */
   for(x=1; x<totb-1; x++)   *(LOC++) = ch | ck;
   *(LOC) = crb | ck;

   for(x=0; x<totl; x++)     			  /* middenstuk */
      {
        LOC = SCHERMPTR + (topy+x+1)*COMAX + topx;
        *(LOC++) = cv | ck;
	for(y=1; y<totb-1; y++)  *(LOC++) = 0x20 | ck;
        *(LOC) = cv | ck;
      }

   LOC = SCHERMPTR + (topy+totl+1)*COMAX + topx;
   *(LOC++) = clo | ck;     			  /* onderkant */
   for(x=1; x<totb-1; x++)   *(LOC++) = ch | ck;
   *(LOC) = cro | ck;
}


void veeg(int topy,int topx,int totl,int totb)       /* veeg scherm schoon */
{
int y, x;
 for(y=topy; y<topy+totl; y++)
      for(x=topx; x<topx+totb; x++)
		 *(SCHERMPTR + y*COMAX + x) = 'Î' | 0x4600;
}


int load_dir()
{
   int nr;
   FILE *fp;
   char str[80];
   char nop[80];
   char *sp="               ";
   char *p;

   if ((fp=fopen("menu.dir","r")) == NULL) error(0,Menu);
   nr=0;
   while ( fgets(str,79,fp) != NULL )
     {
      strcpy(nop,strtok(str,"|"));
      strcat(nop,sp);
      nop[MENU_breedte_item-3]=0;
      arg[nr][0] = 0;
      arg2[nr][0]= 0;
      strcpy(titel[nr],nop);
      strcpy(path[nr],strtok(NULL,"|\r\n"));
      strcpy(file[nr],strtok(NULL,"|\r\n"));
      p=strtok(NULL,"|\r\n");
      if(p) strcpy(arg[nr],p);
      p=strtok(NULL,"|\r\n");
      if(p) strcpy(arg2[nr],p);
      nr++;
     }
  return(nr);
 }


void initdisplay()
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int far *)( (reg.h.al != 7) ? 0XB8000000L : 0XB0000000L);
HERC=(int) (reg.h.al !=7) ? 0 : 1 ;
}


void cursoroff()
{
_setcursortype(_NOCURSOR);
}

void cursoron()
{
mode(3);
}

void mode(int mode_code)
{
 union REGS r;
 r.h.al=mode_code;
 r.h.ah=0;
 int86(0x10,&r,&r);
}

void palette(int pnum)
 {
  union REGS r;
  r.h.bh=1;
  r.h.bl=pnum;
  r.h.ah=11;
  int86(0x10,&r,&r);
 }


/* ERROR handler
   include error.h
    error nummer >100 kunnen als eigen errors worden gebruikt
    als errornum == 0 dan worden de eigen errornummers gebruikt
 */
void error(int errornum,int errr)
{
char nop[80];
int color=0xE100;

swrite(24,5,"                             \0",color);
swrite(24,6,"                             \0",color);
if(errornum==0) errornum=errr;

if (errornum<100)
  {
     strcpy(nop,strerror(errornum));
     nop[strlen(nop)-1]=0;
     swrite(25,5,nop,color);
  }
  else
   {
  switch (errornum)
    {
	case Dir:
	       swrite(25,5,"Directory not found\0",color);
	       swrite(55,5,curdir,color);
	       break;
        case Input:
	      swrite(25,5,"Can not open input file\0",color);
 	      break;
        case Output:
	      swrite(25,5,"Can not open output file\0",color);
	      break;
        case File:
	      swrite(25,5,"File not found\0",color);
	      break;
        case Menu:
	      swrite(25,5,"Can not find MENU.DIR\0",color);
	      break;
        default:
	      swrite(25,5,"Unknown Error  \0",color);
        }
      }
      swrite(25,6,"Error:   \0",color);
      swrite(32,6,itoa(errornum,nop,10),color);

  swrite(24,8," Press any key to continue   \0",0x3100);
  chdir(curdir);
  getch();
  einde();
  }


void getstartdir(char * cdir)
{

   strcpy(cdir,"?:\\");
   curdir[0]='A'+getdisk();
   getcurdir(0,cdir+3);
}

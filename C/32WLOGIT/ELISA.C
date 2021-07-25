// elisa inlees programma om prt files aan te maken EJN
//AANPASSING WINDOWS FEB 1995

#include "wlogit.h"

int	 Elisa(HWND hwnd);
int    read_elisa_file(HWND hwnd);
void   pas_index_aan(int, int , int, int );
char   *remove_lt_space(char *inputline);
void   instellingen(HWND hwnd);
void   prnt_prtfile(int,HWND hwnd);
extern BOOL ElisaFileOpen( HWND hwnd );
extern BOOL InstelFileOpen( HWND hwnd );
extern char *Extinctiefile,*Instelfile;
extern FILE *fpin;

#define MAXBREED 13
#define MAXLANG 13
#define LEN_NAAM 8
struct elisa {
	float verdunning;			 			//array voor 1 plaatje
	char monsternaam[LEN_NAAM];		// monsternamen 5 characters
	float concyk;
	float extinctie;
} pos[MAXLANG][MAXBREED];

int index[MAXLANG*MAXBREED+5][2];	// array voor sort index
int gelukt=FALSE;												// index[?][0] = x
												// index[?][1] = y
char  inputline[255],nop[255],nop1[25];

/***************************  main ***************************/
int Elisa(HWND hwnd)
{
int aantalindex;
gelukt=FALSE;
instellingen(hwnd);
if (gelukt)
	{
	gelukt=FALSE;
	aantalindex=read_elisa_file(hwnd);
	}
if (gelukt)
	{
	gelukt=FALSE;
	prnt_prtfile(aantalindex,hwnd);
	}
return gelukt;
}

/********** extincties inlezen en samenvoegen met instellingen ********/
int read_elisa_file(HWND hwnd)
{
	int n,xx,yy,j,x,y,fout,indx,cmp,dummy;
	char monsternieuw[9],monsterindex[9],*p;
	float verdnieuw,verdindex,cmpf;
	int regel=0,cols;
	long int count;
	char ext[15][15];

	CMUFileOpen(hwnd,2);
	if ((fpin=fopen(Loaded_file_name,"rb")) == NULL)
	{
	 gelukt=FALSE;
	 MessageBox (hwnd, "Can not open inputfile \n","Disk fault?",MB_OK | MB_ICONQUESTION);
	 return(gelukt);
	}
	while(TRUE)
	 {
		if(fgets(inputline,255,fpin)==NULL)    break;
//		count+=strlen(inputline)+2;
		strcpy(nop,"  ");
		strcat(nop,inputline);
		for(n=0;n<strlen(inputline);n++)
		{                       /*verwijder tekst en verander , of * in . */
			if(inputline[n]==',')
			{
			 if(inputline[0]==',') inputline[n]=' ';
			 else if(isdigit(inputline[n-1])) inputline[n]='.';
			}
			if(inputline[n]=='*') inputline[n]='.';
			if(inputline[n]=='-') inputline[n]=' ';
			if(inputline[n]=='+') inputline[n]=' ';
			if(inputline[n]>57) inputline[n]=' ';
			if(inputline[n]<32) inputline[n]=' ';
		}

	strcpy(inputline,remove_lt_space(inputline));
	fout=FALSE;
	if(strlen(inputline)<25)  fout=TRUE;
	if(!fout)
	{
	x=1;
//	strcpy(inputline,remove_lt_space(inputline)); // verwijder spaties
//	strcat(inputline,"");
	strcpy(nop,"$ ");
	strcat(nop,inputline);
	strcpy(inputline,nop);
	strcpy(ext[x],strtok(inputline," "));    // neem ext uit lijn
	while(1)
	{
	p=strtok(NULL," ");                      // neem item uit lijn
	if (p==NULL) break;
	if(strlen(p)>3)strcpy(ext[x++],p);
//	if(strlen(p)<4) x--;                     // als geen extinctie
	}
/*	if(x>7)                            // Als geen regel is weggegooid
	{
	 for(n=1;n<x;n++)	if(ext[n][1]!='.')  fout=TRUE;     // is extinctie ?
	}
	else fout=TRUE;
*/
   if(x<8) continue;
	if(!fout)
	 {
		cols=x;
		regel++;                                 /* ingelezen goede regel */
		if (regel>12)
		 {
		 MessageBox(hwnd,"Too many lines\nMore than one plate in file?\nNext plates discarded",
								"Error",MB_OK);
		 fout=TRUE;
		 break;
		 }
		if(x>10)	for(x=1;x<13;x++)		pos[regel][x].extinctie=atof(ext[x]);
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
//  clrscr();
//  swrite(30,20,"Sorteren\0",0x2400);
  regel=(regel*(cols-1))/12;
  for (y=1;y<=regel;y++)
	{
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
	 }
	}
/*   for(i=1;i<indx;i++)
	printf( "%s    %f   \n",pos[index[i][1]][index[i][0]].
	monsternaam,pos[index[i][1]][index[i][0]].verdunning);
	printf("\n\n");
*/
gelukt=TRUE;
return(indx);
}
//-----------------------------------------------------------------------
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

/**************** instellingen inlezen *************************/
void instellingen(HWND hwnd)
{
int x,y,fout;
char item[MAXBREED][20];

	CMUFileOpen(hwnd,1 );
	if ((fpin=fopen(Loaded_file_name,"rb")) == NULL)
	{
	 gelukt=FALSE;
	 MessageBox (hwnd, "Can not open inputfile \n","Disk fault?",MB_OK | MB_ICONQUESTION);
	 return;
	}
	x=y=0;
	while(TRUE)
	 {
	  if(fgets(inputline,255,fpin)==NULL)    break;
	  strcpy(inputline,remove_lt_space(inputline));
	  fout=FALSE;
	  if(inputline[0]=='%')   fout=TRUE;
	  if(strlen(inputline)<30) fout=TRUE;
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
		 pos[y][x].concyk=MISSING;
		 strcpy(nop1	,strtok(NULL," "));
		 pos[y][x].verdunning=atof(nop1);
		}
		else
			/* Anders is het een concentratie v/d ijklijn */
		{
		  strcpy(nop1,strtok(item[x]," "));
		  pos[y][x].concyk=atof(nop1);
		  pos[y][x].verdunning=1;
		  sprintf(pos[y][x].monsternaam,"IJK");
		  if(item[x][0] != '?')
			sprintf(pos[y][x].monsternaam,"0%-5.5s",nop1);
		  else
			sprintf(pos[y][x].monsternaam,"~%-5.5s",nop1);
		}

	 }   /* WHILE */
		 }     /* if fout loop */
		}      /* while loop */
fclose(fpin);
gelukt=TRUE;
}

void prnt_prtfile(int regels,HWND hwnd)
{
 int i;
 hwnd=hwnd;
 for(i=1;i<=regels;i++)
	  {
		if(pos[index[i][1]][index[i][0]].concyk!=MISSING)
		  {
		  data[0][i]=(double)pos[index[i][1]][index[i][0]].concyk;
		  strcpy((char*)naam[i]," Cal");
		  }
		else
			{
			strcpy((char*)naam[i]," ");
			strcat((char*)naam[i],pos[index[i][1]][index[i][0]].monsternaam);
			data[0][i]=MISSING;
			}
		data[1][i]=(double)pos[index[i][1]][index[i][0]].extinctie;
		data[2][i]=(double)pos[index[i][1]][index[i][0]].verdunning;
		}
// strcpy((char*)label[0]," Concentration");
// strcpy((char*)label[1]," Extinction");
 geladen = TRUE;
 rows=i-1;
 columns=3;
 gelukt=TRUE;
}

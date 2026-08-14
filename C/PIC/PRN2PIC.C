/* assembly lotus.pic file RCA 021087 EJN 100290*/
#include "stdio.h"
#include "string.h"
#include "dos.h"
#include "conio.h"

#define TRUE  1
#define FALSE 0
#define NULLPOINTER (char *) 0
#define MAXLEN 255
#define PIC_KEYWORDS 24
#define POSITION_KEYWORDS 9
#define NOOFKEYWORDS PIC_KEYWORDS + POSITION_KEYWORDS
#define MOVE 160
#define DRAW 162
#define FILL 48
#define FILLO 208
#define COLOR0 176
#define COLOR1 177
#define COLOR2 178
#define COLOR3 179
#define COLOR4 180
#define COLOR5 181
#define COLOR6 182
#define COLOR7 183
#define COLOR8 184
#define COLOR9 185
#define COLOR10 186
#define COLOR11 187
#define COLOR12 188
#define COLOR13 189
#define COLOR14 190
#define COLOR15 191
#define TEXT 168
#define FONT 167
#define SIZE 172
#define END 96
#define CENTER 0
#define CENTERLEFT 1
#define CENTERTOP 2
#define CENTERRIGHT 3
#define CENTERBOTTOM 4
#define TOPLEFT 5
#define TOPRIGHT 6
#define BOTTOMLEFT 7
#define BOTTOMRIGHT 8

struct {
   char keyword[13];
   unsigned char op;
} optable[NOOFKEYWORDS]= {
   "COLOR0",COLOR0,
   "COLOR1",COLOR1,
   "COLOR10",COLOR10,
   "COLOR11",COLOR11,
   "COLOR12",COLOR12,
   "COLOR13",COLOR13,
   "COLOR14",COLOR14,
   "COLOR15",COLOR15,
   "COLOR2",COLOR2,
   "COLOR3",COLOR3,
   "COLOR4",COLOR4,
   "COLOR5",COLOR5,
   "COLOR6",COLOR6,
   "COLOR7",COLOR7,
   "COLOR8",COLOR8,
   "COLOR9",COLOR9,
   "DRAW",DRAW,
   "END",END,
   "FILL",FILL,
   "FILLO",FILLO,
   "FONT",FONT,
   "MOVE",MOVE,
   "SIZE",SIZE,
   "TEXT",TEXT,
   "CENTER",CENTER,
   "CENTERLEFT",CENTERLEFT,
   "CENTERTOP",CENTERTOP,
   "CENTERRIGHT",CENTERRIGHT,
   "CENTERBOTTOM",CENTERBOTTOM,
   "TOPLEFT",TOPLEFT,
   "TOPRIGHT",TOPRIGHT,
   "BOTTOMLEFT",BOTTOMLEFT,
   "BOTTOMRIGHT",BOTTOMRIGHT
};
char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char    outfile[]  = "PIC.PIC";


int far *SCHERMPTR;
int *PTR  = (int *) 0XB800L ;



int	main(argc, argv)
int	argc;
char	*argv[];
{
   int op,i,found;
   long int count;
   unsigned char bytes,c,n,direction,position,size,vertices,screenprint;
   char text[MAXLEN], inputline[MAXLEN], argument[10];
   char posstring[20];
   char *token;
   double h_size,v_size;
   char nopp[80];
   float bars;
   FILE *fpin, *fpout;

   clrscr();
   initdisplay();

   swrite(20,2,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x3400);
   swrite(20,3,"³          PRN naar PIC vertaler          ³",0x3400);
   swrite(20,4,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x3400);
   sprintf(nopp,"     Inputfile : %s    ------>                 ",argv[1]);
   nopp[45]=0;
   swrite(0,7,nopp,0x1600);
   sprintf(nopp,"Outputfile : %s                           ",outfile);
   nopp[40]=0;
   swrite(40,7,nopp,0x1600);

   if (argc==1) {
	   swrite(3,20,"USE: PRN2PIC textfile-name\0",0300);
       exit(-1);
   }
   if ((fpin=fopen(argv[1],"rb")) == NULL) {
       printf("cannot open input-file %s\n",argv[1]);
       exit(-1);
   }
   if ((fpout=fopen(outfile,"wb")) == NULL) {
       printf("cannot open output-file %s\n",outfile);
       exit(-1);
   }

  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2700);
  strcpy(nopp,"                     ");

   bars=(float)( filelength(fileno(fpin)) ) / 21;



   screenprint=(argc>2);
   for (n=0;n<17;n++){
      c=header_vector[n];
      if (screenprint) printf("%3X ",c);
      putc(c,fpout);
   }
   if (screenprint) putchar('\n');
   count=30;
   do {

	  fgets(inputline,MAXLEN,fpin);
	  count+=strlen(inputline);
	   strnset(nopp,176,(int)(count/bars));
	   swrite(31,11,nopp,0x3400);
	  token=strtok(inputline,", ;");
      found=FALSE;
	  for (n=0;n<PIC_KEYWORDS;n++)
		{
		 if (strcmp(token,optable[n].keyword)==0)
		  {
            op=optable[n].op;
            found=TRUE;
            break;
         }
      }
      if (found==FALSE) {
         printf(" onbekend keyword \"%s\" in regel %4ld\n",token,count);
         exit(-1);
      }
      if (screenprint) printf("%s (%2X):",token,op);
      else fputc(op,fpout);
      switch (op) {
      case MOVE:
      case DRAW:
      case SIZE:
         for (n=0;n<2;n++) {
            token=strtok(NULLPOINTER," ,");
            i=atoi(token);
            if (screenprint) printf(" %i",i);
            else {
               fputc((unsigned char)(i/256),fpout);
               fputc((unsigned char)(i%256),fpout);
            }
         }
         break;
      case FILL:
      case FILLO:
         token=strtok(NULLPOINTER," ,");
         vertices=(unsigned char) atoi(token);
         if (screenprint) printf(" vertices: %i\n",vertices);
         else fputc(vertices-1,fpout);
         for (n=0;n<vertices;n++) {
            fgets(inputline,60,fpin);
            token=strtok(inputline,", ;");
            i=atoi(token);
            if (screenprint) printf(" %i",i);
            else {
               fputc((unsigned char)(i/256),fpout);
               fputc((unsigned char)(i%256),fpout);
            }
            token=strtok(NULLPOINTER," ,");
            i=atoi(token);
            if (screenprint) printf(" %i\n",i);
            else {
               fputc((unsigned char)(i/256),fpout);
               fputc((unsigned char)(i%256),fpout);
            }
         }
         break;
      case FONT:
         token=strtok(NULLPOINTER," ,");
         i=atoi(token);
         if (screenprint) printf(" %i",i);
         else fputc((unsigned char)(i),fpout);
         break;
      case TEXT:
         token=strtok(NULLPOINTER," ,");
         i=atoi(token);
         if (screenprint) printf(" %i ",i);
         direction=i/90;
         token=strtok(NULLPOINTER," ,");
		 for (n=PIC_KEYWORDS;n<NOOFKEYWORDS;n++) {
            if (strcmp(token,optable[n].keyword)==0) {
               position=optable[n].op;
               found=TRUE;
               break;
            }
         }
         if (found==FALSE) {
            printf(" onbekend keyword \"%s\" in regel %4ld\n",token,count);
            exit(-1);
         }
         if (screenprint) printf("%s",token);
         else fputc(direction*16+position,fpout);
         token=strtok(NULLPOINTER,"\"");
/*		 token=strtok(NULLPOINTER,"\";");*/
         if (screenprint) printf(" \"%s\" (%i)",token,strlen(token));
         else {
            fputs(token,fpout);
            fputc('\0',fpout);
         }
         break;
      }
   } while (op!=END);
close(fpin);
close(fpout);
gotoxy(1,17);
printf("Conversie klaar\nPIC.PIC kan getekend worden met:\n1) LOTUS PRINTGRAPH\n2) DRAWPIC PIC.PIC\n");
}

/********************** init display  *****************/



initdisplay()
{
union REGS reg;

reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int far *)( (reg.h.al == 7) ? 0XB0000000L : 0XB8000000L);
}


/***********************  write text  ****************/

swrite(int posx,int posy,char * tekst,int kleur)

{
   int far *nop= SCHERMPTR + posy*80 + posx;
   int x=0;

   while(tekst[x] != '\0')
        {
         *(nop+x) = tekst[x] | kleur;
         x++;
         }
}

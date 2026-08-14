/* pic2prn: disassembly lotus.pic file 031087 RCA 100290 EJN*/

#include "stdio.h"
#include "dos.h"

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
#define END 6
#define MAXLEN 255
#define TRUE 1
#define FALSE 0

char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char    outputfile[]="PIC.PRN";
char    print = FALSE;

int far *SCHERMPTR;
int *PTR  = (int *) 0XB800L ;

int	main(int argc, char **argv)
{
   int op,nop,pos,w,x,y;
   char nopp[80];
   float bars;
   long int count;
   unsigned char charx,chary,chop,font,c,n,direction,position,size,vertices;
   char text[MAXLEN];
   char posstring[20];
   double h_size,v_size;
   FILE *fpin;
   FILE *fpout;
   nop=argc;
   clrscr();
   initdisplay();

   swrite(20,2,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x3400);
   swrite(20,3,"³          PIC naar PRN vertaler          ³",0x3400);
   swrite(20,4,"ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ",0x3400);
   sprintf(nopp,"     Inputfile : %s    ------>                 ",argv[1]);
   nopp[45]=0;
   swrite(0,7,nopp,0x1600);
   sprintf(nopp,"Outputfile : %s                           ",outputfile);
   nopp[40]=0;
   swrite(40,7,nopp,0x1600);

   if ((fpin=fopen(argv[1],"rb")) == NULL) {
       printf("cannot open input file %s\n",argv[1]);
       exit(-1);
   }
   if ((fpout=fopen(outputfile,"wb")) == NULL) {
       printf("cannot open output file %s\n",outputfile);
       exit(-1);
   }



  swrite(30,10,"ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",0x1700);
  swrite(30,11,"³                     ³",0x1700);
  swrite(30,12,"ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ",0x1700);
  swrite(30,13,"0%        50%      100%",0x2700);
  strcpy(nopp,"                     ");

   bars=(float)( filelength(fileno(fpin)) ) / 21;

   nop=0;
   for (n=0;n<17;n++){
      c=getc(fpin);
      if (c!=header_vector[n]) {
         printf("%s is geen PIC-file\n",argv[1]);
         exit(-1);
      }
   }
   chop=getc(fpin);
   count=35;
   while ((chop>>4) != END && nop<20 ){
	   strnset(nopp,176,(int)(count/bars));
	   swrite(31,11,nopp,0x3400);
	  switch(chop){
          case MOVE:
              x=256*getc(fpin)+getc(fpin);
              y=256*getc(fpin)+getc(fpin);
              count+=4;
              if(print) printf("MOVE    %6d %6d;\n",x,y);
              w=fprintf(fpout,"MOVE    %6d %6d;\r\n",x,y);
              if (w<0) error(w,count);
              break;
          case DRAW:
              x=256*getc(fpin)+getc(fpin);
              y=256*getc(fpin)+getc(fpin);
              count+=4;
              if(print) printf("DRAW    %6d %6d;\n",x,y);
			  w=fprintf(fpout,"DRAW    %6d %6d;\r\n",x,y);
              if (w<0) error(w,count);
              break;
          case FILL:
              vertices=getc(fpin);
              for (c=1;c<vertices;c++) {
                  x=256*getc(fpin)+getc(fpin);
                  y=256*getc(fpin)+getc(fpin);
                  count+=4;
                  if(print) printf("FILL    %6d %6d;\n",x,y);
                  w=fprintf(fpout,"FILL    %6d %6d;\r\n",x,y);
                  if (w<0) error(w,count);
              }
              break;
          case FILLO:
              vertices=getc(fpin);
              for (c=1;c<vertices;c++) {
                  x=256*getc(fpin)+getc(fpin);
                  y=256*getc(fpin)+getc(fpin);
                  count+=4;
                  if(print) printf("FILLO   %6d %6d;\n",x,y);
                  w=fprintf(fpout,"FILLO   %6d %6d;\r\n",x,y);
                  if (w<0) error(w,count);
              }
              break;
          case COLOR0:
              if(print) printf("COLOR0;\n");
              w=fprintf(fpout,"COLOR0;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR1:
              if(print) printf("COLOR1;\n");
              w=fprintf(fpout,"COLOR1;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR2:
              if(print) printf("COLOR2;\n");
              w=fprintf(fpout,"COLOR2;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR3:
              if(print) printf("COLOR3;\n");
              w=fprintf(fpout,"COLOR3;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR4:
              if(print) printf("COLOR4;\n");
              w=fprintf(fpout,"COLOR4;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR5:
              if(print) printf("COLOR5;\n");
              w=fprintf(fpout,"COLOR5;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR6:
              if(print) printf("COLOR6;\n");
              w=fprintf(fpout,"COLOR6;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR7:
              if(print) printf("COLOR7;\n");
              w=fprintf(fpout,"COLOR7;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR8:
              if(print) printf("COLOR8;\n");
              w=fprintf(fpout,"COLOR8;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR9:
              if(print) printf("COLOR9;\n");
              w=fprintf(fpout,"COLOR9;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR10:
              if(print) printf("COLOR10;\n");
              w=fprintf(fpout,"COLOR10;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR11:
              if(print) printf("COLOR11;\n");
              w=fprintf(fpout,"COLOR11;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR12:
              if(print) printf("COLOR12;\n");
              w=fprintf(fpout,"COLOR12;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR13:
              if(print) printf("COLOR13;\n");
              w=fprintf(fpout,"COLOR13;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR14:
              if(print) printf("COLOR14;\n");
              w=fprintf(fpout,"COLOR14;\r\n");
              if (w<0) error(w,count);
              break;
          case COLOR15:
              if(print) printf("COLOR15;\n");
              w=fprintf(fpout,"COLOR15;\r\n");
              if (w<0) error(w,count);
              break;
          case TEXT:
			  pos=0;
              text[MAXLEN-1]='\0';
              c=getc(fpin);
              count++;
              direction=c>>4;
			  position=c-direction*16;
              do {
                 c=getc(fpin);
                 count++;
                 text[pos]=c;
                 pos++;
              }  while (c!=0 && pos<MAXLEN);
              switch (position) {
                 case 0:
                    strcpy(posstring,"CENTER");
                    break;
                 case 1:
                    strcpy(posstring,"CENTERLEFT");
                    break;
                 case 2:
                    strcpy(posstring,"CENTERTOP");
                    break;
                 case 3:
                    strcpy(posstring,"CENTERRIGHT");
                    break;
                 case 4:
                    strcpy(posstring,"CENTERBOTTOM");
                    break;
                 case 5:
                    strcpy(posstring,"TOPLEFT");
                    break;
                 case 6:
                    strcpy(posstring,"TOPRIGHT");
                     break;
                 case 7:
                    strcpy(posstring,"BOTTOMLEFT");
                    break;
                 case 8:
                    strcpy(posstring,"BOTTOMRIGHT");
                     break;
              }
              if(print) printf("TEXT %3i %s \"%s\";\n",90*direction,posstring,text);
              w=fprintf(fpout,"TEXT %3i %s \"%s\";\r\n",90*direction,posstring,text);
              if (w<0) error(w,count);
              break;
          case FONT:
              font=getc(fpin);
              count++;
              if(print) printf("FONT %2d;\n",font);
              w=fprintf(fpout,"FONT %2d;\r\n",font);
              if (w<0) error(w,count);
              break;
          case SIZE:
              x=256*getc(fpin)+getc(fpin);
              y=256*getc(fpin)+getc(fpin);
              count+=4;
              if(print) printf("SIZE    %6d %6d;\n",x,y);
              w=fprintf(fpout,"SIZE    %6d %6d;\r\n",x,y);
              if (w<0) error(w,count);
              break;
          default:
              printf("NOP     onbekende opcode %d\n",chop);
              nop++;
      }
      chop=getc(fpin);
      count++;
   }
   w=fprintf(fpout,"END;\r\n");
   if (w<0) error(w,count);
   close(fpin);
   close(fpout);
}

error(n,count)
int n;
{
   printf("write error %d at bytecount %ld\n",n,count);
   exit(-1);
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
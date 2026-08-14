/* translate lotus.pic file to kyocera F1000 laserprinter
         14/2/1988 EJN */

#include "stdio.h"
#include "string.h"
#include "PIC.H"

#define MAXLEN 80

extern int far *SCHERMPTR;
extern char *filedir(char *dir);
extern swrite(int ,int ,char * ,int);
extern char fileload[40];
extern float facx,facy;
extern int show;
extern int size;

FILE *fileout;
FILE *fpin;

char    header_vector[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};
char    outputfile[]="KYO.PRN";

char *font      = "FONT \0";        /* Instructies naar kyocera */
char *tekst     = "TEXT \0";
char *end       = "RES; EXIT;\0";
char *start     = "!R! UNIT D;\0";
char *move      = "MAP \0";
char *draw      = "DAP \0";
char *fill	= "FILL \0";
char *fillo	= "FILL \0";
char *sise	= "cmnt SIZE \0";

static char color[15][15] ={ "cmnt LINE 1; ",
			     "cmnt LINE 2; ",
			     "cmnt LINE 3; ",
			     "cmnt LINE 4; ",
 			     "cmnt LINE 5; ",
	  		     "cmnt LINE 6; ",
			     "cmnt LINE 7; ",
			     "cmnt LINE 8; ",
			     "cmnt LINE 9; ",
			     "cmnt LINE 10; ",
			     "cmnt LINE 11; ",
			     "cmnt LINE 12; ",
			     "cmnt LINE 13; ",
			     "cmnt LINE 14; ",
			     "cmnt LINE 15; " };
int x,y;

/**************** TRANSLATE PIC FILE *********************/

translate_picfile()
{

   int w,op,pos,nop,sizex,sizey,dotszin;
   long int count,counter;
   unsigned char chop,c,n,direction,position,vertices;
   char text[MAXLEN];
   char posstring[80];
   unsigned char eddie[8];   /*dummy*/

   int fouten = 0;
   sizex=sizey=30;       /* size character 30 dpi */
   if ((fpin=fopen(fileload,"rb")) == NULL)
       {
       printf("cannot open input file %s\n",fileload);
       exit(-1);
       }

   if ((fileout=fopen(outputfile,"wb")) == NULL)
       {
       printf("cannot open output file %s\n",outputfile);
       exit(-1);
       }

   for (n=0;n<17;n++)
      {
      c=getc(fpin);
      if (c!=header_vector[n])
         {
         printf("%s is geen PIC-file\n");
         exit(-1);
         }
        }

     count=1;
     if (show) printf("%s \n",start);
     w=fprintf(fileout,"%s \r\n",start);
     if (w<0) error(w,count);
        swrite(10,10,"Bytes translated:\0",0x1300);
   chop=getc(fpin);
   counter=count=18;
   while ((chop>>4) != END && fouten <20){
      if(counter++ > 20)
	{
	ltoa(count,eddie,10);
        swrite(29,10,eddie,0x1300);
	counter-=20;
	}
      switch(chop){

          case MOVE:
	      getxy();
              count+=4;
              if (show) printf("%s %d, %d;\n",move,x,y);
              w=fprintf(fileout,"%s %d, %d;\r\n",move,x,y);
              if (w<0) error(w,count);
              break;

          case DRAW:
	      getxy();
              count+=4;
              if (show) printf("%s %d, %d;\n",draw,x,y);
              w=fprintf(fileout,"%s %d, %d;\r\n",draw,x,y);
              if (w<0) error(w,count);
              break;

          case FILL:
              vertices=getc(fpin);
              for (c=1;c<vertices;c++) {
		  getxy();
                  count+=4;
                  if (show) printf("%s %d, %d;\n",fill,x,y);
                  w=fprintf(fileout,"%s %d, %d;\r\n",fill,x,y);
                  if (w<0) error(w,count);
              }
              break;
          case FILLO:
              vertices=getc(fpin);
              for (c=1;c<vertices;c++) {
		  getxy();
                  count+=4;
                  if (show) printf("%s %d, %d;\n",fillo,x,y);
                  w=fprintf(fileout,"%s %d, %d;\r\n",fillo,x,y);
                  if (w<0) error(w,count);
              }
              break;
          case COLOR0:
          case COLOR1:
          case COLOR2:
          case COLOR3:
          case COLOR4:
          case COLOR5:
          case COLOR6:
          case COLOR7:
          case COLOR8:
          case COLOR9:
          case COLOR10:
          case COLOR11:
          case COLOR12:
          case COLOR13:
          case COLOR14:
          case COLOR15:
              if (show) printf("%s \n",color[chop-176]);
              w=fprintf(fileout,"%s \r\n",color[chop-176]);
              if (w<0) error(w,count);

              break;

          case TEXT:
              pos=0;
              text[MAXLEN-1]='\0';
              c=getc(fpin);
              count++;
              direction=c>>4;
              position=c%16;
              do {
                 c=getc(fpin);
                 count++;
                 text[pos]=c;
                 pos++;
                 }
              while (c!=0 && pos<MAXLEN);
              dotszin=pos*sizex;
    if(direction==1 || direction ==3)
        {

      switch (position) {
                 case 0:
                    strcpy(posstring,"CENTER");
                    y+=dotszin/2;
		    x-=sizey;
	            break;
                 case 1:
                    strcpy(posstring,"CENTERLEFT");
		    y+=dotszin;
		    x-=sizey*2;
                    break;
                 case 2:
                    strcpy(posstring,"CENTERTOP");
                    y+=dotszin/2;
		    x-=sizey*2;
                    break;
                 case 3:
                    strcpy(posstring,"CENTERRIGHT");
                    y+=dotszin;
		    x-=sizey;
                    break;
                 case 4:
                    strcpy(posstring,"CENTERBOTTOM");
                    y+=dotszin/2;
                    x-=sizey;
                    break;
                 case 5:
                    strcpy(posstring,"TOPLEFT");
		    x-=sizey*2;
                    y+=dotszin;
                    break;
                 case 6:
                    strcpy(posstring,"TOPRIGHT");
                    y+=dotszin;
		    x-=sizey*2;
                     break;
                 case 7:
                    strcpy(posstring,"BOTTOMLEFT");
                    y+=dotszin;
                    break;
                 case 8:
                    strcpy(posstring,"BOTTOMRIGHT");
                    y+=dotszin;
                     break;
              }
       nop=x;
       x=y;
       y=nop;

           }
       else
       {
      switch (position) {
                 case 0:
                    strcpy(posstring,"CENTER");
                    x-=dotszin/2;
		    y+=sizey/2;
	            break;
                 case 1:
                    strcpy(posstring,"CENTERLEFT");
		    y+=sizey/2;
		
                    break;
                 case 2:
                    strcpy(posstring,"CENTERTOP");
                    x-=dotszin/2;
		    y+=sizey;
                    break;
                 case 3:
                    strcpy(posstring,"CENTERRIGHT");
                    x-=dotszin;
		    y+=sizey/2;
                    break;
                 case 4:
                    strcpy(posstring,"CENTERBOTTOM");
                    x-=dotszin/2;
                    break;
                 case 5:
                    strcpy(posstring,"TOPLEFT");
		    y+=sizey;
                    break;
                 case 6:
                    strcpy(posstring,"TOPRIGHT");
                    x-=dotszin;
		    y+=sizey;
                     break;
                 case 7:
                    strcpy(posstring,"BOTTOMLEFT");
                    break;
                 case 8:
                    strcpy(posstring,"BOTTOMRIGHT");
                    x-=dotszin;
                     break;
                  }
           }

       if (show) printf("%s %s ;\n","cmnt", posstring);
       w=fprintf(fileout,"%s %s;\r\n","cmnt",posstring);

    if(direction==1 || direction ==3)
        {
/*       nop=x;
       x=y;
       y=nop;
  */
       if (show) printf("%s %d;\n",font,17);
       w=fprintf(fileout,"%s %d;\r\n",font,17);
       if (show) printf("%s %d, %d;\n",move,LX-x,y);
       w=fprintf(fileout,"%s %d, %d;\r\n",move,LX-x,y);
        }
        else
          {
          if (show) printf("%s %d, %d;\n",move,x,y);
          w=fprintf(fileout,"%s %d, %d;\r\n",move,x,y);
          }

    if (w<0) error(w,count);

    if (show) printf("%s '%s';\n",tekst,text);
       w=fprintf(fileout,"%s '%s';\r\n",tekst,text);
    if (w<0) error(w,count);

    if(direction==1 || direction ==3)   /* zet font weer recht */
       {
       if (show) printf("%s %d;\n",font,1);
       w=fprintf(fileout,"%s %d;\r\n",font,1);
       if (w<0) error(w,count);
         }

       break;

          case FONT:
              getc(fpin);
              count++;
              if (show) printf("%s %d\n",font,1);
              w=fprintf(fileout,"%s %d;\r\n",font,1);
              if (w<0) error(w,count);
              break;

          case SIZE:
              x=256*getc(fpin)+getc(fpin);
              y=256*getc(fpin)+getc(fpin);
              x=(int)( (float) x / facx);
              y=(int)( (float) y / facy);
              count+=4;
 	      sizex=30;
 	      sizey=30;
              if (show) printf("%s %d, %d ;\n",sise,x,y);
              w=fprintf(fileout,"%s %d, %d;\r\n",sise,x,y);
              if (w<0) error(w,count);
              break;

          default:
              printf("NOP     onbekende opcode %d\n",chop);
              fouten++;
      }
      chop=getc(fpin);
      count++;
   }

   printf("%s\nLengte PIC file %5d bytes\n",end,count);
   w=fprintf(fileout,"%s\r\n",end);
   if (w<0) error(w,count);

   if(fclose(fileout)==EOF)
       {
	 printf("\n\n    WRITE ERROR <---> DISK FULL ?????\n\n");
         printf("Press a key\n\n");
	 getch();
       }
   fclose(fpin);
   printf("\nResultaat in %s\n",outputfile);
}


error(n,count)
int n;
{
   printf("write error %d at bytecount %ld\n\n\nDisk full???\n\n",n,count);
   printf("Press a key\n");
   getch();
   exit(-1);
}


getxy()

{
   x=256*getc(fpin)+getc(fpin);
   y=256*getc(fpin)+getc(fpin);
   x=200+(int)( (float) x / facx);
   y=2000-(int)( (float) y / facy);
}
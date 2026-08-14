// pic2prn2pic: disassembly lotus.pic file 031087 RCA 100290 EJN 180994 EJN

#include "picdump.h"
#define MAXLEN 1024


int	pic2prn(HWND hwnd,char *inputfile,char *outputfile);
int	prn2pic(HWND hwnd,char *inputfile, char* outputfile);
int   error(HWND hwnd,int n,long count);

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³          PIC naar PRN vertaler          ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
int	pic2prn(HWND hwnd,char *inputfile, char* outputfile)
{
	int 		nop,pos,w,x,y,x1,y1;
	long int count;
	unsigned char chop,font,n,c,direction,position,vertices;
	char 		text[MAXLEN];
	char 		posstring[20];
	FILE 		*fpin;
	FILE 		*fpout;
	if ((fpin=fopen(inputfile,"rb")) == NULL) {
		 sprintf(text, "cannot open input file %s\n", inputfile);
		 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
		 return(0);
	}
	if ((fpout=fopen(outputfile,"wb")) == NULL) {
		 sprintf(text, "cannot open output file %s\n", outputfile);
		 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
		 return(0);
	}
	nop=0;
	for (n=0;n<17;n++){
		c=getc(fpin);
		if (c!=header_vector[n]) {
		 sprintf(text, "%s is geen PIC-file\n", inputfile);
		 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
		 return(0);
		}
	}
	chop=getc(fpin);
	count=35;
	while ((chop>>4) != _END && nop<20 ){
	  switch(chop){
			 case MOVE:
				  x=256*getc(fpin)+getc(fpin);
				  y=256*getc(fpin)+getc(fpin);
				  count+=4;
				  w=fprintf(fpout,"MOVE    %6d %6d;\r\n",x,y);
				  if (w<0) error(hwnd,w,count);
				  break;
			 case DRAW:
				  x=256*getc(fpin)+getc(fpin);
				  y=256*getc(fpin)+getc(fpin);
				  count+=4;
				  w=fprintf(fpout,"DRAW    %6d %6d;\r\n",x,y);
				  if (w<0) error(hwnd,w,count);
				  break;
			 case FILL:
				  vertices=getc(fpin);
				  w=fprintf(fpout,"FILL %3d",vertices);
				  if (w<0) error(hwnd,w,count);
				  for (n=0;n<=vertices;n++)
					{
					 x=256*getc(fpin)+getc(fpin);
					 y=256*getc(fpin)+getc(fpin);
					 count+=4;
					 w=fprintf(fpout," %4d %4d",x,y);
					 if (w<0) error(hwnd,w,count);
					}
				  w=fprintf(fpout,";\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case FILLO:
				  vertices=getc(fpin);
				  w=fprintf(fpout,"FILLO %3d",vertices);
				  if (w<0) error(hwnd,w,count);
				  for (n=0;n<=vertices;n++)
					 {
						x=256*getc(fpin)+getc(fpin);
						y=256*getc(fpin)+getc(fpin);
						count+=4;
						w=fprintf(fpout," %4d %4d",x,y);
						if (w<0) error(hwnd,w,count);
					 }
				  w=fprintf(fpout,";\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR0:
				  w=fprintf(fpout,"COLOR0;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR1:
				  w=fprintf(fpout,"COLOR1;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR2:
				  w=fprintf(fpout,"COLOR2;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR3:
				  w=fprintf(fpout,"COLOR3;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR4:
				  w=fprintf(fpout,"COLOR4;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR5:
					w=fprintf(fpout,"COLOR5;\r\n");
              if (w<0) error(hwnd,w,count);
              break;
          case COLOR6:
				  w=fprintf(fpout,"COLOR6;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR7:
				  w=fprintf(fpout,"COLOR7;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR8:
				  w=fprintf(fpout,"COLOR8;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR9:
				  w=fprintf(fpout,"COLOR9;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR10:
				  w=fprintf(fpout,"COLOR10;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR11:
				  w=fprintf(fpout,"COLOR11;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR12:
				  w=fprintf(fpout,"COLOR12;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR13:
				  w=fprintf(fpout,"COLOR13;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR14:
				  w=fprintf(fpout,"COLOR14;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR15:
				  w=fprintf(fpout,"COLOR15;\r\n");
				  if (w<0) error(hwnd,w,count);
				  break;
			 case COLOR16:							// voor RGB kleuren
				  x=256*getc(fpin)+getc(fpin);
				  y=256*getc(fpin)+getc(fpin);
				  x1=256*getc(fpin)+getc(fpin);
				  y1=256*getc(fpin)+getc(fpin);
				  count+=8;
				  w=fprintf(fpout,"COLOR16     %6d %6d %6d %6d;\r\n",x,y,x1,y1);
				  if (w<0) error(hwnd,w,count);
				  break;
			 case _TEXT:
				  pos=0;
				  memset(text,0,MAXLEN-1);
//				  text[MAXLEN-1]='\0';
				  c=getc(fpin);
				  count++;
				  direction=c>>4;
				  position=c-direction*16;
				  text[pos]=' ';
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
				  w=fprintf(fpout,"TEXT %3i %s \"%s\";\r\n",90*direction,posstring,text);
				  if (w<0) error(hwnd,w,count);
				  break;
			 case _FONT:
				  font=getc(fpin);
				  count++;
				  w=fprintf(fpout,"FONT %2d;\r\n",font);
				  if (w<0) error(hwnd,w,count);
				  break;
			 case _SIZE:
				  x=256*getc(fpin)+getc(fpin);
				  y=256*getc(fpin)+getc(fpin);
				  count+=4;
				  w=fprintf(fpout,"SIZE %4d %4d;\r\n",x,y);
				  if (w<0) error(hwnd,w,count);
				  break;
			 default:
				  nop++;
		}
		chop=getc(fpin);
		count++;
	}
	w=fprintf(fpout,"END;\r\n");
	if (w<0) error(hwnd,w,count);
	fclose(fpin);
	fclose(fpout);
return(1);
}

int error(HWND hwnd,int n,long count)
{
 char text[MAXLEN];
 sprintf(text, "write error %d at bytecount %ld\n",n,count);
 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
 return(0);
}

// assembly lotus.pic file RCA 021087 EJN 100290
#define NULLPOINTER (char *) 0
#define PIC_KEYWORDS 25
#define POSITION_KEYWORDS 9
#define NOOFKEYWORDS PIC_KEYWORDS + POSITION_KEYWORDS

struct {
	char keyword[13];
	unsigned char op;
} optable[NOOFKEYWORDS]= {
	{"COLOR0",COLOR0},
	{"COLOR1",COLOR1},
	{"COLOR10",COLOR10},
	{"COLOR11",COLOR11},
	{"COLOR12",COLOR12},
	{"COLOR13",COLOR13},
	{"COLOR14",COLOR14},
	{"COLOR15",COLOR15},
	{"COLOR16",COLOR16},
	{"COLOR2",COLOR2},
	{"COLOR3",COLOR3},
	{"COLOR4",COLOR4},
	{"COLOR5",COLOR5},
	{"COLOR6",COLOR6},
	{"COLOR7",COLOR7},
	{"COLOR8",COLOR8},
	{"COLOR9",COLOR9},
	{"DRAW",DRAW},
	{"END",_END},
	{"FILL",FILL},
	{"FILLO",FILLO},
	{"FONT",_FONT},
	{"MOVE",MOVE},
	{"SIZE",_SIZE},
	{"TEXT",_TEXT},
	{"CENTER",CENTER},
	{"CENTERLEFT",CENTERLEFT},
	{"CENTERTOP",CENTERTOP},
	{"CENTERRIGHT",CENTERRIGHT},
	{"CENTERBOTTOM",CENTERBOTTOM},
	{"TOPLEFT",TOPLEFT},
	{"TOPRIGHT",TOPRIGHT},
	{"BOTTOMLEFT",BOTTOMLEFT},
	{"BOTTOMRIGHT",BOTTOMRIGHT}
};

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//³          PRN naar PIC vertaler          ³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

int	prn2pic(HWND hwnd,char *inputfile, char* outputfile)
{
	int op,i,found;
	long int count;
	unsigned char c,n,direction,position,vertices;
	char text[MAXLEN], inputline[1024];
	char *token;
	FILE *fpin, *fpout;
	if ((fpin=fopen(inputfile,"rb")) == NULL) {
		 sprintf(text, "cannot open input file %s\n", inputfile);
		 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
		 return(0);
	}
	if ((fpout=fopen(outputfile,"wb")) == NULL) {
		 sprintf(text, "cannot open output file %s\n", outputfile);
		 MessageBox(hwnd, text, "Error", MB_ICONSTOP);
		 return(0);
	}


	for (n=0;n<17;n++){
		c=header_vector[n];
		putc(c,fpout);
	}
	count=30;
	do {

	  fgets(inputline,MAXLEN,fpin);
	  count+=strlen(inputline);
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
			sprintf(text, "onbekend keyword \"%s\" in regel %4ld\n",token,count);
			MessageBox(hwnd, text, "Error", MB_ICONSTOP);
			return(0);
		}
		fputc(op,fpout);
		switch (op) {
		case MOVE:
		case DRAW:
		case _SIZE:
			for (n=0;n<2;n++) {
				token=strtok(NULLPOINTER," ,");
				i=atoi(token);
				fputc((unsigned char)(i/256),fpout);
				fputc((unsigned char)(i%256),fpout);
			}
			break;
		case COLOR16:  		// Voor RGB kleuren
			for (n=0;n<4;n++) {
				token=strtok(NULLPOINTER," ,");
				i=atoi(token);
				fputc((unsigned char)(i/256),fpout);
				fputc((unsigned char)(i%256),fpout);
			}
			break;
		case FILL:
		case FILLO:
			token=strtok(NULLPOINTER," ,;");
			vertices=(unsigned char) atoi(token);
			fputc(vertices,fpout);
			for (n=0;n<=vertices;n++) {
				token=strtok(NULLPOINTER," ,");
				i=atoi(token);
				fputc((unsigned char)(i/256),fpout);
				fputc((unsigned char)(i%256),fpout);
				token=strtok(NULLPOINTER," ,");
				i=atoi(token);
				fputc((unsigned char)(i/256),fpout);
				fputc((unsigned char)(i%256),fpout);
			}
			break;
		case _FONT:
			token=strtok(NULLPOINTER," ,");
			i=atoi(token);
			fputc((unsigned char)(i),fpout);
			break;
		case _TEXT:
			token=strtok(NULLPOINTER," ,");
			i=atoi(token);
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
			sprintf(text, "onbekend keyword \"%s\" in regel %4ld\n",token,count);
			MessageBox(hwnd, text, "Error", MB_ICONSTOP);
			return(0);
			}
			fputc(direction*16+position,fpout);
			token=strtok(NULLPOINTER,"\"");
			if(token[0]!=';')	fputs(token,fpout);
			fputc('\0',fpout);
			break;
		}
	} while (op!=_END);
fputc(_END,fpout);
fclose(fpin);
fclose(fpout);
sprintf(text, "Conversie klaar\n gesaved als %s file\n",outputfile);
MessageBox(hwnd, text, "Translation", MB_ICONSTOP);
return(1);
}



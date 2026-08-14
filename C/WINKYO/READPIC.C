// -------------------- readpic.c ---------------------------------
// draw en dump lotus.pic file EJN
//windows version 110894
// 160789 positionering aangepast
// 210789 Hercules/CGA screendump toegevoegd
// 210789 Grootte size verbeterd
// 150290 EGA/VGA aangepast
// 170793 Test einde file toegevoegd & onbekende opcodemelding verwijderd
// 120894 Windows versie

#include "picdump.h"

#define M_convert2x (int) (H_OFFSET+ (float)x / HSF)
#define M_convert2y (int) (float)MaxY-(V_OFFSET+(float)y/VSF)
#define M_getxeny   	x=256*getc(fp)+getc(fp);\
							y=256*getc(fp)+getc(fp);\
							count+=4;

POINT drawpos[1000];
int	drawtel;

float 		xnop,ynop;
char  		header_vector_pic[] = {1,0,0,0,1,0,8,0,68,0,0,0,0,12,127,9,6};

void Interpret(HDC hdc)
{
 int n;
 int maxx=0,maxy=0;
 int minx=32000,miny=32000;

 if(drawtel>10 &&
	 drawpos[0].x == drawpos[drawtel-1].x &&
	 drawpos[0].y == drawpos[drawtel-1].y )
	{
	 for(n=0;n<drawtel;n++)
		{
		 if(minx>drawpos[n].x)  minx=drawpos[n].x;
		 if(miny>drawpos[n].y)  miny=drawpos[n].y;
		 if(maxx<drawpos[n].x)  maxx=drawpos[n].x;
		 if(maxy<drawpos[n].y)  maxy=drawpos[n].y;
		}
	 DrawEllipse(hdc,(double)minx,(double)maxy,(double)maxx,(double)miny);
	 }
 else  for(n=0;n<drawtel;n++)	 Draw(hdc,drawpos[n].x,drawpos[n].y);
memset(&drawpos,0,sizeof(drawpos));
drawtel=0;

}

int ReadPic(HDC hdc,char* filenaam)
{
	unsigned char 	chop,c,n,vertices;
	int 		pos,nop,count,font=1;
	int 		x,y,x1,y1,direction,position;
	int 		linethickness;
	int 		sizexx=80;//,sizeyy=80;   // uitgangsgroote letters
	int 		sizex=80 ,sizey=80;    // SIZE PIC FILE LETTERS VOOR INIT
	float 	lettergrootte;
	char 		text[80];
	int 		fill=FALSE;
	FILE 		*fp;

	memset(&drawpos,0,sizeof(drawpos));
	drawtel=0;
	lettergrootte=1.0;
	if ((fp=fopen(filenaam,"rb")) == NULL)
	 {
 //	 argv[1]=filedir("*.PIC");
 //	 fp=fopen(argv[1],"rb");
		 return(FALSE);
	 }

	nop=0;
	linethickness=LastLTButton-200; // define picdump.h + 200
	Piccolor(hdc,0,linethickness);
	for (n=0;n<17;n++){
		c=getc(fp);
		if (c!=header_vector_pic[n]) {//	 printf("%s is geen PIC-file",argv[1]);
				return(FALSE);		}
	}
	chop=getc(fp);
	count=17;
	while ((chop>>4) != _END && nop<20 ){
	switch(chop){
	  case MOVE:
//			if(drawpos){  Interpret(hdc);}
			M_getxeny
			if(font>10) break;
			Move(hdc,x,y);
		break;
	  case DRAW:
			M_getxeny
			if(font>10) break;
//			drawpos[drawtel].x=x;
//			drawpos[drawtel++].y=y;
//			if(drawtel==1000) Interpret(hdc);
			Draw(hdc,x,y);
			break;

	  case FILL:
			fill=TRUE;
	  case FILLO:
			vertices=getc(fp);
			for (c=0;c<=vertices;c++)
			  {
				 M_getxeny
				 drawpos[c].x=x;
				 drawpos[c].y=y;
			  }
			if(font<11) 	DrawPolygon(hdc,drawpos,vertices+1,fill);
			fill=FALSE;
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
			Piccolor(hdc,chop-COLOR0,linethickness); break;
	  case COLOR16:     // Get RGB colors
			  M_getxeny
			  x1=x;y1=y;
			  M_getxeny
			  PicRGBcolor(hdc,x1,y1,x,y);		//R,G,B,linethickness
			  break;
	  case _TEXT:
			if(font>10) break;
			pos=0;
			text[80-1]='\0';
			c=getc(fp);
			count++;
			direction=c>>4;
			position=c%16;
			do {
				c=getc(fp);
				count++;
				text[pos]=c;
				pos++;
				}  while (c!=0 && pos<80);
			pos--;

		 switch (direction) {
		  case 0:
		  case 2:	direction=0; 	break;
		  case 1:
		  case 3:   direction=90;	break;
			}

		switch (position) {
		 case 0:	/* CENTER */ 		SetTextAlign(hdc,TA_CENTER|TA_BASELINE);		break;
		 case 1:	/* CENTERLEFT */  SetTextAlign(hdc,TA_LEFT  |TA_BASELINE);		break;
		 case 2:	/* CENTERTOP */   SetTextAlign(hdc,TA_CENTER|TA_TOP);				break;
		 case 3:	/* CENTERRIGHT */ SetTextAlign(hdc,TA_RIGHT|TA_BASELINE);      break;
		 case 4:	/* CENTERBOTTOM */SetTextAlign(hdc,TA_CENTER|TA_BASELINE);		break;
		 case 5:	/* TOPLEFT */		SetTextAlign(hdc,TA_LEFT  |TA_TOP);				break;
		 case 6:	/* TOPRIGHT */		SetTextAlign(hdc,TA_RIGHT |TA_TOP);				break;
		 case 7:	/* BOTTOMLEFT */	SetTextAlign(hdc,TA_LEFT  |TA_BASELINE);		break;
		 case 8:	/* BOTTOMRIGHT */	SetTextAlign(hdc,TA_RIGHT |TA_BASELINE);
				}
			Pictext(hdc,direction,position,text,lettergrootte);
			break;
	  case _FONT:
			font=getc(fp);
			count++;
			if(font>10 && font <19)
			 {
			  getc(fp);
			  M_getxeny
			  x1=x;y1=y;
			  getc(fp);
			  M_getxeny
			  count+=2;
			  if(font==11) DrawEllipse(hdc,x1-x,y1+y,x1+x,y1-y);
			  if(font==12) DrawFillEllipse(hdc,x1-x,y1+y,x1+x,y1-y);
/*			  if(font==13 || font==14)
					{
					drawpos[0].x=x1+x;
					drawpos[0].y=y1+y;
					drawpos[1].x=x1+x;
					drawpos[1].y=y1-y;
					drawpos[2].x=x1-x;
					drawpos[2].y=y1-y;
					drawpos[3].x=x1-x;
					drawpos[3].y=y1+y;
					if(font==13) fill=TRUE; else fill=FALSE;
					DrawPolygon(hdc,drawpos,4,fill);
					}
*/
				if(font>12) font=10;
				Move(hdc,x1,y1);

			 }
			break;
	  case _SIZE:
			sizex=( 256 * getc(fp)+getc(fp)) ;
			sizey=( 256 * getc(fp)+getc(fp)) ;
			lettergrootte=(float)sizex/(float)sizexx;
			if(lettergrootte<=0) lettergrootte=1;
			count+=4;
			break;
	  default:
//	      printf("onbekende opcode %d\n",chop);
//              closegraph();	/* Return the system to text mode
//              exit(-1);
			break;
		}
	if (feof(fp)) {      //test einde file
	fclose(fp);
	break;
	  }
	chop=getc(fp);
	count++;
	}
//if(drawpos) Interpret(hdc);

fclose(fp);
sizey=sizey;
return(TRUE);
}



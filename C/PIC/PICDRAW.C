
/*************** openen en verwerken PIC file ***************/

picopen()
{
int n;
unsigned char c;


   if ((fpout=fopen(outfile,"wb")) == NULL)
       {
       printf("cannot open output-file %s\n",outfile);
       exit(-1);
       }

   for (n=0;n<17;n++)
      {
      c=header_vector[n];
      fputc(c,fpout);
      }
   picsize(80,80);
   picfont(1);
   piccolor(0);
}

picmove(x,y)
 int x,y;
 {
   fputc(MOVE,fpout);
   picxy(x,y);
 }

 picfont(font)
 char font;
  {
   fputc(FONT,fpout);
   fputc(font,fpout);
  }

piccolor(color)
 int color;
 {
   if (color==0) fputc(COLOR0,fpout);
   if (color==1) fputc(COLOR1,fpout);
   if (color==2) fputc(COLOR2,fpout);
   if (color==3) fputc(COLOR3,fpout);
 }

picsize(x,y)
 int x,y;
 {
   fputc(SIZE,fpout);
   picxy(x,y);
 }

  picdraw(x,y)
int x,y;
{
   fputc(DRAW,fpout);
   picxy(x,y);
}

pictext(direction,position,s)
 int direction,position;
 char *s;
 {
   fputc(TEXT,fpout);
   fputc((unsigned char)direction*16+position,fpout);
   fputs(s,fpout);
   fputc('\0',fpout);
 }

picend()
{
   fputc(96,fpout);    /*END*/
   fclose(fpout);
}

picxy(x,y)
 int x,y;
 {
   fputc(x/256,fpout);
   fputc(x%256,fpout);
   fputc(y/256,fpout);
   fputc(y%256,fpout);
 }

picbox(ox,oy,lx,ly)
 int ox,oy,lx,ly;
 {
   picmove(ox   ,oy   );
   picdraw(ox   ,oy+ly);
   picdraw(ox+lx,oy+ly);
   picdraw(ox+lx,oy   );
   picdraw(ox   ,oy   );
 }


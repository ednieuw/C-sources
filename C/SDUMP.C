/*   screendump 200789  Ed Nieuwenhuys  */


#include <stdio.h>
#include "conio.h"
#include <dos.h>
#include "bios.h"
#include "io.h"
#include "fcntl.h"
#include "\sys\stat.h"

unsigned char far *video;
unsigned char far *vid;
int *SCHERMPTR  = (int *) 0XB800L ;

int printer;
int MaxX, MaxY = 0;

main()
{
register int colomn;
register unsigned int pixy , regel , nop;
unsigned char rowsum[720];
unsigned char str1[] = {27,64,27,65,8};      /* reset /linespacing 8/72 inch */
unsigned char str2[] = {27,76,128,2};                     /* ESC K, 640 char */
unsigned char str3[] = {13,10};
unsigned char str4[] = {13,10,27,65,12};             /* linespacing 1/6 inch */
unsigned char str5[] = {32,32,32,32,32,32,32,32,32,32,32,32};
nop = 0;

initdisplay();
if (MaxX==720) str2[2]+=80;    				/*ESC K,720 char */


prnonoff();
printer = open("PRN" , O_WRONLY | O_BINARY);
write(printer,str1,5);

 for ( regel = 0 ; regel< (MaxY/8) ; regel++)
 {

     for ( colomn = 0 ; colomn < MaxX ; colomn++)
     {
      rowsum[colomn] = 0 ;                                /* zet byte op nul */

      for ( pixy = 0 ; pixy < 8 ; pixy++ )
         {
          if (scr_rdot( colomn ,nop + pixy ))               /* if screen dot */
             rowsum[colomn] += (128 >> pixy );
         }                                                /* einde loop byte*/

      }    					    /* einde loop kolom */

   write(printer,str5,12);				/* print 12 spaces*/
   write(printer,str2,4);			   /* ESC K,mod256,div256 */
   write(printer,rowsum,MaxX) ;
   write(printer,str3,2);              /* print CR/LF */
    nop += 8;
    }                                                         /* einde regel */

 write(printer,str4,5);                             /* linespacing 1/6 inch */
 close(printer);
 }

int scr_rdot(int x, int y)
{
  if (MaxX==640)
  {
    video= MK_FP (SCHERMPTR, 0x2000 * (y&1) + 80 * (y>>1) + (x>>3) );
    return((*video&0x80>>(x&7)) !=0);
  }
  else
  {
	vid= MK_FP (0x2000 * (y&3) , 90 * (y>>2) + (x>>3) );
	video= MK_FP (SCHERMPTR, vid);
    return((*video&0x80>>(x&7)) !=0);
  }
}



initdisplay()
{
union REGS reg;
reg.h.ah=15;
int86(0X10,&reg,&reg);
SCHERMPTR=(int *)( (reg.h.al != 7) ? 0XB800L : 0XB000L);
if(reg.h.al!=7) { MaxX=640; MaxY=200;}
  else {MaxX=720; MaxY=348;}
}


prnonoff()
{
 if(biosprint(2,0,0) !=0x90)
 {
  sound(2200);
  delay(13);
  nosound();
  sound(200);
  delay(300);
  nosound();
  exit(-1);
 }
/* Verlaat het programma als de printer uitstaat*/
 }



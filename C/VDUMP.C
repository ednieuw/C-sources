/*   screendump 09 aug 1990  Ed Nieuwenhuys  */
/* Maakt kleine screendump van CGA, EGA, VGA en Hercules schermen. */

#include "stdio.h"
#include "dos.h"
#include "bios.h"
#include "io.h"
#include "fcntl.h"
#include "\sys\stat.h"
#include "conio.h"

extern void getpix(void);                             /* asm in vega.obj*/
void initdisplay(void);                               /* test mode en init */
void prnonoff();                                      /* test printer */

unsigned char far *video;
unsigned char far *vid;
int *SCHERMPTR  = (int *) 0XB800L ;

int printer;
int MaxX, MaxY = 0;
int page;
int mode;
/***************************** MAIN **************************************/
main()
{
register int colomn,deler,compensatie,a,b;
register unsigned int pixy , regel , nop;
unsigned char rowsum[1025];
unsigned char str1[] = {27,64,27,65,8};              /* linespacing 8/72 inch */
unsigned char str2[] = {27,76,128,2};                /* ESC K, 640 char */
unsigned char str3[] = {13,10};                      /* CR/LF */
unsigned char str4[] = {13,10,27,65,12};             /* linespacing 1/6 inch */
unsigned char str5[] = {32,32,32,32,32,32,32,32,32,32,32,32};

initdisplay();
if (MaxX==720) str2[2]=208;                          /*ESC K,720 char */
if (MaxX==320) {str2[2]=64; str2[3]=1;}              /* ESC K,320 char */

prnonoff();
printer = open("PRN" , O_WRONLY | O_BINARY);
write(printer,str1,5);
nop = 0;

if(MaxY<350) deler=8; else deler=16;                  /* als >350 regels */
if(MaxY<350) compensatie=1; else compensatie=2;       /* compress hoogte */
 {
 for ( regel = 0 ; regel< (MaxY/deler) ; regel++)
  {
     for ( colomn = 0 ; colomn < MaxX ; colomn++)
     {
      rowsum[colomn] = 0 ;                            /* zet byte op nul */
      for ( pixy = 0 ; pixy < deler ; pixy++ )
         {
          b = (pixy/2)*2;                             /* pixy even? */
          if(b==pixy) a=0;                            /* voeg 2 pix samen */
          if (scr_rdot( colomn ,nop + pixy ))         /* if screen dot */
           {
            if(deler==16)
            {
             if(b == pixy)
                {
                rowsum[colomn] += (128 >> (pixy/compensatie));
                a=1;
                }
            else if(!a)    rowsum[colomn] += (128 >> (pixy/compensatie));
             }
           else    rowsum[colomn] += (128 >> pixy);
           }
         }                                             /* einde loop byte*/
      }                                                /* einde loop kolom */
    write(printer,str5,5);                             /* print 5 spaces */
    write(printer,str2,4);                             /* ESC K,mod256,div256 */
    write(printer,rowsum,MaxX) ;                       /* print bitimage */
    write(printer,str3,2);                             /* print CR/LF */
    nop += deler;
    }                                                  /* einde regel */
   }
 write(printer,str4,5);                                /* linespacing 1/6 inch */
 close(printer);
 }

/**************************** SCR_RDOT *************************************/

int scr_rdot(int x, int y)
{
union REGS reg;
  if (MaxX==720)
  {
    vid= MK_FP (0x2000 * (y&3) , 90 * (y>>2) + (x>>3) );
    video= MK_FP (SCHERMPTR, vid);
    return((*video&0x80>>(x&7)) !=0);
  }
  else if (mode == 19)
    {
    vid= MK_FP (0, y * MaxX + x );
    video= MK_FP (SCHERMPTR, vid);
    return(*video !=0);
    }
  else if (mode>16)
     {
     _AX = y;
     _BX = x;
     getpix();
     return((int)_CH);
     }
  else if (mode >12)
     {
    video= MK_FP (SCHERMPTR, (MaxX>>3) * y + (x>>3) );
    return((*video&0x80>>(x&7)) !=0);
     }
  else
  {
    video= MK_FP (SCHERMPTR,0x2000 * (y&1) +  80 * (y>>1) + (x>>3) );
    return((*video&0x80>>(x&7) ) !=0);
  }
}
/**************************** INITDISPLAY ********************************/
void initdisplay()
{
union REGS reg;
reg.h.ah=15;
int86(0x10,&reg,&reg);
page=(int)reg.h.bh;
mode=reg.h.al;
if(mode==4)  { MaxX=640; MaxY=200; SCHERMPTR  = (int *) 0XB800L ;}
if(mode==5)  { MaxX=640; MaxY=200; SCHERMPTR  = (int *) 0XB800L ;}
if(mode==6)  { MaxX=640; MaxY=200; SCHERMPTR  = (int *) 0XB800L ;}
if(mode==7)  { MaxX=720; MaxY=348; SCHERMPTR  = (int *) 0XB000L ;}
if(mode==13) { MaxX=320; MaxY=200; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==14) { MaxX=640; MaxY=200; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==15) { MaxX=640; MaxY=350; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==16) { MaxX=640; MaxY=350; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==17) { MaxX=640; MaxY=480; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==18) { MaxX=640; MaxY=480; SCHERMPTR  = (int *) 0XA000L ;}
if(mode==19) { MaxX=320; MaxY=200; SCHERMPTR  = (int *) 0XA000L ;}

if ( mode<4 || (mode>0x18 && mode<0x25) ) { geninterrupt(5); exit(1);}
                                   /* call print screen interrupt */
}

/****************************** PRNONOFF ******************************/
void prnonoff()
{

if(biosprint(2,0,0) != 0x90)
 {
  sound(4200);
  delay(83);
  nosound();
  sound(200);
  delay(300);
  nosound();
  exit(-1);
 }
/* Verlaat het programma als de printer uitstaat*/
 }

/*************************  ASM SOURCE VOOR VEGA **********************;

;**********************************************************************;
;*                              V E G A                               *;
;*--------------------------------------------------------------------*;
;*    Task           : Creates elementary functions for accessing the *;
;*                     graphic modes on an EGA/VGA card               *;
;*--------------------------------------------------------------------*;
;*    Author         : MICHAEL TISCHER                                *;
;*    Modified by    : Ed Nieuwenhuys                                 *;
;*    Developed on   :  10/3/1988                                     *;
;*    Last update    :  9/8/1990                                      *;
;*--------------------------------------------------------------------*;
;*    Assembly       : MASM VEGA;                                     *;
;*                     LINK VEGA;                                     *;
;*--------------------------------------------------------------------*;
;**********************************************************************;

;== Constants ==========================================================

VIO_SEG     = 0A000h              ;Segment address of video RAM
                                  ;in graphic mode
LINE_LEN    = 80                  ;Every graphi line in EGA/VGA graphic
                                  ;modes require 80 bytes
BITMASK_REG = 8                   ;Bitmask register
MODE_REG    = 5                   ;Mode register
FUNCSEL_REG = 3                   ;Function select register
MAPSEL_REG  = 4                   ;Map-Select register
ENABLE_REG  = 1                   ;Enable Set/Reset register
SETRES_REG  = 0                   ;Set/Reset register
GRAPH_CONT  = 3CEh                ;Port addressd of graphic controller
OP_MODE     = 0                   ;Comparison operator mode:
                                  ;   00h = Replace
                                  ;   08h = AND comparison
                                  ;   10h = OR comparison
                                  ;   18h = EXCLUSIVE OR comparison

GR_640_350  = 10h                 ;BIOS code for 640x350-pixel
                                  ;16-color graphic mode
TX_80_25    = 03h                 ;BIOS code for 80*25-char.


PUBLIC _getpix
PUBLIC _setpix
PUBLIC _fillscr

_TEXT    segment    byte public 'CODE'
    assume    cs:_TEXT,ds:_DATA
_TEXT    ends

_DATA    segment word public 'DATA'
d@    label    byte
d@w    label    word
b@    label    byte
b@w    label    word
_DATA    ends

_TEXT    segment    byte public 'CODE'

;== Code ===============================================================


vega:
;-- PIXPTR: Computes the address of a pixel within video RAM for the   -
;--         new EGA/VGA graphic modes
;-- Input    : AX    = Graphic line
;--            BX    = Graphic column
;-- Output   : ES:BX = Pointer to the byte in video RAM containing pixel
;--            CL    = Number of right shifts for the byte
;--                  = Number of byte shifts in ES:BX needed to isolate
;--                    the pixel
;--            AH    = Bitmask for combining with all other pixels
;-- Registers: ES, AX, BX and CL are changed

pixptr    proc near
          push dx                 ;Push DX onto stack

          mov  cl,bl              ;Save low byte of graphic column
          mov  dx,LINE_LEN        ;Number of bytes per line to DX
          mul  dx                 ;AX = graphic line * LINE_LEN
          shr  bx,1               ;Shift graphic column three places to
          shr  bx,1               ;the right, divide by 8
          shr  bx,1
          add  bx,ax              ;Add line offset

          mov  ax,VIO_SEG         ;Load segment address of video RAM
          mov  es,ax              ;into ES

          and  cl,7               ;And bits 4 - 7 of graphic column
          xor  cl,7               ;Turn bits 0 - 3 then
                                  ;subtract 7 - CL
          mov  ah,1               ;After shift, bit 0 should be
                                  ;left alone

          pop  dx                 ;Pop DX off of stack
          ret                     ;Back to caller

pixptr    endp

;-- _setpix: Sets a graphic pixel in the new EGA/VGA graphic modes ------
;-- Input    : AX    = graphic line
;--            BX    = graphic column
;--            CH    = pixel color
;-- Output   : none
;-- Registers: ES, DX and CL are changed

_setpix    proc near

          push bp
      push si
      push di

          push ax                 ;Push coordinates onto
          push bx                 ;the stack

          call pixptr             ;Computer pointer to the pixel

          mov  dx,GRAPH_CONT      ;Load port addr. of graphic controller

          ;-- Set bit position in bitmask register ---------------------

          shl  ah,cl              ;Mask for bit to be changed
          mov  al,BITMASK_REG     ;Move bitmask register from AL
          out  dx,ax              ;Write to register

          ;-- Set read mode 0 and write mode 2 -- ----------------------

          mov  ax,MODE_REG + (2 shl 8) ;Reg. no. and ,mode value
          out  dx,ax              ;Write in the register

          ;-- Define comparison mode between preceding latch -----------
          ;-- contents, and CPU byte                         -----------

          mov  ax,FUNCSEL_REG + (OP_MODE shl 8) ;Write register number
          out  dx,ax              ;and comparison operator

          ;-- Pixel control --------------------------------------------

          mov  al,es:[bx]         ;Load latches
          mov  es:[bx],ch         ;Move color into bitplanes

          ;-- Set altered registers to their default (BIOS) ------------
          ;-- status                                        ------------

          mov  ax,BITMASK_REG + (0FFh shl 8) ;Set old bitmask
          out  dx,ax              ;Write in the register
          mov  ax,MODE_REG        ;Write old value for for mode register
          out  dx,ax              ;into register
          mov  ah,FUNCSEL_REG     ;Write old value for function select
          out  dx,ax              ;register into register

          pop  bx                 ;Pop coordinates off of stack
          pop  ax                 ;
          pop di
      pop si
      pop bp
          ret                     ;Back to caller

_setpix    endp

;-- _getpix: Places a pixel's color in one of the new EGA/VGA -----------
;--         graphic modes
;-- Input    : AX    = graphic line
;--            BX    = graphic column
;-- Output   : CH    = graphic pixel color
;-- Registers: ES, DX , CX and DI are changed

_getpix    proc near

          push bp
      push si
      push di

          push ax                 ;Push coordinates onto
          push bx                 ;the stack

          call pixptr             ;Computer pointer to pixel
          mov  ch,ah              ;Move bitmask to CH
          shl  ch,cl              ;Shift bitmask by bit positions
                                                                        
          mov  di,bx              ;Move video RAM offset to DI
          xor  bl,bl              ;Color value will be computed in BL

          mov  dx,GRAPH_CONT      ;Load graphic controller port address
          mov  ax,MAPSEL_REG + (3 shl 8) ;Access bitplane #3
                                                                        
          ;-- Go through each of the four bitplanes --------------------

gp1:      out  dx,ax              ;Activate bitplane #AH only
          mov  bh,es:[di]         ;Get byte from the bitplane
          and  bh,ch              ;Omit uninteresting bits
          neg  bh                 ;Bit 7 = 1, when a pixel is set
          rol  bx,1               ;Shift bit 7 from BH to Bit 1 in BL

          dec  ah                 ;Decrement bitplane number
          jge  gp1                ;Not  -1 yet? --> next bitplane
                                                                        
          ;-- The map select register must not be reset, since        --
          ;-- the EGA- and VGA-BIOS default to a value of 0           --
                                                                        
          mov  ch,bl              ;Get color from CH
          pop  bx                 ;Pop coordinates off
          pop  ax                 ;of stack
          pop di
      pop si
      pop bp                                                              
          ret                     ;Back to caller

_getpix    endp
                                                                        
;-- _fillscr: Sets all screen pixels to one color ------ ----------------
;-- Input    : AX    = number of graphic lines on the screen
;--            CH    = pixel color
;-- Output   : none
;-- Registers: ES, AX, CX, DI, DX and BL are changed
                                                                        
_fillscr   proc near
                                                                        
          push bp
      push si
      push di                                                              

          mov  dx,GRAPH_CONT      ;Load graphic controller port address
          mov  al,SETRES_REG      ;Numbmer of Set-/Reset registers
          mov  ah,ch              ;Move bit combination to AL
          out  dx,ax              ;Write to the register
                                                                        
          mov  ax,ENABLE_REG + (0Fh shl 8) ;Write 0F(h) in the
          out  dx,ax              ;Enable Set-/Reset register
                                                                        
          mov  bx,LINE_LEN / 2    ;Length of a graphic line / 2 into BX
          mul  bx                 ;Multiply by number of graphic lines
          mov  cx,ax              ;Move to CX as repeat counter
          xor  di,di              ;Address first byte in video RAM
          mov  ax,VIO_SEG         ;Segment address of video RAM
          mov  es,ax              ;Load into ES
          cld                     ;Increment on string instructions
          rep  stosw              ;Fill video RAM
                                                                        
          ;-- Return old contents of Enable Set-/Reset register    -----

          mov  dx,GRAPH_CONT      ;Load graphic controller port address
          mov  ax,ENABLE_REG      ;Write 00(h) in Enable Set-/
          out  dx,ax              ;Reset register
                                                                        
          pop di
      pop si
      pop bp                                                              
          ret                     ;Back to caller
                                                                        
_fillscr   endp
                                                                        
;== End ================================================================
                                                                        
_TEXT    ends                   ;End of code segment
    end vega
*/
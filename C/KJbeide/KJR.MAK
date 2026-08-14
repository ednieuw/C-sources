PROJ = KJ
PROJFILE = KJ.MAK
DEBUG = 1

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
BRFLAGS  =  /o $(PROJ).bsc
BROWSE	= 0
CFLAGS_G  = /AL /W4 /BATCH
CC  = cl
CFLAGS_D  = /qc /Gi$(PROJ).mdt /Zr /Zi /Od
CFLAGS_R  = /Ot
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi
LINKER	= link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
IMPLIB	= implib
LFLAGS_G  =  /NOI /ST:4096  /BATCH
LFLAGS_D  = /CO /INC /FAR /PACKC
LFLAGS_R  = /EXE /FAR /PACKC
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LLIBS_G  = graphic keyboard screen strings

OBJS  = KJ.obj RL00.obj KJ1.obj KJ0.obj KJ2.obj
SBRS  = KJ.sbr RL00.sbr KJ1.sbr KJ0.sbr KJ2.sbr

all: $(PROJ).exe

.SUFFIXES: .c .sbr .obj

KJ.obj : KJ.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj0.h\
	kj1.h kj2.h rl00.h

KJ.sbr : KJ.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj0.h\
	kj1.h kj2.h rl00.h

RL00.obj : RL00.C D:\MC\INC\conio.h D:\MC\INC\dos.h D:\MC\INC\graph.h\
	D:\MC\INC\keyboard.h D:\MC\INC\screen.h D:\MC\INC\signal.h\
	D:\MC\INC\stdlib.h

RL00.sbr : RL00.C D:\MC\INC\conio.h D:\MC\INC\dos.h D:\MC\INC\graph.h\
	D:\MC\INC\keyboard.h D:\MC\INC\screen.h D:\MC\INC\signal.h\
	D:\MC\INC\stdlib.h

KJ1.obj : KJ1.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj0.h rl00.h

KJ1.sbr : KJ1.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj0.h rl00.h

KJ0.obj : KJ0.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj1.h kj2.h rl00.h

KJ0.sbr : KJ0.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj1.h kj2.h rl00.h

KJ2.obj : KJ2.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj0.h rl00.h

KJ2.sbr : KJ2.C D:\MC\INC\conio.h D:\MC\INC\ctype.h D:\MC\INC\graphic.h\
	D:\MC\INC\keyboard.h D:\MC\INC\malloc.h D:\MC\INC\memory.h\
	D:\MC\INC\screen.h D:\MC\INC\stdio.h D:\MC\INC\stdlib.h\
	D:\MC\INC\string.h D:\MC\INC\strings.h D:\MC\INC\time.h kjas.h kj.h\
	kj0.h rl00.h


$(PROJ).bsc : $(SBRS)
	$(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_R)
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
!IF $(DEBUG)
	$(ILINK) -a -e "$(LINKER) @$(PROJ).lrf" $@
!ELSE
	$(LINKER) @$(PROJ).lrf
!ENDIF


.c.sbr :
!IF $(DEBUG)
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FR$@ $<
!ELSE
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FR$@ $<
!ENDIF

.c.obj :
!IF $(DEBUG)
	$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
	$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
	$(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
	CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)

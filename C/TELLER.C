#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>

void main()
{
	struct time begin,eind;
	register long tel=0,tel1=0;
	register int fd;
	int handle;
	float sec=0;
	clock_t start, end;
do
  {
	start = clock();
	do
	{
//		fd=_open("bestand",O_RDWR);   _close(fd);
		_dos_open("bestand",O_RDWR,&handle);   _dos_close(handle);
		tel++;
		sec=(clock()- start) / CLK_TCK;
	}
	while(sec<10.0);
	printf("iteraties/sec: %5.0f sec: %f\n",(float)tel/sec,sec);
	tel=0;
	tel1++;
 }
 while(tel1<1);
}



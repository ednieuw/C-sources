#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <stdlib.h>
#include <string.h>
#include <share.h>


void main()
{
	FILE *fp;
	struct time begin,eind;
	register long tel=0,tel1=0;
	int fdin,fdout,fdout1;
	float sec=0;
	clock_t start, end;
	char text[]="This is a test made by Ed \0";
	char filenaam[20]="test.tst";
	char filenaam1[20]="test1.tst";
	void *buf;
	long bytes=0;
	long bufsize=0;

	if ((fdout = open(filenaam, O_CREAT | O_WRONLY|O_DENYNONE)) == -1)
	  { 	perror("Open Error"); exit(1);}

	for(tel=0;tel<100;tel++)
	 {
		if ((bytes = write(fdout, text, strlen(text))) == -1)
		 {
			printf("1 Write Failed.\n");
			exit(1);
		 }
	bufsize+=bytes;
	}
	if (close(fdout) == -1)
	  {	perror("close Error");	exit(1);	  }
	printf("write: %d bytes written.\n",bufsize);
	if(bufsize>50000){printf("file >50.000 bytes\n"); exit(1);}
	buf=malloc(bufsize+10);

do
  {
  start = clock();
  do
  {

	fdin =open(filenaam,O_RDONLY);
	if (( read(fdin, buf, bufsize)) == -1)
	  {	perror("2 Read Error");	exit(1);	  }
	close(fdin);

	if ((fdout = open(filenaam, O_CREAT | O_WRONLY)) == -1)
	  { 	perror("4 Open Error"); exit(1);}
	if ((bytes = write(fdout, buf,bufsize)) == -1)
	  {	perror("3 Write Error");		exit(1);	  }
	close(fdout);

	tel++;
	sec=(clock()- start) / CLK_TCK;
  }
	while(sec<10.0);
 printf("iteraties/sec: %5.0f |  %f sec\n",(float)tel/sec,sec);
 tel=0;
 tel1++;
 }
 while(tel1<1);
 free(buf);
}



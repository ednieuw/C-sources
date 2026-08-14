#include "stdio.h"
#include "dir.h"
#include "dos.h"

main()
{
struct ffblk ffblk;
int done;
printf("Searching hidden file\n");

done=findfirst("*.*",&ffblk,255);
while(!done)
 {
   if((ffblk.ff_attrib & 2)==2)
       printf("  %s  %d  %d\n",ffblk.ff_name,ffblk.ff_attrib,ffblk.ff_attrib&2);
     done= findnext(&ffblk);
     }
     }

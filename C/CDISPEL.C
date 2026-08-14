
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "time.h"
#include "bios.h"
#include "math.h"

short amatrix[50][22],matrix_beste[50][22],ress[50],bress[50];
short row,column;

short matrix[50][22]={ {0,3,5,2,4,4,9,6,6,0,3,4,0,2,6,5,0,6,8,0} };

void einde(void);
short  tel_nul(int rij);

int main()
{
 short m,n,no1,no2,noo,rij,rijbeste=23;
 short digit1,digit2,res;
 short aantal_nullen;
long tottel=0;
 randomize();
 while(!kbhit())
 {
/* for(m=0;m<50;m++)
  for(n=0;n<19;n++)
   matrix[m][n]=0;
*/
 tottel++;
 matrix[0][0]=0;
 matrix[0][1]=3;
 matrix[0][2]=5;
 matrix[0][3]=2;
 matrix[0][4]=4;
 matrix[0][5]=4;
 matrix[0][6]=9;
 matrix[0][7]=6;
 matrix[0][8]=6;
 matrix[0][9]=0;
 matrix[0][10]=3;
 matrix[0][11]=4;
 matrix[0][12]=0;
 matrix[0][13]=2;
 matrix[0][14]=6;
 matrix[0][15]=5;
 matrix[0][16]=0;
 matrix[0][17]=6;
 matrix[0][18]=8;
 matrix[0][19]=0;


/* randomize();   */
 rij=0;
 while(tel_nul(rij)<19)
   {
   no1=19;
   while(matrix[rij][no1]==0)                 no1=random(19);
   no2=random(19);
   while(matrix[rij][no2]==0 || no1==no2)     no2=random(19);

   res=matrix[rij][no1]*matrix[rij][no2];
   ress[rij]=res;
   digit1=res/10;
   digit2=res%10;
   if(no1>no2){ noo=no2; no2=no1; no1=noo;}
   for(n=0;n<19;n++)	matrix[rij+1][n]=matrix[rij][n];

   matrix[rij+1][no1]=digit1;
   matrix[rij+1][no2]=digit2;
   rij++;
   if(rij>23) break;
   }
   if(rij<rijbeste)
   {
    puts("\r\n");
    for(m=0;m<=rij;m++)
     {
      bress[m]=ress[m];
      bress[m+1]=0;
      puts("\r\n");
      for(n=0;n<19;n++)
       {
	matrix_beste[m][n]=matrix[m][n];
	printf("%d",matrix[m][n]);
	rijbeste=rij;
       }
       printf(" %d  %d",m,ress[m]);
     }
      printf("\r\ntotaal doorgerekend %ld\r\n",tottel);


    for(m=0;m<=rijbeste;m++)
     {
      fputs(" \r\n",stdprn);
      for(n=0;n<19;n++)
       {
	fprintf(stdprn,"%d",matrix_beste[m][n]);
       }
       fprintf(stdprn," %d  %d",m,bress[m]);
     }
  fprintf(stdprn,"\r\ntotaal doorgerekend %ld\r\n",tottel);
   fputs("\r\n",stdprn);
  }
  }
   fprintf(stdprn,"\r\ntotaal doorgerekend %ld\r\n",tottel);
   fputs("\r\n",stdprn);

  einde();
  return(1);
}

short  tel_nul(int rij)
{
 int n,tel=0;
 for (n=0;n<20;n++)
   if(matrix[rij][n]==0) tel++;
 return tel;
}

void einde(void )
{
 exit(1);
}


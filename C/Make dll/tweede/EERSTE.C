int  Reken(double *row, int rowaantal,double *res1  ,double *res2,double *res3);

int  Reken(double *row, int rowaantal,double *res1  ,double *res2,double *res3)
{
  int n;
 *res1=*res2=*res3=0 ;
	for (n=0;n<rowaantal;n++)
		{
		 (*res1)+= row[n];
		 (*res2)++;
		 }
  *res3=(*res1) / (*res2);
 return (123);
 }

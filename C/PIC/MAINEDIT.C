
int	main(int argc,char **argv)
{
  char argvfile[15];
  int keuze=999;
   if(argc<1) exit(-1);
   clrscr();
   strcpy(argvfile,argv[1]);

   initdisplay();     /* init tekstscherm voor swrite */
   clrscr();
#define PIC     0
#define PRN     1
#define DTA     2
   if(arc==2)
   {
	  while(keuze>3)
	  {
	   box(4,28,5,16,2,0);
	   swrite("30,5,"1 PIC ---> PRN\0",0x1300);
	   swrite("30,6,"2 PRN ---> PIC\0",0x1300);
	   swrite("30,7,"3 DTA ---> PIC\0",0x1300);
	   keuze = getch()-49;
	  }


   switch(keuze)
	{
   case PIC:
		   if (argc==1) filenaam=filedir("*.dta");
		   pic2prn(filenaam, "ed.txt");
		   break;

   case PRN:
		   if (argc==1) filenaam=filedir("*.dta");
		   prn2pic(filenaam, "ed.txt");
		   teken_graph("q.pic");
		   break;

   case DTA:
		   if (argc==1) filenaam=filedir("*.dta");
		   dta2prn_file(filenaam, "ed.txt");
		   teken_graph("q.pic");
		   break;
	}
   exit(0);
}


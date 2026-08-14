#include "ttest.h"
/* module $opt.c */
int get_options(int graph_type);
int modify_text(void);
void read_environment(void);
int stat_menu(void);
extern void   p_parameters(int i, int show_flag);

/* local routines */
void  column_menu(void);
void  eval_menu(void);
void  extra_text_menu(void);
void  fill_menu(void);
void  legend_menu(void);
void  linetype_menu(void);
void  minmax_menu(void);
void  modify_prob_strings(void);
int   save_menu(void);
float set_env_variable(char *env_string, float default_value);
void  symbol_menu(int flag);
void  ycal_type_menu(void);
void  yesno(int flag, char *yesstring, char *nostring);

int get_options(int graph_type) {
int i, n, option, c, hgrid, vgrid;
int *ip;
float f;
char default_string[41];
   for (;;) {
      write_stat_strings();
      do {
         hgrid=Showclasses%2;
         vgrid=Showclasses/2;
         clrscr();
         if ( graph_type==DENDROGRAM) {
               cprintf("O - Orthogonal layout: %s\r\n",Ortho?"YES":"NO");
               cprintf("S - Statistic for correlation:\r\n");
               yesno(Mean_flag,"Pearson","Spearman");
               cprintf("X - modify length of X-axis (default:%d)\r\n", Clxlength);
         } else if (graph_type==XYCLUSTER) {
               cprintf("C - Clustering coefficient   :");
               yesno(Distance,"Distance","Correlation coefficient");
               cprintf("D - Data-symbols in graph    :");
               yesno(Fullname, "Full testname","Single letter");
               cprintf("S - Statistic for correlation:");
               yesno(Mean_flag,"Pearson","Spearman");
               cprintf("X - X-axis length           :%d/%d)\r\n", Clxlength, X_length);
         } else if (graph_type==DOTMATRIX || cormatrix_flag) {
               cprintf("B - Bars or Dots            :");
               yesno(!Dotflag, "Bar", "Dot");
/*
               cprintf("C - Color or Black/White    :");
               yesno(Color>1?TRUE:FALSE, "Color", "B/W");
*/
               if (graph_type!=DOTMATRIX) {
                  cprintf(
                       "C - Coefficient             :");
                  yesno(Distance,"Distance","Correlation coefficient");
               }
               cprintf("D - Data-symbols            : %c, %c\r\n", Symbol_string[1], Symbol_string[4]);
               cprintf("G - Grid (Horizontal)       :");
               yesno(hgrid, "Yes", "No");
               cprintf("         (Vertical)         :");
               yesno(vgrid, "Yes", "No");
               cprintf("L - Log-transformation      :");
               yesno(Logflag, "Yes", "No");
               cprintf("M - MinMax:\r\n");
               cprintf("       Ymin                 : %s\r\n",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
               cprintf("       Ymax                 : %s\r\n",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
               cprintf("P - Position of calibrator  : ");
               if (Legendpos<3) cprintf("Below data\r\n");
               else if (Legendpos==3) cprintf("Left of data\r\n");
               else if (Legendpos==4) cprintf("No calibrator\r\n");
               cprintf("S - Sorted data             :");
               yesno(Rank_column, "Yes", "No");
               if (Rank_column) {
                  if (Outcome_test>-1) cprintf("   Data sorted by test %s\r\n",testname[Outcome_test]);
                  else                 cprintf("   Automatic sorting by row AND column\r\n");
               }
               cprintf("X - X-axis length           :%d/%d)\r\n", Clxlength, X_length);
         } else {
            if (graph_type==OGIVE || Stat==-1)
               cprintf("A - Axis format             : %c\r\n", Ycal<10?'0'+Ycal:'A'+Ycal-10);
            else {
               cprintf("A - Axis and errorbar format: %c\r\n", Ycal<10?'0'+Ycal:'A'+Ycal-10);
               if (graph_type== TTEST) {
                  cprintf("       line length          : %.2f x column-width\r\n",Stat);
                  if (Combi) cprintf("       cap                  : %.2f x bar-size\r\n", Cap);
                  else cprintf("       tip                  : %.2f x symbol-size\r\n", Tip);
                  if (Sd_flag || Sem_flag || Iqr_flag) {
                     cprintf("       Error-line position  : ");
                     if (Sd_flag==1 || Sem_flag==1 || Iqr_flag==1) cprintf("Central\r\n");
                     if (Sd_flag==2 || Sem_flag==2 || Iqr_flag==2) cprintf("Right\r\n");
                     if (Sd_flag==3 || Sem_flag==3 || Iqr_flag==3) cprintf("Left\r\n");
                     if (Sd_flag==4 || Sem_flag==4 || Iqr_flag==4) cprintf("Inside\r\n");
                     if (Sd_flag==5 || Sem_flag==5 || Iqr_flag==5) cprintf("Outside\r\n");
                  }
               } else
               cprintf("       cap                  : %.2f x bar-width\r\n", Cap);
            }
            cprintf(   "B - Black/white or color    :");
            yesno(Color>1?TRUE:FALSE, "Color", "B/W");
            if (graph_type!=OGIVE && !(Subselect==3 && graph_type==TTEST)) {
               cprintf("C - Connecting lines        :");
               yesno(Line, "Yes", "No");
            }
   /*
            else if (graph_type==PERCENTAGE_POSITIVE && Cutoff_flag) {
               cprintf(   "C - Cut-off values\r\n"
                         "    from worksheet row:");
               yesno(Cutoff_flag==1, "Yes", "No");
            }
   */
            if (graph_type==TTEST) {
               cprintf(   "D - Data-symbols            :");
               yesno((Datasymbol==-1), "Yes", "No");
               if (Datasymbol==-1)cprintf(   "       symbol-sequence      : \"%s\"\r\n",Symbol_string);
               else               cprintf(   "       symbol used          : %d\r\n",Datasymbol);
                                  cprintf(   "       size                 : %s\r\n", (Symbol_size==-1)?"AUTO":itoa(Symbol_size,"         ",10));
               if (Datasymbol==-1) {
                  if (Legendpos==0 && !Legend)
                                  cprintf(   "       symbol-legends printed in %d column(s)\r\n",Legend);
                  if (Legendpos!=0)
                                  cprintf(   "       symbol-legends printed at position %d\r\n",Legendpos);
               }
               if (Subselect && !No_ranges)
                  cprintf("    second column legend %s plotted points\r\n", (Ranges_in_header)?"ABOVE":"BELOW");
            }
            else if (graph_type==OGIVE) {
               cprintf(   "D - Different linetypes     :");
               yesno((Datasymbol==-1), "Yes", "No");
               if (Datasymbol==-1)cprintf(   "       line-sequence        : \"%s\"\r\n",Symbol_string);
               else               cprintf(   "       line-type used       : %d\r\n",Datasymbol);
            }
            if (graph_type==TTEST || graph_type==BAR_GRAPH || graph_type==OGIVE) {
               cprintf(   "F - Fill shaded block       :");
               yesno(fillmode(), "Yes", "No");
            }
/*
            if (graph_type==TTEST) {
               cprintf(   "G - show Grid               :");
               yesno(Showclasses, "Yes", "No");
            }
*/
            if (graph_type==BAR_GRAPH) {
               cprintf(   "G - Graph type              :");
               yesno(Median_flag==2, "Box", "Bar");
            }
            if (graph_type==TTEST && Cutoff_flag) {
               cprintf(   "H - Horizontal cut_off lines:");
               yesno(Cutoff_flag==1, "Yes", "No");
            }
            if (graph_type== TTEST ||
                graph_type== BAR_GRAPH ||
                graph_type== PEARSON ||
                graph_type== OGIVE) {
                  cprintf("L - Log-transformation      :");
                yesno(Logflag, "Yes", "No");
                }
            cprintf(      "M - MinMax:\r\n");
            cprintf(      "       Ymin                 : %s\r\n",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
            cprintf(      "       Ymax                 : %s\r\n",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
            if (graph_type==TTEST)
               cprintf(   "N - Number of classes       : %d\r\n",Noofclasses);
            if (
                  (graph_type == BAR_GRAPH) ||
                  graph_type == PEARSON ||
                  graph_type == SPEARMAN ||
                  graph_type == PERCENTAGE_POSITIVE ||
                  graph_type == RELATIVE_RISK ||
                  graph_type == ODDS_RATIO) {
               cprintf(   "N - Number of observations  :");
               yesno(Nn, "Yes", "No");
            }
            if (Subselect && graph_type!=OGIVE && !(graph_type==BAR_GRAPH && Median_flag==2)) {
               cprintf(   "R - Ranges shown in graph   :");
               yesno(No_ranges?0:1, "Yes", "No");
            }
            if (graph_type==TTEST || graph_type==BAR_GRAPH) {
               cprintf(     "S - Statistic lines         : %s\r\n", (Stat==-1)?"none":range_string);
               cprintf(     "    text of probability     :");
               yesno(Prob_flag, "Shown", "Not shown");
               if (Mean_flag)
                  cprintf(  "    ttest analysis mode     : %s\r\n",statmode_string[Mean_flag-1]);
            } else if (graph_type==OGIVE) {
               if (Centile)
               cprintf(     "S - Statistic:       Centile: %d\r\n",(int)(100.01*Centile));
               else cprintf("S - Statistic               : Median\r\n");
            } else if (
               graph_type==PERCENTAGE_POSITIVE ||
               graph_type==PEARSON ||
               graph_type==SPEARMAN ||
               graph_type==RELATIVE_RISK ||
               graph_type==ODDS_RATIO) {
               cprintf(   "S - Statistic:\r\n"
                         "    confidence interval     :");
               yesno(Sem_flag, "SE", "95% CI");
            }
            cprintf(      "X - length of X-axis        : %d\r\n",X_length);
         }
         if ((graph_type==TTEST && !Logflag) ||
             (graph_type==DOTMATRIX) ||
             ( graph_type==DENDROGRAM ||
               (
                (graph_type==PEARSON || graph_type==SPEARMAN) &&
                (Outcome_test==-1 && Graph_type!=XYCLUSTER)
               )
             )
           ) {
            cprintf(   "Z - Zero forced             :");
            yesno(Zero, "Yes", "No");
         }
         cprintf("\r\nSelect option or press RETURN to accept current settings: ");
         option=getkey(0);
         if (option==RETURN || option==EXEC || option==ESC) {
            clrscr();
            return 0;
         }
      } while (strpos("ABCDFGHLMNOPRSXZ0",option,0)==-1);
      switch (option) {
         case 'A':
            if (graph_type==DOTMATRIX ||
                graph_type==DENDROGRAM ||
                graph_type==XYCLUSTER ||
                cormatrix_flag) break;
            ycal_type_menu();
            clrscr();
            if (Stat==-1 || ((Graph_type==BAR_GRAPH || Graph_type==TTEST) && Median_flag==2)) break;
            if (
               graph_type == BAR_GRAPH ||
               graph_type == PEARSON ||
               graph_type == SPEARMAN ||
               graph_type == PERCENTAGE_POSITIVE ||
               graph_type == RELATIVE_RISK ||
               graph_type == ODDS_RATIO) {
               sprintf(default_string,"%.2f",Cap);
               cprintf("\r\n0     = no cap on error-bar\r\n"
                           "0 - 2 = length of cap-line as fraction of bar size (default:%s): ",default_string);
               if (keycopy(buffer, default_string,40)==0) return 0;
               Cap=atof(buffer);
               if (Cap>2) Cap/=100;
               if (Cap>2) Cap=CAP;
            } else if (Graph_type == TTEST) {
               if (Sd_flag || Sem_flag || Iqr_flag) {
                  cprintf("Error-line position:\r\n"
                             "   L = Left\r\n"
                             "   C = Central\r\n"
                             "   R = Right\r\n"
                             "   I = Inside (alternating Right/Left)\r\n"
                             "   O = Outside (alternating Left/Right)\r\n"
                             "(Default: ");
                  if (Sd_flag==1 || Sem_flag==1 || Iqr_flag==1) cprintf("Central): ");
                  if (Sd_flag==2 || Sem_flag==2 || Iqr_flag==2) cprintf("Right): ");
                  if (Sd_flag==3 || Sem_flag==3 || Iqr_flag==3) cprintf("Left): ");
                  if (Sd_flag==4 || Sem_flag==4 || Iqr_flag==4) cprintf("Inside): ");
                  if (Sd_flag==5 || Sem_flag==5 || Iqr_flag==5) cprintf("Outside): ");
                  c=getkey(0);
                  if (c=='C') {
                    if (Sd_flag) Sd_flag=1;
                    if (Sem_flag) Sem_flag=1;
                    if (Iqr_flag) Iqr_flag=1;
                  }
                  if (c=='R') {
                    if (Sd_flag) Sd_flag=2;
                    if (Sem_flag) Sem_flag=2;
                    if (Iqr_flag) Iqr_flag=2;
                  }
                  if (c=='L') {
                    if (Sd_flag) Sd_flag=3;
                    if (Sem_flag) Sem_flag=3;
                    if (Iqr_flag) Iqr_flag=3;
                  }
                  if (c=='I') {
                    if (Sd_flag) Sd_flag=4;
                    if (Sem_flag) Sem_flag=4;
                    if (Iqr_flag) Iqr_flag=4;
                  }
                  if (c=='O') {
                    if (Sd_flag) Sd_flag=5;
                    if (Sem_flag) Sem_flag=5;
                    if (Iqr_flag) Iqr_flag=5;
                  }
                  cprintf("\r\n");
                  if (Sd_flag==1 || Sem_flag==1 || Iqr_flag==1) cprintf("Central");
                  if (Sd_flag==2 || Sem_flag==2 || Iqr_flag==2) cprintf("Right");
                  if (Sd_flag==3 || Sem_flag==3 || Iqr_flag==3) cprintf("Left");
                  if (Sd_flag==4 || Sem_flag==4 || Iqr_flag==4) cprintf("Inside");
                  if (Sd_flag==5 || Sem_flag==5 || Iqr_flag==5) cprintf("Outside");
                  cprintf("\r\n\r\n");
               }
               if (!(Mean_flag || Median_flag)) break;
               sprintf(default_string,"%.2f",Stat);
               cprintf("Options for error-lines:\r\n\r\n");
               if (Sd_flag>1 || Sem_flag>1 || Iqr_flag>1) {
                    cprintf("S = Symbol in error-line\r\n"
                            "B = Bar in error-line\r\n"
                            "default: %s ",Combi?"Bar":"Symbol");
                    c=getkey(0);
                    if (c=='S') Combi=0;
                    if (c=='B') Combi=COMBI;
                    cprintf("\r\n%s\r\n",Combi?"Bar":"Symbol");
                    cprintf("\r\n0.01 - 2 = horizontal position of error line as fraction of column width (%s): ", default_string);
               } else cprintf("\r\n0        = no statistics in distribution picture\r\n"
                                "0    - 2 = length of lines as fraction of column width (default:%s): ",default_string);
               if (keycopy(buffer, default_string,40)==0) return 0;
               Stat=atof(buffer);
               if (Stat>2) Stat/=100;
               if (Stat>2) Stat=STAT;
               if (Sd_flag>1 || Sem_flag>1 || Iqr_flag>1) {
                  if (!Combi) {
                     sprintf(default_string,"%d",Legend_symbol_size);
                     cprintf("\r\nSize of %s-symbol in error line (4-400, default %d): ",Mean_flag?"Mean":"Median",Legend_symbol_size);
                     if (keycopy(buffer, default_string,40)==0) return 0;
                     i=atoi(buffer);
                     if (i>=4 && i<=400) Legend_symbol_size=i;
                  } else {
                     sprintf(default_string,"%d",Combi);
                     cprintf("\r\nBar width for error line (1-400, default %d): ",Combi);
                     if (keycopy(buffer, default_string,40)==0) return 0;
                     i=atoi(buffer);
                     if (i>=1 && i<=400) Combi=i;
                  }
               }
               sprintf(default_string,"%.2f",Combi?Cap:Tip);
               cprintf("\r\n\r\n0     = no %s on error-line\r\n"
                               "0 - 4 = length of %s-line as fraction of %s size (default:%s): ",Combi?"cap":"tip",Combi?"cap":"tip",Combi?"bar":"symbol",default_string);
               if (keycopy(buffer, default_string,40)==0) return 0;
               f=atof(buffer);
               if (f>4) f/=100;
               if (!Combi) {
                   if (f<4) Tip=f;
               } else {
                   if (f<4) Cap=f;
               }
            }
            break;
         case 'B':
            if (graph_type==DOTMATRIX || cormatrix_flag) Dotflag=(Dotflag)?0:1;
            else Color=(Color>1)?1:MAXCOLOR;
            break;
         case 'C':
            if (graph_type==XYCLUSTER || cormatrix_flag) Distance=(Distance)?0:1;
            else if (graph_type==DOTMATRIX) Color=(Color>1)?1:MAXCOLOR;
            else Line=(Line)?0:1;
/*
            if (graph_type!=PERCENTAGE_POSITIVE) Line=(Line)?0:1;
            else {
               Cutoff_flag=(Cutoff_flag==2)?1:2;
               calc_flag=FALSE;
            }
*/
            break;
         case 'D':
            if (graph_type==TTEST || graph_type==DOTMATRIX || cormatrix_flag) symbol_menu(TRUE);
            else if (graph_type==OGIVE) linetype_menu();
            else if (graph_type==XYCLUSTER) Fullname=(Fullname)?0:1;
            break;
         case 'F':
            fill_menu();
            break;
         case 'G':
            if (graph_type==TTEST) Showclasses=(Showclasses)?0:1;
            else if (graph_type==BAR_GRAPH) {
               Median_flag=(Median_flag==2)?1:2;
               Mean_flag=Sem_flag=Sd_flag=0;
               Iqr_flag=1;
               Centile=0;
               Stat=STAT;
               calc_flag=FALSE;
            } else if (graph_type==DOTMATRIX || cormatrix_flag) {
               clrscr();
               strcpy(buffer, "NHVB");
               cprintf("N = No grid\r\n"
                       "H = Horizontal grid\r\n"
                       "V = Vertical grid\r\n"
                       "B = Both horizontal and vertical grid\r\n\r\n"
                       "Default: %c ",buffer[Showclasses]);
               c=strpos(buffer,getkey(0),0);
               if (c>=0 && c<4) Showclasses=c;
/*
               cprintf("Horizontal Grid (Y/N) default:%c ",hgrid?'Y':'N');
               c=getkey(0);
               if (c=='Y') hgrid=TRUE;
               if (c=='N') hgrid=FALSE;
               if (c==SPACE) hgrid=(hgrid)?0:1;
               cprintf("%c\r\n",hgrid?'Y':'N');
               cprintf("Vertical Grid (Y/N) default:%c ",vgrid?'Y':'N');
               c=getkey(0);
               if (c=='Y') vgrid=TRUE;
               if (c=='N') vgrid=FALSE;
               if (c==SPACE) vgrid=(vgrid)?0:1;
               Showclasses=2*vgrid+hgrid;
*/
            }
            break;
         case 'H':
            Cutoff_flag=(Cutoff_flag==2)?1:2;
            calc_flag=FALSE;
            break;
         case 'L':
            calc_flag=FALSE;
            if (graph_type== TTEST ||
               graph_type== BAR_GRAPH ||
               graph_type== DOTMATRIX ||
               graph_type== PERCENTAGE_POSITIVE ||
               graph_type== OGIVE ||
               cormatrix_flag) {
                  Logflag=(Logflag)?0:1;
                  if ((graph_type==TTEST || graph_type==BAR_GRAPH) && Prob_flag) write_prob_strings(FALSE);
               }
            else if (graph_type== PEARSON) do {
               clrscr();
               cprintf("1 - Log-transformation primary test(s) :");
               yesno(Logflag, "Yes", "No");
               cprintf("2 - Log-transformation 2nd test :");
               yesno(Logflag2, "Yes", "No");
               cprintf("RETURN to accept ");
               n=getch();
               if (n=='1') Logflag=(Logflag)?0:1;
               if (n=='2') Logflag2=(Logflag2)?0:1;
            } while (n!=RETURN);
            break;
         case 'M':
            calc_flag=FALSE;
            minmax_menu();
            break;
         case 'N':
            if (graph_type==TTEST) {
               CLS;
               cprintf("\r\nnumber of classes (max=%d,default=%d) : ",MAXNOOFCLASSES, Noofclasses);
               buffer[0]=5;
               cgets(buffer);
               n=atoi(buffer+2);
               if (n>1 && n<=MAXNOOFCLASSES) Noofclasses=n;
            } else Nn=(Nn) ? 0: 1;
            break;
         case 'O':
         case '0':
            if ((graph_type==PEARSON ||
                graph_type==SPEARMAN) &&
                Outcome_test==-1) Ortho=(Ortho)?0:1;
            break;
         case 'P':
            if (graph_type==DOTMATRIX || cormatrix_flag) {
                if (Legendpos<3) Legendpos=3;
                else if (Legendpos==3) Legendpos=4;
                else Legendpos=0;
            }
            break;
         case 'R':
            No_ranges=(No_ranges)?0:1;
            break;
         case 'S':
            if (cormatrix_flag) break;
            if (graph_type==DOTMATRIX) {
               Rank_column=(Rank_column)?0:1;
               if (Rank_column) Outcome_test=select_outcome_test();
               break;
            }
            calc_flag=0;
            if (graph_type==PERCENTAGE_POSITIVE ||
               graph_type==PEARSON ||
               graph_type==SPEARMAN ||
               graph_type==RELATIVE_RISK ||
               graph_type==ODDS_RATIO) Sem_flag=Sem_flag?0:1;
            else if (graph_type==DENDROGRAM ||
                     graph_type==XYCLUSTER) {
                Mean_flag=(Mean_flag)?0:1;
                Median_flag=(Mean_flag)?1:0;
            } else if (!stat_menu()) return 0;
            break;
         case 'T':
            ycal_type_menu();
            break;
         case 'X':
            cprintf("\r\nHorizontal size of picture (max=%d,default=%d) : ",X_LENGTH, X_length);
            buffer[0]=9;
            cgets(buffer);
            n=atoi(buffer+2);
            if (n>1 && n<=X_LENGTH) X_length=n;
            if (
              ((graph_type==PEARSON || graph_type==SPEARMAN) && Outcome_test==-1) ||
                graph_type==DENDROGRAM ||
                graph_type==XYCLUSTER ||
                graph_type==DOTMATRIX ||
                cormatrix_flag
            ) {
                cprintf("\r\nLength of x-axis of (max=%d,default=%d) : ",X_LENGTH, Clxlength);
                buffer[0]=9;
                cgets(buffer);
                n=atoi(buffer+2);
                if (n>1 && n<=X_LENGTH) Clxlength=n;
            }
            break;
         case 'Z':
            Zero=(Zero)?0:1;
            calc_flag=FALSE;
            break;
/*
         case SPACE:
            clrscr();
            return save_menu();
               CLS;
               cprintf("\r\nIntra-range distance  (-2.00 / +2.00, default=%.2f) : ",Intra);
               gets(buffer);
               if (*buffer) {
                   f=atof(buffer);
                   if (f>=-2.0 && f<=2.00) Intra=f;
               }
               cprintf("\r\nInter-range distance  (-2.00 / +2.00, default=%.2f) :",Inter_bar);
               gets(buffer);
               if (*buffer) {
                  f=atof(buffer);
                  if (f>=-2.0 && f<=2.00) Inter_bar=f;
               }
            break;
*/
      }
   }
}

void yesno(int flag, char *yesstring, char *nostring) {
   if (flag) {
      cprintf(" %s  ", nostring);
      HLON;
      cprintf("%s\r\n", yesstring);
      HLOFF;
   } else {
      putch(' ');
      HLON;
      cprintf("%s", nostring);
      HLOFF;
      cprintf("  %s\r\n", yesstring);
   }
}

void linetype_menu(void) {
   int c;
   clrscr();
   cprintf("Different line_types?\r\n\r\nRETURN = YES\r\nSPACE =  NO\r\nC      = YES/Color\r\nB      = NO/Color\r\n");
   c=getch();
   if (c==ESC) return;
   Color=1;
   if (c=='c' || c=='C' ) { Color=15;  Datasymbol=-1;}
   if (c=='b' || c=='B' ) { Color=15;  Datasymbol= 0;}
   if(c==RETURN) Datasymbol=-(c==RETURN);
   cprintf("\r\n\r\nLine-types available: \"%s\" \r\n"
      "0 = solid line\r\n"
      "1 = ----------\r\n"
      "2 = ..........\r\n"
      "3 = -.-.-.-.-.\r\n"
      "4 = -...-...-.\r\n"
      "5 = ========== double solid line\r\n"
      "6 = = = = = =  double dashed line\r\n"
      "7 = :::::::::: double dotted line\r\n"
      "8 = =:=:=:=:=: double dashdotted line\r\n"
      "9 = solid/dotted lines\r\n"
      , Symbol_string);
   if (Datasymbol==-1) {
      cprintf("Positions in the line code sequence actually used: %s\r\n",Symbol_string);
      cprintf("Type symbol code sequence or press RETURN\r\ncurrent symbol code sequence: \"%s\": ",Symbol_string);
      strcpy(Symbol_string, keycopy(buffer, Symbol_string, 19));
   } else {
      cprintf("Default line-type %d: ", Datasymbol);
      c=getkey(0)-'0';
      if (c==ESC) return;
      if (c!=RETURN && c>=0 && c<=9) Datasymbol=c;
   }
}

void symbol_menu(int flag) {
   int n, r, t, c, response, selected_symbol, symbolpos, column;
   if (flag) {
      clrscr();
      if (Graph_type==TTEST) {
         cprintf("Different data-symbols?\r\n\r\nRETURN = YES\r\nSPACE =NO\r\n");
         c=getch();
         if (c==ESC) return;
         if (c==SPACE) Datasymbol=1;
         else if (c==RETURN) Datasymbol=-1;
         else if (Datasymbol==-1) Datasymbol=1;
         if (Datasymbol==-1) {
            cprintf("L = Literal: actual number printed instead of symbol\r\n"
                 "S = Symbol (circle, square or triangle) instead of number\r\n"
                 "Default: %c ",(Literal)?'L':'S');
            c=getkey(0);
            if (c=='L') {
               Literal=TRUE;
               Legendpos=0;
               Legend=0;
            } else if (c=='S') {
               Literal=FALSE;
               Legend=LEGEND;
               Legendpos=LEGENDPOS;
            }
         }
      } else {
         Literal=FALSE;
         Legend=LEGEND;
         Legendpos=LEGENDPOS;
      }
      if (!Literal || Graph_type==DOTMATRIX || cormatrix_flag) {
         if (Graph_type==TTEST) putch('S');
         cprintf("\r\n\r\nSymbols available: \r\n"
         "0 = open circle\r\n"
         "1 = filled circle\r\n"
         "2 = open square\r\n"
         "3 = filled square\r\n"
         "4 = open triangle\r\n"
         "5 = filled triangle\r\n"
         "6 = open inverted triangle\r\n"
         "7 = filled inverted triangle\r\n"
         "8 = *\r\n"
         "9 = vertical line\r\n\r\n");
         if (Graph_type==DOTMATRIX || cormatrix_flag) {
            cprintf("\r\nData-symbol for valid data (default: %c)",Symbol_string[1]);
            c=getch();
            if (c!=RETURN && c!=ESC && c>='0' && c<='9') Symbol_string[1]=c;
            cprintf("\r\n\r\nData-symbol for missing data (default: %c)",Symbol_string[4]);
            c=getch();
            if (c!=RETURN && c!=ESC && c>='0' && c<='9') Symbol_string[4]=c;
            return;
         }
         if (Datasymbol==-1) {
            cprintf("Positions in the symbol code sequence actually used: %s\r\n",used_symbolstring);
            cprintf("Type symbol code sequence or press RETURN\r\ncurrent symbol code sequence: \"%s\": ",Symbol_string);
            strcpy(Symbol_string, keycopy(buffer, Symbol_string, 19));
         } else {
            cprintf("Default symbol %d: ", Datasymbol);
            c=getkey(0);
            if (c==ESC) return;
            if (c!=RETURN && c>='0' && c<'0'+SYMBOLS) Datasymbol=c-'0';
            if (c=='L') Literal=TRUE;
         }
         if (Datasymbol==9) {
            Symbol_size=16;
            Intra=20;
         }
         if (Color>1) {
            clrscr();
            cprintf("\r\n\r\nColors available: \r\n"
               "0 = White\r\n"
               "1 = Dark blue\r\n"
               "2 = Green\r\n"
               "3 = Cyan\r\n"
               "4 = Red\r\n"
               "5 = Magenta\r\n"
               "6 = Yellow\r\n"
               "7 = Light grey\r\n"
               "8 = Dark grey\r\n"
               "9 = Medium blue\r\n\r\n");
            cprintf("Positions in the color code sequence currently used: %s\r\n",Symbol_color);
            cprintf("Type color code sequence or press RETURN: ");
            strcpy(Symbol_color, keycopy(buffer, Symbol_color, SYMBOLS));
        }
      }
      clrscr();
      cprintf("Symbol size (0-400, default %s\)\r\n\"?\"=AUTO\r\n: ",(Symbol_size>-1)?itoa(Symbol_size,"          ", 10):"AUTO");
      gets(buffer);
      if (*buffer) {
         if (*buffer=='?') n=-1;
         else n=atoi(buffer);
         if (n>=-1 && n<=400) Symbol_size=n;
      }
      cprintf("\r\nHorizontal inter-symbol distance (0 - 200, default %s\)\r\n\"?\"=AUTO\r\n: ", (Intra!=-1)?itoa(Intra,"          ", 10):"AUTO");
      gets(buffer);
      if (*buffer) {
         if (*buffer=='?') n=-1;
         else n=atoi(buffer);
         if (n>=0 && n<=200) Intra=n;
      }
#ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d:Symbol_string=\"%s\", Symbol_size=%d Intra=%d", __FILE__, __LINE__, Symbol_string, Symbol_size, Intra);
#endif
   }
   if (symbols>0 && !Literal && (Datasymbol==-1 || (Subselect>1 && !No_ranges))) {
      for (;;) {
         clrscr();
         cprintf("Legends for codes: \r\n");
         for (selected_symbol=0;selected_symbol<SYMBOLS;selected_symbol++) {
            if (symbol_in_use[selected_symbol]) cprintf("\r\nLegend for code %d: \"%s\"",selected_symbol,symbol_legend[selected_symbol]);
         }
         cprintf("\r\n\r\nRETURN = accept\r\nModify which legend: ");
         do {
            response=getkey(0);
            if (response==ESC || response==CANCEL) return;
            selected_symbol=response-'0';
         } while (response!=RETURN && (!symbol_in_use[selected_symbol] || selected_symbol<0 || selected_symbol>=SYMBOLS));
         if (response==RETURN) break;
         strinput(symbol_legend[selected_symbol]);
         if (Subselect>1) {
            for (t=0, n=0; test0[t]!=-99 && t<nooftests; t++) if (test0[t]!=-99) n++;
            for (t=0, symbolpos=0; t<selected_symbol && t<SYMBOLS  ; t++) if (symbol_in_use[t]) symbolpos++;
            for (t=0; t<n;t++) {
               column=(Subselect==3)?t*symbols+symbolpos:t+symbolpos*n;
               if (column<MAX_COL) strcpy(evaltext[column],symbol_legend[selected_symbol]);
            }
         }
      }
   }
   if (Datasymbol==-1 && !Literal) {
      clrscr();
      cprintf("Legend at the indicated position (4=no legend)\r\ndefault:%d:\r\n\r\n"
                  "%c%c%c%c%c%c%c\r\n"
                  "%c3   2%c1\r\n"
                  "%c     %c\r\n"
                  "%c     %c\r\n"
                  "%c%c%c%c%c%c%c\r\n"
                  "0\r\n\r\n",Legendpos,218,196,196,196,196,196,191,
                                    179,179,179,179,179,179,
                                    192,196,196,196,196,196,217);
      response=getkey(0);
      if (response==ESC) return;
      if (response>='0' && response<'4') Legendpos=response-'0';
      if (response=='0' && Legend==0) Legend=1;
      if (response=='4' || !symbols) {
         Legendpos=0;
         Legend=0;
      }
      if (Legendpos==0 && Legend>0) {
         cprintf("\r\n\r\nLegends printed over %d column(s) ",Legend);
         r=getkey(0);
         if (r>='0' && r<'9') Legend=r-'0';
      }
   }
}

void ycal_type_menu(void) {
   int c;
   clrscr();
   cprintf("Type of Y-axis plot:\r\n\r\n"
          "0 - 3 : no top- or bottomline\r\n\r\n"
          "0 - two-sided, tick marks inside\r\n"
          "1 - two-sided, tick marks outside\r\n"
          "2 - left-side only, tick marks inside\r\n"
          "3 - left-side only, tick marks outside\r\n\r\n"
          "4 - 7 : bottomline, no top-line\r\n\r\n"
          "4 - two-sided, tick marks inside\r\n"
          "5 - two-sided, tick marks outside\r\n"
          "6 - left-side only, tick marks inside\r\n"
          "7 - left-side only, tick marks outside\r\n\r\n"
          "8 - B : top- and bottomlines\r\n\r\n"
          "8 - two-sided, tick marks inside\r\n"
          "9 - two-sided, tick marks outside\r\n"
          "A - left-side only, tick marks inside\r\n"
          "B - left-side only, tick marks outside\r\n"
          "\r\ndefault: %d :",Ycal);
  do {
     c=getkey(0);
     if (c==RETURN) return;
  } while (result &&  (c<'0' || c>'9') && c!='A' && c!='B');
  if (!result) return;
  if (c<='9') Ycal=c-'0';
  if (c=='A') Ycal=10;
  if (c=='B') Ycal=11;
}

int stat_menu(void) {
int c, stat_status=1, i, statmode;
    calc_flag=FALSE;
    clrscr();
    new_stat=TRUE;
    if (! (Mean_flag || Iqr_flag))       statmode=0;
    else if (Mean_flag && Sd_flag)       statmode=1;
    else if (Mean_flag && Sem_flag)      statmode=2;
    else if (Mean_flag)                  statmode=3;
    else if (Median_flag==1 && Iqr_flag) statmode=4;
    else if (Median_flag==1)             statmode=5;
    else if (Median_flag==2)             statmode=6;
    if (Sd_flag==2 || Sem_flag==2 || Iqr_flag==2) stat_status=2;
    if (Sd_flag==3 || Sem_flag==3 || Iqr_flag==3) stat_status=3;
    if (Graph_type!=OGIVE) {
       if (Graph_type != BAR_GRAPH) cprintf("\r\n0= No statistics");
       cprintf("\r\n1= Mean +/- SD\r\n2= Mean +/- SEM\r\n3= Mean (no range)\r\n");
       if (Centile) cprintf("4= %d-centile +/- SE\r\n",(int)(Centile*100.01));
       else cprintf("4= Median /IQR\r\n");
       if (Centile) cprintf("5= %d-centile (no range)\r\n",(int)(Centile*100.01));
       else cprintf("5= Median (no range)\r\n");
       if (Graph_type == BAR_GRAPH || Graph_type==TTEST) cprintf("6= Box-plot\r\n");
       cprintf("Default: %d", statmode);
       do {
          c=getkey(0);
       } while (result &&  (c<'0' || c>'6') && c!=RETURN);
       if (!result) return 0;
       switch (c) {
          case CTRL_C:
             farewell(pic_no);
             break;
          case ESC:
          case CANCEL:
             clrscr();
             return (0);
          case '0':
/**/
             Mean_flag=0;
             Sd_flag=0;
             Sem_flag=0;
             Iqr_flag=0;
             Median_flag=0;
/**/
             Stat=-1;
             break;
          case '1':
             if (!Mean_flag) Mean_flag=1;
             Sd_flag=stat_status;
             Sem_flag=0;
             Iqr_flag=0;
             Median_flag=0;
             Stat=STAT;
             break;
          case '2':
             if (!Mean_flag) Mean_flag=1;
             Sd_flag=0;
             Sem_flag=stat_status;
             Iqr_flag=0;
             Median_flag=0;
             Stat=STAT;
             break;
          case '3':
             if (!Mean_flag) Mean_flag=1;
             Sd_flag=0;
             Sem_flag=0;
             Iqr_flag=0;
             Median_flag=0;
             Stat=STAT;
             break;
          case '4':
             Mean_flag=0;
             Sd_flag=0;
             Sem_flag=0;
             Iqr_flag=stat_status;
             Median_flag=1;
             Stat=STAT;
             break;
          case '5':
             Mean_flag=0;
             Sd_flag=0;
             Sem_flag=0;
             Iqr_flag=0;
             Median_flag=1;
             Stat=STAT;
             break;
          case '6':
             Mean_flag=0;
             Sd_flag=0;
             Sem_flag=0;
             Iqr_flag=stat_status;
             Median_flag=2;
             Stat=STAT;
             break;
       }
    }
    if (c!=RETURN && Stat>-1) {
       if (Graph_type==OGIVE) {
          sprintf(range_string,"%d",(int)(Centile?Centile*100.01:50));
          cprintf("\r\n\r\nCentile-option (1-99)\r\ndefault: %s ",range_string);
       } else if (Median_flag==1) {
             if (Iqr_flag) {
                cprintf("\r\n\r\nCentile-option:\r\nI = IQR (25-, 50-  and 75- centiles)\r\n"
                    "1-99: corresponding centile +/- SE\r\ndefault: ");
                if (Centile) cprintf("%d-centile ",(int)(Centile*100.01));
                else cprintf("IQR ");
             } else {
                cprintf("\r\n\r\nCentile-option: 1-99: corresponding centile\r\ndefault: ");
                if (Centile) cprintf("%d-centile ",(int)(Centile*100.01));
                else cprintf("50-centile = median");
             }
       }
       if (Median_flag==1 || Graph_type==OGIVE) {
          if (Centile) sprintf(range_string,"%d",(int)Centile*100.01);
          else strcpy(range_string,"IQR");
          keycopy(buffer, range_string,40);
          Centile=atof(buffer);
          if (Centile>1) Centile=(Centile+0.001)/100;
          if (Centile<0.01 || Centile>0.99) Centile=0;
          make_centiles(nooftests,Outliers_flag);
       }
    }
    if ((Graph_type==TTEST || Graph_type==BAR_GRAPH) && Median_flag!=2) {
       clrscr();
       cprintf("Probability value printed as text in figure (Y/N) default: %c ",Prob_flag?'Y':'N');
       c=getkey(0);
       if (c==SPACE) Prob_flag=(Prob_flag)?0:1;
       else if (c=='Y' || c=='J') Prob_flag=TRUE;
       else if (c=='N') Prob_flag=FALSE;
       cprintf("\r\n%s\r\n",Prob_flag?"YES":"NO");
       if (Prob_flag) {
          cprintf("\r\n\r\nLog transformation of data (Y/N) default: %c ", Logflag?'Y':'N');
          c=getkey(0);
          if (c=='Y' || c=='J') Logflag=TRUE;
          else if (c=='N') Logflag=FALSE;
          cprintf("\r\n%s\r\n",Logflag?"YES":"NO");
          for (i=0; i<4; i++) cprintf("\r\n%d = %s",i+1, statmode_string[i]);
          cprintf("\r\n\r\ndefault: %d: ", Mean_flag);
          c=getch()-'0';
          if (c>0 && c<5) Mean_flag=c;
          clrscr();
          write_prob_strings(FALSE);
       }
    }
    return 1;
}

int modify_text(void) {
   int n;
   for (;;) {
      clrscr();
      cprintf("F - First title: \"%s\"\r\n",First_title);
      cprintf("S - Second title: \"%s\"\r\n",Second_title);
      cprintf("Y - Y-axis title: \"%s\"\r\n",y_axis_title);
      cprintf("L - modify Legends\r\n"
             "    staggered test-legends: ");
      yesno(Stagger,"YES","NO");
      if (Subselect && !No_ranges)  cprintf("    second column legend %s plotted points\r\n", (Ranges_in_header)?"ABOVE":"BELOW");
      if (Prob_flag) cprintf("P - modify Probability text\r\n");
      cprintf("T - modify Text size\r\n");
      cprintf("E - modify/add Extra text\r\n");
      if (Datasymbol==-1) {
        if (Legendpos==0) cprintf("    symbol-legends printed in %d column(s)\r\n",Legend);
        if (Legendpos!=0) cprintf("    symbol-legends printed at position %d\r\n",Legendpos);
      }
      do option=getkey(0); while (option!=RETURN && option!=ESC && strpos("FSYLPTE",option,0)==-1);
      if (option==ESC) return 0;
      if (option==RETURN) return 1;
      switch (option) {
         case 'E':
            extra_text_menu();
            break;
         case 'F':
            clrscr();
            cprintf("First title \"%s\": ",First_title);
            if (keycopy(buffer, First_title, MAX_LEN)==0) return -1;
            strcpy(First_title,buffer);
            break;
         case 'L':
            legend_menu();
            break;
         case 'P':
            modify_prob_strings();
            break;
         case 'S':
            clrscr();
            cprintf("Second title \"%s\": ",Second_title);
            if (keycopy(buffer, Second_title, MAX_LEN)==0) return -1;
            strcpy(Second_title,buffer);
            break;
         case 'T':
              clrscr();
              cprintf("Text-size: (max=%d,default=%d)", 3*TEXTSIZE, Textsize);
/*
              buffer[0]=10;
*/
              gets(buffer);
/*
              n=atoi(buffer+2);
*/
              n=atoi(buffer);
              if (n>0 && n<=3*TEXTSIZE) {
                 Textsize=n;
                 Vsize=Textsize*9;
                 Hsize=Textsize*8;
              }
              break;
         case 'Y':
            clrscr();
            cprintf("Y-axis title\r\nDefault: \"%s\"\r\n\"?\"=AUTOMATIC\r\n: ",y_axis_title);
            if (keycopy(buffer, y_axis_title, MAX_LEN)==0) return 0;
            if (result!=RETURN) {
               User_legend_flag=(*buffer=='?')?FALSE:TRUE;
               if (User_legend_flag) strcpy(y_axis_title,buffer);
            }
            if (!User_legend_flag) {
               do {
                  clrscr();
                  cprintf("Include statistic-text in Y-axis title");
                  yesno(Stattext_flag,"YES","NO");
                  cprintf("\r\nRETURN to accept, other key to change ");
                  n=getkey(0);
                  if (n!=RETURN) Stattext_flag=Stattext_flag?0:1;
               } while (n!=RETURN);
            }
            write_stat_strings();
            break;
      }
   }
}

/* local routines */

void modify_prob_strings(void) {
   int c, i, d, ps;
   do {
      clrscr();
      cprintf("Current \"probability text\" setting:\r\n");
      for (i=ps=0; i<PSTRING; i++) if (pstart[i]!=-1 && pvalue[i]!=MISSING) {
         cprintf("%d testcolumns %d/%d (vert.pos:%.2f): %s\r\n",i,pstart[i],pend[i],pvalue[i],pstring[i]);
         ps=TRUE;
      }
      if (ps) cprintf("\r\n\r\nRETURN to accept\r\nChoose 0-%d to change or add text: ",PSTRING);
      else {
         clrscr();
         cprintf("Choose 0-%d to add \"probability\" text: ",PSTRING);
      }
      c=getch();
      if (c==RETURN || c==ESC) return;
      if (c>='0' && c<PSTRING+'0') {
         i=c-'0';
         cprintf("\r\nStart column (default %d, 'D' for Delete): ", pstart[i]);
         d=getkey(0);
         if (!result) return;
         if (d=='D') pstart[i]=-1;
         else if (d>='0' && d<=PSTRING+'0') pstart[i]=d-'0';
         if (pstart[i]>-1) {
            cprintf("\r\nSelected start column: %d\r\nEnd column (default %d, 'D' for Delete): ", pstart[i], pend[i]);
            d=getkey(0);
            if (!result) return;
            if (d=='D') pstart[i]=-1;
            else if (d>='0' && d<=PSTRING+'0') pend[i]=d-'0';
         } if (pstart[i]>-1) {
            cprintf("\r\nSelected end column: %d\r\nVertical position of probability text (default %.3f): ", pend[i],pvalue[i]);
            gets(buffer);
            if (*buffer) pvalue[i]=atof(buffer);
         }
         p_parameters(i, TRUE);
         if (pstring[i][0]==' ') pstart[i]=-1;
      }
   } while (c!=RETURN && ps);
}

void legend_menu() {
   int option;
   if (Subselect && (
              Graph_type==OGIVE ||
              Graph_type==DENDROGRAM ||
              Graph_type==XYCLUSTER)) {
      eval_menu();
      return;
   }
   if (!(
         (Datasymbol==-1) ||
         (Subselect && !No_ranges)) ||
        ( !Subselect && (
           Graph_type==OGIVE ||
           Graph_type==DENDROGRAM ||
           Graph_type==XYCLUSTER))) {
      column_menu();
      return;
   }
   clrscr();
   cprintf("T - Test-legends\r\n");
   cprintf("    staggered test-legends: ");
   yesno(Stagger,"YES","NO");
   if (Datasymbol==-1 || (Subselect>1 && !No_ranges)) {
      cprintf("C - Code-legends\r\n");
      if (Subselect>1) {
         cprintf("P - Position of second column-legend:\r\n"
                 "    %s rather than %s plotted data\r\n", (Ranges_in_header?"ABOVE":"BELOW"),(Ranges_in_header?"BELOW":"ABOVE"));
         if (!Ranges_in_header) cprintf(
              "    %s rather than %s test legends\r\n",(Swap?"ABOVE":"BELOW"), (Swap?"BELOW":"ABOVE"));
      }
      if (Datasymbol==-1) {
         if (Legendpos==0 && !Legend) cprintf("    symbol-legends printed in %d column(s)\r\n",Legend);
         if (Legendpos!=0)            cprintf("    symbol-legends printed at position %d\r\n",Legendpos);
      }
   }
   if (Subselect==1 && !No_ranges) {
      cprintf("S - Selection criteria-legends\r\n"
              "P - Position of second column-legend:\r\n"
              "    %s rather than %s plotted data\r\n", (Ranges_in_header?"ABOVE":"BELOW"),(Ranges_in_header?"BELOW":"ABOVE"));
      if (!Ranges_in_header) cprintf(
              "    %s rather than %s test legends\r\n",(Swap?"ABOVE":"BELOW"), (Swap?"BELOW":"ABOVE"));
   }
   cprintf("\r\n\r\nEscape or RETURN= previous menu ");
   option=getkey(0);
   if (option>256) return;
   switch (option) {
      case 'C':
         if (Datasymbol==-1 || (Subselect>1 && !No_ranges)) symbol_menu(FALSE);
         break;
      case 'P':
         do {
            clrscr();
            cprintf("Position of second column-legend:\r\n"
                "%s rather than %s plotted data", (Ranges_in_header?"ABOVE":"BELOW"),
                                                  (Ranges_in_header?"BELOW":"ABOVE"));
            cprintf("\r\n\r\nPress RETURN to accept\r\nAny other key to swap ");
            if ((option=getch())!=RETURN && option !=ESC) Ranges_in_header=(Ranges_in_header)?0:1;
         } while (option !=RETURN && option!=ESC);
         if (option==ESC) break;
         if (!Ranges_in_header) do {
            clrscr();
            cprintf("Range-names %s rather than %s test-legends\r\n",
                   (Swap?"ABOVE":"BELOW"),(Swap?"BELOW":"ABOVE"));
            cprintf("\r\n\r\nPress RETURN to accept\r\nAny other key to swap ");
            if ((option=getch())!=RETURN && option!=ESC) Swap=(Swap)?0:1;
         } while (option !=RETURN && option!=ESC);
      break;
      case 'S':
         if (Subselect==1 && !No_ranges) eval_menu();
         break;
      case 'T':
         column_menu();
         break;
   }
}

void column_menu() {
   int t, response;
   for (;;) {
      clrscr();
      show_text(textstring, completestring, 1, strlen(completestring), 18);
      cprintf("RETURN= accept\r\nModify which legend (%c-%c) : ",'B'-2*(Subselect>0), numbertoletter(columns+1));
      do {
         response=getkey(1);
         if (response==ESC) return;
         if (columns<25 || response-'a'>columns-24) response=toupper(response);
         if (response>255) return;
         if (response!=RETURN) t=lettertonumber(response);
      } while (response!=RETURN && (t<(Subselect?0:2) || t>=columns+2));
      if (response==RETURN) break;
      if (t==0) strinput(samplenumber_string);
      else if (t==1) strinput(samplecode_string);
      else strinput(testname[t-2]);
   }
   for (;;) {
      gotoxy(1,23);
      cprintf("\r\nStaggered test-legends: ");
      yesno(Stagger,"YES","NO");
      cprintf("RETURN to accept ");
      response=getkey(0);
      if (response==RETURN || response==ESC) break;
      Stagger=(Stagger)?0:1;
   }
}

void eval_menu() {
   int t, row, response, redo=FALSE;
   char *parsestring;
   int error, result_of_parser;
   parsestring=defaultstring[6];
   for (;;) {
      clrscr();
      cprintf("Selection criteria for evaluation:");
      show_text(evalstring, selectstring, 3, nooftests, 18);
      cprintf("RETURN= accept\r\nModify which criterion (%c-%c) : ",'@', (nooftests<27)?'@'+nooftests-1:'a'+nooftests-28);
      do {
         response=getkey(0);
         if (response>256) return;
         t=lettertonumber(response);
      } while (response!=RETURN && (t<0 || t>=nooftests));
      if (response==RETURN) break;
      redo=TRUE;
      do {
         cprintf("\r\ncode %d: testname:\"%s\"\r\ncriterion\"%s\"",t,testname[test[t]],evalstring[t]);
         keycopy(buffer, evalstring[t], MAX_LEN);
         if (!*buffer) break;
         strcpy(parsestring, buffer);
         toupper_evalstring(parsestring, columns);
         cprintf("evaluating %s ... ",parsestring);
         error=0;
         for (row=0; row<rows; row++) {
            result_of_parser=parse(parsestring,row);
            if (result_of_parser==-1 || error) {
               error++;
               cprintf("error in parse-string \"%s\": %s\r\nPress any key to continue", parsestring, errstring);
               if (getch()==ESC) break;
            }
         }
      } while (error);
      strcpy(evalstring[t], parsestring);
      expand_evalstring(t);
   }
   if (redo) make_subselection(FALSE);
   sort_flag=FALSE;
   calc_flag=FALSE;
   for (;;) {
      clrscr();
      cprintf("Selection text:");
      show_text(evaltext, selectstring, 3, nooftests, 18);
      cprintf("RETURN= accept\r\nModify which text (%c-%c) : ",'@', (nooftests<27)?'@'+nooftests-1:'a'+nooftests-28);
      do {
         response=getkey(0);
         if (response>256) return;
         t=lettertonumber(response);
      } while (response!=RETURN && (t<0 || t>=nooftests));
      if (response==RETURN) break;
      cprintf("\r\ncode %d: testname:\"%s\ criterion\"%s\"\r\ndefault text:\"%s\" : ",t,testname[test[t]],evalstring[t],evaltext[t]);
      keycopy(buffer, evaltext[t], MAX_LEN);
      strcpy(evaltext[t],buffer);
   }
}

void extra_text_menu() {
   int t, response;
   for (;;) {
      CLS;
      cprintf("\Extra text at the indicated positions:\r\n\r\n"
               "0         1\r\n"
               "  2  3  4\r\n\r\n"
               "  5  6  7\r\n"
               "8         9\r\n\r\n");
      for (t=0;t<10;t++) if (extra_text[t][0]) cprintf("\r\nextra text %d: %s",t,extra_text[t]);
      cprintf("\r\n\r\nRETURN= accept\r\nModify/add which text (0 - 9) : ");
      do {
         response=getkey(0);
         t=response-'0';
      } while (response!=RETURN && (t<0 || t>9));
      if (response==RETURN) break;
      strinput(extra_text[t]);
      if (isvoid(extra_text[t])) extra_text[t][0]='\0';
   }
#ifdef TRACE
   if (Trace==2) {
      fprintf(tracefile,"%s %4d extra text:\r\n",__FILE__, __LINE__);
      for (t=0;t<10;t++) fprintf(tracefile,"\n%d \"%s\"",t,extra_text[t]);
   }
#endif
}

void fill_menu(void) {
   int i, t, option;
   char default_value[16];
   do {
      clrscr();
      HLON3;cprintf("F - Fill block option:\r\n\r\n");
      for (t=0; t<FILL_COL && t<nooftests; t++) {
         if (test[t]!=-1) {
            HLON3;cprintf("%d: %24s: Fill ",t+1, testname[test[t]]);HLOFF;
            yesno(filltype[t]>-1,"ON","OFF");
         }
         if (t==20 && nooftests>21) {
            MORE;
         }
      }
      HLON3;cprintf(  "\r\n%c: %24s: Fill ",'G', "Global block fill");HLOFF;
      yesno(filltype[FILL_COL]>-1,"ON","OFF");
      cprintf("\r\n\r\nSelect 1-%d (or option 'G') to toggle Fill On/Off\r\n"
              "\r\nor press RETURN to accept Fill block status\r\n", nooftests);
      i=getkey(0);
      if (i==ESC) return;
      t=i-'1';
      if (t>=0 && t<=FILL_COL && t<nooftests && test[t]!=-1) filltype[t]=(filltype[t]==-1)?FILLTYPE:-1;
      if (i=='G') filltype[FILL_COL]=(filltype[FILL_COL]==-1)?FILLTYPE:-1;
   } while (i!=RETURN);
   if (!fillmode()) return;
   for (i=0; i<=FILL_COL; i++) {
      if (i<FILL_COL && (test[i]==-1 || i>=nooftests)) continue;
      clrscr();
      if (filltype[i]!=-1) {
         if (i!=FILL_COL) cprintf("Test %s (range: %.3f-%.3f) fill-type:\r\n",testname[test[i]], min[test[i]], max[test[i]]);
         else cprintf("Global fill-type:\r\n");
         cprintf("0     = unfilled (top- and bottomlines)\r\n"
                 "1 - 5 = fill density (1=low, 5=high)\r\n\r\n"
                 "Default=%d: ", filltype[i]);
         option=getkey(0);
         if (option==ESC) return;
         if (option>='0' && option<='5') filltype[i]=option-'0';
         cprintf("\r\n\r\nLower level=%s ",(fill_min[i]==MISSING)?"AUTO":ftoa(fill_min[i]));
         cprintf("Upper level=%s\r\n\r\n",(fill_max[i]==MISSING)?"AUTO":ftoa(fill_max[i]));
         cprintf("'?'= AUTOMATIC\r\n\r\n");
         ftoa(fill_min[i]); strcpy(default_value,buffer);
         cprintf("\r\nLow value y-axis (%s) ",(fill_min[i]==MISSING)?"AUTO":ftoa(fill_min[i]));
         if (keycopy(buffer, default_value,16)==0) return;
         if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') fill_min[i]=MISSING;
         else if (*buffer) fill_min[i]=atof(buffer);
         ftoa(fill_max[i]); strcpy(default_value,buffer);
         cprintf("\r\nHigh value y-axis (%s) ",(fill_max[i]==MISSING)?"AUTO":ftoa(fill_max[i]));
         if (keycopy(buffer, default_value,16)==0) return;
         if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') fill_max[i]=MISSING;
         else if (*buffer) fill_max[i]=atof(buffer);
         if (fill_max[i]!=MISSING && fill_max[i]<fill_min[i]) {
            fill_max[i]=MISSING;
            fill_min[i]=MISSING;
         }
      }
   }
}

void minmax_menu(void) {
   clrscr();
   cprintf("M - MinMax: ");
   cprintf("Ymin=%s ",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
   cprintf("Ymax=%s\r\n\r\n",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
   cprintf("'?'= AUTOMATIC\r\n\r\n");
   cprintf("\r\nYmin=lowest value y-axis (%s) ",(Ymin==MISSING)?"AUTO":ftoa(Ymin));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Ymin=MISSING;
   else if (*buffer) Ymin=atof(buffer);
   cprintf("\r\nYmax=highest value y-axis (%s) ",(Ymax==MISSING)?"AUTO":ftoa(Ymax));
   gets(buffer);
   if (*buffer=='?' || *buffer=='/' || *buffer=='A' || *buffer=='a') Ymax=MISSING;
   else if (*buffer) Ymax=atof(buffer);
/*
   cprintf("\r\n\"Excluded\" values included in statistic calculations (Y/N): RETURN=%s",Outliers_flag?"YES":"NO");
   if (getch()!=RETURN ) Outliers_flag=!Outliers_flag;
*/
   Outliers_flag=TRUE;

}

void read_environment() {
   char *s;
   startup_drive=getdisk();
   s=getenv("TARGET");
   if (!s) s=getenv("TARGET_DRIVE");
   if (!s) s=getenv("OUTPUT");
   if (!s) s=getenv("OUT");
   if (s[0]>='A' && s[0]<'Z')  {
      Target_drive=s[0]-'A';
      ttpic[0]= ttlog[0]= s[0];
   } else Target_drive=TARGET_DRIVE;
   Small= set_env_variable("SMALL", 0);
   Trace=set_env_variable("TRACE", 0);
   s=getenv("INTER_T");
   if (!s) Inter_t=-1;
   else    Inter_t=atof(s);
   Combi=set_env_variable("COMBI", 0);
/*
   s=getenv("COLOR");
   if (!s) s=getenv("COLOUR");
   if (s) Color=atoi(s);
   else Color=COLOR;
   if (Color<1) Color=1;
   if (Color>COLOR) Color=COLOR;
   s=getenv("FIRST");
   if (!s) s=getenv("FIRSTTITLE");
   if (s) strcpy(First_title,s);
   else *First_title='\0';
   s=getenv("SECOND");
   if (!s) s=getenv("SECONDTITLE");
   if (s) strcpy(Second_title,s);
   else *Second_title='\0';
   s=getenv("Y_AXIS");
   if (!s) s=getenv("YAXIS");
   if (!s) s=getenv("Y_AXISTITLE");
   if (!s) s=getenv("Y_AXIS_TITLE");
   if (s) {
      strcpy(Y_axis_title,s);
      User_legend_flag=TRUE;
   }
   else {
      *Y_axis_title='\0';
      User_legend_flag=FALSE;
   }
   s=getenv("SYMBOLSTRING");
   if (s) strncpy(Symbol_string,s,19);
   else strcpy(Symbol_string,SYMBOLSTRING);
   s=getenv("TEXT0");
   if (s) strcpy(extra_text[0],s);
   s=getenv("TEXT1");
   if (s) strcpy(extra_text[1],s);
   s=getenv("TEXT2");
   if (s) strcpy(extra_text[2],s);
   s=getenv("TEXT3");
   if (s) strcpy(extra_text[3],s);
   s=getenv("TEXT4");
   if (s) strcpy(extra_text[4],s);
   s=getenv("TEXT5");
   if (s) strcpy(extra_text[5],s);
   s=getenv("TEXT6");
   if (s) strcpy(extra_text[6],s);
   s=getenv("TEXT7");
   if (s) strcpy(extra_text[7],s);
   s=getenv("TEXT8");
   if (s) strcpy(extra_text[8],s);
   s=getenv("TEXT9");
   if (s) strcpy(extra_text[9],s);
   s=getenv("SIZE");
   if (!s) s=getenv("SYMBOL_SIZE");
   if (!s) Symbol_size=-1;
   else    Symbol_size=atoi(s);
   s=getenv("INTRA");
   if (!s) Intra=-1;
   else    Intra=atof(s);
   s=getenv("INTER_BAR");
   if (!s) Inter_bar=-1;
   else    Inter_bar=atof(s);
   Nn=(int)set_env_variable("NN",        NN);
   s=getenv("PLOTTER");
   if (s) Plotter=atoi(s);
   else Plotter=PLOTTER;
   Textsize=(int)set_env_variable("TEXTSIZE",TEXTSIZE);
   Centile= (double)set_env_variable("CENTILE",   CENTILE);
   if (Centile>1) (Centile+0.001)/=100;
   if (Centile<0.05 || Centile>0.95) Centile=0;
   Stagger=set_env_variable("STAGGER",      0);
   Show=set_env_variable("SHOW",       1);
   Swap=set_env_variable("SWAP",       SWAP);
   s=getenv("LOG");
   if (!s) s=getenv("YLOG");
   if (s) Logflag=atoi(s);
   else Logflag=0;
   s=getenv("GRAPH_TYPE");
   if (!s) s=getenv("TYPE");
   if (s) Graph_type=atoi(s);
   else Graph_type=GRAPH_TYPE;
   Ycal=set_env_variable("YCAL",       YCAL);
   Line=set_env_variable("LINE",       0);
   Datasymbol=set_env_variable("DATASYMBOL", 1);
   Zero=set_env_variable("ZERO",1);
   Sd_flag=set_env_variable("SD",         1);
   Sem_flag=set_env_variable("SEM",        0);
   Iqr_flag=set_env_variable("IQR",        1);
   Median_flag  =set_env_variable("MEDIAN",     0);
   Mean_flag    =set_env_variable("MEAN",       1);
   Stat=set_env_variable("STAT",       STAT);
   Skipnegatives=set_env_variable("SKIPNEGATIVES", 0);
   Legend=set_env_variable("LEGEND", LEGEND);
   Legendpos=set_env_variable("LEGENDPOS", LEGENDPOS);
   Group=set_env_variable("GROUP", GROUP);
   Cap=set_env_variable("CAP", CAP);
   Tip=set_env_variable("TIP", TIP);
   Bar=set_env_variable("BAR", 1);
   All=set_env_variable("ALL", 0);
   Ymax= set_env_variable("YMAX", MISSING);
   Ymin= set_env_variable("YMIN", MISSING);
   if (Ymin>-MISSING || Ymin<MISSING) Ymin=MISSING;
   if (Ymax>-MISSING || Ymax<MISSING) Ymax=MISSING;
   X_length=set_env_variable("X_LENGTH", X_LENGTH);
   if (X_length>X_LENGTH || X_length<=0) X_length=X_LENGTH;
   if (Sem_flag && Sd_flag) Sd_flag=0;
   if (Mean_flag && Median_flag) Mean_flag=0;
   Noofclasses  =(int)(set_env_variable("CLASSES",DEFAULTNOOFCLASSES));
   if (Noofclasses<2 || Noofclasses>MAXNOOFCLASSES) Noofclasses=DEFAULTNOOFCLASSES;
   if (Group>MAX_COL) Group/=1000;
   if (Group<1) Group=10;
   if (Symbol_size>=1000) Symbol_size/=10;
   if (Plotter>10 || Plotter <1) Plotter=PLOTTER;
   if (Intra< 0) Intra=-1;
   if (Intra>4) Intra/=100;
   if (Intra>4) Intra=-1;
   if (Inter_t<0) Inter_t=INTER_T;
   if (Inter_t>10) Inter_t/=100;
   if (Inter_t>10) Inter_t=INTER_T;
   if (Inter_bar< -100) Inter_bar=INTER_BAR;
   if (Inter_bar>10 || Inter_bar < -10) Inter_bar/=100;
   if (Inter_bar>10) Inter_bar=INTER_BAR;
   if (Cap<0) Cap=CAP;
   if (Cap>2) Cap/=100;
   if (Cap>2) Cap=CAP;
   if (Tip<0) Tip=TIP;
   if (Tip>4) Tip/=100;
   if (Tip>4) Tip=TIP;
   if (Stat<0) Stat=STAT;
   if (Stat>2) Stat/=100;
   if (Stat>2) Stat=STAT;
*/
}

int save_menu(void) {
   int i;
   clrscr();
   cprintf("RETURN = save both .PIC- and .PRN-file\r\n"
          "SPACE  = save only the .PIC-file\r\n"
          "S      = save only the .PRN file (Statistical results)\r\n"
          "Esc    = do not save\r\n"
          "Q      = quit program\r\n");
   i=getkey(0);
   if (!i) return 0;
   switch (i) {
      case RETURN:
         return 2;
      case SPACE:
         return 1;
      case 'S':
         return 3;
      case ESC:
         return 0;
      case 'Q':
         farewell(pic_no);
         return 0;
  }
  return -1;
}

float set_env_variable(char *env_string, float default_value) {
   char *s;
   s=getenv(env_string);
   if (s==0)			return default_value;
   if (!stricmp(s,"AUTO"))	return MISSING;
   if (!stricmp(s,"OFF"))	return 0;
   if (!stricmp(s,"FALSE"))	return 0;
   if (!stricmp(s,"ON"))	return 1;
   if (!stricmp(s,"TRUE"))	return 1;
   return atof(s);
}

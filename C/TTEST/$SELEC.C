/* CCCCCCCCCCCCCCCCCCCCCCC test-selection  CCCCCCCCCCCCCCCCCCCCCCCCCCCC */
#include "ttest.h"
extern void frame(int hor,int vert);
/* module $selec.c */
int   getselectionmode(void);
int   make_subselection(int flag);
int   select_graph(int default_type);
int   select_tests(void);
void  select_data(void);
int   select_outcome_test(void);

/* local routines */
/*
int strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines);
*/
int getselectionmode(void) {
    int c, i, symbol;
    clrscr();
    symbols=find_symbols_in_use(-2);
    cprintf("RETURN  = use all valid data per selected column\r\n"
           "SPACEBAR= make sub-selection within selected columns\r\n");
    if (symbols) cprintf("A= automatic sub-selection on the basis of sample-code (column A)\r\n");
    c=getkey(0);
    if (!result) return -1;
    if (c==SPACE)  {
       Line=0;                                 
       return 1;
    }
    if (c=='A' && symbols) {
       if (symbols==1) return 2;
       cprintf("\r\n\r\nExample for 3 tests (B, C and D) and 2 codes (0 and 1)\r\n");
       cprintf("                     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\r\n"
               "P - Paired tests,    ³  B0  C0  D0   ³   B1  C1  D1 ³\r\n"
               "                     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\r\n\r\n"
               "                     ÚÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄ¿\r\n"
               "N - Non-paired tests ³  B0  B1  ³ C0  C1 ³  D0  D1  ³\r\n"
               "                     ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÙ\r\n\r\n"
               "default: Paired tests\r\n");
       while ((c=getkey(0))!=RETURN && c!='P' && c!='N');
       if (!result) return -1;
       if (c=='N') {
          Subselect=NON_PAIRED;
          Line=0;
       } else Subselect=PAIRED;
       cprintf("Selected: %s\r\n\r\n",(Subselect==PAIRED?"P = PAIRED":"N = NON-PAIRED"));
       do {
          cprintf("\r\nWhich codes? Default:%s :", used_symbolstring);
          keycopy( buffer, used_symbolstring, SYMBOLS+1);
          for (i=0;i<SYMBOLS;i++) selected_symbolstring[i]='0';
          for (i=0, symbols=0; i<strlen(buffer) && symbols<SYMBOLS; i++) {
             symbol=buffer[i]-'0';
             if (symbol>=0 && symbol< SYMBOLS) {
               if (symbol_in_use[symbol]) {
                 selected_symbolstring[symbol]='1';
                 symbols++;
               }
             } else {
                cprintf("\r\nNo rows with code %c in \"%s\"\r\nPress any key to re-try", buffer[i], symbolstring);
                c=getkey(0);
                if (!result) return -1;
                symbols=0;
                break;
             }
         }
       } while (!symbols);
       #ifdef TRACE
          if (Trace==2) {
            fprintf(tracefile,"\n%s %4d: used_symbolstring:\"%s\" selected_symbolstring=\"%s\"",__FILE__, __LINE__, used_symbolstring, selected_symbolstring);
            for (i=0; i<SYMBOLS; i++) fprintf(tracefile,"\nsymbol_in_use[%d]=%d Symbol_color[%d]=%d",
               i, symbol_in_use[i], i, Symbol_color[i]);
          }
       #endif
       return Subselect;
    }
    if (c==CANCEL || c==ESC || c==CTRL_C) return -1;
    return 0;
}

int make_subselection(int flag) {
   char *parsestring, *resultstring;
   char shorttestname[16];
   char symbol;
   char temptest[MAX_COL+1];
   char tempsymbol[MAX_COL];
   int c, i, t, r, n, row, column, symbolpos;
   int retry, error, result_of_parser, found, defaultno;
   #ifdef TRACE
      float d;
   #endif
   for (t=0;t<columns;t++) temptest[t]=numbertoletter(t+2);
   temptest[columns]='\0';
/*
   if (symbols<=0) symbols= find_symbols_in_use(0);
*/
   symbols= find_symbols_in_use(-2);
   if (Subselect && flag) {
      if (Subselect>1 && !symbols) return 0;
/*
      for (r=0;r<6;r++) strcpy(defaultstring[r],"@>0");
*/
      parsestring=defaultstring[6];
      resultstring=defaultstring[7];
      if (Subselect>1) strcpy(teststring,temptest);
   }

/* select tests: */
   if (flag) do {
      do {
         retry=FALSE;
         do {
            clrscr();
            show_text(textstring, temptest, 1, strlen(temptest), 12);
            cprintf("Select tests as single-letter code\r\n"
                   "select SPACEBAR for empty test\r\n\r\n"
                   "default test-selection:%s\r\n",teststring);
            if (Subselect>1 && symbols>1) cprintf("%d codes found in column A; maximum number of tests=%d\r\n",
               symbols, MAX_COL/symbols);
            if (keycopy( buffer, teststring, MAX_COL)==0) return 0;
            if ((c=not_substring(temptest, buffer))>0) {
               cprintf("\r\nUnknown test %c\r\nPress any key to re-try ",c);
               if (getch()==ESC) return 0;
            }
         } while (c);
         strcpy(teststring, buffer);
         if (!*teststring) return 0;
         nooftests=strlen(teststring);
         if (Subselect>1 && nooftests*symbols >MAX_COL) {
            retry=TRUE;
            cprintf("too many tests (%d) selected:\r\n"
                   "%d codes found in column A; maximum number of tests=%d\r\n"
                   "Press any key to re-try ",
               nooftests, symbols, MAX_COL/symbols);
            getkey(0);
            if (!result) return 0;
         }
      } while (!check_teststring(teststring, columns) || retry);
      #ifdef TRACE
         if (Trace==2) fprintf(tracefile,"\n%s %4d teststring:%s",__FILE__, __LINE__,teststring);
      #endif
      if (!nooftests) break;
      for (t=0; t<strlen(teststring); t++) test0[t]=test[t]=(teststring[t]==SPACE)?-1:lettertonumber(teststring[t])-2;
      if (nooftests<=MAX_COL) test0[nooftests]=-99;
      clrscr();
      cprintf("\r\nselection made: \r\n");
      for (t=0; t<nooftests; t++) {
         if (test0[t]==-1) continue;
         column=test0[t];
         cprintf("series %2d column %c: %s\r\n", t+1, numbertoletter(column+2),testname[column]);
         if (t%16==15 && nooftests-t>4) {
            MORE;
         }
      }
      cprintf("\r\nRETURN= accept\r\n");
      i=getkey(0);
      if (!result) return 0;
   } while (i!=RETURN);
   else {
      for (t=0; t<nooftests; t++) {
         test[t]=test0[t];
         teststring[t]=test0[t]==-1?SPACE:numbertoletter(test0[t]+2);
      }
      if (nooftests<=MAX_COL) {
         test0[nooftests]=-99;
         teststring[nooftests]='\0';
      }
   }

/* Phase 2 */
/* select data (= rows) from selected tests: */
/*
   symbols=-1;
*/
   clear_array1();
   if (nooftests<1) {
      cprintf("\r\n%s %4d : nooftests=%d teststring=\"%s\"",__FILE__, __LINE__ ,nooftests, teststring);
      if (getch()==ESC) farewell(-1);                                              
      putch('\r');
      putch('\n');
   }
   for (t=0;t<nooftests;t++)  valid_data[t]=0;
   switch (Subselect) {
      case 0:
         select_data();
         result=TRUE;
         break;
      case 1:
         for (t=0;t<nooftests;t++) test[t]=test0[t];
         if (flag) {
            clrscr();
            show_text(textstring, completestring, 1, strlen(completestring), 12);
            cprintf("Input your selection criteria using @, A, B-Z,= <> & | !\r\n"
                   "      EXAMPLES:              interpretation:\r\n"
                   "              -  @<10= cases 1-9\r\n"
                   "              -  A=2= cases where test-code equals 2\r\n"
                   "              -  C!=2= cases where test-score C is unequal to 2\r\n"
                   "              -  D>=2 & E!=0= cases where test-score D>=2 AND E is NOT 0\r\n"
                   "              -  F<=2 | G=0= cases where test-score F<=2 OR  G=0\r\n");
            for (t=0, defaultno=5; t<nooftests; t++) {
               if (test0[t]==-1) {
                  *evalstring[t]=*evaltext[t]='\0';
                  continue;
               }
               column=test0[t];
               do {
                  for (r=0;r<t && r<4; r++) {
                     gotoxy(1,21-r);
                     clreol();
                     gotoxy(1,21-r);
                     if (test0[t-r-1]>-1) cprintf("%2d (%c): %-16s %s: %3d cases found\r\n",
                        t-r, numbertoletter(test0[t-r-1]+2),strleft(shorttestname,testname[test0[t-r-1]], 16), evalstring[t-r-1], valid_data[t-r-1]);
                     else cprintf(               "%2d    : empty test\r\n",t-r);
/*
                     if (test0[t-r-1]>-1) cprintf("%2d (%c): %-16s %s\r\n",
                        t-r, numbertoletter(test0[t-r-1]+2),strleft(shorttestname,testname[test0[t-r-1]], 16), evalstring[t-r-1]);
                     else cprintf(               "%2d    : empty test\r\n",t-r);
*/
                  }
                  do {
                     for (r=23; r<26; r++) {
                        gotoxy(1,r);
                        clreol();
                     }
                     gotoxy(1,23);
                     cprintf("series %2d column %c: %s\r\ndefault: %s\r\n"
                            "       : ",
                        t+1, numbertoletter(column+2),testname[column],defaultstring[defaultno]);
                     error=0;
                     result=0;
                     strcpy(resultstring,keycopy(buffer, defaultstring[defaultno], MAX_LEN));
                     if (! *resultstring) {
                        if (result==UP) defaultno=(defaultno>0)?defaultno-1:5;
                        else if (result==DOWN) defaultno=(defaultno<5)?defaultno+1:0;
                     }
                  } while (! (*resultstring || result==ESC));
                  strcpy(parsestring, resultstring);
                  toupper_evalstring(parsestring, columns);
                  gotoxy(1,25);clreol();
                  cprintf("evaluating %s ... ",parsestring);
                  error=0;
                  for (row=0; row<rows; row++) {
/*
                     result_of_parser=parse(evalstring[t],row);
*/
                     result_of_parser=parse(parsestring,row);
                     if (result_of_parser==-1 || error) {
                        cprintf("error in parse-string \"%s\": %s\r\nPress any key to continue", parsestring, errstring);
                        if (getch()==ESC) return 0;
                        break;
                     }
                     casnum[row][t]=(result_of_parser)?row:rows;
                  }
                  if (result_of_parser!=-1) {
                     for (row=0, valid_data[t]=0; row<rows; row++) if (getdata(row, t, FALSE) > MISSING) valid_data[t]++;
/*
                     result_of_parser=parse(parsestring,0);
                     if (result_of_parser==-1 || error) {
                        for (r=23; r<26; r++) {
                           gotoxy(1,r);
                           clreol();
                        }
                        gotoxy(1,23);
                        cprintf("error in parse-string \"%s\": %s\r\nPress RETURN to re-try", parsestring, errstring);
                        i=getkey(0);
                        if (!result) return 0;
                     }
*/
                  }
               } while (result_of_parser==-1);
               for (r=0,found=FALSE;r<6;r++) {
                  if (!strncmp(parsestring,defaultstring[r],80)) {
                     found=TRUE;
                     break;
                  }
               }
               if (!found) {
                  for (r=1;r<6;r++) strcpy(defaultstring[r-1],defaultstring[r]);
                  strcpy(defaultstring[5],parsestring);
                  defaultno=5;
               }
               strcpy(evalstring[t], parsestring);
               expand_evalstring(t);
               #ifdef TRACE
                  if (Trace==2) fprintf(tracefile,"\n%s %4d parsestring \"%s\"",__FILE__, __LINE__, parsestring);
               #endif
            }
         } else {
            for (t=0; t<nooftests; t++) {
               if (test0[t]==-1) continue;
               error=0;
               for (row=0; row<rows; row++) {
                  result_of_parser=parse(evalstring[t],row);
                  if (result_of_parser==-1 || error) {
                     cprintf("error in parse-string \"%s\": %s\r\nPress any key to continue", parsestring, errstring);
                     getch();
                     return 0;
                  }
                  casnum[row][t]=(result_of_parser)?row:rows;
               }
               for (row=0, valid_data[t]=0; row<rows; row++) if (getdata(row, t, FALSE) > MISSING) valid_data[t]++;
            }
            #ifdef TRACE
               if (Trace==2) fprintf(tracefile,"\r\n%s %4d:t=%d valid_data[t]=%d",__FILE__, __LINE__,t,valid_data[t]);
            #endif
         }
         for (t=0, n=0;t<nooftests;t++) if (n<valid_data[t]) n=valid_data[t];
/*
         if (flag) {
            clrscr();
            cprintf("\r\nselection made: \r\n");
            for (t=0, n=0;t<nooftests;t++) {
               if (test0[t]==-1) continue;
               cprintf("%c: n=%3d %15s %s\r\n", numbertoletter(test0[t]+2), valid_data[t], testname[test0[t]], evalstring[t]);
               if (t%16==15 && nooftests-t>4) {
                  MORE;
               }
            }
            #ifdef TRACE
               if (Trace==2) {
                 fprintf(tracefile,"\n%s %4d\n",__FILE__, __LINE__);
                 for (row=0; row<rows; row++) {
                    fprintf(tracefile, "\nrow %3d: ",row);
                    for (t=0;t<nooftests;t++) {
                       if (test0[t]==-1) continue;
                       fprintf(tracefile,"(%3d,%2d) %13.4f", casnum[row][t], test0[t], getdata(row,t,FALSE));
                    }
                 }
              }
            #endif
         }
*/
         break;
      case 2:
      case 3:
         result=TRUE;
/*
         for (i=symbols=0;i<SYMBOLS;i++) if (!(selected_symbolstring[i]=='0') && symbol_in_use[i]) symbols++;
*/
         for (i=0;i<SYMBOLS;i++) {
            if (selected_symbolstring[i]=='0' || !symbol_in_use[i]) continue;
            symbol=symbolstring[i];
            if (symbol<'0' || symbol-'0'>SYMBOLS) continue;
            if (flag) {
               cprintf("\r\nLegend for symbol %c (default:%s):",symbol, symbol_legend[i]);
               keycopy(buffer, symbol_legend[symbol-'0'], MAX_LEN);
               strcpy(symbol_legend[symbol-'0'],buffer);
            }
            for (t=0, nooftests=0; test0[t]!=-99 && t<MAX_COL; t++) if (test0[t]!=-99) nooftests++;
            for (t=0, symbolpos=0; t<symbol-'0' && t<SYMBOLS  ; t++) if (symbol_in_use[t]) symbolpos++;
            for (t=0;t<nooftests;t++) {
               column=(Subselect==3)?t*symbols+symbolpos:t+symbolpos*nooftests;
               if (column<0 || column>=MAX_COL) {
                  cprintf("\r\n%s %4d : error: column %d out of range",__FILE__, __LINE__,column);
                  if (getch()==27) farewell(pic_no);
                  putch('\r');
                  putch('\n');
               }
               tempsymbol[column]=symbol;
               if (flag) {
                  strcpy(evaltext[column],symbol_legend[symbol-'0']);
                  sprintf(evalstring[column],"A=%i",symbolstring[i]);
               }
               teststring[column]= numbertoletter(test0[t]+2);
               test[column]=test0[t];
               #ifdef TRACE
                  if (Trace==2) fprintf(tracefile,"\n%4d:t=%d column=%d evaltext[%d]:\"%s\" evalstring[%d]:\"%s\" test[%d]=%d",
                        __LINE__, t, column, column, evaltext[column], column, evalstring[column], column, test[column]);
               #endif
            }
         }
         nooftests*=symbols;
         for (t=0;t<nooftests;t++) used_symbolstring[t]=tempsymbol[t];
         used_symbolstring[nooftests]='\0';
         teststring[nooftests]='\0';
         if (flag) cprintf("\r\nprocessing test ");
         for (t=0;t<nooftests;t++) {
            symbol=used_symbolstring[t];
            if (selected_symbolstring[symbol-'0']=='0') continue;
            if (flag) cprintf("%s(%c) ", numbertolotus(test[t]+1,"  "),symbol);
            for (row=0; row<rows; row++) casnum[row][t]= (rowlabel[row]==symbol)?row:rows;
            for (row=0,valid_data[t]=0; row<rows; row++) if (getdata(row, t, FALSE) > MISSING) valid_data[t]++;
         }
         break;
      }
   #ifdef TRACE
    if (Trace==2) {
      fprintf(tracefile,"\n%s %4d:casnum[][]-array",__FILE__, __LINE__);
      for (row=0;row<rows;row++) {
         fprintf(tracefile,"\nrow %4d ",row);
         for (t=0;t<nooftests;t++) {
            if (test0[t]==-1) continue;
            fprintf(tracefile,"%4d", casnum[row][t]);
         }
      }
   }
   #endif
   if (symbols<=0) symbols=find_symbols_in_use(-1);
/*
   if (Stat!=-1) {
      get_mean(0);
      if (nooftests==2) get_diff(0, 1);
   }
*/
   #ifdef TRACE
   if (Trace) {
      fprintf(tracefile,"\n%s %4d: sub-selected dataset\n",__FILE__,__LINE__);
      for (row=0;row<rows;row++) {
         for (t=0; t<nooftests;t++) {
            if (test0[t]==-1) continue;
            d=getdata( row, t, FALSE);
            if (d>MISSING) fprintf(tracefile, "%8.3f",d);
            else fprintf(tracefile, "%8s","...");
         }
         fprintf(tracefile,"\n");
      }
   }
   #endif
   process_data();
   return nooftests;
}

int select_graph(int default_type) {
   int n, graph_type, result, xpos, ypos;
   clrscr();
   xpos=1;
   ypos=13;
   gotoxy(xpos,ypos);
   frame(33,NOOFGRAPHS+2);
   xpos+=2;
   ypos++;
   for (n=0;n<NOOFGRAPHS;n++) {
      gotoxy(xpos,ypos+n);
      cprintf("%s\r\n",graph_name[n]);
      startchars[n]=graph_name[n][0];
   }
   startchars[NOOFGRAPHS]='\0';
   graph_type=TTEST;
   while ((result= strmenu1(
       graph_name[graph_type], &graph_type, NOOFGRAPHS, xpos, ypos + graph_type))==0);
   if (graph_type==DENDROGRAM || graph_type==XYCLUSTER) {
      Outcome_test=-1;
      clrscr();
      cprintf("Hierarchial clustering based on correlation coefficients:\r\n"
              "RETURN       : Spearman Rho\r\n"
              "Any other key: Pearson linear correlation coefficient r\r\n");
      if (getch()==RETURN) {
         Mean_flag=FALSE;
         Median_flag=TRUE;
      } else {
         Mean_flag=TRUE;
         Median_flag=FALSE;
      }
   } else {
      while (Stat==-1 && Graph_type==BAR_GRAPH) {
         if (!stat_menu()) {
             Mean_flag=1;
             Sd_flag=1;
             Sem_flag=0;
             Iqr_flag=0;
             Median_flag=0;
             Stat=STAT;
         }
      }
   }
   if (graph_type==PERCENTAGE_POSITIVE ||
       graph_type==PEARSON ||
       graph_type==SPEARMAN ||
       graph_type==RELATIVE_RISK ||
       graph_type==ODDS_RATIO)
          Sem_flag=0;
   if ( result== -1) return default_type;
   return graph_type;
}

void select_data(void) {
   int t, n, row;
   clear_array1();
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d %s: select_data: nooftests=%d Cutoff_flag=%d", __LINE__, __FILE__, nooftests, Cutoff_flag);
#endif
   for (t=0;t<nooftests;t++) {
/*
      for (row=0; row<rows; row++) casnum[row][t]= data0[row][test0[t]] > MISSING? row:-1;
      niet handig: row is dan niet meer te achterhalen bv. bij Spearman
      for (row=0, r=0; row<rows; row++, r++) casnum[r][t]= row;
*/
      if (test[t]>-1) {
         for (row=0; row<rows; row++) casnum[row][t]= row;
         for (row=0, valid_data[t]=0; row<rows; row++) if (getdata(row, t, FALSE) > MISSING) valid_data[t]++;
      } else {
         for (row=0; row<rows; row++) casnum[row][t]= rows;
         valid_data[t]=0;
      }
      N[t]=valid_data[t]; /* is dit nodig?? */

#ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d %s: t=%d valid_data[t]=%d", __LINE__, __FILE__, t,valid_data[t]);
#endif
   }
#ifdef TRACE
   if (Trace==2) {
      fprintf(tracefile,"\n%s %4d:casnum[][]-array",__FILE__, __LINE__);
      for (row=0;row<rows;row++) {
         fprintf(tracefile,"\nrow %4d ",row);
         for (t=0;t<nooftests;t++) fprintf(tracefile,"%3d", casnum[row][t]);
      }
   }
#endif
   for (t=0,n=0;t<nooftests;t++) if (n<valid_data[t]) n=valid_data[t];
   if (!n) {
      clrscr();
      cprintf("\r\n\r\nno valid data found\r\n");
      for (t=0,n=0;t<nooftests;t++) cprintf( "test[%d]=%d valid_data[%d]=%d\r\n",t, test[t], t, valid_data[t]);
      cprintf("\r\nPress Esc to quit\r\nPress any other key to continue ");
      if (getch()==27) farewell(-1);
      putch('\n');
      return;
   }
}

int select_outcome_test(void) {
   int t;
   char testcode[2];
   char tempstring[MAX_COL+1];
   clrscr();
   for (t=0;t<columns;t++) tempstring[t]=numbertoletter(t+2);
   tempstring[columns]='\0';
   show_text(textstring, tempstring, 1, strlen(tempstring), 12);
   if (Graph_type==DOTMATRIX) cprintf("Select ranking test as a single-letter code\r\n");
   else cprintf("Select outcome event as a single-letter code\r\n");
   if (Graph_type==RELATIVE_RISK || Graph_type==ODDS_RATIO)
         cprintf("\r\nSelect A for comparing cases with code 0 versus code 1\r\n"
                  "         as found in worksheet column A\r\n");
   else if (
         Graph_type==PEARSON ||
         Graph_type==SPEARMAN)
      cprintf("\r\nSelect A (=All) for correlation matrix file\r\n");
   else if (Graph_type==DOTMATRIX)
      cprintf("\r\nSelect A (=Automatic) for full ranking (row AND column)\r\n"
                  "Select ? to disable ranking\r\n");
   do {
      strcpy(testcode, keycopy( buffer, "B", 2));
      if (!*testcode || *testcode==SPACE) return Outcome_test;
      if ((Graph_type==RELATIVE_RISK ||
           Graph_type==ODDS_RATIO ||
           Graph_type==PEARSON ||
           Graph_type==SPEARMAN)
           &&
           toupper(*testcode)=='A') return -1;
      if (Graph_type==DOTMATRIX) {
         if (toupper(*testcode)=='A') {
            Rank_column=TRUE;
            return -1;
         } else if (*testcode=='?' || *testcode=='/') {
            Rank_column=FALSE;
            return 0;
         }
      }
      t=check_teststring(testcode, columns)?lettertonumber(testcode[0])-2:-1;
   } while (t<0 || t>=columns);
   return t;
}

/*

/* local routine */

int strmenu(int current_pos, int *selected_test, int rows, int startrow,int nooflines) {
     int pos, key;
     int n, row, col, maxnoofitems;
     char shorttestname[MAX_LEN];
     maxnoofitems=4*nooflines;
     if (rows>maxnoofitems) rows=maxnoofitems;
     if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
     *selected_test=-2;
     startchars[rows]='\0';
     n= current_pos;
     row=startrow + n % nooflines;
     col=20*(n/nooflines);
     CURSOR(row,col);
     HLON;
     cprintf("%c %-18s", flag[n], strleft(shorttestname, testname[n], 17));
     HLOFF;
     key=getkey(0);
     if (!result && key==ESC) {
        *selected_test= -1;
        return -1;
     }
     CURSOR(row,col);
     cprintf("%c %-18s", flag[n], strleft(shorttestname, testname[n], 17));
     if (key<=SPACE || key>256) {
        switch (key) {
           case UP:
                n--;
                if (n<0) n=rows-1;
                result= n;
              break;
           case DOWN:
                n++;
                if (n > rows-1) n=0;
                result= n;
              break;
           case TAB:
           case RIGHT:
                if (n<rows-nooflines) n+=nooflines;
                result= n;
              break;
           case BACKTAB:
           case LEFT:
                if (n>nooflines-1) n-=nooflines;
                result= n;
              break;
           case PgUp:
                n=(n>nooflines-1 && rows>nooflines) ?
                     nooflines:
                     0;
                result= n;
              break;
           case HOME:
                n=0;
                result= n;
              break;
           case PgDn:
                 n=(n>nooflines-1 || nooflines>rows) ?
                     rows-1:
                     nooflines-1;
                if (n>rows-1) n=rows-1;
                result= n;
              break;
           case SHIFT_HOME:
                n=rows-1;
                result= n;
              break;
           case CTRL_A:
              *selected_test= -4;
              result= n;
              break;
           case CTRL_B:
           case CTRL_Z:
              *selected_test= -3;
              result= n;
              break;
           case RETURN:
           case SPACE:
              *selected_test=n;
              result= n;
              break;
           case EXEC:
           case F10:
              *selected_test= -1;
              result= n;
              break;
           case CTRL_C:
               farewell(pic_no);
           default:
               result= n;
               break;
        }
    } else {
            pos=strpos(startchars,toupper(key),n);
            n=(pos<0) ? n : pos;
            result=  n;
    }
    return result;
}
*/

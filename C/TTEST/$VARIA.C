/* GGGGGGGGGGGGGGGGGG  MISCELLANEOUS ROUTINES  GGGGGGGGGGGGGGGGGGGGGG */
#include "ttest.h"
/* module $varia.c */
int    check_teststring(char *teststring, int columns);
int    count_char(char *s, char c);
void   dotted_h_line(int x0, int x1, int y0);
void   dump_arrays(void);
void   dump_block(unsigned char *pointer, int noofbytes);
int    empty_tests(int first_test, int last_test);
void   expand_evalstring(int t);
int    fillmode(void);
int    find_dx( int point, int n, int intra);
int    find_symbols_in_use(int subset);
char  *ftoa(float f);
long   getaddress(void far *address);
int    getkey(int flag);
int    get_maxlegend(void);
int    get_bottomlines(int *range_type, int *testnamelines, int *evaltextlines);
int    get_headerlines(int range_type, int testnamelines, int evaltextlines);
int    get_legendlines(void);
char   get_symbol(int datapoint, int column, int *row);
int    isdecimal(char c);
int    isdelimiter(char c);
int    isnumber(char *s);
int    isvoid(char *s);
char  *keycopy(char *target,char *source,int maxlen);
int    lettertonumber(char c);
int    lotustonumber(char *s);
int    not_substring(char *s, char *substring);
char   numbertoletter(int n);
char  *numbertolotus(int i, char *s);
char  *quoted(char *buffer, char *s);
void   reset_prob(void);
void   show_text(char **text, char *teststring, int startrow, int textlines, int screenlines);
char  *strinput(char *s);
char  *stripwhites(char *s);
char  *strleft(char *buffer, char *s, int n);
int    strmenu1(char *name, int *current_pos, int rows, int xpos, int ypos);
int    strpos(char *s,char c,int pos);
int    symbols_in_use(void);
void   toupper_evalstring(char *evalstring, int columns);
void   write_screen(int startrow, int rows, int nooflines, int new);
void   write_prob_strings(int show_flag);
void   write_stat_strings(void);
void   p_parameters(int i, int show_flag);
void   p_values(int i, int t0, int t1, int level, int showflag);

int check_teststring(char *teststring, int columns) {
   int i,n, result=1;
   for (i=0;i<strlen(teststring);i++) {
      if (teststring[i]==SPACE) continue;
      n=lettertonumber(teststring[i]);
      if (n<-1) result=0;
      if (n>columns+2) {
         n=lettertonumber(toupper(teststring[i]));
         if (n>=0 && n<columns+2) teststring[i]=toupper(teststring[i]);
         else result=0;
      }
   }
   return result;
}   

int count_char(char *s, char c) {
int n=0, result=0;
   while(s[n]) if (s[n++]==c) result++;
   return result;
}

void dotted_h_line(int x0, int x1, int y0) {
   for (;x0+10<=x1;x0+=40) {
      picmove(x0,y0);
      picdraw(x0+10,y0);
   }
}

#ifdef TRACE
void dump_arrays(void) {
   int column, row, t;
   if (Trace==2) {
      fprintf(tracefile,"\ndata0\n        ");
      for (column=0;column<columns;column++) fprintf(tracefile,"%8d",column);
      for (row=0;row<rows;row++) {
        fprintf(tracefile,"\nrow %4d ",row);
        for (column=0;column<columns;column++) {
           if (data0[row][column]>MISSING) fprintf(tracefile,"%8.3f",data0[row][column]);
           else                            fprintf(tracefile,"%8s","...");
        }
      }
      fprintf(tracefile,"\n\nrank0\n");
      for (row=0;row<rows+1;row++) {
        fprintf(tracefile,"\n%3d",row);
        for (column=0;column<columns+1;column++) {
           fprintf(tracefile, "%3d",rank0[row][column]);
        }
      }
      fprintf(tracefile,"\n\nrank\n");
      for (row=0;row<rows+1;row++) {
        fprintf(tracefile,"\n%3d",row);
        for (column=0;column<columns+1;column++) {
           fprintf(tracefile, "%3d",rank[row][column]);
        }
      }
      fprintf(tracefile,"\n\nextra_rank\n");
      for (row=0;row<rows+1;row++) {
        fprintf(tracefile,"\n%3d",row);
        for (column=0;column<4;column++) {
           fprintf(tracefile, "%3d",extra_rank[row][column]);
        }
      }
      fprintf(tracefile,"\n\ncasnum[][]-array");
      for (row=0;row<rows+1;row++) {
         fprintf(tracefile,"\nrow %4d ",row);
         for (t=0;t<nooftests+1;t++) fprintf(tracefile,"%3d", casnum[row][t]);
      }
      fprintf(tracefile,"\n\nclass[][]-array");
      for (row=0;row<Noofclasses+3;row++) {
         fprintf(tracefile,"\nclass %4d ",row);
         for (t=0;t<nooftests+3;t++) fprintf(tracefile,"%3d", class[row][t]);
      }
      fprintf(tracefile,"\n\nclasscounter[][]-array");
      for (row=0;row<Noofclasses+3;row++) {
         fprintf(tracefile,"\nclass %4d ",row);
         for (t=0;t<nooftests+3;t++) fprintf(tracefile,"%3d", classcounter[row][t]);
      }
      fprintf(tracefile,"\n\ntextpointer");
      dump_block((unsigned char *)textpointer,(4*columns+SYMBOLS+20)*(MAX_LEN+1));
/*      for (i=0;i<(4*columns+SYMBOLS+20)*(MAX_LEN+1)+80;i++) {
         if (!(i%79)) fprintf(tracefile, "\n");
         c=textpointer[i];
         fprintf(tracefile,"%c",c>31?c:'.');
      }
*/
      fprintf(tracefile,"\nbuffer:\n");
      dump_block((unsigned char *)buffer, 512);
/*      for (i=0;i<512+80;i++) {
         if (!(i%79)) fprintf(tracefile, "\n");
         c=buffer[i];
         fprintf(tracefile,"%c",c>31?c:'.');
      }
*/
   }
}

void dump_block(unsigned char *pointer, int noofbytes) {
int n, nn;
unsigned char c;
    for (n=-32; n<noofbytes+16; n+=16) {
        fprintf(tracefile, "%4d ",n);
        for (nn=0;nn<16;nn++){
            c=*(pointer+n+nn);
            fprintf(tracefile, "%2X ",c%256);
        }
        for (nn=0;nn<16;nn++){
            c=*(pointer+n+nn);
            fprintf(tracefile, "%c",c>31 && c<127?c:'.');
        }
        fprintf(tracefile,"\n");
    }
}
#endif

int empty_tests(int first_test, int last_test) {
   int t,n;
   for (t=first_test, n=0; t<=last_test; t++) if (!valid_data[t]) n++;
   return n;
}

void expand_evalstring(int t) {
   int pos, pos2,code, maxlen=MAX_LEN, quote=0;
   char c;
   for (pos2=0;pos2<maxlen; pos2++) evaltext[t][pos2]='\0';
   for (pos=0, pos2=0;pos<strlen(evalstring[t]) && pos2 < maxlen-1; pos++) {
      c=evalstring[t][pos];
      if (c=='\'' || c=='\"') quote=(quote)?0:1;
      if (c<'@' || (c>'Z' && c<'a'|| quote) || c>'z') evaltext[t][pos2++]=c;
      else if (!quote && (code=lettertonumber(c))<columns+2 && pos2 + strlen(testname[code]) + 1 < maxlen) {
         strcat(evaltext[t],textstring[code]);
         pos2=strlen(evaltext[t]);
      }
   }
}

int fillmode(void){
   int i;
   for (i=0; i<=FILL_COL; i++) if (filltype[i]>-1) return 1;
   return 0;
}

int find_dx( int point, int n, int intra) {
    return point * intra -(n-1)*intra/2;
}

int find_symbols_in_use(int testcode) {
   int i,t, pos, symbols=0, color, max_test, max_row;
   char symbol;
/*
   max_test=nooftests;
*/
   max_row=rows;
   if (testcode==-1) max_test=nooftests;
   else if (testcode==-2) max_test=columns;
   else max_test=1;
   if (testcode==-1 || testcode==-2) {
      for (i=0;i<SYMBOLS;i++) {
         used_symbolstring[i]=0;
         symbol_in_use[i]=FALSE;
         selected_symbolstring[i]='0';
/*
         Symbol_color[i]=i;  /************ veranderd: i was 0 ********/
*/
      }
      selected_symbolstring[SYMBOLS]='\0';
   }
   for (t=0; t<max_test; t++) {
      if (test0[t]==-99 || !valid_data[test0[t]]) continue;
      for (i=0;i<max_row;i++) {
         if (data0[i][test0[t]]==MISSING) continue;
/*
         symbol=(testcode==-1)?rowlabel[casnum[i][test0[t]]]:rowlabel[i];
*/
         symbol=rowlabel[i];
         pos=strpos(symbolstring,symbol,0);
         if (pos>-1) symbol_in_use[pos]=symbol;
      }
   }
   for (i=0,symbols=0;i<SYMBOLS;i++) {
      if (symbol_in_use[i]) {
         used_symbolstring[symbols]=symbolstring[i];
         selected_symbolstring[i]='1';
         symbols++;
      }
   }
   used_symbolstring[symbols]='\0';
/*
   if (Color>0) {
      for (i=0, color=0; i< SYMBOLS; i++) {
         if (symbol_in_use[i]) {
            Symbol_color[i]=1 + color%(Color+1);
            color++;
         }
      }
   } else for (i=0; i< SYMBOLS; i++) if (symbol_in_use[i]) Symbol_color[i]=1;
*/
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d: used_symbolstring:\"%s\" selected_symbolstring=\"%s\"",__FILE__, __LINE__, used_symbolstring, selected_symbolstring);
         for (i=0; i<SYMBOLS; i++) fprintf(tracefile,"\nsymbol_in_use[%d]=%d Symbol_color[%d]=%d",
            i, symbol_in_use[i], i, Symbol_color[i]);
   #endif
   return symbols;
}

char *ftoa(float f) {
   sprintf(buffer,"%.3f",f);
   return buffer;
}

long getaddress(void far *address) {
   return 16*FP_SEG(address) + FP_OFF(address);
}

int getkey(int flag) {
int c;
   result=1;
   c=getch();
   if (c==SPECIAL_KEY) {
      c=256+getch();
      if (c>F5 && c<=F10) return c;
   } else {
      if (c==ESC) result=0;
      if (c==CTRL_C) farewell(pic_no);
      if (!flag) c=toupper(c);
   }
   return c;
}

int get_bottomlines(int *range_type, int *testnamelines, int *evaltextlines) {
   int n, t, bottomlines;
   *range_type=*testnamelines=*evaltextlines=0;
   if (Subselect) *range_type= (nooftests>1 && strcmp(evaltext[0],evaltext[1])) ? 1 : 2;
   for (t=0;t<nooftests;t++) {
      if (!N[t]) continue;
      if ((n=count_char(testname[test[t]],'\\')+1) > *testnamelines) *testnamelines=n;
   }
   if (Subselect) for (t=0;t<nooftests;t++) {
      if (!N[t]) continue;
      if ((n=count_char(evaltext[t],'\\')+1)> *evaltextlines) *evaltextlines=n;
   }
   bottomlines=(Datasymbol && Legend>0 && !Legendpos && symbols && Graph_type==TTEST) ?1+ceil((double)symbols/Legend):0;
   if (Subselect) {
      if (No_ranges) bottomlines+= (*range_type==1) ? *evaltextlines: *testnamelines;
      else {
         if (Ranges_in_header) bottomlines+= (*range_type==1) ? *evaltextlines: *testnamelines;
         else                  bottomlines+= *evaltextlines + *testnamelines;
      }
   } else bottomlines+= *testnamelines;
   if (Stagger) bottomlines++;
   #ifdef TRACE
      if (Trace==2) tracefile=fopen("TRACE","at");
            fprintf(tracefile,"\n%4d bottomlines=%d range_type=%d testnamelines=%d evaltextlines=%d",
          __LINE__, bottomlines, *range_type, *testnamelines, *evaltextlines);
   #endif
   return bottomlines;
}

int get_headerlines(int range_type, int testnamelines, int evaltextlines) {
   int headerlines;
   strcpy(First_title , stripwhites(First_title));
   strcpy(Second_title, stripwhites(Second_title));
   strcpy(Y_axis_title, stripwhites(Y_axis_title));
   if      (*Second_title) headerlines= 2;
   else if (*First_title)  headerlines= 1;
   else                    headerlines= 0;
   if (Subselect && Ranges_in_header && !No_ranges) headerlines += (range_type==1) ? testnamelines: evaltextlines;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d headerlines=%d",
       __LINE__, headerlines);
   #endif
   return headerlines;
}

int get_legendlines(void) {
   int i, t, n;
   for (i=0,t=0, n=0; i<SYMBOLS; i++) {
      if (!symbol_in_use[i]) continue;
      n+=count_char(symbol_legend[i],'\\')+1;
      t++;
   }
   return n;
}

int get_maxlegend(void) {
   int i, len, maxlen=0;
   if (!Datasymbol) return 0;
   for (i=0; i<SYMBOLS; i++) {
         if (!symbol_in_use[i]) continue;
         len=strcspn(symbol_legend[i],"\\");
/* GAAT DUS SOMS FOUT WANNEER VERVOLG VAN LEGENDE LANGER IS DAN EERSTE STUK! */
         if ( maxlen < len) maxlen=len;
   }
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d maxlegend=%d",__FILE__, __LINE__,maxlen);
   #endif
   return maxlen;
}

char get_symbol(int datapoint, int column, int *row) {
   char c;
   *row= rank[datapoint][column];
   c= (Subselect) ? rowlabel[casnum[*row][column]]: rowlabel[*row];
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d %s: datapoint=%2d column=%2d *row=%3d symbol=%d",
         __LINE__, __FILE__, datapoint, column, *row, c);
   #endif
   if (c>='0' && c<'0'+SYMBOLS) return (c-'0');
   if (c>=' ' && c<127) return c;
   return 0;
}

int isdecimal(char c) {
   if (isdigit(c) ||
       c=='.'     ||
       c=='+'     ||
       c=='-') return 1;
   else return 0;
}

int isdelimiter(char c) {
   return (isspace(c) || c==',' || c==';');
}

int isnumber(char *s) {
int n, len=strlen(s);
   if (!len)                                        return 0;
   for (n=0; n<len ; n++)
      if (!(isdecimal(s[n])))                       return 0;
   if (s[len]=='.')                                 return 0;
   for (n=1;n<len;n++) if (s[n]=='+' || s[n]=='-')  return 0;
   for (n=0;n<len-1;n++) if (s[n]=='.')  
      for (n++;n<len-1;n++) if (s[n]=='.')          return 0;
   return 1;
}

int isvoid(char *s) {
   int i;
   for (i=0;s[i];i++) if (!isspace(s[i])) return 0;
   return 1;
}

char *keycopy(char *target,char *source,int maxlen) {
int t,tt,s,insert=0;
int c;
   result=0;
   for (t=0;t<maxlen;t++) target[t]='\0';
   maxlen--;
   c=0;
   while(kbhit()) getch();
   for (t=0,s=0;t<maxlen && c!=RETURN;) {
      c=getkey(1);
      if (!c) return 0;
      if (c>31 && c<255) {
         putch(c);
         target[t++]=c;
         if (insert==0 && (source[s+1])!='\0') s++;
      } else {
         switch (c) {
         case EXEC:
         case RETURN:
            if (!target[0]) {
               strcpy(target,source);
               result=RETURN;
            }
            else target[t]='\0';
            break;
         case ESC:
         case CANCEL:
         case BACKTAB:
            target[0]=0;
            result=ESC;
            c=RETURN;
            break;
         case BACKSPACE:
            insert=0;
            if (t>0) {
               cprintf("\b \b");
               t--;
            }
            if (s>0) s--;
            break;
         case RIGHT:
            insert=0;
            if (t<maxlen && source[s]){
               putch(source[s]);
               target[t++]=source[s++];
            }
            break;
         case DELETE:
            source++;
            insert=0;
            break;
         case INSERT:
            insert=1;
            break;
         case LEFT:
            insert=0;
            if (t>0) {
               cprintf("\b \b");
               t--;
            }
            if (s>0) s--;
            break;
         case F3:
            insert=0;
            while (t<maxlen && source[s]) {
               putch(source[s]);
               target[t++]=source[s++];
            }
            break;
         case F5:
            insert=0;
            putch('@');
            putch('\n');
            for (tt=0;tt<t;tt++) {
               source[tt]=target[tt];
               putch('\b');
            }
            for (t=0;t<maxlen;t++) target[t]='\0';
            s=t=0;
            break;
         case UP:
            result=UP;
            target[0]='\0';
            c=RETURN;
            break;
         case DOWN:
            result=DOWN;
            target[0]='\0';
            c=RETURN;
            break;
         case CTRL_C:
            farewell(pic_no);
            break;
         }
      }
   }
   return target;
}

int lettertonumber(char c) {
   int result=-1;
   if (c!=SPACE) {
      if (c >= '@' && c<='Z') result= c-'@';
      else if (c >='a')       result= 27+c-'a';
      if (result !=-1 && result>=columns+2) result= toupper(c)-'@';
/*
      if (result<0 || result>=columns+2) cprintf("\r\nnon-existing test %c\r\n",c);
*/
   }
   return result;
}

int lotustonumber(char *s) {
   int result;
   if (*s==SPACE) result=-1;
   else if (strlen(s)==1) result=toupper(s[0])-'A';
   else result=26 * (toupper(s[0])-'A'+1) + toupper(s[1])-'A';
   if (result<0 || result >256) result =-1;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d lotustonumber %s -> %d",__FILE__, __LINE__, s, result);
   #endif
   return result;
}

int not_substring(char *s, char *substring) {
/* returns char from substring NOT found in mainstring s */
   int i, ii, found, c;
   for (i=0; i<strlen(substring); i++) {
      found=FALSE;
      c=toupper(substring[i]);
      if (c==SPACE) found=TRUE;
      else for (ii=0; ii<strlen(s) && !found; ii++) if (c==toupper(s[ii])) found=TRUE;
      if (!found) break;
   }
   if (!found) return c;
   return 0;
}

char numbertoletter(int n) {
   if (n==-1) return SPACE;
   if (n>=0 && n<27) return '@'+n;
   else if (n>=27 && n<columns+2) return 'a'+n-27;
   else {
      cprintf("\r\nnon-existing test %d\r\n",n);
      cprintf("press any key to continue ");
      if (getch()==ESC) farewell(pic_no);
   }
   return '\0';
}

char *numbertolotus(int i, char *s) {
   if (i==-1) strcpy(s," ");
   else if (i<26) sprintf(s, "%c",i+'A');
   else      sprintf(s, "%c%c",i/26+'A'-1,i%26+'A');
   return s;
}

char *quoted(char *buffer, char *s) {
   if (!buffer || !s) {
printf("\n%s %4d : null-pointer assignment in quoted() ",__FILE__, __LINE__);
printf("\nbuffer=%p s=%p", buffer, s);
if (getch()==27) farewell(-1);
putch('\n');
}
   sprintf(buffer,"\"%s\"",s);
   return buffer;
}

void reset_prob(void) {
   int i;
   pstart[0]=0;
   pend[0]=1;
   pvalue[0]=MISSING;
   strcpy (pstring[0],"p=??");
   for (i=1; i<PSTRING; i++) {
      pstart[i]=-1;
      pend[i]=-1;
      pvalue[i]=MISSING;
      pstring[i][0]='\0';
   }
}

void show_text(char **text, char *teststring, int startrow, int textlines, int screenlines) {
   char shorttestname[MAX_LEN];
   int t, row, column, screencolumns, maxlen;
   if (strlen(teststring)<=0 || screenlines<=0) {
      clrscr();
      cprintf("Program error: teststring=\"%s\" textlines=%d screenlines=%d\r\n", teststring, textlines, screenlines);
      if (getch()==ESC) farewell(-1);
      return;
   }
   screencolumns= 1+(textlines-1)/screenlines;
   maxlen= 80/screencolumns;
   maxlen -= (*teststring) ? 1:3;
   #ifdef TRACE
       if (Trace==2) fprintf(tracefile,"\n%4d text[0]:%s teststring: %s lettertonumber(teststring[0]): %d  text[lettertonumber(teststring[0])]: %s textlines=%d screenlines=%d screencolumns=%d maxlen=%d",
          __LINE__, text[0], teststring, lettertonumber(teststring[0]), text[lettertonumber(teststring[0])], textlines, screenlines, screencolumns, maxlen);
   #endif
   for (t=0; t<textlines; t++) {
      row= startrow + t%screenlines;
      column= 1 + t/screenlines * maxlen;
      column+= (*teststring) ? 1:3;
      gotoxy(column, row);
      if (*teststring) cprintf("%c %-*s", teststring[t], maxlen-2, strleft(shorttestname, text[lettertonumber(teststring[t])], maxlen-3));
      else             cprintf("%3d %-*s", t+1, maxlen, strleft(shorttestname, text[t], maxlen-1));
   }
   gotoxy(1,startrow+screenlines+1);
}

char *strinput(char *s) {
   cprintf("\r\n%s: ",s);
   keycopy(buffer,s, MAX_LEN);
   strcpy(s,buffer);
   return s;
}

char *stripwhites(char *s) {
   int p;
   char *s2;
   s2=s;
   for (p=0; p<strlen(s);p++) {
      if (isspace(s[p])) s2++;
      else break;
   }
   if (strlen(s2)>0) {
      for (p=strlen(s2)-1;strlen(s2)>0;p--) {
         if (isspace(s2[p])) {
            s2[p]='\0';
         } else break;
      }
   }
   if (s2[0]==QUOTE){
      s2++;
      if (s2[p-1]==QUOTE) s2[p-1]='\0';
  }
  s=s2;
  return s2;
}

char *strleft(char *buffer, char *s, int n) {
   int i;
   for (i=0; s[i] && i<n; i++) buffer[i]=s[i];
   buffer[i]='\0';
   return buffer;
}

int strmenu1(char *name, int *item, int rows, int xpos, int ypos) {
     int result=FALSE;
     int key, pos;
     gotoxy(xpos,ypos);
     HLON;
     cprintf("%s",name);
     HLOFF;
     key=getkey(0);
     gotoxy(xpos,ypos);
     cprintf("%s",name);
     if (key<=SPACE || key>256) {
        switch (key) {
           case UP:
              (*item)--;
              if (*item<0) *item=rows-1;
              break;
           case DOWN:
              (*item)++;
              if (*item > rows-1) *item=0;
              break;
           case PgUp:
           case HOME:
              *item=0;
              break;
           case PgDn:
              *item=rows-1;
              break;
           case RETURN:
           case SPACE:
           case EXEC:
               result = TRUE;
               break;
           case ESC:
           case CANCEL:
               result = -1;
               break;
           case CTRL_C:
               farewell(pic_no);
           default:
               result = FALSE;
               break;
        }
    } else {
         pos=strpos(startchars,toupper(key),*item);
         if (pos<0) result=0;
         else {
            *item= pos;
            result = 1;
         }
    }
    return result;
}

int strpos(char *s,char c,int pos) {
/* finds next occurrence of c in s starting at pos */
int p;
     for (p=pos+1;s[p];p++) if (s[p]==c) return p;
     for (p=0;p<=pos;p++) if (s[p]==c) return p;
     return -1;
}

int symbols_in_use() {
   int i,result;
   for (i=0,result=0; i<SYMBOLS;i++) if (symbol_in_use[i]) result++;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%s %4d symbols_in_use:%d",__FILE__, __LINE__, result);
   #endif
   return result;
}

void toupper_evalstring(char *evalstring, int columns) {
   int i, quote=0;
   char c;
   for (i=0;i<strlen(evalstring);i++) {
      c=evalstring[i];
      if (c=='\'' || c=='\"') quote=(quote)?0:1;
      else if (c>='a' && columns<26 && !quote) evalstring[i]=toupper(c);
   }
}   

void write_screen(int startrow, int rows, int nooflines, int new) {
int n, row, col, maxnoofitems;
char shorttestname[MAX_LEN];
  maxnoofitems=4*nooflines;
  if (rows>maxnoofitems) rows=maxnoofitems;
  if (rows>MAXNOOFITEMS) rows=MAXNOOFITEMS;
  for (n=0;n<rows;n++) {
      row=startrow + n % nooflines;
      col=20*(n/nooflines);
      CURSOR(row,col);
      if (new) cprintf("%c %-18s", flag[n], strleft(shorttestname, testname[n], 17));
      else cprintf("%c",flag[n]);
  }
}

void write_prob_strings(int showflag) {
   int i;
   calc_flag=FALSE;
   if (!prob_text_flag) for (i=0; i<PSTRING; i++) {
      pstart[i]=-1;
      pvalue[i]=MISSING;
      strcpy(pstring[i],"p=???");
   }
   Prob_flag=FALSE;
   process_data();
   adjust_minmax();
   Prob_flag=TRUE;
   switch (nooftests) {
      case 0:
      case 1:
        pstart[0]=-1;
        pend[0]=-1;
        break;
     case 2:
        p_values(0, 0, 1, 1, showflag);
        break;
     case 3:
        p_values(0, 0, 1, 1, showflag);
        p_values(1, 1, 2, 1, showflag);
        p_values(2, 0, 2, 2, showflag);
        break;
     case 4:
        p_values(0, 0, 1, 1, showflag);
        p_values(1, 1, 2, 1, showflag);
        p_values(2, 2, 3, 1, showflag);
        p_values(3, 0, 2, 2, showflag);
        p_values(4, 1, 3, 3, showflag);
        p_values(5, 0, 3, 4, showflag);
        break;
     case 5:
        p_values(0, 0, 1, 1, showflag);
        p_values(1, 1, 2, 1, showflag);
        p_values(2, 2, 3, 1, showflag);
        p_values(3, 3, 4, 1, showflag);
        p_values(4, 0, 2, 2, showflag);
        p_values(5, 2, 4, 2, showflag);
        p_values(6, 0, 3, 3, showflag);
        p_values(7, 1, 4, 4, showflag);
        p_values(8, 1, 3, 5, showflag);
        p_values(9, 0, 4, 6, showflag);
        break;
    }
    for (i=0; i<PSTRING; i++) if (pstart[i]!=-1 && pvalue[i]!=MISSING) {
       if (ymin>pvalue[i] && !(Logflag && pvalue[i]<=0)) ymin=pvalue[i];
       if (ymax<pvalue[i]) ymax=pvalue[i];
    }
}

void p_values(int i, int t0, int t1, int level, int showflag) {
float levelvalue;
   if (N[t0]&&N[t1]) {
      pstart[i]=t0;
      pend[i]=t1;
      levelvalue=2*(float)level*Vsize/scale_yfactor;
      if (Logflag) pvalue[i]=ymax*exp(levelvalue);
      else pvalue[i]=ymax+levelvalue;
/*
if (Logflag) cprintf("\r\n%s %4d : ymax=%.3f exp(%.3f)=%.3f pvalue=%.3f",
__FILE__, __LINE__, ymax, levelvalue, exp(levelvalue), pvalue[i]);
else cprintf("\r\n%s %4d : ymax=%.3f levelvalue=%.3f pvalue=%.3f",
__FILE__, __LINE__, ymax, levelvalue, pvalue[i]);
if (getch()==ESC) farewell(-1);
putch('\r');
putch('\n');
*/
      p_parameters(i, showflag);
   } else {
      pstart[i]=-1;
      pvalue[i]=MISSING;
   }
}

void p_parameters(int i, int show_flag) {
  double p;
  if (!(pstart[i]>-1 && N[pstart[i]] && pend[i]>-1 && N[pend[i]])) {
     pstart[i]=-1;
     return;
  }
  p=tt_calc(pstart[i], pend[i], Mean_flag);
  if (Mean_flag && show_flag) cprintf("\r\np=%.4lf, (%s)\r\n", p, statmode_string[Mean_flag-1]);
/*
cprintf("\r\ni=%d pstart=%d pend=%d, p=%.4lf, (%s)\r\n", i, pstart[i], pend[i],p, statmode_string[Mean_flag-1]);
cprintf("\r\n%s %4d : ",__FILE__, __LINE__);
if (getch()==ESC) farewell(-1);
putch('\r');
putch('\n');
*/
  if (p>0.05) strcpy(pstring[i],"NS");
  else if (p>=0.0095) sprintf(pstring[i],"p=%.2lf",p);
  else if (p>=0.00095) sprintf(pstring[i],"p=%.3lf",p);
  else if (p>=0) strcpy(pstring[i],"p<0.001");
  else strcpy(pstring[i],"p=??");
  if (!show_flag) return;
  cprintf("\r\nDefault text: \"%s\" ",pstring[i]);
  keycopy(buffer, pstring[i], 40);
  if (!result) {
     pstart[i]=-1;
     return;
  }
  strcpy(pstring[i], buffer);
  if (pstring[i][0]==' ') pstart[i]=-1;
}

void write_stat_strings(void) {
  if (Graph_type==OGIVE) {
     if (!User_legend_flag) {
        strncpy(Y_axis_title,testunit[test[0]],MAX_LEN);
        strcpy(y_axis_title,Y_axis_title);
        *range_string='\0';
     }
     return;
  }
  strcpy(stat_string,"(none)");
  switch (Graph_type) {
     case TTEST:
     case BAR_GRAPH:
        if (Stat==-1) strcpy(range_string,"");
        else if (Sd_flag) strcpy(range_string,"mean (SD)");
        else if (Sem_flag) strcpy(range_string,"mean (SE)");
        else if (Mean_flag) strcpy(range_string,"mean");
        else if (Median_flag==1) {
           if (Centile) {
              if (Iqr_flag) sprintf(range_string,"%d-centile (SE)",(int)(Centile*100.001));
              else sprintf(range_string,"%d-centile",(int)(Centile*100.001));
           } else {
              if (Iqr_flag) strcpy(range_string,"median (IQR)");
              else strcpy(range_string,"median");
           }
        }
        else if (Median_flag==2) {
           strcpy(range_string,"10-,25-,50-,75-,90-ile");
           strcpy(stat_string,"box-plot");
        }
        else strcpy(range_string,"");
        break;
     case PERCENTAGE_POSITIVE:
     case PEARSON:
     case SPEARMAN:
     case RELATIVE_RISK:
     case ODDS_RATIO:
        strcpy(range_string,(Sem_flag)?"SE":"95% CI");
        break;
     default:
        strcpy(range_string,"");
  }
  if (Mean_flag) {
     strcpy(stat_string, "mean");
  } else if (Median_flag==1) {
     if (Centile) sprintf(stat_string,"%d-centile",(int)(Centile*100.001));
     else strcpy(stat_string, "median");
  }
  if (!User_legend_flag) {
     switch (Graph_type) {
        case TTEST:
        case BAR_GRAPH:
        case DOTMATRIX:
           if (!cormatrix_flag) strncpy(Y_axis_title,testunit[test[0]],MAX_LEN);
           break;
         case PERCENTAGE_POSITIVE:
           if (Cutoff_flag!=1) sprintf(Y_axis_title,"Percentage >%g",Cut_off);
           else strcpy(Y_axis_title,"Percentage positive");
           break;
         case PEARSON:
           sprintf(Y_axis_title,"Pearson r");
           if (Outcome_test!=-1) {
              strcat(Y_axis_title, " for ");
              strcat(Y_axis_title, testname[Outcome_test]);
           } else if (Distance) strcat(Y_axis_title, " distance coefficients");
           break;
         case SPEARMAN:
           sprintf(Y_axis_title,"Spearman rho",testname[Outcome_test]);
           if (Outcome_test!=-1) {
              strcat(Y_axis_title, " for ");
              strcat(Y_axis_title, testname[Outcome_test]);
           } else if (Distance) strcat(Y_axis_title, " distance coefficients");
           break;
         case RELATIVE_RISK:
           sprintf(Y_axis_title,"Relative risk for %s", testname[Outcome_test]);
           break;
         case ODDS_RATIO:
           sprintf(Y_axis_title,"Odds ratio for %s", testname[Outcome_test]);
           break;
      }
      if (Stattext_flag && *range_string && !cormatrix_flag) {
         if (*Y_axis_title>' ') sprintf(y_axis_title,"%s\\%s", Y_axis_title, range_string);
         else sprintf(y_axis_title,"%s", range_string);
      }
/*
      if (Stattext_flag && *range_string && !(Median_flag==2)) sprintf(y_axis_title,"%s\\%s", Y_axis_title, range_string);
*/
      else strcpy(y_axis_title, Y_axis_title);
   }
}

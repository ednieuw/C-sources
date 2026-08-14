/* BBBBBBBBBBBBBBBBBB   FILE I/O   BBBBBBBBBBBBBBBBBBB */
#define FTTCODE "RCAFTT01"
#define LENFILENAME 12
#define SCREENSTARTROW 4
#define NOOFLINES (25-SCREENSTARTROW)
#define SCREENCOLUMNS 80
#define TEXTCOLUMNS 4
#define MAXFILES (TEXTCOLUMNS*NOOFLINES)
#define COPYBUFFERSIZE 128

char filenames[MAXFILES][LENFILENAME+1];

#include "ttest.h"
#include "bios.h"

/* module $file.c */
int  check_output_file(char *new_name, char *default_name, char *ext);
int  check_dosname(char *s);
int  check_drive(char *s);
void display_logfile(void);
char *getfile(char *s);
char *make_ttname(char *tt_file, char *filename, int pic_no);
void openin(char *s);
void openout(char *s);
void open_logfile(char *s);
void open_tracefile(char *s);
int  picsave(char *source, char *target, char *ext);
int  piccopy(char *source, char *target);
void read_casenames(char *f);
void read_data(char *f, int protocol_code);
int read_ftt(char *s);
void read_ile(char *f);
void read_info(char *f);
void read_ranks(char *f);
void read_testunits(char *f);
void read_wksdata(void);
void write_ftt(char *s);

/* local routines: */
int  fgetline(FILE *fpin, char *s);
void parse_line(int column, char *buffer);
char *make_filename(char *fullname, char *default_path, char *default_ext, int forced_ext_flag);
int strpos2(char *s,char c,int pos);
void makedate(int *m,int *d,int *y,unsigned total);
void maketime(int *h,int *m,int *s,unsigned total);
char *strnleft2(char *buffer, char *s1, char stop, int n);
int strmenu(int current_pos, int rows, int startrow,int nooflines);

int check_dosname(char *s) {
   char c; int i, namestart=0, extlen, len, dotpos=0;
   if (strpos2(s,'?',0) !=-1) return 0;
   if (strpos2(s,'*',0) !=-1) return 0;
   if (strpos2(s,'/',0) !=-1) return 0;
   if (s[1]==':' && (c=toupper(*s))>'@' && c<'F') namestart=2;
   if (strpos2(s,':',namestart) !=-1) return 0;
   if (strlen(s)-namestart>1 && strpos2(s,' ',namestart+1) !=-1) return 0;
   do {
      i=strpos2(s, '\\', namestart)+1;
      if (i) namestart=i;
   } while (i);
   i=strpos2(s,'.',namestart);
   if (i>-1) {
      dotpos=++i;
      if (i<strlen(s) && strpos2(s,'.',i)!=-1) {
         return 0;
      }
      extlen=strlen(s)-dotpos+1;
      if (extlen<2 || extlen>4) return 0;
   } else extlen=0;
   if ((len=strlen(s)-extlen-namestart)>8) return 0;
   return len;
}

int check_drive(char *s) {
int  result, drive,namestart=0;
char c,tuffer[512];
   if (s[1]==':' && (c=toupper(*s))>'@' && c<'C') namestart=2;
   drive=c-'A';
   if(namestart==2 && drive<2)
   {
   result = biosdisk(4,drive,0,0,0,1,tuffer);
   if(result==128)    {
        gotoxy(1,13);
        cprintf("Drive %c Not Ready\r\n",drive +'A');
        return(0);
      }
   }
   return 1;
}

int check_output_file(char *new_name, char *default_name, char *ext) {
   FILE *fptemp;
   strcpy(new_name,default_name);
   result=0;
   make_filename(new_name,Output_path,ext,1);
   fptemp=fopen(new_name,"rb");
   while (result!=RETURN && fptemp!=NULL) {
        fclose(fptemp);
        clrscr();
        cprintf("Output file \"%s\" already exists\r\nenter new name or press RETURN to overwrite\r\noutput-PIC-file :",new_name);
        if (keycopy(new_name, default_name, 80)==NULL) return 0;
        strcpy(default_name, new_name);
        if (result==RETURN) cprintf(" \"%s\"",new_name);
        if (*new_name==NULL) return 0;
        make_filename(new_name,Output_path,ext,1);
        fptemp=fopen(new_name,"rb");
   }
   fclose(fptemp);
   return 1;
}

void display_logfile(void) {
   int i=0;
   openin(ttlog);
   clrscr();
   while (fgets(buffer, 240, fpin)){
      cprintf("%s",buffer);
      i++;
      if (i%22==0) {
         MORE;
      }
   }
   fclose(fpin);
}

char *make_ttname(char *tt_file, char *filename, int pic_no) {
   int i;
   char c;
   char drive[MAXDRIVE];
   char dir[MAXDIR];
   char shortname[MAXFILE];
   char ext[MAXEXT];
   pic_no=pic_no%100;
   strncpy(buffer, filename ,80);
   fnsplit(buffer, drive, dir, shortname, ext);
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d:make_ttname: tt_file=\"%s\" outfile=%s buffer=\"%s\" shortname=\"%s\" pic_no=%d ",
        __LINE__, tt_file, filename, buffer, shortname, pic_no);
#endif
   i=0;
   while ((c=shortname[i])!='\0' && i<8) tt_file[i++]=c;
   while (i<8) tt_file[i++]='_';
   if (tt_file[7]>='0' && tt_file[7]<'9') tt_file[7]=tt_file[7]+1;
   else {
         if (tt_file[7]=='9') {
         if (tt_file[6]<'9' && tt_file[7]>='0') {
            tt_file[6]=tt_file[6]+1;
            tt_file[7]=0;
         }
      } else tt_file[7]='0';
   }
/*
   tt_file[6]='0'+pic_no/10;
   tt_file[7]='0'+pic_no%10;
*/
   tt_file[8]='\0';
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"result: \"%s\"",tt_file);
#endif
   strcpy(filename, tt_file);
   return tt_file;
}

void openin(char *s) {
   if ((fpin=fopen(s,"rb")) == NULL) {
       E4;
       cprintf("\r\n\r\nNo worksheet available for TTEST; run T0.EXE first\r\n\r\nPress any key ... ");
       getch();
       farewell(pic_no);
   }
}

void openout(char *s) {
   if ((fpout=fopen(s,"wb")) == NULL) {
       E5;
       getch();
       farewell(pic_no);
   }
}

void open_logfile(char *s) {
   setdisk(startup_drive);
   fclose(logfile);
   if ((logfile=fopen(s,"wt"))==0) {
      cprintf("cannot open \"%s\"\r\nPress any key to continue ... ",s);
      if (getch()==27) farewell(pic_no);
      putch('\r');
      putch('\n');
   }
   clearerr(logfile);

/*
   else cprintf("\r\n\r\n%s open\r\n",s);
*/
}
#ifdef TRACE
void open_tracefile(char *s) {
   if ((tracefile=fopen(s,"wt"))==0) {
      cprintf("cannot open \"%s\"",s);
      getch();
      farewell(pic_no);
   } else cprintf("%s open\r\n",s);
}
#endif
int picsave(char *source, char *target, char *ext) {
   char cli[MAX_LEN+1];
   int i, files_written, ok;
   result=0;
   if (!strlen(target)) strcpy(target,filename);
   i=strpos(target,'.',0);
   if (i==-1) i=strlen(target);
   strncpy(buffer, target, i>0?i:80);
   buffer[i]='\0';
   strcpy(target, buffer);
   strcpy(cli,buffer);
   result=0;
/*
   gotoxy(1,23);
*/
   cprintf("Default file-name: %s%s (RETURN=ok) --------\b\b\b\b\b\b\b\b",target, ext);
   do {
      if (keycopy(buffer,target,MAX_LEN)==0 || result==ESC) {
         strcpy(target, cli);
         return 0;
      }
      if (!(check_dosname(buffer))) {
        ok=FALSE;
        clrscr();
        cprintf("\"%s\" is not a valid filename\r\n"
               "re-enter filename: ", buffer);
      } else ok=TRUE;
   } while (!ok);
   strncpy(target,buffer,80);
/*
   setdisk(Target_drive);
*/
   if (!check_output_file(target,buffer,ext)) return 0;
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d: picsave source=%s target=%s ext=%s",
      __LINE__, source, target, ext);
#endif
   do {
      files_written=piccopy(source,target);
      if (files_written<=0) {
         HLON;                                                           
         cprintf("\r\n%s\r\nError saving file \"%s\"\r\n", target);
         HLOFF;
         cprintf("\r\n\r\nRETURN = re-try\r\n"
               "Esc    = do NOT save, go back to main menu\r\n");
         i=getkey(0);
         if (!i) return 0;
         switch (i) {
            default:
            case RETURN:
            case EXEC:
              break;
            case ESC:
            case CANCEL:
               return 0;
         }
      } else if (!save_flag) save_flag=TRUE;
   } while (!files_written);
   success=1;
   return 1;
}

void read_casenames(char *f) {
int row,i;
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d read_casenames",__LINE__);
#endif
   for (i=0;i<SYMBOLS;i++) symbol_in_use[i]=0;
   openin(f);
   for (row=0;row<rows;row++) {
      fgetline(fpin, buffer);
      if (toupper(buffer[0])=='C' && toupper(buffer[1])=='U') {
         if (toupper(buffer[2])=='T') {
            if (toupper(buffer[3])=='O' || toupper(buffer[4])=='O') {
               if (toupper(buffer[4])=='F' || toupper(buffer[5])=='F') Cutoff_flag=1;
            } else rowlabel[row-Cutoff_flag]=*buffer;
         } else rowlabel[row-Cutoff_flag]=*buffer;
      } else rowlabel[row-Cutoff_flag]=*buffer;
      if (*buffer>='0' && *buffer<SYMBOLS+'0') {
         i=*buffer-'0';
         if (!symbol_in_use[i]) {
            if (strlen(buffer)>2) strncpy(symbol_legend[i],buffer+2,MAX_LEN);
            else sprintf(symbol_legend[i],"code %d",i);
            symbol_in_use[i]=1;
         }
      }
      if (buffer[0]=='$' && buffer[1]=='$') if (symbolstart<0) symbolstart=row;
   }
   fclose(fpin);
/*
   if (symbolstart>-1) rows=symbolstart;
*/
}

void read_data(char *f, int protocol_code) {
int row,column;
float value;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d read_data protocol=%d",__LINE__,protocol_code);
   #endif
   openin(f);
   if (protocol_code != PROTOCOL_CODE ) {
      cprintf("\r\nprotocol-code %d: odd data-protocol, run T0.EXE first\n",protocol_code);
      getch();
      farewell(pic_no);
   } else {
      if (Cutoff_flag) for (column=0;column<columns;column++) data0[rows-1][column]=MISSING;
      for (row=0;row<true_rows;row++) {
        for (column=0;column<true_columns;column++) {
           if (fread(&value, sizeof(float), 1, fpin)<=0) {;
              E2;
              getch();
              farewell(pic_no);
           }
           if (Cutoff_flag && row==0) cutoff_value[column]=value;
           else if (row<rows && column<columns) data0[row-Cutoff_flag][column]=value;
        }
      }
   }
   for (column=0; column<MAX_COL; column++) data0[rows][column]=MISSING;
   #ifdef TRACE
   if (Trace) {
      fprintf(tracefile,"\n        ");
      for (column=0;column<columns;column++) fprintf(tracefile,"%8d",column);
      for (row=0;row<=rows;row++) {
        fprintf(tracefile,"\nrow %4d ",row);
        for (column=0;column<columns;column++) {
           if (data0[row][column]>MISSING) fprintf(tracefile,"%8.3f",data0[row][column]);
           else fprintf(tracefile,"%8s","...");
        }
      }
   }
   #endif
   fclose(fpin);
}

int read_ftt(char *s) {
   char buffer2[MAX_LEN], *pointer;
   int type, i, item, subitem, t, no_text;
   unsigned int opcode;
   float f;
   initialise_arrays();
   for (t=0;t<columns;t++) test[t]=test0[t]=t;
   test[t]=test0[t]=-99;
   nooftests=columns;
   if ((fpin=fopen(s,"rb")) == NULL) return FALSE;
   clrscr();
   cprintf("Format file found: %s\r\n", s);
   while (fgetline(fpin, buffer2)!=EOF) {
      opcode=atoi(buffer2);
      type=opcode/1000;
      item=(opcode%1000)/100;
      subitem=opcode%100;
      pointer=buffer2+5;
      if (type==STRING_ARRAY && item==DA_testname) {
         if (strcmp(testname[subitem], pointer)) {
            HLON3;
            cprintf("WARNING: a change found in worksheet column %s:\r\nworksheet label:\"%s\" <> column legend:\"%s\"\r\n",numbertolotus(subitem+1,"   "), testname[subitem], pointer);
            HLOFF;
         }
         else cprintf("worksheet column %s: %s\r\n", numbertolotus(subitem+1,"   "), pointer);
      }
      if (type==INTEG && subitem==DI_graph_type) cprintf("\r\nGraph_type: %s\r\n", graph_name[atoi(pointer)]);
   }
/*
   if (!ftt_flag) {
      gotoxy(1,1);
*/
      fclose(fpin);
      cprintf("\r\nRETURN = use complete format file: \"%s\"\r\n"
           "P      = PICture information only, text from format file NOT used\r\n"
           "SPACE  = do NOT use format file, use default values instead\r\n"
           "ESC    = quit TTEST\r\n", s);
      clreol();
      i=getkey(0);
      if (i==ESC) farewell(pic_no);
      clrscr();
      if (i=='P') no_text=TRUE;
      else {
         if (i!=RETURN) return FALSE;
         else no_text=FALSE;
      }
/*
   }
*/
   fpin=fopen(s,"rb");
   if (fgetline(fpin, buffer2)==EOF) {
      fclose(fpin);
      return FALSE;
   }
   if (strcmp(buffer2,FTTCODE)) {
      clrscr();
      cprintf("Incorrect file format for %s\r\nPress any key to continue ",s);
      if (getch()==ESC) farewell(pic_no);
      fclose(fpin);
      return FALSE;
   }
   if (fgetline(fpin, ftt_name)==EOF) {
      fclose(fpin);
      return FALSE;
   }
   nooftests=0;
   if (fgetline(fpin, buffer2)==EOF) {
      fclose(fpin);
      return FALSE;
   }
   i=atoi(buffer2);
   while (i<MAX_COL && nooftests<MAX_COL) {
/*
      read testcodes until first number in line until this number > MAX_COL;
      probleem:
            test[nooftests]= (i>-2 && i<columns)? i: columns-1;
      geeft (columns-1) voor i==-1 ?!
      oorzaak: columns gedefinieerd als unsigned integer;
      i wordt (dus??) ook gezien als unsigned
*/
      if (i==-1) test0[nooftests]= -1;
      else if (i>=0 && i<columns) test0[nooftests]= i;
      else test0[nooftests]=columns-1;
      nooftests++;
      if (fgetline(fpin, buffer2)==EOF) {
         fclose(fpin);
         return TRUE;
      }
      i=atoi(buffer2);
   }
   if (nooftests<=MAX_COL) test0[nooftests]=-99;
   if (nooftests<1) {
      for (t=0; t<columns; t++) test[t]=test0[t]=t;
      test[t]=test0[t]=-99;
      nooftests=columns;
   }

/* dit gaat naar A:-drive!
#ifdef TRACE
   if (Trace==2) {
      fprintf(tracefile,"\n%4d %s:",__LINE__, __FILE__);
      for (t=0; t<nooftests; t++) if (test0[t]!=-1) fprintf(tracefile,"\ntest0 %d=%d", t, test0[t]);
   }
#endif
*/
   do {
      opcode=atoi(buffer2);
      type=opcode/1000;
      item=(opcode%1000)/100;
      subitem=opcode%100;
      pointer=buffer2+5;
/*
#ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d %s:opcode=%u type=%d item=%d subitem=%d",
            __LINE__, __FILE__,opcode,type,item,subitem);
#endif
*/
      switch (type) {
         case STRING_ARRAY:
            switch (item) {
               case DA_evalstring:
                  if (subitem>=0 && subitem<MAX_COL) strcpy(evalstring[subitem], pointer);
                  break;
               case DA_evaltext:
                  if (subitem>=0 && subitem<MAX_COL) {
                     if (no_text) expand_evalstring(subitem);
                     else strcpy(evaltext[subitem], pointer);
                  }
                  break;
               case DA_extra_text:
                  if (!no_text && subitem>=0 && subitem<10) strcpy(extra_text[subitem], pointer);
                  break;
               case DA_symbol_legend:
                  if (!no_text && subitem>=0 && subitem<SYMBOLS) strcpy(symbol_legend[subitem], pointer);
                  break;
               case DA_testname:
                  if (!no_text && subitem>=0 && subitem<MAX_COL) strcpy(testname[subitem], pointer);
                  break;
               case DA_testunit:
                  if (!no_text && subitem>=0 && subitem<MAX_COL) strcpy(testunit[subitem], pointer);
                  break;
               case DA_defaultstring:
                  if (subitem>=0 && subitem<8) strcpy(defaultstring[subitem], pointer);
                  break;
               case DA_pstring:
                  if (!no_text && subitem>=0 && subitem<PSTRING) strcpy(pstring[subitem], pointer);
                  break;
            }
            break;
         case SINGLE_STRING:
            switch (subitem) {
               case DS_cut_off1:
                  strncpy(Cut_off1, pointer,9);
                  Cut_off1[9]='\0';
                  break;
               case DS_cut_off2:
                  strncpy(Cut_off2, pointer,9);
                  Cut_off2[9]='\0';
                  break;
               case DS_first_title:
                  if (!no_text) {
                     if (strlen(pointer)>=MAX_LEN) {
                        strncpy(First_title, pointer, MAX_LEN);
                        First_title[MAX_LEN]='\0';
                     } else strcpy(First_title, pointer);
                  }
                  break;
               case DS_second_title:
                  if (!no_text) {
                     if (strlen(pointer)>=MAX_LEN) {
                        strncpy(Second_title, pointer, MAX_LEN);
                        Second_title[MAX_LEN]='\0';
                     } else strcpy(Second_title, pointer);
                  }
                  break;
               case DS_selected_symbolstring:
                  strncpy(selected_symbolstring, pointer, SYMBOLS);
                  selected_symbolstring[SYMBOLS]='\0';
                  break;
               case DS_symbol_string:
                  strncpy(Symbol_string, pointer, 19);
                  Symbol_string[19]='\0';
                  break;
               case DS_symbol_color:
                  strncpy(Symbol_color, pointer, 19);
                  Symbol_color[19]='\0';
                  break;
               case DS_y_axis_title:
                  if (!no_text) {
                     strncpy(Y_axis_title, pointer, MAX_LEN);
                     Y_axis_title[MAX_LEN]='\0';
                  }
                  break;
            }
            break;
         case INTEG:
            if (subitem>=0 && subitem<NOOFINTS) Int_array[subitem]=atoi(pointer);
            break;
         case FLOATING:
            if (subitem>=0 && subitem<NOOFFLOATS) {
               f=atof(pointer);
               Float_array[subitem]=(f<=-999999.0)?MISSING:f;
            }
            break;
         case INT_ARRAY:
            switch(item) {
               case DIA_filltype:
                  if (subitem>=0 && subitem<=FILL_COL) filltype[subitem]=atoi(pointer);
                  break;
               case DIA_pstart:
                  if (no_text) break;
                  if (subitem>=0 && subitem<PSTRING) pstart[subitem]=atoi(pointer);
                  break;
               case DIA_pend:
                  if (no_text) break;
                  if (subitem>=0 && subitem<PSTRING) pend[subitem]=atoi(pointer);
                  break;
            }
            break;
         case FLOAT_ARRAY:
            switch(item) {
               case DFA_fill_min:
                  if (subitem>=0 && subitem<=FILL_COL) {
                     f=atof(pointer);
                     fill_min[subitem]=(f<=-999999.0)?MISSING:f;
                  }
                  break;
               case DFA_fill_max:
                  if (subitem>=0 && subitem<=FILL_COL) {
                     f=atof(pointer);
                     fill_max[subitem]=(f<=-999999.0)?MISSING:f;
                  }
                  break;
               case DFA_pvalue:
                  if (no_text) break;
                  if (subitem>=0 && subitem<PSTRING) {
                     f=atof(pointer);
                     pvalue[subitem]=(f<=-999999.0)?MISSING:f;
                  }
                  break;
            }
            break;
      }
   } while (fgetline(fpin, buffer2)!=EOF);
   fclose(fpin);

   if (Outcome_test>(signed)columns) Outcome_test=columns-1;
   if (User_legend_flag) {
      if (no_text) User_legend_flag=FALSE;
      else strcpy(y_axis_title, Y_axis_title);
   }
/*
   prob_text_flag=Prob_flag/2;
   prob_pos_flag= Prob_flag%2;
*/
   write_stat_strings();
   clrscr();
   return TRUE;
}

void read_ile(char *f) {
int column;
   openin(f);
   fgetline(fpin, buffer);
   fgetline(fpin, buffer);
   fgetline(fpin, buffer);
   for (column=0;column<columns;column++) if (fgetline(fpin,buffer)) parse_line(column,buffer);
   fclose(fpin);
}

void read_info(char *f) {
   int t;
   char string1[80];
   char string2[80];
   char string3[80];

#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d read_info",__LINE__);
#endif
   openin(f);
   fgetline(fpin, string1);
   fgetline(fpin, string2);
   fgetline(fpin, string3);
   fscanf(fpin,"%d %d %d", &true_rows, &true_columns, &protocol_code);
   if (protocol_code != PROTOCOL_CODE) {
      cprintf("Problem:  Protocol_code %d; expected: %d\r\n",protocol_code, PROTOCOL_CODE);
      cprintf("Solution: Run T0.EXE first\r\n\r\nPress any key");
      getch();
      farewell(pic_no);
   }
   if (true_rows>MAX_ROW || true_columns>MAX_COL) {
      cprintf("\r\nMax array size %d rows %d columns; found: %d rows %d columns\r\n",MAX_ROW,MAX_COL,true_rows,true_columns);
      cprintf("\r\nPress Esc to quit\r\nAny other key to continue ");
      if (getch()==ESC) farewell(pic_no);
   }
   if (true_rows>MAX_ROW) rows=MAX_ROW;
   else rows=true_rows;
   if (true_columns>MAX_COL) columns=MAX_COL;
   else columns=true_columns;
   allocate();
   initialise_arrays();
   strncpy(filename,string1,79);
   strncpy(info1   ,string2,79);
   strncpy(info2   ,string3,79);
   strcpy( filename,stripwhites(filename));
   strcpy( outfile, filename);
   strcpy( info1,   stripwhites(info1));
   strcpy( info2,   stripwhites(info2));
   nooftests=columns;
   for (t=0; t<columns; t++) {
      test[t]=t;
      teststring[t]= numbertoletter(t+2);
   }
   teststring[columns]='\0';
   fscanf(fpin,"centile-option: %8.2lf\r\n",&Centile);
   fclose(fpin);
}

void read_ranks(char *f) {
int row, column, r, cutoff_rank[MAX_COL];
   for (row=0;row<rows;row++) for (column=0;column<columns;column++) rank0[row][column]=rows;
   openin(f);
   if (Cutoff_flag){
      for (row=0;row<true_rows;row++) {
        for (column=0;column<true_columns;column++) {
           r=getw(fpin);
           if (row<rows && column<columns){
              rank0[row][column]=r>0?r-1:rows;
              if (!r) cutoff_rank[column]=row;
           }
        }
      }
   } else {
     for (row=0;row<true_rows;row++) {
        for (column=0;column<true_columns;column++) {
           r=getw(fpin);
           if (row<rows && column<columns) rank0[row][column]=r;
        }
     }
   }
   fclose(fpin);
   if (Cutoff_flag) {
     for (column=0;column<columns;column++) {
        for (row=cutoff_rank[column];row<rows-1;row++) rank0[row][column]= rank0[row+1][column]--;
        rank0[rows-1][column]=rows;
     }
   }
   for (column=0; column<columns; column++) {
/*
      min[column]=getdata(rank0[rows-N[column]][column],column,TRUE);
      max[column]=getdata(rank0[rows-1][column],column,TRUE);
*/
      max[column]=getdata(rank0[0][column],column,TRUE);
      min[column]=getdata(rank0[N[column]-1][column],column,TRUE);
   }
   ymin=min[0];
   ymax=max[0];
   if (columns>1) {
      for (column=0; column<columns; column++) {
         if (ymin>min[column]) ymin=min[column];
         if (ymax<max[column]) ymax=max[column];
      }
   }

#ifdef TRACE
   if (Trace==2) {
      fprintf(tracefile,"\n%s %4d: sorted casnum-pointers\n",__FILE__,__LINE__);
      for (row=0;row<rows;row++) {
        fprintf(tracefile,"\n%3d",row);
        for (column=0;column<columns;column++) {
           fprintf(tracefile, "%3d",rank0[row][column]);
        }
      }
      fprintf(tracefile,"\n%s %4d: sorted worksheet_data\n",__FILE__,__LINE__);
      for (row=0;row<=rows;row++) {
         fprintf(tracefile,"\nscore %4d ",row);
         for (column=0;column<columns;column++) {
           if (data0[rank0[row][column]][column]>MISSING) fprintf(tracefile,"%8.2f",
                 data0[rank0[row][column]][column]);
           else fprintf(tracefile,"%8s","...");
         }
      }
    }
#endif
}

void read_testunits(char *f) {
   int column;
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d read_testunits",__LINE__);
   #endif
   openin(f);
   for (column=0;column<columns;column++) {
      fgetline(fpin, buffer);
      stripwhites(buffer);
      strncpy(testunit[column],buffer,MAX_LEN);
   }
   fclose(fpin);
}

void read_wksdata(void) {
   int i;
/*
   read_info("c:wksdata.inf");
*/
   cprintf("wksdata.ids: case codes\r\n");
   read_casenames("wksdata.ids");
   cprintf("wksdata.dat: actual data\r\n");
   read_data("wksdata.dat",protocol_code);
   cprintf("wksdata.ile: testnames and test-statistics\r\n");
   read_ile("wksdata.prn");
   cprintf("wksdata.rnk: rank numbers\r\n");
   read_ranks("wksdata.rnk");
   cprintf("wksdata.u  : testunits\r\n");
   read_testunits("wksdata.u");
   if (!*Second_title && *info2) strncpy(Second_title,info2,MAX_LEN);
   if (!*First_title && *info1)  strncpy(First_title,info1,MAX_LEN);
   for (i=0; i<columns+2; i++) completestring[i]=numbertoletter(i);
      completestring[columns+2]='\0';
   #ifdef TRACE
      if (Trace==2) fprintf(tracefile,"\n%4d read_wksdata: rows=%d columns=%d protocolcode=%d Cutoff_flag=%d",
            __LINE__, rows, columns, protocol_code, Cutoff_flag);
   #endif
}

/* local routines */

int fgetline(FILE *fpin, char *s) {
   int c;
   int pos=0, non_whites=0;
   while((c=fgetc(fpin))!=EOF && c!='\r' && c!='\n' && pos<BUFFERSIZE) {
      if (c==26) {
         c=EOF;
         break;
      }
      if (c!=' ' && c!='\t') {
         non_whites ++;
         s[pos]=c;
         pos++;
      } else if (pos) {
         s[pos]=c;
         pos++;
      }
   }
   while((c=fgetc(fpin))!=EOF && (c=='\r' || c=='\n'))
      ;
   ungetc(c,fpin);
   s[pos]='\0';
   if (non_whites==0 && c==EOF) return EOF;
   else return non_whites;
}

char *make_filename(char *fullname, char *default_path, char *default_ext, int forced_ext_flag) {
   int flag;
   char drive[MAXDRIVE];
   char default_drive[MAXDRIVE];
   char dir[MAXDIR];
   char default_dir[MAXDIR];
   char shortname[MAXFILE];
   char ext[MAXEXT];
   flag = fnsplit(default_path, default_drive, default_dir, shortname, ext);
   flag = fnsplit(fullname, drive, dir, shortname, ext);
   if (!(flag & DRIVE))      strcpy( drive, default_drive);
   if (!(flag & DIRECTORY))  strcpy( dir, default_dir);
   if (!(flag & EXTENSION) || forced_ext_flag)  strcpy( ext, default_ext);
   fnmerge(fullname,drive,dir,shortname,ext);
   fnmerge(default_path,drive,dir,"*",ext);
   return fullname;
}

void parse_line(int column, char *linebuffer) {
   int pos, pos1, pos2, p, n, len;
   char value[20];
   float digit[4];
   char c;
/*
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d column %2d: %s", __LINE__, column, linebuffer);
#endif
*/
   for (n=0, pos2=strlen(linebuffer)-1; n<4 ;n++) {
      *value='\0';
      do {
         c=linebuffer[pos2];
         if (isdelimiter(c) ) pos2--;
      } while (isspace(c) && pos2>-1 );
      pos1=pos2;
      while (pos1>0 && isdecimal(c=linebuffer[pos1])) pos1--;
      for (pos=pos1;!isdecimal(linebuffer[pos]) && pos<pos2+1;pos++)
         ;
      for (p=0;pos<pos2+1;p++,pos++)
         if (p<20) value[p]=linebuffer[pos];
         else {
            cprintf("\r\nERROR in line %s: number too long: %p\n",
               linebuffer,p);
            getch();
            farewell(pic_no);
         }
      value[p]='\0';
      digit[3-n]= (float)atof(value);
      pos2=pos1;
   }
   if (pos1>0) {
      len=pos1<MAX_LEN?pos1:MAX_LEN;
      linebuffer[len+1]='\0';
      buffer=stripwhites(linebuffer);
      strcpy(testname[column], buffer);
   }
   valid_data0[column]= valid_data[column]= N[column]= (Cutoff_flag)?(int) digit[0]-1:(int) digit[0];
   i25[column]        =digit[1];
   i50[column]        =digit[2];
   i75[column]        =digit[3];
#ifdef TRACE
   if (Trace==2) fprintf(tracefile,"\n%4d %s %s: valid_data[%d]=%d",
          __LINE__,__FILE__, linebuffer, column, valid_data0[column]);
#endif
}

void write_ftt(char *s) {
   int ii, t;
   unsigned int opcode;
   if ((fpout=fopen(s,"wb")) == NULL) return;
   clearerr(fpout);
   fprintf(fpout,"%s\n%s\r\n", FTTCODE, filename);
   for (t=0; test0[t]!=-99 && t<nooftests; t++) fprintf(fpout,"%d\r\n",test0[t]);
   if (Subselect!=-1) {
      opcode=1000*STRING_ARRAY+100*DA_evalstring;
      for (t=0; t<nooftests; t++) if (test[t]!=-1) fprintf(fpout,"%u %s\r\n",opcode+t, evalstring[t]);
      opcode=1000*STRING_ARRAY+100*DA_evaltext;
      for (t=0; t<nooftests; t++) if (test[t]!=-1) fprintf(fpout,"%u %s\r\n",opcode+t, evaltext[t]);
   }
   opcode=1000*STRING_ARRAY+100*DA_extra_text;
   for (t=0; t<10; t++) if (extra_text[t][0]) fprintf(fpout,"%u %s\r\n",opcode+t, extra_text[t]);
   opcode=1000*STRING_ARRAY+100*DA_symbol_legend;
   for (t=0; t<SYMBOLS; t++) if (symbol_in_use[t]) fprintf(fpout,"%u %s\r\n",opcode+t, symbol_legend[t]);
   opcode=1000*STRING_ARRAY+100*DA_testname;
   for (t=0; test0[t]!=-99 && t<nooftests; t++) if (test[t]!=-1) fprintf(fpout,"%u %s\r\n",opcode+test0[t], testname[test0[t]]);
   opcode=1000*STRING_ARRAY+100*DA_testunit;
   for (t=0; test0[t]!=-99 && t<nooftests; t++) if (test[t]!=-1) fprintf(fpout,"%u %s\r\n",opcode+test0[t], testunit[test0[t]]);
   if (Subselect==1) {
      opcode=1000*STRING_ARRAY+100*DA_defaultstring;
      for (t=0; t<6; t++) fprintf(fpout,"%u %s\r\n",opcode+t, defaultstring[t]);
   }
   opcode=1000*STRING_ARRAY+100*DA_pstring;
   for (t=0; t<PSTRING; t++) if (pstart[t]!=-1) fprintf(fpout,"%u %s\r\n",opcode+t, pstring[t]);
   opcode=1000*SINGLE_STRING;
   fprintf(fpout, "%u %s\r\n", opcode+DS_cut_off1,     Cut_off1);
   fprintf(fpout, "%u %s\r\n", opcode+DS_cut_off2,     Cut_off2);
   if (*First_title)  fprintf(fpout, "%u %s\r\n", opcode+DS_first_title,  First_title);
   else fprintf(fpout, "%u %s\r\n", opcode+DS_first_title,  " ");
   if (*Second_title) fprintf(fpout, "%u %s\r\n", opcode+DS_second_title, Second_title);
   else fprintf(fpout, "%u %s\r\n", opcode+DS_second_title, " ");
   fprintf(fpout, "%u %s\r\n", opcode+DS_symbol_string,Symbol_string);
   fprintf(fpout, "%u %s\r\n", opcode+DS_y_axis_title, y_axis_title);
   fprintf(fpout, "%u %s\r\n", opcode+DS_selected_symbolstring, selected_symbolstring);
   fprintf(fpout, "%u %s\r\n", opcode+DS_symbol_color,Symbol_color);
   opcode=1000*INTEG;
   for (ii=0;ii<NOOFINTS;ii++) {
      fprintf(fpout,"%u %d\r\n",opcode+ii, Int_array[ii]);
/*
      if (!(Subselect && ii==DI_subselect)) fprintf(fpout,"%u %d\r\n",opcode+ii, Int_array[ii]);
      else fprintf(fpout,"%u %d\r\n",opcode+ii, 1);
*/
   }
   opcode=1000*FLOATING;
   for (ii=0;ii<NOOFFLOATS;ii++) fprintf(fpout,"%u %.3f\r\n",opcode+ii, Float_array[ii]);
   opcode=1000*INT_ARRAY+100*DIA_filltype;
   for (t=0; t<=FILL_COL; t++) fprintf(fpout,"%u %d\r\n",opcode+t, filltype[t]);
   opcode=1000*INT_ARRAY+100*DIA_pstart;
   for (t=0; t<PSTRING; t++) if (pstart[t]>-1) fprintf(fpout,"%u %d\r\n",opcode+t, pstart[t]);
   opcode=1000*INT_ARRAY+100*DIA_pend;
   for (t=0; t<PSTRING; t++) if (pstart[t]>-1) fprintf(fpout,"%u %d\r\n",opcode+t, pend[t]);
   opcode=1000*FLOAT_ARRAY+100*DFA_fill_min;
   for (t=0; t<=FILL_COL; t++) if (filltype[t]>-1) fprintf(fpout,"%u %.3f\r\n",opcode+t, fill_min[t]);
   opcode=1000*FLOAT_ARRAY+100*DFA_fill_max;
   for (t=0; t<=FILL_COL; t++) if (filltype[t]>-1) fprintf(fpout,"%u %.3f\r\n",opcode+t, fill_max[t]);
   opcode=1000*FLOAT_ARRAY+100*DFA_pvalue;
   for (t=0; t<PSTRING; t++) if (pstart[t]>-1) fprintf(fpout,"%u %.3f\r\n",opcode+t, pvalue[t]);
   fclose(fpout);
}

int strpos2(char *s,char c,int pos) {
/* finds occurrence of c in s starting at pos */
int p;
     if (pos>strlen(s)) cprintf("\r\nerror: position %d in \"%s\" not found\n", pos, s);
     for (p=pos;s[p];p++) if (s[p]==c) return p;
     return -1;
}

char *getfile(char *s) {
    int nooffiles,selected_file;
    int done,row,col,month,day,year,hour,minute,second;
    struct ffblk ffblk;
    for (col=0; col<TEXTCOLUMNS; col++) {
       gotoxy(1+col*SCREENCOLUMNS/TEXTCOLUMNS, SCREENSTARTROW-1);
           cprintf("Filename YY/MM/DD");
        }
    done=findfirst(s,&ffblk,0);
        if (done) return 0;
    for(nooffiles=0,done=0;!done && nooffiles<MAXFILES;nooffiles++) {
        row=SCREENSTARTROW + nooffiles%NOOFLINES;
                col=1+SCREENCOLUMNS/TEXTCOLUMNS*(nooffiles/NOOFLINES);
        gotoxy(col, row);
        strcpy(filenames[nooffiles],ffblk.ff_name);
        maketime(&hour,&minute,&second,ffblk.ff_ftime);
        makedate(&month,&day,&year,ffblk.ff_fdate);
        cprintf("%-8s %2.2d/%2.2d/%2.2d\r\n",strnleft2(buffer, filenames[nooffiles],'.',8), year, month, day);
                done=findnext(&ffblk);
    }
    if (!nooffiles) {
       clrscr();
       cprintf("No format files found\r\nPress RETURN to continue ");
       if (getch()==ESC) farewell(pic_no);
       return NULL;
    }
    selected_file = strmenu(
           0,
           nooffiles,
           SCREENSTARTROW,
           NOOFLINES
    );
    if (selected_file==-1) return NULL;
    strcpy(s,filenames[selected_file]);
    return s;
}

void makedate(int *m,int *d,int *y,unsigned total) {
    *m=(total>>5)&0XF;
    *d=total & 0X1F;
    *y=(total>>9)&0X3F;
    *y+=80;
}

void maketime(int *h,int *m,int *s,unsigned total) {
    *h=(total>>11) & 0X1F;
    *m=(total>>5) & 0X3F;
    *s=total & 0X1F;
}

char *strnleft2(char *buffer, char *s1, char stop, int n) {
   int i=0;
   do buffer[i]=s1[i++];
   while  (i && i<n && s1[i]!=stop);
   buffer[i]='\0';
   return buffer;
}

int strmenu(int current_pos, int rows, int startrow,int nooflines) {
     int pos, key;
     int n, result, row, col, maxnoofitems;
     maxnoofitems=TEXTCOLUMNS*nooflines;
     if (rows>maxnoofitems) rows=maxnoofitems;
     if (rows>MAXFILES) rows=MAXFILES;
     for (n=0;n<rows;n++) startchars[n]=toupper(*(filenames[n]));
     startchars[rows]='\0';
     n = current_pos;
     do {
        row=startrow + n % nooflines;
        col=1+SCREENCOLUMNS/TEXTCOLUMNS*(n/nooflines);
        gotoxy(col, row);
        HLON;
        cprintf("%-8s",strnleft2(buffer,filenames[n],'.',8));
        HLOFF;
        key=getkey(0);
        gotoxy(col, row);
        cprintf("%-8s",strnleft2(buffer,filenames[n],'.',8));
        if (key<=SPACE || key>256) {
           switch (key) {
              case UP:
                   n--;
                   if (n<0) n=rows-1;
                 break;
              case DOWN:
              case SPACE:
                   n++;
                   if (n > rows-1) n=0;
                 break;
              case TAB:
              case RIGHT:
                   if (n<rows-nooflines) n+=nooflines;
                 break;
              case BACKTAB:
              case LEFT:
                   if (n>nooflines-1) n-=nooflines;
                 break;
              case PgUp:
                   n=(n>nooflines-1 && rows>nooflines) ?
                        nooflines:
                        0;
                 break;
              case HOME:
                   n=0;
                 break;
              case PgDn:
                   n=(n>nooflines-1 || nooflines>rows) ?
                        rows-1:
                        nooflines-1;
                   if (n>rows-1) n=rows-1;
                 break;
              case SHIFT_HOME:
                   n=rows-1;
                 break;
                case RETURN:
                case EXEC:
                   result = RETURN;
                 break;
                case ESC:
                case CANCEL:
                   n=-1;
                 break;
              case CTRL_C:
                  farewell(pic_no);
              default:
                  break;
           }
       } else {
               pos=strpos(startchars,toupper(key),n);
               n=(pos<0) ? n : pos;
       }
     }  while (result!=RETURN && n>=0);
   return n;
}

int piccopy(char *source, char *target) {
   int n1, n2;

   while ((fpin=fopen(source,"rb"))== NULL) {
       clrscr();
       cprintf("Cannot open source file \"%s\"\r\nPress any key to continue ",source);
       if (getch()!=RETURN) farewell(-1);
       return 0;
   }
   while ((fpout=fopen(target,"wb"))== NULL) {
       fclose(fpin);
       clrscr();
       cprintf("Cannot open target file \"%s\"\r\nPress any key to continue ",target);
       if (getch()!=RETURN) farewell(-1);
       return 0;
   }
   cprintf("\r\nWriting PIC-file  \"%s\"",target);
   do {
      n1=fread(buffer,1,COPYBUFFERSIZE,fpin);
      if (n1>0) n2=fwrite(buffer,1,n1,fpout);
      if (n2<n1) {
         fputc(END,fpout);
         fclose(fpin);
         fclose(fpout);
         clrscr();
         cprintf("Error writing to target file \"%s\"; disc full?\r\nPress any key to continue ",target);
         if (getch()!=RETURN) farewell(-1);
         return 0;
      }
   } while (n1==COPYBUFFERSIZE);
   fclose(fpin);

   fputc(END,fpout); /* sometimes (for unknown reasons) incomplete copy -> extra END-code added */

   fclose(fpout);
   return 1;
}


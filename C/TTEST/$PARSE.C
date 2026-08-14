/********** PARSER **************************************/

/* evaluate eval-string with parser         */
/* parser based on : Schildt p 555; rca 091287 		*/
/* precedence:						*/
/*	level10	()					*/
/*	level9	! unary-				*/
/*	level8	^					*/
/*	level7	*:%					*/
/*	level6	+-					*/
/*	level5	<>					*/
/*	level4	=					*/
/*	level3	&					*/
/*	level2	|					*/
/*	level1	assignment				*/
/********************************************************/

#include "ttest.h"
/* module $parse.c */

int parse(char *evalstring,int row);

/* local routines: */
void  arith( char o, float *r, float *h);   /* perform arithmetic */
int   check_teststring(char *teststring, int columns);
float find_var(char *s);                   /* find the value of a variable */
void  get_exp(float *result);        /* entry point into parser: get expression */
void  get_token(void);
int   isdelim(char c);
int   isvariable(char c);
int   iswhite(char c);
int   is_in( char ch, char *s);
void  level1(float *result);         /* process an assignment statement */
void  level2(float *result);        /* | (logical OR) */
void  level3(float *result);        /* logical AND  */
void  level4(float *result);        /* level 4: EQ */
void  level5(float *result);        /* level  5:  <> */
void  level6(float *result);        /* add or substract */
void  level7(float *result);        /* multiply or divide */
void  level8(float *result);        /* process an exponent */
void  level9(float *result);        /* unary + or - */
void  level10(float *result);       /* parenthesized expression */
void  primitive (float *result);    /* get actual value of a number */
void  putback(void);             /* return token to its resting place */
void  serror( int error, char c);   /* display a syntax error */
void  toupper_evalstring(char *evalstring, int columns);
void  unary(char o,float *r);
char *unquote(char *s);


enum {DELIMITER=1, LE, NE, GE, VARIABLE, NUMBER};

int parse(char *parsestring, int row) {
    char evalstring[MAX_LEN];
	 float answer;
    int t;
    strcpy(evalstring, parsestring);
    unquote(evalstring);
    vars[0]=row+1;
    vars[1]=rowlabel[row]-'0';
/*
    if (Trace==2) fprintf(tracefile,"\n%4d %3d vars[1]=%d ",
       __LINE__, row, vars[1]);
*/
    for (t=0;t<columns;t++) if (is_in(numbertoletter(t+2), evalstring)) vars[t+2]=data0[row][t];
    *errstring='\0';
    error_status=0;
    evalpointer=evalstring;
    get_exp(&answer);
    #ifdef TRACE
       if (Trace==2 && error_status) fprintf(tracefile,"\nanswer:%.2f error_status=%d",
       answer, error_status);
    #endif
    if (error_status) return -1;
    if (answer==0) return 0;
    return 1;
}

char *unquote(char *s) {
   char resultstring[81], tempstring[81], *ptr;
   ptr=strtok(s,"\"'");
   strcpy(resultstring,s);
   while ((ptr=strtok(NULL,"\"'"))!=NULL) {
      sprintf(tempstring,"%s%d",resultstring, ptr[0]-'0');
      strcpy(resultstring,tempstring);
      ptr=strtok(NULL,"\"'");
      if (ptr) strcat(resultstring,ptr);
   }
   strcpy (s,resultstring);
   return s;
}

/* local routines */
void get_exp(float *result) {        /* entry point into parser: get expression */
    get_token();
	if (!*token) {
        serror(2,SPACE);
		return;
	}
	level2(result);
}

void get_token() {
	char *temp, c;
	temp=token;
    tok_type=0;
    while (iswhite(*evalpointer)) ++evalpointer;
	c=*evalpointer;
	if (is_in(c, "!<>|&+-*/%^=()\r\n")) {
		tok_type=DELIMITER;
        operator_type=0;
        *temp++=*evalpointer++;
		if (*evalpointer=='=') {
            if (c=='!') operator_type=NE;
            else if (c=='<') operator_type=LE;
            else if (c=='>') operator_type=GE;
            *temp++=*evalpointer++;
        }
		if (*evalpointer=='>') {
            if (c=='=') operator_type=GE;
            *temp++=*evalpointer++;
        }
		if (*evalpointer=='<') {
            if (c=='=') operator_type=LE;
            *temp++=*evalpointer++;
        }
	}
	else if (isvariable(*evalpointer)) {
		while (!isdelim(*evalpointer)) {
			*temp++ = *evalpointer++;
		}
		tok_type=VARIABLE;
	}
	else if (isdigit(*evalpointer)) {
		while (!isdelim(*evalpointer)) *temp++ = *evalpointer++;
		tok_type=NUMBER;
	}
	*temp='\0';
}

void level1(float *result) {     /* process an assignment statement */
	int slot, ttok_type;
	char temp_token[80];
	if (tok_type==VARIABLE) {
		/* save old token */
		strcpy(temp_token,token);
		ttok_type=tok_type;
		slot=lettertonumber(*token)+1;
		get_token();
		if (*token != '=') {
			putback();
			strcpy(token,temp_token);
			tok_type = ttok_type;
		}
		else {
			get_token();
			level2(result);
			vars[slot]=*result;
			return;
		}
	}
	level2(result);
}

void level2(float *result) {        /* | (logical OR)  */
	float hold;
    level3(result);
	while(*token=='|') {
		get_token();
		level3(&hold);
		arith('|',result,&hold);
	}
}

void level3(float *result) {        /* logical AND */
	float hold;
    level4(result);
	while(*token=='&') {
		get_token();
		level4(&hold);
		arith('&',result,&hold);
	}
}

void level4(float *result) {        /* level 4: EQ */
	float hold;
    level5(result);
	while(*token=='=') {
		get_token();
		level5(&hold);
		arith('=',result,&hold);
	}
}

void level5(float *result) {        /* level  5:  <> */
	register char op;
	float hold;
    level6(result);
	while((op=*token)=='<' || op=='>') {
		get_token();
		level6(&hold);
		arith(op,result,&hold);
	}
}

void level6(float *result) {        /*add or substract */
	register char op;
	float hold;
    level7(result);
	while((op=*token)=='+' || op=='-') {
		get_token();
		level7(&hold);
		arith(op,result,&hold);
	}
}

void level7(float *result) {        /* multiply or divide */
    register char op;
	float hold;
    level8(result);
	while((op=*token)=='*' || op=='/' || op == '%') {
		get_token();
		level8(&hold);
		arith(op,result,&hold);
	}
}

void level8(float *result) {        /* process an exponent */
	float hold;
    level9( result);
	if (*token=='^') {
		get_token();
		level8(&hold);
		arith('^',result,&hold);
	}
}

void level9(float *result) {        /* unary + or - */
	register char op;
	op=0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-' || *token == '!') {
		op = *token;
		get_token();
	}
	level10(result);
	if (op) unary(op,result);
}

void level10(float *result) {       /* parenthesized expression */
    if ((*token == '(' )&& (tok_type == DELIMITER)) {
		get_token();
		level1(result);
        if (*token != ')' ) serror(1,SPACE);
		get_token();
	}
	else primitive(result);
}

void primitive (float *result) {    /* get actual value of a number */
    switch (tok_type) {
		case VARIABLE:
			*result=find_var(token);
			get_token();
			return;
		case NUMBER:
			*result=atof(token);
			get_token();
			return;
		default:
            serror(0,SPACE);
	}
}

void arith( char o, float *r, float *h) {   /* perform arithmetic */
	register int ex,t;
/*
if (Trace==2) fprintf(tracefile,"optype=%d; (%.2f %c %.2f) =", operator_type, *r, o, *h);
*/
	switch (o) {
        case '-': { (*r) = (*r) - (*h); break; }
        case '+': { (*r) = (*r) + (*h); break; }
        case '*': { (*r) = (*r) * (*h); break; }
        case '/': { (*r) = (*r) / (*h); break; }
        case '%': { (*r) = (float)((int) (*r) % (int)(*h)); break; }
        case '^': { ex=*r; if (*h==0) { *r=1; break; } for (t=*h-1;t>0;--t) *r=(*r)* ex;}
        case '>': { if (operator_type==GE) *r=((*r)>=(*h)) ? 1:0; else *r=(*r > *h) ? 1:0; break; }
        case '<': { if (operator_type==LE) *r=((*r)<=(*h)) ? 1:0; else *r=(*r < *h) ? 1:0; break; }
        case '&': { *r=( ((*r) && (*h)) ) ? 1:0; break; }
        case '|': { *r=((int)(*r)|((int)*h)) ? 1:0; break; }
        case '=': { if (operator_type==LE) *r=((*r)<=(*h)) ? 1:0;
               else if (operator_type==NE) *r=((*r)!=(*h)) ? 1:0;
               else if (operator_type==GE) *r=((*r)>=(*h)) ? 1:0;
               else *r=((*r)==(*h)) ? 1:0;  break;}
	}
/*
    if (Trace==2) fprintf(tracefile,"%.2f", *r);
*/
}

void unary(char o,float *r) {
	if (o== '-') *r = -(*r);
	if (o== '!') *r =(*r) ? 0 : 1;
}

void putback() {        /* return token to its resting place */
	char *t;
	t=token;
	for (;*t;t++) evalpointer--;
}

float find_var(char *s) {   /* find the value of a variable */
    if (!isvariable(*s)) { serror(1,SPACE); return 0; }
	return vars[lettertonumber(*token)];
}

void serror( int error, char c) {   /* display a syntax error */
    switch(error) {
     case 0:
       strcpy(errstring,"syntax error ... ");
       break;
     case 1:
       strcpy(errstring,"unbalanced parenthese ");
       break;
     case 2:
       strcpy(errstring,"no expression present ");
       break;
     case 3:
       sprintf(errstring, "non-existing test %c\n",c);
       break;
   }
   error_status=1;
}

int iswhite(char c) {
	if (c==' ' || c==TAB) return 1;
	return 0;
}

int isdelim(char c) {
	if (is_in( c, " <>!&|+-*/%^=()") || c==TAB || c=='\r' || c==0) return 1;
	return 0;
}

int is_in( char ch, char *s) {
	while (*s) if (*s++==ch) return 1;
	return 0;
}

int isvariable(char c) {
	if (isalpha(c)|| (c=='@')) return 1;
	return 0;
}


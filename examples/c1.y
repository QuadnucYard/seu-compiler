
%union{   
    float val;
    int dval;
    char sval[20];
}

%token  /* 3456  */<val> NUM <dval> VARIABLE//56475
%left    '+'/*666*/'-' /* 3456  */
%left    '*' '/' // 7777

%start program

/* %token    
 OR  300   "||"    
  LPAREN "("    
     <val>   
       RPAREN ")"  */

%type <val> expr

    
%{
	
/*for Visual studio */
/*	#define  __STDC__   0   */   
    #include "c1.y.tab.h"
	#include <stdio.h>   
    #include <string.h>
    #include <stdlib.h>
    void yyerror(char*);
    int yylex(void);
	
    char yytext[100];
    int sym[26];

    YYSTYPE yylval;
/**
%} 
 */


%}



%%

program:
    program statement '\n'
    |%empty
    ;
statement:
     expr    {printf("%f\n", $1);}
     |VARIABLE '=' expr    {sym[$1] = $3;}
     ;
expr:
    NUM { $$ = yylval.val; }
    |VARIABLE{$$ = sym[$1];}
    |expr '+' expr    {$$ = $1 + $3;}
    |expr '-' expr    {$$ = $1 - $3;}
    |expr '*' expr    {$$ = $1 * $3;}
    |expr '/' expr    {$$ = $1 / $3;}
    |'-' expr %prec '*' {$$ = - $2;}
    |'('expr')'    {$$ = $2;}
    ;
	

%%

int yylex() {
    static int tokens[] = {256, '+', 256, '*', 256, '\n', 0};
	static const char* texts[] = {"6.8", "+", "7.5", "*", "-3", "\n", 0};
	static int cnt = 0;
	if (texts[cnt])
		strcpy(yytext, texts[cnt]);
    if (tokens[cnt] == 256)
		yylval.val = atof(texts[cnt]);
    return tokens[cnt++];
}

void yyerror(char* s)
{
    fprintf(stderr, "%s\n", s);
}

int main() {
	parse();
	return 0;
}
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	int yylex();
	char yytext[100];

#define YYSTYPE_IS_DECLARED
typedef float YYSTYPE; 

YYSTYPE yylval;

%}


%token NUM

%start input

%%
input
	: line	{ printf("acc!\n"); }
	;

line
	: '\n'
	| expr '\n'  { printf ("expr %f\n", $1); }
	;

expr
	: expr '+' term { $$ = $1 + $3; printf("%f = %f + %f\n",$$,$1,$3); }
	| expr '-' term { $$ = $1 - $3; printf("%f = %f - %f\n",$$,$1,$3); }
	| term          { $$ = $1; } 
	;

term
	: term '*' fact { $$ = $1 * $3; printf("%f = %f * %f\n",$$,$1,$3); }
	| term '/' fact { $$ = $1 / $3; printf("%f = %f / %f\n",$$,$1,$3); }
	| fact			{ $$ = $1; } 
	;

fact
	: NUM			{ $$ = yylval.val; }
	| '(' expr ')'  { $$ = $2; }
	;

%%

int yylex() {
    static int tokens[] = {256, '+', 256, '*', 256, '\n', 0};
	static const char* texts[] = {"6.8", "+", "7.5", "*", "-3", "\n", 0};
	static int cnt = 0;
	if (texts[cnt])
		strcpy(yytext, texts[cnt]);
	if (tokens[cnt] == 256)
		yylval = atof(texts[cnt]);
	printf("yylex\n");
    return tokens[cnt++];
}

int main() {
	parse();
	return 0;
}
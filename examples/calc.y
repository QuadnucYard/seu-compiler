%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	int yylex();

#define YYSTYPE_IS_DECLARED
typedef float YYSTYPE; 

YYSTYPE yylval;

%}

%error-verbose

%token NUM

%start input

%%
input
	: line	{ printf("acc!\n"); }
	;

line
	: '#'
	| expr '#'  { printf ("expr %f\n", $1); }
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
	: NUM			{ $$ = yylval; printf("num %f\n", $1); }
	| '(' expr ')'  { $$ = $2; }
	;

%%

int main() {
	parse();
	return 0;
}
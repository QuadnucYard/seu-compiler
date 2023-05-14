%{
	#include <stdio.h>
	#include <stdlib.h>
	extern char yytext[];
	extern int column,line,prevCol;
	int printed = 0;
	void yyerror(char const *s);
	int yylex();
%}

%error-verbose

%token ID

%start S

%%

S : L '=' R | R ;
L : '*' R | ID ;
R : L ;

%%


void yyerror(char const *s)
{
	printf("Error in line %d column %d : ", line,prevCol);
	printf("\"%s\"\n",s);
	printed = 1;
	exit (3);
}

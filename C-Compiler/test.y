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

S : L '=' R { $$ = $1 + $3; puts("S -> L '=' R"); } 
| R  { puts("S -> R"); } ;
L : '*' R   { puts("L -> '*' R"); } 
  | ID  { puts("L -> ID"); }  ;
R : L  { puts("R -> L"); }  ;

%%

int yylex() {
	static int tokens[] = {256, '=', '*', 256, 0};
	static int cnt = 0;
	return tokens[cnt++];
}

void yyerror(char const *s)
{
	printf("Error in line %d column %d : ", line,prevCol);
	printf("\"%s\"\n",s);
	printed = 1;
	exit (3);
}

int main() {
	parse();
	return 0;
}
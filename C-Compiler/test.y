%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	char yytext[100];
	int column,line,prevCol;
	int printed = 0;
	void yyerror(char const *s);
	int yylex();
%}



%token ID

%start S

%%

S : L '=' R { $$ = $1 + $3; puts("S -> L '=' R"); } 
| R  { puts("S -> R"); } ;
L : '*' R   { puts("L -> '*' R"); } 
  | ID  { puts("L -> ID"); }  ;
R : L  { puts("R -> L"); }  ;

%%
/* ID = * ID */
int yylex() {
	static int tokens[] = {256, '=', '*', 256, 0};
	static const char* texts[] = {"aa", "=", "*", "bb", 0};
	static int cnt = 0;
	if (texts[cnt])
		strcpy(yytext, texts[cnt]);
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
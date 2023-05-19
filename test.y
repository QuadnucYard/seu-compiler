%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	char yytext[100];
	int column,line,prevCol;
	int printed = 0;
	int yylval;
	void yyerror(char const *s);
	int yylex();
#define YYSTYPE_IS_DECLARED
typedef float YYSTYPE; 

%}

%error-verbose

%token NUM 

%start input



%%
input :  line	{ printf("acc!\n"); }
;

line :
  '\n'
| expr '\n'  { printf ("expr %f\n", $1); }

;

expr :
  expr '+' term { $$ = $1 + $3; printf("%d = %d + %d\n",$$,$1,$3); }
| expr '-' term { $$ = $1 - $3; printf("%d = %d - %d\n",$$,$1,$3); }
| term          { $$ = $1; } 
;

term :
  term '*' fact { $$ = $1 * $3; printf("%d = %d * %d\n",$$,$1,$3); }
| term '/' fact { $$ = $1 / $3; printf("%d = %d / %d\n",$$,$1,$3); }
| fact			{ $$ = $1; } 
;

fact :
    NUM			{ $$ = yylval; printf("num %d\n", $1); }
| '(' expr ')'  { $$ = $2; }
;

%%

int yylex() {
	static int tokens[] = {'(',256, '*', 256, '+', 256,')','*',256,'\n', 0};
	static const char* texts[] = {"(","10", "*", "5", "+", "5",")","*","2","\n", 0};
	static int cnt = 0;
	if (texts[cnt])
		strcpy(yytext, texts[cnt]);
	if (tokens[cnt] == 256) {
		yylval = atoi(yytext);
	}
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
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>
	
	char yytext[100];
	int column,line,prevCol;
	int printed = 0;
	
	void yyerror(char const *s);
	int yylex();
	
	typedef struct{
		int len;
		float val;
	} YYSTYPE; 
	#include "y.tab.h"
	YYSTYPE yylval;

	
%}

%error-verbose

%token NUM0 NUM1 

%start Start



%%
Start :  S	{ printf("acc! The result is %f \n",$1.val); }
;

S : 
  L '.' L  { $$.val = $1.val + $3.val*pow(2,-$3.len); printf ("%f = %f . %f", $$.val, $1.val, $2.val); }
| L		   { $$.val = $1.val;   printf ("%f = %f", $$.val, $1.val); }
;

L :    
  L B { $$.val = $1.val*2 + $2.val; $$.len=$1.len+1; printf("L = L B(%f = %f)\n",$$.val,$1.val,$2.val); }
| B  { $$.val = $1.val; $$.len=1; printf("L = B(%f = %f)\n",$$.val,$1.val); }
;

B :
  NUM0 { $$.val = 0; printf("B = NUM0(%f = 0)\n",$$.val); }
| NUM1 { $$.val = 1; printf("B = NUM1(%f = 1)\n",$$.val); }
;

%%

int yylex() {
	static int tokens[] = {257, '.', 257, 0};
	static const char* texts[] = {"1", ".", "1", 0};
	static int cnt = 0;
	if (texts[cnt])
		strcpy(yytext, texts[cnt]);
	if (tokens[cnt] == NUM0||tokens[cnt] == NUM1) {
		yylval.val = atoi(yytext);
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
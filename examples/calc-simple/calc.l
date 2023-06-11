%{
#include <stdio.h>
#include <string.h>

#define YYSTYPE_IS_DECLARED
typedef float YYSTYPE; 

#include "calc.y.tab.h"
%}

D			[0-9]
E			([Ee][+-]?{D}+)

%%

[1-9]{D}*				{ yylval = atoi(yytext); return(NUM); }

{D}+{E}					{ yylval = atof(yytext); return(NUM); }
{D}*"."{D}+{E}?			{ yylval = atof(yytext); return(NUM); }
{D}+"."{D}*{E}?			{ yylval = atof(yytext); return(NUM); }

"-"								{ return('-'); }
"+"								{ return('+'); }
"*"								{ return('*'); }
"/"								{ return('/'); }
"\n"							{ return('\n'); }
"("							    { return('('); }
")"							    { return(')'); }

[ \t\v\f]						{ }
.								{ }


%%

int yywrap() { 
    return 1;
}
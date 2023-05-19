%{
unsigned tokenCount = 0;
%}
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			([Ee][+-]?{D}+)
P                       ([Pp][+-]?{D}+)
FS			(f|F|l|L)
IS                      ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))


%%


L?\"(\\.|[^\\"\n])*\"			

"..."							 
">>="							 
"<<="							 
"+="							 
"-="							 
"*="							 
"/="							 
"%="							 
"&="							 
"^="							 
"|="							 
">>"							 
"<<"							 
"++"							 
"--"							 
"->"							 
"&&"							 
"||"							 
"<="							 
">="							 
"=="							 
"!="							 
";"								 
("{"|"<%")						 
("}"|"%>")						 
","								 
":"								 
"="								 
"("								 
")"								 
("["|"<:")						 
("]"|":>")						 
"."								 
"&"								 
"!"								 
"~"								 
"-"								 
"+"								 
"*"								 
"/"								 
"%"								 
"<"								 
">"								 
"^"								 
"|"								 
"?"								 
{L}({L}|{D})*					{ tokenCount++; }

[ \t\v\n\f]						{  }
.								{ /* Add code to complain about unmatched characters */ }

%%

int main(int argc, char* argv[]) {
    freopen("E:/Project/seu-compiler/C-Compiler/test.c", "r", stdin);
    yylex();
    printf("tokencount: %d",tokenCount);
    return 0;
}

int yywrap() { 
    return 1;
}
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			([Ee][+-]?{D}+)
P                       ([Pp][+-]?{D}+)
FS			(f|F|l|L)
IS                      ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))

%{
#include <stdio.h>
#include "y.tab.h"

void lineColumn(void);
void multiLineComment(void);
%}
%%

"/*"							{ multiLineComment(); }
"//"[^\n]*              		{ /* consume //-comment */ }


"auto"							{ printf("|%s| %d\n", yytext, yy_act); }
"_Bool"							{ printf("|%s| %d\n", yytext, yy_act); }
"break"							{ printf("|%s| %d\n", yytext, yy_act); }
"case"							{ printf("|%s| %d\n", yytext, yy_act); }
"char"							{ printf("|%s| %d\n", yytext, yy_act); }
"_Complex"						{ printf("|%s| %d\n", yytext, yy_act); }
"const"							{ printf("|%s| %d\n", yytext, yy_act); }
"continue"						{ printf("|%s| %d\n", yytext, yy_act); }
"default"						{ printf("|%s| %d\n", yytext, yy_act); }
"do"							{ printf("|%s| %d\n", yytext, yy_act); }
"double"						{ printf("|%s| %d\n", yytext, yy_act); }
"else"							{ printf("|%s| %d\n", yytext, yy_act); }
"enum"							{ printf("|%s| %d\n", yytext, yy_act); }
"extern"						{ printf("|%s| %d\n", yytext, yy_act); }
"float"							{ printf("|%s| %d\n", yytext, yy_act); }
"for"							{ printf("|%s| %d\n", yytext, yy_act); }
"goto"							{ printf("|%s| %d\n", yytext, yy_act); }
"if"							{ printf("|%s| %d\n", yytext, yy_act); }
"_Imaginary"					{ printf("|%s| %d\n", yytext, yy_act); }
"#include"						{ printf("|%s| %d\n", yytext, yy_act); }
"inline"						{ printf("|%s| %d\n", yytext, yy_act); }
"int"							{ printf("|%s| %d\n", yytext, yy_act); }
"long"							{ printf("|%s| %d\n", yytext, yy_act); }
"register"						{ printf("|%s| %d\n", yytext, yy_act); }
"restrict"						{ printf("|%s| %d\n", yytext, yy_act); }
"return"						{ printf("|%s| %d\n", yytext, yy_act); }
"short"							{ printf("|%s| %d\n", yytext, yy_act); }
"signed"						{ printf("|%s| %d\n", yytext, yy_act); }
"sizeof"						{ printf("|%s| %d\n", yytext, yy_act); }
"static"						{ printf("|%s| %d\n", yytext, yy_act); }
"struct"						{ printf("|%s| %d\n", yytext, yy_act); }
"switch"						{ printf("|%s| %d\n", yytext, yy_act); }
"typedef"						{ printf("|%s| %d\n", yytext, yy_act); }
"union"							{ printf("|%s| %d\n", yytext, yy_act); }
"unsigned"						{ printf("|%s| %d\n", yytext, yy_act); }
"void"							{ printf("|%s| %d\n", yytext, yy_act); }
"volatile"						{ printf("|%s| %d\n", yytext, yy_act); }
"while"							{ printf("|%s| %d\n", yytext, yy_act); }

{L}({L}|{D})*					{ printf("|%s| %d\n", yytext, yy_act); }

0[xX]{H}+{IS}?					{ printf("|%s| %d\n", yytext, yy_act); }
0[0-7]*{IS}?					{ printf("|%s| %d\n", yytext, yy_act); }
[1-9]{D}*{IS}?					{ printf("|%s| %d\n", yytext, yy_act); }
L?'(\\.|[^\\'\n])+'				{ printf("|%s| %d\n", yytext, yy_act); }

{D}+{E}{FS}?					{ printf("|%s| %d\n", yytext, yy_act); }
{D}*"."{D}+{E}?{FS}?			{ printf("|%s| %d\n", yytext, yy_act); }
{D}+"."{D}*{E}?{FS}?			{ printf("|%s| %d\n", yytext, yy_act); }
0[xX]{H}+{P}{FS}?				{ printf("|%s| %d\n", yytext, yy_act); }
0[xX]{H}*"."{H}+{P}?{FS}?     	{ printf("|%s| %d\n", yytext, yy_act); }
0[xX]{H}+"."{H}*{P}?{FS}?     	{ printf("|%s| %d\n", yytext, yy_act); }


L?\"(\\.|[^\\"\n])*\"			{ printf("|%s| %d\n", yytext, yy_act); }

"..."							{ printf("|%s| %d\n", yytext, yy_act); }
">>="							{ printf("|%s| %d\n", yytext, yy_act); }
"<<="							{ printf("|%s| %d\n", yytext, yy_act); }
"+="							{ printf("|%s| %d\n", yytext, yy_act); }
"-="							{ printf("|%s| %d\n", yytext, yy_act); }
"*="							{ printf("|%s| %d\n", yytext, yy_act); }
"/="							{ printf("|%s| %d\n", yytext, yy_act); }
"%="							{ printf("|%s| %d\n", yytext, yy_act); }
"&="							{ printf("|%s| %d\n", yytext, yy_act); }
"^="							{ printf("|%s| %d\n", yytext, yy_act); }
"|="							{ printf("|%s| %d\n", yytext, yy_act); }
">>"							{ printf("|%s| %d\n", yytext, yy_act); }
"<<"							{ printf("|%s| %d\n", yytext, yy_act); }
"++"							{ printf("|%s| %d\n", yytext, yy_act); }
"--"							{ printf("|%s| %d\n", yytext, yy_act); }
"->"							{ printf("|%s| %d\n", yytext, yy_act); }
"&&"							{ printf("|%s| %d\n", yytext, yy_act); }
"||"							{ printf("|%s| %d\n", yytext, yy_act); }
"<="							{ printf("|%s| %d\n", yytext, yy_act); }
">="							{ printf("|%s| %d\n", yytext, yy_act); }
"=="							{ printf("|%s| %d\n", yytext, yy_act); }
"!="							{ printf("|%s| %d\n", yytext, yy_act); }
";"								{ printf("|%s| %d\n", yytext, yy_act); }
("{"|"<%")						{ printf("|%s| %d\n", yytext, yy_act); }
("}"|"%>")						{ printf("|%s| %d\n", yytext, yy_act); }
","								{ printf("|%s| %d\n", yytext, yy_act); }
":"								{ printf("|%s| %d\n", yytext, yy_act); }
"="								{ printf("|%s| %d\n", yytext, yy_act); }
"("								{ printf("|%s| %d\n", yytext, yy_act); }
")"								{ printf("|%s| %d\n", yytext, yy_act); }
("["|"<:")						{ printf("|%s| %d\n", yytext, yy_act); }
("]"|":>")						{ printf("|%s| %d\n", yytext, yy_act); }
"."								{ printf("|%s| %d\n", yytext, yy_act); }
"&"								{ printf("|%s| %d\n", yytext, yy_act); }
"!"								{ printf("|%s| %d\n", yytext, yy_act); }
"~"								{ printf("|%s| %d\n", yytext, yy_act); }
"-"								{ printf("|%s| %d\n", yytext, yy_act); }
"+"								{ printf("|%s| %d\n", yytext, yy_act); }
"*"								{ printf("|%s| %d\n", yytext, yy_act); }
"/"								{ printf("|%s| %d\n", yytext, yy_act); }
"%"								{ printf("|%s| %d\n", yytext, yy_act); }
"<"								{ printf("|%s| %d\n", yytext, yy_act); }
">"								{ printf("|%s| %d\n", yytext, yy_act); }
"^"								{ printf("|%s| %d\n", yytext, yy_act); }
"|"								{ printf("|%s| %d\n", yytext, yy_act); }
"?"								{ printf("|%s| %d\n", yytext, yy_act); }

[ \t\v\n\f]						{ lineColumn(); }
.								{ /* Add code to complain about unmatched characters */ }

%%

int main(int argc, char* argv[]) {
    freopen("E:/Project/seu-compiler/C-Compiler/test.c", "r", stdin);
    yylex();
    return 0;
}

int yywrap() { 
    return 1;
}

int column = 1;
int prevCol = 1;
int line = 1;

void multiLineComment(void)
{
	char c, prev = 0;

	int startLine = line, startCol = prevCol;
	while ((c = input()) != 0)      /* (EOF maps to 0) */
	{
		if (c == '/' && prev == '*')
			return;
		prev = c;
		prevCol = column;
		if (c == '\n'){
			column = 1;
			line++;
		}
		else if (c == '\t')
		{
			column += 4;
		}
		else
		{
			column++;
		}
	}
	printf("Error in line %d column %d : unterminated comment\n", startLine,startCol);
	exit (2);
}



void lineColumn(void)
{
	int i;
	for (i = 0; yytext[i] != '\0'; i++)
	{
		prevCol = column;
		if (yytext[i] == '\n'){
			column = 1;
			line++;
		}
		else if (yytext[i] == '\t')
		{
			column += 4;
		}
		else
		{
			column++;
		}
	}

}
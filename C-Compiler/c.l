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


"auto"							{ lineColumn(); return(AUTO); }
"_Bool"							{ lineColumn(); return(BOOL); }
"break"							{ lineColumn(); return(BREAK); }
"case"							{ lineColumn(); return(CASE); }
"char"							{ lineColumn(); return(CHAR); }
"_Complex"						{ lineColumn(); return(COMPLEX); }
"const"							{ lineColumn(); return(CONST); }
"continue"						{ lineColumn(); return(CONTINUE); }
"default"						{ lineColumn(); return(DEFAULT); }
"do"							{ lineColumn(); return(DO); }
"double"						{ lineColumn(); return(DOUBLE); }
"else"							{ lineColumn(); return(ELSE); }
"enum"							{ lineColumn(); return(ENUM); }
"extern"						{ lineColumn(); return(EXTERN); }
"float"							{ lineColumn(); return(FLOAT); }
"for"							{ lineColumn(); return(FOR); }
"goto"							{ lineColumn(); return(GOTO); }
"if"							{ lineColumn(); return(IF); }
"_Imaginary"					{ lineColumn(); return(IMAGINARY); }
"#include"						{ lineColumn(); return(HINCLUDE); }
"inline"						{ lineColumn(); return(INLINE); }
"int"							{ lineColumn(); return(INT); }
"long"							{ lineColumn(); return(LONG); }
"register"						{ lineColumn(); return(REGISTER); }
"restrict"						{ lineColumn(); return(RESTRICT); }
"return"						{ lineColumn(); return(RETURN); }
"short"							{ lineColumn(); return(SHORT); }
"signed"						{ lineColumn(); return(SIGNED); }
"sizeof"						{ lineColumn(); return(SIZEOF); }
"static"						{ lineColumn(); return(STATIC); }
"struct"						{ lineColumn(); return(STRUCT); }
"switch"						{ lineColumn(); return(SWITCH); }
"typedef"						{ lineColumn(); return(TYPEDEF); }
"union"							{ lineColumn(); return(UNION); }
"unsigned"						{ lineColumn(); return(UNSIGNED); }
"void"							{ lineColumn(); return(VOID); }
"volatile"						{ lineColumn(); return(VOLATILE); }
"while"							{ lineColumn(); return(WHILE); }

{L}({L}|{D})*					{ lineColumn(); return(IDENTIFIER); }

0[xX]{H}+{IS}?					{ lineColumn(); return(CONSTANT); }
0[0-7]*{IS}?					{ lineColumn(); return(CONSTANT); }
[1-9]{D}*{IS}?					{ lineColumn(); return(CONSTANT); }
L?'(\\.|[^\\'\n])+'				{ lineColumn(); return(CONSTANT); }

{D}+{E}{FS}?					{ lineColumn(); return(CONSTANT); }
{D}*"."{D}+{E}?{FS}?			{ lineColumn(); return(CONSTANT); }
{D}+"."{D}*{E}?{FS}?			{ lineColumn(); return(CONSTANT); }
0[xX]{H}+{P}{FS}?				{ lineColumn(); return(CONSTANT); }
0[xX]{H}*"."{H}+{P}?{FS}?     	{ lineColumn(); return(CONSTANT); }
0[xX]{H}+"."{H}*{P}?{FS}?     	{ lineColumn(); return(CONSTANT); }


L?\"(\\.|[^\\"\n])*\"			{ lineColumn(); return(STRING_LITERAL); }

"..."							{ lineColumn(); return(ELLIPSIS); }
">>="							{ lineColumn(); return(RIGHT_ASSIGN); }
"<<="							{ lineColumn(); return(LEFT_ASSIGN); }
"+="							{ lineColumn(); return(ADD_ASSIGN); }
"-="							{ lineColumn(); return(SUB_ASSIGN); }
"*="							{ lineColumn(); return(MUL_ASSIGN); }
"/="							{ lineColumn(); return(DIV_ASSIGN); }
"%="							{ lineColumn(); return(MOD_ASSIGN); }
"&="							{ lineColumn(); return(AND_ASSIGN); }
"^="							{ lineColumn(); return(XOR_ASSIGN); }
"|="							{ lineColumn(); return(OR_ASSIGN); }
">>"							{ lineColumn(); return(RIGHT_OP); }
"<<"							{ lineColumn(); return(LEFT_OP); }
"++"							{ lineColumn(); return(INC_OP); }
"--"							{ lineColumn(); return(DEC_OP); }
"->"							{ lineColumn(); return(PTR_OP); }
"&&"							{ lineColumn(); return(AND_OP); }
"||"							{ lineColumn(); return(OR_OP); }
"<="							{ lineColumn(); return(LE_OP); }
">="							{ lineColumn(); return(GE_OP); }
"=="							{ lineColumn(); return(EQ_OP); }
"!="							{ lineColumn(); return(NE_OP); }
";"								{ lineColumn(); return(';'); }
("{"|"<%")						{ lineColumn(); return('{'); }
("}"|"%>")						{ lineColumn(); return('}'); }
","								{ lineColumn(); return(','); }
":"								{ lineColumn(); return(':'); }
"="								{ lineColumn(); return('='); }
"("								{ lineColumn(); return('('); }
")"								{ lineColumn(); return(')'); }
("["|"<:")						{ lineColumn(); return('['); }
("]"|":>")						{ lineColumn(); return(']'); }
"."								{ lineColumn(); return('.'); }
"&"								{ lineColumn(); return('&'); }
"!"								{ lineColumn(); return('!'); }
"~"								{ lineColumn(); return('~'); }
"-"								{ lineColumn(); return('-'); }
"+"								{ lineColumn(); return('+'); }
"*"								{ lineColumn(); return('*'); }
"/"								{ lineColumn(); return('/'); }
"%"								{ lineColumn(); return('%'); }
"<"								{ lineColumn(); return('<'); }
">"								{ lineColumn(); return('>'); }
"^"								{ lineColumn(); return('^'); }
"|"								{ lineColumn(); return('|'); }
"?"								{ lineColumn(); return('?'); }

[ \t\v\n\f]						{ lineColumn(); }
.								{ /* Add code to complain about unmatched characters */ }

%%

int yywrap(void)
{
	return 1;
}

int main (void) {
	yyparse ( );
	return 0;
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
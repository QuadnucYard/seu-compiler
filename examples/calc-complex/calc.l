D			[0-9]
E			([Ee][+-]?{D}+)

%{
    /*
     *  一个简单计算器的Lex词法文件
     */
	void yyerror(char*);
	#include "calc.y.tab.h"
%}

%%

     /* a-z为变量 */   
[a-z]	{
            yylval.val = *yytext - 'a';
            return VARIABLE;
    	}

    /* 整数 */
[1-9]{D}*	{
            yylval.val = atoi(yytext);
            return NUM;
    	}

    /* 浮点数 */
{D}+{E} |	 	
    {D}*"."{D}+{E}? |
    {D}+"."{D}*{E}?
        {
            yylval.val = atof(yytext);
            return NUM;
    	}

    /* 运算符 */
[-+()=/*\n]	{return *yytext;}

"sin" { return SIN; }
"cos" { return COS; }
"tan" { return TAN; }
"abs" { return ABS; }

    /* 空白被忽略 */
[ \t]    ;

    /* 其他字符都是非法的 */
.    yyerror("无效的输入字符");

%%

int yywrap() {
    return 1;
}
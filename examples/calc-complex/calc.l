
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
[0-9]+	{
            yylval.val = atoi(yytext);
            return NUM;
    	}

    /* 运算符 */
[-+()=/*\n]	{return *yytext;}

    /* 空白被忽略 */
[ \t]    ;

    /* 其他字符都是非法的 */
.    yyerror("无效的输入字符");

%%

int yywrap() {
    return 1;
}
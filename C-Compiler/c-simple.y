%{
	#include <stdio.h>
	#include <stdlib.h>
	char yytext[];
	int column,line,prevCol;
	int printed = 0;
	void yyerror(char const *s);
	int yylex();

#define YYSTYPE_IS_DECLARED

typedef struct {
	char name[32];
	int type;
	int addr;
} Variable;

typedef struct {
	Variable vars[10];
	int nvars;
} Env;

Env env;

typedef union {
	struct {
		int truelist;
		int falselist;
	} be;
	struct {
		int nextlist;
	} cf;
	struct {
		int addr;
	} expr;
	struct {
		lexeme;
	} var;
	int instr;
} YYSTYPE;

extern union {
	int d;
	float f;
	char s[32];
} yylval;

int nextinstr = 100;

int put_var(char* name) {

}

int get_var(char* name) {
	for (int i = 0; i < env.nvars; i++) {
		if (strcmp(name, env.vars[i].name))
			return env.vars[i].addr;
	}
	return 0;
}

int new_temp() {
	static int next_addr = 100;
	next_addr += 4;
	return next_addr - 4;
}

%}

%error-verbose

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token HINCLUDE

%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE RESTRICT
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token BOOL COMPLEX IMAGINARY
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token TRUE FALSE

%start P


%%

P 
	: D
	| S
	;

D
	: T IDENTIFIER ';' D
	| 
	;

T
	: INT
	| FLOAT
	;

S
	: IDENTIFIER '=' E
	| IF '(' B ')' S
	| WHILE '(' B ')' S
	;

L
	: L S
	| S
	;

E
	: E '+' E { $$.expr.addr = new_temp(); printf("%d + %d\n", $1.expr.addr, $3.expr.addr); nextinstr++; }
	| E '-' E { $$.expr.addr = new_temp(); printf("%d - %d\n", $1.expr.addr, $3.expr.addr); nextinstr++; }
	| '-' E { $$.expr.addr = new_temp(); printf("- %d\n", $2.expr.addr); nextinstr++; }
	| '(' E ')' { $$.expr.addr = $1.expr.addr; }
	| IDENTIFIER { $$.expr.addr = get_var(); printf("> %s\n", "id"); }
	;

B
	: TRUE { $$.be.truelist = nextinstr; printf("goto _\n"); nextinstr++; }
	| FALSE { $$.be.falselist = nextinstr; printf("goto _\n"); nextinstr++; }
	| E '&' E { $$.be.truelist = nextinstr; $$.be.falselist = nextinstr + 1; printf("if %d & %d goto _\n", $1.expr.addr, $3.expr.addr); nextinstr++; printf("goto _\n"); nextinstr++; }
	| '!' B { $$.be.truelist = $2.be.falselist; $$.be.falselist = $2.be.truelist; }
	| '(' B ')' { $$.be.truelist = $2.be.truelist; $$.be.falselist = $2.be.falselist; }
	| B OR_OP M B
	| B AND_OP M B
	;

M
	: { $$.instr = nextinstr; }
	;

%%



void yyerror(char const *s)
{
	printf("Error in line %d column %d : ", line,prevCol);
	printf("\"%s\"\n",s);
	printed = 1;
	exit (3);
}

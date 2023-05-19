[[USER_CODE_1]]

#include <stdio.h>

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
	#define YYSTYPE_IS_TRIVIAL 1
	#define yystype YYSTYPE /* obsolescent; will be withdrawn */
	#define YYSTYPE_IS_DECLARED 1
#endif

typedef unsigned char yytype_uint8;

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL [[YYFINAL]]
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST -1

/* Store start symbol */
// #define YYSTART [[YYSTART]]

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS [[YYNTOKENS]]
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS [[YYNNTS]]
/* YYNRULES -- Number of rules.  */
#define YYNRULES [[YYNRULES]]
/* YYNRULES -- Number of states.  */
#define YYNSTATES [[YYNSTATES]]

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK -1
#define YYMAXUTOK [[YYMAXUTOK]]

static const yytype_uint8 yytranslate[] = [[yytranslate]];

static const char* const yytname[] = [[yytname]];

short LALR1_action[][YYNTOKENS] = {[[action_table]]};
short LALR1_goto[][YYNNTS] = {[[goto_table]]};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = [[yyr1]];
/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = [[yyr2]];

short defact[] = [[get_defact]];
short table[] = [[get_table]];
short pact[] = [[get_pact]];

void parse() {
	int symbol_stack[100];
	int* symbol_sp;
	int state_stack[100];
	int* state_sp;

	YYSTYPE yyvsa[100];
	YYSTYPE* yyvsp;
	YYSTYPE yyval;

	int yychar = yytranslate[yylex()];

	symbol_sp = symbol_stack;
	state_sp = state_stack;
	yyvsp = yyvsa;

	*++symbol_sp = yychar;
	*++state_sp = 0;
	++yyvsp;

	while (yychar != 0 || *state_sp != 1) {
		short info = LALR1_action[*state_sp][yychar];
		if (info >= 0) {
			*++symbol_sp = yychar;
			*++state_sp = info;
			yychar = yytranslate[yylex()];
		} else {
			short yyn = -info;
			short yylen = yyr2[yyn];
			yyval = yyvsp[1 - yylen];
			switch (yyn) {
			[[reduce]]
			}
			int temp = *symbol_sp--;
			state_sp -= yyr2[yyn];
			symbol_sp -= yyr2[yyn];
			yyvsp -= yyr2[yyn];
			*++symbol_sp = yyr1[yyn];
			*++state_sp = LALR1_goto[*state_sp][*symbol_sp];
			++yyvsp;
		}
	}
}

[[USER_CODE_3]]
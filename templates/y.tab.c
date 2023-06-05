[[USER_CODE_1]]

#include <stdio.h>

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef [[YYSTYPE]] YYSTYPE;
	#define YYSTYPE_IS_TRIVIAL 1
	#define yystype YYSTYPE /* obsolescent; will be withdrawn */
	#define YYSTYPE_IS_DECLARED 1
#endif

typedef short yytype_uint8;

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL [[YYFINAL]]
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST [[YYLAST]]

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

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = [[yyr1]];
/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = [[yyr2]];

[[IF(C1)]]
short yydefact[] = [[yydefact]];
short yypact[] = [[yypact]];
short yydefgoto[] = [[yydefgoto]];
short yypgoto[] = [[yypgoto]];
short yytable[] = [[yytable]];
short yycheck[] = [[yycheck]];
[[ELSE]]
short LALR1_action[][YYNTOKENS] = {[[action_table]]};

short LALR1_goto[][YYNNTS] = {[[goto_table]]};
[[FI]]

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

	while (yychar != 0 || *state_sp >= 1) {
[[IF(C1)]]
		short yyn = yydefact[*state_sp];
		if (yyn == 0)
			yyn = yytable[yypact[*state_sp] + yychar];
[[ELSE]]
		short yyn = LALR1_action[*state_sp][yychar];
[[FI]]
		if (yyn >= 0) {
			*++symbol_sp = yychar;
			*++state_sp = yyn;
			*++yyvsp = yyval;
			yychar = yytranslate[yylex()];
			 /* printf("shift to state %d \n",*state_sp);
            for(int* i=state_stack+1;i<=state_sp;i++)
                printf("%d ", *i);
                printf("\n"); */
		} else {
			yyn = -yyn;
			short yylen = yyr2[yyn];
			yyval = yyvsp[1 - yylen];
			switch (yyn) {
			[[reduce]]
			}
			if(yyn==1)
				break;
			printf("%s -> \n", yytname[yyr1[yyn] + YYNTOKENS]);
			state_sp -= yyr2[yyn];
			symbol_sp -= yyr2[yyn];
			yyvsp -= yyr2[yyn];
			*++symbol_sp = yyr1[yyn];
			*++yyvsp = yyval;
[[IF(C1)]]
			yyn = yypgoto[*symbol_sp] + *state_sp;
			int x = 0 <= yyn && yyn <= YYLAST && yycheck[yyn] == *state_sp ?
				yytable[yyn] : yydefgoto[*symbol_sp];
[[ELSE]]
			int x = LALR1_goto[*state_sp][*symbol_sp];
[[FI]]
			*++state_sp = x;
			/* printf("goto state %d \n",x);
            for(int* i=state_stack+1;i<=state_sp;i++)
                printf("%d ", *i);
                printf("\n"); */
		}
	}
}

[[USER_CODE_3]]
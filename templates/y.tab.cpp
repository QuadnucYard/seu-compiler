#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL [[YYFINAL]]
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST -1

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

const int base = 0;

static const unsigned char yytranslate[] = [[yytranslate]];

static const char* const yytname[] = [[yytname]];

short LALR1_action[][128] = {[[action_table]]};
short LALR1_goto[][128] = {[[goto_table]]};

stack<int> token_stack;
stack<int> state_stack;

short get_rhs[] = [[get_rhs]];
short get_newstate[] = [[get_newstate]];
short get_lhs[]= [[get_lhs]];

short defact[] = [[get_defact]];
short table[] = [[get_table]];
short pact[] = [[get_pact]];

void pop_stack(int cnt, int new_state) {
	for (int i = 0; i < cnt; i++) {
		token_stack.pop();
		state_stack.pop();
	}
	//state_stack.push(new_state);
}

void parse() {
	auto point = yylex();
	//
	token_stack.push(point);
	state_stack.push(0);
	while (point != '$') {
		auto info = LALR1_action[state_stack.top()][point];

		if (info >= 0){
			state_stack.push(info);
			//
			point = yylex();
		//
			token_stack.push(point);
		}
			
		else if (info < base) {
			[[reduce]]
			int temp=token_stack.top();
            token_stack.pop();
			pop_stack(get_rhs[base - info], get_newstate[base - info]);
			token_stack.push(get_lhs[base - info]);
			auto next_info = LALR1_goto[state_stack.top()][abs(token_stack.top())];
            //state_stack.pop();
			state_stack.push(next_info);
            token_stack.push(temp);
            point=temp;
		}

		else{
			//
			point = yylex();
		//
			token_stack.push(point);
		}

		
	}

	while(1){
        auto info = LALR1_action[state_stack.top()][point];

		[[reduce]]

		int temp=token_stack.top();
            token_stack.pop();
			pop_stack(get_rhs[base - info], get_newstate[base - info]);
			token_stack.push(get_lhs[base - info]);
			auto next_info = LALR1_goto[state_stack.top()][abs(token_stack.top())];
			state_stack.push(next_info);
            if(token_stack.top()==0)
                break;
            token_stack.push(temp);
            point=temp;
	}
}

void parse_compressed() {
	auto point = yylex();
	//
	token_stack.push(point);
	state_stack.push(0);
	while (point != '$') {
		auto d_info = defact[state_stack.top()];
		auto info = d_info == 0 ? table[pact[state_stack.top()] + point] : d_info;
		if (info >= 0)
			state_stack.push(info);
		else if (info < base) {
			//auto action = get_action_info(base - info);

			/*
                case 1:
                    {action}
                    ...            
            */
			[[reduce]]

			int temp=token_stack.top();
            token_stack.pop();
			pop_stack(get_rhs[base - info], get_newstate[base - info]);
			auto next_info = LALR1_goto[state_stack.top()][token_stack.top()];
			state_stack.push(next_info);
            token_stack.push(temp);
		}

		//
		point = yylex();
		//
		token_stack.push(point);
	}
}

//
int yylex() { return 1; };

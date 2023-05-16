#include <iostream>
#include <stack>
#include <string>

using namespace std;

const int base = -1;

int LALR1_action[][128] = {[[action_table]]};
int LALR1_goto[][128] = {[[goto_table]]};
string yytex;

void pop_stack(int cnt, int new_state) {
	for (int i = 0; i < cnt; i++) {
		token_stack.pop();
		state_stack.pop();
	}
	token_stack.push(new_state);
}

void parse() {
	stack<int> token_stack;
	stack<int> state_stack;

	auto point = yylex();
	//
	token_stack.push(point);
	state_stack.push(0);
	while (point != '$') {
		auto info = LALR1_action[state_stack.top()][-point];
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

			auto next_info = LALR1_goto[state_stack.top()][token_stack.top()];
			state_stack.push(next_info);
		}

		//
		point = yylex();
		//
		token_stack.push(point);
	}
}

//
int yylex() { return 1; };

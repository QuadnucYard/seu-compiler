#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
const int base = -1;

vector<vector<int>> LALR1_action = {$action_table};
vector<vector<int>> LALR1_goto = {$goto_table};
string yytex;

void scanner() {
	stack<int> token_stack;
	stack<int> state_stack;

	//获取下个词
	auto point = yylex();
	//
	token_stack.push(point);
	state_stack.push(0);
	while (point != '$') {
		auto info = LALR1_action[state_stack.top()][-point];
		if (info >= 0)
			state_stack.push(info);
		else if (info < base) {
			//规约
			//auto action = get_action_info(base - info);

            /*
                case 1:
                    {action}
                    ...            
            */
		   	[[reduce]]


			/* for (int i = 0; i < get_pro_size(base - info); i++){
                token_stack.pop();
                state_stack.pop();
            }
			token_stack.push(get_pro_lhs(base - info)); */

            auto next_info=LALR1_goto[state_stack.top()][token_stack.top()];
            state_stack.push(next_info);
		}

		//
		point = yylex();
		//
		token_stack.push(point);
	}
}

//
int yylex() {
	return 1;
};


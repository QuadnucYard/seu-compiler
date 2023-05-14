#include "yacc/yacc_gen.hpp"
#include "yacc/analyzer.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

namespace comp {
	void yacc_code::gen_table() {
		qy::templater temp{"G:\\vscode\\seu-compiler\\include\\yacc\\y.tab.cpp"};
		/* temp.set_string("$action_table", fmt::format("{}", fmt::join(LALR1_action, ", ")));
        fmt::print("{}", fmt::join(LALR1_action, ", "));
		temp.set_string("$goto_table", fmt::format("{}", fmt::join(LALR1_goto, ", "))); */
        string s_action;
        string s_goto;
        for(int i=0;i<LALR1_action.size();i++){
            s_action+="{"+LALR1_action[i][0];
            for(int j=1;j<LALR1_action[0].size();j++){
                s_action+=", "+LALR1_action[i][j];
            }
            s_action+="}, \n";
        }

        for(int i=0;i<LALR1_goto.size();i++){
            s_goto+="{"+LALR1_goto[i][0];
            for(int j=1;j<LALR1_goto[0].size();j++){
                s_goto+=", "+LALR1_goto[i][j];
            }
            s_action+="}, \n";
        }
        temp.set_string("$action_table",s_action);
        temp.set_string("$goto_table",s_goto);
	}

	void yacc_code::set_action(std::vector<std::vector<int>> t_action) {
		this->LALR1_action = t_action;
	}

	void yacc_code::set_goto(std::vector<std::vector<int>> t_goto) {
		this->LALR1_goto = t_goto;
	}

    void yacc_code::gen_case(const Parser& parser){
        qy::templater temp{"G:\\vscode\\seu-compiler\\include\\yacc\\y.tab.cpp"};
        std::string result={};
        result+="switch(base-info){\n";
        // SyntacticAnalyzer parser=Parser::get_parser();
        for(auto pro:parser.get_parser().rules){
            result+="   case: "+(pro.id);   result+=": {\n";
            result+="       "+pro.action;   result+="\n";
            result+="       for (int i = 0; i <"+pro.rhs.size();    result+="; i++){\n";
            result+="           token_stack.pop();\n";
            result+="           state_stack.pop();\n";
            result+="       }\n";
            result+="       token_stack.push("+pro.lhs;     result+=");\n";
        }
        temp.set_string("[[reduce]]",result);
    }
} // namespace comp
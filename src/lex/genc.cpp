#include "lex/genc.hpp"
#include "lex/fa.hpp"
#include "lex/lexer.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>

extern const char* lex_tmpl;

namespace comp {
	LexCodeGen::LexCodeGen(const Lexer& lexer) : lexer{lexer}, tmpl{lex_tmpl} {}

	void LexCodeGen::operator()(const DFA& dfa) {
		gen_nxt_table(dfa);
		gen_accept_table(dfa);
		gen_nultrans();
	}

	void LexCodeGen::gen_nxt_table(const DFA& dfa) {
		std::string result;
		result += fmt::sprintf(
			R"(static yyconst short yy_nxt[][128] =
{ )"); //感觉默认128挺合理的，这里我暂时不改了。
		int size = dfa.accept_states.size();
		for (int i = 0; i < size; i++) {
			nultrans.push_back(0);
			result += "{";
			int move[128];
			std::memset(move, i * (-1), sizeof(int) * 128);
			for (auto&& [v, w] : dfa.graph.iter_edges(i)) {
				move[w] = v;
			}

			if (move[1] != (-1) * i)
				nultrans[i] = move[1];

			for (int j = 0; j < 128; j++) {
				result += std::to_string(move[j]);
				if (j != 127)
					result += ", ";
				if (i % 10 == 0)
					result += "\n";
			}
			result += "    } ;\n";
		}
		result += "    } ;\n";
		tmpl.set_string("[[yy_nxt]]", result);
	};

	void LexCodeGen::gen_accept_table(const DFA& dfa) {
		std::string result = {};
		/*
        #define YY_NUM_RULES 101
        #define YY_END_OF_BUFFER 102
        */
		result += fmt::sprintf(
			R"(#define YY_NUM_RULES %d
#define YY_END_OF_BUFFER %d
            )",
			lexer.actions.size(), lexer.actions.size() + 1);
		//yy_accept
		result += fmt::sprintf(
			R"(static yyconst short int yy_accept[%d] =
{   %d,)",
			dfa.accept_states.size(), dfa.accept_states[0]);
		int i = 1;
		for (i; i < dfa.accept_states.size(); i++) {
			result += std::to_string(dfa.accept_states[i]);
			if (i != dfa.accept_states.size() - 1)
				result += ", ";
			if (i % 10 == 0)
				result += "\n";
		}
		result += "    } ;\n";
		result += fmt::sprintf(R"(static yy_state_type yy_last_accepting_state;
static char *yy_last_accepting_cpos;)");
		tmpl.set_string("[[yy_accept]]", result);
	}

	void LexCodeGen::gen_nultrans() {
		std::string result = {};
		result += fmt::sprintf(
			R"(static yyconst yy_state_type yy_NUL_trans[%d] =
{   %d,)",
			nultrans.size(), nultrans[0]);
		int i = 1;
		for (i; i < nultrans.size(); i++) {
			result += std::to_string(nultrans[i]);
			if (i != nultrans.size() - 1)
				result += ", ";
			if (i % 10 == 0)
				result += "\n";
		}
		result += "    } ;\n";
		result += fmt::sprintf(R"(#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
#define YY_RESTORE_YY_MORE_OFFSET
char *yytext;)");
		tmpl.set_string("[[yy_NUL_trans]]", result);
	};

	/*
    #include "y.tab.h"
    #include <stdio.h>

    void lineColumn(void);
    void multiLineComment(void);
    */

	void LexCodeGen::gen_case() {
		//对应yy.c：line 6210起
		std::string result = {};
		//第一个case定义为-1了,为了协调在这里先加1和flex写法一致
		result += R"(
			case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos + 1;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;)";
		int i = 0;
		for (i; i < lexer.actions.size(); i++) {
			result += fmt::sprintf(
				R"(case %d:
YY_RULE_SETUP
%s
    YY_BREAK)",
				i + 1, lexer.actions[i]);
		}
		tmpl.set_string("[[ACTIONS]]", result);
	};

	/*
	下面是yywrap等部分
	*/

} // namespace comp
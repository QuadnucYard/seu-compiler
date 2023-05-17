#include "lex/genc.hpp"
#include "lex/fa.hpp"
#include "lex/lexer.hpp"
#include "utils/outfmt.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>

extern const char* lex_tmpl;

namespace comp {
	LexCodeGen::LexCodeGen(const Lexer& lexer) : lexer{lexer}, tmpl{lex_tmpl} {}

	void LexCodeGen::operator()(const DFA& dfa) {
		gen_accept_table(dfa);
		gen_nxt_table(dfa);
		gen_case();
	}

	void LexCodeGen::gen_nxt_table(const DFA& dfa) {
		std::string result;
		int move[128];
		int size = dfa.accept_states.size();
		std::vector<int> nultrans(size, 0);

		for (int i = 0; i < size; i++) {
			std::memset(move, -1, sizeof(move));
			for (auto&& [v, w] : dfa.graph.iter_edges(i))
				move[w] = v;

			if (move[1] != -i)
				nultrans[i] = move[1];

			result += qy::format_array(move);
			result += ",";
		}
		tmpl.set_string("[[YY_NXT]]", result);
		tmpl.set_string("[[YY_NUL_TRANS]]", qy::format_array(nultrans, {.with_brace = false}));
	};

	void LexCodeGen::gen_accept_table(const DFA& dfa) {
		/*
        #define YY_NUM_RULES 101
        #define YY_END_OF_BUFFER 102
        */
		tmpl.set_string("[[YY_NUM_RULES]]", fmt::to_string(lexer.actions.size()));
		tmpl.set_string("[[YY_END_OF_BUFFER]]", fmt::to_string(lexer.actions.size() + 1));
		//yy_accept
		tmpl.set_string("[[YY_ACCEPT]]",
						qy::format_array(dfa.accept_states, {.with_brace = false}));
	}

	/*
    #include "y.tab.h"
    #include <stdio.h>

    void lineColumn(void);
    void multiLineComment(void);
    */

	void LexCodeGen::gen_case() {
		//对应yy.c：line 6210起
		std::string result;
		//第一个case定义为-1了,为了协调在这里先加1和flex写法一致
		for (size_t i = 0; i < lexer.actions.size(); i++) {
			result += fmt::sprintf(
				R"(case %d:
YY_RULE_SETUP
%s
    YY_BREAK
)",
				i + 1, lexer.actions[i]);
		}
		tmpl.set_string("[[ACTIONS]]", result);
	};

	/*
	下面是yywrap等部分
	*/

} // namespace comp
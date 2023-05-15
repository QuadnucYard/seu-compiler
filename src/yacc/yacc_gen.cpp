#include "yacc/yacc_gen.hpp"
#include "yacc/parser.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>

namespace comp {

	yacc_code::yacc_code(const fs::path& path) : temp{path.string()} {}

	void yacc_code::gen(const SyntacticAnalyzer& analyzer) {
		gen_table();
		gen_case(analyzer);
	}

	void yacc_code::gen_table() {
		string s_action;
		string s_goto;
		for (int i = 0; i < LALR1_action.size(); i++)
			s_action += fmt::format("{{{}}}, \n", fmt::join(LALR1_action[i], ", "));
		for (int i = 0; i < LALR1_goto.size(); i++)
			s_goto += fmt::format("{{{}}}, \n", fmt::join(LALR1_goto[i], ", "));
		temp.set_string("[[action_table]]", s_action);
		temp.set_string("[[goto_table]]", s_goto);
	}

	void yacc_code::set_action(std::vector<std::vector<int>> t_action) {
		this->LALR1_action = std::move(t_action);
	}

	void yacc_code::set_goto(std::vector<std::vector<int>> t_goto) {
		this->LALR1_goto = std::move(t_goto);
	}

	void yacc_code::gen_case(const SyntacticAnalyzer& analyzer) {
		std::string result = {};
		result += "switch (base - info) {\n";
		for (auto& prod : analyzer.rules) {
			result += fmt::sprintf(
				R"(            case %d:
                %s
                pop_stack(%d, %d);
                )",
				prod.id, prod.action, prod.rhs.size(), prod.lhs);
		}
		result += "        }\n";
		temp.set_string("[[reduce]]", result);
	}
} // namespace comp
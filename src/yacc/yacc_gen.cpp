#include "yacc/yacc_gen.hpp"
#include "utils/outfmt.hpp"
#include "yacc/parser.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>


namespace comp {

	yacc_code::yacc_code(std::string_view tmpl) : temp{tmpl} {}

	void yacc_code::gen(const parsing_table& pt, const SyntacticAnalyzer& analyzer) {
		gen_table(pt);
		gen_case(analyzer);
	}

	void yacc_code::gen_table(const parsing_table& pt) {
		string s_action;
		string s_goto;
		for (int i = 0; i < pt.action.rows(); i++)
			s_action += fmt::format("{},\n", qy::format_array(pt.action.iter_row(i)));
		for (int i = 0; i < pt.goto_.rows(); i++)
			s_goto += fmt::format("{},\n", qy::format_array(pt.goto_.iter_row(i)));
		temp.set_string("[[action_table]]", s_action);
		temp.set_string("[[goto_table]]", s_goto);
	}

	void yacc_code::gen_case(const SyntacticAnalyzer& analyzer) {
		std::string result = {};
		result += "switch (base - info) {\n";
		for (auto& prod : analyzer.rules) {
			if (!prod.action.empty())
				result += fmt::sprintf(
					R"(case %d:
    %s
)",
					prod.id, prod.action);
		}
		result += "        }\n";
		temp.set_string("[[reduce]]", result);
	}
} // namespace comp
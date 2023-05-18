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
		gen_rhs(analyzer);
		gen_newstate(analyzer);
		gen_compressed(analyzer, pt);
	}

	void yacc_code::gen_table(const parsing_table& pt) {
		string s_action;
		string s_goto;
		for (size_t i = 0; i < pt.action.rows(); i++)
			s_action += fmt::format("{},\n", qy::format_array(pt.action.iter_row(i)));
		for (size_t i = 0; i < pt.goto_.rows(); i++)
			s_goto += fmt::format("{},\n", qy::format_array(pt.goto_.iter_row(i)));
		temp.set_string("[[action_table]]", s_action);
		temp.set_string("[[goto_table]]", s_goto);
	}

	void yacc_code::gen_rhs(const SyntacticAnalyzer& analyzer) {
		temp.set_string("[[get_rhs]]",
						qy::format_array(analyzer.rules | std::views::transform([](auto&& t) {
											 return t.rhs.size();
										 })));
	}

	void yacc_code::gen_newstate(const SyntacticAnalyzer& analyzer) {
		temp.set_string("[[get_newstate]]",
						qy::format_array(analyzer.rules | std::views::transform(&production::lhs)));
	}

	void yacc_code::gen_compressed(const SyntacticAnalyzer& analyzer, const parsing_table& pt) {
		auto pt_c = pt.compress();
		temp.set_string("[[get_defact]]", qy::format_array(pt_c.defact));
		temp.set_string("[[get_table]]", qy::format_array(pt_c.table));
		temp.set_string("[[get_pact]]", qy::format_array(pt_c.pact));
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
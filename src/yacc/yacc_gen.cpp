#include "yacc/yacc_gen.hpp"
#include "utils/outfmt.hpp"
#include "yacc/parser.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>

namespace comp {

	yacc_code::yacc_code(const Parser& parser, std::string_view tmpl) :
		parser{parser}, analyzer{parser.analyzer}, temp{tmpl} {}

	void yacc_code::gen(const parsing_table& pt) {
		gen_info();
		gen_translate();
		gen_table(pt);
		gen_case();
		gen_yyr();
		gen_compressed(pt);
	}

	void yacc_code::gen_info() {
		temp.set_string("[[YYFINAL]]", analyzer.tokens.size() + 1);
		temp.set_string("[[YYNTOKENS]]", analyzer.tokens.size());
		temp.set_string("[[YYNNTS]]", analyzer.nonterminals.size());
		temp.set_string("[[YYNRULES]]", parser.actions.size());
		temp.set_string("[[YYMAXUTOK]]", parser.translate.size() - 1);
	}

	void yacc_code::gen_translate() {
		string s_translate;
		temp.set_string("[[yytranslate]]", qy::format_array(parser.translate));

		std::vector<string> tname;
		std::ranges::copy(analyzer.tokens, std::back_inserter(tname));
		std::ranges::transform(analyzer.nonterminals, std::back_inserter(tname), std::identity{},
							   &nonterminal::name);
		temp.set_string("[[yytname]]",
						qy::format_array(tname | std::views::transform([](const string& s) {
											 return fmt::format("\"{}\"", s);
										 })));
	}

	void yacc_code::gen_table(const parsing_table& pt) {
		string s_action;
		string s_goto;
		for (size_t i = 0; i < pt.action.rows(); i++)
			s_action += fmt::format("{},", qy::format_array(pt.action.iter_row(i)));
		for (size_t i = 0; i < pt.goto_.rows(); i++)
			s_goto += fmt::format("{},", qy::format_array(pt.goto_.iter_row(i)));
		temp.set_string("[[action_table]]", s_action);
		temp.set_string("[[goto_table]]", s_goto);
		temp.set_string("[[YYNSTATES]]", pt.action.rows());
	}

	void yacc_code::gen_yyr() {
		temp.set_string("[[yyr1]]",
						qy::format_array(analyzer.rules | std::views::transform(&production::lhs)));
		temp.set_string("[[yyr2]]",
						qy::format_array(analyzer.rules | std::views::transform([](auto&& t) {
											 return t.rhs.size();
										 })));
	}

	void yacc_code::gen_compressed(const parsing_table& pt) {
		auto pt_c = pt.compress();
		temp.set_string("[[get_defact]]", qy::format_array(pt_c.defact));
		temp.set_string("[[get_table]]", qy::format_array(pt_c.table));
		temp.set_string("[[get_pact]]", qy::format_array(pt_c.pact));
	}

	void yacc_code::gen_case() {
		std::string result = {};
		for (auto& prod : analyzer.rules) {
			if (!prod.action.empty()) {
				string act = qy::replace_all(prod.action, "$$", "(yyval)");
				for (size_t i = 1; i <= prod.rhs.size(); i++) {
					qy::replace_all_inplace(
						act, fmt::format("${}", i),
						fmt::format("(yyvsp[({}) - ({})])", i, prod.rhs.size()));
				}
				result += fmt::sprintf(
					R"(case %d:
    %s
    break;
)",
					prod.id, act);
			}
		}
		temp.set_string("[[reduce]]", result);
	}
} // namespace comp
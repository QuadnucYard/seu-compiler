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
		temp.set_string("[[YYNSTATES]]", pt.action.rows());
		gen_translate();
		gen_yyr();
		if (parser.options.compress) {
			gen_compressed(pt);
		} else {
			gen_table(pt);
		}
		gen_case();
	}

	void yacc_code::gen_info() {
		temp.set_string("[[YYFINAL]]", analyzer.tokens.size() + 1);
		temp.set_string("[[YYNTOKENS]]", analyzer.tokens.size());
		temp.set_string("[[YYNNTS]]", analyzer.nterms.size());
		temp.set_string("[[YYNRULES]]", parser.actions.size());
		temp.set_string("[[YYMAXUTOK]]", parser.translate.size() - 1);
	}

	void yacc_code::gen_translate() {
		string s_translate;
		temp.set_string("[[yytranslate]]", qy::format_array(parser.translate));

		std::vector<string> tname;
		std::ranges::transform(analyzer.tokens, std::back_inserter(tname), std::identity{},
							   &token::name);
		std::ranges::transform(analyzer.nterms, std::back_inserter(tname), std::identity{},
							   &nonterminal::name);
		temp.set_string("[[yytname]]",
						qy::format_array(tname | std::views::transform([](const string& s) {
											 return fmt::format("\"{}\"", s);
										 })));
	}

	void yacc_code::gen_yyr() {
		temp.set_string("[[yyr1]]",
						qy::format_array(analyzer.rules | std::views::transform(&production::lhs)));
		temp.set_string("[[yyr2]]",
						qy::format_array(analyzer.rules | std::views::transform([](auto&& t) {
											 return t.rhs.size();
										 })));
	}

	void yacc_code::gen_table(const parsing_table& pt) {
		temp.set_bool("C1", false);
		string s_action;
		string s_goto;
		for (size_t i = 0; i < pt.action.rows(); i++)
			s_action += fmt::format("{},", qy::format_array(pt.action.iter_row(i)));
		for (size_t i = 0; i < pt.goto_.rows(); i++)
			s_goto += fmt::format("{},", qy::format_array(pt.goto_.iter_row(i)));
		temp.set_string("[[action_table]]", s_action);
		temp.set_string("[[goto_table]]", s_goto);
		temp.set_string("[[YYLAST]]", 0);
	}

	void yacc_code::gen_compressed(const parsing_table& pt) {
		temp.set_bool("C1", true);
		auto pt_c = pt.compress();
		temp.set_string("[[yydefact]]", qy::format_array(pt_c.defact));
		temp.set_string("[[yypact]]", qy::format_array(pt_c.pact));
		temp.set_string("[[yydefgoto]]", qy::format_array(pt_c.defgoto));
		temp.set_string("[[yypgoto]]", qy::format_array(pt_c.pgoto));
		temp.set_string("[[yytable]]", qy::format_array(pt_c.table));
		temp.set_string("[[yycheck]]", qy::format_array(pt_c.check));
		temp.set_string("[[YYLAST]]", pt_c.table.size() - 1);
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
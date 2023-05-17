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

	void yacc_code::gen_rhs(const SyntacticAnalyzer& analyzer) {
		string s_rhs;
		s_rhs = std::to_string(analyzer.rules[0].rhs.size());
		for (int i = 1; i < analyzer.rules.size(); i++)
			s_rhs += fmt::format(", {}", analyzer.rules[i].rhs.size());
		temp.set_string("[[get_rhs]]", s_rhs);
	}

	void yacc_code::gen_newstate(const SyntacticAnalyzer& analyzer) {
		string s_newstate;
		s_newstate = std::to_string(analyzer.rules[0].lhs);
		for (int i = 1; i < analyzer.rules.size(); i++)
			s_newstate += fmt::format(", {}", analyzer.rules[i].lhs);
		temp.set_string("[[get_newstate]]", s_newstate);
	}

	void yacc_code::gen_defact(const SyntacticAnalyzer& analyzer,const parsing_table& pt){
		parsing_table_compressed pt_compressed=analyzer.compress_table_more(pt);
		string s_defact;
		string s_table;
		string s_pact;
		s_defact=pt_compressed.defact[0];
		s_table=pt_compressed.table[0];
		s_pact=pt_compressed.pact[0];
		int len_d=pt_compressed.defact.size();
		int len_t=pt_compressed.table.size();
		int len_p=pt_compressed.pact.size();
		for(int i=1;i<len_d;i++)
			s_defact+=fmt::format(", {}",pt_compressed.defact[i]);
		for(int i=1;i<len_t;i++)
			s_table+=fmt::format(", {}",pt_compressed.table[i]);	
		for(int i=1;i<len_p;i++)
			s_pact+=fmt::format(", {}",pt_compressed.pact[i]);
		temp.set_string("[[get_defact]]", s_defact);
		temp.set_string("[[get_table]]", s_table);
		temp.set_string("[[get_pact]]", s_pact);
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
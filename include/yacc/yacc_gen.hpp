#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	struct parsing_table;
	class SyntacticAnalyzer;

	class yacc_code {
		qy::templater temp;

	public:
		yacc_code() = default;
		yacc_code(std::string_view tmpl);
		void gen(const parsing_table& pt, const SyntacticAnalyzer& analyzer);
		void gen_table(const parsing_table& pt);
		void gen_case(const SyntacticAnalyzer& analyzer);

		void gen_rhs(const SyntacticAnalyzer& analyzer);
		
		void gen_newstate(const SyntacticAnalyzer& analyzer);

		void gen_defact();

		inline void dump(const fs::path& path) { temp.dump(path); }

		


	};
} // namespace comp
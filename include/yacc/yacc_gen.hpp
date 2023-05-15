#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	struct parsing_table;
	class SyntacticAnalyzer;

	class yacc_code {
		std::vector<std::vector<int>> LALR1_action;
		std::vector<std::vector<int>> LALR1_goto;
		qy::templater temp;

	public:
		yacc_code() = default;
		yacc_code(const fs::path& path);
		void gen(const parsing_table& pt, const SyntacticAnalyzer& analyzer);
		void gen_table(const parsing_table& pt);
		void gen_case(const SyntacticAnalyzer& analyzer);

		inline void dump(const fs::path& path) { temp.dump(path); }
	};
} // namespace comp
#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	class SyntacticAnalyzer;

	class yacc_code {
		std::vector<std::vector<int>> LALR1_action;
		std::vector<std::vector<int>> LALR1_goto;
		qy::templater temp;

	public:
		yacc_code() = default;
		yacc_code(const fs::path& path);
		void gen(const SyntacticAnalyzer& analyzer);
		void gen_table();
		void set_action(std::vector<std::vector<int>> t_action);
		void set_goto(std::vector<std::vector<int>> t_goto);
		void gen_case(const SyntacticAnalyzer& analyzer);

		inline void dump(const fs::path& path) { temp.dump(path); }
	};
} // namespace comp
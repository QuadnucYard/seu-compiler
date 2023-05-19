#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	struct parsing_table;
	class SyntacticAnalyzer;
	class Parser;

	class yacc_code {
		qy::templater temp;
		const Parser& parser;
		const SyntacticAnalyzer& analyzer;

	public:
		yacc_code(const Parser& parser, std::string_view tmpl);

		void gen(const parsing_table& pt);

		void gen_info();

		void gen_translate();

		void gen_table(const parsing_table& pt);

		void gen_case();

		void gen_yyr();

		void gen_compressed(const parsing_table& pt);

		inline qy::templater& templater() { return temp; }

		inline void dump(const fs::path& path) { temp.dump(path); }
	};
} // namespace comp
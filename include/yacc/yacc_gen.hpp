#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	struct parsing_table;
	class SyntacticAnalyzer;
	class Parser;
	class YParser;

	class YaccCodeGen {
		qy::templater temp;
		const Parser& parser;
		const YParser& yparser;
		const SyntacticAnalyzer& analyzer;

	public:
		YaccCodeGen(const Parser& parser, const YParser& yparser, std::string_view tmpl);

		void gen_inc();

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
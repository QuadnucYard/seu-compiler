#pragma once
#include "analyzer.hpp"
#include "options.hpp"
#include "yacc_gen.hpp"

namespace comp {

	class Yacc {
		using Options = ParserOptions;

	public:
		Yacc(const Options& options);

		void process(const fs::path& src_path);

	private:
		Options options;
		SyntacticAnalyzer analyzer;

		friend class YaccCodeGen;
	};
} // namespace comp

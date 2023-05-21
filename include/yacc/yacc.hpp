#pragma once
#include "analyzer.hpp"
#include "options.hpp"
#include "yacc_gen.hpp"

namespace comp {

	class Parser : public Recognizer {
		using Options = ParserOptions;

	public:
		Parser(const Options& options);

		void process(const fs::path& src_path);

	private:
		Options options;
		SyntacticAnalyzer analyzer;

		struct DeclHandler;
		struct RulesHandler;

		friend class YaccCodeGen;
	};
} // namespace comp

#pragma once
#include "analyzer.hpp"
#include "options.hpp"
#include "yacc_gen.hpp"

namespace comp {

	class Parser : public Recognizer {
		using Options = ParserOptions;

		/// @brief Raw description of rules
		struct RawRule {
			string lhs;
			std::vector<std::vector<string>> rhs;
		};

	public:
		Parser(const Options& options);
		
		std::vector<string> actions;
		void process(const fs::path& src_path);

	private:
		sid_t get_symbol_id(const string& name) const;

	private:
		inline static string aug_start{"$start"};
		Options options;
		string start_symbol;
		
		std::vector<RawRule> rules;
		dict<sid_t> symbol_map; // Token id to index, token为正数，nonterminal为负数
		SyntacticAnalyzer analyzer;

		struct DeclHandler;
		struct RulesHandler;
	};
} // namespace comp

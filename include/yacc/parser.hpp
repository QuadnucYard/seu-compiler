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

		void process(const fs::path& src_path);

	private:
		sid_t get_symbol_id(const string& name) const;

	private:
		inline static string aug_start{"$accept"};
		Options options;
		string start_symbol;
		std::vector<sid_t> translate; // Map lex token id to yacc token id
		std::vector<RawRule> rules;
		std::vector<string> actions;
		dict<sid_t> symbol_map; // Token id to index, token为正数，nonterminal为负数
		SyntacticAnalyzer analyzer;

		struct DeclHandler;
		struct RulesHandler;

		friend class yacc_code;
	};
} // namespace comp

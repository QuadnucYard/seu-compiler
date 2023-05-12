#pragma once
#include "analyzer.hpp"

namespace comp {

	class Parser : public Recognizer {
	private:
		/// @brief Raw description of rules
		struct RawRule {
			string lhs;
			std::vector<std::vector<string>> rhs;
		};

	public:
		void process(const fs::path& src_path);

	private:
		sid_t get_symbol_id(const string& name) const;

	private:
		inline static string aug_start{"$start"};
		string start_symbol;
		std::vector<RawRule> rules;
		std::vector<string> actions;
		dict<sid_t> symbol_map; // Token id to index, token为正数，nonterminal为负数
		SyntacticAnalyzer analyzer;

		struct DeclHandler;
		struct RulesHandler;
	};
} // namespace comp

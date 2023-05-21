#pragma once
#include "common/gparser.hpp"
#include "symbol.hpp"
#include <unordered_map>
#include <unordered_set>

namespace comp {
	class SyntacticAnalyzer;

	class YParser : GParser {
	private:
		struct RawRule {
			string lhs;
			std::vector<std::vector<string>> rhs;
		};

		inline constexpr static string aug_start{"$accept"};

		SyntacticAnalyzer& analyzer;
		string start_symbol;
		std::unordered_set<sid_t> used_num;
		std::vector<RawRule> rules;
		std::vector<string> actions;
		std::unordered_map<string, sid_t> symbol_map;
		std::vector<std::pair<token::assoc_flag, std::vector<string>>> prec;
		std::unordered_map<string, string> nterm_types;
		string union_type;
		std::vector<string> prologues;
		string epilogue;

	public:
		YParser(std::istream& in, SyntacticAnalyzer& analyzer);

		void parse();

		void finalize();

	private:
		void _declaration();

		void _directive();

		void _rules();

		friend class YaccCodeGen;
	};
} // namespace comp

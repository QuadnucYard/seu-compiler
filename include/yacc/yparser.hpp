#pragma once
#include "common/gparser.hpp"
#include "symbol.hpp"
#include <unordered_map>
#include <unordered_set>

namespace comp {
	class SyntacticAnalyzer;

	class YParser : GParser {
	private:
		// 必须要有这个，因为在解析的时候不知道非终结符的id
		struct RawRule {
			std::vector<string> rhs;
			string prec;
			string action;
		};

		struct RawRuleGroup {
			string lhs;
			std::vector<RawRule> rules;
		};

		inline constexpr static string aug_start{"$accept"};

		SyntacticAnalyzer& analyzer;
		string start_symbol;
		std::unordered_set<sid_t> used_num;
		std::vector<RawRuleGroup> rules;
		std::unordered_map<string, sid_t> symbol_map;
		std::vector<std::pair<token::assoc_flag, std::vector<string>>> prec;
		std::unordered_map<string, string> nterm_types;
		string union_type;

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

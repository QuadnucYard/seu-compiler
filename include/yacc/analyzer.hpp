#pragma once
#include "common/recognizer.hpp"
#include <span>
#include <sul/dynamic_bitset.hpp>
#include <vector>

namespace comp {

	/// @brief Symbol id. Negative for terminals?
	using sid_t = int;
	/// @brief Vector of symbols.
	using symbol_vec = std::vector<sid_t>;

	struct grammar_rule {
		sid_t lhs;
		symbol_vec rhs;
		string action;
	};

	using grammar_rule_list = std::vector<grammar_rule>;

	/// The iterator of
	using grammar_rule_group = std::span<grammar_rule>;

	struct nonterminal {
		string name;
		grammar_rule_group productions;
		sul::dynamic_bitset<> first;  // Set of first
		sul::dynamic_bitset<> follow; // Set of follow
		bool nullable{false};

		nonterminal() = default;

		nonterminal(const string& name, grammar_rule_group productions) :
			name(name), productions(productions) {}
	};

	class SyntacticAnalyzer {
	public:
		static const sid_t END_MARKER = 0;
		grammar_rule_list rules;
		std::vector<nonterminal> nonterminals;
	};
} // namespace comp
#pragma once
#include "parsing_table.hpp"
#include "symbol.hpp"
#include "yacc/options.hpp"
#include <unordered_map>

namespace comp {

	class SyntacticAnalyzer {
	public:
		constexpr static sid_t END_MARKER = 0;

		struct item {
			using key_type = std::pair<const production*, unsigned>;

			const production* prod; // The pointer to the production.
			unsigned dot;			// The position of the lookahead mark.
			symbol_set follow;		// Set of follow

			inline size_t size() const { return prod->rhs.size(); }

			inline bool has_next() const { return dot < size(); }

			inline sid_t next() const { return prod->rhs[dot]; }

			inline bool has_next1() const { return dot + 1 < size(); }

			inline sid_t next1() const { return prod->rhs[dot + 1]; }

			inline item next_item() const { return {prod, dot + 1, follow}; }

			inline key_type key() const { return {prod, dot}; }

			inline bool weak_equals(const item& o) const { return prod == o.prod && dot == o.dot; }

			inline bool operator==(const item& o) const = default;
			inline auto operator<=>(const item& o) const = default;

			inline size_t hashcode() const noexcept {
				return prod->id ^ (dot << 8) ^ std::hash<symbol_set>{}(follow);
			}
		};

		struct item_set {
			size_t kernel_size;		 // How many kernel items in the set.
			std::vector<item> items; // All items in the set

			/// @brief Get the derived set of items.
			/// @param symbol The next symbol.
			/// @return The new set of items.
			item_set get_goto(sid_t symbol) const;

			/// @brief Compare two `item_set` only by kernel items.
			/// @param o Another item_set
			/// @return
			bool kernel_equals(const item_set& o) const;

			bool operator==(const item_set& o) const;

			inline size_t hashcode() const noexcept;

			inline size_t kern_hashcode() const noexcept;
		};

		struct state_graph {
			std::vector<item_set> states;
			std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> dfa;
		};

	public:
		parsing_table process(const ParserOptions& options);

		/// @brief Get symbol name by symbol id.
		/// @param sym Symbol id. May be positive or negative.
		/// @return Name of the symbol
		const string& get_symbol_name(sid_t sym) const;

		const token& get_token(const string& name) const;
		token& get_token(const string& name);

		const nonterminal& get_nterm(const string& name) const;
		nonterminal& get_nterm(const string& name);

		string to_string(const symbol_set& set) const;
		string to_string(const item& it) const;
		string to_string(const item_set& is) const;
		void to_dot(const state_graph& sg, const fs::path& path) const;

	private:
		symbol_set single_set(sid_t s) const;
		void get_nullables();
		void get_firsts();
		state_graph get_LR1_states() const;
		item_set initial_closure() const;
		item_set closure(const item_set& is) const;
		symbol_set next_follow(const item& it) const;
		parsing_table get_LR1_table(const state_graph& LR1_states) const;
		parsing_table get_LALR1_table(const state_graph& LR1_states,
									  parsing_table& LR1_table) const;

	public:
		std::vector<token> tokens;		 // All tokens involved. Index == sid
		production_list rules;			 // All rules involved.
		std::vector<nonterminal> nterms; // All nonterminals involved. Index == -sid
		std::vector<sid_t> translate;	 // Map lex token id to yacc token id
	};
} // namespace comp

template <>
struct std::hash<comp::SyntacticAnalyzer::item> {
	std::size_t operator()(comp::SyntacticAnalyzer::item const& it) const noexcept {
		return it.hashcode();
	}
};

template <>
struct std::hash<comp::SyntacticAnalyzer::item_set> {
	std::size_t operator()(comp::SyntacticAnalyzer::item_set const& is) const noexcept {
		return is.hashcode();
	}
};
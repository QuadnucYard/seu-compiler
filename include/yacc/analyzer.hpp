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

	using symbol_set = sul::dynamic_bitset<>;

	struct production {
		sid_t lhs;
		symbol_vec rhs;
		string action;
	};

	using production_list = std::vector<production>;

	/// The iterator of
	using production_group = std::span<production>;

	struct nonterminal {
		string name;
		production_group productions;
		symbol_set first; // Set of first
		bool nullable{false};

		nonterminal() = default;

		nonterminal(const string& name, production_group productions, size_t size) :
			name(name), productions(productions), first(size) {}
	};

	class SyntacticAnalyzer {
	private:
		struct item {
			using key_type = std::pair<const production*, unsigned>;

			const production* prod;
			unsigned dot;
			symbol_set follow;

			inline size_t size() const { return prod->rhs.size(); }

			inline bool has_next() const { return dot < size(); }

			inline sid_t next() const { return prod->rhs[dot]; }

			inline bool has_next1() const { return dot + 1 < size(); }

			inline sid_t next1() const { return prod->rhs[dot + 1]; }

			inline item next_item() const { return {prod, dot + 1, follow}; }

			inline key_type key() const { return {prod, dot}; }

			inline bool operator==(const item& o) const = default;
			inline auto operator<=>(const item& o) const = default;
		};

		struct item_set {
			size_t kernel_size;
			std::vector<item> items;

			// bool contains(const item& i) const;

			/// @brief Get the derived set of items.
			/// @param symbol The next symbol.
			/// @return The new set of items.
			item_set get_goto(sid_t symbol) const;

			bool operator==(const item_set& o) const;

			// inline auto operator<=>(const item_set& o) const = default;
		};

	public:
		void process();
		const string& get_symbol_name(sid_t sym) const;
		string to_string(const symbol_set& set) const;
		string to_string(const item& it) const;
		string to_string(const item_set& is) const;

	private:
		symbol_set single_set(sid_t s) const;
		void get_nullables();
		void get_firsts();
		item_set initial_closure() const;
		item_set closure(const item_set& is) const;

	public:
		static const sid_t END_MARKER = 0;
		std::vector<string> tokens;
		production_list rules;
		std::vector<nonterminal> nonterminals;
	};
} // namespace comp
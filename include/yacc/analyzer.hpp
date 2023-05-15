#pragma once
#include "common/recognizer.hpp"
#include "utils/matrix.hpp"
#include <bitset>
#include <span>
#include <vector>

namespace comp {

	/// @brief Symbol id. 正数表示终结符，负数表示终结符，0 表示 `$` 或 `S'`
	using sid_t = int;
	/// @brief Vector of symbols.
	using symbol_vec = std::vector<sid_t>;

	/// @brief `bitset` 存储的符号集合
	using symbol_set = std::bitset<128>;

	/// @brief 产生式
	struct production {
		sid_t id;		// 全局序号
		sid_t lhs;		// 左部
		symbol_vec rhs; // 右部
		string action;	// 语义动作
	};

	using production_list = std::vector<production>;

	/// The iterator of
	using production_group = std::span<production>;

	/// @brief 非终结符
	struct nonterminal {
		string name;				  // 字面值
		production_group productions; // 所有产生式
		symbol_set first;			  // Set of first
		bool nullable{false};		  // 是否可产生ε

		nonterminal() = default;

		nonterminal(const string& name, production_group productions) :
			name(name), productions(productions), first{} {}
	};

	struct parsing_table {
		qy::matrix<sid_t> action;
		qy::matrix<sid_t> goto_;
	};

	class SyntacticAnalyzer {
	public:
		static const sid_t END_MARKER = 0;

	private:
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
		};

		struct item_set {
			size_t kernel_size;		 // How many kernel items in the set.
			std::vector<item> items; // All items in the set

			// bool contains(const item& i) const;

			/// @brief Get the derived set of items.
			/// @param symbol The next symbol.
			/// @return The new set of items.
			item_set get_goto(sid_t symbol) const;

			/// @brief Compare two `item_set` only by kernel items.
			/// @param o Another item_set
			/// @return
			bool kernel_equals(const item_set& o) const;

			bool operator==(const item_set& o) const;

			// inline auto operator<=>(const item_set& o) const = default;
			// inline bool operator<(const item_set& o) const {
			// 	return kernel_size < o.kernel_size ||
			// 		   kernel_size == o.kernel_size &&
			// 			   std::ranges::lexicographical_compare(items, o.items);
			// }
		};

		struct state_graph {
			std::vector<item_set> states;
			std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> atn;
		};

	public:
		parsing_table process();

		/// @brief Get symbol name by symbol id.
		/// @param sym Symbol id. May be positive or negative.
		/// @return Name of the symbol
		const string& get_symbol_name(sid_t sym) const;

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
		parsing_table get_LR1_table(const state_graph& LR1_states) const;
		parsing_table get_LALR1_table(const state_graph& LR1_states,
									  const parsing_table& LR1_table) const;

	public:
		std::vector<string> tokens;			   // All tokens involved. Index == sid
		production_list rules;				   // All rules involved.
		std::vector<nonterminal> nonterminals; // All nonterminals involved. Index == -sid
	};
} // namespace comp
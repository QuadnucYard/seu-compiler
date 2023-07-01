#pragma once
#include <bitset>
#include <span>
#include <string>
#include <vector>

namespace comp {
	/// @brief Symbol id. 正数表示终结符，负数表示终结符，0 表示 `$` 或 `S'`
	using sid_t = int;
	/// @brief Vector of symbols.
	using symbol_vec = std::vector<sid_t>;

	/// @brief `bitset` 存储的符号集合
	using symbol_set = std::bitset<128>;

	using ssize_t = std::int64_t;

	using std::string;

	/// @brief 产生式
	struct production {
		sid_t id;		// 全局序号
		sid_t lhs;		// 左部
		symbol_vec rhs; // 右部
		sid_t prec;		// 优先级
		string action;	// 语义动作
	};

	using production_list = std::vector<production>;

	/// The iterator of
	using production_group = std::span<production>;

	struct token {
		enum class assoc_flag {
			NONE,
			LEFT,
			RIGHT,
		};

		string name;						// 名称
		string literal;						// 字面值
		string tag;							// union中的标记
		sid_t num;							// 数值，暂不考虑指定
		bool is_defined{false};				// 是否在 %token 声明
		assoc_flag assoc{assoc_flag::NONE}; // 结合性
		sid_t prec{0};						// 优先级
	};

	/// @brief 非终结符
	struct nonterminal {
		string name;				  // 字面值
		string tag;					  // 标记
		production_group productions; // 所有产生式
		symbol_set first;			  // Set of first
		bool nullable{false};		  // 是否可产生ε

		nonterminal() = default;

		nonterminal(const string& name, production_group productions) :
			name(name), productions(productions), first{} {}
	};
} // namespace comp

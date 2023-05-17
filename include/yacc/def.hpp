#pragma once
#include <vector>

namespace comp {
	/// @brief Symbol id. 正数表示终结符，负数表示终结符，0 表示 `$` 或 `S'`
	using sid_t = int;
	/// @brief Vector of symbols.
	using symbol_vec = std::vector<sid_t>;
} // namespace comp

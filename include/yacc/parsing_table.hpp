#pragma once
#include "symbol.hpp"
#include "utils/fs.hpp"
#include "utils/matrix.hpp"
#include <unordered_map>

namespace comp {

	struct parsing_table_compressed {
		constexpr static sid_t ERR{0};

		symbol_vec defact;
		symbol_vec pact;
		symbol_vec defgoto;
		symbol_vec pgoto;
		symbol_vec table;
		symbol_vec check;
		std::vector<std::unordered_map<sid_t, sid_t>> guard; // 用来解决不走def的问题

		std::tuple<bool, size_t, sid_t> find_embed(auto&& slice, size_t l, size_t base) const;
		std::tuple<bool, size_t, sid_t> embed(auto&& slice, size_t l, size_t base);
	};

	struct parsing_table {
		constexpr static sid_t ERR{parsing_table_compressed::ERR};

		qy::matrix<sid_t> action;
		qy::matrix<sid_t> goto_;

		void to_csv(const fs::path& path) const;

		parsing_table_compressed compress() const;
	};

} // namespace comp

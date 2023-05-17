#pragma once
#include "def.hpp"
#include "utils/matrix.hpp"

namespace comp {

	struct parsing_table_compressed {
		symbol_vec defact;
		symbol_vec pact;
		symbol_vec defgoto;
		symbol_vec pgoto;
		symbol_vec table;
		symbol_vec check;
	};

	struct parsing_table {
		constexpr static sid_t ERR{0};

		qy::matrix<sid_t> action;
		qy::matrix<sid_t> goto_;

		parsing_table_compressed compress_less() const;
		parsing_table_compressed compress() const;
	};

} // namespace comp

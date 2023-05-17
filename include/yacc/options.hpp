#pragma once
#include <string>

namespace comp {
	struct ParserOptions {
		std::string header_file;
		std::string outfile;
		std::string lr1_pda_dot;
	};
} // namespace comp

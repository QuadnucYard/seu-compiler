#pragma once
#include "glexer.hpp"

namespace comp {
	/// @brief Parser for grammar file.
	class GParser {
	protected:
		GLexer lexer;
		GToken tok;
		int section{0}; // Section number, from 0 to 2
	public:
		GParser(std::istream& in) : lexer{in} {}

		void parse() {}

	protected:
		GToken& next0() {
			tok = lexer.scan_noop();
			return tok;
		}

		bool check_section() {
			if (tok.type == GToken::DIR && tok._string() == "%%") {
				section++;
				next0();
				return true;
			}
			return false;
		}
	};
} // namespace comp

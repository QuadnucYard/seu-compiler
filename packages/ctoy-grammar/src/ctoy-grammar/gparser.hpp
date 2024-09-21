#pragma once

#include "glexer.hpp"
#include <string>
#include <vector>

namespace comp {

/// @brief Parser for grammar file.
class GParser {
  public:
    GParser(std::istream& in) : lexer{in} {}

  protected:
    GToken& next0() {
        tok = lexer.scan_noop();
        return tok;
    }

    bool check_section(bool take_next = true) {
        if (tok.kind == GTokenKind::DIR && tok.as_string() == "%%") {
            section++;
            if (take_next) next0();
            return true;
        }
        return false;
    }

  protected:
    GLexer lexer;
    GToken tok;
    int section{0}; // Section number, from 0 to 2

    std::vector<string> prologues;
    string epilogue;
};

} // namespace comp

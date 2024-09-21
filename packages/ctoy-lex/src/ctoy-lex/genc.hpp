#pragma once

#include "ctoy-utils/templater.hpp"

namespace comp {

struct DFA;
class Lex;
class LParser;

class LexCodeGen {
  public:
    LexCodeGen(const Lex& lexer, const LParser& lparser);

    void operator()(const DFA& dfa);

    void dump(const std::filesystem::path& path) { tmpl.dump(path); }

    qy::templater& templater() { return tmpl; }

  private:
    void gen_nxt_table(const DFA& dfa);
    void gen_accept_table(const DFA& dfa);
    void gen_all_table(const DFA& dfa);
    void gen_case();

  private:
    const Lex& lexer;
    const LParser& lparser;

    qy::templater tmpl;
};
} // namespace comp

#pragma once

#include "./fa.hpp"

namespace comp {

class Lex {
  public:
    struct Options {
        string outfile;
        string scanner_nfa_dot;
        string scanner_dfa_dot;
        bool compress;
    };

  public:
    Lex(const Options& options);
    void process(std::istream& source_file);

  private:
    Options options;
    DFABuilder dfa_builder;

    friend class LexCodeGen;
};

} // namespace comp

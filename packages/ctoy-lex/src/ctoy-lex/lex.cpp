#include "./lex.hpp"
#include "./genc.hpp"
#include "./lparser.hpp"
#include "./regex.hpp"
#include "ctoy-utils/stopwatch.hpp"

namespace comp {

Lex::Lex(const Options& options) : options{options} {}

void Lex::process(std::istream& source_file) {
    qy::stopwatch sw;

    LParser lparser(source_file);
    lparser.parse();

    sw.record("Parse grammar");

    // 补充一个 catchall
    lparser.rules.emplace_back("[\\s\\S]", "ECHO;");

    for (auto&& rule : lparser.rules)
        dfa_builder.add_re(unescape_regex(rule.expr));

    sw.record("Add REs");

    auto&& [nfa, dfa] = dfa_builder.join_nfa();
    // 增加一个处理0转换的点
    dfa.graph.resize(dfa.size() + 1);
    dfa.accept_states.push_back(static_cast<vid_t>(lparser.rules.size()));

    sw.record("Generate scanner");

    if (!options.scanner_nfa_dot.empty()) nfa.to_dot(options.scanner_nfa_dot);
    if (!options.scanner_dfa_dot.empty()) dfa.to_dot(options.scanner_dfa_dot);

    LexCodeGen codegen(*this, lparser);

    codegen(dfa);

    sw.record("Generate code");

    codegen.dump(options.outfile);

    sw.record("Dump code");

    sw.print("Lex");
}

} // namespace comp

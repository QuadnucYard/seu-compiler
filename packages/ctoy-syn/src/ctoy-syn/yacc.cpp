#include "./yacc.hpp"
#include "./yparser.hpp"

const char* yacc_tmpl{""};

using namespace qy;

namespace comp {

Yacc::Yacc(const Options& options) : options{options} {}

void Yacc::process(std::istream& source_file) {
    auto yparser = YParser(source_file, analyzer);
    yparser.parse();
    yparser.finalize();

    std::ofstream tab_inc_file(options.header_file);

    YaccCodeGen code_gen{*this, yparser, yacc_tmpl};

    code_gen.gen_inc();

    auto pt = analyzer.process(options);
    code_gen.gen(pt);

    code_gen.dump(options.outfile);
}

} // namespace comp

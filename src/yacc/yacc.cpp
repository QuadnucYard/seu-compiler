#include "yacc/yacc.hpp"
#include "yacc/yparser.hpp"
#include <fstream>

extern const char* yacc_tmpl;

using namespace qy;

namespace comp {

	Parser::Parser(const Options& options) : options{options} {}

	void comp::Parser::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

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
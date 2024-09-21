#include "yacc/yacc.hpp"
#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char const* argv[]) {
	using namespace std::string_literals;

	argparse::ArgumentParser prog("seu-yacc");

	prog.add_argument("input").help("Path of input .l file");
	prog.add_argument("--header-file")
		.default_value("y.tab.h"s)
		.help("Path of output header file.");
	prog.add_argument("-oFILE", "--outfile")
		.default_value("y.tab.c"s)
		.help("Path of output parser file.");
	prog.add_argument("--lr1-pda")
		.default_value(""s)
		.implicit_value("lr1-pda.dot"s)
		.nargs(argparse::nargs_pattern::optional)
		.help("Set to output LR(1) state graph.");
	prog.add_argument("-C1").default_value(false).implicit_value(true).help(
		"Set to compress LALR(1) parsing table. Default: false.");

	try {
		prog.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << prog;
		return 1;
	}

	auto input_file = prog.get("input");

	auto yacc = comp::Yacc({
		.header_file = prog.get("--header-file"),
		.outfile = prog.get("--outfile"),
		.lr1_pda_dot = prog.get("--lr1-pda"),
		.compress = prog.get<bool>("-C1"),
	});

	try {
		yacc.process(input_file);
	} catch (const std::exception& err) {
		std::cerr << "fuck!" << err.what() << std::endl;
		return 2;
	}

	return 0;
}
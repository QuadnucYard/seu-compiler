#include "lex/lexer.hpp"
#include <argparse/argparse.hpp>
#include <iostream>

int main(int argc, char const* argv[]) {
	using namespace std::string_literals;

	argparse::ArgumentParser prog("seu-lex");

	prog.add_argument("input").help("Path of input .l file");
	prog.add_argument("-oFILE", "--outfile").default_value("lex.yy.c");
	prog.add_argument("--scanner-nfa")
		.default_value(""s)
		.implicit_value("nfa.dot"s)
		.nargs(argparse::nargs_pattern::optional);
	prog.add_argument("--scanner-dfa")
		.default_value(""s)
		.implicit_value("dfa.dot"s)
		.nargs(argparse::nargs_pattern::optional);

	try {
		prog.parse_args(argc, argv);
	} catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << prog;
		return 1;
	}

	auto input_file = prog.get("input");

	auto lexer = comp::Lexer({
		.outfile = prog.get("--outfile"),
		.scanner_nfa_dot = prog.get("--scanner-nfa"),
		.scanner_dfa_dot = prog.get("--scanner-dfa"),
	});
	lexer.process(input_file);

	return 0;
}
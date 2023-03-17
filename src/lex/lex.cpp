#include <iostream>
#include "lex/lexer.hpp"

int main(int argc, char** argv) {
	// The main is used for command line.
	std::cout << fs::current_path() << std::endl;
	comp::Lexer().process("E:\\Project\\seu-compiler\\C-Compiler\\c.l");
	return 0;
}
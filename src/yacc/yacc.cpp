#include <iostream>
#include "yacc/parser.hpp"

int main() {
	std::cout << "yacc" << std::endl;
	comp::Parser().process("E:\\Project\\seu-compiler\\C-Compiler\\c.y");
	return 0;
}
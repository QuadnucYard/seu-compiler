#include "lex/fa.hpp"
#include"lex/regex.hpp"
int main(int argc, char const* argv[]) {
	using namespace comp;
	DFABuilder b;
	b.add_re(unescape_regex("[a-zA-Z_]([a-zA-Z_][0-9])*"));
	return 0;
}

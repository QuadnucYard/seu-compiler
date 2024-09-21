#include "lex/fa.hpp"
#include "lex/regex.hpp"

int main(int argc, char const* argv[]) {
	using namespace comp;
	DFABuilder b;
	b.add_re(unescape_regex(R"(\"[ a-c]*\")"));

	return 0;
}

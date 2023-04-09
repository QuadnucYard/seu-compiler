#include "lex/fa.hpp"

int main(int argc, char const* argv[]) {
	using namespace comp;
	DFABuilder b;
	b.add_re("(a|b)*abb", 1);
	return 0;
}

#include "utils/string_utils.hpp"
#include <cassert>
#include <set>

int main(int argc, char const* argv[]) {
	assert(qy::trim_left(" \t\n 123 67 \t ") == "123 67 \t ");
	assert(qy::trim_right(" \t 123 67 \t\n ") == " \t 123 67");
	assert(qy::trim(" \t \n123 67\n \t ") == "123 67");
	assert(qy::trim("abc de") == "abc de");
	assert(qy::trim_left(" \t\n ") == "");
	assert(qy::trim_right(" \t\n ") == "");
	assert(qy::trim(" \t\n ") == "");

	int a[]{1, 2, 3};
	assert(qy::join(a, ", ") == "1, 2, 3");
	using namespace std::string_literals;
	std::set b{"apple"s, "tree"s, "abandon"s};
	assert(qy::join(b, "! ") == "abandon! apple! tree");

	assert(qy::is_quoted(R"("")"));
	assert(!qy::is_quoted(R"(")"));
	assert(!qy::is_quoted("\"\\\""));
	assert(qy::is_quoted(R"("abc")"));
	return 0;
}

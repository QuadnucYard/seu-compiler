#include "utils/outfmt.hpp"

int main(int argc, char const* argv[]) {
	int a[]{1, 5, 563, 654, 993, 32435, 345};
	unsigned w = 8;
	fmt::print("{}\n", qy::format_array(a, {.line_size = 3, .field_width = w, .indent = 0}));
	fmt::print("{}\n", qy::format_array(a, {.line_size = 3, .field_width = w, .indent = 4}));
	int b[][4]{{1, 3, 6, 6}, {1, 5, 7, 1}, {7, 6, 4, 1}, {8, 5, 3, 6}};
	fmt::print("{}\n", qy::format_array_2d(b, {.line_size = 3, .field_width = 4, .indent = 0}));
	fmt::print("{}\n", qy::format_array_2d(b, {.line_size = 3, .field_width = 4, .indent = 4}));
	return 0;
}

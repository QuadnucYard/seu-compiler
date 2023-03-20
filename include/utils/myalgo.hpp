#pragma once
#include <algorithm>

namespace qy::ranges {
	ptrdiff_t index_of(auto&& R, auto&& v) {
		auto it = std::ranges::find(R, v);
		return it == std::end(R) ? -1 : it - std::begin(R);
	}
} // namespace qy::ranges

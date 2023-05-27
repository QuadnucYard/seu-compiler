#pragma once
#include <algorithm>
#include <ranges>

namespace qy::ranges {
	ptrdiff_t index_of(auto&& R, auto&& v) {
		auto it = std::ranges::find(R, v);
		return it == std::end(R) ? -1 : it - std::begin(R);
	}

	template <typename I, typename S>
	auto pair_range(const std::pair<I, S>& p) {
		return std::ranges::subrange(p.first, p.second);
	}
} // namespace qy::ranges

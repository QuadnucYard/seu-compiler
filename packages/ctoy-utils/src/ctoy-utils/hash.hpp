#include <functional>
#include <ranges>

namespace qy {

	template <typename T>
	inline void hash_combine(size_t& seed, const T& val) {
		seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2); //0x9e3779b9
	}

	//auxiliary generic functions
	template <typename T>
	inline void hash_val(size_t& seed, const T& val) {
		hash_combine(seed, val);
	}

	template <typename T, typename... Types>
	inline void hash_val(size_t& seed, const T& val, const Types&... args) {
		hash_combine(seed, val);
		hash_val(seed, args...);
	}

	//auxiliary generic function
	template <typename... Types>
	inline size_t hash_val(const Types&... args) {
		size_t seed = 0;
		hash_val(seed, args...);
		return seed;
	}

	template <std::ranges::input_range R>
	inline size_t hash_range(R&& r) {
		size_t seed = 0;
		for (auto&& x : r)
			hash_combine(seed, x);
		return seed;
	}

	template <typename I, typename S>
	inline size_t hash_range(I first, S last) {
		size_t seed = 0;
		for (; first != last; ++first)
			hash_combine(seed, *first);
		return seed;
	}

	template <std::ranges::input_range R>
	inline size_t hash_range(R&& r, auto&& proj) {
		size_t seed = 0;
		for (auto&& x : r)
			hash_combine(seed, std::invoke(proj, x));
		return seed;
	}

	template <typename I, typename S>
	inline size_t hash_range(I first, S last, auto&& proj) {
		size_t seed = 0;
		for (; first != last; ++first)
			hash_combine(seed, std::invoke(proj, *first));
		return seed;
	}
} // namespace qy

template <typename T1, typename T2>
struct std::hash<std::pair<T1, T2>> {
	std::size_t operator()(std::pair<T1, T2> const& p) const noexcept {
		return qy::hash_val(p.first, p.second);
	}
};
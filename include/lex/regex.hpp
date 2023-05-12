#pragma once
#include <string>
#include <unordered_map>

namespace comp {
	template <typename T = std::string>
	using dict = std::unordered_map<std::string, T>;

	std::pair<size_t, std::string> unescape_regex(std::string_view s,
												  const dict<std::string>& definitions);

	std::string unescape_regex(std::string_view s);

} // namespace comp

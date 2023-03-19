#pragma once
#include <sstream>
#include <string>

namespace qy {
	inline bool is_blank_lead(std::string_view s) {
		return s.length() == 0 || isblank(s[0]);
	}

	inline std::string_view trim_left(std::string_view s) {
		return s.substr(s.find_first_not_of(" \t\r\n\v\f"));
	}

	inline std::string_view trim_right(std::string_view s) {
		return s.substr(0, s.find_last_not_of(" \t\r\n\v\f") + 1);
	}

	inline std::string_view trim(std::string_view s) {
		return trim_left(trim_right(s));
	}

	template <typename I>
	std::ostream& join(I it, I end_it, std::ostream& o, std::string_view sep = "") {
		if (it != end_it)
			o << *it++;
		while (it != end_it)
			o << sep << *it++;
		return o;
	}

	template <typename I>
	std::string join(I it, I end_it, std::string_view sep = "") {
		std::ostringstream ostr;
		join(it, end_it, ostr, sep);
		return ostr.str();
	}

	std::string join(const auto& c, std::string_view sep = "") {
		return join(std::begin(c), std::end(c), sep);
	}

} // namespace qy
#include <string>

namespace qy {
	inline bool is_blank_lead(std::string_view s) {
		return s.length() == 0 || isblank(s[0]);
	}

	inline std::string_view trim_left(std::string_view s) {
		return s.substr(s.find_first_not_of(" \t\v\n\f"));
	}

	inline std::string_view trim_right(std::string_view s) {
		return s.substr(0, s.find_last_not_of(" \t\v\n\f") + 1);
	}

	inline std::string_view trim(std::string_view s) {
		return trim_left(trim_right(s));
	}
}
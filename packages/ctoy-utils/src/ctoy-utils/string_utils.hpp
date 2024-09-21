#pragma once
#include <sstream>
#include <string>

namespace qy {
	inline bool is_blank_lead(std::string_view s) {
		return s.length() == 0 || isblank(s[0]);
	}

	inline std::string_view trim_left(std::string_view s) {
		if (auto p = s.find_first_not_of(" \t\r\n\v\f"); p != std::string::npos)
			return s.substr(p);
		return "";
	}

	inline std::string_view trim_right(std::string_view s) {
		if (auto p = s.find_last_not_of(" \t\r\n\v\f"); p != std::string::npos)
			return s.substr(0, p + 1);
		return "";
	}

	inline std::string_view trim(std::string_view s) {
		return trim_left(trim_right(s));
	}

	inline bool is_quoted(std::string_view s) {
		auto l = s.length();
		if (l < 2 || s[0] != '"' || s[l - 1] != '"')
			return false;
		if (s[l - 2] == '\\')
			return false;
		return true;
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

	inline std::string join(const auto& c, std::string_view sep = "") {
		return join(std::begin(c), std::end(c), sep);
	}

	inline std::string& replace_all_inplace(std::string& src, std::string_view old_value,
											std::string_view new_value) {
		for (std::string::size_type pos{0}; pos != std::string::npos; pos += new_value.length()) {
			if ((pos = src.find(old_value, pos)) != std::string::npos)
				src.replace(pos, old_value.length(), new_value);
			else
				break;
		}
		return src;
	}

	inline std::string replace_all(std::string src, std::string_view old_value,
								   std::string_view new_value) {
		return replace_all_inplace(src, old_value, new_value);
	}

	inline char unescape(char c) {
		switch (c) {
		case '\\':
			return '\\';
		case 'b':
			return '\b';
		case 'f':
			return '\f';
		case 'n':
			return '\n';
		case 't':
			return '\t';
		case 'v':
			return '\v';
		}
		return 0;
	}

	inline char unescape_char(std::string_view s) {
		if (s[1] == '\\')
			return unescape(s[2]);
		return s[1];
	}

	inline std::string unescape_string(std::string_view s) {
		std::string res;
		bool escaped = false;
		for (auto c : s.substr(1, s.length() - 2)) {
			if (escaped) {
				res.push_back(unescape(c));
				escaped = false;
			} else if (c == '\\') {
				escaped = true;
			} else {
				res.push_back(c);
			}
		}
		return res;
	}

} // namespace qy
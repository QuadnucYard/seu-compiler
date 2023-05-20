#include "lex/regex.hpp"
#include "utils/exceptions.hpp"
#include <fmt/core.h>
#include <stack>

namespace comp {
	std::pair<size_t, std::string> unescape_regex(std::string_view s,
												  const dict<std::string>& definitions) {
		std::stack<char> bra; // Brackets stack

		//is )
		auto match = [&s, &bra](char c) {
			if (bra.empty())
				throw syntax_error(fmt::format(
					"Regex parsing error for \"{}\". Receive '{}', but no bracket expected.", s,
					c));
			if (bra.top() != c)
				throw syntax_error(
					fmt::format("Regex parsing error for \"{}\". Receive '{}', expected '{}'.", s,
								c, bra.top()));
			bra.pop();
		};

		//is [ matched
		auto is_squared = [&bra]() {
			return !bra.empty() && bra.top() == '[';
		};

		std::string res;
		//前一个字符是否是'\'
		bool escaped = false;
		//当前是否在引号的环境里
		bool quoted = false;
		size_t brace_start = -1;
		for (size_t i = 0; i < s.length(); i++) {
			char c = s[i];
			if (escaped) { // Escape chars
				if (c == '0')
					c = '\0';
				else if (c == 'r')
					c = '\r';
				else if (c == 'n')
					c = '\n';
				else if (c == 't')
					c = '\t';
				else if (c == 'v')
					c = '\v';
				else if (c == 'b')
					c = '\b';
				else if (c == 'f')
					c = '\f';
				else if (c == 's' || c == 'S' || c == 'w' || c == 'W' || c == 'd' || c == 'D')
					c = -c;
				res += c;
				escaped = false;
				continue;
			}
			if (c == '\\') { // Escape next
				escaped = true;
				continue;
			}
			if (c == '"' && !is_squared()) { // Quotation outside the square brackets
				quoted = !quoted;
				continue;
			}
			if (quoted) { // If quoted, simple add to result
				res += c;
				continue;
			}
			bool is_special = !is_squared();
			switch (c) {
			case '(':
			case '[':
				bra.push(c);
				break;
			case '{':
				brace_start = i;
				bra.push(c);
				break;
			case ')':
				match('(');
				break;
			case ']':
				match('[');
				is_special = true;
				break;
			case '}':
				match('{');
				res += definitions.at(std::string{s.substr(brace_start + 1, i - brace_start - 1)});
				brace_start = -1;
				continue;
			case ' ':
				if (is_squared())
					break; // Fallthrough trick
			case '\t':
				return {i, res};
			case '*':
			case '+':
			case '?':
			case '|':
			case '.':
				// Such symbols inside "[]" are all literals.
				break;
			case '^':
				// Only the one strictly after '[' is special.
				is_special = is_squared() && s[i - 1] == '[';
				break;
			default:
				is_special = false;
				break;
			}
			if (brace_start == static_cast<size_t>(-1)) // If outside the braces
				res += is_special ? -c : c;
		}
		if (quoted)
			throw syntax_error(
				fmt::format("Regex parsing error for \"{}\". Mismatched quotation char.", s));
		if (escaped)
			throw syntax_error(
				fmt::format("Regex parsing error for \"{}\". Mismatched escape char.", s));
		if (!bra.empty())
			throw syntax_error(fmt::format(
				"Regex parsing error for \"{}\". Unenclosed bracket '{}'.", s, bra.top()));
		return {s.length(), res};
	}

	std::string unescape_regex(std::string_view s) { return unescape_regex(s, {}).second; }

} // namespace comp

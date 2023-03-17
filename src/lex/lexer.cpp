#include "lex/lexer.hpp"
#include "utils/exceptions.hpp"
#include "utils/string_utils.hpp"
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <stack>

namespace comp {

	void Lexer::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

		std::ofstream yylex_file("lex.yy.c");

		SourceHandler h(yylex_file);
		int lineno = 0; // Line number, from 0

		std::ostringstream action; // Action string
		std::string re; // The leading expression
		for (std::string s; std::getline(source_file, s); lineno++) {
			if (h.code(s))
				continue;
			else if (s == "%%") {
				if (++h.section == 2) {
					// TODO Generate and output FA
				}
				continue;
			}
			if (h.section == 0) {
				// Here process macro.
				// Assume that it obeys "define before use"
				size_t p1 = s.find_first_of(" \t"), p2 = s.find_first_not_of(" \t", p1);
				if (p1 != std::string::npos && p2 != std::string::npos)
					definitions.emplace(s.substr(0, p1), get_re(s.substr(p2)).second);
				continue;
			}
			// Here section == 1
			if (s.length() == 0 || isblank(s[0])) {
				action << s << '\n';
				continue;
			}
			// Now s[0] is not blank
			// New rule
			if (!re.empty()) {
				fmt::print("=={}==\n||{}||\n", re, qy::trim(action.str()));
				// TODO Add new RE
				action.str("");
			}
			// Now find the RE
			auto [p, ss] = get_re(s);
			re = std::move(ss);
			action << ((std::string_view)s).substr(p);
			// TODO Support multiple RE with one action
		}
	}

	std::pair<size_t, std::string> Lexer::get_re(const std::string& s) const {
		std::stack<char> bra; // Brackets stack

		auto match = [&bra](char c) {
			if (bra.empty() || bra.top() != c)
				throw syntax_error("Syntax error");
			bra.pop();
		};
		auto is_squared = [&bra]() {
			return !bra.empty() && bra.top() == '[';
		};

		std::string res;

		bool escaped = false;
		bool quoted = false;
		size_t brace_start = -1;
		for (size_t i = 0; i < s.length(); i++) {
			char c = s[i];
			if (escaped) { // Escape chars
				if (c == '0') c = '\0';
				else if (c == 'r') c = '\r';
				else if (c == 'n') c = '\n';
				else if (c == 't') c = '\t';
				else if (c == 'v') c = '\v';
				else if (c == 'b') c = '\b';
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
			switch (c) {
			case '.':
				c = WILDCARD;
				break;
			case '(':
			case '[':
				bra.push(c); break;
			case '{':
				brace_start = i;
				bra.push(c); break;
			case ')':
				match('('); break;
			case ']':
				match('['); break;
			case '}':
				match('{');
				res += definitions.at(s.substr(brace_start + 1, i - brace_start - 1));
				brace_start = -1;
				continue;
			case ' ':
				if (is_squared()) break; // Fallthrough trick
			case '\t':
				return { i, res };
			default:
				break;
			}
			if (brace_start == -1) // If outside the braces
				res += c;
		}
		if (quoted || escaped || !bra.empty())
			throw syntax_error("Syntax error");
		return { s.length(), res };
	}

}


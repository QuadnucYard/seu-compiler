#include "common/glexer.hpp"
#include "utils/exceptions.hpp"
#include <fmt/core.h>
#include <sstream>
#include <unordered_map>

namespace comp {
	GLexer::GLexer(std::istream& in) : in{in} { getc(); }

	GToken GLexer::scan_noop() {
		while (in) {
			scan_skip();
			if (peek == '%')
				return get_directive();
			if (peek == '<')
				return get_tag();
			if (peek == '\'')
				return get_char();
			if (peek == '"')
				return get_string();
			if (peek == '{')
				return get_codeblock();
			if (std::isdigit(peek))
				return get_num();
			if (std::isalpha(peek))
				return get_id();
			if (peek == ':' || peek == ';' || peek == '|')
				return get_op();
		}
		return {GToken::END, EOF};
	}

	void GLexer::scan_skip() {
		for (skip_ws(); skip_comment(); skip_ws())
			;
		skip_ws();
	}

	char GLexer::get_peek() const { return peek; }

	char GLexer::getc() {
		prev = peek;
		peek = in.get();
		if (prev == '\n') {
			column = 0;
			line++;
		} else if (peek == '\t')
			column += 4;
		else
			column++;
		return peek;
	}

	void GLexer::unget() {
		in.unget();
		in.unget();
		getc();
	}

	void GLexer::skip_ws() {
		for (; std::isspace(peek); peek = getc())
			;
	}

	bool GLexer::skip_comment() {
		if (peek == '/') {
			if (getc() == '/') {
				while (getc() != '\n')
					;
				return true;
			} else if (peek == '*') {
				while (!(getc() == '/' && prev == '*'))
					;
				getc();
				return true;
			} else {
				unget();
			}
		}
		return false;
	}

	GToken GLexer::get_directive() {
		string s{peek};
		if (getc() == '{')
			return get_prologue();
		/* else if (peek == '%') {
				getc();
				return {GToken::SEP, 0};
			} */
		do {
			s.push_back(peek);
		} while (std::isalnum(getc()));
		return {GToken::DIR, s};
	}

	GToken GLexer::get_num() {
		int v = 0;
		do {
			v = v * 10 + peek - '0';
		} while (std::isdigit(getc()));
		return {GToken::INT, v};
	}

	GToken GLexer::get_id() {
		string s;
		do {
			s.push_back(peek);
		} while (std::isalnum(getc()) || peek == '_');
		return {GToken::ID, s};
	}

	GToken GLexer::get_tag() {
		string s;
		do {
			s.push_back(peek);
		} while (getc() != '>');
		s.push_back(peek);
		getc();
		return {GToken::TAG, s};
	}

	GToken GLexer::get_char() {
		string s;
		do {
			s.push_back(peek);
		} while (getc() != '\'');
		s.push_back(peek);
		getc();
		return {GToken::CHAR, s};
	}

	GToken GLexer::get_string() {
		string s;
		do {
			s.push_back(peek);
		} while (getc() != '"');
		s.push_back(peek);
		getc();
		return {GToken::STRING, s};
	}

	GToken GLexer::get_op() {
		char c = peek;
		getc();
		return {GToken::OP, c};
	}

	GToken GLexer::get_regex(const std::unordered_map<string, string>& definitions) {
		int bra = 0;
		std::string s;
		while (!isspace(peek)) {
			if (peek == '\\') {
				s.push_back(peek);
				s.push_back(getc());
				getc();
			} else if (peek == '(') {
				s.push_back(peek);
				getc();
				bra++;
			} else if (peek == ')') {
				s.push_back(peek);
				getc();
				bra--;
			} else if (peek == '"')
				read_string(s);
			else if (peek == '[')
				read_closure(s, '[', ']');
			else if (peek == '{') {
				string m;
				read_closure(m, '{', '}');
				s.append(definitions.at(m.substr(1, m.length() - 2)));
			} else {
				s.push_back(peek);
				getc();
			}
		}
		return {GToken::RE, s};
	}

	GToken GLexer::get_codeblock() {
		string s;
		for (int bra = 1; bra > 0;) {
			s.push_back(peek);
			getc();
			if (peek == '{')
				bra++;
			else if (peek == '}')
				bra--;
			read_char(s);
			read_string(s);
			read_comment(s);
			read_comment_ml(s);
		}
		s.push_back(peek);
		getc();
		return {GToken::ACT, s};
	}

	GToken GLexer::get_prologue() {
		string s;
		for (getc(); !(peek == '}' && prev == '%');) {
			s.push_back(peek);
			getc();
			read_char(s);
			read_string(s);
			read_comment(s);
			read_comment_ml(s);
		}
		// 当前 peek == '}'
		s.pop_back();
		getc();
		return {GToken::PRO, s};
	}

	GToken GLexer::get_epilogue() {
		std::ostringstream iss;
		iss << in.rdbuf();
		return {GToken::EPI, iss.str()};
	}

	void GLexer::read_line(string& s) {
		/* string tmp;
		std::getline(in, tmp);
		s.push_back(peek);
		s.append(tmp);
		prev = '\n';
		line++;
		column = -1;
		getc(); */
		do {
			s.push_back(peek);
		} while (getc() != EOF && prev != '\n');
	}

	void GLexer::read_closure(string& s, char left, char right) {
		if (peek == left && prev != '\\') {
			do {
				s.push_back(peek);
			} while (getc() != right || prev == '\\');
			s.push_back(peek);
			getc();
		}
	}

	// 读完字符
	void GLexer::read_char(string& s) {
		if (peek == '\'' && prev != '\\') {
			do {
				s.push_back(peek);
			} while (getc() != '\'' || prev == '\\');
			s.push_back(peek);
			getc();
		}
	}

	void GLexer::read_string(string& s) { read_closure(s, '"', '"'); }

	void GLexer::read_string_unquoted(string& s) {
		if (peek == '"' && prev != '\\') {
			while (getc() != '"' || prev == '\\')
				s.push_back(peek);
			getc();
		}
	}

	void GLexer::read_comment(string& s) {
		if (peek == '/' && prev == '/') {
			do {
				s.push_back(peek);
			} while (getc() != '\n');
			s.push_back(peek); // 换行符加上
			getc();
		}
	}

	void GLexer::read_comment_ml(string& s) {
		if (peek == '*' && prev == '/') {
			do {
				s.push_back(peek);
			} while (!(getc() == '/' && prev == '*'));
			s.push_back(peek);
			getc();
		}
	}
} // namespace comp

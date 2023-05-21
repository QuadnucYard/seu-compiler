#include "common/glexer.hpp"
#include <sstream>

namespace comp {
	GLexer::GLexer(std::istream& in) : in{in} { getc(); }

	GToken GLexer::scan_noop() {
		while (in) {
			skip_ws();
			skip_comment();
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

	char GLexer::getc() {
		prev = peek;
		peek = in.get();
		if (peek == '\n') {
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

	void GLexer::skip_comment() {
		if (peek == '/') {
			if (getc() == '/') {
				while (getc() != '\n')
					;
			} else if (peek == '*') {
				while (!(getc() == '/' && prev == '*'))
					;
				getc();
			} else {
				unget();
			}
		}
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

	void GLexer::read_string(string& s) {
		if (peek == '"' && prev != '\\') {
			do {
				s.push_back(peek);
			} while (getc() != '"' || prev == '\\');
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

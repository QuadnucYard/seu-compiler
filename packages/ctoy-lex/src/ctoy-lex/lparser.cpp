#include "lex/lparser.hpp"
#include "utils/string_utils.hpp"
#include <cassert>

namespace comp {
	LParser::LParser(std::istream& in) : GParser{in} {}

	void LParser::parse() {
		next0();
		_declaration();
		_rules();
		if (section == 2 && !lexer.eof()) {
			epilogue = lexer.get_epilogue()._string();
		}
	}

	void LParser::_declaration() {
		while (section == 0) {
			if (check_section(false))
				break;
			switch (tok.type) {
			case GToken::DIR:
				_directive();
				break;
			case GToken::PRO:
				prologues.push_back(tok._string());
				next0();
				break;
			case GToken::ID:
				string a = tok._string();
				string b = _regex()._string();
				definitions.emplace(a, b);
				next0();
				break;
			}
		}
	}

	void LParser::_directive() {}

	void LParser::_rules() {
		while (!lexer.eof()) {
			_regex();
			if (tok._string() == "%%")
				break;

			int cnt = 1;
			rules.emplace_back(tok._string(), "");
			lexer.scan_skip();
			// 这里有个麻烦的问题，只有首个字符位于首列的才算是一条新的规则
			// 如果下一个是 '|'，那么判定有新的规则
			// 否则一直读到下一条规则
			while (lexer.get_peek() == '|') {
				lexer.getc();
				rules.emplace_back(_regex()._string(), "");
				lexer.scan_skip();
				cnt++;
			}
			string action;
			do {
				lexer.read_line(action);
			} while (isspace(lexer.get_peek()));
			action = string{qy::trim(action)};
			while (cnt--)
				rules[rules.size() - cnt - 1].action = action;
		}
		section++;
	}

	GToken& LParser::_regex() {
		lexer.scan_skip();
		return tok = lexer.get_regex(definitions);
	}

	void LParser::finalize() {}

} // namespace comp

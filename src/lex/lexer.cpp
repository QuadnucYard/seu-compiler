#include "lex/lexer.hpp"
#include "lex/regex.hpp"
#include "utils/string_utils.hpp"
#include <fmt/core.h>
#include <fstream>
#include <sstream>

namespace comp {

	struct Lexer::DefHandler {
		Lexer& lexer;

		DefHandler(Lexer& lexer) : lexer(lexer) {}

		void operator()(string&& s) {
			// Here process macro.
			// Assume that it obeys "define before use"
			size_t p1 = s.find_first_of(" \t"), p2 = s.find_first_not_of(" \t", p1);
			if (p1 != std::string::npos && p2 != std::string::npos)
				lexer.definitions.emplace(s.substr(0, p1), lexer.get_re(s.substr(p2)).second);
		}
	};

	struct Lexer::RuleHandler {
		Lexer& lexer;
		std::ostringstream action; // Action string
		std::string re;			   // The leading expression
		std::vector<string> all_re;
		bool finalized{false};

		RuleHandler(Lexer& lexer) : lexer(lexer) {}

		void operator()(string&& s) {
			if (s.length() == 0 || isblank(s[0])) {
				// Still previous rule.
				action << s << '\n';
				return;
			}
			// Now s[0] is not blank
			// New rule, and process previous rule
			if (!re.empty()) {
				// fmt::print("=={}==\n||{}||\n", re, qy::trim(action.str()));
				all_re.push_back(re);
				lexer.actions.push_back(string{qy::trim(action.str())});
				// lexer.dfa_builder.add_re(re);
				action.str("");
			}
			// Now find the RE
			auto [p, ss] = lexer.get_re(s);
			re = std::move(ss);
			action << ((std::string_view)s).substr(p);
			// TODO Support multiple RE with one action
		}

		void finalize() {
			if (finalized)
				return;
			finalized = true;
			if (!re.empty()) {
				all_re.push_back(re);
				lexer.actions.push_back(string{qy::trim(action.str())});
			}
			// TODO Generate and output FA
		}
	};

	void Lexer::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

		std::ofstream yylex_file("lex.yy.c");

		SourceHandler h(yylex_file);
		DefHandler hDef(*this);
		RuleHandler hRule(*this);

		for (std::string s; std::getline(source_file, s); h.lineno++) {
			if (h.code(s))
				continue;
			else if (s == "%%") {
				if (++h.section == 2)
					hRule.finalize();
				continue;
			}
			if (h.section == 0)
				hDef(std::move(s));
			else // Here section == 1
				hRule(std::move(s));
		}
		hRule.finalize();
		// 这里好像没处理最后一个rule
		// 需要加个判断，is_valid 表示有读rule
	}

	std::pair<size_t, std::string> Lexer::get_re(const std::string& s) const {
		return unescape_regex(s, definitions);
	}

} // namespace comp

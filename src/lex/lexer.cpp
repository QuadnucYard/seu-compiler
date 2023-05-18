#include "lex/lexer.hpp"
#include "lex/genc.hpp"
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
				lexer.dfa_builder.add_re(re);
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
				lexer.dfa_builder.add_re(re);
			}
			// Catchall handler
			lexer.dfa_builder.add_re(lexer.get_re("[.\n]").second);
			lexer.actions.push_back("ECHO;");
			// lexer.dfa_builder.add_re("");
			// lexer.actions.push_back({});
		}
	};

	Lexer::Lexer(const Options& options) : options{options} {}

	void Lexer::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

		SourceHandler h;
		DefHandler hDef(*this);
		RuleHandler hRule(*this);

		LexCodeGen codegen(*this);

		for (std::string s; std::getline(source_file, s); h.lineno++) {
			if (h.code(s))
				continue;
			else if (s == "%%") {
				++h.section;
				if (h.section == 1) {
					codegen.templater().set_string("[[USER_CODE_1]]",
												   h.code_content + "[[USER_CODE_1]]");
					h.code_content.clear();
				}
				continue;
			}
			if (h.section == 0)
				hDef(std::move(s));
			else // Here section == 1
				hRule(std::move(s));
		}
		hRule.finalize();

		auto&& [nfa, dfa] = dfa_builder.join_nfa();
		// 增加一个处理0转换的点
		dfa.graph.resize(dfa.size() + 1);
		dfa.accept_states.push_back(static_cast<vid_t>(actions.size()));
		if (!options.scanner_nfa_dot.empty())
			nfa.to_dot(options.scanner_nfa_dot);
		if (!options.scanner_dfa_dot.empty())
			dfa.to_dot(options.scanner_dfa_dot);

		codegen.templater().set_string("[[USER_CODE_1]]", "");
		codegen.templater().set_string("[[USER_CODE_2]]", "");
		codegen.templater().set_string("[[USER_CODE_3]]", h.code_content);

		codegen(dfa);
		codegen.dump(options.outfile);
	}

	std::pair<size_t, std::string> Lexer::get_re(const std::string& s) const {
		return unescape_regex(s, definitions);
	}

} // namespace comp

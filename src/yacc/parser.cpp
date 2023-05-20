#include "yacc/parser.hpp"
#include "utils/exceptions.hpp"
#include "utils/string_utils.hpp"
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fstream>
#include <sstream>
#include <stack>

extern const char* yacc_tmpl;

namespace comp {

	static char unescape(char c) {
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

	static char unescape_char(std::string_view s) {
		if (s[1] == '\\')
			return unescape(s[2]);
		return s[1];
	}

	static string unescape_string(std::string_view s) {
		string res;
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

	struct Parser::DeclHandler {
		Parser& parser;
		std::ostream& tab_inc_file;

		std::vector<std::pair<token::assoc_flag, std::vector<string>>> prec;
		std::unordered_map<string, string> nterm_types;

		DeclHandler(Parser& parser, std::ostream& tab_inc_file) :
			parser(parser), tab_inc_file(tab_inc_file) {
			parser.analyzer.tokens = {{"$end", "$", "", 0}};
			parser.translate.resize(256, -1);
			parser.translate[0] = 0;
		}

		void operator()(string&& s) {
			std::istringstream iss(s);
			std::string kw;
			iss >> kw;
			if (kw.starts_with("%")) {
				if (kw == "%start")
					iss >> parser.start_symbol;
				else if (kw == "%token") {
					string tag;
					while (iss >> kw) {
						if (kw.starts_with("<") && kw.ends_with(">"))
							tag = std::move(kw);
						else if (qy::is_quoted(kw)) {
							parser.analyzer.tokens.back().literal = unescape_string(kw);
						} else if (auto x = std::atoi(kw.c_str()); x > 0) {
							parser.analyzer.tokens.back().num = x;
						} else {
							sid_t tid = static_cast<sid_t>(parser.analyzer.tokens.size());
							parser.symbol_map[kw] = tid;
							parser.translate.emplace_back(tid);
							parser.analyzer.tokens.emplace_back(kw, "", tag, 0);
							fmt::print(tab_inc_file, "\t{} = {},\n", kw,
									   parser.translate.size() - 1);
						}
					}
				} else if (kw == "%type") {
					string tag;
					while (iss >> kw) {
						if (kw.starts_with("<") && kw.ends_with(">"))
							tag = std::move(kw);
						else
							nterm_types.emplace(std::move(kw), tag);
					}
				} else if (kw == "%left" || kw == "%right" || kw == "%nonassoc") {
					auto flag = kw == "%left"	 ? token::assoc_flag::LEFT
								: kw == "%right" ? token::assoc_flag::RIGHT
												 : token::assoc_flag::NONE;
					std::vector<string> tok;
					while (iss >> kw)
						tok.push_back(std::move(kw));
					prec.emplace_back(flag, std::move(tok));
				} else if (kw == "%union") {
				}
			}
		}

		void finalize() const {
			sid_t i = 1;
			for (auto&& [assoc, a] : prec) {
				for (auto&& s : a) {
					auto& t = parser.analyzer.get_token(s);
					t.assoc = assoc;
					t.prec = i;
				}
				i++;
			}
			for (auto&& [t, tag] : nterm_types) {
				parser.analyzer.get_nterm(t).tag = tag;
			}
		}
	};

	struct Parser::RulesHandler {
		Parser& parser;
		std::istringstream iss;
		RawRule rule;
		string prev;
		string action;
		bool action_started;
		string t;
		bool ended{false};

		RulesHandler(Parser& parser) : parser(parser) {
			// BUG: Here aug_start is empty!
			parser.rules.push_back({aug_start, {}});
			parser.symbol_map.emplace(aug_start, 0);
			parser.actions.push_back("");
			action_started = false;
		}

		void operator()(string&& s) {
			iss.str(s);
			iss.clear(); // Must use this to clear state!
			while (iss >> t) {
				// BUG 需要分词，用空格分隔不靠谱  但目前就假设必须要空格分隔
				if (t == ":") {
					rule.lhs = std::move(prev);
					parser.symbol_map.emplace(rule.lhs, -static_cast<sid_t>(parser.rules.size()));
					rule.rhs.push_back({});
				} else if (t == "|") {
					rule.rhs.push_back({});
					parser.actions.push_back(string{qy::trim(action)});
					action.clear();
				} else if (t == ";") {
					parser.rules.push_back(std::move(rule));
					rule.rhs.clear();
					parser.actions.push_back(string{qy::trim(action)});
					action.clear();
				} else if (action_started && t == "}") {
					action += t;
					action_started = false;
				} else if (action_started || t == "{") {
					action_started = true;
					action += t;
					action += ' ';
				} else if (!rule.rhs.empty()) {
					if (t == "%empty") {
					} else if (t == "%prec") {
					} else if (!t.starts_with('%')) {
						rule.rhs.back().push_back(t);
					} else {
						throw syntax_error(fmt::format("Unknown definition token: {}.", t));
					}
				}
				prev = std::move(t);
			}
		}

		/// @brief Convert raw rules into discretized rules, and generate FA.
		void finalize() {
			if (ended)
				return;
			ended = true;
			auto& ana = parser.analyzer;

			parser.rules[0].rhs = {{parser.start_symbol}};

			size_t k = 0;
			for (size_t i = 0; i < parser.rules.size(); i++) {
				for (auto&& r : parser.rules[i].rhs) {
					symbol_vec sv;
					for (auto& s : r) {
						// Register tokens
						if (!parser.symbol_map.contains(s)) {
							auto tid = static_cast<sid_t>(ana.tokens.size());
							parser.symbol_map.emplace(s, tid);
							char char_val = unescape_char(s);
							parser.translate[char_val] = tid;
							ana.tokens.emplace_back(s, unescape_string(s), "", char_val);
						}
						sv.push_back(parser.get_symbol_id(s));
					}
					ana.rules.emplace_back(static_cast<sid_t>(ana.rules.size()),
										   static_cast<sid_t>(i), sv, parser.actions[k++]);
				}
			}
			// 必须要在最后，这样才能保证rules是固定的，span有效
			for (size_t i = 0, s = 0; i < parser.rules.size(); i++) {
				size_t sz = parser.rules[i].rhs.size();
				ana.nterms.emplace_back(parser.rules[i].lhs, std::span{ana.rules.begin() + s, sz});
				s += sz;
			}
		}
	};

	Parser::Parser(const Options& options) : options{options} {}

	void comp::Parser::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

		std::ofstream tab_inc_file(options.header_file);

		SourceHandler h;

		DeclHandler hDecl(*this, tab_inc_file);
		RulesHandler hRule(*this);

		yacc_code code_gen{*this, yacc_tmpl};

		tab_inc_file << "enum yytokentype {\n";

		for (string s; std::getline(source_file, s); h.lineno++) {
			if (h.code(s))
				continue;
			else if (s.starts_with("%%")) {
				++h.section;
				if (h.section == 1) {
					tab_inc_file << "};\n";
					tab_inc_file << R"(
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;)";
					code_gen.templater().set_string("[[USER_CODE_1]]",
													h.code_content + "[[USER_CODE_1]]");
					h.code_content.clear();
				}
				continue;
			}
			if (s.empty())
				continue;
			if (h.section == 0) {
				hDecl(std::move(s));
			} else {
				// Here section == 1
				// ! Currently not support actions
				hRule(std::move(s));
			}
		}
		// End process
		hRule.finalize();
		hDecl.finalize();

		code_gen.templater().set_string("[[USER_CODE_1]]", "");
		code_gen.templater().set_string("[[USER_CODE_2]]", "");
		code_gen.templater().set_string("[[USER_CODE_3]]", h.code_content);

		auto pt = analyzer.process(options);
		code_gen.gen(pt);

		code_gen.dump(options.outfile);
	}

	sid_t comp::Parser::get_symbol_id(const string& name) const { return symbol_map.at(name); }

} // namespace comp
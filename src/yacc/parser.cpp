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

	struct Parser::DeclHandler {
		Parser& parser;
		std::ostream& tab_inc_file;

		DeclHandler(Parser& parser, std::ostream& tab_inc_file) :
			parser(parser), tab_inc_file(tab_inc_file) {
			parser.analyzer.tokens.emplace_back("$");
		}

		void operator()(string&& s) {
			std::istringstream iss(s);
			std::string kw;
			iss >> kw;
			if (kw.starts_with("%")) {
				if (kw == "%start")
					iss >> parser.start_symbol;
				else if (kw == "%token") {
					while (iss >> kw) {
						auto token_index = static_cast<sid_t>(parser.analyzer.tokens.size());
						parser.symbol_map[kw] = token_index;
						parser.analyzer.tokens.emplace_back(kw);
						fmt::print(tab_inc_file, "\t{} = {},\n", kw, token_index);
					}
				}
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

		static string unescape(std::string_view s) {
			// s = s.substr(1, s.length() - 2);
			return string{s};
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
					rule.rhs.back().push_back(unescape(t));
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

			size_t k=0;
			for (size_t i = 0; i < parser.rules.size(); i++) {
				for (auto&& r : parser.rules[i].rhs) {
					symbol_vec sv;
					for (auto& s : r) {
						// Register tokens
						if (!parser.symbol_map.contains(s)) {
							parser.symbol_map.emplace(s, static_cast<sid_t>(ana.tokens.size()));
							ana.tokens.emplace_back(s);
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
				ana.nonterminals.emplace_back(parser.rules[i].lhs,
											  std::span{ana.rules.begin() + s, sz});
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

		yacc_code code_gen{yacc_tmpl};

		tab_inc_file << "enum yytokentype {\n";

		for (string s; std::getline(source_file, s); h.lineno++) {
			if (h.code(s))
				continue;
			else if (s == "%%") {
				++h.section;
				if (h.section == 1) {
					tab_inc_file << "};\n";
					tab_inc_file << R"(
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;)";
					code_gen.templater().set_string("[[USER_CODE_1]]",
													h.code_content + "[[USER_CODE_1]]");
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

		code_gen.templater().set_string("[[USER_CODE_1]]", "");
		code_gen.templater().set_string("[[USER_CODE_2]]", "");
		code_gen.templater().set_string("[[USER_CODE_3]]", h.code_content);

		auto pt = analyzer.process(options);
		code_gen.gen(pt, analyzer);

		code_gen.dump(options.outfile);
	}

	sid_t comp::Parser::get_symbol_id(const string& name) const { return symbol_map.at(name); }

} // namespace comp
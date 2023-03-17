#include "yacc/parser.hpp"
#include "utils/exceptions.hpp"
#include "utils/string_utils.hpp"
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fstream>
#include <sstream>
#include <stack>

namespace comp {

	struct Parser::DeclHandler {
		Parser& parser;
		std::ostream& tab_inc_file;
		int token_index = 258;

		DeclHandler(Parser& parser, std::ostream& tab_inc_file): parser(parser), tab_inc_file(tab_inc_file) {}

		void operator()(string&& s) {
			std::istringstream iss(s);
			std::string kw;
			iss >> kw;
			if (kw.starts_with("%")) {
				if (kw == "%start")
					iss >> parser.start_symbol;
				else if (kw == "%token") {
					while (iss >> kw)
						fmt::print(tab_inc_file, "\t{} = {},\n", kw, token_index++);
				}
			}
		}
	};

	struct Parser::RulesHandler {
		Parser& parser;
		std::istringstream iss;
		GrammarRule rule;
		string prev;
		string t;

		RulesHandler(Parser& parser): parser(parser) {}

		void operator()(string&& s) {
			iss.str(s);
			iss.clear(); // Must use this to clear state!
			while (iss >> t) {
				if (t == ":") {
					rule.lhs = std::move(prev);
					rule.rhs.push_back({});
				} else if (t == "|") {
					rule.rhs.push_back({});
				} else if (t == ";") {
					parser.rules.push_back(std::move(rule));
					rule = {};
				} else if (!rule.rhs.empty()) {
					rule.rhs.back().push_back(t);
				}
				prev = std::move(t);
			}
		}
	};

	void comp::Parser::process(const fs::path& src_path) {
		std::ifstream source_file(src_path);
		if (!source_file.is_open())
			throw std::runtime_error("File not found");

		std::ofstream tab_inc_file("y.tab.h");
		std::ofstream tab_src_file("y.tab.c");

		SourceHandler h(tab_src_file);

		DeclHandler hDecl(*this, tab_inc_file);
		RulesHandler hRule(*this);

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
				} else if (h.section == 2) {
					// TODO Generate and output FA
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
	}
}
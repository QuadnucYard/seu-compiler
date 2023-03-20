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
		int token_index = 128;

		DeclHandler(Parser& parser, std::ostream& tab_inc_file) :
			parser(parser), tab_inc_file(tab_inc_file) {
			// 暂时不考虑 token id 离散化
			for (int i = 32; i < 128; i++) {
				parser.symbol_map.emplace(fmt::format("'{}'", static_cast<char>(i)), i);
			}
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
						parser.symbol_map[kw] = token_index;
						fmt::print(tab_inc_file, "\t{} = {},\n", kw, token_index++);
					}
				}
			}
			parser.analyzer.token_num = token_index;
		}
	};

	struct Parser::RulesHandler {
		Parser& parser;
		std::istringstream iss;
		RawRule rule;
		string prev;
		string t;
		bool ended;

		RulesHandler(Parser& parser) : parser(parser) {
			parser.rules.push_back({"$start"});
			parser.symbol_map.emplace("$start", 0);
		}

		void operator()(string&& s) {
			iss.str(s);
			iss.clear(); // Must use this to clear state!
			while (iss >> t) {
				// BUG 需要分词，用空格分隔不靠谱
				if (t == ":") {
					rule.lhs = std::move(prev);
					parser.symbol_map.emplace(rule.lhs, -static_cast<sid_t>(parser.rules.size()));
					rule.rhs.push_back({});
				} else if (t == "|") {
					rule.rhs.push_back({});
				} else if (t == ";") {
					parser.rules.push_back(std::move(rule));
					rule.rhs.clear();
				} else if (!rule.rhs.empty()) {
					rule.rhs.back().push_back(t);
				}
				prev = std::move(t);
			}
		}

		/// @brief Convert raw rules into discretized rules, and generate FA.
		void finalize() {
			ended = true;
			auto& ana = parser.analyzer;

			parser.rules[0].rhs = {{parser.start_symbol}};

			for (size_t i = 0; i < parser.rules.size(); i++) {
				for (auto&& r : parser.rules[i].rhs) {
					symbol_vec sv;
					for (auto&& s : r)
						sv.push_back(parser.get_symbol_id(s));
					ana.rules.emplace_back(static_cast<sid_t>(i), sv, "");
					// TODO action也是与rule对应的
				}
			}
			ana.rules[0].rhs.push_back(SyntacticAnalyzer::END_MARKER); // Add end-marker
			// 必须要在最后，这样才能保证rules是固定的，span有效
			for (size_t i = 0, s = 0; i < parser.rules.size(); i++) {
				size_t sz = parser.rules[i].rhs.size();
				ana.nonterminals.emplace_back(parser.rules[i].lhs,
											  std::span{ana.rules.begin() + s, sz},
											  parser.analyzer.token_num);
				s += sz;
			}
			parser.analyzer.process();
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
					hRule.finalize();
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
		if (!hRule.ended) {
			hRule.finalize();
		}
		/**
		 * 终结符和非终结符
		 * 全部离散化。终结符的id在之前已经有了，但还需要进一步离散化
		 * 终结符的数量多少？对应token数量，256起步
		 * 这个级别，bitset为64B，umap每个元素12B起步……集合操作也不如bitset
		 * 直接上mini_set吧
		 */
	}

	sid_t comp::Parser::get_symbol_id(const string& name) const {
		return symbol_map.at(name);
	}

} // namespace comp
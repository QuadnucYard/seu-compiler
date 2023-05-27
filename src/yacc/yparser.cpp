#include "yacc/yparser.hpp"
#include "common/exceptions.hpp"
#include "utils/string_utils.hpp"
#include "yacc/analyzer.hpp"
#include <cassert>
#include <fmt/std.h>

namespace comp {
	YParser::YParser(std::istream& in, SyntacticAnalyzer& analyzer) :
		GParser{in}, analyzer{analyzer} {
		analyzer.tokens = {{"$end", "$", "", 0}};
		analyzer.translate.resize(256, -1);
		analyzer.translate[0] = 0;

		rules.push_back({aug_start, {{start_symbol}}});
		symbol_map.emplace(aug_start, 0);
		actions.push_back("");
	}

	void YParser::parse() {
		next0();
		_declaration();
		_rules();
		if (section == 2) {
			epilogue = lexer.get_epilogue()._string();
		}
	}

	void YParser::_declaration() {
		while (section == 0) {
			if (check_section())
				break;
			switch (tok.type) {
			case GToken::DIR:
				_directive();
				break;
			case GToken::PRO:
				prologues.push_back(tok._string());
				next0();
				break;
			}
		}
	}

	void YParser::_directive() {
		auto dir = std::get<string>(tok.val);
		if (dir == "%start") {
			start_symbol = next0()._string();
			next0();
		} else if (dir == "%token") {
			string tag;
			while (next0().type != GToken::DIR) {
				switch (tok.type) {
				case GToken::TAG:
					tag = std::move(tok._string());
					break;
				case GToken::STRING:
					analyzer.tokens.back().literal = qy::unescape_string(tok._string());
					break;
				case GToken::INT:
					analyzer.tokens.back().num = tok._int();
					used_num.insert(tok._int());
					break;
				case GToken::ID: {
					sid_t tid = static_cast<sid_t>(analyzer.tokens.size());
					auto kw = tok._string();
					symbol_map[kw] = tid;
					analyzer.translate.emplace_back(tid);
					analyzer.tokens.emplace_back(kw, "", tag, 0, true);
					break;
				}
				default:
					return;
				}
			}
		} else if (dir == "%type") {
			string tag;
			while (next0().type != GToken::DIR) {
				if (tok.type == GToken::TAG)
					tag = tok._string();
				else if (tok.type == GToken::ID)
					nterm_types.emplace(tok._string(), tag);
				else
					break;
			}
		} else if (dir == "%union") {
			assert(next0().type == GToken::ACT);
			union_type = tok._string();
			next0();
		} else if (dir == "%left" || dir == "%right" || dir == "%nonassoc") {
			auto flag = dir == "%left"	  ? token::assoc_flag::LEFT
						: dir == "%right" ? token::assoc_flag::RIGHT
										  : token::assoc_flag::NONE;
			std::vector<string> tokens;
			while (next0().type != GToken::DIR)
				tokens.push_back(std::move(tok._string()));
			prec.emplace_back(flag, std::move(tokens));
		} else {
			throw syntax_error("Unknown directive.");
		}
	}

	void YParser::_rules() {
		RawRule rule;
		while (section == 1 && tok.type != GToken::END) {
			if (check_section())
				break;
			assert(tok.type == GToken::ID);
			rule.lhs = tok._string();
			assert(next0().type == GToken::OP && tok._char() == ':');
			symbol_map.emplace(rule.lhs, -static_cast<sid_t>(rules.size()));
			rule.rhs.push_back({});
			actions.push_back({});
			for (next0();; next0()) {
				if (tok.type == GToken::OP) {
					auto op = tok._char();
					if (op == '|') {
						rule.rhs.push_back({});
						actions.push_back({});
					} else if (op == ';') {
						rules.push_back(std::move(rule));
						rule.rhs.clear();
						break;
					}
				} else if (tok.type == GToken::ACT) {
					actions.back() = tok._string();
				} else if (tok.type == GToken::DIR) {
					auto dir = tok._string();
					if (dir == "%empty") {
					} else if (dir == "%prec") {
					}
				} else if (tok.type == GToken::ID || tok.type == GToken::CHAR) {
					rule.rhs.back().push_back(tok._string());
				} else {
					throw syntax_error(
						fmt::format("Unknown definition token: <{}>{}.", tok.type, tok.val));
				}
			}
			next0();
		}
	}

	void YParser::finalize() {
		// rules
		rules[0].rhs[0][0] = start_symbol;
		for (size_t i = 0, k = 0; i < rules.size(); i++) {
			for (auto&& r : rules[i].rhs) {
				symbol_vec sv;
				for (auto& s : r) {
					// Register tokens
					if (!symbol_map.contains(s)) {
						auto tid = static_cast<sid_t>(analyzer.tokens.size());
						symbol_map.emplace(s, tid);
						char char_val = qy::unescape_char(s);
						analyzer.translate[char_val] = tid;
						analyzer.tokens.emplace_back(s, qy::unescape_string(s), "", char_val);
					}
					sv.push_back(symbol_map.at(s));
				}
				analyzer.rules.emplace_back(static_cast<sid_t>(analyzer.rules.size()),
											static_cast<sid_t>(i), sv, actions[k++]);
			}
		}
		// 生成非终结符
		// 必须要在最后，这样才能保证rules是固定的，span有效
		for (size_t i = 0, s = 0; i < rules.size(); i++) {
			size_t sz = rules[i].rhs.size();
			analyzer.nterms.emplace_back(rules[i].lhs, std::span{analyzer.rules.begin() + s, sz});
			s += sz;
		}
		// 处理 token 的标号
		for (sid_t tid = 256; auto& t : analyzer.tokens | std::views::drop(1)) {
			if (t.num == 0 && t.is_defined) {
				while (used_num.contains(tid))
					++tid;
				t.num = tid++;
			}
		}
		// 处理类型、结合性、优先级
		for (sid_t i = 1; auto&& [assoc, a] : prec) {
			for (auto&& s : a) {
				auto& t = analyzer.get_token(s);
				t.assoc = assoc;
				t.prec = i;
			}
			i++;
		}
		for (auto&& [t, tag] : nterm_types) {
			analyzer.get_nterm(t).tag = tag;
		}
	}
} // namespace comp

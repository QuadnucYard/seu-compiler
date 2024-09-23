#include "./yparser.hpp"
#include "./analyzer.hpp"
#include "ctoy-grammar/gtoken.hpp"
#include "ctoy-utils/exceptions.hpp"
#include "ctoy-utils/string_utils.hpp"
#include <cassert>
#include <fmt/std.h>

namespace comp {
YParser::YParser(std::istream& in, SyntacticAnalyzer& analyzer) : GParser{in}, analyzer{analyzer} {
    analyzer.tokens = {{"$end", "$", "", 0}};
    analyzer.translate.resize(256, -1);
    analyzer.translate[0] = 0;
    rules.push_back({aug_start, {{{{start_symbol}}, "", ""}}});
    symbol_map.emplace(aug_start, 0);
}

void YParser::parse() {
    next0();
    _declaration();
    _rules();
    if (section == 2) { epilogue = lexer.get_epilogue().as_string(); }
}

void YParser::_declaration() {
    while (section == 0) {
        if (check_section()) break;
        switch (tok.kind) {
        case GTokenKind::DIR: _directive(); break;
        case GTokenKind::PRO:
            prologues.push_back(tok.as_string());
            next0();
            break;
        default: break;
        }
    }
}

void YParser::_directive() {
    auto dir = std::get<string>(tok.val);
    if (dir == "%start") {
        start_symbol = next0().as_string();
        next0();
    } else if (dir == "%token") {
        string tag;
        while (next0().kind != GTokenKind::DIR) {
            switch (tok.kind) {
            case GTokenKind::TAG: tag = tok.as_string(); break;
            case GTokenKind::STRING:
                analyzer.tokens.back().literal = qy::unescape_string(tok.as_string());
                break;
            case GTokenKind::INT:
                analyzer.tokens.back().num = tok.as_int();
                used_num.insert(tok.as_int());
                break;
            case GTokenKind::ID: {
                sid_t tid = static_cast<sid_t>(analyzer.tokens.size());
                auto kw = tok.as_string();
                symbol_map[kw] = tid;
                analyzer.translate.emplace_back(tid);
                analyzer.tokens.emplace_back(kw, "", tag, 0, true);
                break;
            }
            default: return;
            }
        }
    } else if (dir == "%type") {
        string tag;
        while (next0().kind != GTokenKind::DIR) {
            if (tok.kind == GTokenKind::TAG) tag = tok.as_string();
            else if (tok.kind == GTokenKind::ID) nterm_types.emplace(tok.as_string(), tag);
            else break;
        }
    } else if (dir == "%union") {
        assert(next0().kind == GTokenKind::ACT);
        union_type = tok.as_string();
        next0();
    } else if (dir == "%left" || dir == "%right" || dir == "%nonassoc") {
        auto flag = dir == "%left"    ? token::assoc_flag::LEFT
                    : dir == "%right" ? token::assoc_flag::RIGHT
                                      : token::assoc_flag::NONE;
        std::vector<string> tokens;
        while (next0().kind != GTokenKind::DIR)
            tokens.push_back(tok.as_string());
        prec.emplace_back(flag, std::move(tokens));
    } else {
        throw syntax_error("Unknown directive.");
    }
}

void YParser::_rules() {
    RawRuleGroup group;
    while (section == 1 && tok.kind != GTokenKind::END) {
        if (check_section(false)) break;
        assert(tok.kind == GTokenKind::ID);
        group.lhs = tok.as_string();
        assert(next0().kind == GTokenKind::OP && tok.as_char() == ':');
        symbol_map.emplace(group.lhs, -static_cast<sid_t>(rules.size()));
        group.rules.push_back({});
        for (next0();; next0()) {
            if (tok.kind == GTokenKind::OP) {
                auto op = tok.as_char();
                if (op == '|') {
                    group.rules.push_back({});
                } else if (op == ';') {
                    rules.push_back(std::move(group));
                    group.rules.clear();
                    break;
                }
            } else if (tok.kind == GTokenKind::ACT) {
                group.rules.back().action = tok.as_string();
            } else if (tok.kind == GTokenKind::DIR) {
                auto dir = tok.as_string();
                if (dir == "%empty") {
                } else if (dir == "%prec") {
                    group.rules.back().prec = next0().as_string();
                }
            } else if (tok.kind == GTokenKind::ID || tok.kind == GTokenKind::CHAR) {
                string s = tok.as_string();
                group.rules.back().rhs.push_back(s);
            } else {
                // throw syntax_error(
                // 	fmt::format("Unknown definition token: <{}>{}.", tok.type, tok.val));
            }
        }
        next0();
    }
}

void YParser::finalize() {
    rules[0].rules[0].rhs[0] = start_symbol;
    // Register tokens
    for (auto& group : rules) {
        for (auto& r : group.rules) {
            for (auto& s : r.rhs) {
                if (!symbol_map.contains(s)) {
                    auto tid = static_cast<sid_t>(analyzer.tokens.size());
                    symbol_map.emplace(s, tid);
                    char char_val = qy::unescape_char(s);
                    analyzer.translate[char_val] = tid;
                    analyzer.tokens.emplace_back(s, qy::unescape_string(s), "", char_val);
                }
            }
        }
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
    // rules
    for (size_t i = 0; i < rules.size(); i++) {
        for (auto&& r : rules[i].rules) {
            symbol_vec sv;
            for (auto& s : r.rhs)
                sv.push_back(symbol_map.at(s));
            analyzer.rules.emplace_back(
                static_cast<sid_t>(analyzer.rules.size()), static_cast<sid_t>(i), std::move(sv),
                r.prec.empty() ? 0 : analyzer.tokens[symbol_map.at(r.prec)].prec, r.action);
        }
    }
    // 生成非终结符
    // 必须要在最后，这样才能保证rules是固定的，span有效
    for (size_t s = 0; auto& group : rules) {
        size_t sz = group.rules.size();
        analyzer.nterms.emplace_back(group.lhs, std::span{analyzer.rules.begin() + s, sz});
        s += sz;
    }
    // 非终结符的tag
    for (auto&& [t, tag] : nterm_types) {
        analyzer.get_nterm(t).tag = tag;
    }
    // 求每个rule的优先级
    for (auto&& rule : analyzer.rules) {
        if (rule.prec > 0) continue;
        // 用最后一个非终结符的优先级表示这个rule的优先级
        for (auto s : std::views::reverse(rule.rhs)) {
            if (s > 0) {
                rule.prec = analyzer.tokens[s].prec;
                break;
            }
        }
    }
}
} // namespace comp

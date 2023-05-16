#include "yacc/analyzer.hpp"
#include "utils/graph.hpp"
#include "utils/graphviz.hpp"
#include "utils/myalgo.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <iterator>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <tl/enumerate.hpp>
#include <unordered_set>

using tl::views::enumerate;

namespace comp {

	SyntacticAnalyzer::item_set SyntacticAnalyzer::item_set::get_goto(sid_t symbol) const {
		item_set result;
		for (auto& item : items) {
			if (item.has_next() && item.next() == symbol)
				result.items.push_back(item.next_item());
		}
		result.kernel_size = result.items.size();
		return result;
	}

	bool SyntacticAnalyzer::item_set::kernel_equals(const item_set& o) const {
		return kernel_size == o.kernel_size &&
			   std::ranges::equal(items.begin(), items.begin() + kernel_size, o.items.begin(),
								  o.items.begin() + o.kernel_size, {}, &item::key, &item::key);
	}

	bool SyntacticAnalyzer::item_set::operator==(const item_set& o) const {
		return kernel_size == o.kernel_size &&
			   std::equal(items.begin(), items.begin() + kernel_size, o.items.begin(),
						  o.items.begin() + o.kernel_size);
	}

	parsing_table SyntacticAnalyzer::process() {
		get_nullables();
		get_firsts();

		// Print firsts
		// for (auto& t : nonterminals)
		// 	fmt::print("First({})={}\n", t.name, to_string(t.first));
		// fmt::print("\n");

		auto sg = get_LR1_states();
		if (sg.states.size() <= 100)
			to_dot(sg, "test_lr1.dot");
		auto pt = get_LR1_table(sg);
		auto pt2 = get_LALR1_table(sg, pt);
		compress_table(pt2);
		compress_table_more(pt2);
		return pt2;
	}

	const string& SyntacticAnalyzer::get_symbol_name(sid_t sym) const {
		return sym >= 0 ? tokens[sym] : nonterminals[-sym].name;
	}

	string SyntacticAnalyzer::to_string(const symbol_set& set) const {
		string s{};
		for (size_t i = 0; i < tokens.size(); i++)
			if (set.test(i))
				s.append(tokens[i]);
		return "{" + s + "}";
	}

	string SyntacticAnalyzer::to_string(const item& it) const {
		string s{nonterminals[it.prod->lhs].name};
		s.append(" ->");
		for (size_t i = 0; i < it.prod->rhs.size(); i++) {
			auto sym = it.prod->rhs[i];
			s.push_back(std::cmp_equal(i, it.dot) ? '.' : ' ');
			if (sym >= 0)
				s.append(tokens[sym]);
			else
				s.append(nonterminals[-sym].name);
		}
		if (!it.has_next())
			s.push_back('.');
		s.append("\t").append(to_string(it.follow));
		return s;
	}

	string SyntacticAnalyzer::to_string(const item_set& is) const {
		string s{};
		for (auto& it : is.items)
			s.append(to_string(it)).append("\n");
		return s;
	}

	void SyntacticAnalyzer::to_dot(const state_graph& sg, const fs::path& path) const {
		const auto _to_string_p = [this](const item& it) {
			string s{nonterminals[it.prod->lhs].name};
			s.append(R"( \-\>)");
			for (size_t i = 0; i < it.prod->rhs.size(); i++) {
				auto sym = it.prod->rhs[i];
				s.append(std::cmp_equal(i, it.dot) ? "·" : " ");
				s.append(qy::graphviz::label_escape(get_symbol_name(sym)));
			}
			if (!it.has_next())
				s.append("·");
			s.append("\\l");
			return s;
		};
		const auto _to_string_f = [this](const item& it) {
			string s2{};
			for (size_t i = 0; i < tokens.size(); i++)
				if (it.follow.test(i))
					s2.append(qy::graphviz::label_escape(tokens[i]));
			s2.append("\\n");
			return s2;
		};
		qy::graphviz::digraph dot{path.string(), "LR(1)"};
		dot.rankdir("LR").node_option("shape", "record").node_option("fontname", "Consolas");
		for (auto&& [i, s] : enumerate(sg.states)) {
			dot.node(i, fmt::format("I{} | {{ {} | {} }}", i,
									fmt::join(std::views::transform(s.items, _to_string_p), " "),
									fmt::join(std::views::transform(s.items, _to_string_f), " ")));
		}
		for (auto&& [u, e] : sg.atn) {
			auto&& [v, w] = e;
			dot.edge(u, v, get_symbol_name(w));
		}
	}

	symbol_set SyntacticAnalyzer::single_set(sid_t s) const {
		symbol_set set;
		set.set(s);
		return set;
	}

	void SyntacticAnalyzer::get_nullables() {
		bool changed = true;
		while (changed) {
			changed = false;
			// ! 不考虑空产生式，那么nullable都是false
		}
	}

	void SyntacticAnalyzer::get_firsts() {
		qy::unweighted_graph g0(nonterminals.size());

		// 1. Init graph and firsts

		for (auto& nt : nonterminals)
			for (auto& p : nt.productions)
				for (sid_t s : p.rhs) {
					if (s >= 0) {
						nt.first.set(s);
					} else {
						g0.add_edge(p.lhs, -s);
						if (nonterminals[-s].nullable)
							continue;
					}
					break;
				}

		// 2. Get SCC and order.
		// TODO Can first computation be optimized?

		auto [g, scc] = g0.induce_scc();
		auto seq = g.topological_sort();

		std::vector<symbol_set> firsts(g.size());
		for (auto [i, s] : enumerate(scc))
			firsts[s] |= nonterminals[i].first;

		// 3. 在新图上合并first

		for (sid_t u : std::views::reverse(seq))
			for (sid_t v : g.iter_nexts(u))
				firsts[u] |= firsts[v];

		// 4. 再转回来

		for (auto [i, s] : enumerate(scc))
			nonterminals[i].first = firsts[s];
	}

	SyntacticAnalyzer::state_graph SyntacticAnalyzer::get_LR1_states() const {
		// Generate closures
		// The first closure
		auto initial = initial_closure();
		std::vector<item_set> states{initial};
		// std::map<item_set, size_t> states_map{{initial, 0}};
		std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> atn; // A state graph for test
		for (size_t i = 0; i < states.size(); i++) {
			// Connections between closures
			std::unordered_map<sid_t, item_set> nexts;
			states[i] = closure(states[i]);
			// 需要group by next symbol
			for (auto& p : states[i].items)
				if (p.has_next())
					nexts[p.next()].items.emplace_back(p.next_item());

			for (auto& [k, v] : nexts) {
				v.kernel_size = v.items.size();
				sid_t to;
				if (auto idx = qy::ranges::index_of(states, v); idx == -1) {
					to = static_cast<sid_t>(states.size());
					states.emplace_back(v);
				} else {
					to = static_cast<sid_t>(idx);
				}
				atn.emplace(static_cast<sid_t>(i), std::pair{to, k});
			}
		}
		return {std::move(states), std::move(atn)};
	}

	SyntacticAnalyzer::item_set SyntacticAnalyzer::initial_closure() const {
		return {1, std::vector<item>{{&rules[0], 0, single_set(END_MARKER)}}};
	}

	SyntacticAnalyzer::item_set SyntacticAnalyzer::closure(const item_set& is) const {
		item_set result{is};
		std::queue<size_t> open;
		std::map<item::key_type, size_t> close; // 用 unordered 会无法编译……
		// ERROR! 这个算法是有后效性的
		for (size_t i = 0; i < result.items.size(); i++) {
			close.emplace(result.items[i].key(), i);
			open.emplace(i);
		}

		// TODO 应该也可以做个图出来？

		while (!open.empty()) {
			size_t i = open.front();
			open.pop();
			const auto& it = result.items[i];
			if (!it.has_next())
				continue;
			if (sid_t s = it.next(); s < 0) {
				// 这里其实所有左部相同的都会参与
				for (auto& p : nonterminals[-s].productions) {
					const auto& it = result.items[i]; // Caution realloc trap!!
					// 下面的计算可能有问题
					item new_item{&p, 0,
								  !it.has_next1()  ? it.follow
								  : it.next1() < 0 ? nonterminals[-it.next1()].first
												   : single_set(it.next1())};
					std::pair index{new_item.key()};
					if (auto _i = close.find(index); _i != close.end()) {
						auto& it2 = result.items.at(_i->second);
						auto new_follow = it2.follow | new_item.follow;
						if (it2.follow != new_follow) {
							it2.follow = new_follow;
							open.emplace(_i->second);
						}
					} else {
						close.emplace(index, result.items.size());
						open.emplace(result.items.size());
						result.items.emplace_back(new_item);
					}
				}
			}
		}
		return result;
	}

	parsing_table SyntacticAnalyzer::get_LR1_table(const state_graph& LR1_states) const {
		const auto& [states, atn] = LR1_states;

		size_t n_states = states.size(), n_tokens = tokens.size(),
			   n_nonterminals = nonterminals.size();

		parsing_table LR1_table{{n_states, n_tokens, ERR}, {n_states, n_nonterminals, ERR}};
		auto& [LR1_action, LR1_goto] = LR1_table;

		for (size_t i = 0; i < n_states; i++) {
			auto r = atn.equal_range(i);
			for (auto&& pair : std::ranges::subrange(r.first, r.second)) {
				auto&& e = pair.second;
				// nonterminal
				if (e.second < 0)
					LR1_goto[i][-e.second] = e.first;
				else
					LR1_action[i][e.second] = e.first;
			}
			for (auto& it : states[i].items) {
				if (!it.has_next()) {
					for (size_t j = 0; j < n_tokens; j++) {
						if (it.follow.test(j))
							LR1_action[i][j] = -it.prod->id;
					}
				}
			}
		}
		return LR1_table;
	}

	parsing_table SyntacticAnalyzer::get_LALR1_table(const state_graph& LR1_states,
													 const parsing_table& LR1_table) const {
		auto states = LR1_states.states;
		size_t n_states = states.size(), n_tokens = tokens.size(),
			   n_nonterminals = nonterminals.size();

		std::vector<std::vector<size_t>> kernel_grouped; // 同心状态分组
		for (auto&& [i, is] : enumerate(LR1_states.states)) {
			if (auto it = std::ranges::find_if(
					kernel_grouped, [&](auto& t) { return states[t.at(0)].kernel_equals(is); });
				it != kernel_grouped.end()) {
				it->emplace_back(i);
			} else {
				kernel_grouped.emplace_back(std::vector{i});
			}
		}
		// 状态数：1960。有417个同心状态。

		auto [LALR1_action, LALR1_goto] = LR1_table;

		std::vector<size_t> state_map(n_states); // 删除状态后，标号重映射
		std::iota(state_map.begin(), state_map.end(), 0);

		for (auto&& kern : kernel_grouped) {
			if (kern.size() < 2)
				continue;
			item_set new_set{states[kern[0]]};
			symbol_set ff;
			for (auto& item : new_set.items) {
				if (!item.has_next())
					ff &= item.follow;
			}
			for (size_t j = 1; j < kern.size(); j++) {
				for (auto&& [k, item] : enumerate(states[kern[j]].items)) {
					new_set.items[k].follow &= item.follow;
					if (!new_set.items[k].has_next())
						ff &= new_set.items[k].follow;
				}
			}
			// TODO fix ff
			states[kern[0]] = new_set;
			for (auto k : kern)
				state_map[k] = kern[0];
		}
		// 现在 state_map[i] != i 的都是要删除的。可以保证都是大号映射到小号
		for (ptrdiff_t i = n_states - 1; i >= 0; i--) {
			if (state_map[i] != i) {
				LALR1_action.remove_row(i);
				LALR1_goto.remove_row(i);
				states.erase(states.begin() + i);
			}
		}
		// 然后删掉中间空的
		for (size_t i = 0, cnt = 0; i < n_states; i++) {
			if (auto& x = state_map[i]; x == i) {
				x = cnt++;
			} else {
				x = state_map[x];
			}
		}
		// 对 action/goto 重映射
		for (size_t i = 0; i < n_states; i++) {
			for (size_t j = 0; j < n_tokens; j++)
				if (auto& x = LALR1_action[i][j]; x > 0)
					x = state_map[x];
			for (size_t j = 0; j < n_nonterminals; j++)
				if (auto& x = LALR1_goto[i][j]; x != ERR)
					x = state_map[x];
		}

		return {std::move(LALR1_action), std::move(LALR1_goto)};
	}

	parsing_table_compressed SyntacticAnalyzer::compress_table(const parsing_table& table) const {
		struct table_row {
			size_t r, c, l;
		};

		size_t n_col = table.action.cols();
		std::vector<table_row> rows;
		for (size_t i = 0; i < table.action.rows(); i++) {
			table_row row{i, 0, 0};
			size_t j = 0;
			while (j < n_col && table.action[i][j] == ERR)
				j++;
			if (j == n_col)
				continue;
			row.c = j;
			std::unordered_set<sid_t> cnt;
			for (; j < n_col; j++)
				if (table.action[i][j] != ERR) {
					row.l = j - row.c + 1;
					cnt.insert(table.action[i][j]);
				}
			if (cnt.size() > 1)
				rows.push_back(row);
		}
		std::sort(rows.begin(), rows.end(), [](const table_row& r1, const table_row& r2) {
			return r1.l > r2.l || r1.l == r2.l && r1.r < r2.r;
		}); // 排序稍微有点用

		// 下面考虑一种简单的压缩，不涉及行的交错

		std::vector<sid_t> tab;
		// std::priority_queue<std::pair<size_t, size_t>> gaps; // len, offset
		std::multimap<size_t, size_t> gaps;
		for (auto& row : rows) {
			size_t ins = tab.size();
			if (auto it = gaps.lower_bound(row.l); it != gaps.end()) { // 有空位放
				ptrdiff_t l = it->first - row.l;
				ins = it->second;
				gaps.erase(it);
				// 如果有多余空间就把多出的 gap 插回去
				if (l > 0)
					gaps.emplace(l, ins + row.l);
			}
			if (ins == tab.size()) {
				tab.insert(tab.end(), table.action.iter_row(row.r).begin() + row.c,
						   table.action.iter_row(row.r).begin() + row.c + row.l);
			} else {
				std::copy_n(table.action.iter_row(row.r).begin() + row.c, row.l, tab.begin() + ins);
			}
			for (size_t j = row.c; j < row.c + row.l;) {
				size_t k = j;
				while (k < n_col && table.action[row.r][k] != ERR)
					k++;
				if (k == n_col)
					break;
				// 现在k指向的是ERR
				j = k;
				while (j < n_col && table.action[row.r][j] == ERR)
					j++;
				// 现在j指向的是非ERR
				gaps.emplace(j - k, ins + k);
			}
		}

		return {};
	}

	parsing_table_compressed SyntacticAnalyzer::compress_table_more(
		const parsing_table& table) const {
		struct table_row {
			size_t r, c, l;
			const sid_t* a;
		};

		parsing_table_compressed pt;
		pt.pact.resize(table.action.rows());
		pt.defact.resize(table.action.rows());

		size_t n_col = table.action.cols();
		std::vector<table_row> rows;
		for (size_t i = 0; i < table.action.rows(); i++) {
			table_row row;
			row.r = i;
			size_t j = 0;
			while (j < n_col && table.action[i][j] == ERR)
				j++;
			if (j == n_col)
				continue;
			row.c = j;
			std::unordered_set<sid_t> cnt;
			for (; j < n_col; j++)
				if (table.action[i][j] != ERR) {
					row.l = j - row.c + 1;
					cnt.insert(table.action[i][j]);
				}
			row.a = &table.action[i][row.c];
			if (cnt.size() > 1)
				rows.push_back(row);
			pt.defact[i] = cnt.size() > 1 ? 0 : *cnt.begin();
		}

		// 下面考虑贪心嵌入

		auto& tab = pt.table;

		for (auto& row : rows) {
			for (size_t i = 0; i <= tab.size(); i++) { // 开头
				bool flag = true;
				for (size_t j = 0; j < row.l; j++) {
					if (i + j < tab.size() &&
						(tab[i + j] != ERR && row.a[j] != ERR && tab[i + j] != row.a[j])) {
						flag = false;
						break;
					}
				}
				if (flag) {
					// [i, i + l]
					if (i + row.l > tab.size()) {
						std::copy_n(row.a, tab.size() - i, tab.begin() + i);
						tab.insert(tab.end(), row.a + tab.size() - i, row.a + row.l);
						pt.check.resize(tab.size());
					} else {
						std::copy_n(row.a, row.l, tab.begin() + i);
					}
					for (size_t j = 0; j< row.l; j++) 
						if (row.a[j] != ERR)
							pt.check[i + j] = row.a[j];
						pt.pact[row.r] = i;
					break;
				}
			}
		}

		fmt::print("compressed: {}\n", tab.size());

		return pt;
	}
} // namespace comp

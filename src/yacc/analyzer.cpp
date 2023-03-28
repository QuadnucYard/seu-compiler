#include "yacc/analyzer.hpp"
#include "utils/graph.hpp"
#include "utils/myalgo.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <map>
#include <queue>
#include <ranges>
#include <tl/enumerate.hpp>

namespace comp {

	// bool SyntacticAnalyzer::item_set::contains(const item& i) const {
	// 	return std::ranges::contains(items, i);
	// }

	SyntacticAnalyzer::item_set SyntacticAnalyzer::item_set::get_goto(sid_t symbol) const {
		item_set result;
		for (auto& item : items) {
			if (item.has_next() && item.next() == symbol)
				result.items.push_back(item.next_item());
		}
		result.kernel_size = result.items.size();
		return result;
	}

	bool SyntacticAnalyzer::item_set::operator==(const item_set& o) const {
		return kernel_size == o.kernel_size &&
			   std::equal(items.begin(), items.begin() + kernel_size, o.items.begin(),
						  o.items.begin() + o.kernel_size);
	}

	void SyntacticAnalyzer::process() {
		get_nullables();
		get_firsts();

		// Print firsts
		for (auto& t : nonterminals) {
			fmt::print("First({})={}\n", t.name, to_string(t.first));
		}
		fmt::print("\n");

		// Generate closures
		auto initial = initial_closure();
		std::vector<item_set> states{initial};
		// std::map<item_set, size_t> states_map{{initial, 0}};
		std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> atn;
		for (size_t i = 0; i < states.size(); i++) {
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
				/* if (auto idx = states_map.find(v); idx == states_map.end()) {
					to = static_cast<sid_t>(states.size());
					states_map.emplace(v, states.size());
					states.emplace_back(v);
				} else {
					to = static_cast<sid_t>(idx->second);
				} */
				atn.emplace(static_cast<sid_t>(i), std::pair{to, k});
			}
		}

		for (size_t i = 0; auto& s : states) {
			fmt::print("I{}:\n{}\n", i++, to_string(s));
		}
		for (auto& [u, e] : atn) {
			fmt::print("({}, {}, {})\n", u, e.first, get_symbol_name(e.second));
		}
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

		std::vector<symbol_set> firsts(g.size(), symbol_set(tokens.size()));
		for (auto [i, s] : tl::views::enumerate(scc))
			firsts[s] |= nonterminals[i].first;

		// 3. 在新图上合并first

		for (sid_t u : std::views::reverse(seq))
			for (sid_t v : g.iter_edges(u))
				firsts[u] |= firsts[v];

		// 4. 再转回来

		for (auto [i, s] : tl::views::enumerate(scc))
			nonterminals[i].first = firsts[s];
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

} // namespace comp

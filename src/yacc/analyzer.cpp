#include "yacc/analyzer.hpp"
#include "utils/graph.hpp"
#include "utils/myalgo.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <iterator>
#include <map>
#include <queue>
#include <ranges>
#include <set>
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

	void SyntacticAnalyzer::process() {
		get_nullables();
		get_firsts();

		// Print firsts
		for (auto& t : nonterminals) {
			fmt::print("First({})={}\n", t.name, to_string(t.first));
		}
		fmt::print("\n");

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

		std::vector<std::vector<size_t>> kernel;
		for (size_t i = 0; auto& is : states) {
			if (auto it = std::ranges::find_if(
					kernel, [&](auto& t) { return states[t.at(0)].kernel_equals(is); });
				it != kernel.end()) {
				it->emplace_back(i);
			} else {
				kernel.emplace_back(std::vector{i});
			}
			i++;
		}
		// 状态数：1960。有417个同心状态。

		// 状态转换图存在上面的atn里了，可以转邻接矩阵。
		// 下面是生成表了，分action和goto
		// 表其实就对应邻接矩阵

		/**
		 * TODO:
		 * 1. 构造LR(1)分析表（表的存储方式怎么方便怎么来）
		 * 2. 构造LALR(1)分析表
		 * 3. 输出action,goto邻接矩阵到文件
		 */

		/* 		std::vector<std::vector<size_t>> LR1(
					states.size(), std::vector<size_t>(nonterminals.size() + tokens.size(), -1));
				std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> atn_co = atn;
				for (sid_t i = 0; i < states.size(); i++) {
					auto pair_i = atn_co.find(i);
					while (pair_i != atn_co.end()) {
						auto nt = find(nonterminals.begin(), nonterminals.end(), pair_i->first);
						auto tk = find(tokens.begin(), tokens.end(), pair_i->second.second);
						if (nt != nonterminals.end())
							LR1[i][nt - nonterminals.begin()] == (size_t)pair_i->second.second;
						//
		   LR1[i][find(nonterminals.begin(),nonterminals.end(),pair_i->first)-nonterminals.begin()]=(size_t)pair_i->second.second;
						else if (tk != tokens.end())
							LR1[i][tk - tokens.begin() + nonterminals.size()] ==
								(size_t)pair_i->second.second;
						atn_co.erase(pair_i);
						pair_i = atn_co.find(i);
					}
				}
		 */
		std::vector<std::vector<sid_t>> LR1_action(states.size(),
												   std::vector<sid_t>(tokens.size(), -1));

		std::vector<std::vector<sid_t>> LR1_goto(states.size(),
												 std::vector<sid_t>(nonterminals.size(), -1));

		std::unordered_multimap<sid_t, std::pair<sid_t, sid_t>> atn_co = atn;
		for (sid_t i = 0; i < states.size(); i++) {
			auto pair_i = atn_co.find(i);
			while (pair_i != atn_co.end()) {
				// nonterminal
				if (pair_i->second.second < 0)
					LR1_goto[i][-pair_i->second.second] = pair_i->second.first;

				else
					LR1_action[i][pair_i->second.second] = pair_i->second.first;
				atn_co.erase(pair_i);
				pair_i = atn_co.find(i);
			}
			for (item it : states[i].items) {
				if (it.dot == it.size()) {
					for (sid_t j = 0; j < tokens.size(); j++) {
						if (it.follow.test(j))
							LR1_action[i][j] = -it.prod->lhs;
					}
				}
			}
		}

		// LALR1
		std::vector<std::vector<sid_t>> LALR1_action = LR1_action;
		std::vector<std::vector<sid_t>> LALR1_goto = LR1_goto;

		/* sid_t flag=0;
		for (sid_t i = 0; i<kernel.size();i++) {
			for (sid_t j=0;j<kernel[i].size();j++) {
				flag=0;
				for (SyntacticAnalyzer::item item : states[kernel[i][j]].items){
					if (!item.has_next()){
						kernel[i].erase(j + kernel[i].begin());
						flag=1;
					}
					if(flag){
						j--;
						break;
					}
				}
			}
			if (kernel[i].size() <= 1){
				kernel.erase(kernel.begin() + i);
				i--;
			}
		} */
		std::bitset<128> ff;
		std::set<sid_t>erase_set;
		std::map<sid_t,sid_t>hash;
		for (sid_t i = 0; i < kernel.size(); i++) {
			if (kernel[i].size() < 2) {
				kernel.erase(kernel.begin() + i);
				i--;
			} else {
				item_set new_set;
				new_set.kernel_size = states[kernel[i][0]].kernel_size;
				new_set.items = states[kernel[i][0]].items;
				for (sid_t k = 0; SyntacticAnalyzer::item item : new_set.items) {
						if(!item.has_next())
							ff=ff&item.follow;
						k++;
				}
				for (sid_t j = 1; j < kernel[i].size(); j++) {
					for (sid_t k = 0; SyntacticAnalyzer::item item : states[kernel[i][j]].items) {
						new_set.items[k].follow = item.follow | new_set.items[k].follow;
						if (!new_set.items[k].has_next()) 
							ff=ff&new_set.items[k].follow;
						k++;
					}
				}
				if(ff!=0){
					kernel.erase(kernel.begin()+i);
					i--;
				}
				else{
					states[kernel[i][0]]=new_set;
					for (sid_t j = 1; j < kernel[i].size(); j++){
						erase_set.insert(kernel[i][j]);
						hash.insert({kernel[i][j],kernel[i][0]});
					}
				}	
			}
		}
		sid_t count=0;
		sid_t len =states.size();
		for(sid_t i=0;i<len;i++){
			if(erase_set.contains(i)){
				states.erase(states.begin()+i-count);
				LALR1_action.erase(LALR1_action.begin()+i-count);
				LALR1_goto.erase(LALR1_goto.begin()+i-count);
				count++;
			}
		}

		std::vector<sid_t>effect;
		auto it=erase_set.begin();
		sid_t po=*it;
		sid_t offset=0;
		for(sid_t i=0;i<states.size();i++){
			while(i+offset>=po){
				it++;
				po=*it;
				offset++;
			}
			hash.insert({i+offset,i});
		}
		


		for (size_t i = 0; i < LALR1_action.size(); i++) {
				for (size_t j = 0; j < LALR1_action[0].size(); j++) {
					if (LALR1_action[i][j] == -1)
						continue;
					else {
						LALR1_action[i][j]=hash[LALR1_action[i][j]];
					}
				}
		}


		for (size_t i = 0; i < LALR1_goto.size(); i++) {
				for (size_t j = 0; j < LALR1_goto[0].size(); j++) {
					if (LALR1_goto[i][j] <0)
						continue;
					else {
						LALR1_goto[i][j]=hash[LALR1_goto[i][j]];
					}
				}
		}

		/* for (std::vector<size_t> con : kernel) {
			size_t col = con[0];
			for (size_t i = 1; i < con.size(); i++) {
				for (size_t j = 0; j < LALR1_action[col].size(); j++) {
					if (LALR1_action[i][j] != -1)
						LALR1_action[i][j] = LALR1_action[col][j];
				}
				for (size_t j = 0; j < LALR1_goto[col].size(); j++) {
					if (LALR1_goto[i][j] != -1)
						LALR1_goto[i][j] = LALR1_goto[col][j];
				}
			}
			for (size_t i = 0; i < LALR1_action.size(); i++) {
				for (size_t j = 0; j < LALR1_action[i].size(); j++) {
					if (find(con.begin(), con.end(), LALR1_action[i][j]) != con.end())
						LALR1_action[i][j] = col;
				}
			}
			for (size_t i = 0; i < LALR1_goto.size(); i++) {
				for (size_t j = 0; j < LALR1_goto[i].size(); j++) {
					if (find(con.begin(), con.end(), LALR1_goto[i][j]) != con.end())
						LALR1_goto[i][j] = col;
				}
			}
		} */
		
		




		// os
		std::ofstream outFile("G:\\vscode\\seu-compiler\\LALR1_action.bin",
							  std::ios::binary);

		sid_t row = LALR1_action.size();
		sid_t col = LALR1_action[0].size();
		outFile.write((char*)&row, sizeof(sid_t));
		outFile.write((char*)&col, sizeof(sid_t));

		for (sid_t i = 0; i < row; i++) {
			for (sid_t j = 0; j < col; j++) {
				outFile.write((char*)&LALR1_action[i][j], sizeof(sid_t));
			}
		}
		outFile.close();
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

		std::vector<symbol_set> firsts(g.size());
		for (auto [i, s] : tl::views::enumerate(scc))
			firsts[s] |= nonterminals[i].first;

		// 3. 在新图上合并first

		for (sid_t u : std::views::reverse(seq))
			for (sid_t v : g.iter_nexts(u))
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

	std::vector<size_t> SyntacticAnalyzer::get_index(std::vector<std::vector<size_t>> kerner,
													 size_t target) {
		for (std::vector<size_t> it1 : kerner) {
			for (size_t it2 : it1) {
				if (it2 == target)
					return it1;
			}
		}
		return std::vector<size_t>{};
	}

} // namespace comp

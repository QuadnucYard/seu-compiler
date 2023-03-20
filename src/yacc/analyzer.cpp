#include "yacc/analyzer.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <map>

namespace comp {

	bool SyntacticAnalyzer::item_set::contains(const item& i) const {
		return std::ranges::contains(items, i);
	}

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
		get_follows();
		// Generate closures
		auto initial = initial_closure();
		std::vector<item_set> states{initial};
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
				if (!std::ranges::contains(states, v)) {
					atn.emplace(static_cast<sid_t>(i),
								std::pair{static_cast<sid_t>(states.size()), k});
					states.emplace_back(v);
				}
			}
		}

		for (size_t i = 0; auto& s : states) {
			fmt::print("I{}:\n{}\n", i++, to_string(s));
		}
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
			s.push_back(static_cast<sid_t>(i) == it.dot ? '.' : ' ');
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
		symbol_set set(tokens.size());
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
		// ! Suppose no null-production
		for (auto& sym : nonterminals)
			for (auto& p : sym.productions)
				if (p.rhs.size() > 1 && p.rhs[0] >= 0)
					sym.first.set(p.rhs[0]);
	}

	void SyntacticAnalyzer::get_follows() {}

	SyntacticAnalyzer::item_set SyntacticAnalyzer::initial_closure() const {
		return {1, std::vector<item>{{&rules[0], 0, single_set(END_MARKER)}}};
	}

	SyntacticAnalyzer::item_set SyntacticAnalyzer::closure(const item_set& is) const {
		item_set result{is};
		std::map<item::key_type, size_t> close; // 用 unordered 会无法编译……

		for (size_t i = 0; i < result.items.size(); i++)
			close.emplace(result.items[i].key(), i);

		for (size_t i = 0; i < result.items.size(); i++) {
			const auto& it = result.items[i];
			assert(it.follow.capacity() >= tokens.size());
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
						result.items.at(_i->second).follow |= new_item.follow;
					} else {
						close.emplace(index, result.items.size());
						result.items.emplace_back(new_item);
					}
				}
			}
		}
		return result;
	}

} // namespace comp

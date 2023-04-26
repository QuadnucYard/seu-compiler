#include "lex/fa.hpp"
#include "lex/tompson.hpp"
#include "utils/myalgo.hpp"
#include <bitset>
#include <queue>

namespace comp {
	/// @brief Get a readable structure of graph
	/// @param g
	/// @return vector of edges (u, v, w)
	std::vector<std::tuple<sid_t, sid_t, sid_t>> easy(const qy::weighted_graph& g) {
		std::vector<std::tuple<sid_t, sid_t, sid_t>> res;
		for (auto&& [u, v, w] : g.edges()) {
			res.emplace_back(u, v, w);
		}
		return res;
	}

	void DFABuilder::add_re(const string& re, int index) {
		all_dfa.push_back(nfa2dfa(re2nfa(re, index)));
		auto _g = easy(all_dfa.back().graph);
	}

	NFA DFABuilder::re2nfa(const string& re, int index) const {
		NFA nfa = regexToNFA(re);
		auto _g = easy(nfa.graph);
		return nfa;
	}

	DFA DFABuilder::nfa2dfa(const NFA& nfa) const {
		// 0. 初始NFA：1个accept
		// 1. reverse：NFA->NFA, 1 accept
		// 2. subset: NFA->DFA, n accepts
		// 3. reachable: DFA->DFA, n accepts
		// 4. reverse：DFA->NFA, 1 accept
		// 5. subset: NFA->DFA, n accepts
		// 6. reachable: DFA->DFA, n accepts
		// 倒腾的NFA都只有1个accept, join前的accept states无意义，都是统一的
		return reachable(subset(reverse(reachable(subset(reverse(nfa))))));
	}

	NFA DFABuilder::reverse(const NFA& fa) const {
		NFA nfa;
		sid_t n = static_cast<sid_t>(fa.graph.size());
		nfa.start = n;
		nfa.accept = fa.start;
		nfa.graph = fa.graph.reversed();
		nfa.graph.resize(n + 1);
		nfa.graph.add_edge(nfa.start, fa.accept, EPSILON);
		auto _g = easy(nfa.graph);
		return nfa;
	}

	NFA DFABuilder::reverse(const DFA& fa) const {
		NFA nfa;
		sid_t n = static_cast<sid_t>(fa.graph.size());
		nfa.start = n;
		nfa.accept = fa.start;
		nfa.graph = fa.graph.reversed();
		nfa.graph.resize(n + 1);
		for (size_t i = 0; i < n; i++) {
			if (fa.accept_states[i] != NON_ACCEPT)
				nfa.graph.add_edge(n, static_cast<sid_t>(i), EPSILON);
		}
		auto _g = easy(nfa.graph);
		return nfa;
	}

	DFA DFABuilder::subset(const NFA& nfa) const {
		constexpr size_t MAXV = 128; // 最大符号数
		using vertex_set = std::bitset<MAXV>;

		int n = nfa.graph.size();

		// 求 epsilon closure
		std::vector<vertex_set> epsilon_closure(n);
		for (int i = 0; i < n; i++) {
			std::queue<int> q{{i}};
			std::vector<int8_t> vis(n);
			vis[i] = true;
			while (!q.empty()) {
				int u = q.front();
				q.pop();
				epsilon_closure[i].set(u);
				for (auto&& [v, w] : nfa.graph.iter_edges(u)) {
					if (w == EPSILON && !vis[v]) {
						q.push(v);
						vis[v] = true;
					}
				}
			}
		}

		// 求 follow epsilon
		std::vector<std::unordered_map<int, vertex_set>> follow_epsilon(n);
		for (int u = 0; u < n; u++) {
			for (auto&& [v, w] : nfa.graph.iter_edges(u)) {
				if (w != EPSILON)
					follow_epsilon[u][w] |= epsilon_closure[v];
			}
		}

		// 开始子集构造
		std::vector<vertex_set> states{epsilon_closure[nfa.start]};
		DFA dfa;
		dfa.graph.resize(1);
		for (size_t i = 0; i < states.size(); i++) {
			// 下面要找当前状态接收每个符号到什么状态
			// 先枚举接收的符号
			for (size_t j = 0; j < MAXV; j++) {
				vertex_set next;
				// 然后找当前状态中的对这个符号的follow
				for (sid_t k = 0; k < n; k++) {
					if (!states[i].test(k))
						continue;
					if (follow_epsilon[k].contains(j))
						next |= follow_epsilon[k][j];
				}
				if (next.none())
					continue;
				auto idx = qy::ranges::index_of(states, next);
				if (idx == -1) {
					idx = states.size();
					states.push_back(next);
					dfa.graph.resize(states.size());
				}
				dfa.graph.add_edge(static_cast<int>(i), static_cast<int>(idx), static_cast<int>(j));
			}
			dfa.accept_states.push_back(
				states[i].test(nfa.accept) ? 1 : NON_ACCEPT); // 用1代替虚假的accept状态？
		}
		dfa.start = 0;
		/**
		 * 考虑一下acc
		 * 如果NFA的acc是唯一的，指明是哪个
		 * 那么dfa的acc就是状态是否包含
		 */
		auto _g = easy(dfa.graph);
		return dfa;
	}

	DFA DFABuilder::reachable(const DFA& dfa) const {
		DFA res;
		std::queue<sid_t> q{{dfa.start}};
		std::vector<sid_t> mapped(dfa.graph.size(), -1);
		sid_t cnt = 0;
		mapped[dfa.start] = cnt++;
		while (!q.empty()) {
			sid_t u = q.front();
			q.pop();
			for (auto&& [v, w] : dfa.graph.iter_edges(u)) {
				if (mapped[v] == -1) {
					mapped[v] = cnt++;
					q.push(v);
				}
			}
		}
		// 这里其实有个问题：没考虑是否能到acc
		res.start = dfa.start;
		res.accept_states.resize(cnt);
		for (size_t i = 0; i < dfa.accept_states.size(); i++) {
			if (sid_t u = mapped[i]; u != -1)
				res.accept_states[u] = dfa.accept_states[i];
		}
		res.graph.resize(cnt);
		for (auto&& [u, v, w] : dfa.graph.edges()) {
			if (mapped[u] != -1 && mapped[v] != -1)
				res.graph.add_edge(mapped[u], mapped[v], w);
		}
		auto _g = easy(res.graph);
		return res;
	}

	DFA DFABuilder::join_nfa() const {
		// 图要join在一起，states也一样
		DFA dfa;
		dfa.accept_states.push_back(NON_ACCEPT);
		dfa.graph.resize(1);
		dfa.accept_states.resize(1);
		for (auto&& x : all_dfa) {
			dfa.accept_states.insert(dfa.accept_states.end(), x.accept_states.begin(),
									 x.accept_states.end());
			int s = static_cast<int>(dfa.graph.size()) + x.start; // start在新图中的位置
			dfa.graph.join(x.graph);
			dfa.graph.add_edge(0, s, EPSILON);
		}
		return dfa;
	}
} // namespace comp

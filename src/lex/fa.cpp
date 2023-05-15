#include "lex/fa.hpp"
#include "lex/tompson.hpp"
#include "utils/myalgo.hpp"
#include <bitset>
#include <queue>
#include <set>
#include <tl/enumerate.hpp>
#include <unordered_set>

namespace comp {

#ifdef GRAPH_FMT
	void DFA::to_dot(const fs::path& path) const {
		auto out = fmt::output_file(path.string());
		out.print("digraph G {{\n");
		out.print("    rankdir = LR;\n");
		out.print("    node [shape = none];\n");
		out.print("    \"\";\n");
		out.print("    node [shape = doublecircle];\n");
		for (auto&& [i, a] : tl::views::enumerate(accept_states))
			if (a != DFABuilder::NON_ACCEPT)
				out.print("    {};\n", i);
		out.print("    node [shape = circle];\n");
		out.print("    \"\"  -> {};\n", start);
		for (auto&& [u, v, w] : graph.edges())
			out.print("    {} -> {} [label=\"{}\"];\n", u, v,
					  isprint(w) ? (w == '"' ? fmt::format("'\\\"'") : fmt::format("'{}'", char(w)))
								 : fmt::format("{}", w));
		out.print("}}\n");
	}
#endif

	/// @brief Get a readable structure of graph
	/// @param g
	/// @return vector of edges (u, v, w)
	std::vector<std::tuple<sid_t, sid_t, sid_t>> easy(const qy::weighted_graph& g) {
		return g.to_edge_vector();
	}

	void DFABuilder::add_re(const string& re) {
		all_dfa.push_back(nfa2dfa(re2nfa(re)));
		auto _g = easy(all_dfa.back().graph);
	}

	NFA DFABuilder::re2nfa(const string& re) const {
		NFA nfa = tompson(re);
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
		// 这个 Brzozowski's Algorithm 有问题，不能最小化，不知道是不是实现的锅
		// return reachable(subset(reverse(reachable(subset(reverse(nfa))))));
		return hopcroft(subset(nfa));
	}

	DFA DFABuilder::hopcroft(const DFA& dfa) const {
		using vertex_set = std::bitset<MAXV>;

		std::unordered_set<vertex_set> partition = [&dfa, this] {
			vertex_set s1, s2;
			for (size_t i = 0; i < dfa.size(); i++) {
				(dfa.accept_states[i] == NON_ACCEPT ? s1 : s2).set(i);
			}
			return std::unordered_set{s1, s2};
		}();
		std::unordered_set<vertex_set> worklist = partition;

		while (!worklist.empty()) {
			auto S = *worklist.begin();
			worklist.erase(worklist.begin());
			for (sid_t c = 0; c < MAXS; c++) {
				vertex_set image;
				for (auto&& [u, v, w] : dfa.graph.edges()) {
					if (w == c && S.test(v))
						image.set(u);
				}
				if (image.none())
					continue;
				for (auto& q : decltype(partition){partition}) {
					auto q1 = q & image;
					if (q1.none())
						continue;
					auto q2 = q ^ q1;
					if (q2.none())
						continue;
					partition.erase(q);
					partition.insert(q1);
					partition.insert(q2);
					if (worklist.contains(q)) {
						worklist.erase(q);
						worklist.insert(q1);
						worklist.insert(q2);
					} else if (q1.count() < q2.count()) {
						worklist.insert(q1);
					} else {
						worklist.insert(q2);
					}
					if (S == q)
						break;
				}
			}
		}

		std::vector<vid_t> mapped(dfa.size()); // 原DFA的点映射到哪个点
		for (auto&& [i, s] : tl::views::enumerate(partition)) {
			for (size_t j = 0; j < s.size(); j++) {
				if (s.test(j))
					mapped[j] = static_cast<vid_t>(i);
			}
		}
		std::set<std::tuple<vid_t, vid_t, sid_t>> atn;
		for (auto&& [u, v, w] : dfa.graph.edges())
			atn.emplace(mapped[u], mapped[v], w);
		DFA res(partition.size());
		res.start = mapped[dfa.start];
		for (auto&& [u, v, w] : atn)
			res.graph.add_edge(u, v, w);
		for (size_t i = 0; i < dfa.size(); i++)
			res.accept_states[mapped[i]] = dfa.accept_states[i];
		return res;
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
		using vertex_set = std::bitset<MAXV>;

		vid_t n = static_cast<int>(nfa.graph.size());

		// 求 epsilon closure
		std::vector<vertex_set> epsilon_closure(n);
		for (vid_t i = 0; i < n; i++) {
			std::queue<vid_t> q{{i}};
			std::vector<int8_t> vis(n);
			vis[i] = true;
			while (!q.empty()) {
				vid_t u = q.front();
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
		for (vid_t u = 0; u < n; u++) {
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
			for (sid_t j = 0; j < MAXS; j++) {
				vertex_set next;
				// 然后找当前状态中的对这个符号的follow
				for (vid_t k = 0; k < n; k++) {
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
				dfa.graph.add_edge(static_cast<vid_t>(i), static_cast<vid_t>(idx), j);
			}
			// 用nfa.accept==-1表示存在accepts
			// 如果有多个acc，这个结点应该accept到哪？
			// 是一样的 acc
			vid_t acc = NON_ACCEPT;
			if (nfa.accept == -1) {
				// accept_states is enabled
				for (auto a : nfa.accept_states)
					if (a != NON_ACCEPT) {
						acc = a;
						break;
					}
			} else {
				acc = states[i].test(nfa.accept) ? DUMMY_ACCEPT : NON_ACCEPT;
			}
			dfa.accept_states.push_back(acc);
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
		NFA nfa;
		nfa.start = 0;
		nfa.graph.resize(1);
		nfa.accept = -1;
		nfa.accept_states.push_back(NON_ACCEPT);
		for (auto&& [i, x] : tl::views::enumerate(all_dfa)) {
			nfa.accept_states.insert(nfa.accept_states.end(), x.accept_states.begin(),
									 x.accept_states.end());
			std::replace(nfa.accept_states.begin() + nfa.size(), nfa.accept_states.end(),
						 DUMMY_ACCEPT, static_cast<sid_t>(DUMMY_ACCEPT + i));
			int s = static_cast<int>(nfa.size()) + x.start; // start在新图中的位置
			nfa.graph.join(x.graph);
			nfa.graph.add_edge(0, s, EPSILON);
			// nfa.to_dot("lex_nfa.dot");
		}
		return subset(nfa);
	}
} // namespace comp
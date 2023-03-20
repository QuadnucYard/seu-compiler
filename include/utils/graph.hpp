#pragma once
#include <functional>
#include <ranges>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace qy {

	template <typename Index, typename Edge, typename EdgeDirector>
		requires std::is_integral_v<Index> &&
				 std::is_convertible_v<std::invoke_result_t<EdgeDirector, Edge>, Index> &&
				 std::is_convertible_v<std::invoke_result_t<EdgeDirector, Edge, Index>, Edge>
	class basic_graph {
	private:
		using id_t = Index;
		using id_vec = std::vector<id_t>;
		using id_stack = std::stack<id_t>;
		using edge_type = Edge;
		using storage_type = std::vector<std::vector<edge_type>>;

		static constexpr EdgeDirector edge_dir{};

	public:
		basic_graph(size_t n) : g(n) {}

		size_t size() const { return g.size(); }

		void add_edge(id_t u, auto&&... args) {
			g[u].emplace_back(std::forward<decltype(args)>(args)...);
		}

		auto iter_edges(id_t u) const { return std::ranges::subrange(g[u].begin(), g[u].end()); }

		std::pair<basic_graph, id_vec> induce_scc() const {
			struct garbow_closure {
				const storage_type& g;
				id_vec low, scc_no;
				std::stack<id_t> stack1, stack2;
				id_t dfs_clock{0}, scc_cnt{0};

				garbow_closure(const storage_type& g) : g(g), low(g.size()), scc_no(g.size()) {
					for (size_t i = 0; i < g.size(); i++)
						if (!low[i])
							garbow(static_cast<id_t>(i));
				}

				void garbow(id_t u) {
					stack1.push(u);
					stack2.push(u);
					low[u] = ++dfs_clock;
					for (auto& e : g[u]) {
						id_t v = edge_dir(e);
						if (!low[v])
							garbow(v);
						else if (!scc_no[v])
							while (low[stack2.top()] > low[v])
								stack2.pop();
					}
					if (stack2.top() == u) {
						stack2.pop();
						id_t v;
						do {
							v = stack1.top();
							stack1.pop();
							scc_no[v] = scc_cnt;
						} while (v != u);
						scc_cnt++;
					}
				}
			};

			garbow_closure c(g);
			basic_graph g2(c.scc_cnt);
			for (size_t i = 0; i < g.size(); i++) {
				id_t u = c.scc_no[i];
				for (auto& e : g[i]) {
					id_t v = edge_dir(e);
					if (u != c.scc_no[v])
						g2.add_edge(u, edge_dir(e, c.scc_no[v]));
				}
			}

			return {std::move(g2), std::move(c.scc_no)};
		}

		id_vec topological_sort() const {
			id_t n = static_cast<id_t>(g.size());
			id_vec deg(n);
			id_vec queue;
			queue.reserve(n);
			for (id_t i = 0; i < n; i++)
				for (auto& e : g[i]) {
					id_t v = edge_dir(e);
					deg[v]++;
				}
			for (id_t i = 0; i < n; i++)
				if (!deg[i])
					queue.push_back(i);
			for (id_t i = 0; i < n; i++) {
				id_t u = queue[i];
				for (auto& e : g[u]) {
					id_t v = edge_dir(e);
					if (--deg[v] == 0)
						queue.push_back(v);
				}
			}
			return queue;
		}

	private:
		storage_type g;
	};

	struct __simple_dir {
		auto operator()(auto&& e) const { return e; }

		auto operator()(auto&& e, auto&& v) const { return v; }
	};

	using unweighted_graph = basic_graph<int, int, __simple_dir>;

	struct __pair_first {
		auto operator()(auto&& e) const { return e.first; }

		auto operator()(auto&& e, auto&& v) const { return std::pair{v, e.second}; }
	};

	using weighted_graph = basic_graph<int, std::pair<int, int>, __pair_first>;

} // namespace qy

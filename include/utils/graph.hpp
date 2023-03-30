#pragma once
#include <functional>
#include <ranges>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace qy {

	template <typename Index, typename Edge>
		requires std::is_integral_v<Index>
	class basic_graph {
	private:
		using id_t = Index;
		using id_vec = std::vector<id_t>;
		using id_stack = std::stack<id_t>;
		using edge_type = Edge;
		using storage_type = std::vector<std::vector<edge_type>>;

		struct edge_view_sentinel {};

		struct edge_view_iterator {
			using iterator_concept = std::input_iterator_tag;
			using iterator_category = std::input_iterator_tag;
			using value_type = decltype(std::tuple_cat(std::make_tuple(id_t{}), edge_type{}));

			const basic_graph* g;
			id_t u;
			size_t i;

			edge_view_iterator(const basic_graph* g) : g(g), u(0), i(0) {}

			value_type operator*() const { return std::tuple_cat(std::make_tuple(u), g->g[u][i]); }

			edge_view_iterator& operator++() {
				if (++i >= g->g[u].size()) {
					++u, i = 0;
					while (u < g->size() && g->g[u].empty())
						++u;
				}
				return *this;
			}

			bool operator!=(edge_view_sentinel) const { return g != nullptr && u < g->size(); }

			edge_view_iterator begin() const { return *this; }

			edge_view_sentinel end() const { return {}; }
		};

		struct edge_director {
			/// @brief Get the dest vertex of the edge.
			/// @param e The edge.
			/// @return The dest vertex.
			id_t operator()(const edge_type& e) const { return std::get<0>(e); }

			/// @brief Modify the dest vertex of the edge, and return the new edge.
			/// @param e The edge.
			/// @param v The new dest vertex.
			/// @return The new edge.
			edge_type operator()(edge_type e, id_t v) const {
				std::get<0>(e) = v;
				return e;
			}
		};

		static constexpr edge_director edge_dir{};

	public:
		basic_graph() {}
		basic_graph(size_t n) : g(n) {}

		size_t size() const { return g.size(); }

		void reserve(size_t n) { g.reserve(n); }

		void resize(size_t n) { g.resize(n); }

		basic_graph& add_edge(id_t u, auto&&... args) {
			g[u].emplace_back(std::forward<decltype(args)>(args)...);
			return *this;
		}

		auto edges() const { return edge_view_iterator{this}; }

		auto iter_edges(id_t u) const { return std::ranges::subrange(g[u].begin(), g[u].end()); }

		auto iter_nexts(id_t u) const {
			return g[u] | std::views::transform([](auto&& e) { return edge_dir(e); });
		}

		basic_graph reversed() const {
			basic_graph result(size());
			for (size_t u = 0; u < g.size(); u++)
				for (auto& e : g[u])
				result.add_edge(edge_dir(e), edge_dir(e, static_cast<id_t>(u)));
			return result;
		}

		basic_graph& join(const basic_graph& other) {
			size_t n = size();
			resize(size() + other.size());
			for (size_t i = 0; i < other.size(); i++) {
				for (auto& e : other.g[i])
				add_edge(static_cast<id_t>(n + i), edge_dir(e, static_cast<id_t>(edge_dir(e) + n)));
			}
			return *this;
		}

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
				for (id_t v : iter_nexts(i))
					deg[v]++;
			for (id_t i = 0; i < n; i++)
				if (!deg[i])
					queue.push_back(i);
			for (id_t i = 0; i < n; i++) {
				id_t u = queue[i];
				for (id_t v : iter_nexts(u)) {
					if (--deg[v] == 0)
						queue.push_back(v);
				}
			}
			return queue;
		}

	private:
		storage_type g;
	};

	using unweighted_graph = basic_graph<int, std::tuple<int>>;

	using weighted_graph = basic_graph<int, std::tuple<int, int>>;

} // namespace qy

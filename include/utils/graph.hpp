#pragma once
#include <functional>
#include <ranges>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace qy {

	struct __empty_struct {};

	template <typename Index, typename VertexWeight, typename EdgeWeight>
		requires std::is_integral_v<Index>
	class basic_graph {
	private:
		using size_type = std::size_t;
		using id_t = Index;

		using void_type = __empty_struct;

		using vertex_weight_type = VertexWeight;
		constexpr static bool has_vertex_weight = !std::is_void_v<vertex_weight_type>;
		using safe_vertex_weight_type =
			std::conditional_t<has_vertex_weight, vertex_weight_type, void_type>;
		using edge_weight_type = EdgeWeight;
		using edge_type = std::conditional_t<std::is_void_v<edge_weight_type>, std::tuple<id_t>,
											 std::tuple<id_t, edge_weight_type>>;
		using storage_type = std::vector<std::vector<edge_type>>;

		using id_vec = std::vector<id_t>;
		using vertices_type =
			std::conditional_t<has_vertex_weight, std::vector<vertex_weight_type>, bool>;
		vertices_type vertices;

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

		basic_graph(size_type n) : g(n) {
			g.resize(n);
			if constexpr (has_vertex_weight)
				vertices.resize(n);
		}

		size_type size() const { return g.size(); }

		void reserve(size_type n) {
			g.reserve(n);
			if constexpr (has_vertex_weight)
				vertices.reserve(n);
		}

		void resize(size_type n) {
			g.resize(n);
			if constexpr (has_vertex_weight)
				vertices.resize(n);
		}

		/// @brief Add an edge to the graph.
		/// @param u The start vertex. Should grantee valid index.
		/// @param ...args Arguments to construct the edge.
		/// @return This.
		basic_graph& add_edge(id_t u, auto&&... args) {
			g[u].emplace_back(std::forward<decltype(args)>(args)...);
			return *this;
		}

		std::conditional_t<has_vertex_weight, const safe_vertex_weight_type&, id_t> operator[](
			id_t u) const {
			if constexpr (has_vertex_weight)
				return vertices[u];
			else
				return u;
		}

		std::conditional_t<has_vertex_weight, safe_vertex_weight_type&, id_t> operator[](id_t u) {
			if constexpr (has_vertex_weight)
				return vertices[u];
			else
				return u;
		}

		/// @brief Iterate all edges in the graph.
		/// @return A range of all edges.
		auto edges() const { return edge_view_iterator{this}; }

		/// @brief Iterate edges from given vertex.
		/// @param u The vertex index.
		/// @return A range of edges.
		auto iter_edges(id_t u) const { return std::ranges::subrange(g[u].begin(), g[u].end()); }

		/// @brief Iterate the next vertices can be reached directed.
		/// @param u The start vertex index.
		/// @return A range of next vertices.
		auto iter_nexts(id_t u) const { return g[u] | std::views::transform(edge_dir); }

		/// @brief Get the reversed graph.
		/// @return A reversed graph.
		basic_graph reversed() const {
			basic_graph result(size());
			for (size_t u = 0; u < g.size(); u++)
				for (auto& e : g[u])
				result.add_edge(edge_dir(e), edge_dir(e, static_cast<id_t>(u)));
			return result;
		}

		/// @brief Join another graph and add all edges.
		/// @param other The other graph to be joined.
		/// @return This;
		basic_graph& join(const basic_graph& other) {
			size_t n = size();
			resize(size() + other.size());
			for (size_t i = 0; i < other.size(); i++) {
				for (auto& e : other.g[i])
				add_edge(static_cast<id_t>(n + i), edge_dir(e, static_cast<id_t>(edge_dir(e) + n)));
			}
			return *this;
		}

		/// @brief Get the strongly connect components of the graph.
		/// @return A pair of new graph of SCCs and vertex index mapping.
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

		/// @brief Get the topological order of the graph.
		/// @return A vector of vertex indices that represents the topological order.
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

	using unweighted_graph = basic_graph<int, void, void>;

	using weighted_graph = basic_graph<int, void, int>;

} // namespace qy

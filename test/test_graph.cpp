#include "utils/graph.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

void print_graph(const qy::unweighted_graph& g) {
	for (size_t i = 0; i < g.size(); i++)
		for (auto [v] : g.iter_edges(static_cast<int>(i)))
			fmt::print("{}->{}\n", i, v);
}

void print_graph(const qy::weighted_graph& g) {
	for (size_t i = 0; i < g.size(); i++)
		for (auto [v, w] : g.iter_edges(static_cast<int>(i)))
			fmt::print("{}->{}: {}\n", i, v, w);
}

int main(int argc, char const* argv[]) {
	qy::weighted_graph g0(8);
	// Add edges.
	g0.add_edge(0, std::pair{1, 2});
	g0.add_edge(1, 0, 2);
	g0.add_edge(1, 0, 0);
	g0.add_edge(1, 2, 3);
	g0.add_edge(2, 3, 1);
	g0.add_edge(4, 2, 2);
	g0.add_edge(4, 5, 0);
	g0.add_edge(6, 4, 4);
	g0.add_edge(2, 6, 0);
	g0.add_edge(7, 6, 0);
	// Print edges.
	print_graph(g0);

	for (auto [u, v, w] : g0.edges())
		fmt::print("[{}->{}: {}]\n", u, v, w);

	auto g2 = g0.reversed();
	print_graph(g2);

	qy::unweighted_graph g1(5);
	g1.add_edge(0, 3).add_edge(1, 2);

	{
	fmt::print("Test scc and topo\n");
	auto [g, s] = g0.induce_scc();
	fmt::print("scc: {}\n", s);
	auto ts = g.topological_sort();
	fmt::print("topo: {}\n", ts);
	}

	// Test join
	{
		fmt::print("Test join\n");
		qy::unweighted_graph g1{3}, g2{3};
		g1.add_edge(0, 1).add_edge(1, 2).add_edge(2, 0);
		g2.add_edge(0, 1).add_edge(0, 2);
		g2.join(g1);
		print_graph(g2);

		for (auto [u, v] : g2.edges())
			fmt::print("[{}->{}]\n", u, v);
	}
	return 0;
}
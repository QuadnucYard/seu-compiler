#include "utils/graph.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

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
	for (size_t i = 0; i < g0.size(); i++)
		for (auto [v, w] : g0.iter_edges(static_cast<int>(i)))
			fmt::print("{}->{}: {}\n", i, v, w);

	auto [g, s] = g0.induce_scc();
	fmt::print("scc: {}\n", s);
	auto ts = g.topological_sort();
	fmt::print("topo: {}\n", ts);

	qy::unweighted_graph g1(5);
	g1.add_edge(0, 3).add_edge(1, 2);
	return 0;
}
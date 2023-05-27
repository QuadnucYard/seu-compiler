#include "utils/graph.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <tl/repeat.hpp>

int main(int argc, char const* argv[]) {
	using namespace qy;

	// Graph with vertex weight.
	{
		qy::basic_graph<int, int, void> g(8); // A graph with vertex weight
		fmt::print("{}\n", g[3]);
		g[3] = 666;							  // Change the weight of g[3] to 666.
		fmt::print("{}\n", g[3]);
	}

	qy::weighted_graph g0(8);
	// Add edges.
	g0.add_edge(1, 0, 2); // Add en edge from 1 to 0, weighted 2.
	g0.add_edge(1, 0, 0);
	g0.add_edge(1, 2, 3);
	g0.add_edge(2, 3, 1);
	g0.add_edge(4, 2, 2);
	g0.add_edge(4, 5, 0);
	g0.add_edge(6, 4, 4);
	g0.add_edge(2, 6, 0);
	g0.add_edge(7, 6, 0);
	g0.add_edge(7, std::tuple{6, 0}); // Also OK.
	g0.add_edge(0, std::pair{1, 2});  // Also OK.
	g0.to_dot("test.dot");
	// fmt::print("{}\n", g0[3]);
	// Print edges.
	print_graph(g0);

	// Iterate all edges in the graph.
	for (auto [u, v, w] : g0.edges())
		fmt::print("[{}->{}: {}]\n", u, v, w);

	// Iterate edges from vertex 1.
	for (auto [v, w] : g0.iter_edges(1))
		fmt::print("  ->{}:{}\n", v, w);

	// Iterate edges from vertex 1, but no weight.
	for (auto v : g0.iter_nexts(1))
		fmt::print("  ->{}\n", v);

	// Reverse the graph and print it.
	auto g2 = g0.reversed();
	print_graph(g2);

	// Also support unweighted graphs.
	qy::unweighted_graph g1(5);
	// Chain-style adding edges.
	g1.add_edge(0, 3);
	g1.add_edge(1, 2);

	// Strongly connected components and topological sort
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
		g1.add_edge(0, 1);
		g1.add_edge(1, 2);
		g1.add_edge(2, 0);
		g2.add_edge(0, 1);
		g2.add_edge(0, 2);
		g2.join(g1);
		print_graph(g2);

		for (auto [u, v] : g2.edges())
			fmt::print("[{}->{}]\n", u, v);
	}
	return 0;
}
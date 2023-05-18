#pragma once
#include "utils/graph.hpp"
#include <string>
#include <vector>

namespace comp {

	using std::string;
	using vid_t = int;
	using sid_t = int;

	/// @brief Used for RE to NFA. It describes the start and end node in the NFA graph.
	struct SubNFA {
		vid_t start;
		vid_t end;
	};

	struct DFA {
		qy::weighted_graph graph;		// 存结点的图，可以默认start为0
		vid_t start;						// 初始状态
		std::vector<vid_t> accept_states; // 每个点的accept情况

		inline DFA() = default;

		inline DFA(size_t size) : graph(size), accept_states(size), start{} {}

		inline constexpr size_t size() const { return graph.size(); }

#ifdef GRAPH_FMT
		void to_dot(const fs::path& path) const;
#endif
	};

	struct NFA : public DFA {
		vid_t accept; // 唯一的accept情况
	};

	/// @brief Construct DFA from a regular expression.
	class DFABuilder {
	public:
		constexpr static sid_t EPSILON = -1;
		constexpr static vid_t NON_ACCEPT = -1;
		constexpr static vid_t DUMMY_ACCEPT = NON_ACCEPT + 1;
		constexpr static size_t MAXV = 512; // 最大顶点数
		constexpr static sid_t MAXS = 128;	// 字符集大小 TODO 用一个vector表示字符集

		/// @brief Convert input RE to stored NFA.
		/// @param re Regular expression.
		void add_re(const string& re);

		std::pair<NFA, DFA> join_nfa() const;

	private:
		/// @brief Convert RE to NFA.
		/// @param re Regular expression.
		NFA re2nfa(const string& re) const;

		/// @brief Convert stored NFA to DFA.
		DFA nfa2dfa(const NFA& nfa) const;

		DFA hopcroft(const DFA& dfa) const;

		/// @brief Minimize stored DFA. It won't be called if we use Brzozowski's Algorithm.
		// void minimize_dfa();

		NFA reverse(const DFA& dfa) const;
		NFA reverse(const NFA& nfa) const;

		/// @brief Used for constructing DFA.
		DFA subset(const NFA& nfa) const;

		DFA reachable(const DFA& dfa) const;

	private:
		std::vector<DFA> all_dfa;
		std::vector<sid_t> charset; // 字符集
	};

} // namespace comp

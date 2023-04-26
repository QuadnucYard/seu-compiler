#pragma once
#include "utils/graph.hpp"
#include <string>
#include <vector>

namespace comp {

	using std::string;
	using sid_t = int;

	/// @brief Used for RE to NFA. It describes the start and end node in the NFA graph.
	struct SubNFA {
		int start;
		int end;
	};

	struct NFA {
		qy::weighted_graph graph;
		int start;	// start结点序号
		int accept; // 唯一的accept情况
		std::vector<int> accept_states; // 每个点的accept情况
	};

	struct DFA {
		qy::weighted_graph graph;		// 存结点的图，可以默认start为0
		int start;						// 初始状态
		std::vector<int> accept_states; // 每个点的accept情况
	};

	/// @brief Construct DFA from a regular expression.
	class DFABuilder {
		constexpr static int EPSILON = 0;
		constexpr static int NON_ACCEPT = -1;

	public:
		/// @brief Convert input RE to stored NFA.
		/// @param re Regular expression.
		void add_re(const string& re, int index);

		DFA join_nfa() const;

	private:
		/// @brief Convert RE to NFA.
		/// @param re Regular expression.
		NFA re2nfa(const string& re, int index) const;

		/// @brief Convert stored NFA to DFA.
		DFA nfa2dfa(const NFA& nfa) const;

		/// @brief Minimize stored DFA. It won't be called if we use Brzozowski's Algorithm.
		// void minimize_dfa();

		NFA reverse(const DFA& dfa) const;
		NFA reverse(const NFA& nfa) const;

		/// @brief Used for constructing DFA.
		DFA subset(const NFA& nfa) const;

		DFA reachable(const DFA& dfa) const;

	private:
		std::vector<DFA> all_dfa;
	};

} // namespace comp

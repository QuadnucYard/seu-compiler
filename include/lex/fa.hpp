#pragma once
#include "utils/graph.hpp"
#include <string>
#include <vector>

namespace comp {

	using std::string;

	/// @brief Used for RE to NFA. It describes the start and end node in the NFA graph.
	struct SubNFA {
		int start;
		int end;
	};

	struct NFA {
		SubNFA top; // 顶层start/end。默认它的end为全局唯一的accept。仅在构造初始NFA时使用。
		qy::weighted_graph graph;
		std::vector<int> accept_states; // 用于做Scanner时存所有结点的accept情况
	};

	struct DFA {
		qy::weighted_graph graph;		// 存结点的图，可以默认start为0
		std::vector<int> accept_states; // 每个点的accept情况
	};

	/// @brief Construct DFA from a regular expression.
	class DFABuilder {
	public:
		/// @brief Convert input RE to stored NFA.
		/// @param re Regular expression.
		void add_re(const string& re);

		DFA join_nfa() const;

	private:
		/// @brief Convert RE to NFA.
		/// @param re Regular expression.
		NFA re2nfa(const string& re) const;

		/// @brief Convert stored NFA to DFA.
		DFA nfa2dfa(const NFA& nfa) const;

		/// @brief Minimize stored DFA. It won't be called if we use Brzozowski's Algorithm.
		// void minimize_dfa();

		/// @brief Used for constructing DFA.
		void /*Return type to add*/ subset_construction(/*Arguments to add*/) const;

	private:
		std::vector<DFA> all_dfa;
	};

} // namespace comp

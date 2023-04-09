#include "lex/fa.hpp"
#include "lex/tompson.hpp"

namespace comp {
	void DFABuilder::add_re(const string& re, int index) {
		all_dfa.push_back(nfa2dfa(re2nfa(re, index)));
	}

	NFA DFABuilder::re2nfa(const string& re, int index) const {
		return regexToNFA(re);
	}

	DFA DFABuilder::nfa2dfa(const NFA& nfa) const {
		return DFA();
	}

	DFA DFABuilder::join_nfa() const {
		// 图要join在一起，states也一样
		DFA dfa;
		dfa.accept_states.push_back(NON_ACCEPT);
		dfa.graph.resize(1);
		dfa.accept_states.resize(1);
		for (auto&& x : all_dfa) {
			dfa.accept_states.insert(dfa.accept_states.end(), x.accept_states.begin(),
									 x.accept_states.end());
			int s = static_cast<int>(dfa.graph.size()) + x.start; // start在新图中的位置
			dfa.graph.join(x.graph);
			dfa.graph.add_edge(0, s, EPSILON);
		}
		return dfa;
	}
} // namespace comp

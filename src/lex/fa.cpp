#include "lex/fa.hpp"

namespace comp {
	void DFABuilder::add_re(const string& re) {
		all_dfa.push_back(nfa2dfa(re2nfa(re)));
	}

	NFA DFABuilder::re2nfa(const string& re) const {
		return NFA();
	}

	DFA DFABuilder::nfa2dfa(const NFA& nfa) const {
		return DFA();
	}
} // namespace comp

#pragma once
#include "fa.hpp"
#include <stack>

namespace comp {

	class TompsonAlgo {
		static constexpr int CAT = 1;
		static constexpr sid_t EPSILON = DFABuilder::EPSILON;

	public:
		NFA operator()(std::string_view regex);

	private:
	public:
		void match_range(std::string_view str);
		void match_concat();
		void match_alt();
		void match_opt();
		void match_star();
		void match_plus();

	private:
		std::stack<SubNFA> nfa_stack;
		std::stack<int> op_stack;
		NFA nfa;
	};

	inline NFA tompson(std::string_view regex) {
		return TompsonAlgo()(regex);
	}

} // namespace comp

#pragma once
#include "fa.hpp"
#include <stack>

namespace comp {

	class TompsonAlgo {
	public:
		constexpr static int CAT = 1;
		constexpr static sid_t EPSILON = DFABuilder::EPSILON;
		constexpr static sid_t CHARSET_SIZE = DFABuilder::MAXS;

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

	inline NFA tompson(std::string_view regex) { return TompsonAlgo()(regex); }

} // namespace comp

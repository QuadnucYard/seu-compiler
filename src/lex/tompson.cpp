#include "lex/tompson.hpp"
#include "lex/regex.hpp"
#include <bitset>
#include <unordered_set>

namespace comp {

	NFA TompsonAlgo::operator()(std::string_view regex) {
		string _regex;
		for (char c : regex)
			_regex.push_back(abs(c));

		char cc = -1;
		for (size_t i = 0; i < regex.size(); i++) {
			char c = regex[i];
			switch (-c) {
			case '*':
				match_star();
				break;
			case '+':
				match_plus();
				break;
			case '?':
				match_opt();
				break;
			case '[':
				if (i > 0 && cc != -'(' && cc != -'|') {
					if (!op_stack.empty() && op_stack.top() == CAT)
						match_concat(); // 连接符号
					op_stack.push(CAT);
				}
				{
					size_t j = i;
					i = regex.find(-']', i);
					match_range(regex.substr(j + 1, i - j - 1));
				}
				break;
			case '(':
				if (i > 0 && cc != -'(' && cc != -'|') {
					if (!op_stack.empty() && op_stack.top() == CAT)
						match_concat(); // 连接符号
					op_stack.push(CAT);
				}
				op_stack.push('(');
				break;
			case ')':
				while (op_stack.size() && op_stack.top() == CAT)
					match_concat();
				if (op_stack.top() == '|')
					match_alt();
				if (op_stack.top() == '(')
					op_stack.pop();
				break;
			case '|':
				if (op_stack.top() == CAT)
					match_concat();
				if (op_stack.top() == '|')
					match_alt();
				op_stack.push('|');
				break;
			default:
				int n = static_cast<int>(nfa.graph.size());
				nfa.graph.resize(n + 2);
				if (auto matcher = wildcard_matcher::get(-c)) {
					for (int i = 0; i < CHARSET_SIZE; i++)
						if (matcher(i))
							nfa.graph.add_edge(n, n + 1, i);
				} else {
					nfa.graph.add_edge(n, n + 1, c);
				}
				nfa_stack.push({n, n + 1});
				if (cc > 0 || cc == -'*' || cc == -'+' || cc == -'?')
					op_stack.push(CAT);
				break;
			}
			cc = c;
		}
		while (op_stack.size() && op_stack.top() == CAT)
			match_concat();
		while (op_stack.size() && op_stack.top() == '|')
			match_alt();
		if (nfa.graph.size() == 0) {
			nfa.graph.resize(1);
			nfa.start = nfa.accept = 0;
		} else {
			nfa.start = nfa_stack.top().start;
			nfa.accept = nfa_stack.top().end;
		}
		return nfa;
	}

	auto get_range(std::string_view str) {
		std::bitset<TompsonAlgo::CHARSET_SIZE> id_bitset;
		size_t m = str.length();
		const auto add_char = [&id_bitset](char c) {
			if (auto matcher = wildcard_matcher::get(-c)) {
				for (int i = 0; i < TompsonAlgo::CHARSET_SIZE; i++)
					if (matcher(i))
						id_bitset.set(i);
			} else {
				id_bitset.set(c);
			}
		};
		for (auto prev = std::string::npos;;) {
			size_t p = str.find('-', prev + 1);
			if (p == std::string::npos) { // 找完了
				for (size_t i = prev + 1; i < m; i++)
					add_char(str[i]);
				break;
			} else if (p == 0 || p == m - 1) { // 开头结尾的
				id_bitset.set('-');
				prev = p;
			} else {
				for (size_t i = prev + 1; i < p - 1; i++)
					add_char(str[i]);
				for (char c = str[p - 1]; c <= str[p + 1]; c++)
					id_bitset.set(c);
				prev = p + 1;
			}
		}
		return id_bitset;
	}

	void TompsonAlgo::match_range(std::string_view str) {
		auto id_bitset = !str.empty() && str[0] == -'^' ? get_range(str.substr(1)).flip()
														: get_range(str);
		int n = static_cast<int>(nfa.graph.size());
		nfa.graph.resize(n + 2);
		SubNFA new_sub{n, n + 1};
		nfa_stack.push(new_sub);
		for (int c = 0; c < CHARSET_SIZE; c++)
			if (id_bitset.test(c))
				nfa.graph.add_edge(new_sub.start, new_sub.end, c);
	}

	void TompsonAlgo::match_concat() {
		op_stack.pop();
		SubNFA nfa2 = nfa_stack.top();
		nfa_stack.pop();
		SubNFA nfa1 = nfa_stack.top();
		nfa_stack.pop();
		nfa.graph.add_edge(nfa1.end, nfa2.start, EPSILON);
		nfa_stack.push({nfa1.start, nfa2.end});
	}

	void TompsonAlgo::match_alt() {
		op_stack.pop();
		SubNFA nfa2 = nfa_stack.top();
		nfa_stack.pop();
		SubNFA nfa1 = nfa_stack.top();
		nfa_stack.pop();
		int n = static_cast<int>(nfa.graph.size());
		nfa.graph.resize(n + 2);
		nfa.graph.add_edge(n, nfa1.start, EPSILON);
		nfa.graph.add_edge(n, nfa2.start, EPSILON);
		nfa.graph.add_edge(nfa1.end, n + 1, EPSILON);
		nfa.graph.add_edge(nfa2.end, n + 1, EPSILON);
		nfa_stack.emplace(n, n + 1);
	}

	void TompsonAlgo::match_opt() {
		SubNFA& nfa1 = nfa_stack.top();
		int n = static_cast<int>(nfa.graph.size());
		nfa.graph.resize(nfa.graph.size() + 2);
		nfa.graph.add_edge(n, nfa1.start, EPSILON);
		nfa.graph.add_edge(nfa1.end, n + 1, EPSILON);
		nfa.graph.add_edge(n, n + 1, EPSILON);
		nfa1 = {n, n + 1};
	}

	void TompsonAlgo::match_star() {
		SubNFA& nfa1 = nfa_stack.top();
		int n = static_cast<int>(nfa.graph.size());
		nfa.graph.resize(n + 2);
		nfa.graph.add_edge(n, nfa1.start, EPSILON);
		nfa.graph.add_edge(nfa1.end, n + 1, EPSILON);
		nfa.graph.add_edge(nfa1.end, nfa1.start, EPSILON);
		nfa.graph.add_edge(n, n + 1, EPSILON);
		nfa1 = {n, n + 1};
	}

	void TompsonAlgo::match_plus() {
		SubNFA& nfa1 = nfa_stack.top();
		int n = static_cast<int>(nfa.graph.size());
		nfa.graph.resize(n + 2);
		nfa.graph.add_edge(n, nfa1.start, EPSILON);
		nfa.graph.add_edge(nfa1.end, n + 1, EPSILON);
		nfa.graph.add_edge(nfa1.end, nfa1.start, EPSILON);
		nfa1 = {n, n + 1};
	}
} // namespace comp

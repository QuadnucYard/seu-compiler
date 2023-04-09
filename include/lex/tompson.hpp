#pragma once
#include "fa.hpp"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

// 连接符号：1
// acceptstate:非
// epsilon边： 0

namespace comp {
	// 规约连接符号
	void Connection_id_re(std::stack<SubNFA>& nfastack, std::stack<int>& opstack, NFA nfa) {
		opstack.pop();
		SubNFA nfa2 = nfastack.top();
		nfastack.pop();
		SubNFA nfa1 = nfastack.top();
		nfastack.pop();
		// SubNFA newsub;
		// newsub.start = nfa1.start;
		// newsub.end = nfa2.end;
		// nfa.top = newsub;
		nfa.graph.add_edge(nfa1.end, nfa2.start, 0);
		nfastack.push({nfa1.start, nfa2.end});
	}

	void substring_re(std::string substring, std::stack<SubNFA>& nfastack, std::stack<int>& opstack,
					  NFA nfa) {
		std::vector<int> idset;
		while (substring.find('-') != std::string::npos) {
			int pos = substring.find('-');
			if (substring[pos - 1] == '[' || substring[pos + 1] == ']')
				break;
			for (int j = substring[pos - 1]; j <= substring[pos + 1]; j++) {
				idset.push_back(j);
			}
			substring.erase(pos - 1);
			substring.erase(pos);
			substring.erase(pos + 1);
		}

		for (int j = 1; j < substring.size() - 1; j++) {
			idset.push_back(substring[j]);
		}

		nfa.graph.resize(nfa.graph.size() + 2);
		SubNFA newsub = {nfa.graph.size() - 2, nfa.graph.size() - 1};
		nfastack.push(newsub);
		for (int j = 0; j < idset.size(); j++) {
			nfa.graph.add_edge(newsub.start, newsub.end, idset[j]);
		}
	}

	void or_re(std::stack<SubNFA>& nfastack, std::stack<int>& opstack, NFA nfa) {
		opstack.pop();
		SubNFA nfa2 = nfastack.top();
		nfastack.pop();
		SubNFA nfa1 = nfastack.top();
		nfastack.pop();
		nfa.graph.resize(nfa.graph.size() + 2);
		nfa.graph.add_edge(nfa.graph.size() - 2, nfa1.start, 0);
		nfa.graph.add_edge(nfa.graph.size() - 2, nfa2.start, 0);
		nfa.graph.add_edge(nfa1.end, nfa.graph.size() - 1, 0);
		nfa.graph.add_edge(nfa2.end, nfa.graph.size() - 1, 0);
		nfastack.push({(int)nfa.graph.size()-2, (int)nfa.graph.size()-1});
	}

	void close_re(std::stack<SubNFA>& nfastack, std::stack<int>& opstack, NFA nfa){
		SubNFA nfa1=nfastack.top();
		nfa.graph.resize(nfa.graph.size() + 2);
		nfa.graph.add_edge(nfa.graph.size() - 2, nfa1.start, 0);
		nfa.graph.add_edge(nfa1.end, nfa.graph.size()-1, 0);
		nfa.graph.add_edge(nfa.graph.size()-2,nfa.graph.size()-1,0);
		nfa1.start=nfa.graph.size()-2;
		nfa1.end=nfa.graph.size()-1;
	}

	void star_re(std::stack<SubNFA>& nfastack, std::stack<int>& opstack, NFA nfa){
		SubNFA nfa1=nfastack.top();
		nfa.graph.resize(nfa.graph.size() + 2);
		nfa.graph.add_edge(nfa.graph.size() - 2, nfa1.start, 0);
		nfa.graph.add_edge(nfa1.end, nfa.graph.size()-1, 0);
		nfa.graph.add_edge(nfa1.end,nfa1.start,0);
		nfa.graph.add_edge(nfa.graph.size()-2,nfa.graph.size()-1,0);
		nfa1.start=nfa.graph.size()-2;
		nfa1.end=nfa.graph.size()-1;
	}
	
	void plus_re(std::stack<SubNFA>& nfastack, std::stack<int>& opstack, NFA nfa){
		SubNFA nfa1=nfastack.top();
		nfa.graph.resize(nfa.graph.size() + 2);
		nfa.graph.add_edge(nfa.graph.size() - 2, nfa1.start, 0);
		nfa.graph.add_edge(nfa1.end, nfa.graph.size()-1, 0);
		nfa.graph.add_edge(nfa1.end,nfa1.start,0);
		nfa1.start=nfa.graph.size()-2;
		nfa1.end=nfa.graph.size()-1;
	}

	NFA regexToNFA(string regex) {
		NFA nfa;
		std::stack<SubNFA> nfaStack;
		std::stack<int> opstack;
		int stateCounter = 0;
		for (int i = 0; i < regex.size(); i++) {
			if (regex[i] == '[') {
				if (i > 0 && regex[i - 1] != '(' && regex[i - 1] != '|') {
					// 连接符号
					if (opstack.top() == 1) {
						Connection_id_re(nfaStack, opstack, nfa);
					}
					opstack.push(1);
				}
				std::string substring;
				while (regex[i] != ']')
					substring += regex[i++];
				substring += regex[i];
				substring_re(substring, nfaStack, opstack, nfa);
			} else if (regex[i] == '(') {
				if (i > 0 && regex[i - 1] != '(' && regex[i - 1] != '|') {
					// 连接符号
					if (opstack.top() == 1) {
						Connection_id_re(nfaStack, opstack, nfa);
					}
					opstack.push(1);
					opstack.push(regex[i]);
				} else
					opstack.push(regex[i]);
			} else if (regex[i] == ')') {
				if (opstack.top() == '|') {
					or_re(nfaStack,opstack,nfa);
				}
				if(opstack.top()=='('){
					opstack.pop();
				}
			} else if (regex[i] == '|') {
				if(opstack.top()==1){
					Connection_id_re(nfaStack,opstack,nfa);
				}
				if(opstack.top()=='|'){
					or_re(nfaStack,opstack,nfa);
				}
				opstack.push(regex[i]);
			} else if (regex[i] == '*') {
				star_re(nfaStack,opstack,nfa);
			} else if (regex[i] == '+') {
				plus_re(nfaStack,opstack,nfa);
			} else if (regex[i] == '?') {
				close_re(nfaStack,opstack,nfa);
			} else {
				nfa.graph.resize(nfa.graph.size()+2);
				nfa.graph.add_edge(nfa.graph.size()-2,nfa.graph.size()-1,regex[i]);
				nfaStack.push({(int)nfa.graph.size()-2,(int)nfa.graph.size()-1});
				if(isalpha(regex[i-1]))
					opstack.push(1);	
			}
		}
		if(opstack.top()==1)
			Connection_id_re(nfaStack,opstack,nfa);
		if(opstack.top()=='|')
			or_re(nfaStack,opstack,nfa);
		return nfa;
	}
} // namespace comp


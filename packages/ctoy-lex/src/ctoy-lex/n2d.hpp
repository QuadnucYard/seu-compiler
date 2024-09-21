#include "fa.hpp"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include<queue>
#include<algorithm>
namespace comp{
    //nfa转dfa:
    //（1）对于起始状态和有epsilon边的状态，归为一个集合记为I0
    //（2）对于这个集合，判定对于下一个输入符号，他们会进入的状态，以及通过epsilon边会到达的状态，记为I1…
    //（3）对于每一个new的集合，生成结束之后进行判断，如果装有集合的vector里已经有了，那么丢弃，否则pushback
    //（4）重复这一操作，直到对于vector里的每一个集合，对应于输入符号都有已知的下一步状态。

    DFA Subset_construct(NFA nfa){
        //明确非终结符
        DFA dfa;
        //dfa.accept_states=nfa.accept_states;
        
        std::vector<int>tokens;
        std::vector<int>::iterator it;
		for (auto [u, v, w] : nfa.graph.edges())
			it=std::find(tokens.begin(), tokens.end(),w);
            if(it!=tokens.begin() && it==tokens.end())tokens.push_back(w);//把当前的字符都读进来

        std::queue<vector<int>> waitsets;
        std::vector<int> subset0;
        sebset0.pushback(0);
        std::vector<vector<int>>subsets;

        //将0状态的epsilon边可达的节点放入subset0
        for (auto [v, w] : nfa.graph.iter_edges(static_cast<int>(nfa.top.start))){
            if(w==0){
                subset.push_back(v);
            }
        }
			
        //subsets(用于check)，waitsets(用于确定对应每一终结符)
        subsets.push_back(subset0);
        waitsets.push(subset0);

        while(waitsets.size()!=0){
            std::vector<int> subset =waitsets.top();
            std::vector<vector<int>>::iterator it;
            int i_1 = std::find(subsets.begin(), subsets.end(), subset) - subsets.begin();
            waitsets.pop();
            for(int w : tokens){
                std::vector<int> succ;
                std::queue<int> waitset;
                //move
                for(int i : subset){
                    for (auto [v, w_1] : nfa.graph.iter_edges(static_cast<int>(i))){
                        if(w_1==w){
                            std::vector<int>::iterator it=std::find(succ.begin(), succ.end(), v);
                            if(it==subset.end()){
                               succ.pushback(v);
                               waitset.push(v);
                            }
                        }
                    }
                }
                //follow epsilon
                while(waitset.size()){
                    int t = waitset.front();
                    waitset.pop();
                    for (auto [v, w_1] : nfa.graph.iter_edges(static_cast<int>(t))){
                        if(w_1 == 0){
                            std::vector<int>::iterator it=find(succ.begin(),succ.end(),v);
                            if(it == subset.end()){
                               succ.pushback(v);
                               waitset.push(v);
                            }
                        }   
                    }
                }
                
                if(succ.size() != 0){
                    //有可达的状态
                    std::vector<vector<int>>::interator iter = std::find(subsets.begin(),subsets.end(),succ);
                    if(iter == subset.end()){
                        //succ是新状态
                        subsets.push_back(succ);
                        int n=dfa.graph.size();
                        dfa.graph.resize(n + 1);
                        dfa.graph.add_edge(i_1, n, w);
                        //加入acc
                        for(int i_3 : succ){
                            if(nfa.accept_states(i_3)){
                                dfa.accept_states.push_back(nfa.accept_states(i_3));
                                break;
                            }
                            if(dfa.accept_states.size() < dfa.graph.size())dfa.accept_states.push_back(0);
                        }
                    }
                    else{
                        //succ是旧状态
                        int i_2 = iter - subsets.begin();
                        dfa.graph.add_edge(i_1, i_2, w);
                    }
                }
            }
        }
        return dfa;
    }

    DFA minDFA(NFA nfa){
        //(re->)nfa->reverse(nfa)->subset(reverse(nfa)):dfa->reverse(dfa):nfa->subset:dfa
        //reverse(nfa)
        NFA re_nfa;
        re_nfa.top.start=nfa.top.end;
        re_nfa.top.end=nfa.top.start;
        int nfa_size=nfa.graph.size();
        int re=nfa.accept_states[nfa.top.end];
        
        for(int i=0;i<nfa_size;i++){
            re_nfa.accept_states.push_back(0);
        }
        re_nfa.accept_states[re_nfa.top.end]=re;
        //re_nfa.accept_states[re_nfa.top.start]=0;
        re_nfa.graph=nfa.graph.reverse();

        //subset(reverse(nfa))
        DFA dfa1=Subset_construct(re_nfa);

        //reverse
        NFA re_dfa;
        int new_size=dfa1.graph.size()+1;
        dfa1.graph.resize(new_size);
        for(int i = 0; i < dfa1.graph.size() - 1; i++){
            //int re;
            if(dfa1.accept_states.at(i) != 0){
                //re=dfa1.accept_states.at(i);
                //dfa1.accept_states.at(i)=0;
                dfa1.graph.add_edge(i, new_size-1, 0);
            }
        }
        re_dfa.top.start = dfa1.graph.size() - 1;
        re_dfa.top.end = 0;
        for(int i = 0; i < dfa1.graph.size(); i++){
            re_dfa.accept_states.push_back(0);
        }
        re_dfa.accept_states[0]=re;
        re_dfa.graph=dfa1.graph.reverse();

        //minimize
        DFA new_dfa = Subset_construct(re_dfa);
        return new_dfa;
    }
}
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>
#include "fa.hpp"

namespace comp {
    std::vector<int>equivalent_class(128);
    std::vector<std::vector<int>>yy_nxt;//
    std::vector<std::pair<int,int>>valid_lengths;

    void gen_ec_table(DFA dfa, std::vector<int> & equivalent_class, std::vector<std::vector<int>>&yy_nxt, std::vector<std::pair<int,int>>&valid_lengths){
        int size = dfa.accept_states.size();
        for(int i=0;i<size;i++){
            std::vector<int>move(128,-1);
            for(auto &[v,w]: dfa.graph.iter_edges(i)){
                move[w] = v;
            }
            yy_nxt.push_back(move);

            //这里没有考虑所有状态都不合法的情况，待补充
            std::pair<int,int>length;
            for(int i=0;i<128;i++){
                if(move[i]!=-1)length.first=i;
                break;
            }
            for(int i=127;i>=0;i--){
                if(move[i]!=-1)length.second=i;
                break;
            }
            valid_lengths.push_back(length);
        }
        for(int i=0;i<128;i++){
            equivalent_class[i]=i;
        }
        for(int i=0;i<128;i++){
            if(equivalent_class[i]==i){
                for(int j=i;j<128;j++){
                    bool same;
                    if(equivalent_class[j]==j){
                        same = true;
                        for(int k=0; k<size;k++){
                            if(yy_nxt[k][j]!=yy_nxt[k][i]){
                                same = false;
                                break;
                            }
                        }
                        if(same) equivalent_class[j]=i;
                    }
                }
            }
           
        }

    };
    void gen_chk_table(std::vector<std::pair<int,int>>valid_lengths, std::vector<std::vector<int>>yy_nxt){
        std::vector<int>chk_tbl={};
        std::vector<int>nxt_tbl = {};
        std::vector<int>base_tbl= {};
        for(int i=0; i<valid_lengths.size();i++){
            int len=valid_lengths[i].second-valid_lengths[i].first;
            for(int temp=0; temp< chk_tbl.size(); i++){
                bool safe = true;
                for(int t = temp; t< len ;t++){
                    if( temp+t < chk_tbl.size() && chk_tbl[t+temp]!=-1 && yy_nxt[i][t]!=-1 && yy_nxt[i][t]!=chk_tbl[t+temp]){
                        safe = false;
                        break;
                    }
                }
                if(safe){
                    if(temp +len > chk_tbl.size()){
                        std::copy_n(yy_nxt[i].begin()+valid_lengths[i].first, chk_tbl.size()-temp,chk_tbl.begin()+temp);
                        chk_tbl.insert(chk_tbl.end(),valid_lengths[i].first+chk_tbl.size()-temp, yy_nxt[i].begin()+valid_lengths[i].second); 
                        nxt_tbl.resize(chk_tbl.size());                     
                    }
                    else {
                        std::copy_n(yy_nxt[i].begin()+valid_lengths[i].first, len, chk_tbl.begin()+temp);
                    } 
                    for(int k = 0; k<len; k++){
                        int cur =yy_nxt[i][valid_lengths[i].first+k];
                        if(cur!=-1) {
                            nxt_tbl[k+temp]=cur;
                        }

                    } 
                    base_tbl.push_back(temp-valid_lengths[i].first) ;
                    break;           
                }
            }
        }

    }



}
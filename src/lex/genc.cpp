#include "lex/genc.hpp"
#include "lex/fa.hpp"
#include "lex/lex.hpp"
#include "lex/lparser.hpp"
#include "utils/outfmt.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>
#include <vector>

extern const char* lex_tmpl;

namespace comp {
	const auto plus1 = std::views::transform([](auto&& x) { return x + 1; });

	LexCodeGen::LexCodeGen(const Lex& lexer, const LParser& lparser) :
		lexer{lexer}, lparser{lparser}, tmpl{lex_tmpl} {}

	void LexCodeGen::operator()(const DFA& dfa) {
		gen_accept_table(dfa);
		if (lexer.options.compress){
			gen_all_table(dfa);
		}
		else 
			gen_nxt_table(dfa);
		gen_case();

		tmpl.set_string("[[USER_CODE_1]]", fmt::to_string(fmt::join(lparser.prologues, "")));
		tmpl.set_string("[[USER_CODE_3]]", lparser.epilogue);
	}

	void LexCodeGen::gen_nxt_table(const DFA& dfa) {
		tmpl.set_bool("C1", false);
		std::string result;
		int move[128]{};
		int size = static_cast<int>(dfa.size());

		// accept为size-1的是额外加的那个。需要在最后补一个 catch \0 的。开头也补了一个全0的

		result += qy::format_array(move, {.field_width = 5});
		result += ",";

		for (int i = 1; i <= size; i++) {
			std::ranges::fill(move, -i);
			for (auto&& [v, w] : dfa.graph.iter_edges(i - 1))
				move[w] = v + 1;
			if (i < size)
				move[0] = size;

			result += qy::format_array(move, {.field_width = 5});
			result += ",";
		}

		tmpl.set_string("[[YY_NXT]]", result);

		std::vector<int> nultrans(size + 1, 0);
		int catchall = 0;
		for (int i = 0; i < size; i++) {
			if (dfa.accept_states[i] == lparser.rules.size() - 1)
				catchall = i + 1;
		}

		for (int i = 1; i <= size; i++) {
			nultrans[i] = dfa.accept_states[i - 1] == lparser.rules.size() - 1 ? catchall : 0;
		}
		tmpl.set_string("[[YY_NUL_TRANS]]", qy::format_array(nultrans, {.with_brace = false}));
	};

	void LexCodeGen::gen_accept_table(const DFA& dfa) {
		/*
        #define YY_NUM_RULES 101
        #define YY_END_OF_BUFFER 102
        */
		tmpl.set_string("[[YY_NUM_RULES]]", fmt::to_string(lparser.rules.size()));
		tmpl.set_string("[[YY_END_OF_BUFFER]]", fmt::to_string(lparser.rules.size() + 1));
		//yy_accept
		auto accept_states = dfa.accept_states;
		// accept_states.push_back(static_cast<vid_t>(lexer.actions.size()));
		tmpl.set_string("[[YY_ACCEPT]]",
						qy::format_array(accept_states | plus1, {.with_brace = false}));
	}

    void LexCodeGen::gen_all_table(const DFA& dfa){
        //yy_ec
		tmpl.set_bool("C1", true);	
        int size = static_cast<int>(dfa.accept_states.size());
        std::vector<std::pair<int,int>>valid_len; 
		std::vector<std::vector<int>>yy_nxt;
        for(int i = 1; i <= size; i++){
            std::vector<int>move(128, -i);
            for(auto &[v,w]: dfa.graph.iter_edges(i - 1)){
                move[w] = v + 1;
            }
            move[0] = size + 1;
            yy_nxt.push_back(move);
        }
        std::vector<int>equivalent_class(128);
        for(int i = 0; i < 128; i++){
            equivalent_class[i] = i;
        }
        for(int i = 0; i < 128; i++){
            if(equivalent_class[i] == i){
                for(int j = i; j < 128; j++){
                    bool same;
                    if(equivalent_class[j] == j){
                        same = true;
                        for(int k = 0; k < size; k++){
                            if(yy_nxt[k][j] != yy_nxt[k][i]){
                                same = false;
                                break;
                            }
                        }
                        if(same) equivalent_class[j] = i;
                    }
                }
            }          
        }
		std::vector<int>eq;
		for (int i = 0; i < equivalent_class.size(); i++) {
			auto it = std::find(eq.begin(), eq.end(), equivalent_class[i]);
			if (it == eq.end()) 
				eq.push_back(equivalent_class[i]);
		}
		for (int i = 0; i < equivalent_class.size(); i++) {
			auto it = std::find(eq.begin(), eq.end(), equivalent_class[i]);
			int dis = it - eq.begin();
			if (dis != equivalent_class[i])equivalent_class[i] = dis;
		}

        tmpl.set_string("[[YY_EC]]", qy::format_array(equivalent_class, {.with_brace = false}));
		
		//yy_nul_trans
		std::vector<int> nultrans(size + 1, 0);
		int catchall = 0;
		for (int i = 0; i < size; i++) {
			if (dfa.accept_states[i] == lparser.rules.size() - 1)
				catchall = i + 1;
		}

		for (int i = 1; i <= size; i++) {
			nultrans[i] = dfa.accept_states[i - 1] == lparser.rules.size() - 1 ? catchall : 0;
		}
		tmpl.set_string("[[YY_NUL_TRANS]]", qy::format_array(nultrans, {.with_brace = false}));

        //暴力修改move
        int ec_size = *max_element(equivalent_class.begin(), equivalent_class.end()); 
        for(int i=0; i<= ec_size; i++){
            auto offset = find(equivalent_class.begin(), equivalent_class.end(), i)- equivalent_class.begin();
            for(int j = 0; j < size; j++){
                yy_nxt[j][i] = yy_nxt[j][offset]< 0 ? yy_nxt[j][offset]+1: yy_nxt[j][offset] ;
            }
        }
        for(int j = 0; j < size; j++){
            yy_nxt[j].resize(ec_size + 1);
        }
        


        //这里没有考虑所有状态都不合法的情况，待补充（？）
		for(int i=0; i<size; i++){
			std::pair<int,int>length;
			for(int j = 0; j <= ec_size; j++){
				if(yy_nxt[i][j] != -i){
					length.first = j;
					break;
				}
			}
			for(int j = ec_size; j >= 0; j--){
				if(yy_nxt[i][j] != -i){
					length.second = j;
					break;
				}
			}
			valid_len.push_back(length);
		}
		
        std::vector<int>chk_tbl={};
        std::vector<int>nxt_tbl = {};
        std::vector<int>base_tbl= {};

        for(int i=0; i < valid_len.size(); i++){
            int len = valid_len[i].second - valid_len[i].first + 1;
            for(int temp = 0; temp <= nxt_tbl.size(); temp++){
                bool safe = true;
                for(int t = 0; t < len; t++){
                    if( temp + t < nxt_tbl.size() && nxt_tbl[t+temp]!= -1000 && yy_nxt[i][t]!= -i){
                        safe = false;
                        break;
                    }
                }
                if(safe){
                    if(temp + len > nxt_tbl.size()){
                        nxt_tbl.resize(temp + len, -1000);
                        chk_tbl.resize(temp + len, -1000);
                    }
                    for(size_t j = 0; j< yy_nxt[i].size() ; j++){
                        if(yy_nxt[i][j]!= -i){
                            nxt_tbl[temp + j] = yy_nxt[i][j];
                            chk_tbl[temp + j] = i;
                        }
                    } 
                    base_tbl.push_back(temp - valid_len[i].first) ;
                    break;           
                }
            }
        }
		// int count=chk_tbl.size()-1;
		// while(count>=0){
		// 	if(chk_tbl[count]!=-1000)break;
		// 	else count--;
		// }
		// chk_tbl.resize(count+1);
		// nxt_tbl.resize(count+1);

        tmpl.set_string("[[YY_BASE]]", qy::format_array(base_tbl, {.with_brace = false}));
        tmpl.set_string("[[YY_NXT]]", qy::format_array(nxt_tbl, {.with_brace = false}));
        tmpl.set_string("[[YY_CHK]]", qy::format_array(chk_tbl, {.with_brace = false}));
    }

	/*
    #include "y.tab.h"
    #include <stdio.h>

    void lineColumn(void);
    void multiLineComment(void);
    */

	void LexCodeGen::gen_case() {
		//对应yy.c：line 6210起
		std::string result;
		//第一个case定义为-1了,为了协调在这里先加1和flex写法一致
		for (size_t i = 0; i < lparser.rules.size(); i++) {
			result += fmt::sprintf(
				R"(case %d:
YY_RULE_SETUP
%s
    YY_BREAK
)",
				i + 1, lparser.rules[i].action);
		}
		tmpl.set_string("[[ACTIONS]]", result);
	};

	/*
	下面是yywrap等部分
	*/

} // namespace comp
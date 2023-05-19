#include "lex/genc.hpp"
#include "lex/fa.hpp"
#include "lex/lexer.hpp"
#include "utils/outfmt.hpp"
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <ranges>

extern const char* lex_tmpl;

namespace comp {
	const auto plus1 = std::views::transform([](auto&& x) { return x + 1; });

	LexCodeGen::LexCodeGen(const Parser& parser, const Lexer& lexer) : lexer{lexer}, parser{parser}, tmpl{lex_tmpl} {}

	void LexCodeGen::operator()(const DFA& dfa) {
		gen_accept_table(dfa);
		if (parser.options.compress){
			gen_nxt_table(dfa);
		}
		else 
			gen_all_table(dfa);
		gen_case();
	}

	void LexCodeGen::gen_nxt_table(const DFA& dfa) {
		temp.set_bool("UNCOMPRESS", true);		
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
			if (dfa.accept_states[i] == lexer.actions.size() - 1)
				catchall = i + 1;
		}

		for (int i = 1; i <= size; i++) {
			nultrans[i] = dfa.accept_states[i - 1] == lexer.actions.size() - 1 ? catchall : 0;
		}
		tmpl.set_string("[[YY_NUL_TRANS]]", qy::format_array(nultrans, {.with_brace = false}));
	};

	void LexCodeGen::gen_accept_table(const DFA& dfa) {
		/*
        #define YY_NUM_RULES 101
        #define YY_END_OF_BUFFER 102
        */
		tmpl.set_string("[[YY_NUM_RULES]]", fmt::to_string(lexer.actions.size()));
		tmpl.set_string("[[YY_END_OF_BUFFER]]", fmt::to_string(lexer.actions.size() + 1));
		//yy_accept
		auto accept_states = dfa.accept_states;
		// accept_states.push_back(static_cast<vid_t>(lexer.actions.size()));
		tmpl.set_string("[[YY_ACCEPT]]",
						qy::format_array(accept_states | plus1, {.with_brace = false}));
	}

    void LexCodeGen::gen_all_table(const DFA& dfa){
        //yy_ec
		temp.set_bool("UNCOMPRESS", false);	
        int size = static_cast<int>(dfa.accept_states.size());
        vector<std::pair<int,int>>valid_len; 
        for(int i = 1; i <= size; i++){
            vector<int>move(128, -i);
            for(auto &[v,w]: dfa.graph.iter_edges(i - 1)){
                move[w] = v + 1;
            }
            move[0] = size + 1;
            yy_nxt.push_back(move);
        }
        vector<int>equivalent_class(128);
        for(int i = 0; i < 128; i++){
            equivalent_class[i] = i;
        }
        for(int i = 0; i < 128; i++){
            if(equivalent_class[i] == i){
                for(int j = i; j < 128; j++){
                    bool same;
                    if(equivalent_class[j] == j){
                        same = true;
                        for(int k=0; k < ize; k++){
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
        tmpl.set_string("[[YY_EC]]", qy::format_array(equivalent_class, {.with_brace = false}));
		
		//yy_nul_trans
		std::vector<int> nultrans(size + 1, 0);
		int catchall = 0;
		for (int i = 0; i < size; i++) {
			if (dfa.accept_states[i] == lexer.actions.size() - 1)
				catchall = i + 1;
		}

		for (int i = 1; i <= size; i++) {
			nultrans[i] = dfa.accept_states[i - 1] == lexer.actions.size() - 1 ? catchall : 0;
		}
		tmpl.set_string("[[YY_NUL_TRANS]]", qy::format_array(nultrans, {.with_brace = false}));

        //暴力修改move
        int ec_size = *max_element(equivalent_class.begin(), equivalent_class.end()); 
        for(int i=0; i<= ec_size; i++){
            vector<int>::iterator it = find(equivalent_class.begin(), equivalent_class.end(),i);
            int offset = it-equivalent_class.begin();
            for(int j = 1; j <= size ;j++){
                move[j][i]=move[j][offset];
            }
        }
        for(int j = 1; j <= size ;j++){
            move[j].resize(ec_size + 1);
        }

        //这里没有考虑所有状态都不合法的情况，待补充（？）
		std::pair<int,int>length;
		for(int j = 0; j <= ec_size; j++){
			if(move[j] != -i)length.first = j;
			break;
		}
		for(int j = ec_size; j >= 0; j--){
		if(move[j] != -i)length.second = j;
			break;
		}
		valid_len.push_back(length);

        vector<int>chk_tbl={};
        vector<int>nxt_tbl = {};
        vector<int>base_tbl= {};

        for(int i=0; i < valid_len.size(); i++){
            int len = valid_len[i].second - valid_len[i].first;
            for(int temp = 0; temp <= nxt_tbl.size(); i++){
                bool safe = true;
                for(int t = temp; t < len; t++){
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
                    for(size_t j = 0; j < len;j++){
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
		for (size_t i = 0; i < lexer.actions.size(); i++) {
			result += fmt::sprintf(
				R"(case %d:
YY_RULE_SETUP
%s
    YY_BREAK
)",
				i + 1, lexer.actions[i]);
		}
		tmpl.set_string("[[ACTIONS]]", result);
	};

	/*
	下面是yywrap等部分
	*/

} // namespace comp
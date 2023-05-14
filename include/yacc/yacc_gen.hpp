#pragma once
// #include "analyzer.hpp"
#include "utils/templater.hpp"
#include <fstream>
#include <iostream>
// #include <format> 
#include <fmt/core.h>
#include<vector>
#include"parser.hpp"

namespace comp {
	class yacc_code {
        public:
		std::ofstream of;
		std::vector<std::vector<int>> LALR1_action;
		std::vector<std::vector<int>> LALR1_goto;
        
		void gen_table();
		void set_action(std::vector<std::vector<int>> t_action);
        void set_goto(std::vector<std::vector<int>> t_goto);
        void gen_case(const Parser& parser);
	};
} // namespace comp
#include "yacc/yacc_gen.hpp"

int main(int argc, char const* argv[]) {
	using namespace comp;
    yacc_code test;
    test.set_action({{1,2}});
    test.set_goto({{3,4}});
    test.gen_table();
    return 0;
}
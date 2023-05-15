#include "yacc/analyzer.hpp"

int main(int argc, char const* argv[]) {
	using namespace comp;
	SyntacticAnalyzer b;
    std::vector<production> test_rules;
    test_rules.push_back({'S',{'s'}," "});
	test_rules.push_back({'s',{'0','s','1'}," "});
	test_rules.push_back({'S',{'0','1'}," "});
    b.rules=test_rules;
	b.process();
	return 0;
}

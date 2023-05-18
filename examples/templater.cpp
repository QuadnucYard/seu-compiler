#include "utils/templater.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
	// Pass a string as template.
	qy::templater temp{R"(======
		int a;
		[[something]]
		int b[5];
	=====)"};
	temp.set_string("[[something]]", "char c = 'a';");
	cout << temp.str() << endl;

	// It can be also constructed from path.
	/* qy::templater temp2{"E:\\Project\\seu-compiler\\examples\\aaa.c"};
	temp2.set_string("[[anything]]", "char cccc = 'a';");
	cout << temp2.str() << endl; */

	// Use auto-linked variable
	extern const char* TEMPLATE;
	// auto xx = std::string{TEMPLATE};
	std::ofstream out("test.out.cpp");
	out << TEMPLATE;

	/* qy::templater temp3{R"(======
		0000
		[[IF(cond)]]
			1111
			[[IF(cond2)]]
			6666
			[[FI]]
			2222
		[[FI]]
		000
		[[IF(cond)]]
			3333
			[[IF(cond2)]]
			4444
			[[FI]]
			5555
		[[FI]]
		00
=====)"}; */
	qy::templater temp3{R"(======
		0000
		[[IF(cond)]]
			1111
		[[ELSE]]
			1000
		[[FI]]
			6666
		[[IF(cond2)]]
			2222
		[[FI]]
		000
		[[IF(cond)]]
			3333
		[[FI]]	
			4444
		[[IF(cond2)]]
			5555
		[[FI]]
		00
=====)"};
	temp3.set_bool("cond2", true);
	temp3.set_bool("cond", false);
	cout << temp3.str() << endl;
	return 0;
}

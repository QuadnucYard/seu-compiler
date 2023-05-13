#include "utils/templater.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
	// Pass a string as template.
	qy::templater temp{R"(======
		int a;
		$something
		int b[5];
	=====)"};
	temp.set_string("$something", "char c = 'a';");
	cout << temp.str() << endl;

	// It can be also constructed from path.
	qy::templater temp2{"E:\\Project\\seu-compiler\\examples\\aaa.c"};
	temp2.set_string("$anything", "char cccc = 'a';");
	cout << temp2.str() << endl;
	return 0;
}

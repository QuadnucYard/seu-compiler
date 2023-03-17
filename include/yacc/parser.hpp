#include "common/recognizer.hpp"
#include <vector>

namespace comp {

	struct GrammarRule {
		string lhs;
		std::vector<std::vector<string>> rhs;
	};

	class Parser: public Recognizer {
	public:
		void process(const fs::path& src_path);
	private:
		string start_symbol;
		std::vector<GrammarRule> rules;

		struct DeclHandler;
		struct RulesHandler;
	};
}

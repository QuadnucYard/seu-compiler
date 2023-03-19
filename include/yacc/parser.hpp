#pragma once
#include "common/recognizer.hpp"
#include "analyzer.hpp"

namespace comp {

	class Parser: public Recognizer {
	private:

		/// @brief Raw description of rules
		struct RawRule {
			string lhs;
			std::vector<std::vector<string>> rhs;
		};

	public:
		void process(const fs::path& src_path);
	private:
		string start_symbol;
		std::vector<RawRule> rules;
		std::unordered_map<int, int> token_map; // Token id to index

		struct DeclHandler;
		struct RulesHandler;
	};
}

#pragma once
#include "common/gparser.hpp"
#include <unordered_map>

namespace comp {
	class LParser : GParser {
		friend class Lexer;
		friend class LexCodeGen;

	private:
		struct Rule {
			string expr;
			string action;
		};

		std::unordered_map<string, string> definitions;
		std::vector<Rule> rules;

	public:
		LParser(std::istream& in);

		void parse();

		/// @brief
		void finalize();

	private:
		void _declaration();

		void _directive();

		void _rules();

		GToken& _regex();
	};
} // namespace comp

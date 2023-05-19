#pragma once
#include "utils/templater.hpp"
#include <vector>

namespace comp {

	struct DFA;
	class Lexer;

	class LexCodeGen {
	public:
		LexCodeGen(const Parser& parser, const Lexer& lexer);

		void operator()(const DFA& dfa);

		inline void dump(const fs::path& path) { tmpl.dump(path); }

		inline qy::templater& templater() { return tmpl; }

	private:
		void gen_nxt_table(const DFA& dfa);
		void gen_accept_table(const DFA& dfa);
		void gen_all_table(const DFA& dfa);
		void gen_case();

	private:
		const Lexer& lexer;

		qy::templater tmpl;
	};
} // namespace comp

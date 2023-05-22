#pragma once
#include "fa.hpp"

namespace comp {
	class Lexer {
	public:
		struct Options {
			string outfile;
			string scanner_nfa_dot;
			string scanner_dfa_dot;
			bool compress;
		};

	public:
		Lexer(const Options& options);
		void process(const fs::path& src_path);

	private:
		Options options;
		DFABuilder dfa_builder;

		friend class LexCodeGen;
	};

} // namespace comp
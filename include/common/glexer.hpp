#pragma once
#include "gtoken.hpp"
#include <iostream>
#include <unordered_map>

namespace comp {

	/// @brief Parser for grammar file.
	class GLexer {
	protected:
		std::istream& in;

		int line{0}; // Line number, from 0
		int column{0};
		char peek{}, prev{};

	public:
		GLexer(std::istream& in);

		inline bool eof() const { return in.eof(); }

		GToken scan_noop();

		void scan_skip();

		char get_peek() const;

		/// @brief Get the next char from the stream, and update stats.
		/// @return The peek char.
		char getc();

		/// @brief Put the current char back to the stream
		void unget();

		/// @brief Skip spaces in the stream.
		void skip_ws();

		/// @brief Skip comments in the stream.
		bool skip_comment();

		/// @brief Get a directive token from the stream.
		/// @return A <DIR> token.
		GToken get_directive();

		/// @brief Get a number token from the stream.
		/// @return A <INT> token. (may be <FLOAT> later)
		GToken get_num();

		/// @brief Get an identifier token from the stream.
		/// @return A <ID> token.
		GToken get_id();

		/// @brief Get a tag token from the stream.
		/// @return A <TAG> token.
		GToken get_tag();

		/// @brief Get a char token from the stream.
		/// @return A <CHAR> token.
		GToken get_char();

		/// @brief Get a string token from the stream.
		/// @return A <STRING> token.
		GToken get_string();

		/// @brief Get a operator token from the stream.
		/// @return A <OP> token.
		GToken get_op();

		/// @brief Get a regex token from the stream.
		/// @return A <RE> token.
		GToken get_regex(const std::unordered_map<string, string>& definitions);

		/// @brief Get a codeblock token from the stream.
		/// @return A <ACT> token.
		GToken get_codeblock();

		/// @brief Get a prologue token from the stream.
		/// @return A <PRO>
		GToken get_prologue();

		/// @brief Get a epilogue token from the stream.
		/// @return A <EPI> token.
		GToken get_epilogue();

		void read_line(string& s);

		void read_closure(string& s, char left, char right);

		/// @brief Read the current char into the give string.
		/// @param s String to read to.
		void read_char(string& s);

		/// @brief Read the current string into the give string.
		/// @param s String to read to.
		void read_string(string& s);

		void read_string_unquoted(string& s);

		/// @brief Read the current comment into the give string.
		/// @param s String to read to.
		void read_comment(string& s);

		/// @brief Read the current multi-line comment into the give string.
		/// @param s String to read to.
		void read_comment_ml(string& s);
	};
} // namespace comp

#pragma once
#include "utils/fs.hpp"
#include <fstream>
#include <unordered_map>

namespace comp {

	using std::string;

	template <typename T = std::string>
	using dict = std::unordered_map<std::string, T>;

	class Recognizer {
	public:
		Recognizer() = default;
		Recognizer(const Recognizer& o) = delete;
		Recognizer& operator=(const Recognizer& o) = delete;

	protected:
		struct SourceHandler {
			int section = 0;	// Section number, from 0 to 2
			int lineno = 1;		// Line number, from 1
			bool coded = false; // If wrapped by %{, %}
			string code_content;

			/// @brief Handle code block
			/// @param s Line of input.
			/// @return Whether input is accepted as code.
			bool code(const string& s);
		};
	};
} // namespace comp
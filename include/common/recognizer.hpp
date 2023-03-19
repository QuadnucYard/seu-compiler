#pragma once
#include <filesystem>
#include <unordered_map>
#include <fstream>

namespace fs = std::filesystem;

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
			int section = 0; // Section number, from 0 to 2
			int lineno = 1; // Line number, from 1
			bool coded = false; // If wrapped by %{, %}
			std::ostream& output_file;

			inline SourceHandler(std::ostream& output_file): output_file(output_file) {}

			/// @brief Handle code block
			/// @param s Line of input.
			/// @return Whether input is accepted as code.
			bool code(const std::string& s);
		};
	};
}
#pragma once
#include "common/recognizer.hpp"
#include "fa.hpp"
#include <filesystem>
#include <unordered_map>

namespace comp {

	struct LexRule {};

	class Lexer : public Recognizer {
	private:
		static constexpr char WILDCARD = -1;

	public:
		void process(const fs::path& src_path);

	private:
		std::pair<size_t, std::string> get_re(const std::string& s) const;
		// void parse_source();
		dict<> definitions;
		std::vector<string> actions;
		DFABuilder dfa_builder;

		struct DefHandler;
		struct RuleHandler;

		friend class LexCodeGen;
	};
} // namespace comp
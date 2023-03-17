#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

namespace comp {

	template <typename T = std::string>
	using dict = std::unordered_map<std::string, T>;

	struct LexRule {

	};

	class Lexer {
	private:
		static constexpr char WILDCARD = -1;

	public:
		void process(const fs::path& src_path);

	private:
		std::pair<size_t, std::string> get_re(const std::string& s) const;
		// void parse_source();
		dict<> definitions;
	};
}
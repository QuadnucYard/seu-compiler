#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>

namespace qy {
	class templater {
	public:
		templater(std::string_view string_or_path) {
			if (std::filesystem::exists(string_or_path)) {
				std::filesystem::path path{string_or_path};
				std::ifstream fin(path);
				std::ostringstream iss;
				iss << fin.rdbuf();
				temp = iss.str();
			} else {
				temp = string_or_path;
			}
		}

		void set_string(std::string_view key, std::string_view value) {
			auto p = temp.find(key);
			temp.replace(p, key.length(), value);
		}

		const std::string& str() const { return temp; }

	private:
		std::string temp;
	};
} // namespace qy

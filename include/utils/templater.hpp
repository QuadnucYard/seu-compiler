#pragma once
#include "fs.hpp"
#include <fstream>
#include <sstream>

namespace qy {
	class templater {
	public:
		templater(std::string_view string_or_path) {
			if (fs::exists(string_or_path))
				load(string_or_path);
			else
				temp = string_or_path;
		}

		void set_string(std::string_view key, std::string_view value) {
			auto p = temp.find(key);
			temp.replace(p, key.length(), value);
		}

		const std::string& str() const {
			return temp;
		}

		void load(const fs::path& path) {
			std::ifstream fin(path);
			std::ostringstream iss;
			iss << fin.rdbuf();
			temp = iss.str();
		}

		void dump(const fs::path& path) const {
			std::ofstream out(path);
			out << temp;
		}

	private:
		std::string temp;
	};
} // namespace qy

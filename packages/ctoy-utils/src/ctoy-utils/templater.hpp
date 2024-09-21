#pragma once
#include "fs.hpp"
#include "string_utils.hpp"
#include <fstream>
#include <regex>
#include <sstream>

#if __has_include(<format>)
	#include <format>
	#define FORMAT std::format
#else
	#include <fmt/core.h>
	#define FORMAT fmt::format
#endif

namespace qy {
	class templater {
	public:
		templater(std::string_view string_or_path) {
			if (fs::exists(string_or_path))
				load(string_or_path);
			else
				tmpl = string_or_path;
		}

		void set_string(std::string_view key, std::string_view value) {
			replace_all_inplace(tmpl, key, value);
		}

		void set_string(std::string_view key, const auto&& value) {
			replace_all_inplace(tmpl, key, FORMAT("{}", value));
		}

		void set_bool(std::string_view key, bool value) {
			using namespace std::string_literals;
			if (value) {
				std::regex re{FORMAT(
					R"(\[\[IF\({}\)\]\]([\s\S]*?)\[\[ELSE\]\]([\s\S]*?)\[\[FI\]\])", key)};
				tmpl = std::regex_replace(tmpl, re, "$1");
				std::regex re2{FORMAT(R"(\[\[IF\({}\)\]\]([\s\S]*?)\[\[FI\]\])", key)};
				tmpl = std::regex_replace(tmpl, re2, "$1");
			} else {
				std::regex re{FORMAT(
					R"(\[\[IF\({}\)\]\]([\s\S]*?)\[\[ELSE\]\]([\s\S]*?)\[\[FI\]\])", key)};
				tmpl = std::regex_replace(tmpl, re, "$2");
				std::regex re2{FORMAT(R"(\[\[IF\({}\)\]\]([\s\S]*?)\[\[FI\]\])", key)};
				tmpl = std::regex_replace(tmpl, re2, "");
			}
			return;

			if (value) {
				auto k_if_v = FORMAT("[[IF({})]]", key);
				auto k_if = "[[IF("s;
				auto k_fi = "[[FI]]"s;
				size_t g_scan = 0;
				while (true) {
					auto p = tmpl.find(k_if_v, g_scan);
					if (p == std::string::npos)
						break;
					auto p_scan = p + k_if_v.length();
					size_t p2 = p;
					for (int cnt = 1; cnt > 0;) {
						auto p1 = tmpl.find(k_if, p_scan);
						p2 = tmpl.find(k_fi, p_scan);
						if (p1 != std::string::npos && p1 < p2) {
							cnt++;
							p_scan = p1 + k_if.length();
						} else {
							cnt--;
						}
					}
					tmpl.replace(p, p2 + k_fi.length() + 1, "");
					g_scan = p;
				}
			}
		}

		const std::string& str() const { return tmpl; }

		void load(const fs::path& path) {
			std::ifstream fin(path);
			std::ostringstream iss;
			iss << fin.rdbuf();
			tmpl = iss.str();
		}

		void dump(const fs::path& path) const {
			std::ofstream out(path);
			out << tmpl;
		}

	private:
		std::string tmpl;
	};
} // namespace qy

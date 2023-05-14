#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <regex>

namespace qy::graphviz {
	class digraph {
	private:
		fmt::ostream out;

	public:
		digraph(const std::string& path, std::string_view name) : out{fmt::output_file(path)} {
			out.print("digraph \"{}\" {{\n", name);
		}

		~digraph() {
			out.print("}}\n");
		}

		digraph& rankdir(std::string_view dir) {
			out.print("    rankdir = {};\n", dir);
			return *this;
		}

		digraph& node_option(std::string_view key, std::string_view value) {
			out.print("    node [{} = {};];\n", key, value);
			return *this;
		}

		digraph& node(auto& u) {
			out.print("    {};\n", u);
			return *this;
		}

		digraph& node(auto& u, std::string_view label) {
			out.print("    {} [label = \"{}\"];\n", u, label);
			return *this;
		}

		digraph& edge(auto& u, auto& v, std::string_view label) {
			out.print("    {} -> {} [label = \"{}\"];\n", u, v, label);
			return *this;
		}
	};

	std::string label_escape(const std::string& s) {
		static std::regex pat{R"(([|"{}]))"};
		return std::regex_replace(s, std::regex{R"(([|"{}]))"}, "\\$1");
	}
} // namespace qy::graphviz

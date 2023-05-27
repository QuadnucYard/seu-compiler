#pragma once
#include "utils/string_utils.hpp"
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <initializer_list>
#include <ranges>
#include <regex>

namespace qy::graphviz {
	class digraph {
		using arg_list = std::initializer_list<std::pair<std::string, std::string>>;

	private:
		fmt::ostream out;

	public:
		digraph(const std::string& path, std::string_view name) : out{fmt::output_file(path)} {
			out.print("digraph \"{}\" {{\n", name);
		}

		~digraph() { out.print("}}\n"); }

		digraph& rankdir(std::string_view dir) {
			out.print("    rankdir = {};\n", dir);
			return *this;
		}

		digraph& node_option(std::string_view key, auto&& value) {
			out.print("    node [{} = \"{}\";];\n", key, value);
			return *this;
		}

		digraph& edge_option(std::string_view key, auto&& value) {
			out.print("    edge [{} = \"{}\";];\n", key, value);
			return *this;
		}

		digraph& node(auto& u) {
			out.print("    {};\n", u);
			return *this;
		}

		digraph& node(auto& u, auto&& label) {
			out.print("    {} [label = \"{}\"];\n", u, label);
			return *this;
		}

		digraph& node(auto& u, arg_list args) {
			out.print("    {} [{}];\n", u, join_args(args));
			return *this;
		}

		digraph& edge(auto& u, auto& v) {
			out.print("    {} -> {};\n", u, v);
			return *this;
		}

		digraph& edge(auto& u, auto& v, auto&& label) {
			out.print("    {} -> {} [label = \"{}\"];\n", u, v, label);
			return *this;
		}

		digraph& edge(auto& u, auto& v, auto&& label, arg_list args) {
			out.print("    {} -> {} [label = \"{}\"; {}];\n", u, v, label, join_args(args));
			return *this;
		}

	private:
		auto join_args(const arg_list& args) const {
			return fmt::join(args | std::views::transform([](auto&& p) {
								 return fmt::format("{} = \"{}\"", p.first, p.second);
							 }),
							 "");
		}
	};

	inline std::string label_escape(const std::string& s) {
		static std::regex pat{R"(([|"{}]))"};
		return std::regex_replace(qy::replace_all(s, "\\", "\\\\"), pat, "\\$1");
	}
} // namespace qy::graphviz

namespace qy {
	namespace gv = graphviz;
} // namespace  qy

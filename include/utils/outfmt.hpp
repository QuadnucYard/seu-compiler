
#include <algorithm>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <tl/chunk.hpp>

namespace qy {

	struct format_array_args {
		unsigned line_size{10};
		unsigned field_width{6};
		unsigned indent{4};
		bool with_brace{true};
	};

	template <std::ranges::input_range R>
	std::string format_array(R&& r, format_array_args args = {}) {
		// This format string need to be prolonged
		auto fstr = fmt::format("{}{{:{}}},\n", std::string(args.indent, ' '), args.field_width);
		auto ff = fmt::runtime(fstr);
		std::string s;
		for (auto&& chk : r | tl::views::chunk(args.line_size))
			s += fmt::format(ff, fmt::join(chk, ","));
		return args.with_brace ? fmt::format("{{\n{}}}", s) : s;
	}

	template <std::ranges::input_range R>
	std::string format_array_2d(R&& r, format_array_args args = {}) {
		auto fstr = fmt::format("{}{{:{}}},\n", std::string(args.indent, ' '), args.field_width);
		auto ff = fmt::runtime(fstr);
		std::string s;
		/* auto ff2 = fmt::runtime(
		args.indent > 0 ? 	fmt::format("{{1:{}}}{{{{\n{{2:{}}}{{0}}{{3:{}}}}}}},\n" ,
						args.indent, args.indent, args.indent)
						: fmt::format("{{{{\n{{0}}}}}},\n")
						); */
		// auto ff2 = fmt::runtime(fmt::sprintf("{:%d}{\n{{}}{:%d}},\n", args.indent, args.indent));
		for (auto&& a : r) {
			std::string ss;
			for (auto&& chk : a | tl::views::chunk(args.line_size))
				ss += fmt::format(ff, fmt::join(chk, ","), "");
			// s += fmt::format(ff2, ss, "", "", "");
			s += fmt::format("{{\n{}}},\n", ss);
		}
		return fmt::format("{{\n{}}}", s);
	}
} // namespace qy

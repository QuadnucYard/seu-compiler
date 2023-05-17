
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
	};

	template <std::ranges::input_range R>
	std::string format_array(R&& r, format_array_args args = {}) {
		std::string s;
		auto ff = fmt::runtime(
			args.indent > 0 ? fmt::format("{{1:{}}}{{0:{}}},\n", args.indent, args.field_width)
							: fmt::format("{{0:{}}},\n", args.field_width));
		for (auto&& chk : r | tl::views::chunk(args.line_size))
			s += fmt::format(ff, fmt::join(chk, ","), "");
		return fmt::format("{{\n{}}}", s);
	}

	template <std::ranges::input_range R>
	std::string format_array_2d(R&& r, format_array_args args = {}) {
		std::string s;
		auto ff = fmt::runtime(
			args.indent > 0 ? fmt::format("{{1:{}}}{{0:{}}},\n", args.indent, args.field_width)
							: fmt::format("{{0:{}}},\n", args.field_width));
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

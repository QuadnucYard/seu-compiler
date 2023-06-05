#pragma once
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
		std::string s;
		for (auto&& chk : r | tl::views::chunk(args.line_size))
			s += fmt::format("{:<{}}{:>{}},\n", "", args.indent, fmt::join(chk, ","),
							 args.field_width);
		return args.with_brace ? fmt::format("{{\n{}}}", s) : s;
	}

	template <std::ranges::input_range R>
	std::string format_array_2d(R&& r, format_array_args args = {}) {
		std::string s;
		for (auto&& a : r) {
			std::string ss;
			for (auto&& chk : a | tl::views::chunk(args.line_size))
				ss += fmt::format("{:<{}}{:>{}},\n", "", args.indent, fmt::join(chk, ","),
								  args.field_width);
			s += fmt::format("{:<{}}{{\n{}{:<{}}}},\n", "", args.indent, ss, "", args.indent);
		}
		return fmt::format("{{\n{}}}", s);
	}
} // namespace qy

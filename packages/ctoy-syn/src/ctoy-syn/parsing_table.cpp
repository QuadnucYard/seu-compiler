#include "yacc/parsing_table.hpp"
#include "utils/stopwatch.hpp"
#include <algorithm>
#include <cassert>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <map>
#include <optional>
#include <tl/enumerate.hpp>
#include <tl/to.hpp>

namespace comp {

	void parsing_table::to_csv(const fs::path& path) const {
		auto out{fmt::output_file(path.string())};
		for (size_t i = 0; i < action.rows(); i++) {
			out.print("{},{}\n", fmt::join(action.iter_row(i), ","),
					  fmt::join(goto_.iter_row(i), ","));
		}
	}

	bool is_compat(sid_t x, sid_t y) {
		return x == parsing_table::ERR || y == parsing_table::ERR || x == y;
	}

	std::optional<sid_t> is_only(auto&& slice) {
		sid_t only = parsing_table::ERR;
		for (auto x : slice)
			if (x != parsing_table::ERR) {
				if (only == parsing_table::ERR)
					only = x;
				else if (x != only)
					return {};
			}
		return only;
	}

	std::tuple<bool, size_t, sid_t> parsing_table_compressed::find_embed(auto&& slice, size_t l,
																		 size_t base) const {
		auto const& tab = table;

		// 如果有效值唯一，直接过。这种情况可以扔到一个绝对不会冲突的地方
		if (auto r = is_only(slice))
			return {true, 0, *r};

		auto check_pass =
			[&slice, l, base, &tab,
			 this](bool allow_overflow) -> std::optional<std::tuple<bool, size_t, sid_t>> {
			size_t best = tab.size() + 1; // 最优的嵌入位置
			size_t least_conf = l + 1;	  // 最少冲突数
			sid_t only = ERR;			  // 唯一的冲突值
			for (size_t i = 0; i <= tab.size(); i++) {
				size_t conf = 0;
				sid_t cur_only = ERR;
				bool not_only = false; // 是否冲突的不唯一
				// 如果冲突的是唯一的，那么取冲突最少的
				bool strong_conflict = false;
				for (auto&& [j, x] : tl::views::enumerate(slice)) {
					if (i + j < tab.size() &&
						(x != ERR && guard[i + j].contains(base + j) &&
							 guard[i + j].at(base + j) != x ||
						 check[i + j] == base + j && !is_compat(tab[i + j], x))) {
						strong_conflict = true;
						break;
					}
					if (allow_overflow ? i + j < tab.size() && !is_compat(tab[i + j], x)
									   : i + j >= tab.size() || !is_compat(tab[i + j], x)) {
						conf++;
						if (cur_only == ERR)
							cur_only = x;
						else if (x != ERR && x != cur_only && cur_only != ERR)
							not_only = true;
					}
				}
				if (strong_conflict)
					continue;
				if (conf == 0) // 没有冲突的
					return std::tuple{false, i, ERR};
				if (!not_only && (only == ERR || conf > least_conf)) {
					best = i;
					only = cur_only;
					least_conf = conf;
				}
			}
			if (least_conf != l + 1)
				return std::tuple{false, best, only};
			return {};
		};

		// [i, i + l)
		if (auto res = check_pass(false))
			return *res;

		// 然后是扩张式的嵌入
		auto res = check_pass(true);
		assert(res);

		return *res;
	}

	std::tuple<bool, size_t, sid_t> parsing_table_compressed::embed(auto&& slice, size_t l,
																	size_t base) {
		auto& tab = table;
		auto [b, i, only] = find_embed(slice, l, base);
		// 如果不需要嵌入就返回
		if (b)
			return {b, i, only};

		if (i + l > tab.size()) {
			tab.resize(i + l, ERR);
			check.resize(i + l, -1);
			guard.resize(i + l);
		}
		for (auto&& [j, x] : tl::views::enumerate(slice))
			if (x != ERR) {
				if (x != only) {
					assert(tab[i + j] == ERR || tab[i + j] == x);
					tab[i + j] = x;
					check[i + j] = static_cast<sid_t>(base + j);
				} else if (only != ERR) {
					assert(!guard[i + j].contains(base + j) || guard[i + j].at(base + j) == only);
					guard[i + j][base + j] = only;
				}
			}
		return {b, i, only};
	}

	parsing_table_compressed parsing_table::compress() const {
		qy::stopwatch sw;

		parsing_table_compressed pt;
		pt.pact.resize(action.rows());
		pt.defact.resize(action.rows());
		pt.pgoto.resize(goto_.cols(), 0);
		pt.defgoto.resize(goto_.cols(), -1);

		auto& tab = pt.table;
		static constexpr sid_t DUMMY_P = -100000;

		auto check_act = [n_col = action.cols(), &pt, this](size_t n_row) {
			for (int i = 0; i < n_row; i++) {
				for (int j = 0; j < n_col; j++) {
					if (auto x = action[i][j]; x != ERR) {
						auto yyn = pt.pact[i] + j;
						yyn = yyn >= 0 && yyn < pt.check.size() && pt.check[yyn] == j
								  ? pt.table[yyn]
								  : pt.defact[i];
						if (yyn != x) {
							fmt::print("fail! act[{},{}], expect {}, but get {}\n", i, j, x, yyn);
						}
					}
				}
			}
		};
		auto check_goto = [n_row = goto_.rows(), &pt, this](size_t n_col) {
			for (int i = 0; i < n_row; i++) {
				for (int j = 0; j < n_col; j++) {
					if (auto x = goto_[i][j]; x != ERR) {
						auto yyn = pt.pgoto[j] + i;
						yyn = yyn >= 0 && yyn < pt.check.size() && pt.check[yyn] == i
								  ? pt.table[yyn]
								  : pt.defgoto[j];
						if (yyn != x) {
							fmt::print("fail! goto[{},{}], expect {}, but get {}\n", i, j, x, yyn);
						}
					}
				}
			}
		};

		sw.record("init");

		size_t n_row = action.rows();
		size_t n_col = action.cols();
		for (size_t i = 0; i < action.rows(); i++) {
			size_t row_left = 0, row_right = n_col;
			while (row_left < n_col && action[i][row_left] == ERR)
				++row_left;
			if (row_left == n_col)
				continue;
			while (action[i][row_right - 1] == ERR)
				--row_right;

			size_t row_len = row_right - row_left;
			auto slice = std::span{&action[i][row_left], row_len};

			auto [b, best, only] = pt.embed(slice, row_len, row_left);
			pt.pact[i] = b ? DUMMY_P : static_cast<sid_t>(best - row_left);
			pt.defact[i] = only;
			// check_act(i);
		}

		sw.record("action");

		// 然后压goto
		for (size_t k = 0; k < goto_.cols(); k++) {
			// 找边界
			size_t col_top{0}, col_btm{n_row};
			while (col_top < n_row && goto_[col_top][k] == ERR)
				col_top++;
			if (col_top == n_row)
				continue;
			while (goto_[col_btm - 1][k] == ERR)
				col_btm--;

			size_t col_len = col_btm - col_top;
			auto&& slice = goto_.iter_col(k) | std::views::drop(col_top) | std::views::take(col_len) |
						   tl::to<std::vector>();

			auto [b, best, only] = pt.embed(slice, col_len, col_top);
			pt.pgoto[k] = b ? DUMMY_P : static_cast<sid_t>(best - col_top);
			pt.defgoto[k] = only;
		}

		sw.record("goto");

		size_t new_dummy = tab.size();
		for (auto& p : pt.pact)
			if (p == DUMMY_P)
				p = new_dummy;
		for (auto& p : pt.pgoto)
			if (p == DUMMY_P)
				p = new_dummy;

		sw.record("dummy");

		check_act(n_row);
		check_goto(goto_.cols());

		sw.record("check");

		fmt::print("After compression: {}\n", tab.size());

		sw.print("Compress");

		return pt;
	}
} // namespace comp

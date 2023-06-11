#include "yacc/parsing_table.hpp"
#include "utils/stopwatch.hpp"
#include <algorithm>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <map>
#include <tl/enumerate.hpp>
#include <unordered_set>

namespace comp {

	using ssize_t = std::int64_t;

	void parsing_table::to_csv(const fs::path& path) const {
		auto out{fmt::output_file(path.string())};
		for (size_t i = 0; i < action.rows(); i++) {
			out.print("{},{}\n", fmt::join(action.iter_row(i), ","),
					  fmt::join(goto_.iter_row(i), ","));
		}
	}

	std::tuple<bool, ssize_t, sid_t> find_embed(const symbol_vec& tab, auto&& slice, size_t l) {
		constexpr static auto ERR = parsing_table::ERR;
		ssize_t best = tab.size() + 1; // 最优的嵌入位置
		size_t least_conf = l + 1;	   // 最少冲突数
		sid_t only = ERR;			   // 唯一的冲突值

		std::unordered_set<sid_t> slice_set;
		for (auto x : slice)
			if (x != ERR)
				slice_set.insert(x); // unordered_set 不支持range初始化和(I, S)初始化

		// 如果有效值唯一，直接过
		if (slice_set.size() == 1)
			return {true, 0, *slice_set.begin()};

		auto is_compat = [](sid_t x, sid_t y) {
			return x == parsing_table::ERR || y == parsing_table::ERR || x == y;
		};
		// 先尝试不扩容的嵌入  i可以从负开始吗？可能会导致一个问题：空太少了
		// 貌似找到问题了  应该是找因某个引起的冲突的计数  最好情况：只有一种冲突
		// 找一个位置使得冲突数最多的最少
		// 越界处理有点麻烦？
		// [i, i + l)
		for (ssize_t i = 0; i <= tab.size(); i++) {
			size_t conf = 0;
			sid_t cur_only = ERR;
			bool not_only = false; // 是否冲突的不唯一
			// std::unordered_map<sid_t, sid_t> conf_by_sym;
			// 如果冲突的是唯一的，那么取冲突最少的
			for (auto&& [j, x] : tl::views::enumerate(slice)) {
				if (i + j < 0 || i + j >= tab.size() || !is_compat(tab[i + j], x)) {
					conf++;
					if (cur_only == ERR)
						cur_only = x;
					else if (x != ERR && x != cur_only && cur_only != ERR)
						not_only = true;
				}
			}
			if (conf == 0) // 没有冲突的
				return {false, i, ERR};
			if (!not_only && (only == ERR || conf > least_conf)) {
				best = i;
				only = cur_only;
				least_conf = conf;
			}
		}
		if (least_conf != l + 1)
			return {false, best, only};
		only = ERR;
		least_conf = l + 1;
		// 然后是扩张式的嵌入
		for (size_t i = tab.size() < l ? 0 : tab.size() - l; i <= tab.size(); i++) {
			size_t conf = 0;
			sid_t cur_only = ERR;
			bool not_only = false; // 是否冲突的不唯一
			for (auto&& [j, x] : tl::views::enumerate(slice)) {
				if (i + j >= 0 && i + j < tab.size() && !is_compat(tab[i + j], x)) {
					// 这里已经满足 x != ERR 了
					conf++;
					if (cur_only == ERR)
						cur_only = x;
					else if (x != cur_only)
						not_only = true;
				}
			}
			if (conf == 0) // 没有冲突的
				return {false, i, ERR};
			if (!not_only && (only == ERR || conf > least_conf)) {
				best = i;
				only = cur_only;
				least_conf = conf;
			}
		}
		return {false, best, only};
	}

	std::tuple<bool, ssize_t, sid_t> embed(symbol_vec& tab, symbol_vec& check, auto&& slice,
										   size_t l, size_t base) {
		constexpr static auto ERR = parsing_table::ERR;
		auto [b, i, only] = find_embed(tab, slice, l);
		// 如果不需要嵌入就返回
		if (b)
			return {b, i, only};
		if (i + l > tab.size()) {
			tab.resize(i + l, ERR);
			check.resize(tab.size(), -1);
		}
		for (auto&& [j, x] : tl::views::enumerate(slice))
			if (x != ERR && x != only) {
				tab[i + j] = x;
				check[i + j] = static_cast<sid_t>(base + j);
			}
		return {b, i, only};
	}

	parsing_table_compressed parsing_table::compress() const {
		qy::stopwatch sw;

		struct table_row {
			size_t r, c, l;
			const sid_t* a;
		};

		parsing_table_compressed pt;
		pt.pact.resize(action.rows());
		pt.defact.resize(action.rows());
		pt.pgoto.resize(goto_.cols(), 0);
		pt.defgoto.resize(goto_.cols(), -1);

		sw.record();

		size_t n_row = action.rows();
		size_t n_col = action.cols();
		std::vector<table_row> rows;
		for (size_t i = 0; i < action.rows(); i++) {
			table_row row;
			row.r = i;
			size_t j = 0;
			while (j < n_col && action[i][j] == ERR)
				j++;
			if (j == n_col)
				continue;
			row.c = j;
			std::unordered_set<sid_t> cnt;
			for (; j < n_col; j++)
				if (action[i][j] != ERR) {
					row.l = j - row.c + 1;
					cnt.insert(action[i][j]);
				}
			row.a = &action[i][row.c];
			// if (cnt.size() > 1)
			rows.push_back(row);
			// BUG 这里如果不筛选，结果会不一样
			pt.defact[i] = cnt.size() > 1 ? 0 : *cnt.begin();
		}

		// 下面考虑贪心嵌入
		sw.record();

		auto& tab = pt.table;

		// 先看 pact，如果不对应那么就找defact

		for (auto& row : rows) {
			// TODO 找一下最优的位置
			auto [b, i, only] = embed(tab, pt.check, std::span{row.a, row.l}, row.l, row.c);

			pt.pact[row.r] = b ? -10000 : static_cast<sid_t>(i - row.c);
			pt.defact[row.r] = only;
		}
		// 大概找到问题了。第0行嵌入，全部走def，没有修改check。但是后来嵌入的时候把check改成了其他的。
		// 需要解决一个标识问题：对于走def的情况，如果check一致，那么table的这个位置填数的必须正确。
		for (int i = 0; i < n_row; i++) {
			for (int j = 0; j < n_col; j++) {
				if (auto x = action[i][j]; x != ERR) {
					auto yyn = pt.pact[i] + j;
					yyn = yyn >= 0 && yyn < pt.check.size() && pt.check[yyn] == j ? pt.table[yyn]
																				  : pt.defact[i];
					if (yyn != x) {
						fmt::print("fail! act {},{},{}\n", i, j, x);
					}
				}
			}
		}

		sw.record();
		// 然后压goto
		// pgoto 存首地址，defgoto 存指针
		for (size_t k = 0; k < goto_.cols(); k++) {
			// 找边界
			size_t u{0}, d{n_row}, l{0};
			while (u < n_row && goto_[u][k] == ERR)
				u++;
			if (u == n_row)
				continue;
			while (goto_[d - 1][k] == ERR)
				d--;
			l = d - u;
			// [u, u + l] 嵌入
			auto&& slice = goto_.iter_col(k) | std::views::drop(u) | std::views::take(l);
			auto [b, best, only] = embed(tab, pt.check, slice, l, u);
			pt.pgoto[k] = b ? -10000 : static_cast<sid_t>(best - u);
			pt.defgoto[k] = only;
			// 留给table里冲突的那个
			// 有个问题，如果是最后一个因为溢出而冲突，应该做裁剪
		}
		for (int i = 0; i < n_row; i++) {
			for (int j = 0; j < goto_.cols(); j++) {
				if (auto x = goto_[i][j]; x != ERR) {
					auto yyn = pt.pgoto[j] + i;
					yyn = yyn >= 0 && yyn < pt.check.size() && pt.check[yyn] == i ? pt.table[yyn]
																				  : pt.defgoto[j];
					if (yyn != x) {
						fmt::print("fail! goto {},{},{}\n", i, j, x);
					}
				}
			}
		}

		// 如果这列只有一个，放在defgoto
		// 否则把这列嵌入到table里，挑一个放到defgoto
		sw.record();
		sw.print("Compress");

		return pt;
	}
} // namespace comp

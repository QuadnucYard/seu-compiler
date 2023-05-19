#include "yacc/parsing_table.hpp"
#include <algorithm>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <map>
#include <unordered_set>

namespace comp {

	void parsing_table::to_csv(const fs::path& path) const {
		auto out{fmt::output_file(path.string())};
		for (size_t i = 0; i < action.rows(); i++) {
			out.print("{},{}\n", fmt::join(action.iter_row(i), ","),
					  fmt::join(goto_.iter_row(i), ","));
		}
	}

	parsing_table_compressed parsing_table::compress_less() const {
		struct table_row {
			size_t r, c, l;
		};

		size_t n_col = action.cols();
		std::vector<table_row> rows;
		for (size_t i = 0; i < action.rows(); i++) {
			table_row row{i, 0, 0};
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
			if (cnt.size() > 1)
				rows.push_back(row);
		}
		std::sort(rows.begin(), rows.end(), [](const table_row& r1, const table_row& r2) {
			return r1.l > r2.l || r1.l == r2.l && r1.r < r2.r;
		}); // 排序稍微有点用

		// 下面考虑一种简单的压缩，不涉及行的交错

		std::vector<sid_t> tab;
		// std::priority_queue<std::pair<size_t, size_t>> gaps; // len, offset
		std::multimap<size_t, size_t> gaps;
		for (auto& row : rows) {
			size_t ins = tab.size();
			if (auto it = gaps.lower_bound(row.l); it != gaps.end()) { // 有空位放
				ptrdiff_t l = it->first - row.l;
				ins = it->second;
				gaps.erase(it);
				// 如果有多余空间就把多出的 gap 插回去
				if (l > 0)
					gaps.emplace(l, ins + row.l);
			}
			if (ins == tab.size()) {
				tab.insert(tab.end(), action.iter_row(row.r).begin() + row.c,
						   action.iter_row(row.r).begin() + row.c + row.l);
			} else {
				std::copy_n(action.iter_row(row.r).begin() + row.c, row.l, tab.begin() + ins);
			}
			for (size_t j = row.c; j < row.c + row.l;) {
				size_t k = j;
				while (k < n_col && action[row.r][k] != ERR)
					k++;
				if (k == n_col)
					break;
				// 现在k指向的是ERR
				j = k;
				while (j < n_col && action[row.r][j] == ERR)
					j++;
				// 现在j指向的是非ERR
				gaps.emplace(j - k, ins + k);
			}
		}

		return {};
	}

	parsing_table_compressed parsing_table::compress() const {
		struct table_row {
			size_t r, c, l;
			const sid_t* a;
		};

		parsing_table_compressed pt;
		pt.pact.resize(action.rows());
		pt.defact.resize(action.rows());
		pt.pgoto.resize(goto_.cols(), 0);
		pt.defgoto.resize(goto_.cols(), -1);

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
			if (cnt.size() > 1)
				rows.push_back(row);
			pt.defact[i] = cnt.size() > 1 ? 0 : *cnt.begin();
		}

		// 下面考虑贪心嵌入

		auto& tab = pt.table;

		for (auto& row : rows) {
			for (size_t i = 0; i <= tab.size(); i++) { // 开头
				bool flag = true;
				for (size_t j = 0; j < row.l; j++) {
					if (i + j < tab.size() &&
						(tab[i + j] != ERR && row.a[j] != ERR && tab[i + j] != row.a[j])) {
						flag = false;
						break;
					}
				}
				if (flag) {
					// [i, i + l]
					if (i + row.l > tab.size()) {
						tab.resize(i + row.l, ERR);
						pt.check.resize(tab.size(), ERR);
					}
					for (size_t j = 0; j < row.l; j++)
						if (row.a[j] != ERR) {
							tab[i + j] = row.a[j];
							pt.check[i + j] = row.c + j;
						}
					pt.pact[row.r] = i - row.c;
					break;
				}
			}
		}

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
			size_t best = tab.size(), least_conf = l, best_last_conf = 0;
			// 先尝试不扩容的嵌入
			for (size_t i = 0; i <= tab.size(); i++) {
				size_t conf = 0, last_conf = -1;
				// 其实可以考虑从负数开始循环
				for (size_t j = 0; j < l; j++) {
					if (i + j >= tab.size() || tab[i + j] != ERR && goto_[u + j][k] != ERR &&
												   tab[i + j] != goto_[u + j][k]) {
						conf++;
						last_conf = j;
					}
				}
				if (conf < least_conf) {
					best = i;
					least_conf = conf;
					best_last_conf = last_conf;
				}
			}
			if (least_conf > 1) {
				// 如果不行，那么就扩容
				best = tab.size();
				for (size_t i = tab.size() < l ? 0 : tab.size() - l; i <= tab.size(); i++) {
					size_t conf = 0, last_conf = -1;
					for (size_t j = 0; j < l; j++) {
						if (tab[i + j] != ERR && goto_[u + j][k] != ERR &&
							tab[i + j] != goto_[u + j][k]) {
							conf++;
							last_conf = j;
						}
					}
					if (conf <= 1) {
						best = i;
						least_conf = conf;
						best_last_conf = last_conf;
						break;
					}
				}
			}
			if (best + l > tab.size()) {
				tab.resize(best + l, ERR);
				pt.check.resize(tab.size(), ERR);
			}
			for (size_t j = 0; j < l; j++)
				if (tab[best + j] == ERR) {
					tab[best + j] = goto_[u + j][k];
					pt.check[best + j] = u + j;
				}
			// goto[j][k] = table[pgoto[k] + j], u + pgoto[k] = best
			pt.pgoto[k] = best - u;
			pt.defgoto[k] = goto_[u + best_last_conf][k]; // 留给table里冲突的那个
			// 有个问题，如果是最后一个因为溢出而冲突，应该做裁剪
		}
		// 如果这列只有一个，放在defgoto
		// 否则把这列嵌入到table里，挑一个放到defgoto
		return pt;
	}
} // namespace comp

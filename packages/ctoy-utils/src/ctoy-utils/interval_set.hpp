#include <concepts>
#include <set>

namespace qy {
	/// @brief 维护区间，需要保证相交部分都为单独段
	/// @tparam T
	template <std::integral T>
	class interval_set {
		struct interval {
			T a, b;

			bool operator<(interval const& p1, interval const& p2) const noexcept {
				return p1.b < p2.b;
			}
		};

		using point_type = T;
		using value_type = std::pair<point_type, point_type>;

		struct cmp_by_second {
			bool operator()(value_type const& p1, value_type const& p2) const noexcept {
				return p1.second < p2.second;
			}
		};

	public:
		void insert(point_type x) {
			auto it = m_set.lower_bound({{}, x});
			if (it != m_set.end() && it->first <= x) {
				if (x > it->first)
					m_set.emplace(it->first, x - 1);
				auto node = m_set.extract(it);
				node.value().first = x + 1;
				m_set.insert(std::move(node));
			}
			m_set.emplace(x, x);
		}

		void insert(point_type x1, point_type x2) {
			// m_set.emplace(x1, x2);
			auto it1 = m_set.lower_bound({{}, x1});
			auto it2 = m_set.lower_bound({{}, x2});
			if (it1 == m_set.end()) {
				m_set.emplace(x1, x2);
				return;
			}
			for (; it1 != it2;) {
				auto it3 = it1;
				++it3;
				if (it2->first - it1->second >= 2) {
					m_set.emplace(it1->second + 1, it2->first - 1);
				}
				it1 = it3;
			}
			if (it1 != it2) {
				if (x1 > it1->first) {
					m_set.emplace(it1->first, x1 - 1);
					m_set.emplace(x1, it1->second);
					m_set.erase(it1);
				} else {
					m_set.emplace(x1, it1->first - 1);
				}
				if (x2 < it2->first) {
				} else {
				}
			}
		}

	private:
		std::set<value_type, cmp_by_second> m_set;
	};
} // namespace qy

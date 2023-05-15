#include <ranges>
#include <vector>

namespace qy {

	template <class T>
	class matrix {
	public:
		using value_type = T;
		using size_type = size_t;

		matrix(size_type rows, size_type cols) : m_rows{rows}, m_cols{cols}, m_data(rows * cols) {}

		matrix(size_type rows, size_type cols, const value_type& value) :
			m_rows{rows}, m_cols{cols}, m_data(rows * cols, value) {}

		size_type rows() const { return m_rows; }

		size_type cols() const { return m_cols; }

		const value_type* data() const { return m_data.data(); }

		value_type* data() { return m_data.data(); }

		const value_type* operator[](size_type r) const { return data() + r * m_cols; }

		value_type* operator[](size_type r) { return data() + r * m_cols; }

		void remove_row(size_type r) {
			m_data.erase(m_data.begin() + r * m_cols, m_data.begin() + (r + 1) * m_cols);
			m_rows--;
		}

		auto iter_row(size_type r) const {
			return std::ranges::subrange(m_data.begin() + r * m_cols,
										 m_data.begin() + (r + 1) * m_cols);
		}

		auto iter_row(size_type r) {
			return std::ranges::subrange(m_data.begin() + r * m_cols,
										 m_data.begin() + (r + 1) * m_cols);
		}

	private:
		size_type m_rows;
		size_type m_cols;
		std::vector<value_type> m_data;
	};
} // namespace qy

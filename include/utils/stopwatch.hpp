#pragma once
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <vector>

namespace qy {
	class stopwatch {
	public:
		stopwatch() { m_time = std::chrono::high_resolution_clock::now(); }

		void record() {
			auto now = std::chrono::high_resolution_clock::now();
			m_periods.push_back(now - m_time);
			m_time = now;
		}

		void print(std::string_view prefix = "Stopwatch") {
			fmt::print("{}: {:%S}\n", prefix, fmt::join(m_periods, " "));
		}

	private:
		std::chrono::steady_clock::time_point m_time;
		std::vector<std::chrono::nanoseconds> m_periods;
	};
} // namespace qy

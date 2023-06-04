#pragma once
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <vector>

namespace qy {
	class stopwatch {
		using clock_t = std::chrono::high_resolution_clock;
		using time_t = std::chrono::steady_clock::time_point;
		using timespan_t = std::chrono::nanoseconds;

		struct record_t {
			timespan_t time;
			timespan_t elapsed;
			std::string desc;
		};

	public:
		stopwatch() { m_start = m_time = clock_t::now(); }

		void record(std::string desc = "") {
			auto now = clock_t::now();
			m_periods.emplace_back(now - m_start, now - m_time, desc);
			m_time = now;
		}

		void print(std::string_view prefix = "Stopwatch") {
			fmt::print(">> {}\n", prefix);
			for (auto& p : m_periods)
				fmt::print("{:%S} {:%S} {}\n", p.time, p.elapsed, p.desc);
		}

	private:
		time_t m_start;
		time_t m_time;
		std::vector<record_t> m_periods;
	};
} // namespace qy

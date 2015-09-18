#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

#include <chrono>

class stopwatch {
public:
	stopwatch() {
		begin = end = std::chrono::high_resolution_clock::time_point::min();
	}
	inline void start(void) {
		 begin = std::chrono::high_resolution_clock::now();
	}
	inline void stop(void) {
		end = std::chrono::high_resolution_clock::now();
	}

	template <typename count_type = float, typename period = std::ratio<1,1000>>
	inline count_type count(void) {
		using namespace std::chrono;
		return duration_cast<duration<count_type,period>>(end - begin).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> begin;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

#endif

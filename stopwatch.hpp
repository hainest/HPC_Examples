#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

#include <chrono>

class stopwatch {
public:
	stopwatch() {
		begin = end = std::chrono::steady_clock::time_point::min();
	}
	inline void start(void) {
		 begin = std::chrono::steady_clock::now();
	}
	inline void stop(void) {
		end = std::chrono::steady_clock::now();
	}

	using count_type = std::chrono::duration<float,std::ratio<1,1000>>;
	template <typename T = count_type>
	inline typename T::rep count(void) {
		return std::chrono::duration_cast<T>(end - begin).count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> begin;
	std::chrono::time_point<std::chrono::steady_clock> end;
};

#endif

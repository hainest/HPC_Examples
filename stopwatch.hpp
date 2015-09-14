#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_

#include <chrono>

class stopwatch {
	using namespace std::chrono;
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

	using count_type = std::chrono::duration<float,std::ratio<1,1000>>;
	template <typename T = count_type>
	inline typename T::rep count(void) {
		return std::chrono::duration_cast<T>(end - begin).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> begin;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

#endif

#include "rand_fill.hpp"

#include <iterator>
#include <random>

void rand_fill(std::vector<float, aligned_allocator<float, 32>>& v) {
	std::mt19937 engine;
	auto b = std::begin(v);
	auto e = std::end(v);
	std::normal_distribution<float> nd{};

	while(b != e) {
		*b = nd(engine);
		b = std::next(b);
	}
}

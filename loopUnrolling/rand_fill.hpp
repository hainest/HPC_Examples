#ifndef RAND_FILL_HPP_
#define RAND_FILL_HPP_

#include <vector>
#include "aligned_allocator.hpp"

void rand_fill(std::vector<float, aligned_allocator<float, 32>>& v);

#endif

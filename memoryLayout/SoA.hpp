#ifndef SOA_HPP_
#define SOA_HPP_

#include <vector>
#include "stopwatch.hpp"
#include "aligned_allocator.hpp"
#include <immintrin.h>

template<typename T>
struct SoA {
	SoA(size_t size) {
		x.reserve(size);
		y.reserve(size);
		z.reserve(size);
	}
	void clear(void) noexcept {
		x.clear();
		y.clear();
		z.clear();
	}
	std::vector<T> x, y, z;
};

template<typename T>
struct SoA_aligned {
	SoA_aligned(size_t size) {
		x.reserve(size);
		y.reserve(size);
		z.reserve(size);
	}
	void clear(void) noexcept {
		x.clear();
		y.clear();
		z.clear();
	}
	std::vector<T, aligned_allocator<T>> x, y, z;
};

template<typename dataType, typename collectionType>
void doSoA(const size_t size, collectionType &timings) {
	SoA<dataType> str(size);

	for (size_t i = 0; i < size; i++) {
		str.x[i] = i * 2.0f;
		str.y[i] = i * 3.0f;
		str.z[i] = i * 4.0f;
	}

	stopwatch timer;
	dataType sum { };

	sum = 0.0f;
	timer.start();
	for (unsigned i = 0; i < size; i++) {
		sum += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
	}
	timer.stop();
	timings.emplace_back("SoA", sum, timer.count());

	sum = 0.0f;
	timer.start();
	for (unsigned i = 0; i < size; i += 2) {
		sum += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
		sum += str.x[i + 1] * str.x[i + 1] + str.y[i + 1] * str.y[i + 1] + str.z[i + 1] * str.z[i + 1];
	}
	timer.stop();
	timings.emplace_back("SoA unroll(2)", sum, timer.count());

	sum = 0.0f;
	timer.start();
	for (unsigned i = 0; i < size; i += 4) {
		sum += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
		sum += str.x[i + 1] * str.x[i + 1] + str.y[i + 1] * str.y[i + 1] + str.z[i + 1] * str.z[i + 1];
		sum += str.x[i + 2] * str.x[i + 2] + str.y[i + 2] * str.y[i + 2] + str.z[i + 2] * str.z[i + 2];
		sum += str.x[i + 3] * str.x[i + 3] + str.y[i + 3] * str.y[i + 3] + str.z[i + 3] * str.z[i + 3];
	}
	timer.stop();
	timings.emplace_back("SoA unroll(4)", sum, timer.count());

	sum = 0.0f;
	dataType sum1 { }, sum2 { };
	timer.start();
	for (unsigned i = 0; i < size; i += 4) {
		sum1 += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
		sum1 += str.x[i + 1] * str.x[i + 1] + str.y[i + 1] * str.y[i + 1] + str.z[i + 1] * str.z[i + 1];
		sum2 += str.x[i + 2] * str.x[i + 2] + str.y[i + 2] * str.y[i + 2] + str.z[i + 2] * str.z[i + 2];
		sum2 += str.x[i + 3] * str.x[i + 3] + str.y[i + 3] * str.y[i + 3] + str.z[i + 3] * str.z[i + 3];
	}
	sum = sum1 + sum2;
	timer.stop();
	timings.emplace_back("SoA unroll(4+2)", sum, timer.count());

	sum = 0.0f;
	register dataType sumr1 { }, sumr2 { };
	timer.start();
	for (unsigned i = 0; i < size; i += 4) {
		sumr1 += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
		sumr1 += str.x[i + 1] * str.x[i + 1] + str.y[i + 1] * str.y[i + 1] + str.z[i + 1] * str.z[i + 1];
		sumr2 += str.x[i + 2] * str.x[i + 2] + str.y[i + 2] * str.y[i + 2] + str.z[i + 2] * str.z[i + 2];
		sumr2 += str.x[i + 3] * str.x[i + 3] + str.y[i + 3] * str.y[i + 3] + str.z[i + 3] * str.z[i + 3];
	}
	sum = sumr1 + sumr2;
	timer.stop();
	timings.emplace_back("SoA register unroll(4+2)", sum, timer.count());

	sum = 0.0f;
	timer.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < size; i++) {
		sum += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
	}
	timer.stop();
	timings.emplace_back("SoA OpenMP", sum, timer.count());

	sum = 0.0f;
	timer.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < size; i += 2) {
		sum += str.x[i] * str.x[i] + str.y[i] * str.y[i] + str.z[i] * str.z[i];
		sum += str.x[i + 1] * str.x[i + 1] + str.y[i + 1] * str.y[i + 1] + str.z[i + 1] * str.z[i + 1];
	}
	timer.stop();
	timings.emplace_back("SoA OpenMP+unroll(2)", sum, timer.count());

	sum = 0.0;
	timer.start();
	__m128 mmRes = _mm_setzero_ps();
	dataType *x = str.x.data();
	dataType *y = str.y.data();
	dataType *z = str.z.data();
	for (unsigned i = 0; i < size; i += 4) {
		auto a = _mm_loadu_ps(x + i);
		auto b = _mm_loadu_ps(y + i);
		auto c = _mm_loadu_ps(z + i);
		mmRes = _mm_add_ps(mmRes, _mm_mul_ps(a, a));
		mmRes = _mm_add_ps(mmRes, _mm_mul_ps(b, b));
		mmRes = _mm_add_ps(mmRes, _mm_mul_ps(c, c));
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	timer.stop();
	timings.emplace_back("SoA SSE3", sum, timer.count());

	SoA_aligned<dataType> str_a(2);
	str_a.x.push_back(2.0);
}

#endif

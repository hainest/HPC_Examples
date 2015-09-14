#ifndef AOS_HPP_
#define AOS_HPP_

#include <vector>
#include "stopwatch.hpp"
#include <immintrin.h>

template<typename T>
struct AoS {
	T x, y, z;
//	char a[64-3*sizeof(T)];
}__attribute__((packed));

template<typename dataType, typename collectionType>
dataType doAoS(const size_t size, collectionType &timings) {
	std::vector<AoS<dataType>> f(size);

	for (size_t i = 0; i < size; i++) {
		f[i].x = i * 2.0f;
		f[i].y = i * 3.0f;
		f[i].z = i * 4.0f;
	}

	stopwatch timer;
	dataType sum { };

	timer.start();
	for (auto &v : f) {
		sum += v.x * v.x + v.y * v.y + v.z * v.z;
	}
	timer.stop();
	auto baseSum = sum;
	timings.emplace_back("AoS (plain)", sum, timer.count());

	sum = 0.0;
	timer.start();
	for (unsigned i = 0; i < size; i += 2) {
		sum += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
		sum += f[i + 1].x * f[i + 1].x + f[i + 1].y * f[i + 1].y + f[i + 1].z * f[i + 1].z;
	}
	timer.stop();
	timings.emplace_back("AoS unroll(2)", sum, timer.count());

	sum = 0.0;
	timer.start();
	for (unsigned i = 0; i < size; i += 4) {
		sum += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
		sum += f[i + 1].x * f[i + 1].x + f[i + 1].y * f[i + 1].y + f[i + 1].z * f[i + 1].z;
		sum += f[i + 1].x * f[i + 2].x + f[i + 2].y * f[i + 2].y + f[i + 2].z * f[i + 2].z;
		sum += f[i + 1].x * f[i + 3].x + f[i + 3].y * f[i + 3].y + f[i + 3].z * f[i + 3].z;
	}
	timer.stop();
	timings.emplace_back("AoS unroll(4)", sum, timer.count());

	sum = 0.0;
	timer.start();
	dataType sum1 { }, sum2 { };
	for (unsigned i = 0; i < size; i += 4) {
		sum1 += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
		sum1 += f[i + 1].x * f[i + 1].x + f[i + 1].y * f[i + 1].y + f[i + 1].z * f[i + 1].z;
		sum2 += f[i + 1].x * f[i + 2].x + f[i + 2].y * f[i + 2].y + f[i + 2].z * f[i + 2].z;
		sum2 += f[i + 1].x * f[i + 3].x + f[i + 3].y * f[i + 3].y + f[i + 3].z * f[i + 3].z;
	}
	sum = sum1 + sum2;
	timer.stop();
	timings.emplace_back("AoS unroll(4+2)", sum, timer.count());

	sum = 0.0;
	timer.start();
	register dataType sumr1 { }, sumr2 { };
	for (unsigned i = 0; i < size; i += 4) {
		sumr1 += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
		sumr1 += f[i + 1].x * f[i + 1].x + f[i + 1].y * f[i + 1].y + f[i + 1].z * f[i + 1].z;
		sumr2 += f[i + 1].x * f[i + 2].x + f[i + 2].y * f[i + 2].y + f[i + 2].z * f[i + 2].z;
		sumr2 += f[i + 1].x * f[i + 3].x + f[i + 3].y * f[i + 3].y + f[i + 3].z * f[i + 3].z;
	}
	sum = sumr1 + sumr2;
	timer.stop();
	timings.emplace_back("AoS register unroll(4+2)", sum, timer.count());

	sum = 0.0f;
	timer.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < size; i++) {
		sum += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
	}
	timer.stop();
	timings.emplace_back("AoS OpenMP", sum, timer.count());

	sum = 0.0;
	timer.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < size; i += 2) {
		sum += f[i].x * f[i].x + f[i].y * f[i].y + f[i].z * f[i].z;
		sum += f[i + 1].x * f[i + 1].x + f[i + 1].y * f[i + 1].y + f[i + 1].z * f[i + 1].z;
	}
	timer.stop();
	timings.emplace_back("AoS OpenMP+unroll(2)", sum, timer.count());

	sum = 0.0;
	timer.start();
	__m128 mmRes = _mm_setzero_ps();
	for (unsigned i = 0; i < size; i++) {
		auto r = _mm_set_ps(f[i].x,f[i].y,f[i].z,0.0f);
		mmRes = _mm_add_ps(mmRes,_mm_mul_ps(r,r));
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	timer.stop();
	timings.emplace_back("AoS SSE3", sum, timer.count());

	return baseSum;
}

#endif

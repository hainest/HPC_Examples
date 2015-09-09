#ifndef SUM_HPP_
#define SUM_HPP_

#include <algorithm>
#include <immintrin.h>
#include <omp.h>
#include <tuple>
#include "aligned_allocator.hpp"
#include "stopwatch.hpp"
#include "rand_fill.hpp"

template<typename dataType, typename collectionType>
dataType doSum(const size_t size, collectionType &timings) {

	std::vector<dataType, aligned_allocator<dataType, 32>> v(size);
	rand_fill(v);

	stopwatch clock;
	dataType sum { };

	clock.start();
	sum = std::accumulate(std::begin(v), std::end(v), dataType { });
	clock.stop();
	timings.emplace_back("std::accumulate", sum, clock.count());
	auto baseSum = sum;

	sum = 0.0;
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 2) {
		sum += v[i];
		sum += v[i + 1];
	}
	clock.stop();
	timings.emplace_back("unroll(2)", sum, clock.count());

	sum = 0.0;
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 4) {
		sum += v[i];
		sum += v[i + 1];
		sum += v[i + 2];
		sum += v[i + 3];
	}
	clock.stop();
	timings.emplace_back("unroll(4)", sum, clock.count());

	sum = 0.0;
	clock.start();
	dataType sum1 { }, sum2 { };
	for (unsigned i = 0; i < v.size(); i += 4) {
		sum1 += v[i];
		sum1 += v[i + 1];
		sum2 += v[i + 2];
		sum2 += v[i + 3];
	}
	sum = sum1 + sum2;
	clock.stop();
	timings.emplace_back("unroll(4+2)", sum, clock.count());

	sum = 0.0;
	clock.start();
	register dataType rsum1 { }, rsum2 { };
	for (unsigned i = 0; i < v.size(); i += 4) {
		rsum1 += v[i];
		rsum1 += v[i + 1];
		rsum2 += v[i + 2];
		rsum2 += v[i + 3];
	}
	sum = rsum1 + rsum2;
	clock.stop();
	timings.emplace_back("unroll(4+2,register)", sum, clock.count());

	sum = 0.0;
	sum1 = sum2 = 0.0;
	dataType sum3 { }, sum4 { };
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 16) {
		sum1 += v[i];
		sum1 += v[i + 1];
		sum2 += v[i + 2];
		sum2 += v[i + 3];

		sum1 += v[i + 4];
		sum1 += v[i + 5];
		sum2 += v[i + 6];
		sum2 += v[i + 7];

		sum1 += v[i + 8];
		sum1 += v[i + 9];
		sum2 += v[i + 10];
		sum2 += v[i + 11];

		sum1 += v[i + 12];
		sum1 += v[i + 13];
		sum2 += v[i + 14];
		sum2 += v[i + 15];
	}
	sum = sum1 + sum2 + sum3 + sum4;
	clock.stop();
	timings.emplace_back("unroll(16+4)", sum, clock.count());

	sum = 0.0;
	register dataType rsum3 { }, rsum4 { };
	rsum1 = rsum2 = 0.0;
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 16) {
		rsum1 += v[i];
		rsum1 += v[i + 1];
		rsum2 += v[i + 2];
		rsum2 += v[i + 3];

		rsum1 += v[i + 4];
		rsum1 += v[i + 5];
		rsum2 += v[i + 6];
		rsum2 += v[i + 7];

		rsum1 += v[i + 8];
		rsum1 += v[i + 9];
		rsum2 += v[i + 10];
		rsum2 += v[i + 11];

		rsum1 += v[i + 12];
		rsum1 += v[i + 13];
		rsum2 += v[i + 14];
		rsum2 += v[i + 15];
	}
	sum = rsum1 + rsum2 + rsum3 + rsum4;
	clock.stop();
	timings.emplace_back("unroll(16+4,register)", sum, clock.count());

	sum = 0.0;
	clock.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < v.size(); i++) {
		sum += v[i];
	}
	clock.stop();
	timings.emplace_back("OpenMP", sum, clock.count());

	sum = 0.0;
	clock.start();
#pragma omp parallel for reduction(+:sum) num_threads(2)
	for (unsigned i = 0; i < v.size(); i += 2) {
		sum += v[i];
		sum += v[i + 1];
	}
	clock.stop();
	timings.emplace_back("unroll(2)+OpenMP", sum, clock.count());

	sum = 0.0;
	clock.start();
	__m128 mmRes = _mm_setzero_ps();
	dataType* p = v.data();
	for (unsigned i = 0; i < v.size(); i += 4) {
		mmRes = _mm_add_ps(mmRes, _mm_load_ps(p + i));
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	clock.stop();
	timings.emplace_back("SSE3", sum, clock.count());

	sum = 0.0;
	mmRes = _mm_setzero_ps();
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 16) {
		mmRes = _mm_add_ps(mmRes, _mm_load_ps(p + i));
		mmRes = _mm_add_ps(mmRes, _mm_load_ps(p + i + 4));
		mmRes = _mm_add_ps(mmRes, _mm_load_ps(p + i + 8));
		mmRes = _mm_add_ps(mmRes, _mm_load_ps(p + i + 12));
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	clock.stop();
	timings.emplace_back("unroll(16)+SSE3", sum, clock.count());

	sum = 0.0;
	mmRes = _mm_setzero_ps();
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 16) {
		__m128 v0 = _mm_load_ps(p + i + 0);
		__m128 v1 = _mm_load_ps(p + i + 4);
		__m128 s01 = _mm_add_ps(v0, v1);

		__m128 v2 = _mm_load_ps(p + i + 8);
		__m128 v3 = _mm_load_ps(p + i + 12);
		__m128 s23 = _mm_add_ps(v2, v3);

		mmRes = _mm_add_ps(mmRes, s01);
		mmRes = _mm_add_ps(mmRes, s23);
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	clock.stop();
	timings.emplace_back("unroll(16+4),SSE3", sum, clock.count());

	sum = 0.0;
	mmRes = _mm_setzero_ps();
	clock.start();

	#pragma omp parallel num_threads(2)
	{
		__m128 tmp[2];
		tmp[0] = _mm_setzero_ps();
		tmp[1] = _mm_setzero_ps();
		const auto tid = omp_get_thread_num();

		#pragma omp for
		for (unsigned i = 0; i < v.size(); i += 4) {
			tmp[tid] = _mm_add_ps(tmp[tid], _mm_load_ps(p + i));
		}
		mmRes = tmp[0] + tmp[1];
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	clock.stop();
	timings.emplace_back("OpenMP+SSE3", sum, clock.count());

	sum = 0.0;
	mmRes = _mm_setzero_ps();
	clock.start();
#pragma omp parallel num_threads(2)
	{
		__m128 tmp[2];
		tmp[0] = _mm_setzero_ps();
		tmp[1] = _mm_setzero_ps();
		const auto tid = omp_get_thread_num();
		for (unsigned i = 0; i < v.size(); i += 16) {
			__m128 v0 = _mm_load_ps(p + i + 0);
			__m128 v1 = _mm_load_ps(p + i + 4);
			__m128 s01 = _mm_add_ps(v0, v1);

			__m128 v2 = _mm_load_ps(p + i + 8);
			__m128 v3 = _mm_load_ps(p + i + 12);
			__m128 s23 = _mm_add_ps(v2, v3);

			tmp[tid] = _mm_add_ps(tmp[tid], s01);
			tmp[tid] = _mm_add_ps(tmp[tid], s23);
		}
		mmRes = tmp[0] + tmp[1];
	}
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	mmRes = _mm_hadd_ps(mmRes, mmRes);
	sum = _mm_cvtss_f32(mmRes);
	clock.stop();
	timings.emplace_back("unroll(16+4)+OpenMP+SSE3", sum, clock.count());

	sum = 0.0;
	__m256 mm256Res = _mm256_setzero_ps();
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 8) {
		mm256Res = _mm256_add_ps(mm256Res, _mm256_load_ps(p + i));
	}
	dataType *tmp = reinterpret_cast<dataType*>(&mm256Res);
	sum = tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
	clock.stop();
	timings.emplace_back("AVX", sum, clock.count());

	sum = 0.0;
	mm256Res = _mm256_setzero_ps();
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 32) {
		mm256Res = _mm256_add_ps(mm256Res, _mm256_load_ps(p + i));
		mm256Res = _mm256_add_ps(mm256Res, _mm256_load_ps(p + i + 8));
		mm256Res = _mm256_add_ps(mm256Res, _mm256_load_ps(p + i + 16));
		mm256Res = _mm256_add_ps(mm256Res, _mm256_load_ps(p + i + 24));
	}
	tmp = reinterpret_cast<dataType*>(&mm256Res);
	sum = tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
	clock.stop();
	timings.emplace_back("unroll(32)+AVX", sum, clock.count());

	sum = 0.0;
	mm256Res = _mm256_setzero_ps();
	clock.start();
	for (unsigned i = 0; i < v.size(); i += 32) {
		__m256 v0 = _mm256_load_ps(p + i);
		__m256 v1 = _mm256_load_ps(p + i + 8);
		__m256 s01 = _mm256_add_ps(v0, v1);

		__m256 v2 = _mm256_load_ps(p + i + 16);
		__m256 v3 = _mm256_load_ps(p + i + 24);
		__m256 s23 = _mm256_add_ps(v2, v3);

		mm256Res = _mm256_add_ps(mm256Res, s01);
		mm256Res = _mm256_add_ps(mm256Res, s23);
	}
	tmp = reinterpret_cast<dataType*>(&mm256Res);
	sum = tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7];
	clock.stop();
	timings.emplace_back("unroll(32+4)+AVX", sum, clock.count());

	return baseSum;
}

#endif /* SUM_HPP_ */

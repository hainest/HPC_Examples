#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <tuple>
#include "sum.hpp"
#include "stopwatch.hpp"

int main() {
	using valType = float;
	using tupType = std::tuple<std::string, valType, float>;
	constexpr typename std::vector<valType>::size_type size = 10000000;

	std::vector<tupType> timings;

	auto baseSum = doSum<valType,decltype(timings)>(size,timings);

	std::sort(timings.begin(),timings.end(),[](const tupType &a, const tupType &b){return std::get<2>(a) < std::get<2>(b);});

	std::cout << "\t"
			  << std::left     << std::setw(28) << "Unroll Type"
			  << std::internal << std::setw(15) << "% Difference"
			  << std::internal << std::setw(15) << "Time (ms)\n";

	std::cout << std::setfill('-') << std::setw(80) << "\n" << std::setfill(' ');
	auto pdiff = [](valType a, valType b) {return 100.0f * std::abs(a-b) / ((a+b)/2.0f);};
	for (const auto &x : timings) {
		std::cout << "\t"
				  << std::left     << std::setw(28) << std::get<0>(x)
				  << std::internal << std::setw(15) << std::scientific << pdiff(std::get<1>(x),baseSum)
				  << std::internal << std::setw(15) << std::fixed << std::get<2>(x) << "\n";
	}
}

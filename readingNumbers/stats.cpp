#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

int main() {
	// print min, max, median
	std::vector<int> values;
	std::ifstream fin { "numbers.txt" };
	if (!fin) {
		std::cerr << "Could not open numbers.txt\n";
		return 1;
	}

	using stream = std::istream_iterator<decltype(values)::value_type>;
	std::copy(stream(fin), {}, std::back_inserter(values));
	std::sort(values.begin(), values.end());
	std::cout << "min = " << values[0] << "\nmax = " << values[values.size() - 1] << "\nmedian = ";

	if (values.size() % 2) {
		std::cout << values[values.size() / 2] << std::endl;
	} else {
		auto left = values[(values.size() - 1) / 2];
		auto right = values[(values.size() - 1) / 2 + 1];
		std::cout << (right + left) / 2.0f << std::endl;
	}
}

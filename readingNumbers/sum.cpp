#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>

int main() {
	// print sum
	std::ifstream fin{"numbers.txt"};
	if(!fin) {
		std::cerr << "Could not open numbers.txt\n";
		return 1;
	}
	std::cout << "sum = "
			<< std::accumulate(std::istream_iterator<int>(fin),{},0)
			<< std::endl;
}

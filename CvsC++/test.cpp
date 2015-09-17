#include <fstream>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
	if(argc != 3) {
		std::cerr << "Usage: " << argv[0] << " file N\n";
		return -1;
	}
	
	std::ifstream fin{argv[1]};
	
	if(!fin) {
		std::cerr << "Unable to open file " << argv[1] << "\n";
		return -1;
	}
	
	std::vector<int> x;
	x.reserve(std::stoi(argv[2]));
	
	std::copy(std::istream_iterator<int>(fin),{},std::back_inserter(x));
	
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
	std::sort(x.begin(),x.end());
	auto end = high_resolution_clock::now();
	std::cout << x.size() << ','
			  << duration_cast<microseconds>(end-start).count()
			  << std::endl;
}

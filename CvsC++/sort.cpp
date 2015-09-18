#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include "stopwatch.hpp"

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

	// This is purely an optimization to help reduce the total runtime.
	// It does not affect the measured runtime.
	x.reserve(std::stoi(argv[2]));
	
	std::copy(std::istream_iterator<int>(fin),{},std::back_inserter(x));
	
	stopwatch sw;
	sw.start();
	std::sort(x.begin(),x.end());
	sw.stop();
	std::cout << x.size() << ',' << sw.count<float,std::ratio<1,1000000>>() << "\n";
}

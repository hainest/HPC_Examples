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
	
	std::ifstream fin{argv[1],std::ios::binary};
	
	if(!fin) {
		std::cerr << "Unable to open file " << argv[1] << "\n";
		return -1;
	}
	
	std::vector<int> x(std::stoi(argv[2]));
	fin.read(reinterpret_cast<char*>(x.data()),x.size()*sizeof(int));

	stopwatch sw;
	sw.start();
	std::sort(x.begin(),x.end());
	sw.stop();
	std::cout << x.size() << ',' << sw.count() << "\n";
}

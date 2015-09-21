#include <fstream>
#include <random>
#include <string>
#include <iostream>

int main() {
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<int> dist;

	for(auto n : {1000000, 10000000, 50000000}) {
		std::ofstream fout{"numbers"+std::to_string(n),std::ios::binary};
		std::cout << n << ' ';
		std::flush(std::cout);
		for(int i=0; i<n;++i) {
			auto v = dist(gen);
			fout.write(reinterpret_cast<const char*>(&v),sizeof(v));
		}
	}
	std::cout << std::endl;
}

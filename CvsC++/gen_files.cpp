#include <fstream>
#include <random>
#include <string>

int main() {
	std::random_device device;
	std::mt19937 gen(device());
	std::uniform_int_distribution<> dist;

	for(auto n : {1000000, 10000000, 50000000}) {
		std::ofstream fout{"numbers"+std::to_string(n)};
		for(int i=0; i<n;++i) {
			fout << dist(gen) << "\n";
		}
	}
}

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include "Futoshiki.h"


int main() {
	Cell c(0, 0, 0, 8);
	std::cout << "!!!\n";
	Board board;
	for (int i = 1; i <= 3; ++i) {
		std::cout << "input " << i << std::endl;
		std::ifstream ifs("Input" + std::to_string(i) + ".txt");
		ifs >> board;
		board.solve();
		std::cout << board;
		std::cout << "\n-----------------------\n";
	}
}
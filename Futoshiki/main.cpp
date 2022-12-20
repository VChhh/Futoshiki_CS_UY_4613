#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include "Futoshiki.h"


int main() {
	Board board;
	for (int i = 1; i <= 1; ++i) {
		std::cout << "input " << i << std::endl;
		std::ifstream ifs("Input" + std::to_string(i) + ".txt");
		ifs >> board;
		//Board result;
		//board.solve(result);
		//std::cout << result;

		Board result = Board(board);
		result = board;
		Board b;
		result = b;

		std::cout << "\n-----------------------\n";
		std::cout << result;
		std::cout << "\n-----------------------\n";
	}
}
// author
// Charles Pan, cp3723
// Nick Li, ql2015

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include "Futoshiki.h"


int main() {
	Board board;
	for (int i = 1; i <= 3; ++i) {
		std::cout << "input " << i << std::endl;
		std::ifstream ifs("Input" + std::to_string(i) + ".txt");
		ifs >> board; // read input file
		ifs.close();
		board.solve();
		std::cout << "-----------------------\n";
		std::cout << board;
		std::ofstream ofs;
		ofs.open("Output" + std::to_string(i) + ".txt");
		ofs << board; // write to the output file
		ofs.close();
		std::cout << "-----------------------\n\n";
		board.clear();
	}
}
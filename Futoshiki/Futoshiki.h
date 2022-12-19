#ifndef FUTOSHIKI_H
#define FUTOSHIKI_H


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>


enum direction;


struct Cell
{
	int x;
	int y;
	int value;
	int domain_size;

	Cell();
	Cell(int x, int y, int v, int domain_size = 5);
	Cell(Cell&);
	std::set<int> domain;
	std::vector<std::pair<direction, std::string>> constraints;
	void updateDomain(string, int); // ?
};



class Board {
	friend std::ostream& operator<<(std::ostream& os, const Board& rhs);
	friend std::istream& operator>>(std::istream& is, Board& rhs);
public:
	int filled;
	std::vector<Cell> cells;



	Board(Board&);
	// decrease domain for a whole line
	void updateDomainCross(int x, int y, int v);

	int getConstraint(int index);

	int selectUnassignedVariable();
	bool checkComplete() { return filled == 25; };
	bool solve();


};



#endif // !FUTOSHIKI_H

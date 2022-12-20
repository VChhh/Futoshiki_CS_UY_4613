#ifndef FUTOSHIKI_H
#define FUTOSHIKI_H

// author
// Charles Pan, cp3723
// Nick Li, ql2015

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>


enum direction;


struct Cell
{
public:
	int value;
	int degree;

	Cell();
	Cell(int v, int degree);
	Cell(const Cell&);
	std::set<int> domain;
	std::vector<std::pair<direction, std::string>> constraints;

	Cell& operator=(const Cell& rhs);
};



class Board {
	friend std::ostream& operator<<(std::ostream& os, const Board& rhs);
	friend std::istream& operator>>(std::istream& is, Board& rhs);
public:
	int filled;
	std::vector<Cell> cells;


	Board();
	Board(const Board&);
	// decrease domain for a whole line
	void updateDomainCross(int x, int y, int v);
	int selectUnassignedVariable();
	void update_domain_by_constraits();
	bool checkComplete() { return filled == 25; };
	bool solve();
	void clear() { filled = 0; cells.clear(); }

	Board& operator=(const Board& rhs);
};



#endif // !FUTOSHIKI_H

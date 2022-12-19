#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include "Futoshiki.h"

using namespace std;

enum direction
{
	u = 0, // up
	d = 1, // down
	l = 2, // left
	r = 3  // right
};

Cell::Cell() {
	x = -1;
	y = -1;
	value = -1;
}

Cell::Cell(int x, int y, int v, int domain_size = 5) {
	this->x = x;
	this->y = y;
	this->value = v;
	this->domain_size = domain_size;
	for (int i = 1; i <= 5; i++) domain.insert(i);
}

Cell::Cell(Cell& rhs) {
	this->x = rhs.x;
	this->y = rhs.y;
	this->value = rhs.value;
	this->domain = rhs.domain;
	this->domain_size = rhs.domain_size;
	this->constraints = rhs.constraints;
}

///////////////////////////// BOARD /////////////////////////////////
// output solution
ostream& operator<<(ostream& os, const Board& rhs) {
	int i, j;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			os << rhs.cells[i + 5 * j].value << " ";
		}
		os << endl;
	}
	os << endl;
	return os;
}
// input file data
istream& operator>>(istream& is, Board& rhs) {
	// initialize
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			Cell c(i, j, NULL);
		}
	}

	// fill in initial number
	int x, y, value;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			is >> value;
			if (value != 0) {
				rhs.updateDomainCross(i, j, value);
			}
		}
	}

	// read constraints
	string symbol;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 4; i++) {
			is >> symbol;
			if (symbol != "0") {
				rhs.cells[j * 5 + i].constraints.push_back({ r, symbol });
				rhs.cells[j * 5 + i + 1].constraints.push_back({ l, symbol });
			}
		}
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 5; i++) {
			is >> symbol;
			if (symbol != "0") {
				rhs.cells[j * 5 + i].constraints.push_back({ d, symbol });
				rhs.cells[(j + 1) * 5 + i].constraints.push_back({ u, symbol });
			}
		}
	}
	return is;
}

// copy constructor
Board::Board(Board& rhs) {
	rhs.cells = this->cells;
	rhs.filled = this->filled;

}

void Board::updateDomainCross(int x, int y, int v) {
		for (int i = x; i < 25; i += 5) {
			cells[i].domain.erase(v);
		}
		for (int i = y * 5; i < (y * 5 + 5); i++) {
			cells[i].domain.erase(v);
		}
}

int Board::getConstraint(int index) {
	Cell* c = &cells[index];
	Cell* nbh;
	// TODO
	for (pair<direction, string>& i : c->constraints) {
		direction dir = i.first;
		switch (dir)
		{
		case u:
			break;
		case d:
			break;
		case l:
			break;
		case r:
			break;
		default:
			break;
		}
	}
	
	return 0;
}

int Board::selectUnassignedVariable() {
	int leastDomain = 6;
	int countLeast = 0;
	for (int i = 0; i < 25; i++) {
		int tempLeast = cells[i].domain.size();
		if ( tempLeast <= leastDomain ) {
			if (tempLeast < leastDomain) countLeast = 0;
			leastDomain = tempLeast;
			countLeast++;
		}
	}
	// more than one with the same least domain
	if (countLeast > 1) {
		for (int i = 0; i < 25; i++) {
			//TODO
		}
	}


	return -1;
}

bool Board::solve() {
	if (checkComplete()) {
		std::cout << "output some shit" << endl;
	}
	// 
	int index = selectUnassignedVariable();
	for (int i : cells[index].domain) {
		Board nextNode(*this);
		nextNode.cells[index].value = i;
		nextNode.updateDomainCross(index / 5, index % 5, i);
		if (nextNode.solve()) {
			return true;
		}
	}

	return false;
}
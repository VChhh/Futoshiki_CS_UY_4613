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
	degree = -1;
}

Cell::Cell(int x, int y, int v, int degree = 8) {
	this->x = x;
	this->y = y;
	this->value = v;
	this->degree = degree;
	for (int i = 1; i <= 5; i++) this->domain.insert(i);
}

Cell::Cell(const Cell& rhs) {
	this->x = rhs.x;
	this->y = rhs.y;
	this->value = rhs.value;
	this->domain = rhs.domain;
	this->degree = degree;
	this->constraints = rhs.constraints;
}

///////////////////////////// BOARD /////////////////////////////////
// output solution
ostream& operator<<(ostream& os, const Board& rhs) {
	int i, j;
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			os << rhs.cells[i + 5 * j].value << " "; // not sure if it's correct
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
			Cell c(i, j, 0);
			rhs.cells.push_back(c);
		}
	}
	
	// fill in initial number
	int value;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			is >> value;
			// not blank
			if (value != 0) {
				
				rhs.cells[j * 5 + i].value = value;
				std::cout << "here\n";
				rhs.updateDomainCross(i, j, value);
				rhs.filled++;
			}
			std::cout << "there\n";
		}
	}

	

	// read constraints
	string symbol;
	// horizontal constraints
	// 5 rows of 4 constraints
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 3; i++) {
			is >> symbol;
			if (symbol != "0") {
				rhs.cells[j * 5 + i].constraints.push_back({ r, symbol });
				rhs.cells[j * 5 + i + 1].constraints.push_back({ l, symbol });
			}
		}
	}
	// vertical constraints
	// 4 rows of 5 constraints
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			is >> symbol;
			if (symbol != "0") {
				rhs.cells[j * 5 + i].constraints.push_back({ d, symbol });
				rhs.cells[(j + 1) * 5 + i].constraints.push_back({ u, symbol });
			}
		}
	}

	rhs.update_domain_by_constraits();

	return is;
}

Board::Board() {
	filled = 0;
	for (int i = 0; i < 25; ++i) {
		//Cell c(0, 0, 0, 8);
		//cells.push_back(c);
	}
}

// copy constructor
Board::Board(const Board& rhs) {
	this->cells = rhs.cells;
	this->filled = rhs.filled;
}

// clear domain of current element
// update domains, domain_size, degree of elements on the same row and column
void Board::updateDomainCross(int x, int y, int v) {
	cells[y * 5 + x].domain.clear();
	// vertical
	for (int i = x; i < 25; i += 5) {
		if (cells[i].domain.erase(v)) {
			cells[i].degree--;
		}
	}
	// horizontal
	for (int i = y * 5; i < (y * 5 + 5); i++) {
		if (cells[i].domain.erase(v)) {
			cells[i].degree--;
		}
	}
}

// first apply MRV
// then degree heuristic
// return the index of the selected element
int Board::selectUnassignedVariable() {
	int domain_size_min = 6;
	int degree_max = -1;
	int index = -1;
	for (int i = 0; i < 25; ++i) {
		if (cells[i].value != 0) continue; // skip assigned elements
		if (cells[i].domain.size() < domain_size_min) {
			// MRV
			domain_size_min = cells[i].domain.size();
			degree_max = cells[i].degree;
			index = i;
		}
		else if (cells[i].domain.size() == domain_size_min) {
			// compare degree
			if (cells[i].degree > degree_max) {
				degree_max = cells[i].degree;
				index = i;
			}
		}
	}
	return index;
}

// helper function
// sign == 0, cell.v < value; sign == 1, cell.v > value;
inline void update_cell(std::set<int>& domain, int value, int sign) {
	if (sign == 0) {
		for (int i = value; i <= 5; ++i) {
			domain.erase(i);
		}
	}
	else {
		for (int i = value; i >= 0; --i) {
			domain.erase(i);
		}
	}
}

void Board::update_domain_by_constraits() {
	for (int i = 0; i < 25; ++i) {
		if (cells[i].value != 0) {
			// update domain of adjacent elements
			for (int j = 0; j < cells[i].constraints.size(); ++j) {
				direction dir = cells[i].constraints[j].first;
				string s = cells[i].constraints[j].second;
				if (dir == u) {
					update_cell(cells[i - 5].domain, cells[i].value, s == "^" ? 0 : 1);
				}
				else if (dir == d) {
					update_cell(cells[i + 5].domain, cells[i].value, s == "v" ? 0 : 1);
				}
				else if (dir == l) {
					update_cell(cells[i - 1].domain, cells[i].value, s == "<" ? 0 : 1);
				}
				else { // dir == r
					update_cell(cells[i + 1].domain, cells[i].value, s == ">" ? 0 : 1);
				}
			}
		}
	}
}

bool Board::solve() {
	if (checkComplete()) {
		return true;
	}
	int index = selectUnassignedVariable();
	if (cells[index].domain.size() == 0) return false;
	bool result = false;
	for (int i : cells[index].domain) {
		Board new_board(*this);
		new_board.updateDomainCross(index / 5, index % 5, i);
		new_board.update_domain_by_constraits();
		result |= new_board.solve();
		if (result) return result; // if true, return directly
	}
	return result;
}
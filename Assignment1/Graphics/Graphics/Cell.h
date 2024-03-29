#pragma once
class Cell
{
private:
	int row;
	int col;
	Cell* parent;

public:
	Cell();
	Cell(int r, int c, Cell* p);
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
};


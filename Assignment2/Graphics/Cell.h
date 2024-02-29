#pragma once
class Cell
{
private:
	int row;
	int column;
	Cell* parent;
public:
	Cell();
	Cell(int r, int c,Cell* p);
	~Cell();
	int getRow() { return row; }
	int getColumn() { return column; }
	Cell* getParent() { return parent; }
};


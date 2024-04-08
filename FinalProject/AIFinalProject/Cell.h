#pragma once
class Cell
{
private:
	int row;
	int column;
	double distance;
	Cell* parent;
public:
	Cell();
	Cell(int r, int c, Cell* p);
	Cell(int r, int c, double distance, Cell* p);
	void setRow(int x) { row = x; }
	void setCol(int y) { column = y; }
	~Cell();
	int getRow() { return row; }
	int getColumn() { return column; }
	double getDistanceToTarget() { return distance; }
	Cell*& getParent() { return parent; }
};
#include "Cell.h"



Cell::Cell()
{
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	column = c;
	parent = p;
}
Cell::Cell(int r, int c, double d, Cell* p)
{
	row = r;
	column = c;
	distance = d;
	parent = p;
}


Cell::~Cell()
{
}
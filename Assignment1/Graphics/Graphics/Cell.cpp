#include "Cell.h"

Cell::Cell()
{
	parent = nullptr;
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	col = c;
	parent = p;
}

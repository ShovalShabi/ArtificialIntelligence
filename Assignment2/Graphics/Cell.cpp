#include "Cell.h"



Cell::Cell()
{
}

Cell::Cell(int r, int c, Cell * p)
{
	row = r;
	column = c; 
	parent = p;
}



Cell::~Cell()
{
}

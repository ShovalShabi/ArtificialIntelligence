#include "Cell.h"


/**
 * @brief Default constructor for Cell class.
 */
Cell::Cell()
{
}

/**
 * @brief Constructor for Cell class with row, column, and parent parameters.
 * @param r Row index of the cell.
 * @param c Column index of the cell.
 * @param p Pointer to the parent cell.
 */
Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	column = c;
	parent = p;
}

/**
 * @brief Constructor for Cell class with row, column, distance, and parent parameters.
 * @param r Row index of the cell.
 * @param c Column index of the cell.
 * @param d Distance to target cell.
 * @param p Pointer to the parent cell.
 */
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
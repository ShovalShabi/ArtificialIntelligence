#pragma once
#include "Cell.h"
#include <math.h>

/**
 * @brief Functor to compare cells based on their distance to a target.
 */
class CompareCellsWithDistance
{
public:
	/**
	 * @brief Default constructor for the CompareCellsWithDistance class.
	 */
	CompareCellsWithDistance();

	/**
	 * @brief Destructor for the CompareCellsWithDistance class.
	 */
	~CompareCellsWithDistance();

	/**
	 * @brief Overloaded function call operator to compare two cells based on their distance to a target.
	 * @param c1 Pointer to the first cell.
	 * @param c2 Pointer to the second cell.
	 * @return True if the distance of c1 to the target is greater than the distance of c2 to the target, false otherwise.
	 */
	bool operator() (Cell* c1, Cell* c2) {
		return (c1->getDistanceToTarget() > c2->getDistanceToTarget());
	}
};

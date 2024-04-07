#pragma once
#include "Cell.h"
#include <math.h>

class CompareCellsWithDistance
{
public:
	CompareCellsWithDistance();
	~CompareCellsWithDistance();

	bool operator() (Cell* c1, Cell* c2) {
		return (c1->getDistanceToTarget() > c2->getDistanceToTarget());
	}
};
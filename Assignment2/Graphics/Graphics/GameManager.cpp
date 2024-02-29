#include "GameManager.h"
#include "constants.h"
#include <iostream>


GameManager::GameManager() {
	runBFS = false;
	runDFS = false;
	runBestFirstSearch = false;
	runAStar = false;
}

void GameManager:: RestorePath(Cell* pc, int ** maze)
{
	while (pc->getParent() != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

void GameManager::CheckNeighbor(int row, int column, Cell* pcurrent, int ** maze)
{
	if (maze[row][column] == TARGET)
	{
		runBFS = false; // stop running BFS
		runDFS = false; // stop runnnig DFS
		runBestFirstSearch = false; // stop running best-first-search
		runAStar = false; // stop running A*
		cout << "The solution has been found\n";
		RestorePath(pcurrent,maze);
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		// paint this neighbor gray
		grays.push_back(pn);
		maze[row][column] = ORANGE;
	}
}

void GameManager::RunBFSIteration(int** maze)
{
	Cell* pcurrent;
	int r, c;
	if (grays.empty()) // no solution exists
	{
		runBFS = false; // stop running BFS
		cout << "No solution\n";
		return;
	}
	else // grays is not empty
	{
		pcurrent = *grays.begin();
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		maze[r][c] = VISITED; // paint it black
		grays.erase(grays.begin());
		// add all white neighbors of pcurrent to grays
		// UP
		if (runBFS && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent,maze);
		// DOWN
		if (runBFS && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent,maze);
		// LEFT
		if (runBFS && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r, c - 1, pcurrent,maze);
		// RIGHT
		if (runBFS && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent,maze);

	}
}

void GameManager::RunDFSIteration(int** maze)
{
	Cell* pcurrent;
	int r, c;

	if (grays.empty())
	{
		runDFS = false; // stop running DFS
		cout << "No solution\n";
		return;
	}
	else
	{
		pcurrent = grays.back(); // DFS uses STACK
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		maze[r][c] = VISITED; // paint it black
		grays.pop_back();

		// check neighbors (use random direction)
		int directions[4] = { -1,-1,-1,-1 };
		int dir, place;
		for (dir = 0; dir < 4; dir++)
		{
			do {
				place = rand() % 4;
			} while (directions[place] != -1);
			directions[place] = dir;
		}

		for (int i = 0; i < 4; i++)
		{
			if (runDFS)
			{
				switch (directions[i])
				{
				case 0: // up
					if (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET)
						CheckNeighbor(r + 1, c, pcurrent,maze);
					break;
				case 1: // down
					if (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET)
						CheckNeighbor(r - 1, c, pcurrent, maze);
					break;
				case 2: // left
					if (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET)
						CheckNeighbor(r, c - 1, pcurrent, maze);
					break;
				case 3: // right
					if (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET)
						CheckNeighbor(r, c + 1, pcurrent, maze);
					break;

				}
			}
		}
	}
}

// get distance of given cell to target.
double GameManager:: getDistanceFromCellToTarget(Cell* cur, int targetR, int targetC)
{
	return sqrt(abs(pow(targetR - cur->getRow(), 2) + pow(targetC - cur->getCol(), 2)));
}

// get distance of given cell from Start.
double GameManager:: getDistanceFromCellToStart(Cell* cur, int startR, int startC)
{
	return sqrt(abs(pow(startR - cur->getRow(), 2) + pow(startC - cur->getCol(), 2)));
}

// find the closest cell to TARGET from grays vector & erase it.
Cell* GameManager:: getClosestOptionToBestFirstSearch(int targetR, int targetC) {
	Cell* toRet = *grays.begin();
	double minDistance = 10000000, tempDistance = 1;
	int i = 0, remove = 0;
	for (Cell* cell : grays)// Find cell closest to target
	{
		tempDistance = getDistanceFromCellToTarget(cell,targetR,targetC);
		if (tempDistance < minDistance)
		{
			minDistance = tempDistance;
			toRet = cell;
			remove = i;
		}
		i++;
	}
	grays.erase(grays.begin() + remove); // remove the cell that closest to target from grays vector
	return toRet;
}

// find the closest cell to TARGET + START from grays vector & erase it.
Cell* GameManager:: getClosestOptionToAStar(int startR, int startC, int targetR, int targetC) {
	Cell* toRet = *grays.begin();
	double minHCost = INT_MAX, minGCost = INT_MAX, minTotalCost = INT_MAX, HCost, GCost, totalCost;
	int i = 0, remove = 0; // i = index, remove = index remember to use with vector.erase function.
	for (Cell* cell : grays)// Find cell closest to (target + start)
	{
		HCost = getDistanceFromCellToTarget(cell,targetR,targetC); // distance of cell from target
		GCost = getDistanceFromCellToStart(cell,startR,startC); // distance of cell from start
		totalCost = HCost + GCost; // total cost of cell
		if (totalCost < minTotalCost) // if total cost of cell is minimal -> save it's properties & cell to return
		{
			minTotalCost = totalCost;
			minHCost = HCost;
			minGCost = GCost;
			toRet = cell;
			remove = i; // to execute grays.erase(grays.begin() + remove)
		}
		else if (totalCost == minTotalCost) // if we have 2 cells with the same total cost - we choose to move on with the one that closer to the target -> lower HCost
		{
			if (HCost < minHCost)
			{
				minHCost = HCost;
				minGCost = GCost;
				toRet = cell;
				remove = i; // to execute grays.erase(grays.begin() + remove)
			}
		}
		i++;
	}
	grays.erase(grays.begin() + remove); // remove the cell that closest to (target + start) from grays vector
	return toRet;
}
void GameManager::RunBestFirstSearchIteration(int** maze, int targetR, int targetC)
{
	Cell* pcurrent;
	int r, c;
	if (grays.empty()) // no solution exists
	{
		runBestFirstSearch = false; // stop running BFS
		cout << "No solution\n";
		return;
	}
	else // grays is not empty
	{
		pcurrent = getClosestOptionToBestFirstSearch(targetR,targetC); // get the closest cell to target & pop it from grays vector.
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		maze[r][c] = VISITED; // paint it black
		// add all white neighbors of pcurrent to grays
		// UP
		if (runBestFirstSearch && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent, maze);
		// DOWN
		if (runBestFirstSearch && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent, maze);
		// LEFT
		if (runBestFirstSearch && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r, c - 1, pcurrent, maze);
		// RIGHT
		if (runBestFirstSearch && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent, maze);
	}
}

void GameManager:: RunAStarIteration(int** maze, int startR, int startC, int targetR, int targetC)
{
	Cell* pcurrent;
	int r, c;
	if (grays.empty()) // no solution exists
	{
		runAStar = false; // stop running BFS
		cout << "No solution\n";
		return;
	}
	else // grays is not empty
	{
		pcurrent = getClosestOptionToAStar(startR,startC,targetR,targetC); // get the closest cell to target + start & pop it from grays vector.
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		maze[r][c] = VISITED; // paint it black
		// add all white neighbors of pcurrent to grays
		// UP
		if (runAStar && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent,maze);
		// DOWN
		if (runAStar && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent, maze);
		// LEFT
		if (runAStar && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r, c - 1, pcurrent, maze);
		// RIGHT
		if (runAStar && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent, maze);
	}
}
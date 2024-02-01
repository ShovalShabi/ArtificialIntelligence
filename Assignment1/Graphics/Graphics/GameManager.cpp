#include "GameManager.h"
#include "constants.h"
#include <iostream>


GameManager::GameManager() {
	runBFS = false;
	runDFS = false;
	runBiBFS = false;
}
void GameManager::RestorePath(Cell* pc, int** maze)
{
	while (pc != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

// DFS a cell on row,col can be either WHITE or TARGET
bool GameManager::CheckNeighbourGenerically(int row, int col, Cell* pCurrent,int** maze ,int targetMark, int typeAlgo)
{
	Cell* pn = nullptr;
	if (maze[row][col] == targetMark || (typeAlgo == BIBFS_OPT && (maze[row][col] == VISITED || maze[row][col] == START || maze[row][col] == ORANGE)))
	{
		switch (typeAlgo)
		{
		case BFS_OPT:
			this->setRunBFS(false);
			break;
		case DFS_OPT:
			this->setRunDFS(false);
			break;
		case BIBFS_OPT:
			this->setRunBiBFS(false);
			this->setRunBFS(false);
			break;
		default:
			break;
		}
		RestorePath(pCurrent,maze);

		std::cout << "Met in Position: Row " << row << ", Col " << col << std::endl;


		while (!bfsGrays.empty() && typeAlgo == BIBFS_OPT) {
			Cell* cell = bfsGrays.front();
			bfsGrays.pop();

			while(cell != nullptr) {
				if (cell->getCol() == col && cell->getRow() == row) {
					RestorePath(cell, maze);
					return false;
				}
				cell = cell->getParent();
			}
		}
		return false;
	}
	else // must be "White Neighbour" - SPACE
	{
		pn = new Cell(row, col, pCurrent); // create new Cell
		switch (typeAlgo)
		{
		case BFS_OPT:
			this->getBfsGrays().push(pn);
			break;
		case DFS_OPT:
			this->getDfsGrays().push_back(pn);
			break;
		case BIBFS_OPT:
			this->getBiBfsGrays().push(pn);
			break;
		default:
			break;
		}
		if(typeAlgo != BIBFS_OPT)
			maze[row][col] = ORANGE;
		else
			maze[row][col] = VIOLET;
		return true; // go_on = true
	}
}


void GameManager::runIteration(int** maze, int typeAlgo, int targetMark)
{
	Cell* pc = nullptr;
	int row, col;
	bool go_on = true;
	bool isEmpty = false;

	switch (typeAlgo)
	{
	case BFS_OPT:
		isEmpty = this->getBfsGrays().empty();
		break;
	case DFS_OPT:
		isEmpty = this->getDfsGrays().empty();
		break;
	case BIBFS_OPT:
		isEmpty = this->getBiBfsGrays().empty();
		break;
	default:
		break;
	}

	if (isEmpty)
	{
		switch (typeAlgo)
		{
		case BFS_OPT:
			this->setRunBFS(false);
			break;
		case DFS_OPT:
			this->setRunDFS(false);
			break;
		case BIBFS_OPT:
			this->setRunBiBFS(false);
			break;
		default:
			break;
		}
		cout << "Grays are empty. There is no solution\n";
		return;
	}
	else // grays is not empty
	{
		// pick the last inserted element of the stack
		switch (typeAlgo)
		{
		case BFS_OPT:
			pc = this->getBfsGrays().front();
			this->getBfsGrays().pop();
			break;
		case DFS_OPT:
			pc = this->getDfsGrays().back();
			this->getDfsGrays().pop_back();
			break;
		case BIBFS_OPT:
			pc = this->getBiBfsGrays().front();// had bug here, somehow the the stack isn't empty
			this->getBiBfsGrays().pop();
			break;
		default:
			break;
		}

		// paint it as visted
		if (maze[pc->getRow()][pc->getCol()] != START && typeAlgo != BIBFS_OPT) 
			maze[pc->getRow()][pc->getCol()] = VISITED;
		else if (maze[pc->getRow()][pc->getCol()] != TARGET && typeAlgo == BIBFS_OPT)
			maze[pc->getRow()][pc->getCol()] = BRONZE;


		row = pc->getRow();
		col = pc->getCol();
		// now check all neighbors of pc and if they are white add them to dfs_grays
		// but if one of them is target then stop DFS because target has been found

		// Choose random direction
		if (typeAlgo == DFS_OPT) {
			int directions[DIRECTION] = { -1,-1,-1,-1 };
			int place;
			for (int dir = 0; dir < DIRECTION; dir++)
			{
				do {
					place = rand() % DIRECTION;
				} while (directions[place] != -1);
				directions[place] = dir;
			}

			for (int i = 0; i < 4; i++)
			{
				switch (directions[i])
				{
				case 0: // up
					if (go_on && (maze[row + 1][col] == SPACE || maze[row + 1][col] == targetMark))
						go_on = CheckNeighbourGenerically(row + 1, col, pc, maze, targetMark, typeAlgo);
					break;
				case 1: // down
					if (go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == targetMark))
						go_on = CheckNeighbourGenerically(row - 1, col, pc, maze, targetMark, typeAlgo);
					break;
				case 2: // left
					if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == targetMark))
						go_on = CheckNeighbourGenerically(row, col - 1, pc, maze, targetMark, typeAlgo);
					break;
				case 3: // right
					if (go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == targetMark))
						go_on = CheckNeighbourGenerically(row, col + 1, pc, maze, targetMark, typeAlgo);
					break;
				}
			}
		}
		//BFS types runs here
		else {
			// up
			if (row + 1 < MSZ && (maze[row + 1][col] == SPACE || maze[row + 1][col] == targetMark || (typeAlgo == BIBFS_OPT && (maze[row + 1][col] == VISITED || maze[row + 1][col] == ORANGE))))
				go_on = CheckNeighbourGenerically(row + 1, col, pc, maze, targetMark, typeAlgo);
			// down
			if (row - 1 > 0 && go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == targetMark || (typeAlgo == BIBFS_OPT && (maze[row-1][col] == VISITED || maze[row - 1][col] == ORANGE))))
				go_on = CheckNeighbourGenerically(row - 1, col, pc, maze, targetMark, typeAlgo);
			// left
			if (col - 1 > 0 && go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == targetMark || (typeAlgo == BIBFS_OPT && (maze[row][col - 1] == VISITED || maze[row][col - 1] == ORANGE))))
				go_on = CheckNeighbourGenerically(row, col - 1, pc, maze, targetMark, typeAlgo);
			// right
			if (col + 1 < MSZ && go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == targetMark || (typeAlgo == BIBFS_OPT && (maze[row][col + 1] == VISITED || maze[row][col + 1] == ORANGE))))
				go_on = CheckNeighbourGenerically(row, col + 1, pc, maze, targetMark, typeAlgo);
		}
	}
}
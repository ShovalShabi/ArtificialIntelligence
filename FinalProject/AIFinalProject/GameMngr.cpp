#include "GameMngr.h"
#include <iostream>
#include <math.h>



GameMngr::GameMngr(int numCoins)
{
	this->MonstersTurn = true;
	this->PacmanTurn = false;

	this->pacman = nullptr;
	this->coins = vector<Cell*>(); // all coins

	this->runPackman = false;

	this->coinsLeft = numCoins;
	this->coinsCounter = 0;
	this->runPacmanBFS = false;
	this->PackmanNeedToMove = true;

	this->pacmanGrays = vector<Cell*>();
	this->listOfPriorityQueues = vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>();
	this->ghostNeedToMoveList = vector<bool>();
	this->ghostStuckList = vector<bool>();
	this->ghostFoundPathList = vector<bool>();
	this->ghostCellStartPos = vector<Cell*>();
	this->ghostsList = vector<Cell*>();
}

void GameMngr::RestorePacmanPath(Cell* pc, Cell*& pacman, int ** maze)
{
	double distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	while (distanceBetweenPacmanToPc > 1) // iterate over path to next coin until next move of pacman
	{
		pc = pc->getParent();
		distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	}
	// move pacman one step
	maze[pacman->getRow()][pacman->getColumn()] = SPACE;
	maze[pc->getRow()][pc->getColumn()] = PACMAN;
	pacman->setRow(pc->getRow());
	pacman->setCol(pc->getColumn());
	PackmanNeedToMove = false; // bool flag of pacman movement set to false
	clearMaze(maze);
}


void GameMngr::CheckPacmanNeighbor(int row, int column, Cell* pcurrent, Cell*& pacman, int** maze)
{
	if (maze[row][column] == COIN) // if pacman found a coin to path -> move one step forward to it
	{
		RestorePacmanPath(pcurrent, pacman, maze);
		return;
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		pacmanGrays.push_back(pn);
		maze[row][column] = GRAY;
	}
}

void GameMngr::clearMaze(int** maze) {
	for (int i = 1; i < MSZ - 1; i++)
	{
		for (int j = 1; j < MSZ - 1; j++)
		{
			if (maze[i][j] == GRAY || maze[i][j] == BLACK || maze[i][j] == PATH)
			{
				maze[i][j] = SPACE;
			}
		}
	}
	for (Cell* cell : coins)// Paint again all coins in maze
	{
		maze[cell->getRow()][cell->getColumn()] = COIN;
	}
}

void GameMngr::runPacmanGame(int** maze)
{
	if (runPackman)
	{
		if (coinsLeft > 0)
		{
			GhostsIteration(pacman,maze);
			pacmanIteration(pacman, maze, ghostsList, pacmanGrays);
		}
		else
		{
			cout << "Pacman ate all the coins! Pacman won!\n";
		}
	}
	else
	{
		cout << "Game Over! Buy hey, Pacman collected " << coinsCounter << "!\n";
	}
}

void GameMngr::pacmanIteration(Cell*& pacman, int** maze, vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays)
{

	vector<double> distances;
	double minDistance = MSZ + 1;
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		distances.push_back(GetDistanceBetweenTwoDots(ghostsList[i]->getRow(), ghostsList[i]->getColumn(), pacman->getRow(), pacman->getColumn()));
	}

	int indexClosestGhost = 0;

	for (int i = 0; i < NUMGHOSTS; i++)
	{
		if (distances[i] < minDistance)
		{
			indexClosestGhost = i;
			minDistance = distances[i];
		}
	}

	if (minDistance <= PROXIMITY)
	{
		PacmanRunAwayGhost(pacman, ghostsList[indexClosestGhost], maze);
		clearMaze(maze);
		MonstersTurn = true;
		PacmanTurn = false;
		return; // move one step away and finish turn
	}

	// if getting here- there are no monsters close & pacman can chase coins
	// check if pacman found a coin next to him:
	int pacR = pacman->getRow(), pacC = pacman->getColumn();
	// check if coin is one step up from pacman
	if (maze[pacR + 1][pacC] == COIN)
	{
		checkIfCoin(pacR + 1, pacC, maze);// erase coin
		// move pacman Up
		maze[pacR][pacC] = SPACE;
		pacman->setRow(pacR + 1);
		pacman->setCol(pacC);
		maze[pacR + 1][pacC] = PACMAN;
		clearMaze(maze);
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is down step down from pacman
	else if (maze[pacR - 1][pacC] == COIN) {
		checkIfCoin(pacR - 1, pacC, maze);// erase coin
		// move pacman Down
		maze[pacR][pacC] = SPACE;
		pacman->setRow(pacR - 1);
		pacman->setCol(pacC);
		maze[pacR - 1][pacC] = PACMAN;
		clearMaze(maze);
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is one step right from pacman
	else if (maze[pacR][pacC + 1] == COIN) {
		checkIfCoin(pacR, pacC + 1, maze);// erase coin
		// move pacman Right
		maze[pacR][pacC] = SPACE;
		pacman->setRow(pacR);
		pacman->setCol(pacC + 1);
		maze[pacR][pacC + 1] = PACMAN;
		clearMaze(maze);
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is one step left from pacman
	else if (maze[pacR][pacC - 1] == COIN) {
		checkIfCoin(pacR, pacC - 1, maze);// erase coin
		// move pacman Left
		maze[pacR][pacC] = SPACE;
		pacman->setRow(pacR);
		pacman->setCol(pacC - 1);
		maze[pacR][pacC - 1] = PACMAN;
		clearMaze(maze);
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// if coins are further than 1 step - start BFS & move one step to first coin pacman found:
	PackmanNeedToMove = true;
	RunPacmanBFS(pacman, maze, pacmanGrays, &PackmanNeedToMove, &runPackman);

	MonstersTurn = true;
	PacmanTurn = false;
}


void GameMngr::checkIfCoin(int r, int c, int** maze)
{
	int i = 0;
	bool flag = false;
	for (Cell* coin : coins)
	{
		if (coin->getRow() == r && coin->getColumn() == c) // if we found coin
		{
			cout << "Pacman collected coin on location [" << r << "][" << c << "]!\n";
			coinsLeft--; // less coin remain
			coinsCounter++; // pacman collected another coin
			maze[r][c] = SPACE;
			coins.erase(coins.begin() + i); // remove the coin from coins vector
			flag = true;
		}
		i++;
	}
}

void GameMngr::RunPacmanBFS(Cell*& pacman, int** maze, vector<Cell*>& pacmanGrays, bool* PackmanNeedToMove, bool* runPackman)
{
	pacmanGrays.push_back(pacman);
	while (*PackmanNeedToMove)
	{
		Cell* pcurrent;
		int r, c;
		if (*PackmanNeedToMove)
		{
			if (pacmanGrays.empty()) // no solution exists
			{
				cout << "Pacman cannot reach more coins (blocked with walls)! Game Over!\n";
				*PackmanNeedToMove = false;
				*runPackman = false;
				clearMaze(maze);
				maze[pacman->getRow()][pacman->getColumn()] = PACMAN;
				return;
			}
			else // grays is not empty
			{
				pcurrent = *pacmanGrays.begin();
				r = pcurrent->getRow();
				c = pcurrent->getColumn();
				maze[r][c] = BLACK;
				pacmanGrays.erase(pacmanGrays.begin());
				// add all white neighbors of pcurrent to grays
				// UP
				if (*PackmanNeedToMove && (maze[r + 1][c] == SPACE || maze[r + 1][c] == COIN))
					CheckPacmanNeighbor(r + 1, c, pcurrent, pacman, maze);
				// DOWN
				if (*PackmanNeedToMove && (maze[r - 1][c] == SPACE || maze[r - 1][c] == COIN))
					CheckPacmanNeighbor(r - 1, c, pcurrent, pacman, maze);
				// LEFT
				if (*PackmanNeedToMove && (maze[r][c - 1] == SPACE || maze[r][c - 1] == COIN))
					CheckPacmanNeighbor(r, c - 1, pcurrent, pacman, maze);
				// RIGHT
				if (*PackmanNeedToMove && (maze[r][c + 1] == SPACE || maze[r][c + 1] == COIN))
					CheckPacmanNeighbor(r, c + 1, pcurrent, pacman, maze);
			}
		}
	}
}


void GameMngr::PacmanRunAwayGhost(Cell*& pacman, Cell* ghost, int** maze)
{
	int pacRow = pacman->getRow(), pacCol = pacman->getColumn();
	int ghostRow = ghost->getRow(), ghostCol = ghost->getColumn();
	// check which direction is the best for pacman to run away:
	double distanceIfMoveUp = GetDistanceBetweenTwoDots(pacRow + 1, pacCol, ghostRow, ghostCol);
	double distanceIfMoveDown = GetDistanceBetweenTwoDots(pacRow - 1, pacCol, ghostRow, ghostCol);
	double distanceIfMoveRight = GetDistanceBetweenTwoDots(pacRow, pacCol + 1, ghostRow, ghostCol);
	double distanceIfMoveLeft = GetDistanceBetweenTwoDots(pacRow, pacCol - 1, ghostRow, ghostCol);

	if (distanceIfMoveUp > distanceIfMoveDown && distanceIfMoveUp > distanceIfMoveRight && distanceIfMoveUp > distanceIfMoveLeft && (maze[pacRow + 1][pacCol] == SPACE || maze[pacRow + 1][pacCol] == COIN))
	{
		checkIfCoin(pacRow + 1, pacCol, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow + 1][pacCol] = PACMAN;
		pacman->setRow(pacRow + 1);
		pacman->setCol(pacCol);
		return;
	}
	else if (distanceIfMoveDown > distanceIfMoveUp && distanceIfMoveDown > distanceIfMoveRight && distanceIfMoveDown > distanceIfMoveLeft && (maze[pacRow - 1][pacCol] == SPACE || maze[pacRow - 1][pacCol] == COIN))
	{
		checkIfCoin(pacRow - 1, pacCol, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow - 1][pacCol] = PACMAN;
		pacman->setRow(pacRow - 1);
		pacman->setCol(pacCol);
		return;
	}
	else if (distanceIfMoveRight > distanceIfMoveDown && distanceIfMoveRight > distanceIfMoveUp && distanceIfMoveRight > distanceIfMoveLeft && (maze[pacRow][pacCol + 1] == SPACE || maze[pacRow][pacCol + 1] == COIN))
	{
		checkIfCoin(pacRow, pacCol + 1, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow][pacCol + 1] = PACMAN;
		pacman->setRow(pacRow);
		pacman->setCol(pacCol + 1);
		return;
	}
	else if (distanceIfMoveLeft > distanceIfMoveDown && distanceIfMoveLeft > distanceIfMoveRight && distanceIfMoveLeft > distanceIfMoveUp && (maze[pacRow][pacCol - 1] == SPACE || maze[pacRow][pacCol - 1] == COIN))
	{
		checkIfCoin(pacRow, pacCol - 1, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow][pacCol - 1] = PACMAN;
		pacman->setRow(pacRow);
		pacman->setCol(pacCol - 1);
		return;
	}
}


void GameMngr::GhostsIteration(Cell* pacman, int** maze)
{
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		if (ghostFoundPathList[i])// The game is still relevant
		{
			double distance = GetDistanceBetweenTwoDots(ghostsList[i]->getRow(), ghostsList[i]->getColumn(), pacman->getRow(), pacman->getColumn());
			if (distance > 1)
			{
				ghostNeedToMoveList[i] = true;
				GhostIteration(i, pacman, maze);
			}
			else
			{
				cout << "Monster " << i << " got pacman, GAME OVER!.\n";
				runPackman = false;
			}
		}
		else
		{
			justMove(ghostsList[i], maze);
		}
	}

	MonstersTurn = false;
	PacmanTurn = true;
}

// function to move any stuck monster on o
void GameMngr::justMove(Cell*& pc, int ** maze) {
	int r = pc->getRow(), c = pc->getColumn();
	// Move the monster the the open space	
	// UP
	if (maze[r + 1][c] == SPACE)
	{
		maze[r][c] = SPACE;
		maze[r + 1][c] = GHOST;
		pc->setRow(r + 1);
		pc->setCol(c);
	}
	// DOWN
	else if (maze[r - 1][c] == SPACE)
	{
		maze[r][c] = SPACE;
		maze[r - 1][c] = GHOST;
		pc->setRow(r - 1);
		pc->setCol(c);
	}
	// LEFT
	else if (maze[r][c - 1] == SPACE)
	{
		maze[r][c] = SPACE;
		maze[r][c - 1] = GHOST;
		pc->setRow(r);
		pc->setCol(c - 1);
	}
	// RIGHT
	else if (maze[r][c + 1] == SPACE)
	{
		maze[r][c] = SPACE;
		maze[r][c + 1] = GHOST;
		pc->setRow(r);
		pc->setCol(c + 1);
	}

}

void GameMngr::GhostIteration(int indexGhost, Cell* pacman, int** maze)
{
	if (ghostFoundPathList[indexGhost]) // if the ghost have path to pacman using A* algorithm
	{
		while (ghostNeedToMoveList[indexGhost])
		{
			Cell* pcurrent;
			int r, c;
			if (listOfPriorityQueues[indexGhost].empty()) // The ghost is blocked
			{
				ghostStuckList[indexGhost] = true;// the ghost doesn't have path to packaman
				ghostFoundPathList[indexGhost] = false;// the ghost doesn't have path to packaman
				ghostNeedToMoveList[indexGhost] = false; // Stopping the ghost movement
				clearMaze(maze);
				return;
			}
			else // grays is not empty
			{
				pcurrent = listOfPriorityQueues[indexGhost].top(); // get cell with cheapest cost (distance to start + distance to pacman)
				r = pcurrent->getRow();
				c = pcurrent->getColumn();
				listOfPriorityQueues[indexGhost].pop();
				// add all white neighbors of pcurrent to grays
				// UP
				if (ghostFoundPathList[indexGhost] && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACMAN || maze[r + 1][c] == COIN)
					CheckNeighborForGhost(r + 1, c, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// DOWN
				if (ghostFoundPathList[indexGhost] && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACMAN || maze[r - 1][c] == COIN)
					CheckNeighborForGhost(r - 1, c, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// LEFT
				if (ghostFoundPathList[indexGhost] && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACMAN || maze[r][c - 1] == COIN)
					CheckNeighborForGhost(r, c - 1, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// RIGHT
				if (ghostFoundPathList[indexGhost] && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACMAN || maze[r][c + 1] == COIN)
					CheckNeighborForGhost(r, c + 1, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
			}
		}
	}
}

void GameMngr::CheckNeighborForGhost(int row, int column, Cell*& pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze)
{
	if (maze[row][column] == PACMAN) // if found root to the pacman -> restore path to WHITE and move the monster 3 step
	{
		RestorePathForGhost(pcurrent, ghost, indexGhost, maze, pacman); // move one step
		return;
	}
	else // maze[row][column] is WHITE or maze[row][column] is COIN
	{
		double distanceCellFromStartMonster = GetDistanceBetweenTwoDots(row, column, ghostCellStartPos[indexGhost]->getRow(), ghostCellStartPos[indexGhost]->getColumn());
		double distancePackmanFromStartMonster = GetDistanceBetweenTwoDots(row, column, pacman->getRow(), pacman->getColumn());
		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster + distancePackmanFromStartMonster, pcurrent);
		// paint this neighbor gray
		maze[row][column] = GRAY;
		listOfPriorityQueues[indexGhost].push(pcAStar);
	}
}

void GameMngr::RestorePathForGhost(Cell*& pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman)
{
	while (pc->getParent() != ghost) // all path return to be SPACE instead of gray
	{
		pc = pc->getParent();
	}
	maze[ghost->getRow()][ghost->getColumn()] = SPACE; // remove old location of monster3
	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	ghost = pc;
	ghost->setRow(pc->getRow());
	ghost->setCol(pc->getColumn());
	maze[ghost->getRow()][ghost->getColumn()] = GHOST;
	Cell* cell;
	while (!listOfPriorityQueues[indexGhost].empty())
	{
		cell = listOfPriorityQueues[indexGhost].top();
		maze[cell->getRow()][cell->getColumn()] = SPACE;
		listOfPriorityQueues[indexGhost].pop();
	}
	listOfPriorityQueues[indexGhost].push(ghost);
	ghostNeedToMoveList[indexGhost] = false;
	clearMaze(maze);
}

// get distance between two dots
double GameMngr::GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2)
{
	return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}
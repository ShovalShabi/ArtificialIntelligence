#include "GameMngr.h"
#include "PacmanCollectCoinsState.h"
#include "PacmanRunFromGhostsState.h"
#include "PacmanStuckState.h"
#include "PacmanWonState.h"
#include <iostream>
#include <math.h>



GameMngr::GameMngr(int numCoins)
{
	this->currentState = new PacmanCollectCoinsState();
	this->currentState->OnEnter(this);

	this->pacman = nullptr;
	this->coins = vector<Cell*>(); // all coins

	this->runPackman = false;

	this->coinsLeft = numCoins;
	this->coinsCounter = 0;
	this->packmanNeedToCollectCoins = true;
	this->packmanRunFromGhosts = false;

	this->pacmanGrays = vector<Cell*>();
	this->listOfPriorityQueues = vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>();
	this->ghostNeedToMoveList = vector<bool>();
	this->ghostStuckList = vector<bool>();
	this->ghostFoundPathList = vector<bool>();
	this->ghostCellStartPos = vector<Cell*>();
	this->ghostsList = vector<Cell*>();
}

// Destructor to release resources
GameMngr::~GameMngr() {
	// Deallocate memory for currentState
	delete currentState;

	// Deallocate memory for pacmanGrays
	for (auto gray : pacmanGrays) {
		delete gray;
	}

	// Deallocate memory for ghostsList
	for (auto ghost : ghostsList) {
		delete ghost;
	}

	// Deallocate memory for ghostCellStartPos
	for (auto startPos : ghostCellStartPos) {
		delete startPos;
	}

	// Deallocate memory for priority_queues in listOfPriorityQueues
	for (auto& pq : listOfPriorityQueues) {
		while (!pq.empty()) {
			delete pq.top();
			pq.pop();
		}
	}

	// Print message indicating resources released
	std::cout << "All resources released." << std::endl;
}


void GameMngr::updateMaze(int** maze) {
	for (int i = 1; i < MSZ - 1; i++)
	{
		for (int j = 1; j < MSZ - 1; j++)
		{
			if (maze[i][j] == GRAY || maze[i][j] == BLACK || maze[i][j] == PATH || maze[i][j] == GRAY_PACMAN)
			{
				maze[i][j] = SPACE;
			}
		}
	}
	for (Cell* cell : coins)// Updading postions of the coins again in the maze
	{
		maze[cell->getRow()][cell->getColumn()] = COIN;
	}
	for (Cell* ghost : ghostsList)
	{
		maze[ghost->getRow()][ghost->getColumn()] = GHOST;
	}
	maze[pacman->getRow()][pacman->getColumn()] = PACMAN;
}

void GameMngr::runPacmanGame(int** maze)
{
	if (runPackman)
	{
		if (coinsLeft > 0)
		{
			GhostsIteration(pacman, maze);
			pacmanIteration(pacman, maze, ghostsList, pacmanGrays);
		}
		else
		{
			cout << "Pacman collected all the coins! Pacman won!\n";
			currentState->Transition(this, new PacmanWonState());
		}
	}
	else
	{
		cout << "Game Over! Bye Bye, Pacman collected " << coinsCounter << "!\n";
	}
}

void GameMngr::pacmanIteration(Cell* pacman, int** maze, vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays)
{

	vector<double> distances;
	double minDistance = MSZ + 1;
	for (int i = 0; i < NUM_GHOSTS; i++)
	{
		distances.push_back(GetDistanceBetweenTwoDots(ghostsList[i]->getRow(), ghostsList[i]->getColumn(), pacman->getRow(), pacman->getColumn()));
	}

	int indexClosestGhost = 0;

	for (int i = 0; i < NUM_GHOSTS; i++)
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
		updateMaze(maze);
		currentState->Transition(this, new PacmanRunFromGhostsState());
	}
	else
		currentState->Transition(this, new PacmanCollectCoinsState());

	if (packmanNeedToCollectCoins)
	{
		// if getting here- there are no monsters close & pacman can chase coins
		// check if pacman found a coin next to him:
		int pacR = pacman->getRow(), pacC = pacman->getColumn();
		// check if coin is one step up from pacman
		if (maze[pacR + 1][pacC] == COIN)
		{
			collectCoin(pacR + 1, pacC, maze);// erase coin
			// move pacman Up
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR + 1);
			pacman->setColumn(pacC);
			maze[pacR + 1][pacC] = PACMAN;
			updateMaze(maze);
			return;
		}
		// check if coin is down step down from pacman
		else if (maze[pacR - 1][pacC] == COIN) {
			collectCoin(pacR - 1, pacC, maze);// erase coin
			// move pacman Down
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR - 1);
			pacman->setColumn(pacC);
			maze[pacR - 1][pacC] = PACMAN;
			updateMaze(maze);
			return;
		}
		// check if coin is one step right from pacman
		else if (maze[pacR][pacC + 1] == COIN) {
			collectCoin(pacR, pacC + 1, maze);// erase coin
			// move pacman Right
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR);
			pacman->setColumn(pacC + 1);
			maze[pacR][pacC + 1] = PACMAN;
			updateMaze(maze);
			return;
		}
		// check if coin is one step left from pacman
		else if (maze[pacR][pacC - 1] == COIN) {
			collectCoin(pacR, pacC - 1, maze);// erase coin
			// move pacman Left
			maze[pacR][pacC] = SPACE;
			pacman->setRow(pacR);
			pacman->setColumn(pacC - 1);
			maze[pacR][pacC - 1] = PACMAN;
			updateMaze(maze);
			return;
		}
		// if coins are further than 1 step - start BFS & move one step to first coin pacman found:
		RunPacmanBFS(pacman, maze, pacmanGrays, &packmanNeedToCollectCoins, &runPackman);
	}
}


void GameMngr::collectCoin(int r, int c, int** maze)
{
	int i = 0;
	for (Cell* coin : coins)
	{
		if (coin->getRow() == r && coin->getColumn() == c) // if we found coin
		{
			cout << "Pacman collected coin on location [" << r << "][" << c << "]!\n";
			coinsLeft--; // less coin remain
			coinsCounter++; // pacman collected another coin
			maze[r][c] = SPACE;
			coins.erase(coins.begin() + i); // remove the coin from coins vector
		}
		i++;
	}
}

void GameMngr::RunPacmanBFS(Cell* pacman, int** maze, vector<Cell*>& pacmanGrays, bool* packmanNeedToCollectCoins, bool* runPackman)
{
	pacmanGrays.push_back(pacman);
	while (*packmanNeedToCollectCoins)
	{
		Cell* pcurrent;
		int r, c;
		if (*packmanNeedToCollectCoins)
		{
			if (pacmanGrays.empty()) // no solution exists
			{
				cout << "Pacman cannot reach more coins (blocked with walls)! Game Over!\n";
				*packmanNeedToCollectCoins = false;
				*runPackman = false;
				updateMaze(maze);
				maze[pacman->getRow()][pacman->getColumn()] = PACMAN;
				currentState->Transition(this, new PacmanStuckState());
			}
			else // grays is not empty
			{
				pcurrent = *pacmanGrays.begin();
				r = pcurrent->getRow();
				c = pcurrent->getColumn();
				maze[r][c] = BLACK;
				pacmanGrays.erase(pacmanGrays.begin());
				// add all white neighbors of pcurrent to grays
				// Pacman ignores the GRAY mark of the ghosts and continues to explore path
				// UP
				if (*packmanNeedToCollectCoins && (maze[r + 1][c] == SPACE || maze[r + 1][c] == COIN || maze[r + 1][c] == GRAY || maze[r + 1][c] == GHOST))
					CheckPacmanNeighbor(r + 1, c, pcurrent, pacman, maze);
				// DOWN
				if (*packmanNeedToCollectCoins && (maze[r - 1][c] == SPACE || maze[r - 1][c] == COIN || maze[r - 1][c] == GRAY || maze[r - 1][c] == GHOST))
					CheckPacmanNeighbor(r - 1, c, pcurrent, pacman, maze);
				// LEFT
				if (*packmanNeedToCollectCoins && (maze[r][c - 1] == SPACE || maze[r][c - 1] == COIN || maze[r][c - 1] == GRAY || maze[r][c - 1] == GHOST))
					CheckPacmanNeighbor(r, c - 1, pcurrent, pacman, maze);
				// RIGHT
				if (*packmanNeedToCollectCoins && (maze[r][c + 1] == SPACE || maze[r][c + 1] == COIN || maze[r][c + 1] == GRAY || maze[r][c + 1] == GHOST))
					CheckPacmanNeighbor(r, c + 1, pcurrent, pacman, maze);
			}
		}
	}
}

void GameMngr::RestorePacmanPath(Cell* pc, Cell* pacman, int** maze)
{
	double distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	pacmanGrays.clear();// Clearing irrelavent cells for path
	while (distanceBetweenPacmanToPc > 1) // iterate over path to next coin until next move of pacman
	{
		pacmanGrays.push_back(pc);//inserting back the relevant cell exceot the closest one to pacman
		pc = pc->getParent();
		distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	}
	// move pacman one step
	maze[pacman->getRow()][pacman->getColumn()] = SPACE;
	maze[pc->getRow()][pc->getColumn()] = PACMAN;
	pacman->setRow(pc->getRow());
	pacman->setColumn(pc->getColumn());
	packmanNeedToCollectCoins = false; // bool flag of pacman movement set to false
	updateMaze(maze);
}


void GameMngr::CheckPacmanNeighbor(int row, int column, Cell* pcurrent, Cell* pacman, int** maze)
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
		maze[row][column] = GRAY_PACMAN;
	}
}


void GameMngr::PacmanRunAwayGhost(Cell* pacman, Cell* ghost, int** maze)
{
	int pacRow = pacman->getRow(), pacCol = pacman->getColumn();
	int ghostRow = ghost->getRow(), ghostCol = ghost->getColumn();

	// check which direction is the best for pacman to run away in one step away
	vector<Cell*> cellPointers = {
		new Cell(pacRow + 1,pacCol,nullptr),
		new Cell(pacRow - 1,pacCol,nullptr),
		new Cell(pacRow,pacCol + 1,nullptr),
		new Cell(pacRow,pacCol - 1,nullptr) };

	double maxDistance = -1;
	int maxDistIndex = 0;
	for (int i = 0; i < (int)cellPointers.size(); i++)
	{
		double tempDist = GetDistanceBetweenTwoDots(cellPointers[i]->getRow(), cellPointers[i]->getColumn(), ghostRow, ghostCol);
		if (tempDist >= maxDistance)
		{
			maxDistance = tempDist;
			maxDistIndex = i;
		}
	}
	if (maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] == WALL ||
		maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] == GHOST)// in case of another ghost appears
		return;

	if (maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] == SPACE ||
		maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] == COIN ||
		maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] == GRAY)
	{
		collectCoin(cellPointers[maxDistIndex]->getRow(), cellPointers[maxDistIndex]->getColumn(), maze);
		maze[pacRow][pacCol] = SPACE; // Updating the current postion of pacman to an empty space
		maze[cellPointers[maxDistIndex]->getRow()][cellPointers[maxDistIndex]->getColumn()] = PACMAN; // updating the actual postion of pacman on board
		pacman->setRow(cellPointers[maxDistIndex]->getRow()); //updating the pacman object row position
		pacman->setColumn(cellPointers[maxDistIndex]->getColumn());  //updating the pacman object column position
	}
}


void GameMngr::GhostsIteration(Cell* pacman, int** maze)
{
	for (int i = 0; i < NUM_GHOSTS; i++)
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
				cout << "Ghost " << i << " got pacman, GAME OVER!\n";
				currentState->Transition(this, new PacmanStuckState());
			}
		}
		else
		{
			//The ghost got stuck so it won't move anymore
			ghostStuckList[i] = true;
			ghostFoundPathList[i] = false;
		}
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
				updateMaze(maze);
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
				if (ghostFoundPathList[indexGhost] && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACMAN || maze[r + 1][c] == COIN || maze[r + 1][c] == GRAY_PACMAN || maze[r + 1][c] == GHOST)
					CheckNeighborForGhost(r + 1, c, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// DOWN
				if (ghostFoundPathList[indexGhost] && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACMAN || maze[r - 1][c] == COIN || maze[r - 1][c] == GRAY_PACMAN || maze[r - 1][c] == GHOST)
					CheckNeighborForGhost(r - 1, c, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// LEFT
				if (ghostFoundPathList[indexGhost] && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACMAN || maze[r][c - 1] == COIN || maze[r][c - 1] == GRAY_PACMAN || maze[r][c - 1] == GHOST)
					CheckNeighborForGhost(r, c - 1, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
				// RIGHT
				if (ghostFoundPathList[indexGhost] && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACMAN || maze[r][c + 1] == COIN || maze[r][c + 1] == GRAY_PACMAN || maze[r][c + 1] == GHOST)
					CheckNeighborForGhost(r, c + 1, pcurrent, ghostsList[indexGhost], indexGhost, pacman, maze);
			}
		}
	}
}

void GameMngr::CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze)
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

void GameMngr::RestorePathForGhost(Cell* pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman)
{
	while (pc->getParent() != ghost) // all path return to be SPACE instead of gray
	{
		pc = pc->getParent();
	}
	maze[ghost->getRow()][ghost->getColumn()] = SPACE; // remove old location of monster3
	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), pacman->getRow(), pacman->getColumn());
	ghost = pc;
	ghost->setRow(pc->getRow());
	ghost->setColumn(pc->getColumn());
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
	updateMaze(maze);
}

// get distance between two dots
double GameMngr::GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2)
{
	return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}
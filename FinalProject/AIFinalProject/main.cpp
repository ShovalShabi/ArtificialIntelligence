
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <vector>
#include "Cell.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include "CompareCellsWithDistance.h"
#include <string> 

using namespace std;

const int W = 500; // window Width
const int H = 500; // window Height
const int MSZ = 25;

const int NUM_OF_COINS = 5;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4;
const int GRAY = 5;
const int PATH = 6;
const int COIN = 7;
const int PACKMAN = 8;
const int GHOST = 9;
const int TEMPPACMAN = 10;
const int NUMGHOSTS = 3;
const int PROXIMITY = 3;

bool MonstersTurn = true;
bool PacmanTurn = false;
int countingMoves = 0;
Cell* tempTargetToMonsters;
Cell* pathMonster1, pathMonster2, pathMonster3;


Cell* packman;
int startPacmanR, startPacmanC;
Cell* monster1;
int startRowMonster1, startColMonster1;
Cell* monster2;
int startRowMonster2, startColMonster2;
Cell* monster3;
int startRowMonster3, startColMonster3;
vector<Cell*> coins; // all coins

int** maze;
bool runBFS = false, runDFS = false, runBestFirstSearch = false, runAStar = false, runPackman = false;

bool monster1FoundPath = true, monster2FoundPath = true, monster3FoundPath = true; // if each monster found a path to packman
bool monster1Stuck = false, monster2Stuck = false, monster3Stuck = false; // if each monster is stuck - cannot reach to packman because of walls
bool monster1NeetToMove = true, monster2NeedToMove = true, monster3NeedToMove = true;
int coinsLeft = NUM_OF_COINS;
int coinsCounter = 0;
bool runPacmanBFS = false;


int startR, startC, targetR, targetC;

vector <Cell*> grays;
vector <Cell*> pacmanGrays;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForBestFirstSearch;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForAStar;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForMonster1;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForMonster2;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForMonster3;
priority_queue <Cell*, vector<Cell*>, CompareCellsWithDistance> pqForPacman;
vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>> listOfPriorityQueues(NUMGHOSTS);// PriorityQueues of each ghost
vector<bool> ghostNeedToMoveList(NUMGHOSTS, true);// Flag list to each ghost for movement
vector<bool> ghostStuckList(NUMGHOSTS, false);// Flag list to each ghost for checking if the ghost is stuck
vector<bool> ghostFoundPathList(NUMGHOSTS, true);// Flag list to each ghost for checking id the ghost is stuck
vector<Cell*> ghostCellStartPos;// List to each ghost initial postion



vector<Cell*> ghostsList;// Flag list to each ghost
Cell* generateUniqueCell(int** maze);// Meant for generating ghosts and packman





void InitMaze();
void fillWithZeros();
void runPackmanGame(Cell* packman);
void GhostsIteration(Cell* packman);
double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2);
bool PackmanNeedToMove = true;


void CheckNeighborForMonster1(int row, int column, Cell* pcurrent);
void Monster1Iteration();
void RestorePathForMonster1(Cell* pc);
void CheckNeighborForMonster2(int row, int column, Cell* pcurrent);
void Monster2Iteration();
void RestorePathForMonster2(Cell* pc);
void RestorePathForMonster3(Cell* pc);
void CheckNeighborForMonster3(int row, int column, Cell* pcurrent);
void Monster3Iteration();
void GhostIteration(int indexGhost, Cell* packman);
void CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell* ghost, int indexGhost, Cell* packman);
void RestorePathForGhost(Cell* pc, Cell* ghost, int indexGhost);
void clearMaze();
void justMove(Cell* pc); // if monster is stuck, just move it so it'll look real chase
void pacmanIteration(Cell* packman, int** maze, vector<Cell*> ghostsList);
void PacmanRunAwayGhost(Cell* packman, Cell* ghost, int** maze);
void pacmanRunAwayMonster1();
void pacmanRunAwayMonster2();
void pacmanRunAwayMonster3();
void checkIfCoin(int r, int c, int** maze);
void RunPacmanBFS(Cell* packman, int** maze, vector<Cell*> pacmanGrays, bool* PackmanNeedToMove, bool* runPackman);
void CheckPacmanNeighbor(int row, int column, Cell* pcurrent);
void RestorePacmanPath(Cell* pc);




void init()
{
	srand(time(0));
	glClearColor(0, 0.3, 0.0, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	InitMaze();
}
// get distance of given cell to target.
double getDistanceFromCellToTarget(Cell* cur)
{
	return sqrt(abs(pow(targetR - cur->getRow(), 2) + pow(targetC - cur->getColumn(), 2)));
}

// get distance of given cell from Start.
double getDistanceFromCellToStart(Cell* cur)
{
	return sqrt(abs(pow(startR - cur->getRow(), 2) + pow(startC - cur->getColumn(), 2)));
}



Cell* generateUniqueCell(int** maze)
{
	int x = rand() % MSZ;
	int y = rand() % MSZ;
	while (x == 0 || x == MSZ - 1)
		x = rand() % MSZ;
	while (y == 0 || y == MSZ - 1)
		y = rand() % MSZ;
	while (maze[x][y] != SPACE)
	{
		x = rand() % MSZ;
		y = rand() % MSZ;
	}
	return new Cell(x, y, nullptr);
}

void fillWithZeros()
{
	maze = new int* [MSZ];
	for (int i = 0; i < MSZ; ++i) {
		maze[i] = new int[MSZ];
	}

	// Initialize maze elements
	for (int i = 0; i < MSZ; ++i) {
		for (int j = 0; j < MSZ; ++j) {
		}
	}
}

void InitMaze()
{
	int i, j;
	fillWithZeros();
	// the frame of maze is made of walls
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}
	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 0) // mostly SPACES
			{
				if (rand() % 100 < 80)
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
			else //  mostly walls
			{
				if (rand() % 100 < 60)
					maze[i][j] = WALL;
				else
					maze[i][j] = SPACE;
			}
		}
	//Packman Creation
	int x, y;
	Cell* packman = generateUniqueCell(maze);

	x = packman->getRow();
	y = packman->getColumn();
	maze[x][y] = PACKMAN;
	cout << "Pacman location is [" << x << "][" << y << "]\n";

	//Initialization of the ghosts patameters
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		Cell* ghost = generateUniqueCell(maze);

		x = ghost->getRow();
		y = ghost->getColumn();
		maze[x][y] = GHOST;
		ghostCellStartPos.push_back(new Cell(x, y, nullptr));
		cout << "Ghost " << i << " location is [" << x << "][" << y << "]\n";
	}

	//Initialization of the coins patameters
	for (int i = 0; i < NUM_OF_COINS; i++)
	{
		Cell* coin = generateUniqueCell(maze);

		x = coin->getRow();
		y = coin->getColumn();
		maze[x][y] = COIN;
		cout << "Coin " << i << " location is [" << x << "][" << y << "]\n";
	}
	/*for (int i = 0; i < NUM_OF_COINS + 4; i++) // init players on the board
	{
		int x = rand() % MSZ;
		int y = rand() % MSZ;
		while (x == 0 || x == MSZ - 1)
			x = rand() % MSZ;
		while (y == 0 || y == MSZ - 1)
			y = rand() % MSZ;
		while (maze[x][y] != SPACE)
		{
			x = rand() % MSZ;
			y = rand() % MSZ;
		}
		if (i == 0) // init packman
		{
			maze[x][y] = PACKMAN;
			packman = new Cell(x, y, nullptr);
			tempTargetToMonsters = new Cell(x, y, nullptr); // temp target to all monsters
			startPacmanR = x;
			startPacmanC = y;
		}

		if (i == 1) // init monster 1
		{
			maze[x][y] = GHOST;
			startRowMonster1 = x;
			startColMonster1 = y;
			monster1 = new Cell(x, y, nullptr);
			pqForMonster1.push(monster1);
			cout << "Ghost "<< i <<" location is [" << x << "][" << y << "]\n";
		}
		if (i == 2) // init monster 2
		{
			maze[x][y] = GHOST;
			startRowMonster2 = x;
			startColMonster2 = y;
			monster2 = new Cell(x, y, nullptr);
			pqForMonster2.push(monster2);
			cout << "Monster 2 location is [" << x << "][" << y << "]\n";
		}
		if (i == 3) // init monster 3
		{
			maze[x][y] = GHOST;
			startRowMonster3 = x;
			startColMonster3 = y;
			monster3 = new Cell(x, y, nullptr);
			pqForMonster3.push(monster3);
			cout << "Monster 3 location is [" << x << "][" << y << "]\n";
		}
		if (i > 3)
		{
			maze[x][y] = COIN;
			coins.push_back(new Cell(x, y, nullptr));
			cout << "Coin location is [" << x << "][" << y << "]\n";
		}
	}*/
}

void DrawMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])
			{
			case SPACE:
				glColor3d(0.9, 0.9, 0.9); // light gray
				break;
			case WALL:
				glColor3d(0.3, 0.3, 0.3); // dark gray
				break;
			case START:
				glColor3d(0.5, 0.5, 0.9); // blue
				break;
			case TARGET:
				glColor3d(0, 1, 0); // green
				break;
			case BLACK:
				//glColor3d(0.9, 0.9, 0.9); // light gray
				glColor3d(0, 0, 0); // light gray
				break;
			case GRAY:
				glColor3d(0.9, 0.9, 0.9); // light gray
				break;
			case PATH:
				glColor3d(0.8, 0.5, 1); // purple
				break;
			case PACKMAN:
				glColor3d(0.0, 1.0, 0.0);// set color green
				break;
			case GHOST:
				glColor3d(1, 0, 0);// set color red
				break;
			case COIN:
				glColor3d(1, 1, 0);// set color yellow
				break;
			case TEMPPACMAN:
				glColor3d(0.9, 0.9, 0.9); // light gray
				break;

			}
			// draw square
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j + 1, i);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j, i + 1);
			glEnd();
		}

}

void RestorePath(Cell* pc)
{
	int numberOfSteps = 1;
	while (pc->getParent() != nullptr)
	{
		maze[pc->getRow()][pc->getColumn()] = PATH;
		pc = pc->getParent();
		numberOfSteps++;
	}
	string print = "Cells in shortest path: ";
	print += to_string(numberOfSteps);
	print += "\n";
	cout << print;
}

void RestorePacmanPath(Cell* pc)
{
	double distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
	while (distanceBetweenPacmanToPc > 1) // iterate over path to next coin until next move of pacman
	{
		pc = pc->getParent();
		distanceBetweenPacmanToPc = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
	}
	// move pacman one step
	maze[packman->getRow()][packman->getColumn()] = SPACE;
	maze[pc->getRow()][pc->getColumn()] = PACKMAN;
	packman->setRow(pc->getRow());
	packman->setCol(pc->getColumn());
	PackmanNeedToMove = false; // bool flag of pacman movement set to false
	clearMaze();
}

void CheckNeighbor(int row, int column, Cell* pcurrent)
{
	if (maze[row][column] == TARGET)
	{
		runBFS = false; // stop running BFS
		runDFS = false; // stop runnnig DFS
		runBestFirstSearch = false; // stop running best-first-search
		runAStar = false; // stop running A*
		cout << "The solution has been found!\n";
		RestorePath(pcurrent);
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		Cell* pcBestFirstSearch = new Cell(row, column, getDistanceFromCellToTarget(pn), pcurrent);
		Cell* pcAStar = new Cell(row, column, getDistanceFromCellToStart(pn) + getDistanceFromCellToTarget(pn), pcurrent);

		// paint this neighbor gray
		grays.push_back(pn);
		pqForBestFirstSearch.push(pcBestFirstSearch);
		pqForAStar.push(pcAStar);
		maze[row][column] = GRAY;
	}
}

void CheckPacmanNeighbor(int row, int column, Cell* pcurrent)
{
	if (maze[row][column] == COIN) // if pacman found a coin to path -> move one step forward to it
	{
		RestorePacmanPath(pcurrent);
		return;
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		pacmanGrays.push_back(pn);
		maze[row][column] = GRAY;
	}
}

//void RunAStarIteration()
//{
//	//while (!monster1Path)
//	//{
//	//	Cell* pcurrent;
//	//	int r, c;
//	//	if (pqForMonster1.empty()) // Monster 1 is blocked
//	//	{
//	//		cout << "Monster 1 is stuck.\n";
//	//		return;
//	//	}
//	//	else // grays is not empty
//	//	{
//	//		pcurrent = pqForMonster1.top(); // get cell with cheapest cost (distance to start + distance to pacman)
//	//		r = pcurrent->getRow();
//	//		c = pcurrent->getColumn();
//	//		pqForMonster1.pop();
//	//		// add all white neighbors of pcurrent to grays
//	//		// UP
//	//		if (maze[r + 1][c] == SPACE || maze[r + 1][c] == PACKMAN)
//	//			CheckNeighborForMonster1(r + 1, c, pcurrent);
//	//		// DOWN
//	//		if (maze[r - 1][c] == SPACE || maze[r - 1][c] == PACKMAN)
//	//			CheckNeighborForMonster1(r - 1, c, pcurrent);
//	//		// LEFT
//	//		if (maze[r][c - 1] == SPACE || maze[r][c - 1] == PACKMAN)
//	//			CheckNeighborForMonster1(r, c - 1, pcurrent);
//	//		// RIGHT
//	//		if (maze[r][c + 1] == SPACE || maze[r][c + 1] == PACKMAN)
//	//			CheckNeighborForMonster1(r, c + 1, pcurrent);
//	//	}
//	//}
//}

void clearMaze() {
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


void runPackmanGame(Cell* packman)
{
	if (runPackman)
	{
		if (coinsLeft > 0)
		{
			GhostsIteration(packman);
			pacmanIteration(packman, maze, ghostsList);
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

void pacmanIteration(Cell* packman, int** maze, vector<Cell*> ghostsList)
{
	//double distanceFromMonster1 = GetDistanceBetweenTwoDots(monster1->getRow(), monster1->getColumn(), packman->getRow(), packman->getColumn());  // distance from monster 1 to pacman at the moment
	//double distanceFromMonster2 = GetDistanceBetweenTwoDots(monster2->getRow(), monster2->getColumn(), packman->getRow(), packman->getColumn());  // distance from monster 2 to pacman at the moment
	//double distanceFromMonster3 = GetDistanceBetweenTwoDots(monster3->getRow(), monster3->getColumn(), packman->getRow(), packman->getColumn());  // distance from monster 3 to pacman at the moment

	vector<double> distances;
	int minDistance = MSZ + 1;
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		distances.push_back(GetDistanceBetweenTwoDots(ghostsList[i]->getRow(), ghostsList[i]->getColumn(), packman->getRow(), packman->getColumn()));
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
		PacmanRunAwayGhost(packman, ghostsList[indexClosestGhost], maze);
		clearMaze();
		MonstersTurn = true;
		PacmanTurn = false;
		return; // move one step away and finish turn
	}

	//
	//if (distanceFromMonster1 < distanceFromMonster2 && distanceFromMonster1 < distanceFromMonster3)
	//{
	//	if (distanceFromMonster1 < 3 && monster1FoundPath) {
	//		//cout << "Pacman need to run away from Monster1!\n";
	//		pacmanRunAwayMonster1();
	//		clearMaze();
	//		MonstersTurn = true;
	//		PacmanTurn = false;
	//		return; // move one step away and finish turn
	//	}
	//}
	//else if (distanceFromMonster2 < distanceFromMonster3)
	//{
	//	if (distanceFromMonster2 < 3 && monster2FoundPath) {
	//		//cout << "Pacman need to run away from Monster2!\n";
	//		pacmanRunAwayMonster2();
	//		clearMaze();
	//		MonstersTurn = true;
	//		PacmanTurn = false;
	//		return; // move one step away and finish turn
	//	}
	//}
	//else {
	//	if (distanceFromMonster3 < 3 && monster3FoundPath) {
	//		//cout << "Pacman need to run away from Monster3!\n";
	//		pacmanRunAwayMonster3();
	//		clearMaze();
	//		MonstersTurn = true;
	//		PacmanTurn = false;
	//		return; // move one step away and finish turn
	//	}
	//}
	// if getting here- there are no monsters close & pacman can chase coins
	// check if pacman found a coin next to him:
	int pacR = packman->getRow(), pacC = packman->getColumn();
	// check if coin is one step up from pacman
	if (maze[pacR + 1][pacC] == COIN)
	{
		checkIfCoin(pacR + 1, pacC, maze);// erase coin
		// move packman Up
		maze[pacR][pacC] = SPACE;
		packman->setRow(pacR + 1);
		packman->setCol(pacC);
		maze[pacR + 1][pacC] = PACKMAN;
		clearMaze();
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is down step down from pacman
	else if (maze[pacR - 1][pacC] == COIN) {
		checkIfCoin(pacR - 1, pacC, maze);// erase coin
		// move packman Down
		maze[pacR][pacC] = SPACE;
		packman->setRow(pacR - 1);
		packman->setCol(pacC);
		maze[pacR - 1][pacC] = PACKMAN;
		clearMaze();
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is one step right from pacman
	else if (maze[pacR][pacC + 1] == COIN) {
		checkIfCoin(pacR, pacC + 1, maze);// erase coin
		// move packman Right
		maze[pacR][pacC] = SPACE;
		packman->setRow(pacR);
		packman->setCol(pacC + 1);
		maze[pacR][pacC + 1] = PACKMAN;
		clearMaze();
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// check if coin is one step left from pacman
	else if (maze[pacR][pacC - 1] == COIN) {
		checkIfCoin(pacR, pacC - 1, maze);// erase coin
		// move packman Left
		maze[pacR][pacC] = SPACE;
		packman->setRow(pacR);
		packman->setCol(pacC - 1);
		maze[pacR][pacC - 1] = PACKMAN;
		clearMaze();
		MonstersTurn = true;
		PacmanTurn = false;
		return;
	}
	// if coins are further than 1 step - start BFS & move one step to first coin pacman found:
	PackmanNeedToMove = true;
	RunPacmanBFS(packman, maze, pacmanGrays, &PackmanNeedToMove, &runPackman);

	MonstersTurn = true;
	PacmanTurn = false;
}


void checkIfCoin(int r, int c, int** maze)
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

void RunPacmanBFS(Cell* packman, int** maze, vector<Cell*> pacmanGrays, bool* PackmanNeedToMove, bool* runPackman)
{
	pacmanGrays.push_back(packman);
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
				clearMaze();
				maze[packman->getRow()][packman->getColumn()] = PACKMAN;
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
					CheckPacmanNeighbor(r + 1, c, pcurrent);
				// DOWN
				if (*PackmanNeedToMove && (maze[r - 1][c] == SPACE || maze[r - 1][c] == COIN))
					CheckPacmanNeighbor(r - 1, c, pcurrent);
				// LEFT
				if (*PackmanNeedToMove && (maze[r][c - 1] == SPACE || maze[r][c - 1] == COIN))
					CheckPacmanNeighbor(r, c - 1, pcurrent);
				// RIGHT
				if (*PackmanNeedToMove && (maze[r][c + 1] == SPACE || maze[r][c + 1] == COIN))
					CheckPacmanNeighbor(r, c + 1, pcurrent);
			}
		}
	}
}

//void pacmanRunAwayMonster1()
//{
//	int pacR = packman->getRow(), pacC = packman->getColumn();
//	int monster1R = monster1->getRow(), monster1C = monster1->getColumn();
//	// check which direction is the best for pacman to run away:
//	double distanceIfMoveUp = GetDistanceBetweenTwoDots(pacR + 1, pacC, monster1R, monster1C);
//	double distanceIfMoveDown = GetDistanceBetweenTwoDots(pacR - 1, pacC, monster1R, monster1C);
//	double distanceIfMoveRight = GetDistanceBetweenTwoDots(pacR, pacC + 1, monster1R, monster1C);
//	double distanceIfMoveLeft = GetDistanceBetweenTwoDots(pacR, pacC - 1, monster1R, monster1C);
//
//	if (distanceIfMoveUp > distanceIfMoveDown && distanceIfMoveUp > distanceIfMoveRight && distanceIfMoveUp > distanceIfMoveLeft && (maze[pacR + 1][pacC] == SPACE || maze[pacR + 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR + 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR + 1][pacC] = PACKMAN;
//		packman->setRow(pacR + 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveDown > distanceIfMoveUp && distanceIfMoveDown > distanceIfMoveRight && distanceIfMoveDown > distanceIfMoveLeft && (maze[pacR - 1][pacC] == SPACE || maze[pacR - 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR - 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR - 1][pacC] = PACKMAN;
//		packman->setRow(pacR - 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveRight > distanceIfMoveDown && distanceIfMoveRight > distanceIfMoveUp && distanceIfMoveRight > distanceIfMoveLeft && (maze[pacR][pacC + 1] == SPACE || maze[pacR][pacC + 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC + 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC + 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC + 1);
//		return;
//	}
//	else if (distanceIfMoveLeft > distanceIfMoveDown && distanceIfMoveLeft > distanceIfMoveRight && distanceIfMoveLeft > distanceIfMoveUp && (maze[pacR][pacC - 1] == SPACE || maze[pacR][pacC - 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC - 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC - 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC - 1);
//		return;
//	}
//}
//
//void pacmanRunAwayMonster2()
//{
//	int pacR = packman->getRow(), pacC = packman->getColumn();
//	int monster2R = monster2->getRow(), monster2C = monster2->getColumn();
//	// check which direction is the best for pacman to run away:
//	double distanceIfMoveUp = GetDistanceBetweenTwoDots(pacR + 1, pacC, monster2R, monster2C);
//	double distanceIfMoveDown = GetDistanceBetweenTwoDots(pacR - 1, pacC, monster2R, monster2C);
//	double distanceIfMoveRight = GetDistanceBetweenTwoDots(pacR, pacC + 1, monster2R, monster2C);
//	double distanceIfMoveLeft = GetDistanceBetweenTwoDots(pacR, pacC - 1, monster2R, monster2C);
//
//	if (distanceIfMoveUp > distanceIfMoveDown && distanceIfMoveUp > distanceIfMoveRight && distanceIfMoveUp > distanceIfMoveLeft && (maze[pacR + 1][pacC] == SPACE || maze[pacR + 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR + 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR + 1][pacC] = PACKMAN;
//		packman->setRow(pacR + 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveDown > distanceIfMoveUp && distanceIfMoveDown > distanceIfMoveRight && distanceIfMoveDown > distanceIfMoveLeft && (maze[pacR - 1][pacC] == SPACE || maze[pacR - 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR - 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR - 1][pacC] = PACKMAN;
//		packman->setRow(pacR - 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveRight > distanceIfMoveDown && distanceIfMoveRight > distanceIfMoveUp && distanceIfMoveRight > distanceIfMoveLeft && (maze[pacR][pacC + 1] == SPACE || maze[pacR][pacC + 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC + 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC + 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC + 1);
//		return;
//	}
//	else if (distanceIfMoveLeft > distanceIfMoveDown && distanceIfMoveLeft > distanceIfMoveRight && distanceIfMoveLeft > distanceIfMoveUp && (maze[pacR][pacC - 1] == SPACE || maze[pacR][pacC - 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC - 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC - 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC - 1);
//		return;
//	}
//}
//
//void pacmanRunAwayMonster3()
//{
//	int pacR = packman->getRow(), pacC = packman->getColumn();
//	int monster3R = monster3->getRow(), monster3C = monster3->getColumn();
//	// check which direction is the best for pacman to run away:
//	double distanceIfMoveUp = GetDistanceBetweenTwoDots(pacR + 1, pacC, monster3R, monster3C);
//	double distanceIfMoveDown = GetDistanceBetweenTwoDots(pacR - 1, pacC, monster3R, monster3C);
//	double distanceIfMoveRight = GetDistanceBetweenTwoDots(pacR, pacC + 1, monster3R, monster3C);
//	double distanceIfMoveLeft = GetDistanceBetweenTwoDots(pacR, pacC - 1, monster3R, monster3C);
//
//	if (distanceIfMoveUp > distanceIfMoveDown && distanceIfMoveUp > distanceIfMoveRight && distanceIfMoveUp > distanceIfMoveLeft && (maze[pacR + 1][pacC] == SPACE || maze[pacR + 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR + 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR + 1][pacC] = PACKMAN;
//		packman->setRow(pacR + 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveDown > distanceIfMoveUp && distanceIfMoveDown > distanceIfMoveRight && distanceIfMoveDown > distanceIfMoveLeft && (maze[pacR - 1][pacC] == SPACE || maze[pacR - 1][pacC] == COIN))
//	{
//		checkIfCoin(pacR - 1, pacC);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR - 1][pacC] = PACKMAN;
//		packman->setRow(pacR - 1);
//		packman->setCol(pacC);
//		return;
//	}
//	else if (distanceIfMoveRight > distanceIfMoveDown && distanceIfMoveRight > distanceIfMoveUp && distanceIfMoveRight > distanceIfMoveLeft && (maze[pacR][pacC + 1] == SPACE || maze[pacR][pacC + 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC + 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC + 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC + 1);
//		return;
//	}
//	else if (distanceIfMoveLeft > distanceIfMoveDown && distanceIfMoveLeft > distanceIfMoveRight && distanceIfMoveLeft > distanceIfMoveUp && (maze[pacR][pacC - 1] == SPACE || maze[pacR][pacC - 1] == COIN))
//	{
//		checkIfCoin(pacR, pacC - 1);
//		maze[pacR][pacC] = SPACE;
//		maze[pacR][pacC - 1] = PACKMAN;
//		packman->setRow(pacR);
//		packman->setCol(pacC - 1);
//		return;
//	}
//}

void PacmanRunAwayGhost(Cell* packman, Cell* ghost, int** maze)
{
	int pacRow = packman->getRow(), pacCol = packman->getColumn();
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
		maze[pacRow + 1][pacCol] = PACKMAN;
		packman->setRow(pacRow + 1);
		packman->setCol(pacCol);
		return;
	}
	else if (distanceIfMoveDown > distanceIfMoveUp && distanceIfMoveDown > distanceIfMoveRight && distanceIfMoveDown > distanceIfMoveLeft && (maze[pacRow - 1][pacCol] == SPACE || maze[pacRow - 1][pacCol] == COIN))
	{
		checkIfCoin(pacRow - 1, pacCol, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow - 1][pacCol] = PACKMAN;
		packman->setRow(pacRow - 1);
		packman->setCol(pacCol);
		return;
	}
	else if (distanceIfMoveRight > distanceIfMoveDown && distanceIfMoveRight > distanceIfMoveUp && distanceIfMoveRight > distanceIfMoveLeft && (maze[pacRow][pacCol + 1] == SPACE || maze[pacRow][pacCol + 1] == COIN))
	{
		checkIfCoin(pacRow, pacCol + 1, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow][pacCol + 1] = PACKMAN;
		packman->setRow(pacRow);
		packman->setCol(pacCol + 1);
		return;
	}
	else if (distanceIfMoveLeft > distanceIfMoveDown && distanceIfMoveLeft > distanceIfMoveRight && distanceIfMoveLeft > distanceIfMoveUp && (maze[pacRow][pacCol - 1] == SPACE || maze[pacRow][pacCol - 1] == COIN))
	{
		checkIfCoin(pacRow, pacCol - 1, maze);
		maze[pacRow][pacCol] = SPACE;
		maze[pacRow][pacCol - 1] = PACKMAN;
		packman->setRow(pacRow);
		packman->setCol(pacCol - 1);
		return;
	}
}


void GhostsIteration(Cell* packman)
{
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		if (ghostFoundPathList[i])// The game is still relevant
		{
			double distance = GetDistanceBetweenTwoDots(ghostsList[i]->getRow(), ghostsList[i]->getColumn(), packman->getRow(), packman->getColumn());
			if (distance > 1)
			{
				ghostNeedToMoveList[i] = true;
				GhostIteration(i, packman);
			}
			else
			{
				cout << "Monster " << i << " got packman, GAME OVER!.\n";
				runPackman = false;
			}
		}
		else
		{
			justMove(ghostsList[i]);
		}
	}
	//if (monster1FoundPath || monster2FoundPath || monster3FoundPath) // game still on
	//{
	//	if (monster1FoundPath)
	//	{
	//		double distanceMonster1 = GetDistanceBetweenTwoDots(monster1->getRow(), monster1->getColumn(), packman->getRow(), packman->getColumn());
	//		if (distanceMonster1 > 1)
	//		{
	//			monster1NeetToMove = true;
	//			Monster1Iteration();
	//		}
	//		else
	//		{
	//			cout << "Monster 1 got packman, GAME OVER!.\n";
	//			runPackman = false;
	//		}

	//	}
	//	else
	//	{
	//		justMove(monster1);
	//	}
	//	if (monster2FoundPath)
	//	{
	//		double distanceMonster2 = GetDistanceBetweenTwoDots(monster2->getRow(), monster2->getColumn(), packman->getRow(), packman->getColumn());
	//		if (distanceMonster2 > 1)
	//		{
	//			monster2NeedToMove = true;
	//			Monster2Iteration();
	//		}
	//		else
	//		{
	//			cout << "Monster 2 got packman, GAME OVER!.\n";
	//			runPackman = false;
	//		}

	//	}
	//	else
	//	{
	//		justMove(monster2);
	//	}
	//	if (monster3FoundPath)
	//	{
	//		double distanceMonster3 = GetDistanceBetweenTwoDots(monster3->getRow(), monster3->getColumn(), packman->getRow(), packman->getColumn());
	//		if (distanceMonster3 > 1)
	//		{
	//			monster3NeedToMove = true;
	//			Monster3Iteration();
	//		}
	//		else
	//		{
	//			cout << "Monster 3 got packman, GAME OVER!.\n";
	//			runPackman = false;
	//		}

	//	}
	//	else
	//	{
	//		justMove(monster3);
	//	}
	//}

	MonstersTurn = false;
	PacmanTurn = true;
}

// function to move any stuck monster on o
void justMove(Cell* pc) {
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


//void Monster1Iteration()
//{
//	if (monster1FoundPath) // if monster 1 have path to packman using A* algorithm (not stuck)
//	{
//		while (monster1NeetToMove)
//		{
//			Cell* pcurrent;
//			int r, c;
//			if (pqForMonster1.empty()) // Monster 1 is blocked
//			{
//				monster1Stuck = true; // monster 1 doesn't have path to packaman
//				monster1FoundPath = false;  // monster 1 doesn't have path to packaman
//				monster1NeetToMove = false;
//				clearMaze();
//				return;
//			}
//			else // grays is not empty
//			{
//				pcurrent = pqForMonster1.top(); // get cell with cheapest cost (distance to start + distance to pacman)
//				r = pcurrent->getRow();
//				c = pcurrent->getColumn();
//				pqForMonster1.pop();
//				// add all white neighbors of pcurrent to grays
//				// UP
//				if (monster1FoundPath && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACKMAN || maze[r + 1][c] == COIN)
//					CheckNeighborForMonster1(r + 1, c, pcurrent);
//				// DOWN
//				if (monster1FoundPath && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACKMAN || maze[r - 1][c] == COIN)
//					CheckNeighborForMonster1(r - 1, c, pcurrent);
//				// LEFT
//				if (monster1FoundPath && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACKMAN || maze[r][c - 1] == COIN)
//					CheckNeighborForMonster1(r, c - 1, pcurrent);
//				// RIGHT
//				if (monster1FoundPath && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACKMAN || maze[r][c + 1] == COIN)
//					CheckNeighborForMonster1(r, c + 1, pcurrent);
//			}
//		}
//	}
//}
//
//void CheckNeighborForMonster1(int row, int column, Cell* pcurrent)
//{
//	if (maze[row][column] == PACKMAN) // if found route to the packman -> restore path to WHITE and move the monster 1 step
//	{
//		RestorePathForMonster1(pcurrent); // move one step
//		return;
//	}
//	else // maze[row][column] is WHITE or maze[row][column] is COIN
//	{
//		Cell* pn = new Cell(row, column, pcurrent);
//		double distanceCellFromStartMonster1 = GetDistanceBetweenTwoDots(row, column, startRowMonster1, startColMonster1);
//		double distancePackmanFromStartMonster1 = GetDistanceBetweenTwoDots(row, column, packman->getRow(), packman->getColumn());
//		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster1 + distancePackmanFromStartMonster1, pcurrent);
//		// paint this neighbor gray
//		maze[row][column] = GRAY;
//		pqForMonster1.push(pcAStar);
//	}
//}
//
//void RestorePathForMonster1(Cell* pc)
//{
//	while (pc->getParent() != monster1) // all path return to be SPACE instead of gray
//	{
//		pc = pc->getParent();
//	}
//	maze[monster1->getRow()][monster1->getColumn()] = SPACE; // remove old location of monster1
//	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
//	monster1 = pc;
//	monster1->setRow(pc->getRow());
//	monster1->setCol(pc->getColumn());
//	maze[monster1->getRow()][monster1->getColumn()] = GHOST;
//	Cell* cell;
//	while (!pqForMonster1.empty())
//	{
//		cell = pqForMonster1.top();
//		maze[cell->getRow()][cell->getColumn()] = SPACE;
//		pqForMonster1.pop();
//	}
//	pqForMonster1.push(monster1);
//	monster1NeetToMove = false;
//	clearMaze();
//}

//void Monster2Iteration()
//{
//	if (monster2FoundPath) // if monster 2 have path to packman using A* algorithm
//	{
//		while (monster2NeedToMove)
//		{
//			Cell* pcurrent;
//			int r, c;
//			if (pqForMonster2.empty()) // Monster 2 is blocked
//			{
//				monster2Stuck = true; // monster 2 doesn't have path to packaman
//				monster2FoundPath = false;  // monster 2 doesn't have path to packaman
//				monster2NeedToMove = false;
//				clearMaze();
//				return;
//			}
//			else // grays is not empty
//			{
//				pcurrent = pqForMonster2.top(); // get cell with cheapest cost (distance to start + distance to pacman)
//				r = pcurrent->getRow();
//				c = pcurrent->getColumn();
//				pqForMonster2.pop();
//				// add all white neighbors of pcurrent to grays
//				// UP
//				if (monster2FoundPath && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACKMAN || maze[r + 1][c] == COIN)
//					CheckNeighborForMonster2(r + 1, c, pcurrent);
//				// DOWN
//				if (monster2FoundPath && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACKMAN || maze[r - 1][c] == COIN)
//					CheckNeighborForMonster2(r - 1, c, pcurrent);
//				// LEFT
//				if (monster2FoundPath && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACKMAN || maze[r][c - 1] == COIN)
//					CheckNeighborForMonster2(r, c - 1, pcurrent);
//				// RIGHT
//				if (monster2FoundPath && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACKMAN || maze[r][c + 1] == COIN)
//					CheckNeighborForMonster2(r, c + 1, pcurrent);
//			}
//		}
//	}
//}
//
//void CheckNeighborForMonster2(int row, int column, Cell* pcurrent)
//{
//	if (maze[row][column] == PACKMAN) // if found root to the packman -> restore path to WHITE and move the monster 2 step
//	{
//		RestorePathForMonster2(pcurrent); // move one step
//		return;
//	}
//	else // maze[row][column] is WHITE or maze[row][column] is COIN
//	{
//		Cell* pn = new Cell(row, column, pcurrent);
//		double distanceCellFromStartMonster2 = GetDistanceBetweenTwoDots(row, column, startRowMonster2, startColMonster2);
//		double distancePackmanFromStartMonster2 = GetDistanceBetweenTwoDots(row, column, packman->getRow(), packman->getColumn());
//		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster2 + distancePackmanFromStartMonster2, pcurrent);
//		// paint this neighbor gray
//		maze[row][column] = GRAY;
//		pqForMonster2.push(pcAStar);
//	}
//}
//
//void RestorePathForMonster2(Cell* pc)
//{
//	while (pc->getParent() != monster2) // all path return to be SPACE instead of gray
//	{
//		pc = pc->getParent();
//	}
//	maze[monster2->getRow()][monster2->getColumn()] = SPACE; // remove old location of monster2
//	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
//	monster2 = pc;
//	monster2->setRow(pc->getRow());
//	monster2->setCol(pc->getColumn());
//	maze[monster2->getRow()][monster2->getColumn()] = GHOST;
//	Cell* cell;
//	while (!pqForMonster2.empty())
//	{
//		cell = pqForMonster2.top();
//		maze[cell->getRow()][cell->getColumn()] = SPACE;
//		pqForMonster2.pop();
//	}
//	pqForMonster2.push(monster2);
//	monster2NeedToMove = false;
//	clearMaze();
//}

//void Monster3Iteration()
//{
//	if (monster3FoundPath) // if monster 3 have path to packman using A* algorithm
//	{
//		while (monster3NeedToMove)
//		{
//			Cell* pcurrent;
//			int r, c;
//			if (pqForMonster3.empty()) // Monster 3 is blocked
//			{
//				monster3Stuck = true; // monster 3 doesn't have path to packaman
//				monster3FoundPath = false;  // monster 3 doesn't have path to packaman
//				monster3NeedToMove = false;
//				clearMaze();
//				return;
//			}
//			else // grays is not empty
//			{
//				pcurrent = pqForMonster3.top(); // get cell with cheapest cost (distance to start + distance to pacman)
//				r = pcurrent->getRow();
//				c = pcurrent->getColumn();
//				pqForMonster3.pop();
//				// add all white neighbors of pcurrent to grays
//				// UP
//				if (monster3FoundPath && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACKMAN || maze[r + 1][c] == COIN)
//					CheckNeighborForMonster3(r + 1, c, pcurrent);
//				// DOWN
//				if (monster3FoundPath && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACKMAN || maze[r - 1][c] == COIN)
//					CheckNeighborForMonster3(r - 1, c, pcurrent);
//				// LEFT
//				if (monster3FoundPath && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACKMAN || maze[r][c - 1] == COIN)
//					CheckNeighborForMonster3(r, c - 1, pcurrent);
//				// RIGHT
//				if (monster3FoundPath && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACKMAN || maze[r][c + 1] == COIN)
//					CheckNeighborForMonster3(r, c + 1, pcurrent);
//			}
//		}
//	}
//}

void GhostIteration(int indexGhost, Cell* packman)
{
	if (ghostFoundPathList[indexGhost]) // if the ghost have path to packman using A* algorithm
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
				monster3Stuck = true;
				monster3FoundPath = false;  // the ghost doesn't have path to packaman
				monster3NeedToMove = false;
				clearMaze();
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
				if (ghostFoundPathList[indexGhost] && maze[r + 1][c] == SPACE || maze[r + 1][c] == PACKMAN || maze[r + 1][c] == COIN)
					CheckNeighborForGhost(r + 1, c, pcurrent, ghostsList[indexGhost], indexGhost, packman);
				// DOWN
				if (ghostFoundPathList[indexGhost] && maze[r - 1][c] == SPACE || maze[r - 1][c] == PACKMAN || maze[r - 1][c] == COIN)
					CheckNeighborForGhost(r - 1, c, pcurrent, ghostsList[indexGhost], indexGhost, packman);
				// LEFT
				if (ghostFoundPathList[indexGhost] && maze[r][c - 1] == SPACE || maze[r][c - 1] == PACKMAN || maze[r][c - 1] == COIN)
					CheckNeighborForGhost(r, c - 1, pcurrent, ghostsList[indexGhost], indexGhost, packman);
				// RIGHT
				if (ghostFoundPathList[indexGhost] && maze[r][c + 1] == SPACE || maze[r][c + 1] == PACKMAN || maze[r][c + 1] == COIN)
					CheckNeighborForGhost(r, c + 1, pcurrent, ghostsList[indexGhost], indexGhost, packman);
			}
		}
	}
}

//void CheckNeighborForMonster3(int row, int column, Cell* pcurrent)
//{
//	if (maze[row][column] == PACKMAN) // if found root to the packman -> restore path to WHITE and move the monster 3 step
//	{
//		RestorePathForMonster3(pcurrent); // move one step
//		return;
//	}
//	else // maze[row][column] is WHITE or maze[row][column] is COIN
//	{
//		Cell* pn = new Cell(row, column, pcurrent);
//		double distanceCellFromStartMonster3 = GetDistanceBetweenTwoDots(row, column, startRowMonster3, startColMonster3);
//		double distancePackmanFromStartMonster3 = GetDistanceBetweenTwoDots(row, column, packman->getRow(), packman->getColumn());
//		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster3 + distancePackmanFromStartMonster3, pcurrent);
//		// paint this neighbor gray
//		maze[row][column] = GRAY;
//		pqForMonster3.push(pcAStar);
//	}
//}

void CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell* ghost, int indexGhost, Cell* packman)
{
	if (maze[row][column] == PACKMAN) // if found root to the packman -> restore path to WHITE and move the monster 3 step
	{
		RestorePathForGhost(pcurrent, ghost, indexGhost); // move one step
		return;
	}
	else // maze[row][column] is WHITE or maze[row][column] is COIN
	{
		Cell* pn = new Cell(row, column, pcurrent);
		double distanceCellFromStartMonster = GetDistanceBetweenTwoDots(row, column, ghostCellStartPos[indexGhost]->getRow(), ghostCellStartPos[indexGhost]->getColumn());
		double distancePackmanFromStartMonster = GetDistanceBetweenTwoDots(row, column, packman->getRow(), packman->getColumn());
		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster + distancePackmanFromStartMonster, pcurrent);
		// paint this neighbor gray
		maze[row][column] = GRAY;
		listOfPriorityQueues[indexGhost].push(pcAStar);
	}
}

//void RestorePathForMonster3(Cell* pc)
//{
//	while (pc->getParent() != monster3) // all path return to be SPACE instead of gray
//	{
//		pc = pc->getParent();
//	}
//	maze[monster3->getRow()][monster3->getColumn()] = SPACE; // remove old location of monster2
//	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
//	monster3 = pc;
//	monster3->setRow(pc->getRow());
//	monster3->setCol(pc->getColumn());
//	maze[monster3->getRow()][monster3->getColumn()] = GHOST;
//	Cell* cell;
//	while (!pqForMonster3.empty())
//	{
//		cell = pqForMonster3.top();
//		maze[cell->getRow()][cell->getColumn()] = SPACE;
//		pqForMonster3.pop();
//	}
//	pqForMonster3.push(monster3);
//	monster3NeedToMove = false;
//	clearMaze();
//}

void RestorePathForGhost(Cell* pc, Cell* ghost, int indexGhost)
{
	while (pc->getParent() != ghost) // all path return to be SPACE instead of gray
	{
		pc = pc->getParent();
	}
	maze[ghost->getRow()][ghost->getColumn()] = SPACE; // remove old location of monster3
	double distanceToPacMan = GetDistanceBetweenTwoDots(pc->getRow(), pc->getColumn(), packman->getRow(), packman->getColumn());
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
	clearMaze();
}

// get distance between two dots
double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2)
{
	return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}



void idle()
{
	if (runPackman)
		runPackmanGame(packman);

	glutPostRedisplay(); // indirect call to display
}


void menu(int choice) {
	switch (choice) {
	case 1: // start packman
		cout << "##### Packman started! #####\n";
		runPackman = true;
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(400, 50);
	glutCreateWindow("HW3_SaarGamzo_PACMAN");

	glutDisplayFunc(display); // display is the refresh function
	glutIdleFunc(idle);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Run Packman Game", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();

	return 0;
}
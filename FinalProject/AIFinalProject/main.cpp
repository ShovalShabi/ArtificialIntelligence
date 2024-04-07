
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
#include <chrono>
#include <thread>

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


Cell* packman = nullptr;
vector<Cell*> coins; // all coins

int** maze;
bool runPackman = false;

int coinsLeft = NUM_OF_COINS;
int coinsCounter = 0;
bool runPacmanBFS = false;
bool PackmanNeedToMove = true;


vector <Cell*> pacmanGrays;
vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>> listOfPriorityQueues;// PriorityQueues of each ghost
vector<bool> ghostNeedToMoveList;// Flag list to each ghost for movement
vector<bool> ghostStuckList;// Flag list to each ghost for checking if the ghost is stuck
vector<bool> ghostFoundPathList;// Flag list to each ghost for checking id the ghost is stuck
vector<Cell*> ghostCellStartPos;// List to each ghost initial postion
vector<Cell*> ghostsList;// Flag list to each ghost

Cell* generateUniqueCell(int** maze);// Meant for generating ghosts and packman
void InitMaze(Cell*& packman);
void fillWithZeros();
void runPackmanGame(Cell* packman);
void GhostsIteration(Cell* packman);
double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2);
void GhostIteration(int indexGhost, Cell* packman);
void CheckNeighborForGhost(int row, int column, Cell*& pcurrent, Cell*& ghost, int indexGhost, Cell* packman);
void RestorePathForGhost(Cell*& pc, Cell*& ghost, int indexGhost);
void clearMaze();
void justMove(Cell*& pc); // if monster is stuck, just move it so it'll look real chase
void pacmanIteration(Cell*& packman, int** maze, vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays);
void PacmanRunAwayGhost(Cell*& packman, Cell* ghost, int** maze);
void checkIfCoin(int r, int c, int** maze);
void RunPacmanBFS(Cell*& packman, int** maze, vector<Cell*>& pacmanGrays, bool* PackmanNeedToMove, bool* runPackman);
void CheckPacmanNeighbor(int row, int column, Cell* pcurrent);
void RestorePacmanPath(Cell* pc, Cell*& packman);




void init()
{
	srand(time(0));
	glClearColor(0, 0.3, 0.0, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	InitMaze(packman);
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

void InitMaze(Cell*& packman)
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
	packman = generateUniqueCell(maze);

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
		listOfPriorityQueues.push_back(priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>());// PriorityQueues of each ghost
		listOfPriorityQueues[i].push(ghost);
		ghostNeedToMoveList.push_back(true);// Flag list to each ghost for movement
		ghostStuckList.push_back(false);// Flag list to each ghost for checking if the ghost is stuck
		ghostFoundPathList.push_back(true);// Flag list to each ghost for checking id the ghost is stuck
		ghostsList.push_back(ghost);
		cout << "Ghost " << i << " location is [" << x << "][" << y << "]\n";
	}

	//Initialization of the coins patameters
	for (int i = 0; i < NUM_OF_COINS; i++)
	{
		Cell* coin = generateUniqueCell(maze);

		x = coin->getRow();
		y = coin->getColumn();
		maze[x][y] = COIN;
		coins.push_back(coin);
		cout << "Coin " << i << " location is [" << x << "][" << y << "]\n";
	}
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

void RestorePacmanPath(Cell* pc, Cell*& packman)
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


void CheckPacmanNeighbor(int row, int column, Cell* pcurrent, Cell*& packman)
{
	if (maze[row][column] == COIN) // if pacman found a coin to path -> move one step forward to it
	{
		RestorePacmanPath(pcurrent, packman);
		return;
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		pacmanGrays.push_back(pn);
		maze[row][column] = GRAY;
	}
}

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
			pacmanIteration(packman, maze, ghostsList, pacmanGrays);
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

void pacmanIteration(Cell*& packman, int** maze, vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays)
{

	vector<double> distances;
	double minDistance = MSZ + 1;
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

void RunPacmanBFS(Cell*& packman, int** maze, vector<Cell*>& pacmanGrays, bool* PackmanNeedToMove, bool* runPackman)
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
					CheckPacmanNeighbor(r + 1, c, pcurrent, packman);
				// DOWN
				if (*PackmanNeedToMove && (maze[r - 1][c] == SPACE || maze[r - 1][c] == COIN))
					CheckPacmanNeighbor(r - 1, c, pcurrent, packman);
				// LEFT
				if (*PackmanNeedToMove && (maze[r][c - 1] == SPACE || maze[r][c - 1] == COIN))
					CheckPacmanNeighbor(r, c - 1, pcurrent, packman);
				// RIGHT
				if (*PackmanNeedToMove && (maze[r][c + 1] == SPACE || maze[r][c + 1] == COIN))
					CheckPacmanNeighbor(r, c + 1, pcurrent, packman);
			}
		}
	}
}


void PacmanRunAwayGhost(Cell*& packman, Cell* ghost, int** maze)
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

	MonstersTurn = false;
	PacmanTurn = true;
}

// function to move any stuck monster on o
void justMove(Cell*& pc) {
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

void CheckNeighborForGhost(int row, int column, Cell*& pcurrent, Cell*& ghost, int indexGhost, Cell* packman)
{
	if (maze[row][column] == PACKMAN) // if found root to the packman -> restore path to WHITE and move the monster 3 step
	{
		RestorePathForGhost(pcurrent, ghost, indexGhost); // move one step
		return;
	}
	else // maze[row][column] is WHITE or maze[row][column] is COIN
	{
		double distanceCellFromStartMonster = GetDistanceBetweenTwoDots(row, column, ghostCellStartPos[indexGhost]->getRow(), ghostCellStartPos[indexGhost]->getColumn());
		double distancePackmanFromStartMonster = GetDistanceBetweenTwoDots(row, column, packman->getRow(), packman->getColumn());
		Cell* pcAStar = new Cell(row, column, distanceCellFromStartMonster + distancePackmanFromStartMonster, pcurrent);
		// paint this neighbor gray
		maze[row][column] = GRAY;
		listOfPriorityQueues[indexGhost].push(pcAStar);
	}
}

void RestorePathForGhost(Cell*& pc, Cell*& ghost, int indexGhost)
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

	this_thread::sleep_for(chrono::milliseconds(500)); // Adjust the delay time as needed


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
	glutCreateWindow("Packman Final Project");

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
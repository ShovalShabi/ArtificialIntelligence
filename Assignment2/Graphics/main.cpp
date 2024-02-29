
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <vector>
#include "Cell.h"
#include <iostream>
#include <algorithm>

using namespace std;

const int W = 500; // window Width
const int H = 500; // window Height
const int MSZ = 100;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4;
const int GRAY = 5;
const int PATH = 6;

int maze[MSZ][MSZ] = {0};
bool runBFS = false, runDFS = false, runBestFirstSearch = false, runAStar = false;

int startR, startC, targetR, targetC;

vector <Cell*> grays;

void InitMaze();

void init()
{
	srand(time(0));
	glClearColor(0,0.3,0.0,0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	InitMaze();
}

void InitMaze()
{
	int i, j;

	// the frame of maze is made of walls
	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ-1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}
	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 0) // mostly SPACES
			{
				if(rand()%100<80)
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
	// set START and TARGET
	startR = MSZ / 2;
	startC = MSZ / 2;
	targetR = rand() % MSZ;
	targetC = rand() % MSZ;
	maze[startR][startC] = START;
	// add start Cell to grays
	Cell* pc = new Cell(startR, startC, nullptr);
	grays.push_back(pc);
	maze[targetR][targetC] = TARGET;
}

void DrawMaze()
{
	int i, j;

	for(i=0;i<MSZ;i++)
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
				glColor3d(0.7, 1, 0.7); // light green
				break;
			case GRAY:
				glColor3d(1, 0.3, 0); // orange
				break;
			case PATH:
				glColor3d(0.8, 0.5, 1); // purple
				break;
			}
			// draw square
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j+1, i);
			glVertex2d(j+1, i+1);
			glVertex2d(j, i+1);
			glEnd();
		}
	
}

void RestorePath(Cell* pc)
{
	while (pc->getParent() != nullptr)
	{
		maze[pc->getRow()][pc->getColumn()] = PATH;
		pc = pc->getParent();
	}
}

void CheckNeighbor(int row, int column, Cell* pcurrent)
{
	if (maze[row][column] == TARGET)
	{
		runBFS = false; // stop running BFS
		runDFS = false; // stop runnnig DFS
		runBestFirstSearch = false; // stop running best-first-search
		runAStar = false; // stop running A*
		cout << "The solution has been found\n";
		RestorePath(pcurrent);
	}
	else // maze[row][column] is WHITE
	{
		Cell* pn = new Cell(row, column, pcurrent);
		// paint this neighbor gray
		grays.push_back(pn);
		maze[row][column] = GRAY;
	}
}

void RunBFSIteration()
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
		c = pcurrent->getColumn();
		maze[r][c] = BLACK; // paint it black
		grays.erase(grays.begin());
		// add all white neighbors of pcurrent to grays
		// UP
		if (runBFS && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent);
		// DOWN
		if (runBFS && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent);
		// LEFT
		if (runBFS && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r , c- 1, pcurrent);
		// RIGHT
		if (runBFS && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent);

	}
}

void RunDFSIteration() 
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
		c = pcurrent->getColumn();
		maze[r][c] = BLACK; // paint it black
		grays.pop_back();

		// check neighbors (use random direction)
		int directions[4] = { -1,-1,-1,-1 };
		int dir,place;
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
						CheckNeighbor(r + 1, c, pcurrent);
					break;
				case 1: // down
					if (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET)
						CheckNeighbor(r - 1, c, pcurrent);
					break;
				case 2: // left
					if (maze[r ][c- 1] == SPACE || maze[r ][c- 1] == TARGET)
						CheckNeighbor(r , c- 1, pcurrent);
					break;
				case 3: // right
					if (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET)
						CheckNeighbor(r, c + 1, pcurrent);
					break;

				}
			}
		}
	}
}

// get distance of given cell to target.
double getDistanceFromCellToTarget(Cell* cur)
{
	return sqrt(abs(pow(targetR - cur->getRow(),2) + pow(targetC - cur->getColumn(),2)));
}

// get distance of given cell from Start.
double getDistanceFromCellToStart(Cell* cur)
{
	return sqrt(abs(pow(startR - cur->getRow(), 2) + pow(startC - cur->getColumn(), 2)));
}

// find the closest cell to TARGET from grays vector & erase it.
Cell* getClosestOptionToBestFirstSearch() {
	Cell* toRet = *grays.begin();
	double minDistance = 10000000, tempDistance = 1;
	int i = 0, remove= 0;
	for (Cell* cell : grays)// Find cell closest to target
	{
		tempDistance = getDistanceFromCellToTarget(cell);
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
Cell* getClosestOptionToAStar() {
	Cell* toRet = *grays.begin();
	double minHCost = INT_MAX,minGCost = INT_MAX, minTotalCost = INT_MAX, HCost, GCost,totalCost;
	int i = 0, remove = 0; // i = index, remove = index remember to use with vector.erase function.
	for (Cell* cell : grays)// Find cell closest to (target + start)
	{
		HCost = getDistanceFromCellToTarget(cell); // distance of cell from target
		GCost = getDistanceFromCellToStart(cell); // distance of cell from start
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
void RunBestFirstSearchIteration()
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
		pcurrent = getClosestOptionToBestFirstSearch(); // get the closest cell to target & pop it from grays vector.
		r = pcurrent->getRow();
		c = pcurrent->getColumn();
		maze[r][c] = BLACK; // paint it black
		// add all white neighbors of pcurrent to grays
		// UP
		if (runBestFirstSearch && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent);
		// DOWN
		if (runBestFirstSearch && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent);
		// LEFT
		if (runBestFirstSearch && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r, c - 1, pcurrent);
		// RIGHT
		if (runBestFirstSearch && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent);
	}
}

void RunAStarIteration()
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
		pcurrent = getClosestOptionToAStar(); // get the closest cell to target + start & pop it from grays vector.
		r = pcurrent->getRow();
		c = pcurrent->getColumn();
		maze[r][c] = BLACK; // paint it black
		// add all white neighbors of pcurrent to grays
		// UP
		if (runAStar && (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARGET))
			CheckNeighbor(r + 1, c, pcurrent);
		// DOWN
		if (runAStar && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARGET))
			CheckNeighbor(r - 1, c, pcurrent);
		// LEFT
		if (runAStar && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARGET))
			CheckNeighbor(r, c - 1, pcurrent);
		// RIGHT
		if (runAStar && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARGET))
			CheckNeighbor(r, c + 1, pcurrent);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle() 
{
	if (runBFS)
		RunBFSIteration();
	else 	if (runDFS)
		RunDFSIteration();
	else if (runBestFirstSearch)
		RunBestFirstSearchIteration();
	else if (runAStar)
		RunAStarIteration();

	glutPostRedisplay(); // indirect call to display
}

void menu(int choice) {
	switch (choice) {
	case 1: // BFS
		cout << "Ruuning BFS\n";
		runBFS = true;
		break;
	case 2: // DFS
		cout << "Ruuning DFS\n";
		runDFS = true;
		break;
	case 3: // Best first search
		cout << "Ruuning Best First Search\n";
		runBestFirstSearch = true;
		break;
	case 4: // A*
		cout << "Ruuning A*\n";
		runAStar = true;
		break;

	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(400, 50);
	glutCreateWindow("HW2_SaarGamzo_Maze");

	glutDisplayFunc(display); // display is the refresh function
	glutIdleFunc(idle);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Run BFS", 1);
	glutAddMenuEntry("Run DFS",2);
	glutAddMenuEntry("Run Best-First-Search", 3);
	glutAddMenuEntry("Run A*", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}
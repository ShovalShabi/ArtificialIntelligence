
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <vector>
#include "Cell.h"
#include <iostream>

using namespace std;

const int MSZ = 100;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int GRAY = 4;
const int BLACK = 5;
const int PATH = 6;

int maze[MSZ][MSZ] = {0}; // space
bool runBFS = false, runDFS = false;

queue <Cell*> grays; // queue of gray cells
vector <Cell*> dfs_grays; // stack of gray cells


void InitMaze();

void init()
{
	glClearColor(0.5,0.5,0.5,0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	InitMaze();
}

void InitMaze()
{
	int i, j;

	for (i = 0;i < MSZ;i++)
	{
		maze[0][i] = WALL;
		maze[MSZ-1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for(i=1;i<MSZ-1;i++)
		for (j = 1;j < MSZ - 1;j++)
		{
			if (i % 2 == 1) // mostly spaces
			{
				if (rand() % 10 < 2) // WALL
					maze[i][j] = WALL;
				else maze[i][j] = SPACE;
			}
			else // even lines
			{
				if (rand() % 10 < 4) // space
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}
	maze[MSZ / 2][MSZ / 2] = START;
	maze[rand() % MSZ][rand() % MSZ] = TARGET;

	Cell* pc = new Cell(MSZ / 2, MSZ / 2,nullptr);  // pointer to a cell
	grays.push(pc); // insert first cell to grays
	dfs_grays.push_back(pc); // insert first cell to grays

}

void DrawMaze()
{
	int i, j;

	for(i=0;i<MSZ;i++)
		for (j = 0;j < MSZ;j++)
		{
			switch (maze[i][j]) // set convinient color of a cell
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0,0, 0); // black
				break;
			case START:
				glColor3d(0.5, 0.5, 1); // blue
				break;
			case TARGET:
				glColor3d(1, 0, 0); // red
				break;
			case GRAY:
				glColor3d(1, 0.7, 0); // orange
				break;
			case BLACK:
				glColor3d(0.8, 1, 0.8); // green
				break;
			case PATH:
				glColor3d(1, 0, 1); // magenta
				break;

			}// switch

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j+1, i + 1);
			glVertex2d(j+1 , i );
			glEnd();
		}
}

void RestorePath(Cell* pc)
{
	while (pc != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

// BFS a cell on row,col can be either WHITE or TARGET
bool CheckNeighbour(int row, int col, Cell* pCurrent)
{
	Cell* pn = nullptr; 
	if (maze[row][col] == TARGET)
	{
		runBFS = false;
		RestorePath(pCurrent);
		return false;
	}
	else // must be "White Neighbour" - SPACE
	{
		pn = new Cell(row, col, pCurrent); // create new Cell
		grays.push(pn);
		maze[row][col] = GRAY;
		return true; // go_on = true
	}
}

void RunBFSIteration()
{
	Cell* pc;
	int row, col;
	bool go_on = true;

	if (grays.empty())
	{
		runBFS = false;
		cout << "Grays are empty. There is no solution\n";
		return;
	}
	else // grays is not empty
	{
		// pick the first Cell from grays
		pc = grays.front();
		grays.pop();
		// paint it black
		if(maze[pc->getRow()][pc->getCol()]!=START) maze[pc->getRow()][pc->getCol()] = BLACK;

		row = pc->getRow();
		col = pc->getCol();
		// now check all neighbors of pc and if they are white add them to grays
		// but if one of them is target then stop BFS because target has been found
		
		// up
		if (maze[row + 1][col] == SPACE || maze[row + 1][col] == TARGET)
			go_on = CheckNeighbour(row + 1, col, pc);
		// down
		if (go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == TARGET))
			go_on = CheckNeighbour(row - 1, col, pc);
		// left
		if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == TARGET))
			go_on = CheckNeighbour(row, col - 1, pc);
		// right
		if (go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == TARGET))
			go_on = CheckNeighbour(row, col + 1, pc);

	}
}

// DFS a cell on row,col can be either WHITE or TARGET
bool CheckNeighbourDFS(int row, int col, Cell* pCurrent)
{
	Cell* pn = nullptr;
	if (maze[row][col] == TARGET)
	{
		runDFS = false;
		RestorePath(pCurrent);
		return false;
	}
	else // must be "White Neighbour" - SPACE
	{
		pn = new Cell(row, col, pCurrent); // create new Cell
		dfs_grays.push_back(pn);
		maze[row][col] = GRAY;
		return true; // go_on = true
	}
}

void RunDFSIteration()
{
	Cell* pc;
	int row, col;
	bool go_on = true;

	if (dfs_grays.empty())
	{
		runDFS = false;
		cout << "Grays are empty. There is no solution\n";
		return;
	}
	else // grays is not empty
	{
		// pick the last inserted element of the stack
		pc = dfs_grays.back();
		dfs_grays.pop_back();
		// paint it BLACK
		if (maze[pc->getRow()][pc->getCol()] != START) maze[pc->getRow()][pc->getCol()] = BLACK;

		row = pc->getRow();
		col = pc->getCol();
		// now check all neighbors of pc and if they are white add them to dfs_grays
		// but if one of them is target then stop DFS because target has been found

		// Choose random direction
		int directions[4] = { -1,-1,-1,-1 };
		int place;
		for (int dir = 0;dir < 4;dir++)       
		{
			do {
				place = rand() % 4;
			} while (directions[place] != -1);
			directions[place] = dir;
		}

		for (int i = 0;i < 4;i++)
		{
			switch (directions[i])
			{
			case 0: // up
				if (go_on && (maze[row + 1][col] == SPACE || maze[row + 1][col] == TARGET))
					go_on = CheckNeighbourDFS(row + 1, col, pc);
				break;
			case 1: // down
				if (go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == TARGET))
					go_on = CheckNeighbourDFS(row - 1, col, pc);
				break;
			case 2: // left
				if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == TARGET))
					go_on = CheckNeighbourDFS(row, col - 1, pc);
				break;
			case 3: // right
				if (go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == TARGET))
					go_on = CheckNeighbourDFS(row, col + 1, pc);
				break;
			}
		}
/*
		// UP
		if (maze[row + 1][col] == SPACE || maze[row + 1][col] == TARGET)
			go_on = CheckNeighbourDFS(row + 1, col, pc);
		// RIGHT
		if (go_on && (maze[row][col+1] == SPACE || maze[row ][col+ 1] == TARGET))
			go_on = CheckNeighbourDFS(row , col+ 1, pc);
		// DOWN
		if (go_on &&(maze[row - 1][col] == SPACE || maze[row - 1][col] == TARGET))
			go_on = CheckNeighbourDFS(row - 1, col, pc);
		// LEFT
		if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == TARGET))
			go_on = CheckNeighbourDFS(row, col - 1, pc);
*/
	}
}

// drawings are here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer with background color

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle() 
{

	if (runBFS)
		RunBFSIteration();
	if (runDFS)
		RunDFSIteration();

	glutPostRedisplay(); // call to display indirectly
}

void menu(int choice)
{
	if (choice == 1) // BFS
		runBFS = true;
	if (choice ==2 ) // DFS
		runDFS = true;
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("BFS Example");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle);  // updates function
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("run BFS", 1);
	glutAddMenuEntry("run DFS", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}
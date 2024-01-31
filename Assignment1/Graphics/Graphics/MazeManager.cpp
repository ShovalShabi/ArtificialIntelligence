#include "MazeManager.h"
#include <time.h>
#include <stdlib.h>
#include "Cell.h"
#include "GameManager.h"

void MazeManager::initMaze(GameManager gameManger)
{
	int i, j;

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
			if (i % 2 == 1) // mostly spaces
			{
				if (rand() % 10 < 2) { // WALL
					maze[i][j] = WALL;
				}
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
	start = new Cell(MSZ / 2, MSZ / 2, nullptr);
	maze[start->getRow()][start->getCol()] = START;
	target = new Cell(rand() % MSZ, rand() % MSZ, nullptr);
	maze[target->getRow()][target->getCol()] = TARGET;

	gameManger.getBfsGrays().push(start); // insert first cell to grays
	gameManger.getDfsGrays().push_back(start); // insert first cell to grays
	gameManger.getBiBfsGrays().push(start);
}

void MazeManager::init(GameManager gameManager)
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	this->initMaze(gameManager);
}

void MazeManager::drawMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j]) // set convinient color of a cell
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0, 0, 0); // black
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
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();
		}
}


void MazeManager::restorePath(Cell* pc)
{
	while (pc != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

bool MazeManager::checkNeighbour(int row, int col, Cell* pCurrent)
{
	Cell* pn = nullptr;
	if (maze[row][col] == TARGET)
	{
		runBFS = false;
		this->restorePath(pCurrent);
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

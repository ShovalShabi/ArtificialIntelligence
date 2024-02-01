#include "MazeManager.h"
#include <time.h>
#include <stdlib.h>
#include "Cell.h"
#include "GameManager.h"
#include "glut.h"
#include "Cell.h"
#include <iostream>

MazeManager::MazeManager() {
	this->maze = new int* [MSZ];
	for (int i = 0; i < MSZ; ++i) {
		this->maze[i] = new int[MSZ];
	}

	// Initialize maze elements
	for (int i = 0; i < MSZ; ++i) {
		for (int j = 0; j < MSZ; ++j) {
			this->maze[i][j] = 0;
		}
	}
	this->start = NULL;
	this->target = NULL;

}

MazeManager::~MazeManager() {
	for (int i = 0; i < MSZ; ++i) {
		delete[] this->maze[i];
	}
	delete[] this->maze;
}

void MazeManager::init(GameManager* gameManager)
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	this->initMaze(gameManager);
}

void MazeManager::initMaze(GameManager* gameManager)
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
	Cell* start = new Cell(MSZ / 2, MSZ / 2, nullptr);
	maze[start->getRow()][start->getCol()] = START;
	Cell* target = new Cell(rand() % MSZ, rand() % MSZ, nullptr);
	maze[target->getRow()][target->getCol()] = TARGET;

	gameManager->getBfsGrays().push(start); // insert first cell to grays
	gameManager->getDfsGrays().push_back(start); // insert first cell to grays
	gameManager->getBiBfsGrays().push(target); // insert the target cell to grays

	std::cout << "Start Cell: Row " << start->getRow() << ", Col " << start->getCol() << std::endl;
	std::cout << "Target Cell: Row " << target->getRow() << ", Col " << target->getCol() << std::endl;

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

// drawings are here
void  MazeManager::display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer with background color

	drawMaze();

	glutSwapBuffers(); // show all
}

void MazeManager::idle(GameManager* gameManager)
{

	if (this->getRunBFS())
		gameManager->runIteration(maze, BFS_OPT, TARGET);
	if (this->getRunDFS())
		gameManager->runIteration(maze, DFS_OPT, TARGET);
	if (this->getRunBiBfs()) {
		gameManager->runIteration(maze, BIBFS_OPT, START);
	}

	glutPostRedisplay(); // call to display indirectly
}

void  MazeManager::menu(int choice)
{
	if (choice == BFS_OPT) // BFS
		this->setRunBFS(true);
	if (choice == DFS_OPT) // DFS
		this->setRunDFS(true);
	if (choice == BIBFS_OPT) {	// BiBFS
		this->setRunBiBFS(true);
		this->setRunBFS(true);
	}
}
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
	startR, startC, targetR, targetC = 0, 0, 0, 0;
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
	// set START and TARGET
	startR = MSZ / 2;
	startC = MSZ / 2;
	targetR = rand() % MSZ;
	targetC = rand() % MSZ;
	maze[startR][startC] = START;
	// add start Cell to grays
	Cell* pc = new Cell(startR, startC, nullptr);
	gameManager->getGrays().push_back(pc);
	maze[targetR][targetC] = TARGET;

	std::cout << "Start Cell: Row " << startR << ", Col " << startC << std::endl;
	std::cout << "Target Cell: Row " << targetR << ", Col " << targetC << std::endl;

}

void MazeManager::drawMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j]) // set convenient color of a cell
			{
			case SPACE:
				// White
				glColor3d(1, 1, 1);
				break;
			case WALL:
				// Black
				glColor3d(0, 0, 0);
				break;
			case START:
				// Light Blue
				glColor3d(0.5, 0.5, 1);
				break;
			case TARGET:
				// Red
				glColor3d(1, 0, 0);
				break;
			case ORANGE:
				// Orange
				glColor3d(1, 0.7, 0);
				break;
			case VISITED:
				// Light Green
				glColor3d(0.8, 1, 0.8);
				break;
			case BRONZE:
				//Bronze
				glColor3f(0.1f, 0.1f, 0.0f);
				break;
			case VIOLET:
				//Dark Blue
				glColor3f(0.5f, 0.5f, 0.5f);
				break;
			case PATH:
				// Magenta
				glColor3d(1, 0, 1);
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
	//std::cout << "Before Iteration - BFS: " << gameManager->getRunBFS() << ", DFS: " << gameManager->getRunDFS() << ", BiBFS: " << gameManager->getRunBiBFS() << std::endl;

	if (gameManager->getRunBFS())
		gameManager->RunBFSIteration(maze);

	if (gameManager->getRunDFS())
		gameManager->RunDFSIteration(maze);

	if (gameManager->getRunBestFirstSearch()) 
		gameManager->RunBestFirstSearchIteration(maze,targetR,targetC);

	if (gameManager->getRunAStar())
		gameManager->RunAStarIteration(maze, startR, startC, targetR, targetC);

	//std::cout << "After Iteration - BFS: " << gameManager->getRunBFS() << ", DFS: " << gameManager->getRunDFS() << ", BiBFS: " << gameManager->getRunBiBFS() << std::endl;

	glutPostRedisplay(); // call to display indirectly
}


void  MazeManager::menu(int choice, GameManager* gameManager)
{
	if (choice == BFS_OPT) // BFS
		gameManager->setRunBFS(true);
	if (choice == DFS_OPT) // DFS
		gameManager->setRunDFS(true);
	if (choice == BEST_FIRST_SERACH_OPT) 	// Best First Search
		gameManager->setRunBestFirstSearch(true);
	if (choice == A_STAR_OPT) 	// A Star Search
		gameManager->setRunAStar(true);
}
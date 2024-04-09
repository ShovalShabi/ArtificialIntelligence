#include "MazeMngr.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <chrono>
#include <thread>
#include "glut.h"
#include <iostream>
#include <algorithm>



MazeMngr::MazeMngr()
{
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
}

void MazeMngr::init(GameMngr* gameMngr)
{
	srand((unsigned int)time(NULL));
	glClearColor((GLclampf)0, (GLclampf)0.3, (GLclampf)0.0, (GLclampf)0);
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	InitMaze(gameMngr);
}


Cell* MazeMngr::generateUniqueCell(int** maze)
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

void MazeMngr::fillWithZeros()
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

void MazeMngr::InitMaze(GameMngr* gameMngr)
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
	//Pacman Creation
	int x, y;
	gameMngr->setPacman(generateUniqueCell(maze));

	x = gameMngr->getPacman()->getRow();
	y = gameMngr->getPacman()->getColumn();
	maze[x][y] = PACMAN;
	cout << "Pacman location is [" << x << "][" << y << "]\n";

	//Initialization of the ghosts patameters
	for (int i = 0; i < NUMGHOSTS; i++)
	{
		Cell* ghost = generateUniqueCell(maze);

		x = ghost->getRow();
		y = ghost->getColumn();
		maze[x][y] = GHOST;
		gameMngr->getGhostCellStartPos().push_back(new Cell(x, y, nullptr));
		gameMngr->getListOfPriorityQueues().push_back(priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>());// PriorityQueues of each ghost
		gameMngr->getListOfPriorityQueues()[i].push(ghost);
		gameMngr->getGhostNeedToMoveList().push_back(true);// Flag list to each ghost for movement
		gameMngr->getGhostStuckList().push_back(false);// Flag list to each ghost for checking if the ghost is stuck
		gameMngr->getGhostFoundPathList().push_back(true);// Flag list to each ghost for checking id the ghost is stuck
		gameMngr->getGhostsList().push_back(ghost);
		cout << "Ghost " << i << " location is [" << x << "][" << y << "]\n";
	}

	//Initialization of the coins patameters
	for (int i = 0; i < NUM_OF_COINS; i++)
	{
		Cell* coin = generateUniqueCell(maze);

		x = coin->getRow();
		y = coin->getColumn();
		maze[x][y] = COIN;
		gameMngr->getCoins().push_back(coin);
		cout << "Coin " << i << " location is [" << x << "][" << y << "]\n";
	}
}

void MazeMngr::DrawMaze()
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
				glColor3d(0.5, 0.5, 0.5); // light gray
				break;
			case PATH:
				glColor3d(0.8, 0.5, 1); // purple
				break;
			case PACMAN:
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

void MazeMngr::display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	glutSwapBuffers(); // show all
}


void MazeMngr::idle(GameMngr* gameMngr)
{

	if (gameMngr->getRunPackman())
		gameMngr->runPacmanGame(maze);

	this_thread::sleep_for(chrono::milliseconds(250)); // Adjust the delay time as needed


	glutPostRedisplay(); // indirect call to display
}


void MazeMngr::menu(int choice, GameMngr* gameMngr)
{
	switch (choice)
	{
	case 1: // start packman
		cout << "##### Packman started! #####\n";
		gameMngr->setRunPackman(true);
		break;
	}
}
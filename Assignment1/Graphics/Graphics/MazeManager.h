#pragma once
#define MSZ 100
#define SPACE 0
#define WALL 1
#define START 2
#define TARGET 3
#define GRAY 4
#define BLACK 5
#define PATH 6
#define DIRECTION 4

#include "glut.h"
#include "Cell.h"
#include <iostream>
#include "GameManager.h"


class MazeManager
{
private:
	bool runBFS = false;
	bool runDFS = false;
	bool runBiBFS = false;
	int** maze;
	Cell* start;
	Cell* target;

public:

	MazeManager() {
		this->maze[MSZ][MSZ] = { 0 };
		this->start = NULL;
		this->target = NULL;

	}

	void initMaze(GameManager gameManger);

	void init(GameManager gameManager);

	void drawMaze();

	void restorePath(Cell* pc);

	bool checkNeighbour(int row, int col, Cell* pCurrent);

};


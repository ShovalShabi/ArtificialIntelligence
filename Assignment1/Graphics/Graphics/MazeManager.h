#pragma once
#include "GameManager.h"
#include "constants.h"


class MazeManager
{
private:
	int** maze;
	Cell* start;
	Cell* target;

public:

	MazeManager();

	~MazeManager();

	void initMaze(GameManager* gameManger);

	void init(GameManager* gameManager);

	void drawMaze();

	void display();

	void idle(GameManager *gameManager);

	void menu(int choice, GameManager* gameManager);

};


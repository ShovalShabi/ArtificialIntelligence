#pragma once
#include "GameManager.h"
#include "constants.h"


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

	MazeManager();

	~MazeManager();


	bool getRunBFS() { return runBFS; }
	bool getRunDFS() { return runDFS; }
	bool getRunBiBfs() { return runBiBFS; }
	void setRunBFS(bool change) { runBFS = change; }
	void setRunDFS(bool change) { runDFS = change; }
	void setRunBiBFS(bool change) { runBiBFS = change; }

	void initMaze(GameManager* gameManger);

	void init(GameManager* gameManager);

	void drawMaze();

	void display();

	void idle(GameManager *gameManager);

	void menu(int choice);
};


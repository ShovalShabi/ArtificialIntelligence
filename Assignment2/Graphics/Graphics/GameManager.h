#pragma once
#include <queue>
#include <vector>
#include "Cell.h"

using namespace std;

class GameManager
{
private:
	bool runBFS;
	bool runDFS;
	bool runBestFirstSearch;
	bool runAStar;
	vector <Cell*> grays; // vector of gray cells for regular BFS algo

public:
	GameManager();
	bool getRunBFS() { return runBFS; }
	bool getRunDFS() { return runDFS; }
	bool getRunBestFirstSearch() { return runBestFirstSearch; }
	bool getRunAStar() { return runAStar; }
	vector<Cell*>& getGrays() { return grays; }
	void setRunBFS(bool change) { runBFS = change; }
	void setRunDFS(bool change) { runDFS = change; }
	void setRunBestFirstSearch(bool change) { runBestFirstSearch = change; }
	void setRunAStar(bool change) { runAStar = change; }
	void RestorePath(Cell* pc, int** maze);
	void CheckNeighbor(int row, int column, Cell* pcurrent, int** maze);
	void RunBFSIteration(int** maze);
	void RunDFSIteration(int** maze);
	double getDistanceFromCellToTarget(Cell* cur, int targetR, int targetC);
	double getDistanceFromCellToStart(Cell* cur, int startR, int startC);
	Cell* getClosestOptionToBestFirstSearch(int targetR, int targetC);
	Cell* getClosestOptionToAStar(int startR, int startC, int targetR, int targetC);
	void RunBestFirstSearchIteration(int** maze, int targetR, int targetC);
	void RunAStarIteration(int** maze, int startR, int startC, int targetR, int targetC);
};


#pragma once
#include <queue>
#include <vector>
#include "Cell.h"

using namespace std;

class GameManager
{
private:
	Cell* startCell;
	Cell* targetCell;
	bool runBFS;
	bool runDFS;
	bool runBiBFS;
	queue <Cell*> bfsGrays; // queue of gray cells for regular BFS algo
	vector <Cell*> dfsGrays; // vector of gray cells for regular BFS algo
	queue <Cell*> biBfsGrays; // queue of gray cells for BIDIRECTIONAL BFS algo

public:
	GameManager();
	queue<Cell*>& getBfsGrays() { return bfsGrays; }
	vector<Cell*>& getDfsGrays() { return dfsGrays; }
	queue<Cell*>& getBiBfsGrays() { return biBfsGrays; }
	bool getRunBFS() { return runBFS; }
	bool getRunDFS() { return runDFS; }
	bool getRunBiBFS() { return runBiBFS; }
	void setRunBFS(bool change) {runBFS=change; }
	void setRunDFS(bool change) { runDFS = change; }
	void setRunBiBFS(bool change) { runBiBFS = change; }
	Cell* getStartCell() { return startCell; }
	Cell* getTargetCell() { return targetCell; }
	void setStartCell(Cell* change) { startCell = change; }
	void setTargetCell(Cell* change) { targetCell = change; }
	void RestorePath(Cell* pc, int** maze);
	bool CheckNeighbourGenerically(int row, int col, Cell* pCurrent, int** maze, int targetMark, int typeAlgo);
	void runIteration(int** maze, int typeAlgo, int targetMark);
};


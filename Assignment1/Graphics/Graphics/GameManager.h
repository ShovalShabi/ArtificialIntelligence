#pragma once
#include <queue>
#include <vector>
#include "Cell.h"

#define DFS_OPT 1
#define BFS_OPT 2
#define BIBFS_OPT 3

using namespace std;

class GameManager
{
private:
	queue <Cell*> bfsGrays; // queue of gray cells for regular BFS algo
	vector <Cell*> dfsGrays; // vector of gray cells for regular BFS algo
	queue <Cell*> biBfsGrays; // queue of gray cells for BIDIRECTIONAL BFS algo

public:
	queue<Cell*> getBfsGrays() { return bfsGrays; }
	vector<Cell*> getDfsGrays() { return dfsGrays; }
	queue<Cell*> getBiBfsGrays() { return biBfsGrays; }
};


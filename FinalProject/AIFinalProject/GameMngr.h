#pragma once
#include "Cell.h"
#include "GeneralConsts.h"
#include <vector>

#include <queue>
#include "CompareCellsWithDistance.h"

using namespace std;

class GameMngr
{
private:
	bool MonstersTurn;
	bool PacmanTurn;

	Cell* pacman;
	vector<Cell*> coins; // all coins

	bool runPackman;

	int coinsLeft;
	int coinsCounter;
	bool runPacmanBFS;
	bool PackmanNeedToMove;

	vector <Cell*> pacmanGrays;
	vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>> listOfPriorityQueues;// PriorityQueues of each ghost
	vector<bool> ghostNeedToMoveList;// Flag list to each ghost for movement
	vector<bool> ghostStuckList;// Flag list to each ghost for checking if the ghost is stuck
	vector<bool> ghostFoundPathList;// Flag list to each ghost for checking id the ghost is stuck
	vector<Cell*> ghostCellStartPos;// List to each ghost initial postion
	vector<Cell*> ghostsList;// A list containgn the ghosts pointers

public:
	// Getters
	bool getMonstersTurn() const { return MonstersTurn; }
	bool getPacmanTurn() const { return PacmanTurn; }
	Cell* getPacman() const { return pacman; }
	vector<Cell*>& getCoins() { return coins; }
	bool getRunPackman() const { return runPackman; }
	int getCoinsLeft() const { return coinsLeft; }
	int getCoinsCounter() const { return coinsCounter; }
	bool getRunPacmanBFS() const { return runPacmanBFS; }
	bool getPackmanNeedToMove() const { return PackmanNeedToMove; }
	vector<Cell*>& getPacmanGrays() { return pacmanGrays; }
	vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>& getListOfPriorityQueues() { return listOfPriorityQueues; }
	vector<bool>& getGhostNeedToMoveList() { return ghostNeedToMoveList; }
	vector<bool>& getGhostStuckList() { return ghostStuckList; }
	vector<bool>& getGhostFoundPathList() { return ghostFoundPathList; }
	vector<Cell*>& getGhostCellStartPos() { return ghostCellStartPos; }
	vector<Cell*>& getGhostsList() { return ghostsList; }

	// Setters
	void setMonstersTurn(bool value) { MonstersTurn = value; }
	void setPacmanTurn(bool value) { PacmanTurn = value; }
	void setPacman(Cell* p) { pacman = p; }
	void setCoins(const vector<Cell*>& c) { coins = c; }
	void setRunPackman(bool value) { runPackman = value; }
	void setCoinsLeft(int value) { coinsLeft = value; }
	void setCoinsCounter(int value) { coinsCounter = value; }
	void setRunPacmanBFS(bool value) { runPacmanBFS = value; }
	void setPackmanNeedToMove(bool value) { PackmanNeedToMove = value; }
	void setPacmanGrays(const vector<Cell*>& pg) { pacmanGrays = pg; }
	void setListOfPriorityQueues(const vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>& lopq) { listOfPriorityQueues = lopq; }
	void setGhostNeedToMoveList(const vector<bool>& gntml) { ghostNeedToMoveList = gntml; }
	void setGhostStuckList(const vector<bool>& gsl) { ghostStuckList = gsl; }
	void setGhostFoundPathList(const vector<bool>& gfp) { ghostFoundPathList = gfp; }
	void setGhostCellStartPos(const vector<Cell*>& gcs) { ghostCellStartPos = gcs; }
	void setGhostsList(const vector<Cell*>& gl) { ghostsList = gl; }

	//Actual Functions
	GameMngr(int numCoins);
	void RestorePacmanPath(Cell* pc, Cell*& pacman, int** maze);
	void CheckPacmanNeighbor(int row, int column, Cell* pcurrent, Cell*& pacman, int** maze);
	void clearMaze(int** maze);
	void runPacmanGame(int** maze);
	void pacmanIteration(Cell*& pacman, int** maze, vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays);
	void checkIfCoin(int r, int c, int** maze);
	void RunPacmanBFS(Cell*& pacman, int** maze, vector<Cell*>& pacmanGrays, bool* PackmanNeedToMove, bool* runPackman);
	void PacmanRunAwayGhost(Cell*& pacman, Cell* ghost, int** maze);
	void GhostsIteration(Cell* pacman, int** maze);
	void justMove(Cell*& pc, int** maze);
	void GhostIteration(int indexGhost, Cell* pacman, int** maze);
	void CheckNeighborForGhost(int row, int column, Cell*& pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze);
	void RestorePathForGhost(Cell*& pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman);
	double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2);
};

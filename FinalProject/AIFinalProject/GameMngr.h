#pragma once
#include "Cell.h"
#include "GeneralConsts.h"
#include "State.h"
#include <vector>
#include <queue>
#include "CompareCellsWithDistance.h"

using namespace std;

class State;
class Cell;

/**
 * @brief Class representing the game manager.
 */
class GameMngr
{
private:
	State* currentState; /**< Pointer to the current state of the game. */

	Cell* pacman; /**< Pointer to the cell representing the position of Pacman. */
	vector<Cell*> coins; /**< Vector containing pointers to all coins in the game. */

	bool runPackman; /**< Flag indicating whether Pacman is running. */

	int coinsLeft; /**< Number of coins left in the game. */
	int coinsCounter; /**< Counter for collected coins. */
	bool packmanNeedToCollectCoins; /**< Flag indicating whether Pacman needs to collect coins. */
	bool packmanRunFromGhosts; /**< Flag indicating whether Pacman needs to run from ghosts. */

	vector<Cell*> pacmanGrays; /**< Vector containing pointers to grayed cells around Pacman. */
	vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>> listOfPriorityQueues; /**< Vector containing priority queues of cells for each ghost. */
	vector<bool> ghostNeedToMoveList; /**< Flag list indicating whether each ghost needs to move. */
	vector<bool> ghostStuckList; /**< Flag list indicating whether each ghost is stuck. */
	vector<bool> ghostFoundPathList; /**< Flag list indicating whether each ghost has found a path. */
	vector<Cell*> ghostCellStartPos; /**< Vector containing pointers to the initial positions of ghosts. */
	vector<Cell*> ghostsList; /**< Vector containing pointers to all ghosts in the game. */

public:
	// Getters
	State* getCurrentState() const { return currentState; }
	Cell* getPacman() const { return pacman; }
	vector<Cell*>& getCoins() { return coins; }
	bool getRunPackman() const { return runPackman; }
	int getCoinsLeft() const { return coinsLeft; }
	int getCoinsCounter() const { return coinsCounter; }
	bool getPackmanNeedToCollectCoins() const { return packmanNeedToCollectCoins; }
	bool getPackmanRunFromGhosts() const { return packmanRunFromGhosts; }
	vector<Cell*>& getPacmanGrays() { return pacmanGrays; }
	vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>& getListOfPriorityQueues() { return listOfPriorityQueues; }
	vector<bool>& getGhostNeedToMoveList() { return ghostNeedToMoveList; }
	vector<bool>& getGhostStuckList() { return ghostStuckList; }
	vector<bool>& getGhostFoundPathList() { return ghostFoundPathList; }
	vector<Cell*>& getGhostCellStartPos() { return ghostCellStartPos; }
	vector<Cell*>& getGhostsList() { return ghostsList; }

	// Setters
	void setCurrentState(State* value) { currentState = value; }
	void setPacman(Cell* p) { pacman = p; }
	void setCoins(const vector<Cell*>& c) { coins = c; }
	void setRunPackman(bool value) { runPackman = value; }
	void setCoinsLeft(int value) { coinsLeft = value; }
	void setCoinsCounter(int value) { coinsCounter = value; }
	void setPackmanNeedToCollectCoins(bool value) { packmanNeedToCollectCoins = value; }
	void setPackmanRunFromGhosts(bool value) { packmanRunFromGhosts = value; }
	void setPacmanGrays(const vector<Cell*>& pg) { pacmanGrays = pg; }
	void setListOfPriorityQueues(const vector<priority_queue<Cell*, vector<Cell*>, CompareCellsWithDistance>>& lopq) { listOfPriorityQueues = lopq; }
	void setGhostNeedToMoveList(const vector<bool>& gntml) { ghostNeedToMoveList = gntml; }
	void setGhostStuckList(const vector<bool>& gsl) { ghostStuckList = gsl; }
	void setGhostFoundPathList(const vector<bool>& gfp) { ghostFoundPathList = gfp; }
	void setGhostCellStartPos(const vector<Cell*>& gcs) { ghostCellStartPos = gcs; }
	void setGhostsList(const vector<Cell*>& gl) { ghostsList = gl; }

	//Actual Functions
	/**
	 * @brief Constructor for the GameMngr class.
	 * @param numCoins Number of coins in the game.
	 */
	GameMngr(int numCoins);

	/**
	 * @brief Destructor for the GameMngr class.
	 */
	~GameMngr();

	/**
	 * @brief Restores the path for Pacman.
	 * @param pc Pointer to the current cell.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 */
	void RestorePacmanPath(Cell* pc, Cell* pacman, int** maze);

	/**
	 * @brief Checks the neighbor cells of Pacman.
	 * @param row Row index of the current cell.
	 * @param column Column index of the current cell.
	 * @param pcurrent Pointer to the current cell.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 */
	void CheckPacmanNeighbor(int row, int column, Cell* pcurrent, Cell* pacman, int** maze);

	/**
	 * @brief Updates the maze.
	 * @param maze 2D array representing the maze.
	 */
	void updateMaze(int** maze);

	/**
	 * @brief Runs the Pacman game.
	 * @param maze 2D array representing the maze.
	 */
	void runPacmanGame(int** maze);

	/**
	 * @brief Iterates Pacman's movement.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 * @param ghostsList Vector containing pointers to all ghosts in the game.
	 * @param pacmanGrays Vector containing pointers to grayed cells around Pacman.
	 */
	void pacmanIteration(Cell* pacman, int** maze,vector<Cell*> ghostsList, vector<Cell*>& pacmanGrays);

	/**
	 * @brief Collects a coin.
	 * @param r Row index of the coin.
	 * @param c Column index of the coin.
	 * @param maze 2D array representing the maze.
	 */
	void collectCoin(int r, int c, int** maze);

	/**
	 * @brief Runs the BFS algorithm for Pacman.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 * @param pacmanGrays Vector containing pointers to grayed cells around Pacman.
	 * @param packmanNeedToCollectCoins Pointer to the flag indicating whether Pacman needs to collect coins.
	 * @param runPackman Pointer to the flag indicating whether Pacman is running.
	 */
	void RunPacmanBFS(Cell* pacman, int** maze, vector<Cell*>& pacmanGrays, bool* packmanNeedToCollectCoins, bool* runPackman);

	/**
	 * @brief Makes Pacman run away from a ghost.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param ghost Pointer to the cell representing the ghost.
	 * @param maze 2D array representing the maze.
	 */
	void PacmanRunAwayGhost(Cell* pacman, Cell* ghost, int** maze);

	/**
	 * @brief Iterates through the ghosts' movement.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 */
	void GhostsIteration(Cell* pacman, int** maze);

	/**
	 * @brief Iterates the movement of a specific ghost.
	 * @param indexGhost Index of the ghost.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 */
	void GhostIteration(int indexGhost, Cell* pacman, int** maze);

	/**
	 * @brief Checks the neighbor cells of a ghost.
	 * @param row Row index of the current cell.
	 * @param column Column index of the current cell.
	 * @param pcurrent Pointer to the current cell.
	 * @param ghost Reference to the pointer to the cell representing the ghost.
	 * @param indexGhost Index of the ghost.
	 * @param pacman Pointer to the cell representing Pacman.
	 * @param maze 2D array representing the maze.
	 */
	void CheckNeighborForGhost(int row, int column, Cell* pcurrent, Cell*& ghost, int indexGhost, Cell* pacman, int** maze);

	/**
	 * @brief Restores the path for a ghost.
	 * @param pc Pointer to the current cell.
	 * @param ghost Reference to the pointer to the cell representing the ghost.
	 * @param indexGhost Index of the ghost.
	 * @param maze 2D array representing the maze.
	 * @param pacman Pointer to the cell representing Pacman.
	 */
	void RestorePathForGhost(Cell* pc, Cell*& ghost, int indexGhost, int** maze, Cell* pacman);

	/**
	 * @brief Calculates the distance between two dots.
	 * @param x1 X-coordinate of the first dot.
	 * @param y1 Y-coordinate of the first dot.
	 * @param x2 X-coordinate of the second dot.
	 * @param y2 Y-coordinate of the second dot.
	 * @return Distance between the two dots.
	 */
	double GetDistanceBetweenTwoDots(int x1, int y1, int x2, int y2);
};

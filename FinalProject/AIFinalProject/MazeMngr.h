#pragma once

#include "GameMngr.h"
#include <stdlib.h>

/**
 * @brief Class representing the maze manager.
 */
class MazeMngr
{
private:
    int** maze; /**< Pointer to the 2D array representing the maze. */

public:
    /**
     * @brief Default constructor for MazeMngr class.
     */
    MazeMngr();

    /**
     * @brief Destructor for MazeMngr class.
     */
    ~MazeMngr();

    /**
     * @brief Initializes the maze manager.
     * @param gameMngr Pointer to the game manager.
     */
    void init(GameMngr* gameMngr);

    /**
     * @brief Generates a unique cell in the maze.
     * @param maze 2D array representing the maze.
     * @return Pointer to the generated cell.
     */
    Cell* generateUniqueCell(int** maze);

    /**
     * @brief Fills the maze with zeros.
     */
    void fillWithZeros();

    /**
     * @brief Initializes the maze.
     * @param gameMngr Pointer to the game manager.
     */
    void InitMaze(GameMngr* gameMngr);

    /**
     * @brief Draws the maze.
     */
    void DrawMaze();

    /**
     * @brief Displays the maze.
     */
    void display();

    /**
     * @brief Idles the maze.
     * @param gameMngr Pointer to the game manager.
     */
    void idle(GameMngr* gameMngr);

    /**
     * @brief Handles menu selection.
     * @param choice Selected menu choice.
     * @param gameMngr Pointer to the game manager.
     */
    void menu(int choice, GameMngr* gameMngr);
};
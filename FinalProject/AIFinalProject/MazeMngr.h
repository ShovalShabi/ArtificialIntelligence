#pragma once
#include "GameMngr.h"
#include <stdlib.h>

class MazeMngr
{
private:
    int** maze;

public:
    MazeMngr();
    void init(GameMngr* gameMngr);
    Cell* generateUniqueCell(int** maze);
    void fillWithZeros();
    void InitMaze(GameMngr* gameMngr);
    void DrawMaze();
    void display();
    void idle(GameMngr* gameMngr);
    void menu(int choice, GameMngr* gameMngr);
};

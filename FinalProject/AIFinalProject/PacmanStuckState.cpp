/**
 * @file PacmanStuckState.h
 * @brief Implementation file for the Pacman Stuck State class.
 */

#include "PacmanStuckState.h"

 /**
  * @brief Executes actions when entering the Pacman Stuck State.
  * @param gameMngr Pointer to the Game Manager instance.
  */
void PacmanStuckState::OnEnter(GameMngr* gameMngr)
{
    gameMngr->setPackmanNeedToCollectCoins(false);
    gameMngr->setRunPackman(false); // Pacman has lost the game
}

/**
 * @brief Transition function for the Pacman Stuck State.
 * @details No transitions are made from the Pacman Stuck State as the game is over.
 * @param gameMngr Pointer to the Game Manager instance.
 * @param anotherState Pointer to the new state to transition to.
 */
void PacmanStuckState::Transition(GameMngr* gameMngr, State* anotherState)
{
    // No transitions are made from the Pacman Stuck State as the game is over.
}

/**
 * @brief Executes actions when exiting the Pacman Stuck State.
 * @details No actions are executed when exiting the Pacman Stuck State as the game is over.
 * @param gameMngr Pointer to the Game Manager instance.
 */
void PacmanStuckState::OnExit(GameMngr* gameMngr)
{
    // No actions are executed when exiting the Pacman Stuck State as the game is over.
}

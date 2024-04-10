/**
 * @file PacmanWonState.h
 * @brief Implementation file for the Pacman Won State class.
 */

#include "PacmanWonState.h"

 /**
  * @brief Executes actions when entering the Pacman Won State.
  * @param gameMngr Pointer to the Game Manager instance.
  */
void PacmanWonState::OnEnter(GameMngr* gameMngr)
{
    gameMngr->setRunPackman(false);
}

/**
 * @brief Transition function for the Pacman Won State.
 * @details No transitions are made from the Pacman Won State as the game has been won.
 * @param gameMngr Pointer to the Game Manager instance.
 * @param anotherState Pointer to the new state to transition to.
 */
void PacmanWonState::Transition(GameMngr* gameMngr, State* anotherState)
{
    // No transitions are made from the Pacman Won State as the game has been won.
}

/**
 * @brief Executes actions when exiting the Pacman Won State.
 * @details No actions are executed when exiting the Pacman Won State as the game has been won.
 * @param gameMngr Pointer to the Game Manager instance.
 */
void PacmanWonState::OnExit(GameMngr* gameMngr)
{
    // No actions are executed when exiting the Pacman Won State as the game has been won.
}

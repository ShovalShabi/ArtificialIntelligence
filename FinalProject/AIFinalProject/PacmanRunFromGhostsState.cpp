/**
 * @file PacmanRunFromGhostsState.h
 * @brief Implementation file for the Pacman Run From Ghosts State class.
 */

#include "PacmanRunFromGhostsState.h"
#include "PacmanStuckState.h"
#include "PacmanCollectCoinsState.h"

 /**
  * @brief Executes actions when entering the Pacman Run From Ghosts State.
  * @param gameMngr Pointer to the Game Manager instance.
  */
void PacmanRunFromGhostsState::OnEnter(GameMngr* gameMngr)
{
    gameMngr->setPackmanNeedToCollectCoins(false);
    gameMngr->setPackmanRunFromGhosts(true);
}

/**
 * @brief Transitions to another state based on the current state and the provided state.
 * @param gameMngr Pointer to the Game Manager instance.
 * @param anotherState Pointer to the new state to transition to.
 */
void PacmanRunFromGhostsState::Transition(GameMngr* gameMngr, State* anotherState)
{
    OnExit(gameMngr);
    if (dynamic_cast<PacmanCollectCoinsState*>(anotherState))
    {
        gameMngr->setCurrentState(new PacmanCollectCoinsState());
    }
    else if (dynamic_cast<PacmanRunFromGhostsState*>(anotherState)) // this state can be circular while the minimal proximity distance is still in range of PROXIMITY macro distance
    {
        gameMngr->setCurrentState(new PacmanRunFromGhostsState());
    }
    else
        gameMngr->setCurrentState(new PacmanStuckState());

    gameMngr->getCurrentState()->OnEnter(gameMngr);
}

/**
 * @brief Executes actions when exiting the Pacman Run From Ghosts State.
 * @param gameMngr Pointer to the Game Manager instance.
 */
void PacmanRunFromGhostsState::OnExit(GameMngr* gameMngr)
{
    gameMngr->setPackmanNeedToCollectCoins(true);
    gameMngr->setPackmanRunFromGhosts(false);
}

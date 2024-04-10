/**
 * @file PacmanCollectCoinsState.h
 * @brief Implementation file for the Pacman Collect Coins State class.
 */

#include "PacmanCollectCoinsState.h"
#include "PacmanRunFromGhostsState.h"
#include "PacmanStuckState.h"
#include "PacmanWonState.h"

 /**
  * @brief Executes actions when entering the Pacman Collect Coins State.
  * @param gameMngr Pointer to the Game Manager instance.
  */
void PacmanCollectCoinsState::OnEnter(GameMngr* gameMngr)
{
    gameMngr->setPackmanNeedToCollectCoins(true);
    gameMngr->setPackmanRunFromGhosts(false); // The only state that enters to this state
}

/**
 * @brief Transitions to another state based on the current state and the provided state.
 * @param gameMngr Pointer to the Game Manager instance.
 * @param anotherState Pointer to the new state to transition to.
 */
void PacmanCollectCoinsState::Transition(GameMngr* gameMngr, State* anotherState)
{
    if (dynamic_cast<PacmanRunFromGhostsState*>(anotherState))
    {
        gameMngr->setCurrentState(new PacmanRunFromGhostsState());
    }
    else if (dynamic_cast<PacmanStuckState*>(anotherState))
    {
        gameMngr->setCurrentState(new PacmanStuckState());
    }
    else if (dynamic_cast<PacmanCollectCoinsState*>(anotherState)) // this state can be circular while there are more coins to collect and the minimal proximity distance is no longer than PROXIMITY macro
    {
        gameMngr->setCurrentState(new PacmanCollectCoinsState());
    }
    else
        gameMngr->setCurrentState(new PacmanWonState());

    gameMngr->getCurrentState()->OnEnter(gameMngr);
}

/**
 * @brief Executes actions when exiting the Pacman Collect Coins State.
 * @param gameMngr Pointer to the Game Manager instance.
 */
void PacmanCollectCoinsState::OnExit(GameMngr* gameMngr)
{
    gameMngr->setPackmanNeedToCollectCoins(false);
}

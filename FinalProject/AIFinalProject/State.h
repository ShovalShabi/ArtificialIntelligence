#pragma once
#include "GameMngr.h"

class GameMngr;

/**
 * @brief Pure abstract class representing a state.
 */
class State
{
public:
    /**
     * @brief Handles the actions to be performed upon entering the state.
     * @param gameMngr Pointer to the game manager.
     */
    virtual void OnEnter(GameMngr* gameMngr) = 0;

    /**
     * @brief Handles the transition to another state.
     * @param gameMngr Pointer to the game manager.
     * @param anotherState Pointer to the state to transition to.
     */
    virtual void Transition(GameMngr* gameMngr, State* anotherState) = 0;

    /**
     * @brief Handles the actions to be performed upon exiting the state.
     * @param gameMngr Pointer to the game manager.
     */
    virtual void OnExit(GameMngr* gameMngr) = 0;
};

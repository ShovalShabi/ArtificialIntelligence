#pragma once

#include "State.h"

/**
 * @brief Class representing the state where Pacman is stuck.
 */
class PacmanStuckState : public State
{
public:
    /**
     * @brief Handles the actions to be performed upon entering the state.
     * @param gameMngr Pointer to the game manager.
     */
    void OnEnter(GameMngr* gameMngr) override;

    /**
     * @brief Handles the transition to another state.
     * @param gameMngr Pointer to the game manager.
     * @param anotherState Pointer to the state to transition to.
     */
    void Transition(GameMngr* gameMngr, State* anotherState) override;

    /**
     * @brief Handles the actions to be performed upon exiting the state.
     * @param gameMngr Pointer to the game manager.
     */
    void OnExit(GameMngr* gameMngr) override;
};

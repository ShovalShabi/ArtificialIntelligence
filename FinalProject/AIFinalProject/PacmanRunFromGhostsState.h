#pragma once
#include "State.h"

class PacmanRunFromGhostsState: public State
{
	void OnEnter(GameMngr* gameMngr);
	void Transition(GameMngr* gameMngr, State* anotherState);
	void OnExit(GameMngr* gameMngr);
};


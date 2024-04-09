#pragma once
#include "State.h"

class PacmanWonState: public State
{
	void OnEnter(GameMngr* gameMngr);
	void Transition(GameMngr* gameMngr, State* anotherState);
	void OnExit(GameMngr* gameMngr);
};


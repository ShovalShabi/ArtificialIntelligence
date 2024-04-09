#pragma once
#include "GameMngr.h"

class GameMngr;
// pure abstract class (in Java it is interface)
class State
{
public:
	virtual void OnEnter(GameMngr* gameMngr) = 0;
	virtual void Transition(GameMngr* gameMngr, State* anotherState) = 0; // to another state
	virtual void OnExit(GameMngr* gameMngr) = 0;
};
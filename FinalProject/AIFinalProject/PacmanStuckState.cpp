#include "PacmanStuckState.h"

void PacmanStuckState::OnEnter(GameMngr* gameMngr)
{
	gameMngr->setPackmanNeedToCollectCoins(false);
	gameMngr->setRunPackman(false);// Pacman has lost the game
}

void PacmanStuckState::Transition(GameMngr* gameMngr, State* anotherState)
{
	//Non, pacman has lost the game
}

void PacmanStuckState::OnExit(GameMngr* gameMngr)
{
	//Non, pacman has lost the game
}

#include "PacmanRunFromGhostsState.h"
#include "PacmanStuckState.h"
#include "PacmanCollectCoinsState.h"

void PacmanRunFromGhostsState::OnEnter(GameMngr* gameMngr)
{
	gameMngr->setPackmanNeedToCollectCoins(false);
	gameMngr->setPackmanRunFromGhosts(true);
}

void PacmanRunFromGhostsState::Transition(GameMngr* gameMngr, State* anotherState)
{
	OnExit(gameMngr);
	if (dynamic_cast<PacmanCollectCoinsState*>(anotherState))
	{
		gameMngr->setCurrentState(new PacmanCollectCoinsState());
	}
	else
	{
		gameMngr->setCurrentState(new PacmanStuckState());
	}
	gameMngr->getCurrentState()->OnEnter(gameMngr);
}

void PacmanRunFromGhostsState::OnExit(GameMngr* gameMngr)
{
	gameMngr->setPackmanNeedToCollectCoins(true);
	gameMngr->setPackmanRunFromGhosts(false);
}

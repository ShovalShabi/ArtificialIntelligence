#include "PacmanCollectCoinsState.h"
#include "PacmanRunFromGhostsState.h"
#include "PacmanStuckState.h"
#include "PacmanWonState.h"


void PacmanCollectCoinsState::OnEnter(GameMngr* gameMngr)
{

	gameMngr->setPackmanNeedToCollectCoins(true);
	gameMngr->setPackmanRunFromGhosts(false); // The only state that enters to this state
}

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
	else if (dynamic_cast<PacmanCollectCoinsState*>(anotherState))// this state can be circular while there are more coins to collect and the minimal proximity distance is no longer than PROXIMITY macro
	{
		gameMngr->setCurrentState(new PacmanCollectCoinsState());
	}
	else
	{
		gameMngr->setCurrentState(new PacmanWonState());
	}
	gameMngr->getCurrentState()->OnEnter(gameMngr);

}

void PacmanCollectCoinsState::OnExit(GameMngr* gameMngr)
{
	gameMngr->setPackmanNeedToCollectCoins(false);
}

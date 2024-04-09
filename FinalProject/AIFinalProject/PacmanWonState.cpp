#include "PacmanWonState.h"

void PacmanWonState::OnEnter(GameMngr* gameMngr)
{
	gameMngr->setRunPackman(false);
}

void PacmanWonState::Transition(GameMngr* gameMngr, State* anotherState)
{
	//Nothing Pacman won the game
}

void PacmanWonState::OnExit(GameMngr* gameMngr)
{
	//Nothing Pacman won the game
}

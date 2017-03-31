#include "GameModeState.h"
#include "GameModeStateMachine.h"


GameModeState::GameModeState()
{
}

GameModeState::GameModeState(Application2D * application)
{
	setApplication(application);
}


GameModeState::~GameModeState()
{
}

void GameModeState::init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
}

void GameModeState::add(GameModeStateMachine * stateMachine)
{
	m_stateMachine = stateMachine;
}

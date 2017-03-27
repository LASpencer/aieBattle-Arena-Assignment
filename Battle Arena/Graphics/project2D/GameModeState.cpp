#include "GameModeState.h"
#include "GameModeStateMachine.h"


GameModeState::GameModeState()
{
}

GameModeState::GameModeState(Application2D * application)
{
	SetApplication(application);
}


GameModeState::~GameModeState()
{
}

void GameModeState::Init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
}

void GameModeState::Add(GameModeStateMachine * stateMachine)
{
	m_stateMachine = stateMachine;
}

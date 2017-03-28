#include "GameModeStateMachine.h"
#include "GameModeState.h"


GameModeStateMachine::GameModeStateMachine()
{
}


GameModeStateMachine::~GameModeStateMachine()
{
}

void GameModeStateMachine::Change(std::string name, CreatureArray * playerTeam, CreatureArray * enemyTeam = nullptr)
{
	if (m_currentState != nullptr) {
		m_currentState->Exit();
	}
	m_currentState = m_states[name];
	dynamic_cast<GameModeState*>(m_currentState)->Init(playerTeam, enemyTeam); //HACK test this carefully
}

void GameModeStateMachine::Add(std::string name, State * state)
{
	if (dynamic_cast<GameModeState*>(state) == nullptr) {
		throw std::invalid_argument("GameModeStatMachine states must be of GameModeState class");
	} else{
		StateMachine::Add(name, state);
	}
}

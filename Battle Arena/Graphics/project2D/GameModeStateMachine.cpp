#include "GameModeStateMachine.h"
#include "GameModeState.h"


GameModeStateMachine::GameModeStateMachine()
{
}


GameModeStateMachine::~GameModeStateMachine()
{
}

void GameModeStateMachine::change(std::string name, CreatureArray * playerTeam, CreatureArray * enemyTeam = nullptr)
{
	if (m_currentState != nullptr) {
		m_currentState->exit();
	}
	m_currentState = m_states[name];
	dynamic_cast<GameModeState*>(m_currentState)->init(playerTeam, enemyTeam); //HACK test this carefully
}

void GameModeStateMachine::add(std::string name, State * state)
{
	if (dynamic_cast<GameModeState*>(state) == nullptr) {
		throw std::invalid_argument("GameModeStatMachine states must be of GameModeState class");
	} else{
		StateMachine::add(name, state);
	}
}

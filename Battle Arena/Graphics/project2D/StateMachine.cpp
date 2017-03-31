#include "StateMachine.h"
#include "State.h"


StateMachine::StateMachine()
{
	m_currentState = nullptr;
}


StateMachine::~StateMachine()
{
	// Delete all states
	for (std::map<std::string, State*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		delete it->second;
	}
}

void StateMachine::change(std::string stateName)
{
	if (m_currentState != nullptr) {
		m_currentState->exit();
	}
	m_currentState = m_states[stateName];
	m_currentState->init();
}

void StateMachine::add(std::string name, State * state)
{
	m_states[name] = state;
	state->add(this);
}

State* StateMachine::GetState() {
	return m_currentState;
}


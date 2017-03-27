#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::Add(StateMachine * stateMachine)
{
	m_stateMachine = stateMachine;
}

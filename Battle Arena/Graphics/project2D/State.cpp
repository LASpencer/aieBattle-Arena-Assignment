#include "State.h"



State::State()
{
}


State::~State()
{
}

void State::add(StateMachine * stateMachine)
{
	m_stateMachine = stateMachine;
}

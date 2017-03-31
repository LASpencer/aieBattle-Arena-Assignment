#pragma once
# include <map>
# include <string>

class State;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void change(std::string stateName);
	/* Adds new State to m_states map
		name = key of new state
		state = pointer to new state
	*/
	virtual void add(std::string name, State* state);

	State * GetState();

protected:
	std::map<std::string, State*> m_states;
	State* m_currentState;
};


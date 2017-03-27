#pragma once
# include <map>
# include <string>

class State;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void Change(std::string stateName);
	virtual void Add(std::string name, State* state);

	State * GetState();

protected:
	std::map<std::string, State*> m_states;
	State* m_currentState;
};


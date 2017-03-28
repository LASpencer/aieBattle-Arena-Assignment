#pragma once
#include "StateMachine.h"

struct CreatureArray;

class GameModeStateMachine :
	public StateMachine
{
public:
	GameModeStateMachine();
	~GameModeStateMachine();

	void Change(std::string stateName, CreatureArray* playerTeam , CreatureArray* enemyTeam);
	// TODO override Add function to check states are GameModeState
	/* Adds new State to map, if State cannot be cast as GameModeState throws argument exception
		name = new state key
		state = pointer to new state
	*/
	void Add(std::string name, State* state);
};


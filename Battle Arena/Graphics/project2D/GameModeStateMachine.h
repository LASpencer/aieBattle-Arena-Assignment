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
};


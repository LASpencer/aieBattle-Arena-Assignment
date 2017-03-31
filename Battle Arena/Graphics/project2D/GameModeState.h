#pragma once
#include "State.h"

class GameModeStateMachine;
class Application2D;
struct CreatureArray;

/*
GameModeState
Represents the current state of the application
*/
class GameModeState :
	public State
{
public:
	GameModeState();
	GameModeState(Application2D* application);
	~GameModeState();

	// Initializes newly entered state
	virtual void init(CreatureArray* playerTeam, CreatureArray* enemyTeam = nullptr);

	// Sets a pointer to the GameModeStateMachine that contains this GameModeState
	void add(GameModeStateMachine* stateMachine);

	// Sets a pointer to the Application object
	void setApplication(Application2D* application) {
		m_application = application;
	}

protected:
	Application2D* m_application;
	CreatureArray* m_playerTeam;
	CreatureArray* m_enemyTeam;
};


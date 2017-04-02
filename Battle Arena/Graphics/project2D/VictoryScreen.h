#pragma once
# include "GameModeState.h"

class MessageBar;
/*
VictoryScreen state
Display victory message after player wins the game
Final state, waits for player to hit Esc and end program
*/
class VictoryScreen :
	 public GameModeState
{
public:
	VictoryScreen();
	VictoryScreen(Application2D* application);
	~VictoryScreen();

	virtual void init();
	void init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void exit();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);

protected:
	MessageBar* m_winMessage;
};


#pragma once
# include "GameModeState.h"

class MessageBar;
/*
VictoryScreen state
Display victory message after player wins the game
Final state
*/
class VictoryScreen :
	 public GameModeState
{
public:
	VictoryScreen();
	VictoryScreen(Application2D* application);
	~VictoryScreen();

	virtual void Init();
	void Init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void Exit();
	virtual void Update(float deltaTime);
	virtual void Draw(aie::Renderer2D &renderer);

protected:
	MessageBar* m_winMessage;
};


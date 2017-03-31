#pragma once
#include "GameModeState.h"

class MessageBar;
/*
GameOver state
Display game over message after player loses the game
Final state
*/
class GameOver :
	public GameModeState
{
public:
	GameOver();
	GameOver(Application2D* application);
	~GameOver();

	virtual void init();
	void init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void exit();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);

protected:
	MessageBar* m_gameOverMsg;
};


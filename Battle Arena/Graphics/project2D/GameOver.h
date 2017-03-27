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

	virtual void Init();
	void Init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void Exit();
	virtual void Update(float deltaTime);
	virtual void Draw(aie::Renderer2D &renderer);

protected:
	MessageBar* m_gameOverMsg;
};


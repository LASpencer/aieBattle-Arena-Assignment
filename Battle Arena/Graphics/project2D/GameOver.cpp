#include "GameOver.h"
#include "MessageBar.h"

//TODO actual Game OVer screen

GameOver::GameOver()
{
}

GameOver::GameOver(Application2D * application)
{
	m_application = application;
	m_gameOverMsg = new MessageBar("Game Over", 640, 650);
}


GameOver::~GameOver()
{
	delete m_gameOverMsg;
}

void GameOver::Init()
{
}

void GameOver::Init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
	Init();
}

void GameOver::Exit()
{
}

void GameOver::Update(float deltaTime)
{
}

void GameOver::Draw(aie::Renderer2D & renderer)
{
	m_gameOverMsg->Draw(renderer);
}

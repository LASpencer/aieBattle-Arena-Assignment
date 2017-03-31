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

void GameOver::init()
{
}

void GameOver::init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
	init();
}

void GameOver::exit()
{
}

void GameOver::update(float deltaTime)
{
}

void GameOver::draw(aie::Renderer2D & renderer)
{
	m_gameOverMsg->draw(renderer);
}

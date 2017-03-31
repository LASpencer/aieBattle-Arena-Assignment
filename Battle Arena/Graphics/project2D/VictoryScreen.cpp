#include "VictoryScreen.h"
#include "MessageBar.h"

//TODO actual victory screen

VictoryScreen::VictoryScreen()
{
}

VictoryScreen::VictoryScreen(Application2D * application)
{
	m_application = application;
	m_winMessage = new MessageBar("Victory!", 640.0f, 650.0f);
}


VictoryScreen::~VictoryScreen()
{
	delete m_winMessage;
}

void VictoryScreen::init()
{
}

void VictoryScreen::init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	init();
}

void VictoryScreen::exit()
{
	
}

void VictoryScreen::update(float deltaTime)
{
}

void VictoryScreen::draw(aie::Renderer2D & renderer)
{
	m_winMessage->draw(renderer);
}

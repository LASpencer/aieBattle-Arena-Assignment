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

void VictoryScreen::Init()
{
}

void VictoryScreen::Init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	Init();
}

void VictoryScreen::Exit()
{
	
}

void VictoryScreen::Update(float deltaTime)
{
}

void VictoryScreen::Draw(aie::Renderer2D & renderer)
{
	m_winMessage->Draw(renderer);
}

#include "MainMenu.h"
#include "MessageBar.h"
#include "Input.h"
#include "GameModeStateMachine.h"
#include "Application2D.h"

// TODO actual menu, let the user pick their heroes, other options, then play

MainMenu::MainMenu()
{
}

MainMenu::MainMenu(Application2D * application)
{
	m_application = application;
}


MainMenu::~MainMenu()
{
}

void MainMenu::init()
{
	m_titleBar = new MessageBar("Battle Arena", 640, 650);
	m_menuOptions.clear();
	m_menuOptions.push_back(MainMenuOption("Play", Option::PLAY));
	m_menuOptions.push_back(MainMenuOption("Quit", Option::QUIT));
	m_mainMenuBox = new MainMenuOptionMenuBox(m_menuOptions,600,550);
}

void MainMenu::init(CreatureArray * playerTeam, CreatureArray * enemyTeam)
{
	m_playerTeam = playerTeam;
	m_enemyTeam = enemyTeam;
	init();
}

void MainMenu::exit()
{
	m_menuOptions.clear();
	delete m_mainMenuBox;
	delete m_titleBar;
}

void MainMenu::update(float deltaTime)
{
	// Get user input
	aie::Input* input = aie::Input::getInstance();
	m_mainMenuBox->update(deltaTime);
	// If spacebar was pressed, perform action selected
	if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
		switch (m_mainMenuBox->getCurrent().GetOption()) {
		case Option::PLAY:
			dynamic_cast<GameModeStateMachine*>(m_stateMachine)->change("Battle", m_playerTeam, m_enemyTeam);
			break;
		case Option::QUIT:
			m_application->quit();
			break;
		}
	}
}

void MainMenu::draw(aie::Renderer2D & renderer)
{
	m_titleBar->draw(renderer);
	m_mainMenuBox->draw(renderer);
}

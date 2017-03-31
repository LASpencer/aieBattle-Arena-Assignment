#pragma once
#include <vector>
#include <string>
#include "GameModeState.h"
#include "MenuBox.h"
#include "MainMenuOptionMenuBox.h"


class MessageBar;
class MainMenuOption;
/*
MainMenu state
Displays main menu
Exits to Battle on user input
*/
class MainMenu :
	public GameModeState
{
public:
	MainMenu();
	MainMenu(Application2D* application);
	~MainMenu();

	virtual void init();
	void init(CreatureArray * playerTeam, CreatureArray * enemyTeam);
	virtual void exit();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);

	enum Option {
		NONE,
		PLAY,
		QUIT
	};

protected:
	MessageBar* m_titleBar;
	std::vector<MainMenuOption> m_menuOptions;
	MainMenuOptionMenuBox* m_mainMenuBox;
};

class MainMenuOption {
public:
	MainMenuOption() :m_name(""), m_option(MainMenu::Option::NONE)
	{
	}
	MainMenuOption(std::string name, MainMenu::Option option) : m_name(name), m_option(option)
	{
	}
	std::string GetName() {
		return m_name;
	}
	MainMenu::Option GetOption() {
		return m_option;
	}

	void SetName(std::string name) {
		m_name = name;
	}
	void SetOption(MainMenu::Option option) {
		m_option = option;
	}

private:
	std::string m_name;
	MainMenu::Option m_option; 
};


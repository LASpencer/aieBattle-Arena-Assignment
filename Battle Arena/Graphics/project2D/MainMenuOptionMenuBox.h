#pragma once
#include "MenuBox.h"

class MainMenuOption;

class MainMenuOptionMenuBox :
	public MenuBox
{
public:
	MainMenuOptionMenuBox();
	MainMenuOptionMenuBox(std::vector<MainMenuOption> &contents, float xPos = 0, float yPos = 0, unsigned int boxColour = 0x202020FF, unsigned int textColour = 0xFFFFFFFF, unsigned int selectColour = 0xAAAA22FF);
	MainMenuOptionMenuBox(const MainMenuOptionMenuBox &m);
	~MainMenuOptionMenuBox();

	void Update(float deltaTime);
	void Draw(aie::Renderer2D &renderer);

	void SetContents(std::vector<MainMenuOption> &contents);

	MainMenuOption GetCurrent();

private:
	typedef typename std::vector<MainMenuOption>::iterator contentIterator;

	// Menu contents
	std::vector<MainMenuOption>	m_contents;
	contentIterator m_current;
};


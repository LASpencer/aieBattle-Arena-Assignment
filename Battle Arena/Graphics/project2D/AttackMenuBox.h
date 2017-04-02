#pragma once
#include "MenuBox.h"

class Attack;

/*AttackMenuBox class
MenuBox where each option is a pointer to an attack
*/
class AttackMenuBox :
	public MenuBox
{
public:
	AttackMenuBox();
	AttackMenuBox(std::vector<Attack*> &contents, float xPos = 0, float yPos = 0, unsigned int boxColour = 0x202020FF, unsigned int textColour = 0xFFFFFFFF, unsigned int selectColour = 0xAAAA22FF);
	AttackMenuBox(const AttackMenuBox &m);
	~AttackMenuBox();

	void update(float deltaTime);
	void draw(aie::Renderer2D &renderer);

	void setContents(std::vector<Attack*> &contents);

	// Get attack currently selected by user
	Attack* getCurrent();

private:
	typedef typename std::vector<Attack*>::iterator contentIterator;

	// Menu contents
	std::vector<Attack*>	m_contents;
	contentIterator m_current;
};


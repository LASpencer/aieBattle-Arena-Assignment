#pragma once
#include "Agent.h"
#include "AttackMenuBox.h"
#include <vector>

class AgentHuman :
	public Agent
{
public:
	AgentHuman();
	AgentHuman(aie::Texture* targetArrow);
	~AgentHuman();

	virtual void startTurn(CreatureArray* friends, CreatureArray* enemies, size_t position);
	/*Get player input and set chosenAttack and target based on their selection*/
	virtual void update(float deltaTime);
	/*Set sprite for arrow indicating current target
	targetArrow = texture pointer to be used for sprite
	*/
	void setTargetArrowSprite(aie::Texture* targetArrow);
	/*Draw attack menu and arrow indicating selected target*/
	virtual void draw(aie::Renderer2D &renderer);

	static const float ATTACK_MENU_XPOS;
	static const float ATTACK_MENU_YPOS;

protected:
	// Put possible targets of attack in targetChoices
	virtual void setTargetChoices();

	// Increment m_targetIter unless it hits end of vector
	void incrementTargetChoice();

	// Decrement m_targetIter unless it passes beginning of vector
	void decrementTargetChoice();

	AttackMenuBox m_attackMenu;
	aie::Texture* m_targetArrow;
	std::vector<size_t> m_targetChoices;
	std::vector<size_t>::iterator m_targetIter;
	bool m_pickingTarget;
};


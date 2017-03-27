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

	virtual void StartTurn(CreatureArray* friends, CreatureArray* enemies, size_t position);
	virtual void Update(float deltaTime);
	void SetTargetArrowSprite(aie::Texture* targetArrow);
	virtual void Draw(aie::Renderer2D &renderer);

	static const float ATTACK_MENU_XPOS;
	static const float ATTACK_MENU_YPOS;

protected:
	virtual void SetTargetChoices();

	// Increment m_targetIter unless it hits end of vector
	void IncrementTargetChoice();

	// Decrement m_targetIter unless it passes beginning of vector
	void DecrementTargetChoice();

	AttackMenuBox m_attackMenu;
	aie::Texture* m_targetArrow;
	std::vector<size_t> m_targetChoices;
	std::vector<size_t>::iterator m_targetIter;
	bool m_pickingTarget;
};


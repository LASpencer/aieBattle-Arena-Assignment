#pragma once
#include "Agent.h"

class Behaviour;

class AgentAI :
	public Agent
{
public:
	AgentAI();
	AgentAI(Behaviour* behaviour);
	~AgentAI();

	virtual void StartTurn(CreatureArray * friends, CreatureArray * enemies, size_t position);

	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);

	void setBehaviour(Behaviour* behaviour);

protected:
	Behaviour* m_behaviour;

};


#pragma once
#include "Agent.h"

class Behaviour;

/*AgentAI class

Selects creature's attack and target based on set Behaviour
*/
class AgentAI :
	public Agent
{
public:
	AgentAI();
	AgentAI(Behaviour* behaviour);
	~AgentAI();

	virtual void startTurn(CreatureArray * friends, CreatureArray * enemies, size_t position);

	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D &renderer);

	void setBehaviour(Behaviour* behaviour);

protected:
	Behaviour* m_behaviour;

};


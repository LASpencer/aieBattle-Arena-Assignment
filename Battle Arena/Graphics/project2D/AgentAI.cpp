#include "AgentAI.h"
#include "Attack.h"
#include "Creature.h"
#include "Behaviour.h"

AgentAI::AgentAI()
{
}

AgentAI::AgentAI(Behaviour * behaviour) : m_behaviour(behaviour)
{
}


AgentAI::~AgentAI()
{
}

void AgentAI::StartTurn(CreatureArray * friends, CreatureArray * enemies, size_t position) {
	Agent::StartTurn(friends, enemies, position);
	m_behaviour->SetCreature(m_creature);
	m_behaviour->SetFriends(friends);
	m_behaviour->SetEnemies(enemies);
	m_behaviour->SetPosition(position);
	m_behaviour->SetPossibleAttacks(&m_possibleAttacks);
}

void AgentAI::update(float deltaTime)
{
	// Get chosen attack from current behaviour
	m_chosenAttack = m_behaviour->SelectAttack();
	// Get target from current behaviour
	m_target = m_behaviour->SelectTarget();
	m_decided = true;
}

void AgentAI::draw(aie::Renderer2D & renderer)
{
}

void AgentAI::setBehaviour(Behaviour * behaviour)
{
	m_behaviour = behaviour;
}

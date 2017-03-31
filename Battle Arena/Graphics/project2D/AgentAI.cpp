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

void AgentAI::startTurn(CreatureArray * friends, CreatureArray * enemies, size_t position) {
	Agent::startTurn(friends, enemies, position);
	m_behaviour->setCreature(m_creature);
	m_behaviour->setFriends(friends);
	m_behaviour->setEnemies(enemies);
	m_behaviour->setPosition(position);
	m_behaviour->setPossibleAttacks(&m_possibleAttacks);
}

void AgentAI::update(float deltaTime)
{
	// Get chosen attack from current behaviour
	m_chosenAttack = m_behaviour->selectAttack();
	// Get target from current behaviour
	m_target = m_behaviour->selectTarget();
	m_decided = true;
}

void AgentAI::draw(aie::Renderer2D & renderer)
{
}

void AgentAI::setBehaviour(Behaviour * behaviour)
{
	m_behaviour = behaviour;
}
